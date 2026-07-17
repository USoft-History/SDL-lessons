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

const char *const WINDOW_TITLE = "Lesson 4.2 - FPS Counter";

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

    SDL_Quit();
    
    return 1;
  }

  TTF_Font *mediumFont = TTF_OpenFont("assets/font.ttf", 24);
  if (!mediumFont)
  {
    SDL_Log("TTF_OpenFont error: %s\n", TTF_GetError());

    TTF_Quit();
    SDL_Quit();

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

  SDL_Color white = {255, 255, 255, 255};

  int frameCount = 0;
  int fps = 0;

  Uint32 lastFPSUpdate = SDL_GetTicks();

  char fpsText[64];

  snprintf(fpsText, sizeof(fpsText), "FPS: %d", fps);

  SDL_Surface *fpsSurface =
      TTF_RenderUTF8_Blended(
          mediumFont,
          fpsText,
          white);

  SDL_Texture *fpsTexture =
      SDL_CreateTextureFromSurface(
          renderer,
          fpsSurface);

  SDL_Rect fpsRect = {
      (SCREEN_WIDTH - fpsSurface->w) / 2,
      (SCREEN_HEIGHT - fpsSurface->h) / 2,
      fpsSurface->w,
      fpsSurface->h};

  SDL_FreeSurface(fpsSurface);

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

    frameCount++;

    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastFPSUpdate >= 1000)
    {
      fps = frameCount;
      frameCount = 0;
      lastFPSUpdate = currentTime;

      snprintf(
          fpsText,
          sizeof(fpsText),
          "FPS: %d",
          fps);

      SDL_Surface *newSurface =
          TTF_RenderUTF8_Blended(
              mediumFont,
              fpsText,
              white);

      if (!newSurface)
      {
        SDL_Log("TTF_RenderUTF8_Blended error: %s",
                TTF_GetError());
      }
      else
      {

        SDL_DestroyTexture(fpsTexture);

        fpsTexture =
            SDL_CreateTextureFromSurface(
                renderer,
                newSurface);

        fpsRect.w = newSurface->w;
        fpsRect.h = newSurface->h;

        fpsRect.x =
            (SCREEN_WIDTH - fpsRect.w) / 2;

        fpsRect.y =
            (SCREEN_HEIGHT - fpsRect.h) / 2;

        SDL_FreeSurface(newSurface);
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, fpsTexture, NULL, &fpsRect);

    SDL_RenderPresent(renderer);
  }

  if (fpsTexture)
    SDL_DestroyTexture(fpsTexture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_CloseFont(mediumFont);

  TTF_Quit();
  SDL_Quit();

  return 0;
}