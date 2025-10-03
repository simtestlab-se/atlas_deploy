# SOC (State of Charge) Estimation Module

## Overview
The SOC estimation module provides advanced battery state-of-charge estimation using multiple complementary methods including industry-standard Kalman filtering, traditional hybrid approaches, and power-cycle memory management.

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    SOC Estimation Module                   │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────┐ │
│  │ Kalman Filter   │  │ Hybrid CC+OCV   │  │ Persistence │ │
│  │                 │  │                 │  │             │ │
│  │ • EKF Algorithm │  │ • OCV Lookup    │  │ • EEPROM    │ │
│  │ • Sensor Fusion │  │ • Coulomb Count │  │ • Recovery  │ │
│  │ • Covariance    │  │ • Temperature   │  │ • Integrity │ │
│  │ • Health Check  │  │   Compensation  │  │   Check     │ │
│  └─────────────────┘  └─────────────────┘  └─────────────┘ │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────┐ │
│  │ Cell Config     │  │ OCV Estimation  │  │ Coulomb     │ │
│  │                 │  │                 │  │ Counting    │ │
│  │ • LiFePO4       │  │ • Voltage Rest  │  │ • Current   │ │
│  │   Parameters    │  │ • Temperature   │  │   Integral  │ │
│  │ • Capacity      │  │ • Lookup Table  │  │ • Efficiency│ │
│  │ • Thresholds    │  │ • Interpolation │  │ • Drift     │ │
│  └─────────────────┘  └─────────────────┘  └─────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

## Module Files

| File/Directory | Purpose | Algorithm Type |
|----------------|---------|----------------|
| `kalman_soc/` | Advanced Kalman filter implementation | Industry-standard EKF |
| `soc_estimation.c/h` | Main SOC API and hybrid methods | Hybrid CC+OCV |
| `soc_persistence.c/h` | Power-cycle memory management | EEPROM-based |
| `ocv_estimation/` | Open circuit voltage estimation | Voltage-based |
| `coulomb_counting/` | Current integration methods | Current-based |
| `cell_configuration/` | Battery-specific parameters | Configuration |

## Key Algorithms

### 1. Kalman Filter SOC Estimation (Primary Method)

#### Mathematical Foundation
The Extended Kalman Filter (EKF) implementation uses a 2-state model:

```
State Vector: x = [SOC, ΔCapacity]ᵀ

State Transition:
SOC(k+1) = SOC(k) - (Current × dt) / (Capacity + ΔCapacity) × 100
ΔCapacity(k+1) = ΔCapacity(k) + process_noise

Measurement Models:
- OCV Method: SOC_ocv = f(voltage, temperature)
- Coulomb Method: SOC_cc = integrated_current_based_estimate
```

#### Implementation Features
- **2-State Kalman Filter**: SOC + capacity error compensation
- **Optimal Sensor Fusion**: Combines OCV and coulomb counting optimally
- **Adaptive Covariance**: Updates uncertainty based on measurement quality
- **Temperature Compensation**: Thermal effects on capacity and OCV
- **Health Monitoring**: Real-time filter performance assessment

```c
// Key API Functions
bool InitKalmanSOC(float initial_soc, float temperature_c);
void UpdateKalmanSOC(float current_A, float voltage_V, float temperature_c, float dt_s);
float GetKalmanSOC(void);
bool IsKalmanSOCHealthy(void);
```

### 2. Hybrid CC+OCV Method (Fallback Method)

#### Algorithm Logic
```c
if (battery_at_rest && sufficient_rest_time) {
    // Use OCV-based estimation
    soc = lookup_soc_from_ocv(cell_voltage, temperature);
} else {
    // Use coulomb counting with OCV calibration
    soc = coulomb_counting_soc;
    if (ocv_available) {
        soc = weighted_average(coulomb_soc, ocv_soc, confidence_factors);
    }
}
```

#### Features
- **Automatic Method Selection**: Switches based on battery state
- **Temperature Compensation**: Adjusts both OCV and capacity for temperature
- **Drift Correction**: Periodic recalibration using OCV when available
- **Confidence Weighting**: Quality-based fusion of different estimates

### 3. SOC Persistence System

#### Power-Cycle Memory Management
```c
typedef struct {
    float soc_values[12];           // Per-cell SOC
    float pack_voltage;             // Pack voltage at save
    float temperature;              // Temperature at save
    uint32_t timestamp;             // Save timestamp
    uint32_t crc32;                 // Data integrity check
} SOCPersistenceData_t;
```

#### Features
- **EEPROM Storage**: Non-volatile memory for power-cycle continuity
- **Data Integrity**: CRC32 checksums for corruption detection
- **Smart Recovery**: OCV-based recovery if saved data is invalid
- **Automatic Saving**: Triggered by critical events or time intervals

## Performance Specifications

### Accuracy Targets

| Condition | Kalman Filter | Hybrid CC+OCV |
|-----------|---------------|---------------|
| Normal Operation (0-45°C, 0.1-1C) | ±2-3% | ±3-5% |
| Extreme Temperature (<0°C, >45°C) | ±5-8% | ±8-12% |
| High Current (>1C) | ±3-5% | ±5-10% |
| Aged Battery (>500 cycles) | ±5-8% | ±8-15% |

