@startuml
title ESS Controller - Control Cycle Sequence

actor "Scheduler" as scheduler
participant "AbstractEssController" as controller
participant "Channel System" as channels
participant "Battery Device" as battery
participant "Battery Inverter" as inverter
participant "Virtual ESS Aggregator" as aggregator
participant "Backend / UI" as backend

== Periodic Execution ==
scheduler -> controller : trigger() every N ms

== Data Acquisition ==
controller -> channels : read(SoC, Pgrid, Ppv, Pbat, limits)
channels --> controller : return measured values

controller -> battery : getStateOfCharge()
battery --> controller : SoC value

controller -> inverter : getAvailablePower()
inverter --> controller : PmaxCharge / PmaxDischarge

== Control Logic ==
controller -> controller : computeTargetPower()\n(based on SoC, PV, Grid, Targets)
controller -> controller : applySafetyLimits()\n(voltage, current, temperature)

== Actuation ==
controller -> inverter : setActivePower(Ptarget)
inverter --> controller : acknowledge()

controller -> battery : updateStatus(SoC, PowerDirection)
battery --> controller : confirm update

== Aggregation ==
controller -> aggregator : reportStatus(SoC, Pbat, Pgrid)
aggregator --> controller : aggregated feedback

== Publish ==
controller -> backend : sendTelemetry(SoC, Pbat, Mode, Limits)
backend --> controller : new setpoints or control mode

note over controller
Cycle Time: 1s typical
Handles:
 - Power balancing (charge/discharge)
 - Peak shaving / self-consumption
 - Grid export limit
 - SoC protection
end note
@enduml