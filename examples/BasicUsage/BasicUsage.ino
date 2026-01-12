/*
  BasicUsage.ino
  
  This example demonstrates basic usage of the MultiClickButton library.
  
  Hardware:
  - Connect a push button between pin 2 and GND
  - No external resistor needed (uses INPUT_PULLUP)
  
  Behavior:
  - 1 Click:  Pause/Resume the counter
  - 2 Clicks: Reset the counter to 0
  - 3 Clicks: Show system status
*/

#include <MultiClickButton.h>

// Create button on pin 2
MultiClickButton button(2);

// Program variables
int counter = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("=================================");
    Serial.println("Multi-Click Button - Basic Usage");
    Serial.println("=================================");
    Serial.println("1 Click  = Pause/Resume");
    Serial.println("2 Clicks = Reset counter");
    Serial.println("3 Clicks = Show status");
    Serial.println("=================================");
    
    // Initialize button
    button.begin();
    
    // Attach callback functions
    button.onSingleClick(handlePause);
    button.onDoubleClick(resetCounter);
    button.onTripleClick(showStatus);
}

void loop() {
    // IMPORTANT: Always call update() in loop
    button.update();
    
    // Main code runs only when NOT paused
    if (!button.isPaused()) {
        // Increment counter every second
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate > 1000) {
            counter++;
            Serial.print("Counter: ");
            Serial.println(counter);
            lastUpdate = millis();
        }
    }
}

// Callback function for single click
void handlePause() {
    // Pause state is already toggled automatically
    if (button.isPaused()) {
        Serial.println("\n>>> SYSTEM PAUSED <<<");
        Serial.println("Press again to resume\n");
    } else {
        Serial.println("\n>>> SYSTEM RESUMED <<<\n");
    }
}

// Callback function for double click
void resetCounter() {
    counter = 0;
    Serial.println("\n>>> COUNTER RESET <<<\n");
}

// Callback function for triple click
void showStatus() {
    Serial.println("\n=== SYSTEM STATUS ===");
    Serial.print("Counter: ");
    Serial.println(counter);
    Serial.print("State: ");
    Serial.println(button.isPaused() ? "PAUSED" : "ACTIVE");
    Serial.println("=====================\n");
}