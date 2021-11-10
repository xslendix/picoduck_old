#include "Programs.h"

#include "Commands.h"

#include <Arduino.h>
#include <TinyUSB_Mouse_and_Keyboard.h>
#include <string.h>

void InitProgramNumberPins()
{
    pinMode(PIN_B0, INPUT);
    pinMode(PIN_B1, INPUT);
    pinMode(PIN_B2, INPUT);
    pinMode(PIN_B3, INPUT);
}

byte GetProgramIndexFromPins()
{
    byte final = 0;
    if (digitalRead(PIN_B0) == HIGH) final += 1;
    if (digitalRead(PIN_B1) == HIGH) final += 2;
    if (digitalRead(PIN_B2) == HIGH) final += 4;
    if (digitalRead(PIN_B3) == HIGH) final += 8;
    return final;
}

// TODO: Improve performace
size_t c_strlen(const char *str)
{
    const char *s;

    // FIXME: Possible endless loop
    for (s = str; *s != '\xFF'; ++s) ;

    return (s - str);
}

void RunProgramDataFromIndex(uint8_t progNum)
{
#define CUR_PROG _programs[progNum]

    int default_delay = 0;

    for (size_t i = 0; i < strlen(CUR_PROG); i++) {
        char ch = CUR_PROG[i];
        int new_delay = 0;
        size_t length, n = 0;
        char* location;

        switch (ch)
        {
            case COMMAND_WRITE_CHARACTER: {
                i++;
                ch = CUR_PROG[i];
                Keyboard.write(ch);
                break;
            }
            case COMMAND_ALT: {
                Keyboard.press(KEY_LEFT_ALT);
                i++;
                ch = CUR_PROG[i];
                Keyboard.write(ch);
                Keyboard.release(KEY_LEFT_ALT);
                break;
            }
            case COMMAND_CONTROL: {
                Keyboard.press(KEY_LEFT_CTRL);
                i++;
                ch = CUR_PROG[i];
                Keyboard.write(ch);
                Keyboard.release(KEY_LEFT_CTRL);
                break;
            }
            case COMMAND_CONTROL_ALT: {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_LEFT_ALT);
                i++;
                ch = CUR_PROG[i];
                Keyboard.write(ch);
                Keyboard.release(KEY_LEFT_CTRL);
                Keyboard.release(KEY_LEFT_ALT);
                break;
            }
            case COMMAND_DEFAULT_DELAY: {
                i++;
                ch = CUR_PROG[i];
                default_delay = ch >> 8;
                i++;
                ch = CUR_PROG[i];
                default_delay += ch;
                break;
            }
            case COMMAND_DELAY: {
                new_delay = 0;
                i++;
                ch = CUR_PROG[i];
                new_delay = ch >> 8;
                i++;
                ch = CUR_PROG[i];
                new_delay += ch;
                delay(new_delay);
            }
            case COMMAND_GUI: {
                Keyboard.press(KEY_LEFT_GUI);
                i++;
                ch = CUR_PROG[i];
                Keyboard.write(ch);
                Keyboard.release(KEY_LEFT_GUI);
            }
            case COMMAND_ALT_SHIFT: {
                Keyboard.press(KEY_LEFT_ALT);
                Keyboard.press(KEY_LEFT_SHIFT);
                Keyboard.release(KEY_LEFT_ALT);
                Keyboard.release(KEY_LEFT_SHIFT);
            }
            case COMMAND_SHIFT: {
                Keyboard.press(KEY_LEFT_SHIFT);
                i++;
                ch = CUR_PROG[i];
                Keyboard.write(ch);
                Keyboard.release(KEY_LEFT_SHIFT);
                break;
            }
            case COMMAND_STRING: {
                i++;
                location = (char*) &CUR_PROG[i];
                length = c_strlen(location);
                Keyboard.write((const uint8_t*)location, length);
                break;
            }
            case COMMAND_STRING_DELAY: {
                new_delay = 0;
                i++;
                ch = CUR_PROG[i];
                new_delay = ch >> 8;
                i++;
                ch = CUR_PROG[i];
                new_delay += ch;

                i++;
                location = (char*) &CUR_PROG[i];
                length = c_strlen(location);

                n = 0;
                while (length--) {
                    if (*location != '\r') {
                        if (Keyboard.write(*location))
                            n++;
                        else
                            break;
                    }
                    location++;
                    delay(new_delay);
                }

                Keyboard.write((const uint8_t*)location, length);
                break;
            }
            case COMMAND_REPEAT: {
                // TODO: Implement
                break;
            }
        }

        if (default_delay != 0)
            delay(default_delay);
    };
}
