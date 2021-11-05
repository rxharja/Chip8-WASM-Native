#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include <stdbool.h>
#include "config.h"

struct chip8_screen {
  bool pixels[CHIP8_HEIGHT][CHIP8_WIDTH]; 
};

void chip8_screen_clear(struct chip8_screen* screen);
void chip8_screen_set(struct chip8_screen* screen, int x, int y);
bool chip8_screen_is_set(struct chip8_screen* screen, int x, int y);

// provice screen, x and y, the sprite to draw pointer, and number of bytes in sprite to draw
// if any pixels collide we return true, otherwise false
// essentially if any pixels were changed we return true
bool chip8_screen_draw_sprite(struct chip8_screen* screen, int x, int y, const unsigned char* sprite, int num);

#endif // !CHIP8_SCREEN_H
