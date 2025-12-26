# Embedded Security System Project

## Project Description
The purpose of this project is to design and implement a small embedded security system 
using the TM4C123 LaunchPad and Grove sensors. The system resembles a basic home 
or room alarm that detects motion when locked and activates an alarm if an intruder is 
detected. Users can lock and unlock the system using a custom binary code entered 
through a pushbutton. The goal is to combine input sensors, GPIO functions, output sensors and 
display output into a meaningful real-world embedded system.

## How I built it
This project was developed using Code Composer Studio (CCS) and programmed for a TI Tiva C microcontroller. The system integrates a PIR motion sensor, buzzer, and seven-segment display, all interfaced through the microcontroller’s GPIO ports. The firmware was written primarily in C, with ARM assembly used for simpler non-leaf functions.

## What I Learned
Through this project, I gained hands-on experience with microcontroller programming, including:

- Configuring and controlling GPIO pins for sensor input and actuator output

- Writing embedded C firmware that directly interfaces with hardware

- Integrating ARM assembly with C for low-level function implementation

- Designing control logic suitable for real-time embedded systems

## How It Could Be Improved
The current implementation uses polling to monitor the PIR sensor. A major improvement would be transitioning to an interrupt-driven design, which would:

- Reduce unnecessary CPU usage

- Improve system responsiveness

- Lower overall power consumption

Some additional enhancements could include: power-saving sleep modes, allowing the user to select a unique password instead of giving them a predetermiend one, or expanding the unlock logic to support more complex state handling.

## Video
[Video Demonstration](https://drive.google.com/file/d/1nr90QWSlokFpup-CDFcgp4HEFQfnzJyF/view?usp=sharing)
