## Project Roadmap

<div class="mermaid-wrapper">
```mermaid
gantt
    dateFormat  YYYY-MM-DD
    title Simtestlab Project Roadmap
    axisFormat  W%U
    tickInterval 2week


    section BESS
    BMS Developement      :active,bms, 2025-09-01, 2025-12-15
    EMS Developement      :active,ems, 2025-10-16, 2025-12-05
    System Integration      :active, 2025-12-16, 2026-03-05
    section Software
    Firmware Development :crit,12, 2025-08-06, 15w
    GUI Development      : 2025-11-16, 2025-12-15
    section Hardware
    BMU controller  : 2025-08-16, 15w
    CSC  : 2025-10-1, 8w
    BDUe  : 2025-10-16, 6w
    CSC  : 2025-11-16, 4w
    section IT And Infrastructure
    eSysFlow  : 2025-08-16, 15w



```
</div>

*Note: The timeline is subject to change based on project requirements and resource availability.*


Simtestlab Project Roadmaps

<div class="plantuml-wrapper">
```puml
@startgantt
<style>
ganttDiagram {
  task {
    BackGroundColor yellowgreen
    LineColor red
  }
  done {
    BackGroundColor #80FFFFFF
  }

}
</style>
printscale daily
sunday are closed
Project starts 2025-09-01
today is colored in red
' Long timeline to demonstrate scrolling
-- BMS Development --

[Software Developement] as [P1] requires 60 days and is 50% complete 
[Contactor control] as [bmssw1] requires 5 days and is 50% complete 
[Cell Limits] as [bmssw2] requires 5 days and is 50% complete 
[Soc Estimation] as [bmssw3] requires 10 days and is 50% complete 
[Cell Model] as [bmssw4] requires 10 days and is 50% complete 


[Phase 2 Development] as [P2] lasts 60 days and is 50% complete 

-- EMS Development --
[Phase 3 Testing] as [P3] lasts 30 days
[Phase 4 Deployment] as [P4] lasts 10 days
[Phase 5 Maintenance] as [P5] lasts 90 days

[bmssw1] starts at [P1]'s start
[bmssw2] starts at [bmssw1]'s end
[bmssw3] starts at [bmssw2]'s end
[bmssw4] starts at [bmssw3]'s end

[P2] starts at [P1]'s end
[P3] starts at [P2]'s end
[P4] starts at [P3]'s end
[P5] starts at [P4]'s end

[P1] is colored in LightBlue
[P2] is colored in LightGreen
[P3] is colored in Gold
[P4] is colored in Coral
[P5] is colored in LightGray

'holidays
2025-08-15 to 2025-08-20 are closed

[Planning Phase] is colored in LightBlue
@endgantt


```
</div>


