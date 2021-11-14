#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "chip8.h"
#include "SDL.h"
#include "chip8_memory.h"
#include "config.h"

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
  /* chip8->registers.delay_timer = 0; */
  /* chip8->registers.sound_timer = 0; */
  /* chip8->registers.SP = 0; */
  /* chip8->registers.PC = 0; */
  /* chip8->registers.I = 0; */
  /* memset(chip8->registers.V, 0, sizeof(unsigned char[CHIP8_TOTAL_DATA_REGISTERS]) ); */
  /* memset(chip8->stack.stack, 0, sizeof(unsigned short[CHIP8_TOTAL_STACK_DEPTH]) ); */
  memset(chip8, 0, sizeof(struct Chip8));
  memcpy(chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}

void chip8_load(struct Chip8* chip8, const char* buf, size_t size)
{
  assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
  memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
  chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static char chip8_wait_for_keypress(struct Chip8* chip8) 
{
  SDL_Event event;
  while(SDL_WaitEvent(&event))
  {
    if (event.type != SDL_KEYDOWN) {
      continue;
    }

    char c = event.key.keysym.sym;
    char chip8_key = chip8_keyboard_map(&chip8->keyboard, c);
    if (chip8_key != -1) { 
      return chip8_key; 
    }
  }

  return -1;
}

static void chip8_exec_extended_F(struct Chip8* chip8, unsigned short opcode)
{
  unsigned char x = (opcode >> 8) & 0x000f;

  switch (opcode & 0x0ff) {
    // Fx07 LD Vx, DT: set Vx = delay timer value
    case 0x07:
      chip8->registers.V[x] = chip8->registers.delay_timer;
    break;

    // Fx0A Ld Vx, K: wait for key press, store value of key in Vx
    case 0x0A:
    {
      char key_pressed = chip8_wait_for_keypress(chip8);
      chip8->registers.V[x] = key_pressed;
    }
    break;

    // Fx15 - LD DT ,Vx : set delay timer = Vx
    case 0x15:
      chip8->registers.delay_timer = chip8->registers.V[x];
    break;

    // Fx15 - LD DT ,Vx : set sound timer = Vx
    case 0x18:
      chip8->registers.sound_timer = chip8->registers.V[x];
    break;

    // Fx1E - Add I, Vx : set I = I + Vx
    case 0x1E:
      chip8->registers.I += chip8->registers.V[x];
    break;

    // Fx29 - LD F, Vx : Set I = Location of sprite for digit Vx
    case 0x29:
     chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
    break;

    // Fx33 - LD B, Vx - store BCD representation of Vx in memory locations I, I+1, and I+2
    case 0x33:
    {
      unsigned char hundreds = chip8->registers.V[x] / 100; 
      unsigned char tens = chip8->registers.V[x] / 10 % 10;
      unsigned char units = chip8->registers.V[x] % 10;

      chip8_memory_set(&chip8->memory, chip8->registers.I, hundreds);
      chip8_memory_set(&chip8->memory, chip8->registers.I + 1, tens);
      chip8_memory_set(&chip8->memory, chip8->registers.I + 2, units);
    }
    break;

    // Fx55 store registers V0 through Vx in memory starting at location I
    case 0x55:
    {
      for (size_t i = 0; i <= x; i++) {
        chip8_memory_set(&chip8->memory, chip8->registers.I + i, chip8->registers.V[i]);
      }
    }
    break;

    // Fx65 - LD Vx, [I], Read registers V0 through Vx in memory starting at I
    case 0x65:
    {
      for (size_t i = 0; i <= x; i++) {
        chip8->registers.V[i] = chip8->memory.memory[chip8->registers.I + i];
      }
    }
    break;
  }
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
  unsigned char n = opcode & 0x000f;

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
    if (chip8->registers.V[x] != chip8->registers.V[y]) {
      chip8->registers.PC += 2;
    }
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

    // Dxyn - DRW Vx, Vy, nibble. Draws n-byte sprite to screen
    case 0xD000:
    {
      const unsigned char* sprite = &chip8->memory.memory[chip8->registers.I];
      chip8->registers.V[0x0f] = chip8_screen_draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
    }
    break;

    case 0xE000:
    {
      switch (opcode & 0x00ff) {
        // EX9E - SKP Vx, skip next instruction if key with value of Vx pressed
        case 0x9E:
          if (chip8_key_is_down(&chip8->keyboard, chip8->registers.V[x])) {
            chip8->registers.PC += 2;
          }
        break;

        // EXA1 SKNP Vx, skip next instruction if key with val of Vx is not pressed
        case 0xA1:
          if (!chip8_key_is_down(&chip8->keyboard, chip8->registers.V[x])) {
            chip8->registers.PC += 2;
          }
        break;
      }
    } 
    break;

    case 0xF000:
      chip8_exec_extended_F(chip8, opcode);
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
