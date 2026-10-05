#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_VIDEO);

#if PLATFORM_LINUX
  SDL_Log("Hello Linux!");
#elif PLATFORM_PS2
  SDL_Log("Hello PS 2!");
#else
  SDL_Log("Hello Unknown Platform!");
#endif

  SDL_Quit();

  return 0;
}
