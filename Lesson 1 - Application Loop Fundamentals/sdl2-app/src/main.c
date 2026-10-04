#include <stdbool.h>

#include <SDL2/SDL.h>

#ifdef PLATFORM_PS2
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448
#define WINDOW_FLAGS 0
#define RENDERER_FLAGS 0
#else
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
#endif

const char *const WINDOW_TITLE = "SDL2 Lesson 1";

int main(int argc, char *argv[])
{

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  int exit_code = 0;

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
  }

  window = SDL_CreateWindow(
      WINDOW_TITLE,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      WINDOW_FLAGS);

  if (!window)
  {
    SDL_Log("SDL Create Window Error: %s", SDL_GetError());

    exit_code = 1;
    goto cleanup;
  }

  renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);

  if (!renderer)
  {
    SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());

    exit_code = 1;
    goto cleanup;
  }

  bool running = true;

  while (running)
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

cleanup:
  if (renderer)
  {
    SDL_DestroyRenderer(renderer);
  }

  if (window)
  {
    SDL_DestroyWindow(window);
  }

  SDL_Quit();

  return exit_code;
}