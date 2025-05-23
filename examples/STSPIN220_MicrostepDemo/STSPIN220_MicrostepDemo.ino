/*!
 * @file STSPIN220_MicrostepDemo.ino
 *
 * Microstepping demonstration for the Adafruit STSPIN220 stepper motor driver library.
 * This example steps through each available microstepping mode, performing one full
 * rotation forward and backward for each setting so you can compare the smoothness.
 *
 * Connect:
 * - DIR pin to Arduino digital pin 2
 * - STEP pin to Arduino digital pin 3
 * - MODE1 pin to Arduino digital pin 4 (required for mode switching)
 * - MODE2 pin to Arduino digital pin 5 (required for mode switching)
 * - EN/FAULT pin to Arduino digital pin 6 (optional)
 * - STBY/RESET pin to Arduino digital pin 7 (required for mode switching)
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

// Pin connections (all pins required for this demo)
const int DIR_PIN = 2;      // Direction pin
const int STEP_PIN = 3;     // Step clock pin
const int MODE1_PIN = 4;    // Mode 1 pin (required)
const int MODE2_PIN = 5;    // Mode 2 pin (required)
const int EN_FAULT_PIN = 6; // Enable/Fault pin (optional)
const int STBY_RESET_PIN = 7; // Standby/Reset pin (required)

// Create stepper object with full pin configuration
Adafruit_STSPIN220 myStepper(stepsPerRevolution, STEP_PIN, DIR_PIN, 
                             MODE1_PIN, MODE2_PIN, EN_FAULT_PIN, STBY_RESET_PIN);

// Array of all available step modes
stspin220_step_mode_t stepModes[] = {
  STSPIN220_STEP_FULL,
  STSPIN220_STEP_1_2,
  STSPIN220_STEP_1_4,
  STSPIN220_STEP_1_8,
  STSPIN220_STEP_1_16,
  STSPIN220_STEP_1_32,
  STSPIN220_STEP_1_64,
  STSPIN220_STEP_1_128,
  STSPIN220_STEP_1_256
};

// Human-readable names for each mode
const char* modeNames[] = {
  "Full step",
  "1/2 step",
  "1/4 step", 
  "1/8 step",
  "1/16 step",
  "1/32 step",
  "1/64 step",
  "1/128 step",
  "1/256 step"
};

const int numModes = sizeof(stepModes) / sizeof(stepModes[0]);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial port to connect
  }
  
  Serial.println("Adafruit STSPIN220 Microstepping Demo");
  Serial.println("=====================================");
  Serial.println();
  Serial.println("This demo will cycle through all microstepping modes,");
  Serial.println("performing one rotation forward and backward for each mode.");
  Serial.println("Watch and listen to compare the smoothness!");
  Serial.println();
  
  // Set a moderate speed for demonstration
  myStepper.setSpeed(30); // 30 RPM
  
  Serial.println("Starting demo in 3 seconds...");
  delay(3000);
}

void loop() {
  for (int i = 0; i < numModes; i++) {
    Serial.print("Mode ");
    Serial.print(i + 1);
    Serial.print("/");
    Serial.print(numModes);
    Serial.print(": ");
    Serial.print(modeNames[i]);
    Serial.print(" (");
    
    // Set the new step mode
    if (!myStepper.setStepMode(stepModes[i])) {
      Serial.println(" - FAILED to set mode!");
      continue;
    }
    
    // Reset speed after changing microstepping mode to maintain consistent RPM
    myStepper.setSpeed(30);
    
    int microsteps = myStepper.microstepsPerStep();
    Serial.print(microsteps);
    Serial.println(" microsteps per full step)");
    
    // Calculate total microsteps for one full revolution
    int totalMicrosteps = stepsPerRevolution * microsteps;
    
    Serial.print("  → Forward rotation (");
    Serial.print(totalMicrosteps);
    Serial.print(" microsteps)...");
    myStepper.step(totalMicrosteps);
    Serial.println(" done!");
    
    delay(1000); // Pause between directions
    
    Serial.print("  ← Backward rotation (");
    Serial.print(totalMicrosteps);
    Serial.print(" microsteps)...");
    myStepper.step(-totalMicrosteps);
    Serial.println(" done!");
    
    Serial.println();
    delay(500); // Pause between modes
  }
  
  Serial.println("Demo complete! Starting over in 5 seconds...");
  Serial.println("==============================================");
  Serial.println();
  delay(5000);
}