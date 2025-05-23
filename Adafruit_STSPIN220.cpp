/*!
 * @file Adafruit_STSPIN220.cpp
 *
 * @mainpage Adafruit STSPIN220 Stepper Motor Driver Library
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for Adafruit's STSPIN220 driver for the
 * Arduino platform. It is designed specifically to work with the
 * STSPIN220 Low Voltage Stepper Motor Driver chip.
 *
 * The STSPIN220 is a stepper motor driver integrating a microstepping
 * sequencer (up to 1/256th of a step), two PWM current controllers and
 * a power stage composed of two fully-protected full-bridges.
 *
 * @section dependencies Dependencies
 *
 * This library depends on Arduino.h
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "Adafruit_STSPIN220.h"

/*!
 * @brief Construct a new Adafruit_STSPIN220 object with minimal pin configuration
 * @param number_of_steps Number of steps per full revolution
 * @param step_pin Arduino pin connected to STEP (step clock) input
 * @param dir_pin Arduino pin connected to DIR (direction) input
 */
Adafruit_STSPIN220::Adafruit_STSPIN220(int number_of_steps, int step_pin, int dir_pin) {
  _number_of_steps = number_of_steps;
  _step_pin = step_pin;
  _dir_pin = dir_pin;
  _mode1_pin = -1;
  _mode2_pin = -1;
  _en_fault_pin = -1;
  _stby_reset_pin = -1;
  
  _step_delay = 1000;
  _step_number = 0;
  _last_step_time = 0;
  _step_mode = STSPIN220_STEP_1_16;
  _enabled = true;
  
  pinMode(_step_pin, OUTPUT);
  digitalWrite(_step_pin, HIGH);
  pinMode(_dir_pin, OUTPUT);
  digitalWrite(_dir_pin, HIGH);
}

/*!
 * @brief Construct a new Adafruit_STSPIN220 object with full pin configuration
 * @param number_of_steps Number of steps per full revolution
 * @param step_pin Arduino pin connected to STEP (step clock) input
 * @param dir_pin Arduino pin connected to DIR (direction) input
 * @param mode1_pin Arduino pin connected to MODE1 input
 * @param mode2_pin Arduino pin connected to MODE2 input
 * @param en_fault_pin Arduino pin connected to EN/FAULT pin (optional, -1 if not used)
 * @param stby_reset_pin Arduino pin connected to STBY/RESET pin (optional, -1 if not used)
 */
Adafruit_STSPIN220::Adafruit_STSPIN220(int number_of_steps, int step_pin, int dir_pin,
                                       int mode1_pin, int mode2_pin, int en_fault_pin,
                                       int stby_reset_pin) {
  _number_of_steps = number_of_steps;
  _step_pin = step_pin;
  _dir_pin = dir_pin;
  _mode1_pin = mode1_pin;
  _mode2_pin = mode2_pin;
  _en_fault_pin = en_fault_pin;
  _stby_reset_pin = stby_reset_pin;
  
  _step_delay = 1000;
  _step_number = 0;
  _last_step_time = 0;
  _step_mode = STSPIN220_STEP_1_16;
  _enabled = true;
  
  pinMode(_step_pin, OUTPUT);
  digitalWrite(_step_pin, HIGH);
  pinMode(_dir_pin, OUTPUT);
  digitalWrite(_dir_pin, HIGH);
  pinMode(_mode1_pin, OUTPUT);
  digitalWrite(_mode1_pin, HIGH);
  pinMode(_mode2_pin, OUTPUT);
  digitalWrite(_mode2_pin, HIGH);
  
  if (_en_fault_pin != -1) {
    pinMode(_en_fault_pin, INPUT_PULLUP);
  }
  
  if (_stby_reset_pin != -1) {
    pinMode(_stby_reset_pin, OUTPUT);
    digitalWrite(_stby_reset_pin, HIGH);
  }
  
  if (_mode1_pin != -1 && _mode2_pin != -1) {
    uint8_t mode_bits = (uint8_t)_step_mode;
    digitalWrite(_mode1_pin, mode_bits & 0x01);
    digitalWrite(_mode2_pin, mode_bits & 0x02);
  }
}

/*!
 * @brief Set the speed in revolutions per minute (Arduino Stepper compatible)
 * @param whatSpeed Speed in RPM
 */
void Adafruit_STSPIN220::setSpeed(long whatSpeed) {
  if (whatSpeed <= 0) {
    _step_delay = 1000000;
  } else {
    // Account for microstepping - more microsteps means shorter delay per step
    int microsteps = microstepsPerStep();
    _step_delay = ((60L * 1000L * 1000L) / (_number_of_steps * microsteps)) / whatSpeed;
  }
}

/*!
 * @brief Move the motor a specified number of steps (Arduino Stepper compatible)
 * @param steps_to_move Number of steps to move (positive = forward, negative = reverse)
 */
void Adafruit_STSPIN220::step(int steps_to_move) {
  int steps_left = abs(steps_to_move);
  
  digitalWrite(_dir_pin, steps_to_move > 0);
  delayMicroseconds(1);
  
  while (steps_left > 0) {
    unsigned long now = micros();
    
    if (now - _last_step_time >= _step_delay) {
      singleStep();
      
      if (steps_to_move > 0) {
        _step_number++;
        if (_step_number == _number_of_steps) {
          _step_number = 0;
        }
      } else {
        if (_step_number == 0) {
          _step_number = _number_of_steps;
        }
        _step_number--;
      }
      
      steps_left--;
      _last_step_time = now;
    }
  }
}

