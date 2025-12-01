//main.c
//Author: Ekaterina Vislova
//Date: 2025-11-30

//code for Motion Alert System for Assistive Safety Project for SEH600 course
//Prof.: Jacky Lau
//Seneca Polytechnic, School of Software Design & Data Science

//The FXOS8700 bubble example from the NXP SDK used as a reference for
//-Device initialization with the correct I²C bus initialization sequence
//-Driver functions for reading X, Y axis data from registers
//-Converting 14-bit data into degrees using scaling formula
//NXP Semiconductors, "Bubble Peripheral Demo Application — MCUXpresso SDK," MCUXpresso SDK Documentation, 2025. [Online].
//Available: https://mcuxpresso.nxp.com/mcuxsdk/latest/html/examples/demo_apps/bubble_peripheral/readme.html

//Subsystems,logic, hardware interactions modules developed independently
//-All tilt multi-threshold classification
// Z-value interpretation logic for new orientation modes detection
//-Buzzer alert control (initialization, configuration, sound pattern)
//-LED control design ( busy-wait, assembly functions)
#include <stdio.h>
#include "fsl_debug_console.h"
#include "math.h"
#include "fsl_fxos.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

// Buzzer pin definition
#define BUZZER_GPIO GPIOB//group of pins the buzzer belongs to
#define BUZZER_PIN 5U//pin number 5,PTB5,unsigned integer safer for bit shifting operations.
#define BUZZER_CLOCK kCLOCK_PortB
#define I2C_RELEASE_BUS_COUNT 100U
//Variables
volatile int16_t xAngle = 0;
volatile int16_t yAngle = 0;
volatile int16_t zAngle = 0;
// possible I2C addresses for the FXOS accelerometer(slave)
//Monitors the I2C bus continuously and responds only when its specific address is called
const uint8_t g_accel_address[] = {0x1CU, 0x1DU};//use just Accelerometer addresses on SA0 pin
// Master clears the bus
void BOARD_I2C_ReleaseBus(void);

// Assembly function declarations
void setup(void);
void func_led_on(void);
void func_led_off(void);
void set_red_led(void);
void set_blue_led(void);
// Buzzer (active type, with driver)
void soundBuzzer(void);


void initBuzzer(void) {
    gpio_pin_config_t buzzer_config = {
        kGPIO_DigitalOutput,
        0,
    };
    // Enable clock for buzzer port (for Port B that GPIOB controls)
    //CPU power PORTB internally before using the pins
    CLOCK_EnableClock(BUZZER_CLOCK);
    // Initialize buzzer pin as output,
    GPIO_PinInit(BUZZER_GPIO, BUZZER_PIN, &buzzer_config);
    // Ensure buzzer is initially off,only buzz when needed
    GPIO_PinWrite(BUZZER_GPIO, BUZZER_PIN, 0U);
}

//Creating distinct alarm pattern
//Active buzzer produce a constant tone, sound patterns are made by ON/OFF durations.
void soundBuzzer(void) {
    for (int i = 0; i < 2; i++) {//Repeats the entire pattern 2 times
        GPIO_PinWrite(BUZZER_GPIO, BUZZER_PIN, 1U);//3.3V = Sound ON
        delay_ms(500);//Turns the buzzer ON for 500 ms
        GPIO_PinWrite(BUZZER_GPIO, BUZZER_PIN, 0U);//Turns the buzzer OFF for 200 ms
        delay_ms(200);
    }
}


static void i2c_release_bus_delay(void){
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void delay_ms(uint32_t ms){
    for(uint32_t i = 0; i < ms * 1000; i++)
    {
        __NOP();
    }
}
//MASTER (Microcontroller) actions:
//Send START and slave address (write)
//Send register address to read from
//Send REPEATED START and slave address (read)
//Read data from slave

//Slave (Accelerometer) actions
//Responds only when addressed by master
//Uses master's SCL (does not generate clock)
//Acknowledges commands and data requests
void BOARD_I2C_ReleaseBus(void)// Master clears the bus
{
    uint8_t i = 0;

    BOARD_Init_I2C_GPIO_pins();
    //Drive SDA low first to simulate a start
    GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();
    //Send 9 pulses on SCL and keep SDA high
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 0U);
        i2c_release_bus_delay();
        GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 1U);
        i2c_release_bus_delay();
        GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    //Send stop
    GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 0U);
    i2c_release_bus_delay();
    GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();
    GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 1U);
    i2c_release_bus_delay();
    GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 1U);
    i2c_release_bus_delay();
}



