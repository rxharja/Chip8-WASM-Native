#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "chip8.h"
#include "chip8_screen.h"
#include "chip8_stack.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

const char chip8_default_character_set[] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

void chip8_init(struct Chip8* chip8)
{
  memset(chip8, 0, sizeof(struct Chip8));

  memcpy(chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}

void chip8_load(struct Chip8* chip8, const char* buf, size_t size)
{
  assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
  memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
}

static void chip8_exec_extended_eight(struct Chip8* chip8, unsigned short opcode)
{
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f; 
  unsigned char final_fourbits = opcode & 0x000f;
  unsigned short tmp = 0;

  switch (final_fourbits) {
    // 8xy0 - LD Vx, Vy; store Vy in Vx
    case 0x00:
      chip8->registers.V[x] = chip8->registers.V[y];
      break;

    // 8xy1 - OR Vx, Vy: bitwise OR on Vx and Vy, stores result in Vx
    case 0x01:
      chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
      break;

    // 8xy2 - AND Vx, Vy: bitwise AND on Vx and Vy, stores result in Vx
    case 0x02:
      chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
      break;
      
    // 8xy3 - AND Vx, Vy: bitwise XOR on Vx and Vy, stores result in Vx
    case 0x03:
      chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
      break;

    // 8xy4 - ADD Vx, Vy: Vx = Vx + Vy, VF = Carry
    case 0x04:
      chip8->registers.V[0x0F] = false;
      tmp = chip8->registers.V[x] + chip8->registers.V[y];
      chip8->registers.V[0x0F] = tmp > 0xff;
      chip8->registers.V[x] = tmp;
      break;

    // 8xy4 - SUB Vx, Vy: Vx = Vx - Vy, VF = 1 if Vx > Vy, VF ? NOT BORROW
    case 0x05:
      chip8->registers.V[0x0F] = false;
      chip8->registers.V[0x0F] = chip8->registers.V[x] > chip8->registers.V[y];
      chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
      break;

    // S8xy6 HR Vx {, Vy}: if least significant bit of Vx is 1, Vf is 1, else 0. Vx = Vx / 2
    case 0x06:
      chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;
      chip8->registers.V[x] /= 2;
      break;

    // 8xy7 SUBN Vx, Vy: set Vx = Vy - Vx, set VF = Not Borrow
    case 0x07:
      chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
      chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
      break;

    // 8xyE - SHL Vx {, Vy}: if most significant bit of Vx = 1, Vf = 1, Vx *= 2
    case 0x0E:
      chip8->registers.V[0x0f] = chip8->registers.V[x] >> 7;
      chip8->registers.V[x] *= 2;
      break;
  }
}

static void chip8_exec_extended(struct Chip8* chip8, unsigned short opcode)
{
  unsigned short nnn = opcode & 0x0fff;
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f; 
  unsigned char kk = opcode & 0x00ff;

  switch(opcode & 0xf000)
  {
    // JP address, 1nnn jump to location nnn, in other words set PC to nnn
    case 0x1000:
      chip8->registers.PC = nnn;
      break;

    // CALL address, 2nnn call subroutine at nnn
    case 0x2000:
      chip8_stack_push(chip8, chip8->registers.PC);
      chip8->registers.PC = nnn;
      break;

    // SE Vx, byte, skip next instruction if Vx = kk, pc gets incremented by 2
    case 0x3000:
      if (chip8->registers.V[x] == kk) {
        chip8->registers.PC += 2;
      }
      break;

    // SNE Vx, byte, skip next instruction if Vx != kk, pc gets incremented by 2
    case 0x4000:
      if (chip8->registers.V[x] != kk) {
        chip8->registers.PC += 2;
      }
      break;

    // 5xy0 SE Vx, Vy, compare register Vx to Vy, if equal, increment by 2
    case 0x5000:
      if (chip8->registers.V[x] == chip8->registers.V[y]) {
        chip8->registers.PC += 2;
      }
      break;

    // 6xkk - LD Vx, byte: Set Vx = kk
    case 0x6000:
      chip8->registers.V[x] = kk; 
      break;

    // 7xkk - ADD Vx, byte. Set Vx = Vx + kk
    case 0x7000:
      chip8->registers.V[x] += kk;
      break;

    // 8xyN cases
    case 0x8000:
      chip8_exec_extended_eight(chip8, opcode);
      break;

    // 9xy0 - SNE Vx, Vy; skip next instruction if Vx != Vy
    case 0x9000:
      chip8->registers.PC += (chip8->registers.V[x] != chip8->registers.V[y]) 
        ? 2 : 0;
      break;

    // Annn - LD I, addr: set I = nnn
    case 0xA000:
      chip8->registers.I = nnn;
      break;

    // Bnnn - JP V0, addr
    case 0xB000:
      chip8->registers.PC = nnn + chip8->registers.V[0x00];
      break;

    // Cxkk - RND V0, byte. Set Vx = random Byte AND kk
    case 0xC000:
      srand(clock());
      chip8->registers.V[x] = (rand() % 255) & kk;
      break;
  }
}

void chip8_exec(struct Chip8* chip8, unsigned short opcode)
{
  switch(opcode)
  {
    // CLS : clear display
    case 0x00E0:
      chip8_screen_clear(&chip8->screen);
      break;

    // RET : Return from subroutine
    case 0x00EE:
      chip8->registers.PC = chip8_stack_pop(chip8);
      break;

    default:
      chip8_exec_extended(chip8, opcode);
  }
}
