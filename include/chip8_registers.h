#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include "config.h"

// registers are temporary storage regions in a processor 
// outside of RAM
struct chip8_registers
{
    // chip 8 has 16 general purpose 8 bit registers
    // a char is 8 bit
    unsigned char V[CHIP8_TOTAL_DATA_REGISTERS];
    
    // it also has a 16 bit register called I
    // used to store memory addresses
    unsigned short I;

    // special purpose 8 bit register
    // when non-zero, decrements at a rate of 60hz
    unsigned char delay_timer;
    
    // special purpose 8 bit register
    // when non-zero, decrements at a rate of 60hz
    unsigned char sound_timer;
    
    // 16 bit pseudo register not accessible by chip8 program
    // program counter stores the currently executing address
    unsigned short PC;

    // 8 bit pseudo register not accessible by chip8 program
    // stack pointer points to topmost level of the stack
    unsigned char SP;
};

#endif
