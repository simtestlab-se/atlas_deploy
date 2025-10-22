---
comments: true
title: Signal Pre-Processing
status: draft
---


This section describes how **slow** and **fast** filters are applied to current, cell voltage, and temperature for different BMS functions



## Calibration Parameters

$$
N_{\text{slo}} = 10, \quad N_{\text{fast}} = 5
$$

---

## Slow Inputs (SOC Function, 200 ms)

The **State of Charge (SOC)** function requires slow filtering.

- **Current filter:**

$$
I_{\text{filt-slo}}(k) =
\frac{1}{N_{\text{slo}}}
\sum_{i=0}^{N_{\text{slo}}-1} I(k-i)
$$

- **Cell Voltage filter:**

$$
V_{\text{cell, filt-slo}}(k) =
\frac{1}{N_{\text{slo}}}
\sum_{i=0}^{N_{\text{slo}}-1} V_{\text{cell}}(k-i)
$$

- **Temperature filter:**

$$
T_{\text{filt-slo}}(k) =
\frac{1}{N_{\text{slo}}}
\sum_{i=0}^{N_{\text{slo}}-1} T(k-i)
$$

---

## Fast Inputs (SOP Function, 100 ms)

The **State of Power (SOP)** function requires fast filtering.

- **Current filter:**

$$
I_{\text{filt-fast}}(k) =
\frac{1}{N_{\text{fast}}}
\sum_{i=0}^{N_{\text{fast}}-1} I(k-i)
$$

- **Cell Voltage filter:**

$$
V_{\text{cell, filt-fast}}(k) =
\frac{1}{N_{\text{fast}}}
\sum_{i=0}^{N_{\text{fast}}-1} V_{\text{cell}}(k-i)
$$

- **Temperature filter:**

$$
T_{\text{filt-fast}}(k) =
\frac{1}{N_{\text{fast}}}
\sum_{i=0}^{N_{\text{fast}}-1} T(k-i)
$$

---

## Synchronization Rule

!!! warning "Important"
    Whenever a function uses **current and voltage together**,  
    it must consistently use **either slow-filtered or fast-filtered signals**,  
    but **never mix them**.  
    This ensures synchronization of current & voltage measurements.

---
