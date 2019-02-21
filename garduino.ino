#include "src/App.h"

void setup() {  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Prepare garduino
  if (!APP.setup()) {
    Serial.print("App Setup failed");
  }

  Serial.print("Setup done");
}

void loop() {
  // Run infinite loop
  APP.run();
}
