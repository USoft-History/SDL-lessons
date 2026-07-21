#include <stdbool.h>

#include <SDL2/SDL.h>

#ifdef PLATFORM_PS2
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448

static u8 padBuf[256] __attribute__((aligned(64)));
static struct padButtonStatus buttons;

#else
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#endif

static const char *const WINDOW_TITLE = "SDL2 Lesson 5.3";

static bool running = true;

static SDL_Rect rect = {350, 250, 200, 100};

#ifdef PLATFORM_PS2

void initPadDrivers()
{
  SDL_Log("initPadDrivers()");
  SifInitRpc(0);
  SifLoadModule("rom0:SIO2MAN", 0, NULL);
  SifLoadModule("rom0:PADMAN", 0, NULL);
  padInit(0);
  SDL_Log("initPadDrivers(): OK");
}

int initScene()
{
  initPadDrivers();

  if (!padPortOpen(0, 0, padBuf))
  {
    printf("padPortOpen failed\n");
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
  }

  return 0;
}

void pollEvent()
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

    if (btns & PAD_UP)
    {
      rect.y -= 1;
    }
    if (btns & PAD_DOWN)
    {
      rect.y += 1;
    }
    if (btns & PAD_LEFT)
    {
      rect.x -= 1;
    }
    if (btns & PAD_RIGHT)
    {
      rect.x += 1;
    }
  }
}
#else

int initScene()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("SDL Init Error: %s", SDL_GetError());

    return 1;
  }

  return 0;
}

void handleKey(SDL_KeyboardEvent *key)
{
  if (key->keysym.sym == SDLK_ESCAPE)
    running = false;

  if (key->keysym.sym == SDLK_w)
    rect.y -= 10;

  if (key->keysym.sym == SDLK_s)
    rect.y += 10;

  if (key->keysym.sym == SDLK_a)
    rect.x -= 10;

  if (key->keysym.sym == SDLK_d)
    rect.x += 10;
}

void pollEvent()
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
    case SDL_MOUSEBUTTONUP:
      SDL_Log(
          "Mouse Button Up: %d (x=%d, y=%d)",
          event.button.button, event.motion.x, event.motion.y);

      rect.x = event.motion.x - (rect.w / 2);
      rect.y = event.motion.y - (rect.h / 2);
      break;

      // Keyboard

    case SDL_KEYDOWN:
      handleKey(&event.key);
      break;
    }
  }
}

#endif

int main(int argc, char *argv[])
{
  initScene();

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
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

#else

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

#endif

  while (running)
  {
    pollEvent();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /* Filled Rectangle */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}