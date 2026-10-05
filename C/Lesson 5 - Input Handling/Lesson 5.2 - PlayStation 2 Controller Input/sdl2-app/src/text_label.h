#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum
{
    TEXT_ALIGN_NONE,
    TEXT_ALIGN_CENTER_X,
    TEXT_ALIGN_CENTER_Y,
    TEXT_ALIGN_CENTER
} TextAlign;

typedef struct
{
    char text[128];

    SDL_Color color;

    SDL_Texture *texture;

    SDL_Rect rect;

    TTF_Font *font;

    TextAlign align;

    int baseX;

    int baseY;
} TextLabel;

void TextLabels_Init(int width, int height);

void TextLabel_Create(TextLabel *label, SDL_Renderer *renderer);

void TextLabel_SetText(
    TextLabel *label,
    SDL_Renderer *renderer,
    const char *text);

void TextLabel_Render(SDL_Renderer *renderer, TextLabel *label);

void TextLabel_Destroy(TextLabel *label);

#endif