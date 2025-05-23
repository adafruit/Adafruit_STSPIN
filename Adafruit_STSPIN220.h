/*!
 * @file Adafruit_STSPIN220.h
 *
 * This is a library for the STSPIN220 Low Voltage Stepper Motor Driver
 *
 * Designed specifically to work with the STSPIN220 chip, providing
 * Arduino Stepper library compatible interface for 2-pin control
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef _ADAFRUIT_STSPIN220_H_
#define _ADAFRUIT_STSPIN220_H_

#include "Arduino.h"

/*!
 * @brief Step mode enumeration for microstepping resolution
 */
typedef enum {
  STSPIN220_STEP_FULL = 0x00,      ///< Full step mode (MODE1=0,MODE2=0,MODE3=0,MODE4=0)
  STSPIN220_STEP_1_2 = 0x0A,       ///< 1/2 step mode (MODE1=1,MODE2=0,MODE3=1,MODE4=0)
  STSPIN220_STEP_1_4 = 0x05,       ///< 1/4 step mode (MODE1=0,MODE2=1,MODE3=0,MODE4=1)
  STSPIN220_STEP_1_8 = 0x0D,       ///< 1/8 step mode (MODE1=1,MODE2=1,MODE3=0,MODE4=1)
  STSPIN220_STEP_1_16 = 0x0F,      ///< 1/16 step mode (MODE1=1,MODE2=1,MODE3=1,MODE4=1)
  STSPIN220_STEP_1_32 = 0x01,      ///< 1/32 step mode (MODE1=0,MODE2=0,MODE3=0,MODE4=1)
  STSPIN220_STEP_1_64 = 0x07,      ///< 1/64 step mode (MODE1=1,MODE2=1,MODE3=0,MODE4=1)
  STSPIN220_STEP_1_128 = 0x02,     ///< 1/128 step mode (MODE1=0,MODE2=1,MODE3=0,MODE4=0)
  STSPIN220_STEP_1_256 = 0x03,   ///< 1/256 step mode (MODE1=1,MODE2=1,MODE3=0,MODE4=0)
} stspin220_step_mode_t;


/*! Timing characteristics defines */
#define STSPIN220_TOFF_MIN_US 9              ///< Minimum OFF time with ROFF=10kΩ (μs)
#define STSPIN220_TOFF_MAX_US 125            ///< Maximum OFF time with ROFF=160kΩ (μs)
#define STSPIN220_STCK_MIN_PULSE_NS 100      ///< Minimum STCK pulse width (ns)
#define STSPIN220_DIR_SETUP_TIME_NS 100      ///< DIR input setup time (ns)
#define STSPIN220_DIR_HOLD_TIME_NS 100       ///< DIR input hold time (ns)
#define STSPIN220_STCK_MAX_FREQ_MHZ 1        ///< Maximum STCK frequency (MHz)

/*!
 * @brief Class for interfacing with STSPIN220 stepper motor driver
 *        Provides Arduino Stepper library compatible interface
 */
class Adafruit_STSPIN220 {
public:
  Adafruit_STSPIN220(int number_of_steps, int step_pin, int dir_pin);
  Adafruit_STSPIN220(int number_of_steps, int step_pin, int dir_pin,
                     int mode1_pin, int mode2_pin, int en_fault_pin = -1, 
                     int stby_reset_pin = -1);
  
  // Arduino Stepper library compatible interface
  void setSpeed(long whatSpeed);
  void step(int steps_to_move);
  int version(void);
  
  // Extended STSPIN220 specific functionality
  bool setStepMode(stspin220_step_mode_t mode);
  stspin220_step_mode_t getStepMode();
  int microstepsPerStep();
  
  void singleStep();
  void stepBlocking(int steps, unsigned long delay_us = 1000);
  
  void enable(bool state = true);
  bool isEnabled();
  
  void standby(bool state = true);
  
  bool isFault();
  void clearFault();
  
  void reset();

private:
  // Arduino Stepper library compatible members
  unsigned long _step_delay;        ///< Delay between steps in microseconds
  int _number_of_steps;             ///< Total steps per revolution
  int _step_number;                 ///< Current position in steps
  unsigned long _last_step_time;    ///< Last step timestamp
  
  // STSPIN220 specific pins
  int _step_pin;                    ///< STEP/MODE3 pin (step clock)
  int _dir_pin;                     ///< DIR/MODE4 pin (direction)
  int _mode1_pin;                   ///< MODE1 pin (-1 if not used)
  int _mode2_pin;                   ///< MODE2 pin (-1 if not used)
  int _en_fault_pin;                ///< EN/FAULT pin (-1 if not used)
  int _stby_reset_pin;              ///< STBY/RESET pin (-1 if not used)
  
  // STSPIN220 state
  stspin220_step_mode_t _step_mode; ///< Current microstepping mode
  bool _enabled;                    ///< Power stage enable state
  
};

#endif // _ADAFRUIT_STSPIN220_H_
