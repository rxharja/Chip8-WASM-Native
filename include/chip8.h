#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "chip8_memory.h"
#include "chip8_registers.h"

struct Chip8 {
    struct chip8_memory memory;
    struct chip8_registers registers;
};

#endif