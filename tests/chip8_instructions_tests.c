#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/config.h"
#include "../include/chip8.h"

// Test:
// 00E0 CLS
// clear screen
static void test_chip8_cls(struct Chip8* chip8)
{
  memset(chip8->screen.pixels, 1, sizeof(chip8->screen.pixels));
  chip8_exec(chip8, 0x00E0);
  for(int x = 0; x < CHIP8_WIDTH; x++) {
    for(int y = 0; y < CHIP8_HEIGHT; y++) {
      assert(chip8->screen.pixels[y][x] == false);
    }
  }
}

// Test:
// 1NNN JP ADDR
// skip next instruction if Vx == KK
static void test_chip8_3xkk(struct Chip8* chip8)
{
  int originalPC = 0x00;
  chip8->registers.PC = originalPC;
  chip8->registers.V[0x00] = 0x22;
  chip8_exec(chip8, 0x3022);
  assert(chip8->registers.PC == originalPC + 2);
}

// Test:
// 4XKK SNE Vx, byte
// skip next instruction if Vx != KK
static void test_chip8_4xkk(struct Chip8* chip8)
{
  int originalPC = 0x00;
  chip8->registers.PC = originalPC;
  chip8->registers.V[0x00] = 0x22;
  chip8_exec(chip8, 0x4022);
  assert(chip8->registers.PC != originalPC + 2);
}

// Test:
// 5xy0, SE Vx Vy
// skip next instruction if Vx == Vy
static void test_chip8_5xy0(struct Chip8* chip8)
{
  int originalPC = 0x00;

  chip8->registers.PC = originalPC;
  chip8->registers.V[2] = 0x05;
  chip8->registers.V[3] = 0x10;
  chip8_exec(chip8, 0x5230);
  assert(chip8->registers.PC == originalPC);

  chip8->registers.V[2] = 0x10;
  chip8->registers.V[3] = 0x10;
  chip8_exec(chip8, 0x5230);
  assert(chip8->registers.PC == originalPC + 2);
}

int main(void) {
  printf("CHIP8 Instructions: ");
  
  struct Chip8 chip8;

  chip8_init(&chip8);

  test_chip8_cls(&chip8);
  test_chip8_3xkk(&chip8);
  test_chip8_4xkk(&chip8);
  test_chip8_5xy0(&chip8);

  printf("Success\n");
}
