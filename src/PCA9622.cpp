/*
 * Copyright (C) 2021 Daniel Guedel
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "PCA9622.h"

/******************************* PUBLIC METHODS *******************************/


    /**
     * Constructor for PCA9622 with RGB
     *
     * @param regRedPwm   Register address for red color channel
     * @param regGreenPwm Register address for green color channel
     * @param regBluePwm  Register address for blue color channel
     */
PCA9622::PCA9622(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm) {

  _regRedPwm = regRedPwm;
  _regGreenPwm = regGreenPwm;
  _regBluePwm = regBluePwm;

  _hasWhiteChannel = false;
}

    /**
     * Constructor for PCA9622 with RGBW
     *
     * @param regRedPwm   Register address for red color channel
     * @param regGreenPwm Register address for green color channel
     * @param regBluePwm  Register address for blue color channel
     * @param regWhitePwm Register address for white color channel
     */
PCA9622::PCA9622(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm, uint8_t regWhitePwm)
       : PCA9622(regRedPwm, regGreenPwm, regBluePwm) {

  _regWhitePwm = regWhitePwm;

  _hasWhiteChannel = true;
}

    /**
     * Initialization of the PCA9622
     * Clear Mode register 1 and 2
     *
     * @param deviceAddress I2C address of the PCA9622
     * @param wire          Reference to TwoWire for I2C communication
     */
void PCA9622::begin(uint8_t deviceAddress, TwoWire *wire) {

  _deviceAddress = deviceAddress;

  _wire = wire;
  _wire->begin();

  writeReg(REG_MODE1, 0x0);
  writeReg(REG_MODE2, 0x0);
}

    /**
     * Switch to low-power mode. Oscillator off
     */
void PCA9622::sleep() {

  uint8_t prevReg = readReg(REG_MODE1);
  uint8_t newReg = prevReg | (1 << BIT_SLEEP);

  writeReg(REG_MODE1, newReg);
}

    /**
     * Switch to normal mode
     */
void PCA9622::wakeUp() {

  uint8_t prevReg = readReg(REG_MODE1);
  uint8_t newReg = prevReg & ~(1 << BIT_SLEEP);

  writeReg(REG_MODE1, newReg);
}

    /**
     * Turn on all LEDs. Restores settings saved at turnOff()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LDR_STATE_OFF!
     */
void PCA9622::turnOn() {

  writeReg(REG_LEDOUT0, _storedRegLedout0);
  writeReg(REG_LEDOUT1, _storedRegLedout1);
  writeReg(REG_LEDOUT2, _storedRegLedout2);
  writeReg(REG_LEDOUT3, _storedRegLedout3);
}

    /**
     * Turn off all LEDs. Saves current settings for turnOn()
     * For power saving, see sleep()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LDR_STATE_OFF!
     */
void PCA9622::turnOff() {

  _storedRegLedout0 = readReg(REG_LEDOUT0);
  writeReg(REG_LEDOUT0, LDR_STATE_OFF);
  _storedRegLedout1 = readReg(REG_LEDOUT1);
  writeReg(REG_LEDOUT1, LDR_STATE_OFF);
  _storedRegLedout2 = readReg(REG_LEDOUT2);
  writeReg(REG_LEDOUT2, LDR_STATE_OFF);
  _storedRegLedout3 = readReg(REG_LEDOUT3);
  writeReg(REG_LEDOUT3, LDR_STATE_OFF);
}

    /**
     * Set individual PWM value for a given channel
     *
     * @param regPwm Register address for PWM channel
     * @param pwm    PWM value
     */
void PCA9622::setPwm(uint8_t regPwm, uint8_t pwm) {

  writeReg(regPwm, pwm);
}

    /**
     * Set global PWM value for all channels
     *
     * @param pwm PWM value
     */
void PCA9622::setGrpPwm(uint8_t pwm) {

  writeReg(REG_GRPPWM, pwm);
}

    /**
     * Set up values for blinking mode. Blinking mode needs to be activated
     * manually by calling setGroupControlMode(GROUP_CONTROL_MODE_BLINKING)
     *
     * @param blinkPeriod Period for one blink (turning off and on)
     * @param onOffRatio  Value between 0.0 and 1.0, where e.g. a value of
     *                    0.25 means 1/4 of the time the LEDs are on and
     *                    3/4 of the time the LEDs are off
     */
void PCA9622::setBlinking(uint8_t blinkPeriod, float onOffRatio) {

  int16_t ratio = onOffRatio * 256;

  if (ratio < 0) {
    ratio = 0;
  }
  else if (ratio > 255) {
    ratio = 255;
  }

  writeReg(REG_GRPFREQ, blinkPeriod);
  writeReg(REG_GRPPWM, (uint8_t) ratio);
}

    /**
    * Set PWM values for RGB
    *
    * @param r Value for red color channel
    * @param g Value for green color channel
    * @param b Value for blue color channel
    */
