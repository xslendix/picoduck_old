#!/usr/bin/env python3

import os
import argparse

print("DuckyScript+ to PicoDuck converter")
lines = ['']

parser = argparse.ArgumentParser()
parser.add_argument('--program', '-p', type=int, default=0)
parser.add_argument('--file', '-f', type=str, default='')

args = parser.parse_args()

read_from_file = os.path.exists(args.file) and os.path.isfile(args.file)
program_num = args.program

if not read_from_file:
    print('Please paste your script here, at the end, type "EOF":')
    while lines[-1].strip() != 'EOF':
        lines.append(input())

    lines = lines[1:-1]
else:
    with open(args.file, 'r') as f:
        lines = [a.replace('\n', '') for a in f.readlines()]

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

    if keyname.lower() in 'abcdefghijklmnopqrstuvwxyz':
        return keyname

    return None

# Escape characters in strings
def replaceSpecial(string):
    return string.replace('\\', '\\\\').replace('\n', '\\n').replace('\t', '\\t').replace('"', '\\"').replace('\'', '\\\'').replace('\r', '\\r').replace('%', '\\%')

def parseLine(i, line):
    global final

    line = line.lstrip()
    if line.lower().startswith('rem ') or line.startswith('//'):
        return 'continue'

    argv = line.split(' ');

    argv[0] = argv[0].strip()

    mouse_names = argv[0].startswith('mouse_')

    final += b'"'
    if len(argv) > 1 or mouse_names:
        argv[0] = argv[0].lower()

        keycode = None
        if not mouse_names:
            keycode = getKeyCode(argv[1])

        if argv[0] == 'alt':
            if keycode != None:
                final += b'\\x02""' + keycode.encode()
        elif argv[0] in ['ctrl', 'control']:
            if keycode != None:
                final += b'\\x03""' + keycode.encode()
        elif argv[0] in ['ctrl_alt', 'ctrl-alt']:
            if keycode != None:
                final += b'\\x04""' + keycode.encode()
        elif argv[0] in ['ctrl_shift', 'ctrl-shift']:
            if keycode != None:
                final += b'\\x05""' + keycode.encode()
        elif argv[0] in ['default_delay', 'defaultdelay', 'default-delay']:
            try:
                num = str(int(argv[1]).to_bytes(2, byteorder='big'))[2:-1]
                num = num[:4] + '""' + num[4:]
                final += b'\\x06""' + num.encode()
            except:
                print(f'Failed to parse number on line {i+1}. Skipping instruction {argv[0]}.')
        elif argv[0] == 'delay':
            try:
                num = str(int(argv[1]).to_bytes(2, byteorder='big'))[2:-1]
                num = num[:4] + '""' + num[4:]
                final += b'\\x07""' + num.encode()
            except:
                print(f'Failed to parse number on line {i+1}. Skipping instruction {argv[0]}.')
        elif argv[0] in ['gui', 'windows', 'win']:
            final += b'\\x08""' + keycode.encode()
        elif argv[0] in ['alt_shift', 'alt-shift']:
            final += b'\\x09""'
        elif argv[0] == 'shift':
            final += b'\\x0A""' + keycode.encode()
        elif argv[0] == 'string':
            for i in replaceSpecial(' '.join(argv[1:])):
                final += b'\\x01""'
                final += i.encode()
        elif argv[0] == 'repeat':
            final += b'"'
            for x in range(int(argv[1])):
                ret = parseLine(i-1, lines[i-1])
                if ret != None:
                    return ret
            final += b'"'
        elif argv[0] in ['mouse_left', 'mouse-left']:
            mode = b'\\x01'
            if len(argv) > 1:
                if argv[1].lower() == 'hold':
                    mode = b'\\x02'
                elif argv[1].lower() == 'release':
                    mode = b'\\x03'
            final += b'\\x0A' + mode
        elif argv[0] in ['mouse_mid', 'mouse-mid', 'mouse_middle', 'mouse-middle']:
            mode = b'\\x01'
            if len(argv) > 1:
                if argv[1].lower() == 'hold':
                    mode = b'\\x02'
                elif argv[1].lower() == 'release':
                    mode = b'\\x03'
            final += b'\\x0B' + mode
        elif argv[0] in ['mouse_right', 'mouse-right']:
            mode = b'\\x01'
            if len(argv) > 1:
                if argv[1].lower() == 'hold':
                    mode = b'\\x02'
                elif argv[1].lower() == 'release':
                    mode = b'\\x03'
            final += b'\\x0C' + mode
        elif argv[0] in ['mouse_move', 'mouse-move']:
            try:
                num = str(int(int(argv[1]) + 0xffff/2).to_bytes(2, byteorder='big'))[2:-1]
                num = num[:4] + '""' + num[4:]
                num2 = str(int(int(argv[2]) + 0xffff/2).to_bytes(2, byteorder='big'))[2:-1]
                num2 = num2[:4] + '""' + num2[4:]
                final += b'\\x0D""' + num.encode() + b'""' + num2.encode()
            except Exception as e:
                print(f'Failed to parse number(s) on line {i+1}. Skipping instruction {argv[0]}.')
        elif argv[0] in ['mouse_wheel', 'mouse-wheel']:
            try:
                amount = int(argv[1]) + 127
                final += b'\\x0E\\x'+hex(amount)[-2:].encode()
            except Exception as e:
                print(f'Failed to parse number on line {i+1}. Skipping instruction {argv[0]}.')
    else:
        keycode = getKeyCode(argv[0])
        if keycode != None:
            final += b'\\x01""' + keycode.encode()
    final += b'" // ' + ' '.join(argv).encode() + b'\n'

for i, line in enumerate(lines):
    if line.endswith(' ') or line.endswith('\t'):
        print(f'Warning! Line {i+1} has a trailing newline!')

# Parsing
for i, line in enumerate(lines):
    if parseLine(i, line) == 'continue':
        continue

final += b'"\\xFE"'

filename = 'programs/Program'

if program_num == 0 or program_num > 16:
    print('Which program do you wish to overwrite?')
    while True:
        num = 0
        inp = input('[1-16] or quit: ')
        if inp == 'quit':
            exit(0)
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
else:
    filename += str(program_num) + '.h'

with open(filename, 'wb+') as f:
    f.write(final)
    f.close()

print('Done!')

