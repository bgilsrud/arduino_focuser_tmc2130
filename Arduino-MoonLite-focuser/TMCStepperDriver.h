/*
 * TMCStepperDriver - Trinamic TMCStepperDriver SPI driver
 *
 * Copyright (C)2015 Laurentiu Badea
 * Copyright (C)2022 Ben Gilsrud
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#ifndef TMCSTEPPERDRIVER_H
#define TMCSTEPPERDRIVER_H
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include "TMC_SPI.h"

class TMCStepperDriver : public BasicStepperDriver {
protected:
    short dir_pin = PIN_UNCONNECTED;
    short step_pin = PIN_UNCONNECTED;
    // tWH(STEP) pulse duration, STEP high, min value (1.9us)
    static const int step_high_min = 2;
    // tWL(STEP) pulse duration, STEP low, min value (1.9us)
    static const int step_low_min = 2;
    // tWAKE wakeup time, nSLEEP inactive to STEP (1000us)
    static const int wakeup_time = 1000;
    // also 200ns between ENBL/DIR/Mx changes and STEP HIGH

    // Get max microsteps supported by the device
    short getMaxMicrostep() override;

private:
    // microstep range (1, 16, 32 etc)
    static const short MAX_MICROSTEP = 256;
    tmcSpiDriver tmc_spi;

public:
    /*
     * Fully wired. All the necessary control pins for TMCStepperDriver are connected.
     */
    TMCStepperDriver(int steps, int dir_pin, int step_pin, int cs_pin, int sck_pin, int miso_pin, int mosi_pin);
    short setMicrostep(short microsteps) override;
    void setup(void);
    uint32_t status(void);
};
#endif // TMCSTEPPERDRIVER_H
