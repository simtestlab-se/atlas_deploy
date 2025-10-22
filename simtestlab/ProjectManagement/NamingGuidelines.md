# Prefix rule Interface Signal Naming Convention

## 1. Prefix: `se_`
Indicates shared interface signal between BSW and APP layers.  
- **Prefix rules:**
  - For **BSW ↔ APP exchange signals**, use `se_`.  
    - Example: `se_meas_cellVolt_f[NUM_CELLS]`
  - For **signals internal to an APP SWC**, use the shortname of the SWC as prefix:  
    - `bmsse_` → SOC Estimation SWC  
    - `bmscc_` → Contactor Control SWC  
    - `bmstp_` → Temperature Processing SWC  

**Example:** `se_` + logical type + descriptive name + type suffix  

---

## 2. Logical Group Prefix
Use one of the following signal type prefixes after `se_`:

| Prefix  | Meaning                                      | Examples                     |
|---------|----------------------------------------------|------------------------------|
| `meas_` | Measured signals (voltages, currents, temperatures) | `se_meas_cellVolt_f` |
| `ctrl_` | Control commands or switch statuses         | `se_ctrl_posContactorSw_b`  |
| `calc_` | Calculated values / SOC / derived signals  | `se_calc_socAvg_f`          |
| `fault_`| Fault indicators / diagnostic flags        | `se_fault_ovVolt_b`         |
| `sys_`  | General system-level indicators            | `se_sys_keySw_b`            |

---

## 3. Descriptive Name
- Use **camelCase** for the actual signal name.  
- Be concise but meaningful:

Positive Contactor switch → posContactorSw
Cell balancing active → balancingActive
Overvoltage fault → ovVolt


---

## 4. Type Suffix
Indicate the **data type** at the end of the name:

| Suffix  | Type       | Example                     |
|---------|-----------|-----------------------------|
| `_f`    | float      | `se_meas_packVolt_f`       |
| `_b`    | bool       | `se_ctrl_contactorOpCmd_b` |
| `_u32`  | uint32_t   | `se_sys_time_u32`          |

---

## 5. Units in Comments
Always document units in the **inline comment**. Use **SI units** for clarity:

| Signal       | Unit | Example Comment                |
|--------------|------|--------------------------------|
| Voltage      | V    | `/**< Pack voltage [V] */`    |
| Current      | A    | `/**< Pack current [A] */`    |
| Temperature  | °C   | `/**< NTC temperature [°C] */`|
| SOC          | %    | `/**< Average SOC [%] */`     |
| Timestamp    | ms   | `/**< System timestamp [ms] */`|

---

## 6. Grouping
Organize signals in the header file in **logical blocks**:

- **MEASUREMENTS** → `se_meas_`  
- **CONTROL / STATUS** → `se_ctrl_`  
- **CALCULATED / SOC** → `se_calc_`  
- **FAULT INDICATORS** → `se_fault_`  
- **SYSTEM STATUS** → `se_sys_`  

**Example:**

```c
/* ========================= CONTROL / STATUS ========================= */
extern bool se_ctrl_posContactorSw_b; /**< Positive Contactor SW */
extern bool se_ctrl_negContactorSw_b; /**< Negative Contactor SW */
extern bool se_ctrl_prechargeContactorSw_b; /**< Precharge Contactor SW */

```
# Signal Naming Guidelines

## 7. Optional Guidelines

- **Array signals**: Use plural descriptive names with indices:  
```c
se_meas_cellVolt_f[NUM_CELLS]
```
Single-bit flags: Use _b suffix.

Avoid abbreviations unless standard in project (e.g., OV for overvoltage).

Keep names under 30 characters if possible, but clarity comes first.

Always document the signal with inline comment, including units.

8. Example: Full Signal Name Construction

Signal: Positive Contactor switch (boolean)


| Step        | Name Component                  | Result                                                                      |
| ----------- | ------------------------------- | --------------------------------------------------------------------------- |
| Prefix      | `se_`                           | `se_`                                                                       |
| Type group  | `ctrl_`                         | `se_ctrl_`                                                                  |
| Descriptive | `posContactorSw`                | `se_ctrl_posContactorSw`                                                    |
| Type suffix | `_b`                            | `se_ctrl_posContactorSw_b`                                                  |
| Comment     | `/**< Positive Contactor SW */` | Final declaration: `se_ctrl_posContactorSw_b /**< Positive Contactor SW */` |
