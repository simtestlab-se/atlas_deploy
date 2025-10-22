---
comments: true
title: Current Integrator Specification
status: deprecated
---

# Current Integrator Specification

## Inputs
- Current (fast filtered)

## Outputs
- Integrated current
- Current integrator

---

This function runs every 50 ms and integrates current at a faster rate to capture dynamic variations.  

The basic integration function is defined as:

$$
\text{Int\_current}_{k+1} = \text{Int\_current}_k + I_{\text{fast}}(k) \cdot \Delta T
$$

Where:  
- \(I_{\text{fast}}(k)\) = fast-filtered current at time step \(k\)  
- \(\Delta T\) = sampling time (50 ms)  
- \(\text{Int\_current}_{k+1}\) = integrated current at next time step
