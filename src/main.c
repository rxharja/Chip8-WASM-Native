#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8_keyboard.h"

int main(int argc, char **argv)
{
  struct Chip8 chip8;
  
  chip8_keyboard_down(&chip8.keyboard, 0x0f);
  bool is_down = chip8_key_is_down(&chip8.keyboard, 0x0f);
  printf("%i\n", is_down);

  chip8_keyboard_up(&chip8.keyboard, 0x0f);
  is_down = chip8_key_is_down(&chip8.keyboard, 0x0f);
  printf("%i\n", is_down);

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
      if (event.type == SDL_QUIT) {
        goto out;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 40;
    r.h = 40;
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);
  }
  
out:
  SDL_DestroyWindow(window);
  return 0;
}
