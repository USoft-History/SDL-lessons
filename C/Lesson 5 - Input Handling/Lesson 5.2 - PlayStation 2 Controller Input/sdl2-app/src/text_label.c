#include "text_label.h"

static int screenWidth = 0;
static int screenHeight = 0;

void TextLabels_Init(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

static void TextLabel_UpdatePosition(TextLabel *label)
{
    label->rect.x = label->baseX;
    label->rect.y = label->baseY;

    if (label->align == TEXT_ALIGN_CENTER_X ||
        label->align == TEXT_ALIGN_CENTER)
    {
        label->rect.x =
            (screenWidth - label->rect.w) / 2;
    }

    if (label->align == TEXT_ALIGN_CENTER_Y ||
        label->align == TEXT_ALIGN_CENTER)
    {
        label->rect.y =
            (screenHeight - label->rect.h) / 2;
    }
}

void TextLabel_Create(
    TextLabel *label,
    SDL_Renderer *renderer)
{
    SDL_Surface *surface =
        TTF_RenderUTF8_Blended(
            label->font,
            label->text,
            label->color);

    label->texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface);

    label->rect.w = surface->w;
    label->rect.h = surface->h;

    TextLabel_UpdatePosition(label);

    SDL_FreeSurface(surface);
}

void TextLabel_SetText(
    TextLabel *label,
    SDL_Renderer *renderer,
    const char *text)
{
    strncpy(label->text, text, sizeof(label->text) - 1);
    label->text[sizeof(label->text) - 1] = '\0';

    SDL_DestroyTexture(label->texture);

    SDL_Surface *surface =
        TTF_RenderUTF8_Blended(
            label->font,
            label->text,
            label->color);

    label->texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface);

    label->rect.w = surface->w;
    label->rect.h = surface->h;

    TextLabel_UpdatePosition(label);

    SDL_FreeSurface(surface);
}

void TextLabel_Render(SDL_Renderer *renderer, TextLabel *label)
{
    SDL_RenderCopy(
        renderer,
        label->texture,
        NULL,
        &label->rect);
}

void TextLabel_Destroy(TextLabel *label)
{
    SDL_DestroyTexture(label->texture);
    label->texture = NULL;
}
