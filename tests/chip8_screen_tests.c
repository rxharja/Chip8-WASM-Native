#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/chip8.h"
#include "../include/chip8_screen.h"

// Test:
// collision test 
static void test_chip8_collision_test(struct Chip8* chip8)
{
  bool initial = chip8_screen_draw_sprite(&chip8->screen, 0, 0, &chip8->memory.memory[0x00], 5);
  assert(initial == false);

  bool final = chip8_screen_draw_sprite(&chip8->screen, 0, 0, &chip8->memory.memory[0x00], 5);
  assert(final == true);
}


int main(void) {
  printf("CHIP8 Screen: ");
  struct Chip8 chip8;

  chip8_init(&chip8);

  test_chip8_collision_test(&chip8);

  printf("Success\n");
}
