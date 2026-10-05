#ifdef PLATFORM_PS2
#include "input.h"

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>

static u8 padBuf[256] __attribute__((aligned(64)));
static struct padButtonStatus buttons;

void initPadDrivers()
{
    SDL_Log("initPadDrivers()");
    SifInitRpc(0);
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);
    padInit(0);
    SDL_Log("initPadDrivers(): OK");
}

int Input_Init()
{
    initPadDrivers();

    if (!padPortOpen(0, 0, padBuf))
    {
        SDL_Log("padPortOpen failed");
        return 1;
    }

    return 0;
}

void Input_Poll(InputState *input)
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
        input->up = btns & PAD_UP;
        input->down = btns & PAD_DOWN;
        input->left = btns & PAD_LEFT;
        input->right = btns & PAD_RIGHT;
        input->fire = btns & PAD_CROSS;
    }
}
#endif