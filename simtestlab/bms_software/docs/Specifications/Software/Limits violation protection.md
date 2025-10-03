---
comments: true
---

# Limits Violation Protection

The BMS shall detect any violation on any cell of the pack and react as fast as possible to protect the batteries.  
It shall continuously monitor cell voltages and temperatures and act by either opening the contactor and/or reducing the power limits.

---

## Inputs
- Cell voltage (fast filtered)
- Cell temperature (fast filtered)

## Outputs
- Contactor Immediate Opening Request
- Critical error flag

## Parameters

| Parameter       | Range          |
|-----------------|----------------|
| \(V_{\text{crit,min}}\)      | 2.3 V          |
| \(V_{\text{crit,max}}\)      | 3.8 V          |
| \(T_{\text{crit,min}}\)      | 0 °C           |
| \(T_{\text{crit,max}}\)      | 45 °C          |

![BMS Diagram](bms-limits.svg)

---

## Cell Voltage Limits

1. **High limit**  
   If any cell voltage (fast filtered) \(> V_{\text{crit,max}}\):  
   - Immediately open all contactors  
   - Throw a critical error

2. **Low limit**  
   If any cell voltage (fast filtered) \(< V_{\text{crit,min}}\):  
   - Immediately open all contactors  

---

## Cell Temperature Limits

1. **High limit**  
   If any cell temperature (fast filtered) \(> T_{\text{crit,max}}\):  
   - Immediately open all contactors  
   - Throw a critical error

2. **Low limit**  
   If any cell temperature (fast filtered) \(< T_{\text{crit,min}}\):  
   - Immediately open all contactors  
   - Throw a critical error

---

## Testing Procedure

> **Note:** Testing should be done with fake limits to avoid damaging cells.

### Voltage Limits

- **High Limit Test**
  1. Set cell voltage to ~3–3.2 V  
  2. Set `V_crit_max` to 3.5 V  
  3. Slowly charge the cell until it reaches 3.5 V (current < 10 A)  

- **Low Limit Test**
  1. Set cell voltage to ~3–3.2 V  
  2. Slowly discharge the cell until it crosses 2.9 V (current < 10 A)  

---

### Current Limits

- **Charge Limits**
  1. Adjust `T_crit_min` and `T_crit_max` close to actual cell temperature  

- **Discharge Limits**
  1. **Observe:** Contactors open immediately upon crossing limits  

---


