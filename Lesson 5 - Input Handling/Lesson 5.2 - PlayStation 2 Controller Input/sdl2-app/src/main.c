#include <stdbool.h>

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text_label.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448

typedef struct
{
  u16 code;
  const char *name;
} PadButtonInfo;

static const char *const WINDOW_TITLE = "SDL2 Lesson 5.2";

static u8 padBuf[256] __attribute__((aligned(64)));
static struct padButtonStatus buttons;

static const PadButtonInfo padButtons[] = {
    {PAD_UP, "UP"},
    {PAD_DOWN, "DOWN"},
    {PAD_LEFT, "LEFT"},
    {PAD_RIGHT, "RIGHT"},
    {PAD_CROSS, "CROSS"},
    {PAD_CIRCLE, "CIRCLE"},
    {PAD_TRIANGLE, "TRIANGLE"},
    {PAD_SQUARE, "SQUARE"},
    {PAD_L1, "L1"},
    {PAD_R1, "R1"},
    {PAD_L2, "L2"},
    {PAD_R2, "R2"},
    {PAD_L3, "L3"},
    {PAD_R3, "R3"},
    {PAD_START, "START"},
    {PAD_SELECT, "SELECT"},
};

void initPadDrivers()
{
  SDL_Log("initPadDrivers()");
  SifInitRpc(0);
  SifLoadModule("rom0:SIO2MAN", 0, NULL);
  SifLoadModule("rom0:PADMAN", 0, NULL);
  padInit(0);
  SDL_Log("initPadDrivers(): OK");
}

void pollEvent(TextLabel *label, SDL_Renderer *renderer)
{
  int state = padGetState(0, 0);

  if (state != PAD_STATE_STABLE &&
      state != PAD_STATE_FINDCTP1)
  {
    return;
  }

  if (padRead(0, 0, &buttons) > 0)
  {
    u16 btns = ~buttons.btns;

    for (size_t i = 0; i < sizeof(padButtons) / sizeof(padButtons[0]); i++)
    {
      if (btns & padButtons[i].code)
      {
        char text[64];

        snprintf(
            text,
            sizeof(text),
            "Button: %s (0x%04X)",
            padButtons[i].name,
            padButtons[i].code);

        TextLabel_SetText(label, renderer, text);

        SDL_Log("%s", text);

        break;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  initPadDrivers();

  SDL_Log("try padPortOpen");

  if (!padPortOpen(0, 0, padBuf))
  {
    printf("padPortOpen failed\n");
    return 1;
  }

  SDL_Log("padPortOpen: opened\n");

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
    pollEvent(&keyCodeLabel, renderer);

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