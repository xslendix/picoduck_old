#!/usr/bin/env python3

import os

print("DuckyScript to PicoDuck converter")
print('Please paste your script here, at the end, type "EOF":')
lines = ['']

while lines[-1].strip() != 'EOF':
    lines.append(input())

lines = lines[1:-1]

print('Parsing...')

final = b''

# Parse key names
def getKeyCode(keyname):
    keyname = keyname.lower()

    if len(keyname) < 1:
        return None

    if keyname in ['windows', 'gui', 'super']:
        return '\\x5b'
    if keyname  == 'shift':
        return '\\x10'
    if keyname  == 'alt':
        return '\\x12'
    if keyname in ['control', 'ctrl']:
        return '\\x11'
    if keyname in ['left', 'leftarrow']:
        return '\\x25'
    if keyname in ['up', 'uparrow']:
        return '\\x26'
    if keyname in ['right', 'rightarrow']:
        return '\\x27'
    if keyname in ['down', 'downarrow']:
        return '\\x28'
    if keyname in ['pause', 'break']:
        return '\\x13'
    if keyname == 'capslock':
        return '\\x14'
    if keyname == 'delete':
        return '\\x2E'
    if keyname == 'end':
        return '\\x23'
    if keyname in ['esc', 'escape']:
        return '\\x1b'
    if keyname == 'home':
        return '\\x24'
    if keyname == 'insert':
        return '\\x2D'
    if keyname == 'numlock':
        return '\\x90'
    if keyname == 'pageup':
        return '\\x21'
    if keyname == 'pagedown':
        return '\\x22'
    if keyname == 'printscreen':
        return '\\x2C'
    if keyname == 'enter':
        return '\\n'
    if keyname == 'scrolllock':
        return '\\x91'
    if keyname == 'space':
        return ' '
    if keyname == 'tab':
        return '\\t'
    if len(keyname) == 2 and keyname[0] == 'f':
        try:
            num = int(keyname[1:])
        except:
            return None

        if num > 12 or num < 1:
            return None

        return str((num+111).to_bytes(1, byteorder='big'))[2:-1]

    if keyname in 'abcdefghijklmnopqrstuvwxyz':
        return keyname

    return None

# Escape characters in strings
def replaceSpecial(string):
    return string.replace('\\', '\\\\').replace('\n', '\\n').replace('\t', '\\t').replace('"', '\\"').replace('\'', '\\\'').replace('\r', '\\r').replace('%', '\\%')

# Parsing
for i, line in enumerate(lines):
    line = line.strip()
    if line.lower().startswith('rem'):
        continue

    argv = line.split(' ');

    final += b'"'
    if len(argv) > 1:
        argv[0] = argv[0].strip().lower()

        keycode = getKeyCode(argv[1])
        if keycode == None and (not argv[0] in ['default_delay', 'defaultdelay', 'delay', 'string_delay', 'repeat', 'string']):
            continue

        if argv[0] == 'alt':
            final += b'\\x02""' + keycode.encode()
        elif argv[0] in ['ctrl', 'control']:
            final += b'\\x03""' + keycode.encode()
        elif argv[0] == 'ctrl-alt':
            final += b'\\x04""' + keycode.encode()
        elif argv[0] == 'ctrl-shift':
            final += b'\\x05""' + keycode.encode()
        elif argv[0] in ['default_delay', 'defaultdelay']:
            try:
                final += b'\\x06""' + str(int(argv[1]).to_bytes(2, byteorder='big'))[2:-1].encode()
            except:
                print(f'Failed to parse number on line {i}. Skipping instruction.')
        elif argv[0] == 'delay':
            try:
                final += b'\\x07""' + str(int(argv[1]).to_bytes(2, byteorder='big'))[2:-1].encode()
            except:
                print(f'Failed to parse number on line {i}. Skipping instruction.')
        elif argv[0] in ['gui', 'windows']:
            final += b'\\x08""' + keycode.encode()
        elif argv[0] == 'alt-shift':
            final += b'\\x09""'
        elif argv[0] == 'shift':
            final += b'\\x0A""' + keycode.encode()
        elif argv[0] == 'string':
            final += b'\\x0B""' + replaceSpecial(' '.join(argv[1:])).encode() + b'\\xFF'
        elif argv[0] == 'string_delay':
            if len(argv) < 3:
                print(f'Not enough arguments provided for STRING_DELAY on line {i}. Skipping instruction.')
                continue

            try:
                final += b'\\x0C' + str(int(argv[1]).to_bytes(2, byteorder='big'))[2:-1].encode() + b'""' + argv[2:].encode() + b'\\xFF'
            except:
                print(f'Failed to parse number on line {i}. Skipping instruction.')
        elif argv[0] == 'repeat':
            final += b'\\x0D'
    else:
        keycode = getKeyCode(argv[0])
        if keycode != None:
            final += b'\\x01' + keycode.encode()
    final += b'"'

print('Done!')
print('Which program do you wish to overwrite?')
filename = 'programs/Program'

while True:
    num = 0
    inp = input('[1-16] or quit: ')
    if inp == 'quit':
        os.exit(0)
    else:
        try:
            num = int(inp)
        except:
            num = 0

    if num < 1 or num > 16:
        print('Invalid option! Please pick again.')
    else:
        filename += str(num) + '.h'
        break

with open(filename, 'wb+') as f:
    f.write(final)
    f.close()

print('Done! Press ENTER to continue.')
input()
