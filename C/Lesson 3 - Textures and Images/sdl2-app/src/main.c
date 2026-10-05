
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

const char *const WINDOW_TITLE = "SDL2 Lesson 3";

SDL_Texture *LoadBMP(SDL_Renderer *renderer, const char *imagePath)
{
  SDL_Surface *surface = SDL_LoadBMP(imagePath);
  if (!surface)
  {
    SDL_Log("Failed to load bmp: %s", SDL_GetError());

    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  return texture;
}

SDL_Texture *LoadImage(SDL_Renderer *renderer, const char *imagePath)
{
  SDL_Surface *surface = IMG_Load(imagePath);
  if (!surface)
  {
    SDL_Log("Failed to load image: %s", SDL_GetError());

    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  return texture;
}

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return -1;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
  {
    SDL_Log("SDL Image Init Error: %s\n", IMG_GetError());

    SDL_Quit();

    return -1;
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
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);

  if (!renderer)
  {
    SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  SDL_Texture *fullHeartTexture = LoadBMP(renderer, "assets/heart-full.bmp");
  SDL_Texture *emptyHeartTexture = LoadImage(renderer, "assets/heart-empty.png");

  if (!fullHeartTexture || !emptyHeartTexture)
  {
    SDL_DestroyTexture(fullHeartTexture);
    SDL_DestroyTexture(emptyHeartTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return -1;
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

    SDL_Rect dstRect1 = {10, 10, 64, 64};
    SDL_RenderCopy(renderer, fullHeartTexture, NULL, &dstRect1);

    SDL_Rect dstRect2 = {SCREEN_WIDTH - 74, 10, 64, 64};
    SDL_RenderCopy(renderer, emptyHeartTexture, NULL, &dstRect2);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(fullHeartTexture);
  SDL_DestroyTexture(emptyHeartTexture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  SDL_Quit();

  return 0;
}