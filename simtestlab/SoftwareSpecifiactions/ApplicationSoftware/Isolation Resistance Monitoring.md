# Isolation Resistance Monitoring

## Background
The IRM is used to ensure that the HV BAT+ is sufficiently isolated from the ground (casing) and that the HV BAT- is also sufficiently isolated from the ground.
A set of measurements is performed by the BMS at startup before closing the contactors.
If the measurement results are showing a low isolation value, the BMS shall not close the contactors and shall report an error.

## Requirement
IEC 62477-1 / IEC 60755 (PCS requirements) states that there needs to be at least 500 Ohm per volt on the high voltage side. E.g. a 300V system would need an isolation of 150 kOhm
This only applies to high voltage systems (i.e. > 60V).

```puml
@startuml

' External signal sources (left)
participant "Source" as Inputs

' SWC in the middle   
box "Isolation Resistance Monitoring SWC" #LightGreen
  participant "SWC Input" as In
  participant "SWC Output " as Out

end box

' Signals into SWC ############################# EDIT ONLY HERE #############################
Inputs -> In : HV_BattPos_GND (BDU)
Inputs -> In : HV_BattNeg_GND (BDU)

' Signals from SWC ##########################  EDIT ONLY HERE ################################
Out -> Output : HV_BattPos_GND_cmd (BDU/IO)
Out -> Output : HV_BattNeg_GND_cmd (BDU/IO)
Out -> Output : HV_BattPos_GND_Res (CAN)
Out -> Output : HV_BattNeg_GND_Res (CAN)



' Signals from SWC ##########################  EDIT ONLY HERE ################################
note right of In 
    <b> Calibration Parameters:</b>
    - 
end note
@enduml

```
