# Interactive 3D Model Demo

See the interactive BESS module enclosure model below. You can rotate, zoom, and inspect the model right in your browser.

<!-- Load the model-viewer library -->
<script type="module" src="https://unpkg.com/@google/model-viewer/dist/model-viewer.min.js"></script>

<model-viewer
  src="/models/Pack_Design.glb"
  alt="BESS module enclosure"
  camera-controls
  auto-rotate
  shadow-intensity="0.6"
  style="width:100%;max-width:800px;height:480px;--poster-color: #84c5adff;background-color:#013220;">
</model-viewer>

## Module Design Overview

# Battery Pack Technical Specification Document

- **Model:** model 0.1  
- **Version:** 0.0.1  
- **Date:** October 7, 2025  
- **Designed By:** Akilasekar K

---

## 1. General Description

This document describes the specifications of a custom-designed 12-cell Lithium Iron Phosphate (LFP) battery pack intended for energy storage. The pack is enclosed in a robust, thermally-managed enclosure and conforms to relevant electrical safety standards.

---

## 2. Electrical Specifications

| Parameter              | Value                                 |
|------------------------|----------------------------------------|
| Cell Chemistry         | Lithium Iron Phosphate (LFP)          |
| Number of Cells        | 12 (configured in [series/parallel])  |
| Nominal Voltage        | 38.4 V                                |
| Nominal Capacity       | 72 Ah                                 |
| Energy                 | 3.84 kWh                              |
| Max Charge Voltage     | 43.2 V                                |
| Cut-off Voltage        | 30 V                                  |
| Max Discharge Current  | 100 A                                 |
| Max Charge Current     | 50 A                                  |
| Connector Type         | Amphenol (SurLok Plus, socket)        |

---

## 3. Mechanical Specifications

| Parameter              | Value                  |
|------------------------|------------------------|
| Enclosure Material     | Aluminum 6061          |
| Surface Finish         | Powder coated          |
| Dimensions (L×W×H)     | 550 × 139 × 245 mm     |
| Wall Thickness         | 1.5 mm                 |
| Weight                 | 26 kg (with cells)     |

**Enclosure Features:**

- Top-mounted passive heat sink fins for thermal dissipation  
- Recessed connector panel for cable protection  
- Removable top and front panels for serviceability  

---

## 4. Thermal Management

- Passive cooling heatsink fins  
- Optional forced air cooling depending on load profile  

---

## 5. Safety & Compliance

- Short circuit protection  
- Over-voltage, under-voltage, over-temperature protection  
- Fuse integrated in main circuit  
- High-voltage insulation and creepage distance compliance  
