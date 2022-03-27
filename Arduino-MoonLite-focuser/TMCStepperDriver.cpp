/*
 * TMCStepperDriver

 * Copyright (C)2015 Laurentiu Badea
 * Copyright (C)2022 Ben Gilsrud
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "TMCStepperDriver.h"
#include "Config.h"
#include "TMC_SPI.h"

TMCStepperDriver::TMCStepperDriver(int steps, int dir_pin, int step_pin, int cs_pin, int sck_pin, int miso_pin, int mosi_pin)
    :BasicStepperDriver(steps, dir_pin, step_pin),tmc_spi(cs_pin, sck_pin, miso_pin, mosi_pin, TMC2130, AXIS1_DRIVER_RSENSE)
{
    //Serial.println("hello from tmcstepperdriver");
//./OnStep.ino:    tmcAxis3.setup(AXIS3_DRIVER_INTPOL,AXIS3_DRIVER_DECAY_MODE,AXIS3_DRIVER_CODE,axis3Settings.IRUN,axis3Settings.IRUN);

}

void TMCStepperDriver::setup()
{
    tmc_spi.setup(true, STEALTHCHOP, 1, Axis1_IRUN, Axis1_IRUN * .8);
}

/*
 * Set microstepping mode (1:divisor)
 * Allowed ranges for TMCStepperDriver are 1:1 to 1:32
 * If the control pins are not connected, we recalculate the timing only
 *
 */
short TMCStepperDriver::setMicrostep(short microsteps)
{
    /* FIXME */
    uint8_t chopconf;
    if (microsteps == 1)
        chopconf = 8;
    else if (microsteps == 2)
        chopconf = 7;
    else if (microsteps == 4)
        chopconf = 6;
    else if (microsteps == 8)
        chopconf = 5;
    else if (microsteps == 16)
        chopconf = 4;
    else if (microsteps == 32)
        chopconf = 3;
    else if (microsteps == 64)
        chopconf = 2;
    else if (microsteps == 128)
        chopconf = 1;
    else if (microsteps == 256)
        chopconf = 0;

    if (tmc_spi.refresh_CHOPCONF(chopconf))
        this->microsteps = microsteps;

    return this->microsteps;
}

short TMCStepperDriver::getMaxMicrostep(){
    return TMCStepperDriver::MAX_MICROSTEP;
}

uint32_t TMCStepperDriver::status(void)
{
    return tmc_spi.refresh_DRVSTATUS();
}
