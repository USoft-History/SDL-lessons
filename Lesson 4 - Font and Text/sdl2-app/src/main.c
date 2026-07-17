#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef PLATFORM_PS2
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448
#else
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#endif

const char *const WINDOW_TITLE = "SDL2 Lesson 4";

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
  }

  if (TTF_Init() == -1)
  {
    SDL_Log("TTF_Init FAILED: %s\n", TTF_GetError());

    return 1;
  }

  TTF_Font *mediumFont = TTF_OpenFont("assets/font.ttf", 24);
  if (!mediumFont)
  {
    SDL_Log("TTF_OpenFont error: %s\n", TTF_GetError());
  }

#ifdef PLATFORM_PS2
  SDL_Window *window = SDL_CreateWindow(
      WINDOW_TITLE,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      0);

  if (!window)
  {
    SDL_Log("SDL window is null");
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

#else

  SDL_Window *window = SDL_CreateWindow(
      WINDOW_TITLE,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN);

  if (!window)
  {
    SDL_Log("SDL window is null");
    SDL_Quit();

    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED);

  if (!renderer)
  {
    SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
  }

  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

#endif

  SDL_Color white = {255, 255, 255, 255};

  SDL_Surface *textSurface =
      TTF_RenderUTF8_Blended(mediumFont, "Hello World!", white);

  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(
          renderer,
          textSurface);

  SDL_Rect textRect = {
      (SCREEN_WIDTH - textSurface->w) / 2,
      (SCREEN_HEIGHT - textSurface->h) / 2,
      textSurface->w,
      textSurface->h};

  SDL_FreeSurface(textSurface);

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

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(textTexture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_CloseFont(mediumFont);

  SDL_Quit();

  return 0;
}