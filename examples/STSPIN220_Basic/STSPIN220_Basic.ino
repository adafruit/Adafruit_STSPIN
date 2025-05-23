/*!
 * @file STSPIN220_Basic.ino
 *
 * Basic example for the Adafruit STSPIN220 stepper motor driver library.
 * This example demonstrates how to use the STSPIN220 with Arduino Stepper 
 * library compatible interface.
 *
 * Connect:
 * - STEP pin to Arduino digital pin 2
 * - DIR pin to Arduino digital pin 3
 * - MODE1 pin to Arduino digital pin 4 (optional)
 * - MODE2 pin to Arduino digital pin 5 (optional)
 * - EN/FAULT pin to Arduino digital pin 6 (optional)
 * - STBY/RESET pin to Arduino digital pin 7 (optional)
 *
 * Make sure to connect proper power supply (1.8V-10V) to VS pin
 * and connect stepper motor to OUTA1, OUTA2, OUTB1, OUTB2 outputs.
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 */

#include <Adafruit_STSPIN220.h>

// Define the number of steps per revolution for your stepper motor
// Most steppers are 200 steps per revolution (1.8 degrees per step)
const int stepsPerRevolution = 200;

// Pin connections (adjust according to your wiring)
const int DIR_PIN = 2;      // Direction pin
const int STEP_PIN = 3;     // Step clock pin
const int MODE1_PIN = 4;    // Mode 1 pin (optional, but will let you set microstep mode)
const int MODE2_PIN = 5;    // Mode 2 pin (optional, but will let you set microstep mode)
const int EN_FAULT_PIN = 6; // Enable/Fault pin (optional)
const int STBY_RESET_PIN = 7; // Standby/Reset pin (optional, but will let you set microstep mode)

// Create stepper object with full pin configuration
Adafruit_STSPIN220 myStepper(stepsPerRevolution, STEP_PIN, DIR_PIN, 
                             MODE1_PIN, MODE2_PIN, EN_FAULT_PIN, STBY_RESET_PIN);

// Alternative: Create stepper object with minimal pin configuration, but we default to 1/16 microsteps!
// Adafruit_STSPIN220 myStepper(stepsPerRevolution, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial port to connect
  }
  
  Serial.println("Adafruit STSPIN220 Stepper Motor Test");delay(100);
  
  // Set the speed to 60 RPM
  myStepper.setSpeed(60);
  Serial.println("Set speed"); delay(100);
  
  // Set microstepping mode to 1/16 steps, this is also
  // the 'default' if we dont connect the mode/reset pins
  myStepper.setStepMode(STSPIN220_STEP_1_16);
  
  Serial.println("Setup complete. Starting motor test...");delay(100);
}

void loop() {
  // Calculate total microsteps for one full revolution
  int totalMicrosteps = stepsPerRevolution * myStepper.microstepsPerStep();
  
  Serial.print("Stepping forward one revolution (");
  Serial.print(totalMicrosteps);
  Serial.println(" microsteps)...");
  myStepper.step(totalMicrosteps);
  delay(1000);
  
  Serial.print("Stepping backward one revolution (");
  Serial.print(totalMicrosteps);
  Serial.println(" microsteps)...");
  myStepper.step(-totalMicrosteps);
  delay(1000);
}