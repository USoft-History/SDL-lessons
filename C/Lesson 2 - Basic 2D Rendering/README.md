# Lesson 2 — Basic 2D Rendering

В этом уроке мы познакомимся с базовыми возможностями рисования в SDL2 через SDL_Renderer.

Научимся:

* задавать цвет рисования;
* рисовать точки;
* рисовать линии;
* рисовать прямоугольники;
* рисовать закрашенные прямоугольники.

Все примеры будут работать как на ПК, так и на PlayStation 2 без изменений исходного кода.

---

## Что изучим

### Цвета

Перед рисованием любого примитива необходимо установить цвет:

```SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);```

Параметры:

`R - красный   (0-255)`

`G - зелёный   (0-255)`

`B - синий     (0-255)`

`A - альфа     (0-255)`


Пример:

```SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);```

Белый цвет.

---

### Point

Точка является самым простым графическим примитивом.

```SDL_RenderDrawPoint(renderer, 100, 100);```

Где:

100 - координата X

100 - координата Y

Пример:

```cpp
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
SDL_RenderDrawPoint(renderer, 100, 100);
```

---

### Line

Для рисования линии используется:

```cpp
SDL_RenderDrawLine(renderer,
                   x1, y1,
                   x2, y2);
```

Пример:
```cpp
SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

SDL_RenderDrawLine(renderer,
                   50, 50,
                   300, 200);
```

---

### Rectangle

Для рисования контура прямоугольника используется структура SDL_Rect.
```cpp
SDL_Rect rect =
{
    100,
    100,
    200,
    100
};
```

Где:

```cpp
x = 100
y = 100
width = 200
height = 100
```

Рисование:

```SDL_RenderDrawRect(renderer, &rect);```

Полный пример:

```cpp
SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

SDL_Rect rect =
{
    100,
    100,
    200,
    100
};

SDL_RenderDrawRect(renderer, &rect);
```

---

### Filled Rectangle

Для рисования закрашенного прямоугольника используется:

```SDL_RenderFillRect(renderer, &rect);```

Пример:

```cpp
SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

SDL_Rect rect =
{
    350,
    100,
    200,
    100
};

SDL_RenderFillRect(renderer, &rect);
```

---

### Итоговый пример

```cpp
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
SDL_RenderClear(renderer);

/* Point */
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
SDL_RenderDrawPoint(renderer, 100, 100);

/* Line */
SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
SDL_RenderDrawLine(renderer, 50, 50, 300, 200);

/* Rectangle */
SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

SDL_Rect rect1 =
{
    100,
    250,
    200,
    100
};

SDL_RenderDrawRect(renderer, &rect1);

/* Filled Rectangle */
SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

SDL_Rect rect2 =
{
    350,
    250,
    200,
    100
};

SDL_RenderFillRect(renderer, &rect2);

SDL_RenderPresent(renderer);
```

---

## Результат

После запуска приложения на экране должны отображаться:

* одна точка;
* одна линия;
* прямоугольник с контуром;
* закрашенный прямоугольник.

Таким образом мы познакомились с основными графическими примитивами SDL2 и подготовились к следующему уроку!