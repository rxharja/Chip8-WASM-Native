#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "toot.h"
#include "SDL.h"
#include "chip8.h"
#include "chip8_keyboard.h"
#include "chip8_screen.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
  SDLK_0, SDLK_1, SDLK_2, SDLK_3, 
  SDLK_4, SDLK_5, SDLK_6, SDLK_7, 
  SDLK_8, SDLK_9, SDLK_a, SDLK_b, 
  SDLK_c, SDLK_d, SDLK_e, SDLK_f,
};

int main(int argc, const char **argv)
{

  if (argc < 2) {
    printf("You must provide a file to load.\n");
    return -1;
  }

  const char* filename = argv[1];

  printf("The file name to load is: %s\n", filename);

  FILE* f = fopen(filename, "rb");

  // open passed in file 
  if (!f) {
    printf("failed to open the file\n");
    return -1;
  }

  // seek to the end of the file
  fseek(f, 0, SEEK_END);
  // find position and set it to the size
  long size = ftell(f);
  // return back to start of file to proceed reading
  fseek(f, 0, SEEK_SET);

  char buf[size];

  if (fread(buf, size, 1, f) != 1) {
    printf("Failed to read from file.\n");
    return -1;
  }

  if (fclose(f) == EOF) {
    printf("failed to close the file\n");
    return -1;
  }

  struct Chip8 chip8;

  chip8_init(&chip8);

  chip8_load(&chip8, buf, size);

  for (int k = 0; k <= 30; k+=5) {
    chip8_screen_draw_sprite(&chip8.screen, 2 * k, 0, &chip8.memory.memory[0x00 + k], 5);
  }

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window = SDL_CreateWindow(
      EMULATOR_WINDOW_TITLE,
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      CHIP8_WIDTH * CHIP8_WINDOW_SCALE,
      CHIP8_HEIGHT * CHIP8_WINDOW_SCALE,
      SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
  while (1)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          goto out;
        break;

        case SDL_KEYDOWN:
        {
          char key = event.key.keysym.sym;
          char vkey = chip8_keyboard_map(keyboard_map, key);
          if (vkey != -1) {
            chip8_keyboard_down(&chip8.keyboard, vkey);
          }
        }
        break;

        case SDL_KEYUP:
        {
          char key = event.key.keysym.sym;
          char vkey = chip8_keyboard_map(keyboard_map, key);
          if (vkey != -1) {
            chip8_keyboard_up(&chip8.keyboard, vkey);
          }
        }
        break;

      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    for (int x = 0; x < CHIP8_WIDTH; x++) 
    {
      for (int y = 0; y < CHIP8_HEIGHT; y++) 
      {
        if (chip8_screen_is_set(&chip8.screen, x, y)) 
        {
          SDL_Rect r;
          r.x = x * CHIP8_WINDOW_SCALE;
          r.y = y * CHIP8_WINDOW_SCALE;
          r.w = CHIP8_WINDOW_SCALE;
          r.h = CHIP8_WINDOW_SCALE;

          SDL_RenderFillRect(renderer, &r);
        }
      } 
    }

    SDL_RenderPresent(renderer);

    if (chip8.registers.delay_timer > 0)
    {
      usleep(100);
      chip8.registers.delay_timer -= 1;
      printf("Delaying %i\n ", chip8.registers.delay_timer);
    }

    if (chip8.registers.sound_timer > 0)
    {
      toot(3000, 100 * chip8.registers.sound_timer);
      chip8.registers.sound_timer = 0;
    }

    unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
    chip8_exec(&chip8, opcode);
    chip8.registers.PC += 2;
    printf("%x\n", opcode);
  }
  
out:
  SDL_DestroyWindow(window);
  return 0;
}
