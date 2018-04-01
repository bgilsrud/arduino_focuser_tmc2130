/*
  Moonlite-compatible focuser controller

  ** Version 2.1 **
    Added function to set pin value.

  ** Version 2.0 **
    Modified to be used with the cheaper DRV8825 (or A4988) driver and almost completely rewritten by SquareBoot

  ** Version 1.0 **
    Inspired by (http://orlygoingthirty.blogspot.co.nz/2014/04/arduino-based-motor-focuser-controller.html)
    Modified for INDI, easydriver by Cees Lensink
    Added sleep function by Daniel Franzén
*/

// Firmware version - 2.1
const String VERSION = "21";

// Configuration
#include "Config.h"

// AVR libraries
#include <avr/io.h>                                       // AVR library
#include <avr/wdt.h>                                      // Watchdog timer
#define reboot() wdt_enable(WDTO_30MS); while(1) {}       // To reset the board, call reboot();

// Stepper driver libraries
// AccelStepper, used to provide motor acceleration
#include <AccelStepper.h>
// Driver-specific definitions
#include <BasicStepperDriver.h>
#if STEPPER_TYPE == STEPPER_A4988
#include <A4988.h>
#endif
#if STEPPER_TYPE == STEPPER_DRV8825
#include <DRV8825.h>
#endif
#if STEPPER_TYPE == STEPPER_DRV8834
#include <DRV8834.h>
#endif

#if STEPPER_TYPE == STEPPER_GENERIC
STEPPER_TYPE driver(STEPS_REV, DRIVER_DIR, DRIVER_STEP);
#elif STEPPER_TYPE == STEPPER_DRV8834
STEPPER_TYPE driver(STEPS_REV, DRIVER_DIR, DRIVER_STEP, DRIVER_M0, DRIVER_M1);
#else
STEPPER_TYPE driver(STEPS_REV, DRIVER1_DIR, DRIVER_STEP, DRIVER_M0, DRIVER_M1, DRIVER_M2);
#endif
// Motor control wrappers
// Forward step
void goForward() {
  driver.move(1);
}
// Backward step
void goBackward() {
  driver.move(-1);
}
// AccelStepper object allocation
AccelStepper stepper(goForward, goBackward);

//Define the period to wait before turning power off (in milliseconds)
#define TIMER_DELAY 30000

#define CMD_LENGHT 8

boolean isRunning = false;
boolean isPowerOn = false;

// Used to interpret the MoonLite protocol
char inChar;
char cmd[CMD_LENGHT];
char param[CMD_LENGHT];
char line[CMD_LENGHT];
boolean eoc = false;
int idx = 0;
long timerStartTime = 0;
char tempString[10];

void setup() {
  // Serial connection
  Serial.begin(SERIAL_SPEED);
  // Status LED
  pinMode(LED, OUTPUT);
  
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);

  // Motor driver setup
  driver.setMicrostep(MICROSTEPS);
  // Ignore Moonlite speed
  driver.setRPM(MOTOR_RPM);
  //stepper.setSpeed(MOTOR_PPS);
  //stepper.setMaxSpeed(MOTOR_PPS);
  stepper.setAcceleration(MOTOR_ACCEL);

  turnOff();

  memset(line, 0, CMD_LENGHT);
}

