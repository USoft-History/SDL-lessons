#include <stdbool.h>

#include <SDL2/SDL.h>

#include "input.h"

#ifdef PLATFORM_PS2
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448
#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#else
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)
#endif

#define MOVE_SPEED 5

static const char *const WINDOW_TITLE = "SDL2 Lesson 5.3";

static bool running = true;

static SDL_Rect rect = {(SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 100) / 2, 200, 100};

int initScene()
{
  if (SDL_Init(SDL_INIT_FLAGS) != 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
  }

  if (Input_Init() != 0)
  {
    return 1;
  }

  return 0;
}

void pollEvent()
{
  InputState input = {0};

  Input_Poll(&input);

  if (input.up)
    rect.y -= MOVE_SPEED;
  if (input.down)
    rect.y += MOVE_SPEED;
    
  if (input.left)
    rect.x -= MOVE_SPEED;
  if (input.right)
    rect.x += MOVE_SPEED;

  if (input.quit)
    running = false;
}

int main(int argc, char *argv[])
{

  if (initScene() != 0)
    return 1;

  SDL_Window *window = SDL_CreateWindow(
      WINDOW_TITLE,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      0);

  if (!window)
  {
    SDL_Log("SDL Create Window Error: %s", SDL_GetError());

    SDL_Quit();

    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer)
  {
    SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
  }

  while (running)
  {
    pollEvent();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /* Filled Rectangle */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

    SDL_Delay(16); // ~60 FPS
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}