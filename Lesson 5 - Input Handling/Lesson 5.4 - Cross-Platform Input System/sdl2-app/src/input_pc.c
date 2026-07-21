#ifndef PLATFORM_PS2

#include "input.h"

int Input_Init()
{
    return 0;
}

void Input_Poll(InputState *input)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            input->quit = true;
            return;
        }
    }
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    input->up = keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP];
    input->down = keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN];
    input->left = keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT];
    input->right = keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT];
    input->fire = keyboard[SDL_SCANCODE_SPACE];
    input->quit = keyboard[SDL_SCANCODE_ESCAPE];
}

#endif