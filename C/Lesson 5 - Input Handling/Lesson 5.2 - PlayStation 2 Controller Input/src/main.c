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
#else
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#endif

const char *const WINDOW_TITLE = "SDL2 Lesson 5.2";

int main(int argc, char *argv[])
{
#ifdef PLATFORM_PS2
    SifInitRpc(0);
    sbv_patch_enable_lmb();
#endif

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Joystick *joystick = NULL;

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

    TTF_Font *smallFont = TTF_OpenFont("assets/font.ttf", 10);
    if (!smallFont)
    {
        SDL_Log("TTF_OpenFont error: %s", TTF_GetError());

        goto cleanup;
    }

    TTF_Font *mediumFont = TTF_OpenFont("assets/font.ttf", 18);

    if (!mediumFont)
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

    TextLabel fpsLabel = {
        .text = "FPS: 0",
        .font = smallFont,
        .color = white,
        .baseX = 10,
        .baseY = 10,
        .align = TEXT_ALIGN_NONE};

    TextLabel keyCodeLabel = {
        .text = "Key Code: 0",
        .font = mediumFont,
        .color = white,
        .align = TEXT_ALIGN_CENTER};

    TextLabel_Create(&fpsLabel, renderer);
    TextLabel_Create(&keyCodeLabel, renderer);

    bool running = true;

    char keyCodeText[64];

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
            case SDL_KEYDOWN:
                SDL_Keycode key = event.key.keysym.sym;
                snprintf(keyCodeText, sizeof(keyCodeText), "Key Code: %u (%s)", key, SDL_GetKeyName(key));

                TextLabel_SetText(&keyCodeLabel, renderer, keyCodeText);
                break;
            case SDL_MOUSEMOTION:
                snprintf(keyCodeText, sizeof(keyCodeText), "Mouse Posiion: %d, %d", event.motion.x, event.motion.y);

                TextLabel_SetText(&keyCodeLabel, renderer, keyCodeText);
                break;
            case SDL_MOUSEBUTTONDOWN:
                snprintf(keyCodeText, sizeof(keyCodeText), "Mouse Button Down: %d", event.button.button);

                TextLabel_SetText(&keyCodeLabel, renderer, keyCodeText);
                break;

            case SDL_JOYBUTTONDOWN:
                snprintf(keyCodeText, sizeof(keyCodeText), "Joystick %d Button %d down",
                         event.jbutton.which, event.jbutton.button);

                TextLabel_SetText(&keyCodeLabel, renderer, keyCodeText);
                break;
            }
        }

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

        SDL_SetRenderDrawColor(renderer, 100, 25, 100, 255);
        SDL_RenderClear(renderer);

        TextLabel_Render(renderer, &fpsLabel);
        TextLabel_Render(renderer, &keyCodeLabel);

        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }

    exitCode = 0;

cleanup:

    TextLabel_Destroy(&keyCodeLabel);
    TextLabel_Destroy(&fpsLabel);

    if (joystick)
        SDL_JoystickClose(joystick);

    if (mediumFont)
        TTF_CloseFont(mediumFont);

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