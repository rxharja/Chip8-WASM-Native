#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include "config.h"

// Chip 8 has 4096 bytes of memory
// 0x000 to 0x1FF are reserved for the interpreter
// 0x200 to 0xFFF is the program / data space
struct chip8_memory 
{
    unsigned char memory[CHIP8_MEMORY_SIZE];
};

void chip8_memory_set(struct chip8_memory* memory, int index, unsigned char val);
unsigned char chip8_memory_get(struct chip8_memory* memory, int index);

#endif