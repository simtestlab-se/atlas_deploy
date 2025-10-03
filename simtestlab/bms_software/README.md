

# BMS Master - Battery Management System

Manual:

we use MKdocs for documentation 
all files are version controlled . 

## Overview

This project implements a comprehensive Battery Management System (BMS) for automotive applications using the Infineon XMC4800 microcontroller and BMS6830 AFE (Analog Front End). The system provides advanced State of Charge (SOC) estimation, safety monitoring, and communication capabilities for Li-Ion battery packs.

## Hardware Platform

- **MCU**: Infineon XMC4800-F100 (ARM Cortex-M4)
- **AFE**: Analog Devices BMS6830 (12-cell monitoring)
- **Development Environment**: DAVE IDE 4.5.0
- **Compiler**: ARM GCC 4.9

## Key Features

### Battery Management
- **12-cell Li-Ion battery pack monitoring** (2.6Ah per cell)
- **Dual SOC estimation algorithms**:
  - Primary: Kalman Filter with OCV compensation
  - Fallback: Coulomb Counting
- **Temperature monitoring** via NTC sensors
- **Cell balancing** support with PWM control
- **Voltage and current monitoring**

### Safety & Protection
- **Cell limit violation detection**
- **Contactor control** for pack isolation
- **Real-time monitoring** and fault detection
- **Temperature compensation** for accurate SOC estimation

### Communication
- **CAN bus interface** for vehicle communication
- **EtherCAT slave** for industrial applications
- **Real-time data transmission** of SOC, voltages, and status

## Project Structure

```
BmsMaster/
├── source/app/
│   ├── Application/
│   │   ├── BmsAlgorithm/          # Main SOC estimation coordinator
│   │   ├── BmsSOC/                # SOC estimation modules
│   │   │   ├── BmsSocEstimation/  # Kalman filter implementation
│   │   │   ├── BmsCoulombCounting/ # Coulomb counting backup method
│   │   │   └── OcvEstimation/     # Open Circuit Voltage utilities
│   │   ├── BmsCell/               # Cell configuration and monitoring
│   │   └── BmsPack/               # Pack-level control (contactors)
│   ├── bms/                       # Core BMS tasks and hardware interface
│   ├── adbms6830/                 # BMS6830 AFE driver
│   └── adbms2950/                 # Legacy BMS support
├── Dave/Generated/                # DAVE IDE generated code
├── Libraries/                     # XMC and CMSIS libraries
├── SSC/                          # EtherCAT slave stack
└── Debug/                        # Build output directory
```

## SOC Estimation Algorithm

The system implements a sophisticated dual-algorithm approach:

### Primary: Kalman Filter
- **Extended Kalman Filter** with battery model
- **OCV-based correction** using lookup tables
- **Temperature compensation** for accuracy
- **Dynamic adaptation** to battery aging

### Backup: Coulomb Counting
- **Current integration** for SOC tracking
- **Automatic fallback** when Kalman filter is unavailable
- **Load detection** for OCV measurements
- **Capacity tracking** and correction

## Building the Project

### Prerequisites
- DAVE IDE 4.5.0 or later
- ARM GCC toolchain
- Infineon XMC4800 development board

### Build Steps
1. Open DAVE IDE
2. Import the project: `File > Import > Existing Projects into Workspace`
3. Select the `BmsMaster` directory
4. Build the project: `Project > Build All`

### Command Line Build
```bash
cd Debug
make clean
make all
```

## Configuration

### Battery Configuration
Edit `source/app/Application/BmsCell/cell_configuration/cell_configuration.c`:
- Cell capacity (mAh)
- OCV lookup tables
- Temperature coefficients
- Voltage limits

### Communication Settings
- **CAN bus**: Configure in `can_service.c`
- **EtherCAT**: Settings in `SSC/` directory
- **Baud rates**: Set in DAVE configuration

## Usage

### Initialization
The system automatically initializes on power-up:
1. Hardware initialization (DAVE generated)
2. BMS AFE configuration
3. SOC algorithm initialization
4. Task scheduler startup

### Real-time Operation
- **5ms task**: Voltage and current measurements
- **100ms task**: SOC calculation and CAN transmission
- **500ms task**: Temperature monitoring and balancing

### Monitoring
SOC and status data are transmitted via:
- **CAN messages**: Real-time vehicle integration
- **EtherCAT**: Industrial monitoring systems
- **Debug UART**: Development and diagnostics

## Safety Notes

 **WARNING**: This is automotive battery management software. Improper use can result in:
- Battery damage or fire
- Personal injury
- Vehicle damage

Ensure proper:
- Hardware protection circuits
- Software validation
- System integration testing
- Compliance with automotive standards

## Development Status

### Completed Features
-  Dual SOC estimation algorithms
-  Temperature compensation
-  CAN bus communication
-  Cell monitoring and balancing
-  EtherCAT integration
-  Project structure optimization

### Current Capabilities
- **SOC Accuracy**: ±2% under normal conditions
- **Temperature Range**: -20°C to +60°C
- **Update Rate**: 5-100ms depending on function
- **Cell Count**: 12 cells (expandable)

## Contributing

When modifying the code:
1. Maintain the modular architecture
2. Follow the existing coding style
3. Update documentation
4. Test thoroughly with hardware
5. Validate safety-critical functions

## License

This project is for educational and development purposes. Commercial use requires proper licensing and safety certification.

## Contact

For questions or support regarding this BMS implementation, please refer to the project documentation or contact the development team.

---

**Built with DAVE IDE 4.5.0 | Infineon XMC4800 | ARM Cortex-M4**


