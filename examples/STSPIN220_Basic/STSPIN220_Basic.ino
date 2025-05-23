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
const int STEP_PIN = 2;     // Step clock pin
const int DIR_PIN = 3;      // Direction pin
const int MODE1_PIN = 4;    // Mode 1 pin (optional)
const int MODE2_PIN = 5;    // Mode 2 pin (optional)
const int EN_FAULT_PIN = 6; // Enable/Fault pin (optional)
const int STBY_RESET_PIN = 7; // Standby/Reset pin (optional)

// Create stepper object with full pin configuration
Adafruit_STSPIN220 myStepper(stepsPerRevolution, STEP_PIN, DIR_PIN, 
                             MODE1_PIN, MODE2_PIN, EN_FAULT_PIN, STBY_RESET_PIN);

// Alternative: Create stepper object with minimal pin configuration
// Adafruit_STSPIN220 myStepper(stepsPerRevolution, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("Adafruit STSPIN220 Stepper Motor Test");
  Serial.print("Library version: ");
  Serial.println(myStepper.version());
  
  // Set the speed to 60 RPM
  myStepper.setSpeed(60);
  
  // Set microstepping mode to 1/16 step for smoother operation
  myStepper.setStepMode(STSPIN220_STEP_1_16);
  
  Serial.println("Setup complete. Starting motor test...");
}

void loop() {
  Serial.println("Stepping forward one revolution...");
  myStepper.step(stepsPerRevolution);
  delay(1000);
  
  Serial.println("Stepping backward one revolution...");
  myStepper.step(-stepsPerRevolution);
  delay(1000);
  
  // Test different speeds
  Serial.println("Testing different speeds...");
  
  for (int speed = 10; speed <= 100; speed += 10) {
    Serial.print("Speed: ");
    Serial.print(speed);
    Serial.println(" RPM");
    
    myStepper.setSpeed(speed);
    myStepper.step(stepsPerRevolution / 4); // Quarter turn
    delay(500);
  }
  
  // Test different step modes
  Serial.println("Testing different step modes...");
  
  stspin220_step_mode_t modes[] = {
    STSPIN220_STEP_FULL,
    STSPIN220_STEP_1_2,
    STSPIN220_STEP_1_4,
    STSPIN220_STEP_1_8,
    STSPIN220_STEP_1_16,
    STSPIN220_STEP_1_32,
    STSPIN220_STEP_1_64_A,
    STSPIN220_STEP_1_128,
    STSPIN220_STEP_1_256_A
  };
  
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
  
  myStepper.setSpeed(30); // Slower speed for testing different modes
  
  for (int i = 0; i < 9; i++) {
    Serial.print("Testing ");
    Serial.println(modeNames[i]);
    
    myStepper.setStepMode(modes[i]);
    myStepper.step(stepsPerRevolution / 8); // 1/8 turn
    delay(1000);
  }
  
  // Test enable/disable functionality
  Serial.println("Testing enable/disable...");
  
  Serial.println("Disabling motor for 2 seconds...");
  myStepper.enable(false);
  delay(2000);
  
  Serial.println("Re-enabling motor...");
  myStepper.enable(true); // or just myStepper.enable() since default is true
  
  // Test standby functionality
  Serial.println("Testing standby mode...");
  
  Serial.println("Entering standby (ultra-low power) for 2 seconds...");
  myStepper.standby(true); // or just myStepper.standby() since default is true
  delay(2000);
  
  Serial.println("Waking up from standby...");
  myStepper.standby(false);
  
  Serial.println("Test cycle complete. Waiting 5 seconds before repeating...");
  delay(5000);
}