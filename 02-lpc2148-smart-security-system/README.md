# Smart Security & Monitoring System

**Target MCU:** LPC2148 (ARM7TDMI-S)

## 📌 Overview

This project implements a Smart Security & Monitoring System on the LPC2148 microcontroller, integrating authentication, user interaction, temperature monitoring, and PWM-based fan control.

The firmware is designed with a modular driver architecture and focuses on deterministic system-level behavior, making it suitable for both simulation-based validation and real hardware deployment.

---

## 🔐 Security Features

* **UART-based password authentication**
* **Password change via UART interface**
* **Lockout delay after multiple failed attempts**
* **Secure access before enabling system operation**

---

## 🧭 User Interface

* **16×2 LCD (4-bit mode)**
* **Matrix keypad for:**
    * Menu navigation
    * Temperature simulation
    * Threshold configuration
* **Menu-driven operation:**
    * Monitor Mode
    * Settings Mode

---

## 🌡️ Temperature Monitoring (Simulation-Aware Design)

* **Temperature value simulated using keypad + / - controls**
* **Adjustable temperature threshold via Settings menu**
* **Clear abstraction between sensor input and application logic**

This approach enables repeatable and deterministic testing of control logic and user interaction in simulation environments.

---

## 🌀 Fan / Motor Control (PWM)

* **PWM-based motor (fan) control using LPC2148 PWM module**
* **Motor turns ON automatically when temperature exceeds threshold**
* **Motor turns OFF when temperature returns to normal**
* **Safety shutdown ensures motor is stopped when exiting Monitor Mode**

---

## ⏱️ Over-Temperature Stopwatch

* **Tracks continuous time spent above temperature threshold**
* **Non-blocking implementation using loop tick counting**
* **Live duration updates via UART**
* **Timer resets automatically when temperature normalizes**

This models real-world thermal fault duration monitoring.

---

## 📡 UART Diagnostics

* **Authentication prompts and feedback**
* **Password change interface**
* **Monitor mode status messages**
* **Live over-temperature alert timing**

---

## 🧠 System Architecture

```text
+----------------------------+
|    Application Layer       |
|  Menus • Logic • Control   |
+-------------+--------------+
              |
+-------------v--------------+
|   Driver Abstraction Layer |
| LCD | Keypad | UART | PWM  |
| ADC (Hardware-Ready)       |
+-------------+--------------+
              |
+-------------v--------------+
|        LPC2148 MCU         |
+----------------------------+
```

---

## 🔌 Hardware Pinout

| Component | LPC2148 Pin | Function |
| :--- | :--- | :--- |
| **LCD Data** | `P0.4` – `P0.7` | 4-Bit Data Bus |
| **LCD Control** | `P0.0` (RS), `P0.2` (EN) | Command / Data Selection |
| **UART0** | `P0.8` (Tx), `P0.9` (Rx) | Serial Terminal (9600 Baud) |
| **PWM Output** | `P0.21` (PWM5) | Motor Driver Control (Fan) |
| **ADC Input** | `P0.28` (AD0.1) | LM35 Sensor Output |
| **Keypad Rows** | `P1.16` – `P1.19` | Matrix Output |
| **Keypad Columns** | `P1.20` – `P1.23` | Matrix Input |

---

## 🛠️ Build Instructions

### 1. Environment Setup
* **IDE:** Open the project in Keil uVision 4
* **Target Device:** Select NXP → LPC2148

### 2. Configuration
* **Clock Configuration:**
    * External Oscillator: 12 MHz
    * PLL configures System Clock to 60 MHz

### 3. Compilation
* **Action:** Click Rebuild
* **Expected:** 0 Errors, 0 Warnings

### 4. Load / Run
* Flash the generated `.hex` file to hardware
* Or load the `.hex` into the MCU properties in Proteus

---

## 📁 Project Structure

```text
smart-security-system/
│
├── firmware/
│   ├── smart_security.c
│   ├── system_init.c/.h
│   ├── lcd_driver.c/.h
│   ├── keypad_driver.c/.h
│   ├── uart_driver.c/.h
│   ├── adc_driver.c/.h
│   └── motor_driver.c/.h
│
├── docs/
│   └── smart_security_schematic.png
│
└── README.md
```

---

## ⚙️ Hardware Target Summary

* **MCU:** LPC2148 (ARM7TDMI-S)
* **System Clock:** 60 MHz
* **LCD:** HD44780 compatible (4-bit mode)
* **Input:** Matrix keypad
* **Motor:** DC motor via PWM
* **UART:** 9600 baud, 8N1
* **Sensor:** LM35 (hardware deployment)

---

## 👤 Author

**Vishnu Rach K R**

*Embedded Systems • ARM • Firmware Design*

