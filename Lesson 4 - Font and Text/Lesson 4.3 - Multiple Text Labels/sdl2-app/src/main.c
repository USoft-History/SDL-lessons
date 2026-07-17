#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text_label.h"

#ifdef PLATFORM_PS2
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448
#else
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#endif

static const char *const WINDOW_TITLE = "SDL2 Lesson 4.3 - Multiple Text Labels";

static const SDL_Color white = {255, 255, 255, 255};
static const SDL_Color yellow = {255, 255, 0, 255};
static const SDL_Color green = {0, 255, 0, 255};

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

    return 1;
  }

  TTF_Font *smallFont = TTF_OpenFont("assets/font.ttf", 18);
  if (!smallFont)
  {
    SDL_Log("TTF_OpenFont error: %s\n", TTF_GetError());

    return 1;
  }

  TextLabels_Init(SCREEN_WIDTH, SCREEN_HEIGHT);

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

    TTF_Quit();
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

    TTF_Quit();
    SDL_Quit();

    return 1;
  }

  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

#endif

  int frameCount = 0;
  int fps = 0;
  Uint32 lastFPSUpdate = SDL_GetTicks();

  char fpsText[64];

  TextLabel titleLabel = {
      .font = mediumFont,
      .color = white,
      .baseY = 20,
      .align = TEXT_ALIGN_CENTER_X};

  strcpy(titleLabel.text, WINDOW_TITLE);

  TextLabel_Create(&titleLabel, renderer);

  // FPS Label

  TextLabel fpsLabel = {
      .text = "FPS: 0",
      .font = smallFont,
      .color = yellow,
      .baseX = 50,
      .baseY = 100,
      .align = TEXT_ALIGN_NONE};

  TextLabel_Create(&fpsLabel, renderer);

  // Label 3
  // SDL_Surface *surfaceScore =
  //     TTF_RenderUTF8_Blended(
  //         smallFont,
  //         "Score: 100",
  //         green);

  // SDL_Texture *textureScore =
  //     SDL_CreateTextureFromSurface(
  //         renderer,
  //         surfaceScore);

  // SDL_Rect rectScore = {
  //     50,
  //     150,
  //     surfaceScore->w,
  //     surfaceScore->h};

  // SDL_FreeSurface(surfaceScore);

  TextLabel scoreLabel = {
      .text = "Score: 1000",
      .font = smallFont,
      .color = green,
      .baseX = 50,
      .baseY = 150,
  };

  TextLabel_Create(&scoreLabel, renderer);

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

      TextLabel_SetText(&fpsLabel, renderer, fpsText);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TextLabel_Render(renderer, &titleLabel);
    TextLabel_Render(renderer, &fpsLabel);
    TextLabel_Render(renderer, &scoreLabel);

    SDL_RenderPresent(renderer);
  }

  TextLabel_Destroy(&titleLabel);
  TextLabel_Destroy(&fpsLabel);
  TextLabel_Destroy(&scoreLabel);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_CloseFont(mediumFont);
  TTF_CloseFont(smallFont);

  TTF_Quit();
  SDL_Quit();

  return 0;
}