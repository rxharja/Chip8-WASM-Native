#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/chip8.h"
#include "../include/chip8_memory.h"

// Test:
// memory set 
static void test_chip8_memory_set(struct Chip8* chip8, int index, unsigned short val)
{
  chip8_memory_set(&chip8->memory, index, val);
}

// Test:
// memory get
static void test_chip8_memory_get(struct Chip8* chip8, int index, unsigned short val)
{
  unsigned short result = chip8_memory_get(&chip8->memory, index);

  assert(result == val);
}

int main(void) {
  printf("CHIP8 Memory: ");
  struct Chip8 chip8;

  unsigned short val = 0xFF;

  test_chip8_memory_set(&chip8, 0, val);

  test_chip8_memory_get(&chip8, 0, val);

  printf("Success\n");
}
