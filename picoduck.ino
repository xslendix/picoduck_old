#include <TinyUSB_Mouse_and_Keyboard.h>

#include "Programs.h"

const uint8_t HOLD_PIN = 14; // While this pin is high, the program wont start running.
const uint8_t SEQ_PIN  = 15; // If this pin is high, all programs are gonna run sequencially.

void setup() {
    pinMode(HOLD_PIN, INPUT);
    pinMode(SEQ_PIN, INPUT);
    InitProgramNumberPins();

    Keyboard.begin();
    delay(1000); // Wait for keyboard to be detected

    while (digitalRead(HOLD_PIN) == HIGH) delay(100);

    if (digitalRead(SEQ_PIN) == HIGH)
        for (uint8_t i = 0; i < 16; i++)
            RunProgramDataFromIndex(i);
    else
        RunProgramDataFromIndex(GetProgramIndexFromPins());

    Keyboard.releaseAll(); // Release all keys just in case
};

// Currently unused
void loop() { }
