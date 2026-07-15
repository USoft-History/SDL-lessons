# Lesson 5 — Time and Delta Time

В этом уроке мы познакомимся с управлением временем в SDL2 и научимся делать движение объектов независимым от производительности компьютера.

## Что изучим

* `SDL_GetTicks()`
* FPS (Frames Per Second)
* Delta Time
* Независимое от FPS движение объектов
* Ограничение частоты кадров

---

## Теория

### SDL_GetTicks()

Функция `SDL_GetTicks()` возвращает количество миллисекунд, прошедших с момента запуска SDL.

```c
Uint32 time = SDL_GetTicks();
```

Пример:

```c
printf("Time: %u ms\n", SDL_GetTicks());
```

Если приложение работает 5 секунд, функция вернёт примерно:

```text
5000
```

---

### FPS (Frames Per Second)

FPS показывает количество кадров, отображаемых за одну секунду.

Примеры:

| FPS | Время одного кадра |
| --- | ------------------ |
| 30  | 33.3 мс            |
| 60  | 16.6 мс            |
| 120 | 8.3 мс             |

Чем выше FPS, тем плавнее выглядит анимация.

---

### Проблема зависимости от FPS

Предположим, объект перемещается на 5 пикселей каждый кадр:

```c
x += 5;
```

На компьютере с 30 FPS:

```text
5 × 30 = 150 пикселей/сек
```

На компьютере с 120 FPS:

```text
5 × 120 = 600 пикселей/сек
```

В результате скорость объекта зависит от производительности системы.

---

## Delta Time

Delta Time — время, прошедшее между двумя кадрами.

Обычно вычисляется так:

```c
Uint32 currentTime = SDL_GetTicks();

float deltaTime =
    (currentTime - previousTime) / 1000.0f;

previousTime = currentTime;
```

Переводим миллисекунды в секунды делением на `1000.0f`.

---

## Независимое от FPS движение

Теперь зададим скорость объекта в пикселях в секунду:

```c
float speed = 200.0f;
```

Обновление позиции:

```c
x += speed * deltaTime;
```

Пример:

### 60 FPS

```text
deltaTime = 0.016
```

```text
200 × 0.016 = 3.2 пикселя
```

---

### 30 FPS

```text
deltaTime = 0.033
```

```text
200 × 0.033 = 6.6 пикселя
```

Несмотря на разное количество кадров, объект пройдёт одинаковое расстояние за секунду.

---

## Ограничение частоты кадров

Иногда необходимо ограничить FPS.

Для этого используется:

```c
SDL_Delay()
```

Пример ограничения до 60 FPS:

```c
const Uint32 frameDelay = 1000 / 60;

Uint32 frameStart = SDL_GetTicks();

/* логика и рендеринг */

Uint32 frameTime =
    SDL_GetTicks() - frameStart;

if (frameTime < frameDelay)
{
    SDL_Delay(frameDelay - frameTime);
}
```

---

## Пример главного цикла

```c
Uint32 previousTime = SDL_GetTicks();

while (running)
{
    Uint32 currentTime = SDL_GetTicks();

    float deltaTime =
        (currentTime - previousTime) / 1000.0f;

    previousTime = currentTime;

    x += speed * deltaTime;

    SDL_RenderClear(renderer);

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}
```

---

## Что получилось

В результате этого урока вы научились:

* получать текущее время через `SDL_GetTicks()`;
* понимать принцип работы FPS;
* вычислять Delta Time;
* делать движение независимым от производительности системы;
* ограничивать частоту кадров через `SDL_Delay()`.

---
