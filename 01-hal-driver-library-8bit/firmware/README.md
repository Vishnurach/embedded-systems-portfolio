# PIC16F877A / PIC16F876A – Bare Metal HAL Driver Library (8-bit)

This project is a **bare-metal Hardware Abstraction Layer (HAL)** driver library
developed for **8-bit PIC microcontrollers** using the **XC8 toolchain**.

The goal of this repository is to demonstrate **clean driver design**, **hardware validation**, and **test-oriented development** for commonly used MCU peripherals.

---

## 🎯 Objectives

- Build **reusable, portable HAL drivers**
- Separate **hardware access** from **application logic**
- Validate each peripheral using **dedicated test harnesses**
- Maintain **clean repository structure** suitable for professional review

---

## 🧩 Supported Peripherals

| Peripheral | Description |
|----------|------------|
| LCD | HD44780 compatible LCD (4-bit / 8-bit modes) |
| UART | Polling-based serial communication |
| ADC | 10-bit ADC driver with channel selection |
| I²C | MSSP-based I²C Master (tested with DS1307 RTC) |
| SPI | Master & Slave modes (loopback + dual-chip demos) |
| PWM | CCP-based PWM output with configurable duty cycle |

---

## 📁 Repository Structure
firmware/
├── drivers/
│   ├── inc/        → Public HAL interfaces (.h)
│   └── src/        → Driver implementations (.c)
│
├── examples/       → Verified test harnesses
│
├── docs/           → Schematics & proof-of-function artifacts
│
├── main.c          → Central test workbench (preprocessor selectable)
└── config.h        → MCU configuration & clock settings

---

## 🧪 Test & Verification Strategy

Each driver is validated using a **dedicated test application** located in the `examples/` directory.

### Example Test Harnesses

- `lcd_demo.c` – LCD initialization and text display
- `uart_demo.c` – UART transmit verification
- `adc_demo.c` – ADC channel readout
- `pwm_demo.c` – PWM duty cycle control
- `i2c_demo.c` – DS1307 RTC read/write over I²C
- `spi_loopback_demo.c` – Single-chip SPI loopback test
- `spi_master_demo.c` – SPI master application
- `spi_slave_demo.c` – SPI slave application

All tests are supported by **schematics and screenshots** in the `docs/` folder.

---
## 🧪 How to Run (Test Workbench)

The project uses a **central test workbench** (`firmware/main.c`) to execute individual driver demos.

### Steps

1. Open `firmware/main.c`.
2. Enable the demo you want to test by uncommenting the corresponding macro:
   ```c
   // #define RUN_LCD_DEMO
   // #define RUN_UART_DEMO
   #define RUN_SPI_LOOPBACK   // Active Test
3. Compile the project and upload it to the PIC16F877A (or run the simulation in Proteus).

Note: Only one demo should be enabled at a time to avoid peripheral and resource conflicts.
---

## 🛠️ Design Principles Followed

- **HAL abstraction**: Applications never access registers directly
- **Single-responsibility drivers**
- **No magic numbers** (documented register settings)
- **Compile-time configuration**
- **Regression-safe refactoring** (single driver per peripheral)

---

## 🔧 Toolchain & Hardware

- MCU: PIC16F877A / PIC16F876A
- Compiler: XC8
- Clock: 4 MHz (XT)
- Simulation: Proteus
- Hardware: Breadboard / trainer-compatible

---

## 📌 Notes

- `main.c` acts as a **test workbench**, allowing one demo to be enabled at a time
- Each driver can be reused independently in other projects
- This repository is intended for **learning, validation, and portfolio demonstration**

---

## 👤 Author

**Vishnu Rach K R**  
Embedded Systems & Bare-Metal Development

---