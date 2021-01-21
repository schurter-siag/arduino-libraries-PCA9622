/*
 * Copyright (C) 2021 Daniel Guedel
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef PCA9622_H
#define PCA9622_H

#include <Wire.h>

// Register definitions (page 10, table 5)
#define REG_MODE1      0x00 // Mode register 1
#define REG_MODE2      0x01 // Mode register 2
#define REG_PWM0       0x02 // Brightness control LED0
#define REG_PWM1       0x03 // Brightness control LED1
#define REG_PWM2       0x04 // Brightness control LED2
#define REG_PWM3       0x05 // Brightness control LED3
#define REG_PWM4       0x06 // Brightness control LED4
#define REG_PWM5       0x07 // Brightness control LED5
#define REG_PWM6       0x08 // Brightness control LED6
#define REG_PWM7       0x09 // Brightness control LED7
#define REG_PWM8       0x0A // Brightness control LED8
#define REG_PWM9       0x0B // Brightness control LED9
#define REG_PWM10      0x0C // Brightness control LED10
#define REG_PWM11      0x0D // Brightness control LED11
#define REG_PWM12      0x0E // Brightness control LED12
#define REG_PWM13      0x0F // Brightness control LED13
#define REG_PWM14      0x10 // Brightness control LED14
#define REG_PWM15      0x11 // Brightness control LED15
#define REG_GRPPWM     0x12 // Group duty cycle control
#define REG_GRPFREQ    0x13 // Group frequency
#define REG_LEDOUT0    0x14 // LED output state 0
#define REG_LEDOUT1    0x15 // LED output state 1
#define REG_LEDOUT2    0x16 // LED output state 2
#define REG_LEDOUT3    0x17 // LED output state 3
#define REG_SUBADR1    0x18 // I2C-bus subaddress 1
#define REG_SUBADR2    0x19 // I2C-bus subaddress 2
#define REG_SUBADR3    0x1A // I2C-bus subaddress 3
#define REG_ALLCALLADR 0x1B // LED All Call I2C-bus address

// Mode register 1, MODE1 (page 11, table 6)
#define BIT_AI2     7 // 0: Register Auto-Increment disabled
                      // 1: Register Auto-Increment enabled
#define BIT_AI1     6 // 0: Auto-Increment bit 1 = 0
                      // 1: Auto-Increment bit 1 = 1
#define BIT_AI0     5 // 0: Auto-Increment bit 0 = 0
                      // 1: Auto-Increment bit 0 = 1
#define BIT_SLEEP   4 // 0: Normal mode
                      // 1: Low power mode. Oscillator off
#define BIT_SUB1    3 // 0: PCA9622 does not respond to I2C-bus subaddress 1
                      // 1: PCA9622 responds to I2C-bus subaddress 1
#define BIT_SUB2    2 // 0: PCA9622 does not respond to I2C-bus subaddress 2
                      // 1: PCA9622 responds to I2C-bus subaddress 2
#define BIT_SUB3    1 // 0: PCA9622 does not respond to I2C-bus subaddress 3
                      // 1: PCA9622 responds to I2C-bus subaddress 3
#define BIT_ALLCALL 0 // 0: PCA9622 does not respond to LED All Call I2C-bus address
                      // 1: PCA9622 responds to LED All Call I2C-bus address

// Auto-Increment options, AIn (page 9, table 4)
#define AI_DISABLED 0 // No Auto-Increment
#define AI_ALL      4 // Auto-Increment for all registers. D[4:0] roll over to ‘0 0000’ after the last register (1 1011) is accessed
#define AI_IND      5 // Auto-Increment for individual brightness registers only. D[4:0] roll over to ‘0 0010’ after the last register (1 0001) is accessed
#define AI_GBL      6 // Auto-Increment for global control registers only. D[4:0] roll over to ‘1 0010’ after the last register (1 0011) is accessed
#define AI_IND_GBL  7 // Auto-Increment for individual and global control registers only. D[4:0] roll over to ‘0 0010’ after the last register (1 0011) is accessed

// Mode register 2, MODE2 (page 11, table 7)
#define BIT_DMBLNK  5 // 0: Group control = dimming
                      // 1: Group control = blinking
//#define BIT_INVRT   4 // 0: Output logic state not inverted. Value to use when no external driver used
                      // 1: Output logic state inverted. Value to use when external driver used
#define BIT_INVRT   4 // 0: Reserved (write must always be a logic 0
#define BIT_OCH     3 // 0: Outputs change on STOP command
                      // 1: Outputs change on ACK
//#define BIT_OUTDRV  2 // 0: The 4 LED outputs are configured with an open-drain structure
                      // 1: The 4 LED outputs are configured with a totem pole structure
#define BIT_OUTDRV  2 // 1:Reserved (write must always be a logic 1
//#define BIT_OUTNE1  1 // 0: When OE = 1 (output drivers not enabled), LEDn = 0
                      // 1: When OE = 1 (output drivers not enabled): LEDn = 1 when OUTDRV = 1, LEDn = high-impedance when OUTDRV = 0 (same as OUTNE[1:0] = 10)
#define BIT_OUTNE1  1 // 0: Reserved (write must always be a logic 0
//#define BIT_OUTNE0  0 // 0: When OE = 1 (output drivers not enabled), LEDn = high-impedance
                      // 1: reserved
#define BIT_OUTNE0  0 // 1: Reserved (write must always be a logic 1

// Group control options, DMBLNK (page 11, table 7)
#define GROUP_CONTROL_MODE_DIMMING  0 // Group control = dimming
#define GROUP_CONTROL_MODE_BLINKING 1 // Group control = blinking

// PWM registers 0 to 3, PWMx

//Group duty cycle control, GRPPWM

// Group frequency, GRPFREQ
#define BLINKING_PERIOD_125_MS 3   //   3 =  125ms / (1 / 24Hz)
#define BLINKING_PERIOD_250_MS 6   //   6 =  250ms / (1 / 24Hz)
#define BLINKING_PERIOD_500_MS 12  //  12 =  500ms / (1 / 24Hz)
#define BLINKING_PERIOD_1_S    24  //  24 = 1000ms / (1 / 24Hz)
#define BLINKING_PERIOD_2_S    48  //  48 = 2000ms / (1 / 24Hz)
#define BLINKING_PERIOD_MAX    255 // 255 = 10.73s

#define BLINKING_RATIO_BALANCED 0.5

// LED driver output state, LEDOUT0 to LEDOUT3 (page 14, table 11)
#define BIT_LDR3  6 // LED3  output state control
#define BIT_LDR2  4 // LED2  output state control
#define BIT_LDR1  2 // LED1  output state control
#define BIT_LDR0  0 // LED0  output state control
#define BIT_LDR7  6 // LED7  output state control
#define BIT_LDR6  4 // LED6  output state control
#define BIT_LDR5  2 // LED5  output state control
#define BIT_LDR4  0 // LED4  output state control
#define BIT_LDR11 6 // LED11 output state control
#define BIT_LDR10 4 // LED10 output state control
#define BIT_LDR9  2 // LED9  output state control
#define BIT_LDR8  0 // LED8  output state control
#define BIT_LDR15 6 // LED15 output state control
#define BIT_LDR14 4 // LED14 output state control
#define BIT_LDR13 2 // LED13 output state control
#define BIT_LDR12 0 // LED12 output state control

// LED driver output state, LEDOUTn (page 14, below table 11)
#define LDR_STATE_OFF     0x00 // LED driver x is off (default power-up state)
#define LDR_STATE_ON      0x01 // LED driver x is fully on (individual brightness and group dimming/blinking not controlled)
#define LDR_STATE_IND     0x02 // LED driver x individual brightness can be controlled through its PWMx register
#define LDR_STATE_IND_GRP 0x03 // LED driver x individual brightness and group dimming/blinking can be controlled through its PWMx register and the GRPPWM registers

// I2C-bus subaddress 1 to 3, SUBADRx

// LED All Call I2C-bus address, ALLCALLADR

class PCA9622 {

/******************************* PUBLIC METHODS *******************************/
public:

    /**
     * Constructor for PCA9622 with RGB
     *
     * @param regRedPwm   Register address for red color channel
     * @param regGreenPwm Register address for green color channel
     * @param regBluePwm  Register address for blue color channel
     */
    PCA9622(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm);

    /**
     * Constructor for PCA9622 with RGBW
     *
     * @param regRedPwm   Register address for red color channel
     * @param regGreenPwm Register address for green color channel
     * @param regBluePwm  Register address for blue color channel
     * @param regWhitePwm Register address for white color channel
     */
    PCA9622(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm, uint8_t regWhitePwm);

    /**
     * Initialization of PCA9622
     * Clear Mode register 1 and 2
     *
     * @param deviceAddress I2C address of the PCA9622
     * @param wire          Reference to TwoWire for I2C communication
     */
    void begin(uint8_t deviceAddress, TwoWire *wire);

    /**
     * Switch to low-power mode. Oscillator off
     */
    void sleep();

    /**
     * Switch to normal mode
     */
    void wakeUp();

    /**
     * Turn on all LEDs. Restores settings saved at turnOff()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LDR_STATE_OFF!
     */
    void turnOn();

    /**
     * Turn off all LEDs. Saves current settings for turnOn()
     * For power saving, see sleep()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LDR_STATE_OFF!
     */
    void turnOff();

    /**
     * Set individual PWM value for a given channel
     *
     * @param regPwm Register address for PWM channel
     * @param pwm    PWM value
     */
    void setPwm(uint8_t regPwm, uint8_t pwm);

    /**
     * Set global PWM value for all channels
     *
     * @param pwm PWM value
     */
    void setGrpPwm(uint8_t pwm);

    /**
     * Set up values for blinking mode. Blinking mode needs to be activated
     * manually by calling setGroupControlMode(GROUP_CONTROL_MODE_BLINKING)
     *
     * @param blinkPeriod Period for one blink (turning off and on)
     * @param onOffRatio  Value between 0.0 and 1.0, where e.g. a value of
     *                    0.25 means 1/4 of the time the LEDs are on and
     *                    3/4 of the time the LEDs are off
     */
    void setBlinking(uint8_t blinkPeriod, float onOffRatio);

    /**
    * Set PWM values for RGB
    *
    * @param r Value for red color channel
    * @param g Value for green color channel
    * @param b Value for blue color channel
    */
    void setRGB(uint8_t r, uint8_t g, uint8_t b);

    /**
    * Set PWM values for RGBW. Only available when PCA9622 object was created
    * with the RGBW constructor
    *
    * @param r Value for red color channel
    * @param g Value for green color channel
    * @param b Value for blue color channel
    * @param w Value for white color channel
    */
    void setRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

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
    void setLdrState(uint8_t state, uint8_t regLedout, uint8_t ldrBit);

    /**
    * Set the LED driver output state for all channels. There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param state One of the four possible states
    */
    void setLdrStateAll(uint8_t state);

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
    void setAutoIncrement(uint8_t option);

    /**
    * Set the group control mode. There are two modes:
    *   - GROUP_CONTROL_MODE_BLINKING
    *   - GROUP_CONTROL_MODE_DIMMING
    *
    * @param mode   One of the two possible modes
    */
    void setGroupControlMode(uint8_t mode);

/****************************** PRIVATE METHODS *******************************/
private:

    /**
     * Mapping of different PWM channels to colors
     */
    uint8_t _regRedPwm, _regGreenPwm, _regBluePwm, _regWhitePwm;

    /**
     * Indicates whether PCA9622 was created with the RGBW constructor
     */
     bool _hasWhiteChannel;

    /**
     * Stored register content of LEDOUT when writing LDR_STATE_OFF to all LDRs
     * when calling turnOff()
     */
    uint8_t _storedRegLedout0;
    uint8_t _storedRegLedout1;
    uint8_t _storedRegLedout2;
    uint8_t _storedRegLedout3;

    /**
    * Write data to a register
    *
    * @param registerAddress Register address to write to
    * @param data            Data to write
    */
    void writeReg(uint8_t registerAddress, uint8_t data);

    /**
    * Read data from a register
    *
    * @param registerAddress Register address to read from
     *
     * @return byte read from given registerAddress
     * @return -1 if no byte was available to be read
    */
    uint8_t readReg(uint8_t registerAddress);

    /**
     * I2C address of device.
     */
    uint8_t _deviceAddress;

    /**
     * Object for I2C communication
     */
    TwoWire *_wire;
};
#endif //PCA9622_H