

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
Project starts 2025-09-23
today is colored in red
' Long timeline to demonstrate scrolling
-- Mechanical Design and developement --

[Battery Module Design: I ] as [bmsmd1] requires 11 days and is 100% complete 
[Battery Module Design Upgrade  ] as [bmsmd2] requires 5 days and is 0% complete 
[BESS Rack desgin ] as [bmsmd3] requires 10 days and is 0% complete 
-- Mechanical Componet Procurement --
[BAtteryModule : Procurement] as [bmsmp1] requires 10 days and is 10% complete 

-- Mechanical Manufacturing --
[Production ] as [bmsmpro1] requires 10 days and is 10% complete 

[bmsmp1] starts at [bmsmd1]'s end
[bmsmd2] starts at [bmsmd1]'s end
[bmsmd3] starts at [bmsmd2]'s end
[bmsmd1] is colored in LightBlue
[bmsmpro1] starts at 2025-10-31
'holidays
2025-10-10 to 2025-10-13 are closed
2025-10-18 to 2025-10-21 are closed

@endgantt


```
</div>