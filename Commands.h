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
const char COMMAND_STRING            = '\x0B';
const char COMMAND_STRING_DELAY      = '\x0C';
const char COMMAND_REPEAT            = '\x0D';

// TODO: Implement mouse commands

#endif // COMMANDS_H
