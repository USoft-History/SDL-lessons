# Lesson 4.1 - Hello world

В этом уроке мы познакомимся с библиотекой **SDL_ttf** и научимся отображать текст на экране.

До этого момента мы рисовали только примитивы и изображения. Теперь добавим поддержку шрифтов и выведем на экран первую текстовую строку.

---

## Что изучим

В этом уроке рассмотрим:

* Подключение SDL_ttf
* Инициализацию системы шрифтов
* Загрузку TTF-шрифта
* Создание текстовой поверхности (`SDL_Surface`)
* Создание текстуры из текста (`SDL_Texture`)
* Отрисовку текста через `SDL_RenderCopy`
* Освобождение ресурсов

---

## Используемые функции

### Инициализация SDL_ttf

```c
TTF_Init();
```

Инициализирует библиотеку SDL_ttf.

Проверка результата:

```c
if (TTF_Init() == -1)
{
    SDL_Log("TTF_Init FAILED: %s\n", TTF_GetError());
}
```

---

### Загрузка шрифта

```c
TTF_Font *mediumFont =
    TTF_OpenFont("assets/font.ttf", 24);
```

Параметры:

| Параметр     | Описание                 |
| ------------ | ------------------------ |
| Путь к файлу | TTF-шрифт                |
| Размер       | Высота шрифта в пикселях |


Пример:

```c
TTF_Font *font =
    TTF_OpenFont("assets/font.ttf", 24);
```

---

### Создание цвета текста

```c
SDL_Color white = {255, 255, 255, 255};
```

Структура:

```c
SDL_Color
{
    r,
    g,
    b,
    a
};
```

---

### Создание поверхности с текстом

```c
SDL_Surface *textSurface =
    TTF_RenderUTF8_Blended(
        mediumFont,
        "Hello World!",
        white);
```

Параметры:

| Параметр | Описание             |
| -------- | -------------------- |
| Шрифт    | Загруженный TTF_Font |
| Текст    | Строка UTF-8         |
| Цвет     | SDL_Color            |

Результат:

```c
SDL_Surface*
```

с уже нарисованным текстом.

---

### Создание текстуры

Для вывода через SDL_Renderer необходимо преобразовать поверхность в текстуру:

```c
SDL_Texture *textTexture =
    SDL_CreateTextureFromSurface(
        renderer,
        textSurface);
```

---

### Определение позиции текста

Размер текста можно получить напрямую из поверхности:

```c
textSurface->w
textSurface->h
```

Центрируем текст:

```c
SDL_Rect textRect =
{
    (SCREEN_WIDTH - textSurface->w) / 2,
    (SCREEN_HEIGHT - textSurface->h) / 2,
    textSurface->w,
    textSurface->h
};
```

---

### Освобождение поверхности

После создания текстуры поверхность больше не нужна:

```c
SDL_FreeSurface(textSurface);
```

---

### Отрисовка текста

В игровом цикле:

```c
SDL_RenderCopy(
    renderer,
    textTexture,
    NULL,
    &textRect);
```

Параметры:

| Параметр    | Описание                  |
| ----------- | ------------------------- |
| renderer    | Рендерер                  |
| textTexture | Текстура с текстом        |
| NULL        | Использовать всю текстуру |
| textRect    | Область вывода            |

---

## Игровой цикл

Каждый кадр:

```c
SDL_SetRenderDrawColor(
    renderer,
    0,
    0,
    0,
    255);

SDL_RenderClear(renderer);

SDL_RenderCopy(
    renderer,
    textTexture,
    NULL,
    &textRect);

SDL_RenderPresent(renderer);
```

Результат:

* экран очищается чёрным цветом;
* текст выводится по центру;
* кадр отображается на экране.

---

## Освобождение ресурсов

Текстура:

```c
SDL_DestroyTexture(textTexture);
```

Шрифт:

```c
TTF_CloseFont(mediumFont);
```

SDL:

```c
SDL_Quit();
```

---

## Структура проекта

```text
Lesson4-Font-And-Text/
│
├── assets/
│   └── font.ttf
│
├── src/
│   └── main.c
│
├── Makefile
│
└── README.md
```

---

## Результат

После запуска приложения на экране появится строка:

```text
Hello World!
```

расположенная по центру окна.

---

## Что нового мы узнали

В этом уроке мы познакомились с:

* SDL_ttf
* TTF_Init
* TTF_OpenFont
* SDL_Color
* TTF_RenderUTF8_Blended
* SDL_Surface
* SDL_CreateTextureFromSurface
* SDL_Texture
* SDL_RenderCopy
* TTF_CloseFont

---
