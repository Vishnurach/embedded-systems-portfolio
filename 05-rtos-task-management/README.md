AVR-uCOS-II-ADC-Logger

This repository demonstrates a RTOS design using µC/OS-II on an AVR microcontroller.

## Features
- Two ADC tasks sampling different channels
- Mutex-protected ADC hardware access
- Queue-based inter-task communication
- Centralized UART logging task
- Diagnostic counter for dropped samples
- Background LED blink task

## Design Notes
- ADC tasks never block on UART to preserve real-time behavior
- Samples are intentionally dropped if the queue is full
- UART access is centralized to avoid resource contention
- MISRA-C guidelines are followed with documented deviations

## Target
- MCU: AVR (ATmega series)
- RTOS: µC/OS-II
- Clock: 16 MHz

## Purpose
This project was built to understand RTOS task scheduling, synchronization, and real-time design trade-offs.
