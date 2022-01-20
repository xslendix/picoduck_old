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
        
        // DuckyScript+ Extensions
        if (ch == COMMAND_MOUSE_LEFT) { // FIXME: Left mouse button is not working at all
            int mode = 0;
            i++;
            ch = CUR_PROG[i];
            mode = ch;

            switch (mode) {
            case MOUSE_CLICK:
                Mouse.press(MOUSE_LEFT);
                Mouse.release(MOUSE_LEFT);
            case MOUSE_HOLD:
                Mouse.press(MOUSE_LEFT);
            case MOUSE_RELEASE:
                Mouse.release(MOUSE_LEFT);
            }
        }
        if (ch == COMMAND_MOUSE_MIDDLE) {
            int mode = 0;
            i++;
            ch = CUR_PROG[i];
            mode = ch;

            switch (mode) {
            case MOUSE_CLICK:
                Mouse.press(MOUSE_MIDDLE);
                Mouse.release(MOUSE_MIDDLE);
            case MOUSE_HOLD:
                Mouse.press(MOUSE_MIDDLE);
            case MOUSE_RELEASE:
                Mouse.release(MOUSE_MIDDLE);
            }
        }
        if (ch == COMMAND_MOUSE_RIGHT) {
            int mode = 0;
            i++;
            ch = CUR_PROG[i];
            mode = ch;

            switch (mode) {
            case MOUSE_CLICK:
                Mouse.press(MOUSE_RIGHT);
                Mouse.release(MOUSE_RIGHT);
            case MOUSE_HOLD:
                Mouse.press(MOUSE_RIGHT);
            case MOUSE_RELEASE:
                Mouse.release(MOUSE_RIGHT);
            }
        }
        if (ch == COMMAND_MOUSE_MOVE) {
            int x = 0;
            i++;
            ch = CUR_PROG[i];
            x = ch << 8;
            i++;
            ch = CUR_PROG[i];
            x |= ch;
            ch = CUR_PROG[i];

            int y = 0;
            i++;
            ch = CUR_PROG[i];
            y = ch << 8;
            i++;
            ch = CUR_PROG[i];
            y |= ch;
            ch = CUR_PROG[i];

            x -= 0xffff/2;
            y -= 0xffff/2;

            Mouse.move(x, y);
        }
        if (ch == COMMAND_MOUSE_WHEEL) {
            char amount = 0;
            i++;
            ch = CUR_PROG[i];
            amount = ch - 127;

            Mouse.move(0, 0, amount);
        }

        Keyboard.releaseAll();

        if (default_delay != 0)
            delay(default_delay);
    }
}
