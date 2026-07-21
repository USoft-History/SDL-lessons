#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

#include <SDL2/SDL.h>

typedef enum
{
    INPUT_NONE = 0,

    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,

    INPUT_EXIT
} InputAction;

typedef struct
{
    bool up;
    bool down;
    bool left;
    bool right;

    bool fire;
    bool fire2;

    bool start;
    bool select;

    bool quit;
} InputState;

int Input_Init();

void Input_Poll(InputState * input);

#endif