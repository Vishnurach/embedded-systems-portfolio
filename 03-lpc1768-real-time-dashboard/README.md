# LPC1768 Real-Time Hardware Dashboard

**Target MCU:** LPC1768 (ARM Cortex-M3)

## 📌 Overview

The **LPC1768 Real-Time Hardware Dashboard** is a full-stack embedded systems project that bridges an **ARM Cortex-M3 microcontroller (LPC1768)** with a **Python-based desktop dashboard**.

The system demonstrates **professional bare-metal firmware architecture** using explicit clock ownership, **interrupt-driven non-blocking design**, modular driver abstraction, and **full-duplex UART communication**.  
It continuously samples temperature data from an **LM35 analog temperature sensor**, streams it to a PC in real time, and accepts control commands from the dashboard.

The project is fully **simulation-verified using Keil µVision** and can also run on real hardware.

---

## 📸 Project Demonstration

### 1. Full System Architecture (Keil + Python GUI)

The firmware runs in the Keil simulator with UART0 bridged to a **Virtual COM Port**.  
The Python dashboard communicates in **full-duplex mode**, enabling real-time monitoring and control.

> The `ASSIGN` command in Keil routes UART0 to the virtual serial interface used by the dashboard.

---

### 2. Live Temperature Monitoring

* Real-time ADC sampling via simulated Keil registers
* Temperature updates reflected instantly on the Python GUI

| Phase 1: Idle (30 °C) | Phase 3: High Temp (50 °C) |
| :---: | :---: |
| Simulated ADC Input | Real-time Dashboard Update |

---

## 🚀 Key Features

### Firmware (LPC1768)

* **Interrupt-Driven Architecture**
  * No blocking `while` loops for system timing
  * Timer0 ISR provides a 500 ms heartbeat
  * UART0 RX handled asynchronously via interrupts
* **Real-Time Sensor Monitoring**
  * Continuous ADC sampling (12-bit)
  * LM35 temperature conversion and formatting
* **Remote Hardware Control**
  * Bi-directional UART communication
  * PC dashboard controls on-board LED instantly
* **Modular HAL (Hardware Abstraction Layer)**
  * Clock, Timer, UART, and ADC split into reusable drivers
  * Clean separation between hardware logic and application code

---

## 🧭 PC Dashboard Features

* **Custom Desktop GUI**
  * Built using **Tkinter** and **PySerial**
  * Multi-threaded serial receive loop
* **Live Data Feed**
  * Temperature readings streamed in real time
  * Auto-scrolling log window
* **Remote Control**
  * GUI buttons send commands to control hardware LEDs
  * Immediate feedback from firmware

---

## 🧠 System Architecture

```text
+-----------------------------+
|     PC Dashboard (Python)   |
|  GUI • Serial • Threads    |
+--------------+--------------+
               |
        UART (Full-Duplex)
               |
+--------------v--------------+
|    Application Layer        |
|  main.c • Data Processing   |
+--------------+--------------+
               |
+--------------v--------------+
|   HAL / Driver Layer        |
| Clock | Timer | UART | ADC |
+--------------+--------------+
               |
+--------------v--------------+
|        LPC1768 MCU          |
|      ARM Cortex-M3          |
+-----------------------------+

## 🔌 Hardware Pin Configuration

| Component | LPC1768 Pin | Function | Description |
|--------|------------|----------|------------|
| **Heartbeat LED** | `P0.0` | GPIO Output | Toggles every 500 ms via Timer0 ISR |
| **User LED** | `P0.1` | GPIO Output | Controlled by Python Dashboard |
| **UART0 TX** | `P0.2` | Peripheral | Transmits sensor data to PC |
| **UART0 RX** | `P0.3` | Peripheral | Receives commands (interrupt-driven) |
| **LM35 Output** | `P0.23 (AD0.0)` | Analog Input | Temperature sensor (0–3.3 V) |

---

## ⏱️ Clock & Timing Configuration

* **External Crystal:** 12 MHz
* **PLL0 Output (Fcco):** 400 MHz
* **CPU Clock (CCLK):** 100 MHz
* **Peripheral Clock (PCLK):** 25 MHz
* **Flash Accelerator:** Configured for safe 100 MHz operation

Clock configuration is handled **explicitly** to ensure deterministic behavior across timers, UART, and ADC.

---

## 🌡️ ADC & Temperature Conversion

* **Resolution:** 12-bit (0–4095)
* **ADC Clock:** 5 MHz (safe margin below 13 MHz maximum)
* **Sensor:** LM35 (10 mV / °C)

Conversion logic:
```text
Voltage (mV) = (ADC_Value × 3300) / 4095
Temperature (°C) = Voltage / 10

## 📡 UART Communication Protocol

* **Baud Rate:** 9600 (8N1)
* **Transmit:** ASCII temperature strings
* **Receive Commands:**
  * `A` → Turn User LED ON
  * `B` → Turn User LED OFF

UART receive is **interrupt-driven**, while transmit uses polling for simplicity.

---

## 🧪 Validated Results (Simulation)

| Test Phase | ADC Input | Expected Temp | Result |
|----------|----------|---------------|--------|
| Baseline | 0.300 V | 30.0 °C | ✅ Confirmed |
| Medium | 0.350 V | 35.0 °C | ✅ Confirmed |
| High | 0.500 V | 50.0 °C | ✅ Confirmed |
| Low | 0.100 V | 10.0 °C | ✅ Confirmed |

* UART LED control commands successfully toggled hardware state
* Dashboard reflected status messages correctly

---

## 🛠️ Build & Run Instructions

### 1. Firmware (Keil µVision)

* **IDE:** Keil µVision 5
* **Target Device:** LPC1768
* **XTAL:** 12 MHz
* **Action:** Build (F7)
* **Run:** Hardware or Keil Simulator

---

### 2. Python Dashboard

```bash
pip install pyserial
python dashboard.py

Update serial port inside `dashboard.py`:
```python
SERIAL_PORT = "COM9"
BAUD_RATE = 9600

### 3. Simulation Mode (No Hardware Required)

* Use **com0com** to create virtual ports (e.g., COM8 ↔ COM9)
* Keil Debugger Command Window:
```text
MODE COM8 9600,0,8,1
ASSIGN COM8 <S0IN >S0OUT

Python dashboard connects to COM9

## 📁 Project Structure

```text
LPC1768-Real-Time-Dashboard/
│
├── Firmware/
│   ├── main.c              # Application entry point
│   ├── clock_config.c/.h   # PLL & clock tree setup
│   ├── timer.c/.h          # Timer0 heartbeat, Timer1 delay
│   ├── uart.c/.h           # UART0 driver (interrupt-driven RX)
│   └── adc.c/.h            # 12-bit ADC driver
│
├── Dashboard/
│   └── dashboard.py        # Python GUI
│
├── screenshots/            # Demo & simulation images
└── README.md

## 👤 Author

**Vishnu Rach K R**

*Embedded Systems • ARM Cortex-M • Bare-Metal Firmware • Hardware-to-Desktop Integration*
