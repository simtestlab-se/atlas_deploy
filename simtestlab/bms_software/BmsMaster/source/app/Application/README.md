# BMS Algorithm Test Suite

A comprehensive Battery Management System (BMS) algorithm implementation and test suite for LiFePO4 (Lithium Iron Phosphate) batteries.

## Project Overview

This project implements a complete BMS algorithm stack with the following key components:

### Core Modules

1. **SOC Estimation** (`soc/`)
   - Open Circuit Voltage (OCV) estimation
   - Coulomb counting with temperature compensation
   - Hybrid SOC calculation combining OCV and coulomb counting
   - LiFePO4-specific voltage-SOC lookup tables

2. **Contactor Control** (`ContactorControl/`)
   - Precharge sequence management
   - Main contactor control (Positive, Negative, Precharge)
   - Safety-based contactor logic
   - Voltage differential monitoring

3. **Cell Limit Violation Detection** (`CellLimitViolation/`)
   - Overvoltage protection (>3.65V)
   - Undervoltage protection (<2.50V)
   - Temperature monitoring (0°C to 60°C)
   - Overcurrent protection (>100A)
   - Cell imbalance detection (>50mV difference)

4. **Global Database** (`global_db.c/h`)
   - Centralized data storage for all sensor readings
   - Battery pack parameters and states
   - Temperature and current measurements

## Features

- **Real-time Safety Monitoring**: Continuous monitoring of voltage, current, and temperature limits
- **Intelligent Contactor Management**: Automated precharge sequences and safety shutdowns
- **Accurate SOC Estimation**: Hybrid algorithm using both OCV and coulomb counting methods
- **Temperature Compensation**: Voltage readings adjusted for temperature variations
- **Cell Balancing Support**: Infrastructure for active/passive cell balancing
- **Modular Architecture**: Easy to integrate with different hardware platforms

## Hardware Configuration

- **Battery Chemistry**: LiFePO4 (Lithium Iron Phosphate)
- **Configuration**: 12S1P (12 cells in series, 1 parallel group)
- **Voltage Range**: 2.5V - 3.65V per cell
- **Pack Voltage**: 30V - 43.8V nominal
- **Current Range**: ±100A maximum
- **Temperature Range**: 0°C to 60°C operating

## Test Scenarios

The test suite includes 5 comprehensive scenarios:

1. **Normal Balanced Operation**
   - Balanced cell voltages around 3.3V
   - Low current (50mA no-load condition)
   - Room temperature operation
   - Tests basic SOC calculation and contactor logic

2. **Charging Scenario**
   - Higher cell voltages (3.44V - 3.48V)
   - Charging current (15A)
   - Elevated temperature (30°C)
   - Tests charging protection and SOC accuracy

3. **Discharging Scenario**
   - Lower cell voltages (3.13V - 3.18V)
   - Heavy discharge current (-30A)
   - Higher temperature (35°C)
   - Tests discharge protection and thermal management

4. **Overvoltage Protection**
   - One cell at 3.70V (above 3.65V limit)
   - Cell imbalance condition
   - High temperature (45°C)
   - Tests safety shutdown and fault detection

5. **Undervoltage Protection**
   - One cell at 2.40V (below 2.50V limit)
   - Heavy discharge (-50A)
   - Cold temperature (15°C)
   - Tests critical safety protection

## Building the Project

### Prerequisites
- GCC compiler with C99 support
- Make utility
- Math library support (-lm)

### Compilation
```bash
# Clean previous builds
make clean

# Build the project
make

# Build and run tests
make run

# Debug build
make debug

# Release build  
make release
```

### Windows-Specific Commands
```cmd
cd /d "path/to/Algorithm"
make clean
make
bms_test.exe
```

## Project Structure
```
Application/
├── algorithm.c/h                       # Main algorithm coordination
├── platform_config.h                   # Platform configuration
├── bms_soc_estimation/                 # State of Charge estimation
│   ├── ocv_estimation/                 # Open Circuit Voltage
│   │   └── ocv.c/h
│   ├── coulomb_counting/               # Current integration
│   │   └── coulomb_counting.c/h
│   └── cell_configuration/             # Battery parameters
│       └── cell_configuration.c/h
├── ContactorControl/                   # Contactor management
│   └── ContactorControl.c/h
└── CellLimitViolation/                 # Safety monitoring
    └── CellLimitViolation.c/h
```

## Output Analysis

The test suite provides detailed output including:

- **Pack Status**: Voltage, current, temperature readings
- **Individual Cell Data**: Per-cell voltages and SOC estimates
- **Contactor States**: Real-time status of all contactors
- **Safety Status**: All protection system states
- **SOC Summary**: Min/Max/Average SOC across the pack

### Sample Output Interpretation

```
=== BMS Status ===
Pack Voltage: 39.63 V          # Total pack voltage
Current: -30.00 A               # Negative = discharge
Temperature: 35.0 °C            # Operating temperature

Cell Voltages:                  # Individual cell voltages
Cell  1: 3.150 V  Cell  2: 3.160 V  ...

SOC Estimates (OCV-based):      # State of charge per cell
Cell  1: 35.0%  Cell  2: 40.0%  ...
SOC Summary - Min: 25.0%, Max: 50.0%, Avg: 36.2%

Contactor States:               # Physical contactor positions
Positive: CLOSED                # Main positive contactor
Negative: CLOSED                # Main negative contactor  
Precharge: OPEN                 # Precharge contactor

Safety Status:                  # Protection system status
Overvoltage: OK                 # No cells above 3.65V
Undervoltage: OK                # No cells below 2.50V
Cell Imbalance: OK              # All cells within 50mV
```

## Integration Notes

### Hardware Integration
- Modify `global_db.c` to read from actual ADC/sensors
- Update contactor control functions to interface with GPIO
- Add CAN/UART communication protocols as needed
- Implement actual temperature sensor reading

### Calibration
- Adjust voltage thresholds in `CellLimitViolation.c`
- Update OCV lookup table in `cell_configuration.c`
- Calibrate current sensor scaling factors
- Set appropriate capacity values for coulomb counting

### Real-time Considerations
- Call `Check_CellLimitViolations()` every 10-100ms
- Update `ContactorControl_UpdateLogic()` every 100ms
- SOC estimation can run at 1Hz
- Log critical events for diagnostics

## Safety Features

This BMS implementation includes multiple layers of protection:

1. **Hardware Protection**: Contactor-based isolation
2. **Software Protection**: Real-time limit monitoring  
3. **Redundant Checking**: Multiple safety algorithms
4. **Fail-Safe Design**: Default to safe state on errors
5. **Temperature Compensation**: Voltage adjustments for accuracy

## License

This project is provided as an educational and development reference for BMS algorithms. Ensure proper testing and validation before using in production systems.

## Contributing

When modifying the code:
1. Maintain the modular architecture
2. Add comprehensive test scenarios
3. Update documentation for any parameter changes
4. Validate safety-critical functions thoroughly
5. Follow C99 coding standards
