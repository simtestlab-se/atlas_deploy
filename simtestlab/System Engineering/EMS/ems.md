# OpenEMS Edge – Functional Overview

OpenEMS Edge is the **on-site runtime** of the OpenEMS ecosystem.  
It runs close to your energy hardware, handling data acquisition, control logic, and communication with the backend.

---

## 🧠 What Is It?

OpenEMS Edge is modular and built around the **OSGi framework**.  
Every feature (driver, controller, API, bridge, scheduler…) is an OSGi bundle under `io.openems.edge.*`.

This document lists **all core functionalities other than device drivers**.

---

## ⚙️ 1. Core Framework
📁 `io.openems.edge.core`, `io.openems.edge.common`

| Function | Description |
|-----------|--------------|
| **Component Model** | Defines the `OpenemsComponent` interface; every device, controller, or service extends it. |
| **Channels** | Standardized data points (values + metadata) connecting all modules (e.g. voltage, power, SoC). |
| **Schedulers** | Orchestrates periodic execution of components, controllers, and device polling cycles. |
| **Persistence / Data Buffering** | Local short-term buffering for channel values before backend sync. |
| **Event / Logging system** | Centralized event bus for module logs, alarms, and system state changes. |

---

## 🧠 2. Controllers
📁 `io.openems.edge.controller.*`

These are the **energy management algorithms** that use input from devices and send control setpoints back.

| Type | Example | Purpose |
|------|----------|----------|
| **Battery controllers** | `Controller.Ess.PrepareCharge`, `Controller.Ess.LimitPeakShaving` | Optimize SoC, charge/discharge control |
| **Grid interaction** | `Controller.GridLimitActivePower` | Ensure export/import limits |
| **PV & generation** | `Controller.PvSelfConsumption`, `Controller.FeedinLimitation` | Maximize self-consumption |
| **Tariff & scheduling** | `Controller.Schedule`, `Controller.TimeOfUseTariff` | Execute time-based power profiles |
| **EV & load control** | `Controller.Evcs.*`, `Controller.Load.*` | EV charging and load shedding logic |
| **Balancing / optimization** | `Controller.EnergyBalancer` | Global optimization between sources and sinks |
| **Simulator controllers** | Used for test environments without hardware |

> 🔧 You can write your own by extending `AbstractController`.

---

## 🕸️ 3. Bridges
📁 `io.openems.edge.bridge.*`

Bridges connect Edge logic to communication protocols or buses — not device-specific, but generic protocol handlers.

| Bridge | Description |
|---------|--------------|
| **Modbus (RTU/TCP)** | Base layer for many inverters/meters |
| **HTTP / REST** | Cloud APIs or REST devices |
| **M-Bus** | Metering bus for heat/electric meters |
| **OneWire** | Low-level temperature sensors |
| **CAN / MQTT** *(custom in some OEM forks)* | Additional physical layers |
| **Simulator bridge** | Generates synthetic data for dev/testing |

---

## 🧮 4. Energy Storage System (ESS) Coordination
📁 `io.openems.edge.ess.*`

Manages **battery + inverter + PV as a unified ESS**:
- Combine multiple physical devices into one logical storage unit.
- Handle charging/discharging coordination.
- Compute SoC, power, and energy KPIs.
- Provide a uniform ESS API to controllers.

---

## 🌐 5. APIs & Communication Interfaces
📁 `io.openems.edge.api.*`

| Interface | Function |
|------------|-----------|
| **Websocket API** | Real-time bidirectional link to the Backend / UI |
| **JSON-RPC API** | Local or remote structured communication (config, control, data query) |
| **REST API (via Backend)** | For third-party integration (HTTP layer handled in Backend but initiated here) |
| **MQTT** | Optional for OEM-specific data publication |

---

## 🧾 6. Configuration Management
📁 `io.openems.edge.application`, `io.openems.edge.config`

- Uses declarative OSGi configuration files (`config.ini`, JSON, or Felix web console).  
- Supports **auto-discovery** for some bridges (e.g. Modbus devices).
- Components can be dynamically added or removed without restart.

---

## 🧰 7. Simulation & Testing
📁 `io.openems.edge.simulator`, `io.openems.edge.test`

| Purpose | Features |
|----------|-----------|
| **Hardware simulation** | Simulate PV, battery, meter, load profiles |
| **Controller testing** | Run closed-loop control logic without physical devices |
| **Integration with Backend simulator** | Sync data across Edge ↔ Backend simulation for system tests |

---

## 📊 8. Metrics & Data Channels

Every component exposes `Channels` → numeric, boolean, or state data points.  
Used for:
- Real-time visualization  
- Logging and control  
- Backend time-series export  

Channels support units, scaling, timestamps, min/max, and metadata — similar to signal dictionaries in automotive systems.

---

## 💾 9. Persistence & Buffering

Edge can temporarily store channel values (in-memory or on-disk ring buffer) to:
- Tolerate network outages.  
- Batch-upload to backend when connectivity restores.  
- Enable local analytics.  

---

## 🚨 10. Fault & Safety Handling

| Function | Description |
|-----------|--------------|
| **Component state machine** | Components have `RUNNING`, `OFFLINE`, `ERROR` states. |
| **Watchdog & self-recovery** | Scheduler monitors tasks and restarts failed components. |
| **Fallback modes** | Controllers can degrade gracefully when data is missing. |

---

## 🧩 11. Extension Framework

- Any new module can be added as an **OSGi bundle**.  
- Extensible by configuration — you can dynamically load your own controllers, APIs, or devices.

---

## 🧍 12. User Interaction Layer

Although UI mainly runs in the **OpenEMS UI (web app)**, Edge can:
- Expose local web dashboards (in dev mode)
- Allow configuration updates via REST/WebSocket RPC
- Emit real-time logs to console or file

---

## 🧠 13. Computation / Virtual Device Abstraction

Edge supports “Virtual” components such as:
- **Virtual Meter** (aggregate multiple meters)
- **Virtual ESS** (merge multiple batteries/inverters)
- **Virtual Load** (computed consumption profile)

Useful for hierarchical or multi-device setups.

---

## 🏗️ Functional Stack Diagram

```text
+--------------------------------------------------+
|                OpenEMS Backend (cloud)           |
+-------------------------▲------------------------+
                          │ WebSocket / RPC
+-------------------------┼------------------------+
|               OpenEMS Edge Runtime               |
| ├── Core Framework (OSGi, Channels, Scheduler)   |
| ├── Controllers (logic/algorithms)               |
| ├── Bridges (protocol interfaces)                |
| ├── ESS management                               |
| ├── Persistence + Data Buffer                    |
| ├── API / Communication layer                    |
| └── Simulation / Virtual devices                 |
+-------------------------┼------------------------+
                          │ Modbus, HTTP, M-Bus...
+-------------------------▼------------------------+
|                 Physical Devices                 |
+--------------------------------------------------+


