#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "toot.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/HTML5.h>
#endif

#include <SDL.h>
#include "chip8.h"
#include "chip8_keyboard.h"
#include "chip8_screen.h"

/*
 * Keyboard map mapping the virtual keyboard 
 * to the modern ones
 */
struct Chip8 chip8;

SDL_Renderer *renderer;

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
  SDLK_0, SDLK_1, SDLK_2, SDLK_3, 
  SDLK_4, SDLK_5, SDLK_6, SDLK_7, 
  SDLK_8, SDLK_9, SDLK_a, SDLK_b, 
  SDLK_c, SDLK_d, SDLK_e, SDLK_f,
};

int run(const char *filename);

int load_file(const char* filename);

static void renderLoop();

int init(int argc, const char **argv);

#ifdef __EMSCRIPTEN__
void emscripten_reload(const char* filename);
#endif

#ifndef __EMSCRIPTEN__
int main(int argc, const char **argv)
{
  if (argc < 2) {
    printf("You must provide a file to load.\n");
    return -1;
  }

  const char* filename = argv[1];

  return run(filename);
}
#endif

int run(const char *filename)
{
#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
#endif

  if (load_file(filename) == -1) {
    return -1;
  }

  chip8_keyboard_set_map(&chip8.keyboard, keyboard_map);

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window = SDL_CreateWindow(
      EMULATOR_WINDOW_TITLE,
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      CHIP8_WIDTH * CHIP8_WINDOW_SCALE,
      CHIP8_HEIGHT * CHIP8_WINDOW_SCALE,
      SDL_WINDOW_OPENGL 
  );

  renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(renderLoop, 0, 0);
#else
  while (1) { renderLoop(); }
#endif

  return 1;
}

static void renderLoop()
{
#ifdef __EMSCRIPTEN__
    // 7 clock cycles per refresh seems to be the sweet spot for 
    // emulation running WASM
    /* for (int i = 0; i < 7; i++) { */
#endif

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
#ifdef __EMSCRIPTEN__
          emscripten_cancel_main_loop();
#else
          exit(0);
#endif
        break;

        case SDL_KEYDOWN:
        {
          char key = event.key.keysym.sym;
          char vkey = chip8_keyboard_map(&chip8.keyboard, key);
          if (vkey != -1) {
            chip8_keyboard_down(&chip8.keyboard, vkey);
          }
        }
        break;

        case SDL_KEYUP:
        {
          char key = event.key.keysym.sym;
          char vkey = chip8_keyboard_map(&chip8.keyboard, key);
          if (vkey != -1) {
            chip8_keyboard_up(&chip8.keyboard, vkey);
          }
        }
        break;
      }
#ifdef __EMSCRIPTEN__
      emscripten_sleep(1);
#endif
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

    if (chip8.registers.delay_timer > 0) {
      /* usleep(100); */
      chip8.registers.delay_timer -= 1;
    }

    if (chip8.registers.sound_timer > 0) {
      toot(2000, 20 * chip8.registers.sound_timer);
      chip8.registers.sound_timer = 0;
    }

      unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);

      chip8.registers.PC += 2;

      chip8_exec(&chip8, opcode);
#ifdef __EMSCRIPTEN__
    /* } */
#endif

}

#ifdef __EMSCRIPTEN__
void emscripten_reload(const char* filename)
{
  emscripten_cancel_main_loop();
  load_file(filename);
  emscripten_set_main_loop(renderLoop, 0, 0);
}

#endif
int load_file(const char* filename)
{
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

  chip8_init(&chip8);
  chip8_load(&chip8, buf, size);

  return 0;
}
