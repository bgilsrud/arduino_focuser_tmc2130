// Status LED pin
#define LED 13
#define SERIAL_SPEED 115200

//Stepper driver configuration

// Motor driver type: DRV8825, A4988, DRV8834 or BasicStepperDriver (generic)
#define STEPPER_TYPE DRV8825

// Steps per revolution
#define STEPS_REV 200

// RPM
// Maximum RPM value for NEMA 11 motors are about 400→420 rpm. Decreasing this value should decrease telescope shaking.
#define MOTOR_RPM 80
// Motor pulses per second (pps). See full formula here: https://electronics.stackexchange.com/a/232679
#define MOTOR_PPS (MOTOR_RPM * STEPS_REV / 60)
// Acceleration
#define MOTOR_ACCEL 50

// Pins
#define DRIVER_EN 2
#define DRIVER_DIR 3
#define DRIVER_STEP 4
#define DRIVER_M0 5
#define DRIVER_M1 6
#define DRIVER_M2 7

// Microsteps auto definitions - do not change
#if STEPPER_TYPE == STEPPER_GENERIC
#define MICROSTEPS 1
#elif STEPPER_TYPE == STEPPER_A4988
#define MICROSTEPS 16
#else
#define MICROSTEPS 32
#endif

// Other definitions used to automate driver handling
#define STEPPER_A4988 A4988
#define STEPPER_DRV8825 DRV8825
#define STEPPER_DRV8834 DRV8834
#define STEPPER_GENERIC BasicStepperDriver