/*!
 * @brief Return the library version number (Arduino Stepper compatible)
 * @return Version number
 */
int Adafruit_STSPIN220::version(void) {
  return 220;
}

/*!
 * @brief Set the microstepping mode
 * @param mode The desired step mode from stspin220_step_mode_t
 * @return True if mode was set successfully, false if pins not available
 */
bool Adafruit_STSPIN220::setStepMode(stspin220_step_mode_t mode) {
  if (_stby_reset_pin == -1) return false;
  
  uint8_t mode_bits = (uint8_t)mode;
  
  // Check if we can set this mode with available pins
  if ((_mode1_pin == -1) || (_mode2_pin == -1)) {
    // If mode1/mode2 pins not available, only allow modes where those bits are high (pulled up)
    if ((mode_bits & 0x01) == 0 || (mode_bits & 0x02) == 0) {
      return false; // Mode requires low bits on unavailable pins
    }
  }
  
  Serial.println("reset"); delay(100);
  // Put device into standby/reset
  digitalWrite(_stby_reset_pin, LOW);

  delay(1);
  
  // Set all available mode pins (MODE1, MODE2, STEP/MODE3, DIR/MODE4) 
  if (_mode1_pin != -1) {
    digitalWrite(_mode1_pin, mode_bits & 0x01);
  }
  if (_mode2_pin != -1) {
    digitalWrite(_mode2_pin, mode_bits & 0x02);
  }
  digitalWrite(_step_pin, mode_bits & 0x04);
  digitalWrite(_dir_pin, mode_bits & 0x08);
  
  // Come out of standby to latch the mode
  digitalWrite(_stby_reset_pin, HIGH);
  
  _step_mode = mode;
  return true;
}

/*!
 * @brief Get the current microstepping mode
 * @return Current step mode
 */
stspin220_step_mode_t Adafruit_STSPIN220::getStepMode() {
  return _step_mode;
}

/*!
 * @brief Get the number of microsteps per full step for current mode
 * @return Microsteps per full step (1, 2, 4, 8, 16, 32, 64, 128, or 256)
 */
int Adafruit_STSPIN220::microstepsPerStep() {
  switch (_step_mode) {
    case STSPIN220_STEP_FULL:     return 1;
    case STSPIN220_STEP_1_2:      return 2;
    case STSPIN220_STEP_1_4:      return 4;
    case STSPIN220_STEP_1_8:      return 8;
    case STSPIN220_STEP_1_16:     return 16;
    case STSPIN220_STEP_1_32:     return 32;
    case STSPIN220_STEP_1_64:     return 64;
    case STSPIN220_STEP_1_128:    return 128;
    case STSPIN220_STEP_1_256:    return 256;
    default:                      return 16; // Default to 1/16 step
  }
}

/*!
 * @brief Perform a single step pulse
 */
void Adafruit_STSPIN220::singleStep() {
  digitalWrite(_step_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(_step_pin, HIGH);
}

/*!
 * @brief Move the motor with blocking delay between steps
 * @param steps Number of steps to move (positive = forward, negative = reverse)
 * @param delay_us Delay between steps in microseconds
 */
void Adafruit_STSPIN220::stepBlocking(int steps, unsigned long delay_us) {
  int steps_left = abs(steps);
  
  digitalWrite(_dir_pin, steps > 0);
  delayMicroseconds(1);
  
  for (int i = 0; i < steps_left; i++) {
    singleStep();
    delayMicroseconds(delay_us);
  }
}

/*!
 * @brief Enable or disable the motor power stage
 * @param state True to enable, false to disable (default: true)
 */
void Adafruit_STSPIN220::enable(bool state) {
  if (_en_fault_pin == -1) return;
  
  if (state) {
    pinMode(_en_fault_pin, INPUT_PULLUP);
  } else {
    pinMode(_en_fault_pin, OUTPUT);
    digitalWrite(_en_fault_pin, LOW);
  }
  
  _enabled = state;
}

/*!
 * @brief Check if the motor power stage is enabled
 * @return True if enabled, false if disabled
 */
bool Adafruit_STSPIN220::isEnabled() {
  if (_en_fault_pin == -1) return true; // If no enable pin connected, assume enabled
  
  return _enabled;
}

/*!
 * @brief Put the device into standby mode or wake it up
 * @param state True to enter standby (ultra-low power), false to wake up (default: true)
 */
void Adafruit_STSPIN220::standby(bool state) {
  if (_stby_reset_pin == -1) return;
  
  if (state) {
    // Going into standby/reset - just set the pin low
    digitalWrite(_stby_reset_pin, LOW);
  } else {
    // Coming out of standby/reset - restore the current step mode
    setStepMode(_step_mode);
  }
}

/*!
 * @brief Check if a fault condition exists
 * @return True if fault detected, false if normal operation
 */
bool Adafruit_STSPIN220::isFault() {
  if (_en_fault_pin == -1) return false;
  
  return !digitalRead(_en_fault_pin);
}

/*!
 * @brief Clear fault condition by toggling enable pin
 */
void Adafruit_STSPIN220::clearFault() {
  if (_en_fault_pin == -1) return;
  
  pinMode(_en_fault_pin, OUTPUT);
  digitalWrite(_en_fault_pin, LOW);
  delay(1);
  pinMode(_en_fault_pin, INPUT_PULLUP);
  _enabled = true;
}

/*!
 * @brief Reset the device by toggling the STBY/RESET pin
 */
void Adafruit_STSPIN220::reset() {
  if (_stby_reset_pin == -1) return;
  
  standby(true);
  delay(1);
  standby(false);
}




