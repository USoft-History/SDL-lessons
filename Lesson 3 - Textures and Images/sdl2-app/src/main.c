
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef PLATFORM_PS2
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448
#else
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#endif

const char *const WINDOW_TITLE = "SDL2 Lesson 1";

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
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
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

  SDL_Texture *fullHeartTexture = LoadBMP(renderer, "assets/heart-full.bmp");
  if (!fullHeartTexture)
    return -1;

  // LOAD PNG
  SDL_Surface *emptyHeartSurface = IMG_Load("assets/heart-empty.png");
  if (!emptyHeartSurface)
  {
    SDL_Log("Failed to load image: %s", SDL_GetError());

    return -1;
  }

  SDL_Texture *emptyHeartTexture = SDL_CreateTextureFromSurface(renderer, emptyHeartSurface);
  SDL_FreeSurface(emptyHeartSurface);

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

  SDL_Quit();

  return 0;
}