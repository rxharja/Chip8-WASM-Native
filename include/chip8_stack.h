#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include "config.h"

// forward declared chip8 struct to avoid loop
struct Chip8;

// the stack is an array of 16 16 bit values
// used to store teh address that the interpreter
// should return when finished with a subroutine.
struct chip8_stack {
    unsigned short stack[CHIP8_TOTAL_STACK_DEPTH];
};

void chip8_stack_push(struct Chip8* chip8, unsigned short val);
unsigned short chip8_stack_pop(struct Chip8* chip8);

#endif