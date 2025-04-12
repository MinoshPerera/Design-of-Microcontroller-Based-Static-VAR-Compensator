# ⚡ Off-Grid Inverter with Static VAR Compensator (SVC) ⚙️🔋

![Project Badge](https://img.shields.io/badge/Microcontroller-ATmega328P-blue?style=flat&logo=arduino)
![Platform](https://img.shields.io/badge/Platform-Arduino%20IDE-orange)
 
## 🛠 Project Overview

This project involves the **design and development of a single-phase off-grid inverter** system integrated with a **Static VAR Compensator (SVC)**. It focuses on **real-time power factor correction**, **reactive power compensation**, and **voltage stability**—key factors in enhancing power quality and energy efficiency in off-grid and hybrid renewable energy environments.

> **Final Year Design Project - Electrical & Electronic Engineering**

---

## 🧑‍💻 My Technical Contributions

### ✅ Static VAR Compensator (SVC) Design
- Implemented a **real-time power factor correction system** using capacitor bank switching.
- Integrated the compensation logic to work dynamically based on measured reactive power.

### ✅ Phase Angle Detection
- Designed and implemented a **Zero-Crossing Detector (ZCD)** circuit for phase angle measurement.
- Used square waveform outputs to calculate delay between current and voltage waveforms.

### ✅ Microcontroller Programming
- Programmed the **ATmega328P** to:
  - Calculate **real power**, **power factor**, **phase angle**, and **reactive power**
  - Measure RMS values of voltage and current using the **EmonLib** library
  - Display real-time data on a **20x4 I2C LCD**

### ✅ Capacitor Bank Relay Control
- Developed a **dynamic relay-based capacitor bank switching** mechanism using a **4-channel relay module**.
- Controlled relay switching based on calculated reactive power requirements.

### ✅ Real-Time Monitoring System
- Implemented real-time display of:
  - **Voltage (RMS)**
  - **Current (RMS)**
  - **Phase Angle**
  - **Power Factor**
  - **Active Power**

### ✅ Optimization Results
- **Improved Power Factor from 0.7 to 0.98**
- **Reduced reactive power losses**
- **Enhanced energy efficiency and voltage stability**

---

## 🔌 Hardware Used

- **ATmega328P Microcontroller**
- **230V–12V Step-down Transformer**
- **5A–5mA Current Transformer**
- **Zero-Crossing Detector Circuit**
- **Capacitor Bank (2.5μF, 5μF, 7.5μF, 10μF)**
- **4-Channel Relay Module (MD0083)**
- **20x4 I2C LCD Display**
- **Soldered PCB (Designed on EasyEDA)**

---

## 🧰 Software Tools

- **Arduino IDE** (Code Development & Upload)
- **Proteus** (Circuit Simulation)
- **EasyEDA** (PCB Design)
- **EmonLib** (Energy Monitoring Library)

---

## 🧠 Microcontroller Operation of SVC

The figure below illustrates the **signal flow and component interactions** within the SVC system, coordinated by the ATmega328P microcontroller. It shows how voltage and current sensing, zero-crossing detection, and capacitor switching work together to monitor and correct power factor in real-time.

![Microcontroller Operation of SVC](images/svc_microcontroller_operation.png)

---

## 📈 System Performance

| Parameter         | Before (Initial) | After (With SVC) |
|------------------|------------------|------------------|
| Power Factor      | 0.70             | 0.98             |
| Reactive Loss     | High             | Minimized        |
| Voltage Stability | Unstable         | Stable           |

---
