#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include <stdbool.h>
#include "config.h"

// computers using the chip8 language had a 16 key hexadecimal keypad with the following layout:
// | 1 | 2 | 3 | C
// | 4 | 5 | 6 | D
// | 7 | 8 | 9 | E
// | A | 0 | B | F

struct chip8_keyboard 
{
    bool keyboard[CHIP8_TOTAL_KEYS];    
};
int chip8_keyboard_map(const char* map, char key);
void chip8_keyboard_down(struct chip8_keyboard* keyboard, int key);
void chip8_keyboard_up(struct chip8_keyboard* keyboard, int key);
bool chip8_key_is_down(struct chip8_keyboard* keyboard, int key);

#endif