### Computational Performance
- **Kalman Update Time**: <2ms per iteration
- **Memory Usage**: ~2KB RAM, ~15KB Flash
- **Update Rate**: 10Hz (100ms intervals)
- **Convergence Time**: <30 seconds after initialization

### Power Consumption
- **Active Mode**: ~50µA additional current
- **EEPROM Write**: ~5mA for 10ms (during save operations)
- **Standby Impact**: <1µA additional drain

## Configuration Parameters

### Kalman Filter Tuning
```c
// Process noise (how much we trust the model)
#define SOC_PROCESS_NOISE          0.001f   // SOC drift per second
#define CAPACITY_PROCESS_NOISE     0.01f    // Capacity uncertainty

// Measurement noise (sensor accuracy)
#define OCV_MEASUREMENT_NOISE      0.25f    // OCV SOC variance (%)
#define COULOMB_MEASUREMENT_NOISE  0.1f     // Coulomb counting variance (%)

// Initialization parameters
#define KALMAN_CONVERGENCE_TIME    30.0f    // Seconds to convergence
#define INITIAL_SOC_VARIANCE       25.0f    // Initial uncertainty (%)
```

### LiFePO4 Battery Parameters
```c
// Cell configuration for 12S1P pack
#define BATTERY_NOMINAL_CAPACITY   1000.0f  // mAh
#define CELL_COUNT                 12       // Series cells
#define NOMINAL_VOLTAGE            3.3f     // Volts per cell

// Operating ranges
#define CELL_MIN_VOLTAGE           2.5f     // Volts
#define CELL_MAX_VOLTAGE           3.65f    // Volts
#define OPERATING_TEMP_MIN         -10.0f   // Celsius
#define OPERATING_TEMP_MAX         60.0f    // Celsius
```

### SOC Persistence Settings
```c
#define SOC_SAVE_INTERVAL_MS       5000     // Auto-save interval
#define SOC_EEPROM_START_ADDR      0x1000   // EEPROM address
#define SOC_DATA_RETENTION_DAYS    30       // Data validity period
#define MAX_SOC_RESTORE_AGE_MS     86400000 // 24 hours max age
```

## API Reference

### Primary SOC Interface
```c
// Initialize SOC estimation system
bool SOC_Initialize(float initial_soc_percentage);

// Update SOC with new measurements
void SOC_Update(float current_A, float voltage_V, float temperature_C, float dt_seconds);

// Get current SOC estimate
float SOC_GetEstimate(uint8_t cell_index);
float SOC_GetPackSOC(void);

// Get estimation method being used
SOC_Method_t SOC_GetActiveMethod(void);  // KALMAN or HYBRID

// Health and diagnostics
bool SOC_IsHealthy(void);
float SOC_GetAccuracyEstimate(void);
SOC_DiagnosticInfo_t SOC_GetDiagnostics(void);
```

### Kalman Filter Specific API
```c
// Kalman filter control
bool KalmanSOC_Initialize(const KalmanSOC_Config_t* config);
void KalmanSOC_Reset(float new_initial_soc);
void KalmanSOC_Predict(float current_A, float dt_seconds);
void KalmanSOC_UpdateOCV(float ocv_soc, float confidence);
void KalmanSOC_UpdateCoulomb(float coulomb_soc, float confidence);

// State access
float KalmanSOC_GetSOC(void);
float KalmanSOC_GetCapacityError(void);
float KalmanSOC_GetUncertainty(void);
bool KalmanSOC_IsConverged(void);
```

### Persistence API
```c
// Save/restore operations
bool SOC_SaveStateToPersistentMemory(const float* cell_voltages, 
                                    float temperature, float current);
bool SOC_RestoreStateFromPersistentMemory(float* restored_soc);
bool SOC_ClearPersistentState(void);

// Data validation
bool SOC_ValidatePersistentData(void);
uint32_t SOC_GetPersistentDataAge(void);
```

## Calibration and Tuning

### Initial Calibration Process
1. **Full Charge Calibration**
   ```c
   // Perform full charge to 100% SOC
   // Allow battery to rest for 2+ hours
   // Record OCV at various SOC points during discharge
   // Update OCV lookup table
   ```

2. **Capacity Calibration**
   ```c
   // Perform controlled discharge from 100% to 0%
   // Measure actual capacity delivered
   // Update nominal capacity parameter
   // Verify temperature compensation factors
   ```

3. **Kalman Filter Tuning**
   ```c
   // Collect measurement data over various conditions
   // Analyze SOC estimation errors
   // Tune process and measurement noise parameters
   // Validate convergence behavior
   ```

### Runtime Adaptation
- **Automatic OCV Table Learning**: Updates lookup table based on operational data
- **Capacity Fade Tracking**: Monitors capacity degradation over time
- **Temperature Compensation Updates**: Adapts to thermal behavior changes
- **Measurement Quality Assessment**: Dynamically adjusts confidence factors