void PCA9622::setRGB(uint8_t r, uint8_t g, uint8_t b) {

  setPwm(_regRedPwm, r);
  setPwm(_regGreenPwm, g);
  setPwm(_regBluePwm, b);
}

    /**
    * Set PWM values for RGBW. Only available when PCA9622 object was created
    * with the RGBW constructor
    *
    * @param r Value for red color channel
    * @param g Value for green color channel
    * @param b Value for blue color channel
    * @param w Value for white color channel
    */
void PCA9622::setRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {

  setRGB(r, g, b);

  if (_hasWhiteChannel) {
    setPwm(_regWhitePwm, w);
  }
}

    /**
    * Set the LED driver output state for a given channel. There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param state  One of the four possible states
    * @param ldrBit Lower bit of LDR* (see BIT_LDR*)
    */
void PCA9622::setLdrState(uint8_t state, uint8_t regLedout, uint8_t ldrBit) {

  uint8_t prevReg = readReg(regLedout);
  uint8_t newReg;

  newReg = prevReg & ~(0b11 << ldrBit);
  newReg |= (state << ldrBit);

  writeReg(regLedout, newReg);
}

    /**
    * Set the LED driver output state for all channels. There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param state One of the four possible states
    */
void PCA9622::setLdrStateAll(uint8_t state) {

  uint8_t newReg = ( state << BIT_LDR3
                   | state << BIT_LDR2
                   | state << BIT_LDR1
                   | state << BIT_LDR0);

  writeReg(REG_LEDOUT0, newReg);

  newReg = ( state << BIT_LDR7
           | state << BIT_LDR6
           | state << BIT_LDR5
           | state << BIT_LDR4);

  writeReg(REG_LEDOUT1, newReg);

  newReg = ( state << BIT_LDR11
           | state << BIT_LDR10
           | state << BIT_LDR9
           | state << BIT_LDR8);

  writeReg(REG_LEDOUT2, newReg);

  newReg = ( state << BIT_LDR15
           | state << BIT_LDR14
           | state << BIT_LDR13
           | state << BIT_LDR12);

  writeReg(REG_LEDOUT3, newReg);
}

    /**
    * Set an option for auto increment. There are five options:
    *   - AI_DISABLED
    *   - AI_ALL
    *   - AI_IND
    *   - AI_GBL
    *   - AI_IND_GBL
    *
    * @param option One of the possible five options
    */
void PCA9622::setAutoIncrement(uint8_t option) {

  uint8_t newReg;

  switch (option) {
    case AI_ALL:
      newReg = (1 << BIT_AI2)
             | (0 << BIT_AI1)
             | (0 << BIT_AI0);
      break;

    case AI_IND:
      newReg = (1 << BIT_AI2)
             | (1 << BIT_AI1)
             | (0 << BIT_AI0);
      break;

    case AI_GBL:
      newReg = (1 << BIT_AI2)
             | (0 << BIT_AI1)
             | (1 << BIT_AI0);
      break;

    case AI_IND_GBL:
      newReg = (1 << BIT_AI2)
             | (1 << BIT_AI1)
             | (1 << BIT_AI0);
      break;

    case AI_DISABLED:
    default:
      newReg = (0 << BIT_AI2)
             | (0 << BIT_AI1)
             | (0 << BIT_AI0);
      break;
  }

  writeReg(REG_MODE1, newReg);
}

    /**
    * Set the group control mode. There are two modes:
    *   - GROUP_CONTROL_MODE_BLINKING
    *   - GROUP_CONTROL_MODE_DIMMING
    *
    * @param mode   One of the two possible modes
    */
void PCA9622::setGroupControlMode(uint8_t mode) {

  uint8_t prevReg = readReg(REG_MODE2);

  switch (mode) {
    case GROUP_CONTROL_MODE_BLINKING:
      writeReg(REG_MODE2, prevReg | (1 << BIT_DMBLNK));
      break;

    case GROUP_CONTROL_MODE_DIMMING:
    default:
      writeReg(REG_MODE2, prevReg & ~(1 << BIT_DMBLNK));
      break;
  }
}

/****************************** PRIVATE METHODS *******************************/


    /**
    * Write data to a register
    *
    * @param registerAddress Register address to write to
    * @param data            Data to write
    */
void PCA9622::writeReg(uint8_t registerAddress, uint8_t data) {

  _wire->beginTransmission(_deviceAddress);
  _wire->write(registerAddress);
  _wire->write(data);
  _wire->endTransmission();
}

    /**
    * Read data from a register
    *
    * @param registerAddress Register address to read from
     *
     * @return byte read from given registerAddress
     * @return -1 if no byte was available to be read
    */
uint8_t PCA9622::readReg(uint8_t registerAddress) {

  _wire->beginTransmission(_deviceAddress);
  _wire->write(registerAddress);
  _wire->endTransmission();

  _wire->requestFrom(_deviceAddress, (uint8_t) 1);

  if (_wire->available() == 1) {
    return _wire->read();
  }

  return -1;
}