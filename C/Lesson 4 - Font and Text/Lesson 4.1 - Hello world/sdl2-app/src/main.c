#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

const char *const WINDOW_TITLE = "SDL2 Lesson 4.1 - Hello world";

int main(int argc, char *argv[])
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  TTF_Font *mediumFont = NULL;
  SDL_Texture *textTexture = NULL;

  int result = 1;

  // SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());
    goto cleanup;
  }

  // SDL_ttf
  if (TTF_Init() == -1)
  {
    SDL_Log("TTF_Init FAILED: %s", TTF_GetError());
    goto cleanup;
  }

  // Font
  mediumFont = TTF_OpenFont("assets/font.ttf", 24);

  if (!mediumFont)
  {
    SDL_Log("TTF_OpenFont error: %s", TTF_GetError());
    goto cleanup;
  }

  // Window
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
    goto cleanup;
  }

  // Renderer
  renderer = SDL_CreateRenderer(
      window,
      -1,
      RENDERER_FLAGS);

  if (!renderer)
  {
    SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());
    goto cleanup;
  }

  // Text
  SDL_Color white = {255, 255, 255, 255};

  SDL_Surface *textSurface = TTF_RenderUTF8_Blended(
      mediumFont,
      "Hello World!",
      white);

  if (!textSurface)
  {
    SDL_Log("TTF_RenderUTF8_Blended error: %s", TTF_GetError());
    goto cleanup;
  }

  textTexture = SDL_CreateTextureFromSurface(
      renderer,
      textSurface);

  if (!textTexture)
  {
    SDL_Log("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
    goto cleanup;
  }

  SDL_Rect textRect = {
      (SCREEN_WIDTH - textSurface->w) / 2,
      (SCREEN_HEIGHT - textSurface->h) / 2,
      textSurface->w,
      textSurface->h};

  SDL_FreeSurface(textSurface);

  textSurface = NULL;

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

    SDL_SetRenderDrawColor(
        renderer,
        0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_RenderCopy(
        renderer,
        textTexture,
        NULL,
        &textRect);

    SDL_RenderPresent(renderer);
  }

  result = 0;

cleanup:
  if (textTexture)
    SDL_DestroyTexture(textTexture);

  if (renderer)
    SDL_DestroyRenderer(renderer);

  if (window)
    SDL_DestroyWindow(window);

  if (mediumFont)
    TTF_CloseFont(mediumFont);

  TTF_Quit();
  SDL_Quit();

  return result;
}