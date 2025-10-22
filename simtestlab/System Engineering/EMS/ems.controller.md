@startuml
title OpenEMS Edge - Controller Architecture

package "OpenEMS Edge" #LightGray {
  
  [Edge Core] as core
  [Channel System] as channels
  [Scheduler] as scheduler
  [Bridge Layer] as bridges
  [Devices] as devices
  
  package "Controllers" #LightBlue {
    [Controller Base\n(AbstractController)]
    
    package "ESS Controllers" #LightYellow {
      [Peak Shaving Controller]
      [Self Consumption Controller]
      [Prepare Charge Controller]
    }
    
    package "Grid Controllers" #LightYellow {
      [Grid Export Limit Controller]
      [Reactive Power Controller]
    }
    
    package "Tariff & Schedule" #LightYellow {
      [Time Of Use Controller]
      [Scheduled Setpoint Controller]
    }
    
    package "Load & EV" #LightYellow {
      [EVCS Controller]
      [Load Shedding Controller]
    }
    
    package "Optimization" #LightYellow {
      [Energy Balancer Controller]
      [Optimizer V1/V2]
    }
    
    package "Simulation Controllers" #LightYellow {
      [Virtual ESS Controller]
      [Test/Debug Controller]
    }
  }
}

core --> scheduler : runs controllers periodically
core --> channels : provides live data access
controllers --> channels : read/write channel values
controllers --> devices : control setpoints, read measurements
controllers --> core : register / update status
bridges --> devices : protocol communication (Modbus, HTTP, etc.)
scheduler --> controllers : execute control logic loop

note bottom of controllers
Controllers implement logic to:
- Collect input data via Channels
- Compute control setpoints
- Write outputs to devices
- Maintain state across cycles
end note
@enduml