#include <stdio.h>
#include <stdbool.h>
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

int main(int argc, char **argv)
{
  struct Chip8 chip8;

  chip8_init(&chip8);

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
  }
  
out:
  SDL_DestroyWindow(window);
  return 0;
}
