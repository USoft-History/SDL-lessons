# Lesson 5.3 - Moving Square

В этом уроке мы объединим обработку ввода и отрисовку объектов, чтобы создать управляемый квадрат.

Что изучим:

* напишем с вами кросплатформенное решение для перемещения квадрата на ПК и PlayStation 2

* изменение координат объекта в игровом цикле;
* ограничение движения границами экрана;
* применение ввода для управления игровыми объектами.

После урока вы сможете создавать простые управляемые объекты, которые реагируют на действия игрока на разных платформах.


---

## Алгоритм

1. Взять за основу урок `Lesson 1 - Application Loop Fundamentals`, скопировать из этого урока sdl2-app в вашу рабочую папку

### PS2

2. Нужно подключить:

```cpp
#ifdef PLATFORM_PS2
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
```

3. Для получения событий от джойстика нужно обьявить:

```cpp
static u8 padBuf[256] __attribute__((aligned(64)));
static struct padButtonStatus buttons;
```

4. Создать функцию инициализации драйверов джойстика:

```cpp
void initPadDrivers()
{
  SifInitRpc(0);
  SifLoadModule("rom0:SIO2MAN", 0, NULL);
  SifLoadModule("rom0:PADMAN", 0, NULL);
  padInit(0);
}
```

5. В функции `initScene()` открыть первый порт геймпада:

```cpp
initPadDrivers();

if (!padPortOpen(0, 0, padBuf))
{
  printf("padPortOpen failed\n");
  return 1;
}
```

6. Создать прямоугольник, который будет перемещаться по экрану:

```cpp
static SDL_Rect rect = {350, 250, 200, 100};
```

7. В функции `pollEvent()` получить текущее состояние геймпада:

```cpp
int state = padGetState(0, 0);

if (state != PAD_STATE_STABLE &&
    state != PAD_STATE_FINDCTP1)
{
  return;
}
```

8. Считать нажатые кнопки:
```cpp
if (padRead(0, 0, &buttons) > 0)
{
    u16 btns = ~buttons.btns;
}
```

9. Обработать кнопки:
```cpp
    if (btns & PAD_UP)
    {
      rect.y -= 1;
    }
    if (btns & PAD_DOWN)
    {
      rect.y += 1;
    }
    if (btns & PAD_LEFT)
    {
      rect.x -= 1;
    }
    if (btns & PAD_RIGHT)
    {
      rect.x += 1;
    }
```

---

### PC

10. Создать функцию обработки клавиатуры:
```cpp
void handleKey(SDL_KeyboardEvent *key)
{
  if (key->keysym.sym == SDLK_ESCAPE)
    running = false;

  if (key->keysym.sym == SDLK_w)
    rect.y -= 10;

  if (key->keysym.sym == SDLK_s)
    rect.y += 10;

  if (key->keysym.sym == SDLK_a)
    rect.x -= 10;

  if (key->keysym.sym == SDLK_d)
    rect.x += 10;
}
```

11. Вызвать `void handleKey(SDL_KeyboardEvent *key)` из `while (SDL_PollEvent(&event))`

```cpp

SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      running = false;
      break;

    // Mouse
    case SDL_MOUSEBUTTONUP:
      SDL_Log(
          "Mouse Button Up: %d (x=%d, y=%d)",
          event.button.button, event.motion.x, event.motion.y);

      rect.x = event.motion.x - (rect.w / 2);
      rect.y = event.motion.y - (rect.h / 2);
      break;

      // Keyboard

    case SDL_KEYDOWN:
      handleKey(&event.key); // CALL
      break;
    }
  }

```

 ---

### Отрисовка
12. В главном цикле очистить экран:

```cpp
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
SDL_RenderClear(renderer);
```

13. Установить цвет квадрата:
```cpp
SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
```

14. Нарисовать прямоугольник:
```cpp
SDL_RenderFillRect(renderer, &rect);
```

15. Вывести результат на экран:
```cpp
SDL_RenderPresent(renderer);
```

После запуска программы квадрат будет перемещаться по экрану клавишами W A S D на ПК и крестовиной геймпада на PlayStation 2.

## Вывод

В этом уроке мы создали простой управляемый объект и научились изменять его положение на экране в зависимости от действий пользователя.

Что было сделано:

* реализована обработка ввода с клавиатуры на ПК;
* реализована обработка ввода с геймпада PlayStation 2;
* создан объект с использованием SDL_Rect;
* выполнено изменение координат объекта во время работы программы;
* изучена отрисовка объекта в игровом цикле;
* подготовлена единая логика для работы на разных платформах.

Теперь вы понимаете базовый принцип управления игровыми объектами через ввод пользователя. На этом фундаменте строится движение персонажей, техники, курсоров, камер и большинства интерактивных элементов в играх.