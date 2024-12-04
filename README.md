# Health Indicator Using Cardiac and Thermal Measurements

This project aims to develop a portable system to measure a user's health status in real time. The device uses sensors to monitor heart rate (BPM) and body temperature, displaying the results on a touchscreen TFT display. In case of abnormal values, visual and Bluetooth alerts are triggered.

## Features

- **Real-Time Monitoring**:
  - Heart rate (BPM) via a pulse sensor.
  - Body temperature via an infrared sensor.
- **Data Display**:
  - A touchscreen TFT display shows the vital signs.
- **Automatic Alerts**:
  - Visual alert with an LED matrix (beating heart animation).
  - Bluetooth alert sent to a smartphone.
- **Compact and Portable**:
  - Ergonomic casing designed to fit on a finger.

## Hardware Architecture

- **TFT Display**: ILI9341 (240x320) with XPT2046 controller.
- **Temperature Sensor**: MLX90614.
- **Pulse Sensor**: Photodiode with amplified signal output.
- **LED Matrix**: 8x8 addressable LEDs (WS2812).
- **Bluetooth Module**: HC-05 for wireless communication.

## Requirements

- **Microcontroller**: STM32F103 (BluePill).
- **Necessary Software**:
  - STM32CubeIDE: for firmware development.
  - Altium Designer: for PCB design (optional).
  - Fusion 360: for 3D modeling of the casing.

## Installation and Compilation

**Clone the repository**:
   
   ```bash
   git clone https://github.com/Joliaus/DEEP-health-indicator.git
   cd DEEP-health-indicator