void loop() {
  // Only have to do this if stepper is on
  if (isRunning) {
    stepper.run();
    if (stepper.distanceToGo() == 0) {
      // Start timer to decide when to power off the board
      timerStartTime = millis();
      isRunning = false;
    }

  } else if (isPowerOn) {
    // Turn power off if active time period has passed
    if (millis() - timerStartTime > TIMER_DELAY) {
      turnOff();
    }
  }

  // Read the command until the terminating # character
  while (Serial.available() && !eoc) {
    inChar = Serial.read();
    if (inChar != '#' && inChar != ':') {
      line[idx++] = inChar;
      if (idx >= CMD_LENGHT) {
        idx = CMD_LENGHT - 1;
      }

    } else {
      if (inChar == '#') {
        eoc = true;
      }
    }
  }

  // We may not have a complete command set but there is no character coming in for now and might as well loop in case stepper needs updating
  // eoc will flag if a full command is there to act upon

  // Process the command we got
  if (eoc) {
    memset(cmd, 0, CMD_LENGHT);
    memset(param, 0, CMD_LENGHT);

    // Set the value of a pin
    if ((line[0] == 'A') && (line[1] == 'V')) {
      String pin = "";
      pin.concat(line[2]);
      pin.concat(line[3]);
      String value = "";
      value.concat(line[4]);
      value.concat(line[5]);
      value.concat(line[6]);
      analogWrite(pin.toInt(), value.toInt());
    }

    int len = strlen(line);
    if (len >= 2) {
      strncpy(cmd, line, 2);
    }
    if (len > 2) {
      strncpy(param, line + 2, len - 2);
    }

    memset(line, 0, CMD_LENGHT);
    eoc = false;
    idx = 0;

    // Execute the command

    if (!strcasecmp(cmd, "RS")) {
      reboot();
    }

    // Immediately stop any focus motor movement. Returns nothing
    if (!strcasecmp(cmd, "FQ")) {
      if (!isRunning) {
        turnOn();
      }
      // Stop as fast as possible: sets new target
      stepper.stop();
      stepper.runToPosition();
      // Now stopped after quickstop
    }

    // Go to the new position as set by the ":SNYYYY#" command. Returns nothing.
    // Turn stepper on and flag it is running
    // is this the only command that should actually make the stepper run ?
    if (!strcasecmp(cmd, "FG")) {
      if (!isRunning) {
        turnOn();
      }
    }

    // Returns the temperature coefficient where XX is a two-digit signed (2’s complement)  number.
    if (!strcasecmp(cmd, "GC")) {
      Serial.print("02#");
    }

    // Returns the current stepping delay where XX is a two-digit unsigned  number. See the :SD# command for a list of possible return values.
    // Might turn this into AccelStepper acceleration at some point
    if (!strcasecmp(cmd, "GD")) {
      Serial.print("02#");
    }

    // Returns "FF#" if the focus motor is half-stepped otherwise return "00#"
    if (!strcasecmp(cmd, "GH")) {
      Serial.print("00#");
    }

    // Returns "00#" if the focus motor is not moving, otherwise return "01#",
    // AccelStepper returns Positive as clockwise
    if (!strcasecmp(cmd, "GI")) {
      if (stepper.distanceToGo() == 0) {
        Serial.print("00#");

      } else {
        Serial.print("01#");
      }
    }

    // Returns the new position previously set by a ":SNYYYY" command where YYYY is a four-digit unsigned hex number.
    if (!strcasecmp(cmd, "GN")) {
      sprintf(tempString, "%04X", stepper.targetPosition());
      Serial.print(tempString);
      Serial.print("#");
    }

    // Returns the current position where YYYY is a four-digit unsigned hex number.
    if (!strcasecmp(cmd, "GP")) {
      sprintf(tempString, "%04X", stepper.currentPosition());
      Serial.print(tempString);
      Serial.print("#");
    }

    // Returns the current temperature where YYYY is a four-digit signed (2’s complement) hex number.
    if (!strcasecmp(cmd, "GT")) {
      Serial.print("0020#");
    }

    // Get the version of the firmware as a two-digit decimal number where the first digit is the major version number, and the second digit is the minor version number.
    if (!strcasecmp(cmd, "GV")) {
      Serial.print(VERSION + '#');
    }

    // Set the new temperature coefficient where XX is a two-digit, signed (2’s complement) hex number.
    if (!strcasecmp(cmd, "SC")) {

    }

    // Set the new stepping delay where XX is a two-digit,unsigned hex number.
    if (!strcasecmp(cmd, "SD")) {

    }

    // Set full-step mode.
    if (!strcasecmp(cmd, "SF")) {

    }

    // Set half-step mode.
    if (!strcasecmp(cmd, "SH")) {

    }

    // Set the new position where YYYY is a four-digit
    if (!strcasecmp(cmd, "SN")) {
      // stepper.enableOutputs(); // Turn the motor on here?
      if (!isRunning) {
        turnOn();
      }
      stepper.moveTo(hexToLong(param));
    }

    // Set the current position, where YYYY is a four-digit unsigned hex number.
    if (!strcasecmp(cmd, "SP")) {
      stepper.setCurrentPosition(hexToLong(param));
    }
  }
}

long hexToLong(char *line) {
  return strtol(line, NULL, 16);
}

void turnOn() {
  driver.enable();
  digitalWrite(LED, HIGH);
  isRunning = true;
  isPowerOn = true;
}

void turnOff() {
  driver.disable();
  digitalWrite(LED, LOW);
  isRunning = false;
  isPowerOn = false;
}
