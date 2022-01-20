#include <TinyUSB_Mouse_and_Keyboard.h>

#include "Programs.h"

const uint8_t HOLD_PIN = 0; // While this pin is high, the program wont start running.
const uint8_t SEQ_PIN  = 1; // If this pin is high, all programs are gonna run sequencially.
const uint8_t LED_PIN  = 25;

void setup()
{
    pinMode(HOLD_PIN, INPUT);
    pinMode(SEQ_PIN, INPUT);

    pinMode(LED_PIN, HIGH);
    InitProgramNumberPins();

    Keyboard.begin();
    Mouse.begin();

    delay(1000); // Wait for keyboard to be detected

    while (digitalRead(HOLD_PIN) == HIGH) delay(100);

    if (digitalRead(SEQ_PIN) == HIGH)
        for (uint8_t i = 0; i < 16; i++)
            RunProgramDataFromIndex(i);
    else
        RunProgramDataFromIndex(GetProgramIndexFromPins());

    Keyboard.releaseAll(); // Release all keys just in case

    digitalWrite(LED_PIN, HIGH);
};

// Currently unused
void loop() { }
