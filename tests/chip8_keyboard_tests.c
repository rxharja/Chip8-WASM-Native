#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/chip8.h"
#include "../include/chip8_keyboard.h"



// Test:
// Keyboard down press
static void test_chip8_keyboard_down(struct chip8_keyboard* keyboard)
{
  chip8_keyboard_down(keyboard, 0x0f);
  bool is_down = chip8_key_is_down(keyboard, 0x0f);
  assert(is_down == true);
}

// Test:
// Keyboard release down
static void test_chip8_keyboard_up(struct chip8_keyboard* keyboard)
{
  chip8_keyboard_up(keyboard, 0x0f);
  bool is_down = chip8_key_is_down(keyboard, 0x0f);
  assert(is_down == false);
}

int main(void) {
  printf("CHIP8 Keyboard: ");
  struct Chip8 chip8;
  test_chip8_keyboard_down(&chip8.keyboard);
  test_chip8_keyboard_up(&chip8.keyboard);

  printf("Success\n");
}
