# Project_Motion_Alert_System_for_Assistive_Safety
The solution implements a real-time motion detection and alert system using the FRDM-K66F development board with integrated FXOS8700CQ accelerometer. The system monitors tilt orientation, triggering alerts through RGB LED indicators and an external buzzer. 


# Фuthor: Ekaterina Vislova
# Date: 2025-11-30

code for Motion Alert System for Assistive Safety Project for SEH600 course
Prof.: Jacky Lau
Seneca Polytechnic, School of Software Design & Data Science

# //The FXOS8700 bubble example from the NXP SDK used as a reference for
//-Device initialization with the correct I²C bus initialization sequence
//-Driver functions for reading X, Y axis data from registers
//-Converting 14-bit data into degrees using scaling formula
//NXP Semiconductors, "Bubble Peripheral Demo Application — MCUXpresso SDK," MCUXpresso SDK Documentation, 2025. [Online].
//Available: https://mcuxpresso.nxp.com/mcuxsdk/latest/html/examples/demo_apps/bubble_peripheral/readme.html

# //Subsystems,logic, hardware interactions modules developed independently
//-All tilt multi-threshold classification
// Z-value interpretation logic for new orientation modes detection
//-Buzzer alert control (initialization, configuration, sound pattern)
//-LED control design ( busy-wait, assembly functions)
