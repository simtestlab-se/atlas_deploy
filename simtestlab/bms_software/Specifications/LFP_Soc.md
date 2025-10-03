State of Charge estimation for LFP cells 

This function runs per cell every 100ms (sampling time).

Inputs: 
cellVolt_(filt-slo), 
Current_(filt-fast), 
Filtered cell temperature, 
Cell capacity

Calibration: 
cal_soc_ocv_lo, 
cal_soc_ocv_hi, 
soc_ocv table

Output: 
Soc for each cell, 
Integrated current (for the whole pack)
Writing soc for each cell in nvram.

At startup:
Soc is estimated at ECU start-up in two ways depending on the cell voltage. 
If the cell voltage is contained between cal_soc_ocv_lo and cal_soc_ocv_hi, then the soc should be read from nvram (previously saved value at shutdown). Else, the soc should be estimated based on soc_ocv tables.
Typically cal_soc_ocv_lo = 10% and cal_soc_ocv_hi is 90%. These parameters depend on the cell model.
Note: after flashing the ECU there are no previously saved data for the soc. In this situation, the soc is considered unknown and a soc recalibration routine needs to be executed.

Every loop:
Soc estimation should be done through coulomb counting:
Soc(k) = soc(k-1) +  (I(k).∆T)/Qcell

Where ∆T is the sampling time mentioned above expressed in seconds s. 
I(k) is a filtered current in A at time k. 
And Qcell is the cell capacity.
Obs: Qcell is in A.s (and not A.h, so the value in Ah needs to be converted to As).

At shutdown:
The state of charge estimation should be saved in Nvram at shutdown for each cell.

OCV tables
Soc_ocv table is a table provided by the cell manufacturer. It maps the cell voltage, cell temperature (ignore for this iteration) to the cell soc whenever the cell is at complete rest.
There are two OCV tables, one for discharge and one for charge. One shall read from the OCV charge, after a charging sequence and once the cell is relaxed (>10min).
One shall read from the OCV discharge, after a discharging sequence and once the cell is relaxed. 
The cell is considered relaxed when the current has been below a certain limit (usually 5A - cell dependent) for more than a certain amount of time (usually 10min - cell dependent).
Over a longer period of time (> 1 day) of rest, one shall read the mid-curve of ocv charge and discharge.

Depth of discharge:
The cell manufacturer recommends to use a certain SoC span (e.g. 10-90%) in order to garantee a certain amount of lifecycle (e.g. 4000 cycles).
Therefore one shall use the battery within the agreed SoC regions.
The depth of discharge extends from soc_win_lo to soc_win_hi parameters.

Useful SoC window:
A useful SoC window is to be communicated externally from 0 to 100% such that 0% corresponds to the soc_win_lo and 100% corresponds to soc_win_hi.

Dynamic Hysteresis function (tbd)


-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TESTING
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

a. Coulomb counting integration accuracy:
Bring the cell voltage down to 2.8V. 
Note: This can be done on any system like a cell, module or pack level.
Charge the system with the power supply at constant current = C/3. E.g. if the cell is 72Ah, the charging current will be 24A for 1h. The goal is to push a known amount of coulomb into the system (i.e. 24Ah).
-> Compare: the integrated current in the BMS should be very close to the integrated current provided by the power supply (+/- 1%).

b. OCV reading:
Note: this test requires an accurate capacity estimation of the system (cell, module, pack).
Bring the cell voltage down to 2.8V with a low current (C/10). The cell voltage should be 2.8V when the cell is relaxed (after 10min). The cell is considered depleted and at 0% soc.
Make sure the soc function reads 0% (+/- 2%).

Charge the cell with Qmeas/4 current for 1h, where Qmeas is the previously measured capacity. Then stop charging.
-> Observe: The soc should show 25% (+/- 2%).
Wait for 10-15min.
-> Observe: The soc should should still show 25% (+/- 2%).

Repeat the procedure 2 more times for 50% and 75%.

Lastly, charge the cell with the remaining Qmeas/4 coulomb (the charge current may vary in the end of the charging session).
-> Observe: the soc should show 100% (+/- 2%).
Wait for 10-15min.
-> Observe: The soc should should still show 100% (+/- 2%).

