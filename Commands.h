#ifndef COMMANDS_H
#define COMMANDS_H

const char COMMAND_WRITE_CHARACTER   = '\x01';
const char COMMAND_ALT               = '\x02';
const char COMMAND_CONTROL           = '\x03';
const char COMMAND_CONTROL_ALT       = '\x04';
const char COMMAND_CONTROL_SHIFT     = '\x05';
const char COMMAND_DEFAULT_DELAY     = '\x06';
const char COMMAND_DELAY             = '\x07';
const char COMMAND_GUI               = '\x08';
const char COMMAND_ALT_SHIFT         = '\x09';
const char COMMAND_SHIFT             = '\x0A';

// DuckyScript+ Extenions
const char COMMAND_MOUSE_LEFT          = '\x0A';
const char COMMAND_MOUSE_MIDDLE        = '\x0B';
const char COMMAND_MOUSE_RIGHT         = '\x0C';
const char COMMAND_MOUSE_MOVE          = '\x0D';
const char COMMAND_MOUSE_WHEEL         = '\x0E';

// Mouse modes
const int MOUSE_CLICK   = 1;
const int MOUSE_HOLD    = 2;
const int MOUSE_RELEASE = 3;

// PicoDuck specific
const char PROGRAM_END               = '\xFE';

// TODO: Implement mouse commands

#endif // COMMANDS_H
