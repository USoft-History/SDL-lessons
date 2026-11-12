#ifdef PLATFORM_PS2
#include <kernel.h>
#include <sifrpc.h>
#include <iopheap.h>
#include <sbv_patches.h>
#endif

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text_label.h"

#ifdef PLATFORM_PS2
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448
#define WINDOW_FLAGS 0
#define RENDERER_FLAGS 0

#define MOVE_SPEED 1
#else
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

#define MOVE_SPEED 5
#endif

const char *const WINDOW_TITLE = "SDL2 Lesson 5.2";

bool running = true;

static void handleJoystickEvent(SDL_Joystick *joystick, SDL_Rect *rect)
{
    if (joystick)
    {
        if (SDL_JoystickGetButton(joystick, 4))
            rect->y -= MOVE_SPEED; // UP
        if (SDL_JoystickGetButton(joystick, 5))
            rect->x += MOVE_SPEED; // RIGHT
        if (SDL_JoystickGetButton(joystick, 6))
            rect->y += MOVE_SPEED; // DOWN
        if (SDL_JoystickGetButton(joystick, 7))
            rect->x -= MOVE_SPEED; // LEFT
    }
}

static void handleKeyDown(SDL_Keycode code, SDL_Rect *rect)
{
    if (code == SDLK_ESCAPE)
    {
        running = 0;
    }

    if (code == SDLK_LEFT)
    {
        rect->x -= MOVE_SPEED;
    }
    if (code == SDLK_RIGHT)
    {
        rect->x += MOVE_SPEED;
    }

    if (code == SDLK_UP)
    {
        rect->y -= MOVE_SPEED;
    }
    if (code == SDLK_DOWN)
    {
        rect->y += MOVE_SPEED;
    }
}

int main(int argc, char *argv[])
{
#ifdef PLATFORM_PS2
    SifInitRpc(0);
    sbv_patch_enable_lmb();
#endif

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Joystick *joystick = NULL;

    TTF_Font *smallFont = NULL;

    TextLabel fpsLabel = {0};

    int exitCode = -1;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        SDL_Log("SDL Init Error: %s", SDL_GetError());

        return -1;
    }

    if (TTF_Init() == -1)
    {
        SDL_Log("TTF_Init Failed: %s", TTF_GetError());

        goto cleanup;
    }

    smallFont = TTF_OpenFont("assets/font.ttf", 10);
    if (!smallFont)
    {
        SDL_Log("TTF_OpenFont error: %s", TTF_GetError());

        goto cleanup;
    }

    TextLabels_Init(SCREEN_WIDTH, SCREEN_HEIGHT);

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);

    if (!window)
    {
        SDL_Log("SDL Create Window Error: %s", SDL_GetError());

        goto cleanup;
    }

    renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);

    if (!renderer)
    {
        SDL_Log("SDL Create Renderer Error: %s", SDL_GetError());

        goto cleanup;
    }

    int joystickCount = SDL_NumJoysticks();

    SDL_Log("There are %d joysticks attached", joystickCount);

    if (joystickCount > 0)
    {
        joystick = SDL_JoystickOpen(0);

        if (!joystick)
        {
            SDL_Log("Couldn't open joystick %d: %s", 0, SDL_GetError());
        }
    }

    SDL_Color white = {255, 255, 255};

    int frameCount = 0;
    int fps = 0;

    Uint32 lastFPSUpdateTime = SDL_GetTicks();

    char fpsText[64];

    snprintf(fpsLabel.text, sizeof(fpsLabel.text), "FPS: 0");

    fpsLabel.font = smallFont;
    fpsLabel.color = white;
    fpsLabel.baseX = 10;
    fpsLabel.baseY = 10;
    fpsLabel.align = TEXT_ALIGN_NONE;

    TextLabel_Create(&fpsLabel, renderer);

    SDL_Rect rect = {
        (SCREEN_WIDTH - 100) / 2,
        (SCREEN_HEIGHT - 100) / 2,
        100,
        100};

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_JOYBUTTONDOWN:
                SDL_Log("Joystick %ld button %d down", (long)event.jbutton.which,
                        event.jbutton.button);
                break;

            case SDL_KEYDOWN:
                handleKeyDown(event.key.keysym.sym, &rect);
                break;
            }
        }

        handleJoystickEvent(joystick, &rect);

        frameCount++;

        Uint32 currentTime = SDL_GetTicks();

        if (currentTime - lastFPSUpdateTime >= 1000)
        {
            fps = frameCount;
            frameCount = 0;
            lastFPSUpdateTime = currentTime;

            snprintf(fpsText, sizeof(fpsText), "FPS: %d", fps);

            TextLabel_SetText(&fpsLabel, renderer, fpsText);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // RECT
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        // UI
        TextLabel_Render(renderer, &fpsLabel);

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

cleanup:

    TextLabel_Destroy(&fpsLabel);

    if (joystick)
        SDL_JoystickClose(joystick);

    if (smallFont)
        TTF_CloseFont(smallFont);

    if (renderer)
        SDL_DestroyRenderer(renderer);

    if (window)
        SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return exitCode;
}
