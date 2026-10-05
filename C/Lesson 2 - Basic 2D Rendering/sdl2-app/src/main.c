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
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#endif

const char *const WINDOW_TITLE = "SDL2 Lesson 2";

int main(int argc, char *argv[])
{

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      WINDOW_TITLE,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      WINDOW_FLAGS);

  if (!window)
  {
    SDL_Log("SDL Create Window Error: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);

  if (!renderer)
  {
    SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
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

    /* Point */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 100, 100);

    /* Line */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, 50, 50, 300, 200);

    /* Rectangle */
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect1 = {100, 250, 200, 100};
    SDL_RenderDrawRect(renderer, &rect1);

    /* Filled Rectangle */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect2 = {350, 250, 200, 100};
    SDL_RenderFillRect(renderer, &rect2);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}