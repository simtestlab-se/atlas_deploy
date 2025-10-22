---
comments: true
title: Cycle Counters Specification
status: draft
---

# Cycle Counters Specification

Regardless of whether the cell is new or not, the cell cycle counters shall be set to 0 the first time.  
The counters can remain at 0 until a capacity estimation is made for each cell.  
The cycle counters shall track the consumed cycles.

The cycle count is calculated as:

$$
\text{Cycle Count} = \frac{\text{Total Ah Throughput}}{2 \cdot C_{\text{nom}}}
$$

> **Note:** \(C_{\text{nom}}\) must be updated according to the new measured capacity.

The cell manufacturer provides a table mapping the number of expected cycles versus the cell capacity retention.  
Once the cell capacity has been estimated, one can read the remaining cycles from this table and correct the cycle counter.