## Testing and Validation

### Test Coverage
- **Unit Tests**: Individual algorithm component testing
- **Integration Tests**: Multi-algorithm interaction testing
- **Accuracy Tests**: SOC estimation error validation
- **Stress Tests**: Extreme condition operation
- **Regression Tests**: Algorithm performance over time

### Validation Methods
```c
// Accuracy validation against reference SOC
float reference_soc = perform_reference_measurement();
float estimated_soc = SOC_GetEstimate(cell_index);
float error = fabs(estimated_soc - reference_soc);
assert(error < ACCURACY_THRESHOLD);

// Kalman filter convergence validation
bool converged = KalmanSOC_IsConverged();
float uncertainty = KalmanSOC_GetUncertainty();
assert(converged && uncertainty < MAX_UNCERTAINTY);
```

## Troubleshooting Guide

### Common Issues

#### Poor SOC Accuracy
**Symptoms**: SOC estimates drift from actual values
**Causes**: 
- Incorrect capacity settings
- Poor OCV lookup table
- Insufficient rest time for OCV measurements
- Temperature compensation errors

**Solutions**:
```c
// Re-calibrate capacity
SOC_CalibrateCapacity(measured_capacity_mAh);

// Update OCV table
SOC_UpdateOCVLookupTable(voltage_soc_pairs, num_points);

// Verify temperature sensors
if (temperature_sensor_error) {
    SOC_DisableTemperatureCompensation();
}
```

#### Kalman Filter Divergence
**Symptoms**: Kalman filter health degrades, switches to hybrid mode
**Causes**:
- Incorrect noise parameters
- Sensor measurement errors
- Model mismatch

**Solutions**:
```c
// Reset filter with current best estimate
float current_best_soc = get_ocv_based_soc();
KalmanSOC_Reset(current_best_soc);

// Tune noise parameters
KalmanSOC_SetProcessNoise(new_process_noise);
KalmanSOC_SetMeasurementNoise(new_measurement_noise);
```

#### SOC Persistence Failures
**Symptoms**: SOC resets to default after power cycle
**Causes**:
- EEPROM hardware failure
- Data corruption
- Address conflicts

**Solutions**:
```c
// Test EEPROM functionality
bool eeprom_ok = SOC_TestEEPROM();
if (!eeprom_ok) {
    // Use alternative storage or disable persistence
    SOC_DisablePersistence();
}

// Clear corrupted data
SOC_ClearPersistentState();
```

## Advanced Features

### Machine Learning Integration (Future)
```c
// Neural network SOC enhancement
typedef struct {
    float input_features[10];      // Voltage, current, temperature history
    float hidden_weights[50];      // Trained neural network weights
    float output_correction;       // SOC correction factor
} ML_SOC_Enhancer_t;
```

### Predictive Maintenance
```c
// Battery health trending
typedef struct {
    float capacity_fade_rate;      // % per cycle
    float resistance_increase;     // Ohms per cycle
    uint32_t cycles_to_eol;       // Estimated cycles to end-of-life
    float soc_accuracy_trend;     // Accuracy degradation over time
} PredictiveMaintenance_t;
```

### Cloud Analytics Interface
```c
// Data export for cloud analysis
typedef struct {
    float soc_history[1000];      // Historical SOC data
    float accuracy_metrics[100];   // Accuracy measurements
    BatteryCondition_t condition; // Current battery health
    CalibrationData_t cal_data;   // Calibration parameters
} CloudAnalyticsData_t;
```

## Performance Optimization

### Memory Optimization
- **Static Allocation**: Pre-allocated buffers for deterministic memory usage
- **Compressed Data**: Efficient storage of lookup tables and calibration data
- **Circular Buffers**: Ring buffers for historical data storage

### Computational Optimization
- **Fixed-Point Math**: Optional fixed-point implementation for faster computation
- **Lookup Table Optimization**: Efficient interpolation algorithms
- **Vectorized Operations**: SIMD instructions where available

### Power Optimization
- **Sleep Modes**: Reduced-power operation during idle periods
- **Selective Updates**: Update only necessary components based on battery state
- **Efficient EEPROM Usage**: Minimize write operations to extend EEPROM life

---

## Quick Start Guide

```c
// 1. Initialize SOC system
SOC_Initialize(50.0f);  // Start with 50% SOC estimate

// 2. Main loop integration (10Hz)
while(1) {
    // Read sensors
    float current = read_pack_current();
    float voltage = read_cell_voltage(cell_index);
    float temperature = read_temperature();
    
    // Update SOC estimate
    SOC_Update(current, voltage, temperature, 0.1f);  // 100ms interval
    
    // Get results
    float soc = SOC_GetEstimate(cell_index);
    bool healthy = SOC_IsHealthy();
    
    vTaskDelay(pdMS_TO_TICKS(100));  // 100ms delay
}
```

For implementation details of specific sub-algorithms, refer to the README files in each subdirectory:
- `kalman_soc/README.md` - Kalman filter implementation details
- `ocv_estimation/README.md` - OCV estimation methods
- `coulomb_counting/README.md` - Current integration algorithms
