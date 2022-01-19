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

int strlen_c(const char *str)
{
    int len = 0;
    for (int i = 0; str[i] != PROGRAM_END; i++)
        len++;

    return len;
}

void RunProgramDataFromIndex(uint8_t progNum)
{
#define CUR_PROG _programs[progNum]

    int default_delay = 0;
    int prog_length = strlen_c(CUR_PROG);

    for (int i = 0; i < prog_length; i++) {
        char ch = CUR_PROG[i];

        if (ch == COMMAND_WRITE_CHARACTER) {
            i++;
            ch = CUR_PROG[i];
            Keyboard.write(ch);
        }
        if (ch == COMMAND_ALT) {
            Keyboard.press(KEY_LEFT_ALT);
            delay(100);
            i++;
            ch = CUR_PROG[i];
            Keyboard.write(ch);
        }
        if (ch == COMMAND_CONTROL) {
            Keyboard.press(KEY_LEFT_CTRL);
            i++;
            ch = CUR_PROG[i];
            Keyboard.write(ch);
        }
        if (ch == COMMAND_CONTROL_ALT) {
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            delay(100);
            i++;
            ch = CUR_PROG[i];
            Keyboard.write(ch);
        }
        if (ch == COMMAND_DEFAULT_DELAY) {
            i++;
            ch = CUR_PROG[i];
            default_delay = ch << 8;
            i++;
            ch = CUR_PROG[i];
            default_delay |= ch;
        }
        if (ch == COMMAND_DELAY) {
            int new_delay = 0;
            i++;
            ch = CUR_PROG[i];
            new_delay = ch << 8;
            i++;
            ch = CUR_PROG[i];
            new_delay |= ch;
            ch = CUR_PROG[i];
            delay(new_delay);
        }
        if (ch == COMMAND_GUI) {
            Keyboard.press(KEY_LEFT_GUI);
            delay(100);
            i++;
            ch = CUR_PROG[i];
            Keyboard.write(ch);
        }
        if (ch == COMMAND_ALT_SHIFT) {
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_LEFT_SHIFT);
            delay(100);
        }
        if (ch == COMMAND_SHIFT) {
            Keyboard.press(KEY_LEFT_SHIFT);
            delay(100);
            i++;
            ch = CUR_PROG[i];
            Keyboard.write(ch);
        }

        Keyboard.releaseAll();

        if (default_delay != 0)
            delay(default_delay);
    }
}
