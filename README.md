# Project_Motion_Alert_System_for_Assistive_Safety
The solution implements a real-time motion detection and alert system using the FRDM-K66F development board with integrated FXOS8700CQ accelerometer. The system monitors tilt orientation, triggering alerts through RGB LED indicators and an external buzzer. 


# Author: Ekaterina Vislova
Date: 2025-11-30

# main.c - code for Motion Alert System for Assistive Safety Project for SEH600 course
Seneca Polytechnic, School of Software Design & Data Science
The FXOS8700 bubble example from the NXP SDK used as a reference for
-Device initialization with the correct I²C bus initialization sequence
-Driver functions for reading X, Y axis data from registers
-Converting 14-bit data into degrees using scaling formula
NXP Semiconductors, "Bubble Peripheral Demo Application — MCUXpresso SDK," MCUXpresso SDK Documentation, 2025. [Online].
Available: https://mcuxpresso.nxp.com/mcuxsdk/latest/html/examples/demo_apps/bubble_peripheral/readme.html

Subsystems,logic, hardware interactions modules developed independently
-All tilt multi-threshold classification
 Z-value interpretation logic for new orientation modes detection
-Buzzer alert control (initialization, configuration, sound pattern)
-LED control design ( busy-wait, assembly functions)

# function.s-Led control using Assembly
Assembly code for Motion Alert System for Assistive Safety Project
Code logic for register access is based on documentation for lab work for SEH600 course
Seneca Polytechnic, "SEH500 Lab 7," Seneca BSA GitHub Pages, 2025. [Online]. Available: https://seneca-bsa.github.io/bsa/seh500/lab7/
Prof.: Jacky Lau
Seneca Polytechnic, School of Software Design & Data Science

# Project_Motion_Alert_System_for_Assistive_Safety
The solution implements a real-time motion detection and alert system using the FRDM-K66F development board with integrated FXOS8700CQ accelerometer. The system monitors tilt orientation, triggering alerts through RGB LED indicators and an external buzzer. 


# Hardware Components
The system uses the following hardware, Figure 1., diagram based on the Kinetis K66 MCU [5]:
FRDM-K66F Development Board: Cortex-M4 processor 
FXOS8700CQ Sensor: Integrated accelerometer with I²C interface
RGB LED: Onboard tri-color LED for visual alerts
External Buzzer: 1273-074 DC 3-16V buzzer for auditory alerts
Breadboard and Jumper wires


Fig 1. Hardware Components Connection [5]
 
# Software Architecture

Motion Alert System for Assistive Safety - Class Diagram
demonstrates the code components used in main.c and function.s  files.

# Sensor Initialization and Data Acquisition
The system initializes the FXOS8700CQ accelerometer through I²C communication, configuring the sensor for continuous data acquisition. The FXOS8700 bubble example from the NXP SDK[6] used as a reference for device initialization with the correct I²C bus initialization sequence, driver functions for reading X, Y axis data from registers, converting 14-bit data into degrees using a scaling formula. Constantly monitored Sensor addresses use just 0x1CU, 0x1DU- Accelerometer addresses on SA0 pins, removing monitoring Magnetometer addresses for performance enhancement. Configuration for range, scaling follows the manufacturer's default specifications [3][4]. 
The core processing algorithm implements multi-threshold tilt classification using if conditions (Figure 3) . The classification logic processes X, Y, and Z-axis data to determine device orientation discussed in detail in the Results section. 


Fig 3. - Tilt classification using if conditions

# LED Control 
GPIO operations for LED control are implemented in ARM assembly for optimal performance
Most of the code logic in function.s file for register access is based on documentation for lab work for SEH600 course[8]. Code snippet Figure 5.1

Figure 5.1-  Assembly Code SnippetGPIO operations for LED control

# Active Buzzer Control
The internal oscillator generates the tone and Voltage level determines sound. The buzzer provides audible warnings when the board is tilted beyond 80 degrees on either axis. It produces a distinct double-beep pattern (500ms ON, 200ms OFF, repeated twice) to alert the user of potentially unsafe orientations with a more noticeable pattern than continuous beep. 
Hardware components connection described in Figure 1. 
ON/OFF control via GPIO, the buzzer is connected to Port B, Pin 5 [7] of the board as shown on Figures 5.2,6.


Fig 5.2.- Buzzer control via GPIO

Fig 6.- GPIO functions for buzzer control
4. Results
The motion alert system successfully demonstrated detection of various orientations (Figure 4)  with consistent alert triggering. Implemented the following alert hierarchy based on tilt classification:

Face-Up Horizontal (zAngle > 45°): White LED (all LEDs on)  (Figure 4.1)
Face-Down Horizontal (zAngle < -45°): Red LED + Buzzer (Figure 4.2)
Large Tilt (xAngle > 80° OR yAngle > 80°): Red LED + Buzzer (Figure 4.3, 4.4)
Moderate Tilt (xAngle > 45° OR yAngle > 45°): Blue LED  (Figure 4.5)
Minimum Tilt (xAngle > 15°): Green LED  (Figure 4.6 )

Fig 4 -  Detection of various orientations with FXOS8700CQ accelerometer.
