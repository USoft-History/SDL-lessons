#ifdef PLATFORM_PS2
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <iopheap.h>
#include <sbv_patches.h>
#endif

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448

// SDL_PS2_SKIP_IOP_RESET();

int main(int argc, char *argv[])
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Joystick *joystick = NULL;

    int exit_code = 0;

#ifdef PLATFORM_PS2
    SifInitRpc(0);
    // SifLoadFileInit();
    // SifInitIopHeap();
    sbv_patch_enable_lmb();
#endif

    SDL_Log("Before SDL_Init");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        return -1;
    }

    SDL_Log("After SDL_Init");

    window = SDL_CreateWindow("Joystick testing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (!window)
    {
        goto cleanup;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        goto cleanup;
    }

    SDL_Log("There are %d joysticks attached", SDL_NumJoysticks());

    joystick = SDL_JoystickOpen(0);

    if (!joystick)
    {
        SDL_Log("Couldn't open joystick %d: %s", 0, SDL_GetError());
        goto cleanup;
    }

    int running = 1;

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
                SDL_Log("Joystick %d button %d down",
                        event.jbutton.which,
                        event.jbutton.button);
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

cleanup:

    if (joystick)
    {
        SDL_JoystickClose(joystick);
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }

    if (window)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();

    return exit_code;
}