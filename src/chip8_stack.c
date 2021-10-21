#include "chip8_stack.h"
#include "chip8.h"
#include <assert.h>

static void chip8_stack_in_bounds(struct Chip8* chip8) 
{
    // dont need to check if its below 0 because it is unsigned
    assert(chip8->registers.SP < CHIP8_TOTAL_STACK_DEPTH);
}

void chip8_stack_push(struct Chip8* chip8, unsigned short val)
{
    chip8_stack_in_bounds(chip8);
    chip8->stack.stack[chip8->registers.SP] = val;
    chip8->registers.SP += 1;
}

unsigned short chip8_stack_pop(struct Chip8* chip8)
{
    chip8->registers.SP -= 1;
    chip8_stack_in_bounds(chip8);
    unsigned short val = chip8->stack.stack[chip8->registers.SP];
    return val;
}