int main(void)
{
    fxos_handle_t fxosHandle = {0};
    fxos_data_t sensorData   = {0};
    fxos_config_t config     = {0};
    uint8_t sensorRange      = 0;
    uint8_t dataScale        = 0;
    int16_t xData            = 0;
    int16_t yData            = 0;
    int16_t zData            = 0;
    uint8_t i                = 0;
    uint8_t array_addr_size  = 0;
    status_t result          = kStatus_Fail;

    // Board pin, master's clock system, debug console initialization
    BOARD_InitBootPins();
    BOARD_InitBootClocks();//SCL timing controlled by master's I2C peripheral
    BOARD_I2C_ReleaseBus();//Master clears the bus
    BOARD_I2C_ConfigurePins();//Master sets up I2C pins
    BOARD_InitDebugConsole();
    BOARD_InitPeripherals();

    // Call buzzer function for initialization
    initBuzzer();

    // Call assembly setup to initialize GPIOs for LEDs
    setup();

    // Configure the I2C function
    config.I2C_SendFunc    = BOARD_Accel_I2C_Send;// Master writes to slave
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;// Master reads from slave
    //Device Discovery
    array_addr_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);
    for (i = 0; i < array_addr_size; i++)//Master tries different slave addreses
    {
        config.slaveAddress = g_accel_address[i];
        // Initialize accelerometer sensor if found the accelerometer slave.
        result = FXOS_Init(&fxosHandle, &config);
        if (result == kStatus_Success){
            break;//kStatus_Success indicates slave acknowledged its address, register access
        }
    }
    if (result != kStatus_Success){
        PRINTF("\r\n Sensor Initialization failed\r\n");
        return -1;//No acknowledgment received during communication
    }

    //Slave response:
    //Acknowledges address match
    //provides requested register data
    //Get sensor range value
    //After each byte transmission, I2C hardware checks for acknowledgment from slave
    if (FXOS_ReadReg(&fxosHandle, XYZ_DATA_CFG_REG, &sensorRange, 1) != kStatus_Success){
        return -1;
    }
    if (sensorRange == 0x00){
        dataScale = 2U;
    }
    else if (sensorRange == 0x01){
        dataScale = 4U;// Set to ±4g range
    }
    else if (sensorRange == 0x10){
        dataScale = 8U;
    }
    PRINTF("\r\nYou will see the change of LED color based on board angle\r\n");

    //Main loop to get sensor data and call LEDs and buzzer
    while (1)
    {
        //Get new accelerometer data
    	//Sends data when clock pulses occur
        if (FXOS_ReadSensorData(&fxosHandle, &sensorData) != kStatus_Success){
            return -1;
        }

        //Get the X, Y, Z data from the sensor data structure in 14 bit left format data
        //Slave send MSB, LSB data
        /// 4U = Converts from 16-bit container to actual 14-bit sensor data

        xData = (int16_t)((uint16_t)((uint16_t)sensorData.accelXMSB << 8) | (uint16_t)sensorData.accelXLSB) / 4U;
        yData = (int16_t)((int16_t)((uint16_t)sensorData.accelYMSB << 8) | (uint16_t)sensorData.accelYLSB) / 4U;
        zData = (int16_t)((uint16_t)((uint16_t)sensorData.accelZMSB << 8) | (uint16_t)sensorData.accelZLSB) / 4U;

        //Convert raw data to angle (normalize to 0-90 degrees)
        //8192 = Maximum value representing ±2g (1g = 4096 counts)
        //8192 = 2^13(since 14-bit signed data)
        //This is the maximum positive value for the ±2g range
        //xData = raw acceleration value (-8192 to +8191)
        //* 90 = converts g-force to tilt angle (90° = 1g)
        xAngle = (int16_t)floor((double)xData * (double)dataScale * 90 / 8192);
        if (xAngle < 0) xAngle *= -1;// magnitude only
        yAngle = (int16_t)floor((double)yData * (double)dataScale * 90 / 8192);
        if (yAngle < 0) yAngle *= -1;// magnitude only
        //to preserve the sign of Z to distinguish face‑up and face‑down,do not take absolute value
        zAngle = (int16_t)floor((double)zData * (double)dataScale * 90 / 8192);

        //LED AND BUZZER CONTROL LOGIC
        //The order prevents overlap, if else stops further checks so we might not reach
        //Check flat orientation using Z
        //uses AND (&&) when both axis angles must be small to confirm the board is flat
        if (xAngle < 15 && yAngle < 15) {
            if (zAngle > 45) {
                // Face-up horizontal
                func_led_on(); // WHITE
                //PRINTF("Horizontal Face-Up: x=%d y=%d z=%d\r\n", xData, yData, zData);
                PRINTF("Horizontal Face-Up: x=%d y=%d z=%d\r\n", xAngle, yAngle, zAngle);
            } else if (zAngle < -45) {
                // Face-down horizontal
            	soundBuzzer();
                set_red_led(); // RED
                //PRINTF("Face-Down: x=%d y=%d z=%d\r\n", xData, yData, zData);
                PRINTF("Face-Down: x=%d y=%d z=%d\r\n", xAngle, yAngle, zAngle);
            } else {
                // In-between Z, treat as intermediate
            	func_led_off();
                PRINTF("Intermediate flat: x=%d y=%d z=%d\r\n", xAngle, yAngle, zAngle);
            }
        }
        // If not flat, check tilt thresholds
        // uses OR (||) when either axis exceeding a threshold is enough to trigger a tilt alert
        else if (xAngle > 80 || yAngle > 80) {
            soundBuzzer();
            set_red_led();
            PRINTF("Large Tilt: x=%d y=%d z=%d\r\n", xAngle, yAngle, zAngle);
        }
        else if (xAngle > 45 || yAngle > 45) {
            set_blue_led();
            PRINTF("Moderate Tilt: x=%d y=%d z=%d\r\n", xAngle, yAngle, zAngle);
        }
        else if (xAngle > 15 || yAngle > 15) {
             set_green_led();
            PRINTF("Minimum Tilt: x=%d y=%d z=%d\r\n", xAngle, yAngle, zAngle);
        }
        else {
            func_led_off();
            PRINTF("Intermediate Tilt: x=%d y=%d z=%d\r\n", xAngle, yAngle, zAngle);
        }

        delay_ms(500);
    }
}
