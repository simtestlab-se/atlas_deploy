@startuml
title OpenEMS Edge - Grid Controllers Overview

package "OpenEMS Edge" #LightGray {
  
  [Channel System] as Channels
  [Scheduler] as Scheduler
  [Battery Inverter] as Inverter
  [PV Inverter] as PVInverter
  [Grid Controller Base\n(AbstractGridController)] as GridController

  package "Grid Controllers" #LightBlue {
    [Grid Export Limit Controller] as ExportLimit
    [Reactive Power Controller] as ReactivePower
    [Ramp Rate Limiter] as RampLimiter
    [Voltage/Frequency Controller] as VoltFreq
  }

  [Backend / UI] as Backend
}

Scheduler --> GridController : trigger control cycle
GridController --> Channels : read Pgrid, limits, SoC
GridController --> ExportLimit : compute Ptarget limit
GridController --> ReactivePower : compute Qtarget
GridController --> RampLimiter : apply rate limits
GridController --> VoltFreq : optional grid support
GridController --> Inverter : set P/Q setpoints
GridController --> PVInverter : adjust PV feed-in if needed
GridController --> Backend : telemetry / alarm

ExportLimit -up-|> GridController
ReactivePower -up-|> GridController
RampLimiter -up-|> GridController
VoltFreq -up-|> GridController
@enduml