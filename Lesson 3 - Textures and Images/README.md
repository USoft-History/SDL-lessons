# Lesson 3 — Textures and Images

В этом уроке мы познакомимся с загрузкой и отображением изображений в SDL2.

До этого момента мы рисовали только примитивы (линии, точки и прямоугольники). Теперь научимся работать с текстурами и выводить на экран полноценные изображения.

## Что изучим

В уроке будут рассмотрены следующие темы:

* `SDL_Surface`
* `SDL_Texture`
* Загрузка изображений через `SDL_LoadBMP`
* Использование библиотеки SDL2_image
* `SDL_CreateTextureFromSurface`
* `SDL_RenderCopy`
* Отрисовка изображения на экране

---

## Что такое SDL_Surface

`SDL_Surface` представляет собой изображение, находящееся в оперативной памяти.

Обычно изображение сначала загружается в поверхность (`SDL_Surface`), после чего преобразуется в текстуру (`SDL_Texture`) для отображения через рендерер.

Схема работы:

```text
Файл изображения
        ↓
   SDL_Surface
        ↓
SDL_CreateTextureFromSurface
        ↓
   SDL_Texture
        ↓
 SDL_RenderCopy
        ↓
      Экран
```

---

## Что такое SDL_Texture

`SDL_Texture` — объект, который используется рендерером для вывода изображения на экран.

Текстуры обычно располагаются в памяти графической подсистемы и обеспечивают более быструю отрисовку.

Для большинства игровых объектов используются именно текстуры.

---

## Загрузка BMP изображения

Самый простой способ загрузить изображение в SDL2 — использовать функцию `SDL_LoadBMP`.

Пример:

```cpp
SDL_Surface* surface = SDL_LoadBMP("assets/player.bmp");

if (!surface)
{
    SDL_Log("Failed to load image: %s", SDL_GetError());
}
```

После выполнения мы получим объект `SDL_Surface`.

---

# Создание текстуры

После загрузки изображения необходимо создать текстуру:

```cpp
SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
```

После создания текстуры поверхность больше не нужна:

```cpp
SDL_FreeSurface(surface);
```

Полный пример:

```cpp
SDL_Surface* surface = SDL_LoadBMP("assets/player.bmp");

SDL_Texture* texture =
    SDL_CreateTextureFromSurface(renderer, surface);

SDL_FreeSurface(surface);
```

---

# Отрисовка текстуры

Для отображения изображения используется функция:

```cpp
SDL_RenderCopy();
```

Пример:

```cpp
SDL_Rect dstRect;

dstRect.x = 100;
dstRect.y = 100;
dstRect.w = 64;
dstRect.h = 64;

SDL_RenderCopy(
    renderer,
    texture,
    nullptr,
    &dstRect
);
```

Параметры:

| Параметр | Описание                                         |
| -------- | ------------------------------------------------ |
| renderer | SDL_Renderer                                     |
| texture  | Текстура для отрисовки                           |
| srcRect  | Область внутри текстуры (nullptr = вся текстура) |
| dstRect  | Область на экране                                |

---

# Использование SDL2_image

`SDL_LoadBMP` умеет работать только с BMP-файлами.

Для поддержки современных форматов изображений используется библиотека SDL2_image.

Она позволяет загружать:

* PNG
* JPG
* JPEG
* WEBP
* TIFF
* и другие форматы

Подключение:

```cpp
#include <SDL2/SDL_image.h>
```

Загрузка PNG:

```cpp
SDL_Surface* surface =
    IMG_Load("assets/player.png");
```

Дальнейшая работа полностью аналогична:

```cpp
SDL_Texture* texture =
    SDL_CreateTextureFromSurface(renderer, surface);

SDL_FreeSurface(surface);
```

---

# Полный пример

```cpp
SDL_Surface* surface =
    SDL_LoadBMP("assets/player.bmp");

if (!surface)
{
    SDL_Log("Failed to load image");
    return -1;
}

SDL_Texture* texture =
    SDL_CreateTextureFromSurface(renderer, surface);

SDL_FreeSurface(surface);

SDL_Rect dstRect =
{
    100,
    100,
    64,
    64
};

SDL_RenderCopy(
    renderer,
    texture,
    nullptr,
    &dstRect
);
```

---

# Освобождение ресурсов

После завершения работы необходимо уничтожить текстуру:

```cpp
SDL_DestroyTexture(texture);
```

Это предотвращает утечки памяти.

---

# Итоги

В этом уроке мы изучили:

* Что такое `SDL_Surface`
* Что такое `SDL_Texture`
* Как загрузить BMP-файл через `SDL_LoadBMP`
* Как использовать SDL2_image
* Как создать текстуру через `SDL_CreateTextureFromSurface`
* Как вывести изображение через `SDL_RenderCopy`
* Как правильно освобождать ресурсы

