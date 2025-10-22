@startuml
title ESS Controller - Mode State Machine (Colored)

skinparam state {
  FontSize 13
  FontStyle bold
  RoundCorner 15
  BorderColor Black
}

[*] --> Idle : System Init Complete

state Idle #LightGreen
state Charging #LightBlue
state Discharging #Orange
state Balancing #Yellow
state Error #Red

note right of Idle
  Waiting for conditions:
  - Stable communication
  - Valid SoC and Power readings
end note

note right of Charging
  Controller actions:
  - Calculate charge limit
  - Command inverter: Ptarget < 0
  - Monitor SoC rise and temperature
end note

note right of Discharging
  Controller actions:
  - Calculate discharge limit
  - Command inverter: Ptarget > 0
  - Limit export according to grid settings
end note

note right of Balancing
  Controller actions:
  - Use balancing resistors or low-current discharge
  - Maintain cell voltage uniformity
end note

note right of Error
  Controller actions:
  - Stop inverter commands
  - Notify backend
  - Wait for reset or safe recovery
end note

Idle --> Charging : SoC < SoC_min\nand PV surplus available
Idle --> Discharging : SoC > SoC_max\nand grid demand > 0
Idle --> Balancing : SoC within nominal range\nand cells unbalanced
Idle --> Error : Fault detected (voltage/temp/current)
Idle --> Idle : No valid control target

Charging --> Idle : SoC ≥ SoC_target
Discharging --> Idle : SoC ≤ SoC_target
Balancing --> Idle : All cells balanced
Error --> Idle : Fault cleared
Idle --> Error : Fault re-detected

[*] <-- Idle : Shutdown or Stop Command
@enduml