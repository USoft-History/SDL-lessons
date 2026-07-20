#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text_label.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const char *const WINDOW_TITLE = "SDL2 Lesson 5.1";

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
  }

  if (TTF_Init() == -1)
  {
    SDL_Log("TTF_Init FAILED: %s\n", TTF_GetError());

    return 1;
  }

  TTF_Font *smallFont = TTF_OpenFont("assets/font.ttf", 18);
  if (!smallFont)
  {
    SDL_Log("TTF_OpenFont error: %s\n", TTF_GetError());

    return 1;
  }

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

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window,
      -1,
      0);

  if (!renderer)
  {
    SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
  }

  TextLabels_Init(SCREEN_WIDTH, SCREEN_HEIGHT);

  TextLabel keyCodeLabel = {
      .font = smallFont,
      .color = {0, 255, 0, 255},
      .text = "Key Code: 0",
      .baseY = 20,
      .align = TEXT_ALIGN_CENTER};

  TextLabel_Create(&keyCodeLabel, renderer);

  bool running = true;

  while (running)
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = false;
        break;

      // Mouse

      case SDL_MOUSEMOTION:
        SDL_Log(
            "Mouse Position: %d, %d",
            event.motion.x,
            event.motion.y);
        break;

      case SDL_MOUSEBUTTONDOWN:
        SDL_Log(
            "Mouse Button Down: %d",
            event.button.button);
        break;

      case SDL_MOUSEBUTTONUP:
        SDL_Log(
            "Mouse Button Up: %d",
            event.button.button);
        break;

      case SDL_MOUSEWHEEL:
        SDL_Log(
            "Mouse Wheel: %d",
            event.wheel.y);
        break;

        // Keyboard

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
          running = false;
        }

        if (event.key.keysym.sym == SDLK_SPACE)
        {
          SDL_Log("SPACE pressed");
        }

        char keyCodeText[64];

        snprintf(
            keyCodeText,
            sizeof(keyCodeText),
            "Key Code: %u (%s)",
            event.key.keysym.sym, SDL_GetKeyName(event.key.keysym.sym));

        TextLabel_SetText(&keyCodeLabel, renderer, keyCodeText);

        break;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_SPACE)
        {
          SDL_Log("SPACE released");
        }
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TextLabel_Render(renderer, &keyCodeLabel);

    SDL_RenderPresent(renderer);
  }

  TextLabel_Destroy(&keyCodeLabel);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}