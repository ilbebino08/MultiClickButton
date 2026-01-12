/*
  TwoButtons.ino
  
  This example demonstrates using two MultiClickButton instances simultaneously.
  
  Hardware:
  - Connect button 1 between pin 2 and GND
  - Connect button 2 between pin 3 and GND
  - Connect LED to pin 13 (or use built-in LED)
  
  Button 1:
  - 1 Click:  Pause/Resume
  - 2 Clicks: Reset counter
  - 3 Clicks: Show status
  
  Button 2:
  - 1 Click:  Change mode (Normal/Fast/Debug)
  - 2 Clicks: Increment counter by 10
  - 3 Clicks: Total system reset
*/

#include <MultiClickButton.h>

// Create two buttons
MultiClickButton button1(2);
MultiClickButton button2(3);

// Program variables
int counter = 0;
int mode = 0;  // 0=Normal, 1=Fast, 2=Debug
const int LED_PIN = 13;

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    
    Serial.println("==========================================");
    Serial.println("Multi-Click Button - Two Buttons Example");
    Serial.println("==========================================");
    Serial.println("BUTTON 1 (Pin 2):");
    Serial.println("  1 Click  = Pause/Resume");
    Serial.println("  2 Clicks = Reset counter");
    Serial.println("  3 Clicks = Show status");
    Serial.println("");
    Serial.println("BUTTON 2 (Pin 3):");
    Serial.println("  1 Click  = Change mode");
    Serial.println("  2 Clicks = Increment +10");
    Serial.println("  3 Clicks = Total reset");
    Serial.println("==========================================");
    
    // Initialize buttons
    button1.begin();
    button2.begin();
    
    // Button 1 callbacks
    button1.onSingleClick(button1_Pause);
    button1.onDoubleClick(button1_Reset);
    button1.onTripleClick(button1_Status);
    
    // Button 2 callbacks
    button2.onSingleClick(button2_ChangeMode);
    button2.onDoubleClick(button2_Increment);
    button2.onTripleClick(button2_TotalReset);
}

void loop() {
    // Update both buttons
    button1.update();
    button2.update();
    
    // Main code (only if not paused)
    if (!button1.isPaused()) {
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate > 1000) {
            counter++;
            
            // Different behavior for each mode
            switch(mode) {
                case 0:  // Normal mode
                    Serial.print("[NORMAL] Counter: ");
                    Serial.println(counter);
                    digitalWrite(LED_PIN, LOW);
                    break;
                    
                case 1:  // Fast mode
                    Serial.print("[FAST] Counter: ");
                    Serial.println(counter);
                    digitalWrite(LED_PIN, HIGH);
                    break;
                    
                case 2:  // Debug mode
                    Serial.print("[DEBUG] Counter: ");
                    Serial.print(counter);
                    Serial.print(" | Uptime: ");
                    Serial.print(millis() / 1000);
                    Serial.println("s");
                    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
                    break;
            }
            
            lastUpdate = millis();
        }
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}

// ========== BUTTON 1 CALLBACKS ==========

void button1_Pause() {
    if (button1.isPaused()) {
        Serial.println("\n[BTN1] >>> PAUSED <<<\n");
    } else {
        Serial.println("\n[BTN1] >>> RESUMED <<<\n");
    }
}

void button1_Reset() {
    counter = 0;
    Serial.println("\n[BTN1] >>> COUNTER RESET <<<\n");
}

void button1_Status() {
    Serial.println("\n========== SYSTEM STATUS ==========");
    Serial.print("Counter: ");
    Serial.println(counter);
    Serial.print("Mode: ");
    switch(mode) {
        case 0: Serial.println("NORMAL"); break;
        case 1: Serial.println("FAST"); break;
        case 2: Serial.println("DEBUG"); break;
    }
    Serial.print("State: ");
    Serial.println(button1.isPaused() ? "PAUSED" : "ACTIVE");
    Serial.println("===================================\n");
}

// ========== BUTTON 2 CALLBACKS ==========

void button2_ChangeMode() {
    if (button2.isPaused()) {
        button2.setPaused(false);
    }
    
    mode = (mode + 1) % 3;
    
    Serial.print("\n[BTN2] >>> MODE: ");
    switch(mode) {
        case 0: Serial.println("NORMAL <<<\n"); break;
        case 1: Serial.println("FAST <<<\n"); break;
        case 2: Serial.println("DEBUG <<<\n"); break;
    }
}

void button2_Increment() {
    if (button2.isPaused()) {
        button2.setPaused(false);
    }
    
    counter += 10;
    Serial.print("\n[BTN2] >>> +10 | Counter: ");
    Serial.print(counter);
    Serial.println(" <<<\n");
}

void button2_TotalReset() {
    if (button2.isPaused()) {
        button2.setPaused(false);
    }
    
    counter = 0;
    mode = 0;
    button1.setPaused(false);
    digitalWrite(LED_PIN, LOW);
    
    Serial.println("\n[BTN2] >>> TOTAL RESET <<<");
    Serial.println("Counter = 0 | Mode = NORMAL | State = ACTIVE\n");
}