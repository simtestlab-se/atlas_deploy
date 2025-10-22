---
comments: true
title: LFP Cell State of Charge (SoC) Estimation
status: draft
---

# State of Charge (SoC) Estimation for LFP Cells

This function runs per cell every 100 ms (sampling time).

---

## Inputs
- Cell voltage (slow filtered): `cellVolt_(filt-slo)`
- Current (fast filtered): `Current_(filt-fast)`
- Filtered cell temperature
- Cell capacity

## Calibration
- `cal_soc_ocv_lo`
- `cal_soc_ocv_hi`
- `soc_ocv` table

## Outputs
- SoC for each cell
- Integrated current (for the whole pack)
- Writing SoC for each cell in NVRAM

---

## Startup

At ECU start-up, SoC is estimated in two ways depending on the cell voltage:  
- If the cell voltage is between `cal_soc_ocv_lo` and `cal_soc_ocv_hi`, read SoC from NVRAM (previously saved value at shutdown).  
- Otherwise, estimate SoC based on `soc_ocv` tables.

> **Note:** Typically `cal_soc_ocv_lo = 10%` and `cal_soc_ocv_hi = 90%`. These parameters depend on the cell model.  
> After flashing the ECU, there are no previously saved SoC values. In this situation, SoC is considered unknown and a recalibration routine must be executed.

---

## Every Loop (Coulomb Counting)

SoC is updated using coulomb counting:

$$
\text{SoC}(k) = \text{SoC}(k-1) + \frac{I(k) \cdot \Delta T}{Q_{\text{cell}}}
$$

Where:  
- \(\Delta T\) = sampling time in seconds (0.1 s)  
- \(I(k)\) = filtered current in A at time step \(k\)  
- \(Q_{\text{cell}}\) = cell capacity in A·s (convert Ah → As)

---

## Shutdown

The SoC estimation should be saved in NVRAM at shutdown for each cell.

---

## OCV Tables

- `soc_ocv` table maps cell voltage (and temperature, ignored in this iteration) to SoC when the cell is at rest.  
- Two tables exist: **charge** and **discharge**.  
  - After charging and rest (>10 min), read from OCV charge table.  
  - After discharging and rest (>10 min), read from OCV discharge table.  
- A cell is considered relaxed if current < certain limit (e.g., 5 A) for a certain time (e.g., 10 min).  
- For long-term rest (>1 day), read the mid-curve between OCV charge and discharge.

---

## Depth of Discharge

- Manufacturer recommends using a SoC span (e.g., 10–90%) to guarantee lifecycle (e.g., 4000 cycles).  
- Use battery within agreed SoC regions: `soc_win_lo` → `soc_win_hi`.

---

## Useful SoC Window

- Externally communicated as 0–100%  
- 0% corresponds to `soc_win_lo`  
- 100% corresponds to `soc_win_hi`

---

## Dynamic Hysteresis Function
- To be defined (TBD)

---

# Testing Procedure

### a. Coulomb Counting Integration Accuracy
1. Bring cell voltage down to 2.8 V (cell, module, or pack level).  
2. Charge system at constant current = C/3 (e.g., 72 Ah cell → 24 A for 1 h).  
3. Compare integrated current in BMS vs. power supply (target: ±1%).

---

### b. OCV Reading
1. Bring cell voltage down to 2.8 V with low current (C/10).  
2. Wait until cell is relaxed (~10 min).  
3. Verify SoC = 0% (±2%).  

4. Charge cell with Qmeas/4 current for 1 h. Stop charging.  
   - Observe: SoC ≈ 25% (±2%)  
   - Wait 10–15 min → SoC still ≈ 25% (±2%)

5. Repeat for 50% and 75% SoC.  

6. Charge remaining Qmeas/4 (final charge current may vary).  
   - Observe: SoC ≈ 100% (±2%)  
   - Wait 10–15 min → SoC still ≈ 100% (±2%)
