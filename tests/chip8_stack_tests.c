#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/chip8.h"
#include "../include/chip8_stack.h"

// Test:
// stack push 
static void test_chip8_stack_push(struct Chip8* chip8, unsigned short val)
{
  chip8_stack_push(chip8, val);

  assert(chip8->registers.SP == 1);
}

// Test:
// stack pop
static void test_chip8_stack_pop(struct Chip8* chip8, unsigned short val)
{
  unsigned short result = chip8_stack_pop(chip8);

  assert(chip8->registers.SP == 0);

  assert(result == val);
}

// Test:
// stack depth:
static void test_chip8_stack_depth(struct Chip8* chip8, unsigned short val)
{
  for (int i = 0; i < CHIP8_TOTAL_STACK_DEPTH; i++) {
    chip8_stack_push(chip8, val);
  }

  assert(chip8->registers.SP == CHIP8_TOTAL_STACK_DEPTH);

  for (int i = 0; i < CHIP8_TOTAL_STACK_DEPTH; i++) {
    chip8_stack_pop(chip8);
  }

  assert(chip8->registers.SP == 0);
}

int main(void) {
  printf("CHIP8 Stack: ");
  struct Chip8 chip8;

  unsigned short val = 0x01;

  test_chip8_stack_push(&chip8, val);
  test_chip8_stack_pop(&chip8, val);
  test_chip8_stack_depth(&chip8, val);

  printf("Success\n");
}
