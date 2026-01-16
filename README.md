# Motion Alert System for Assistive Safety
- The solution implements a real-time motion detection and alert system using the FRDM-K66F development board with integrated FXOS8700CQ accelerometer.
- The system **monitors tilt orientation, triggering alerts through RGB LED indicators and an external buzzer.**

# Hardware Components
* The system uses the following hardware
* FRDM-K66F Development Board: Cortex-M4 processor 
* FXOS8700CQ Sensor: Integrated accelerometer with I²C interface
* RGB LED: Onboard tri-color LED for visual alerts
* External Buzzer: 1273-074 DC 3-16V buzzer for auditory alerts
* Breadboard and Jumper wires

# Software Architecture
Motion Alert System for Assistive Safety - Class Diagram
demonstrates the code components used in main.c and function.s  files.
<img width="1378" height="898" alt="Screenshot 2025-11-30 195319" src="https://github.com/user-attachments/assets/72e0fecc-9d73-4059-9cdd-329c04762510" />

# Accelerometer Readings and Data Acquisition
The system initializes the FXOS8700CQ accelerometer through I²C communication, configuring the sensor for continuous data acquisition. The FXOS8700 bubble example from the NXP SDK[6] used as a reference for device initialization with the correct I²C bus initialization sequence, driver functions for reading X, Y axis data from registers, converting 14-bit data into degrees using a scaling formula. Constantly monitored Sensor addresses use just 0x1CU, 0x1DU- Accelerometer addresses on SA0 pins, removing monitoring Magnetometer addresses for performance enhancement. 

The core processing algorithm implements multi-threshold tilt classification using if conditions . The classification logic processes X, Y, and Z-axis data to determine device orientation: 

* Face-Up Horizontal (zAngle > 45°): White LED (all LEDs on) 
* Face-Down Horizontal (zAngle < -45°): Red LED + Buzzer
* Large Tilt (xAngle > 80° OR yAngle > 80°): Red LED + Buzzer 
* Moderate Tilt (xAngle > 45° OR yAngle > 45°): Blue LED 
* Minimum Tilt (xAngle > 15°): Green LED  

# LED Control 
GPIO operations for LED control are implemented in **ARM assembly** for optimal performance
Most of the code logic in **function.s** file for register access is based on documentation for lab work for SEH600 course
Assembly code for Motion Alert System for Assistive Safety Project
Code logic for register access is based on documentation for lab work for SEH500 course
Seneca Polytechnic, "SEH500 Lab 7," Seneca BSA GitHub Pages, 2025. [Online]. Available: https://seneca-bsa.github.io/bsa/seh500/lab7/
Prof.: Jacky Lau
Seneca Polytechnic, School of Software Design & Data Science

# Active Buzzer Control
The internal oscillator generates the tone and Voltage level determines sound. **The buzzer provides audible warnings when the board is tilted beyond 80 degrees on either axis.** It produces a distinct double-beep pattern (500ms ON, 200ms OFF, repeated twice) to alert the user of potentially unsafe orientations with a more noticeable pattern than continuous beep. 
ON/OFF control via GPIO, the buzzer is connected to Port B, Pin 5




# References

The FXOS8700 bubble example from the NXP SDK used as a reference for
* Device initialization with the correct I²C bus initialization sequence
* Driver functions for reading X, Y axis data from registers
* Converting 14-bit data into degrees using scaling formula
NXP Semiconductors, "Bubble Peripheral Demo Application — MCUXpresso SDK," MCUXpresso SDK Documentation, 2025. [Online].
Available: https://mcuxpresso.nxp.com/mcuxsdk/latest/html/examples/demo_apps/bubble_peripheral/readme.html

Subsystems,logic, hardware interactions modules developed independently
* All tilt multi-threshold classification
* Z-value interpretation logic for new orientation modes detection
* Buzzer alert control (initialization, configuration, sound pattern)
* LED control design ( busy-wait, assembly functions)

- [3] T. Ozyagcilar, "Using the Xtrinsic FXOS8700CQ Transient-Acceleration Function," Application Note AN4461, Rev. 0, Freescale Semiconductor, Jun. 2012. [Online]. Available: https://www.nxp.com/docs/en/application-note/AN4461.pdf
- [4] NXP Semiconductors, "FXOS8700CQ: 6-axis sensor with integrated linear accelerometer and magnetometer, Rev. 8," Data Sheet: Technical Data, Apr. 25, 2017. [Online]. Available: https://www.nxp.com/docs/en/data-sheet/FXOS8700CQ.pdf
- [5] NXP Semiconductors, "FRDM-K66F: Low cost development platform for Kinetis K66, K65, and K26 MCUs," Mbed, Apr. 2019. [Online]. Available: https://os.mbed.com/platforms/FRDM-K66F/
- [6] NXP Semiconductors, "Bubble Peripheral Demo Application — MCUXpresso SDK," MCUXpresso SDK Documentation, 2025. [Online]. Available: https://mcuxpresso.nxp.com/mcuxsdk/latest/html/examples/demo_apps/bubble_peripheral/readme.html
- [7]NX P Semiconductors. "LPC GPIO." MCUXpresso SDK API Reference Manual. https://mcuxpresso.nxp.com/api_doc/dev/116/group__lpc__gpio.html(accessed Nov. 30, 2025)
- [8] Seneca College, "SEH500 Lab 7," Seneca BSA GitHub Pages, 2025. [Online]. Available: https://seneca-bsa.github.io/bsa/seh500/lab7/


