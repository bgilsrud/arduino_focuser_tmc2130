// ----- General config -----
#ifndef CONFIG_H
#define CONFIG_H
// Status LED pin
#define LED 2
#define SERIAL_SPEED 115200
#define ENABLE_SW_RS false

// ----- Stepper driver configuration -----
#define STEPS_REV 2038
#define GEAR_RATIO (90/14)
// Motor driver type: BasicStepperDriver (0), DRV8825 (1), A4988 (2), DRV8834 (3), or TMC2130 (4)
#define STEPPER_TYPE 4
// Steps per revolution
#define SINGLE_STEP 1
// Ignored for BasicStepperDriver. Can be 1, 2, 4, 8, 16 or 32
#define FULL_STEP 1
// Ignored for BasicStepperDriver. Can be 1, 2, 4, 8, 16, 32, 64, 128, or 256
#define HALF_STEP 2
// Reverses the direction of the motor
#define REVERSE_DIR false
// RPM. Decreasing this value should decrease telescope shaking.
#define MOTOR_RPM 20
// Acceleration
#define MOTOR_ACCEL 100
// Motor pulses per second (pps). See full formula here: https://electronics.stackexchange.com/a/232679
// #define MOTOR_PPS (MOTOR_RPM * STEPS_REV / 60)
// ----- Driver pins -----
// Driver DIR pin
#define DRIVER_DIR PB2
// Driver STEP pin
#define DRIVER_STEP PB10
// Driver ENABLE pin. Comment to disable functionality
//#define DRIVER_EN 4
// M0. Only for microstepping-enabled drivers
#define MODE0 7
// M1. Only for microstepping-enabled drivers
#define MODE1 8
// M2. Only for microstepping-enabled drivers
#define MODE2 11

// ----- Polar finder light -----
#define ENABLE_POLAR_LIGHT true
#define POT_ANALOG_PIN 0
#define POLAR_LIGHT_LED 10

// ----- Customizable pins -----
// List of pins that can be modified by the serial port
#define CUSTOMIZABLE_PINS {5, 6, 9}

// Axis1 RA/Azm step/dir driver
#define Axis1_MOSI          PA7 //17
#define Axis1_SCK           PA5 //15     // SPI SCK
#define Axis1_CS            PA1 //11     // SPI CS
#define Axis1_MISO          PA6 //16     // SPI MISO
#define Axis1_STEP          PB10 //21     // Step
#define Axis1_DIR           PB2 //20     // Dir
#define Axis1_IRUN          200

#endif
