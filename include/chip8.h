#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "chip8_memory.h"

struct Chip8 {
    struct chip8_memory memory;
};

#endif