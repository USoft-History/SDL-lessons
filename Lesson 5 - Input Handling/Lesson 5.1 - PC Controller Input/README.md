# Lesson 4.1 - PC Controller Input

В этом уроке мы научимся работать с пользовательским вводом на ПК с помощью SDL2.

SDL предоставляет единый API для работы с:

* Клавиатурой
* Мышью
* Геймпадами
* Джойстиками
* Событиями окна

После изучения урока вы сможете:

* Обрабатывать нажатия клавиш.
* Реагировать на отпускание клавиш.
* Работать с игровыми контроллерами.
* Использовать систему событий SDL.
* Реализовывать управление игровым объектом.

---

## Что изучим

Основные функции и структуры SDL для обработки ввода:

```c
SDL_Event
SDL_PollEvent()

SDL_KEYDOWN
SDL_KEYUP

SDL_GameController
SDL_Joystick
```

---

## Инициализация SDL

Для работы с клавиатурой и геймпадами необходимо инициализировать SDL:

```c
if (SDL_Init(SDL_INIT_VIDEO |
             SDL_INIT_AUDIO |
             SDL_INIT_JOYSTICK |
             SDL_INIT_GAMECONTROLLER) != 0)
{
    SDL_Log("SDL_Init Error: %s", SDL_GetError());
    return 1;
}
```

---

## Система событий SDL

SDL использует очередь событий.

Каждый кадр необходимо получать и обрабатывать события:

```c
SDL_Event event;

while (SDL_PollEvent(&event))
{
    // обработка событий
}
```

---

## Закрытие окна

Для завершения приложения используется событие:

```c
SDL_QUIT
```

Пример:

```c
while (SDL_PollEvent(&event))
{
    if (event.type == SDL_QUIT)
    {
        running = 0;
    }
}
```

---

## Обработка клавиатуры

SDL генерирует события:

```c
SDL_KEYDOWN
SDL_KEYUP
```

Пример:

```c
if (event.type == SDL_KEYDOWN)
{
    SDL_Log("Key pressed");
}
```

---

## Проверка конкретной клавиши

Каждая клавиша имеет собственный идентификатор.

Например:

```c
if (event.type == SDL_KEYDOWN)
{
    if (event.key.keysym.sym == SDLK_ESCAPE)
    {
        running = 0;
    }
}
```

---

## Часто используемые клавиши

```c
SDLK_UP
SDLK_DOWN
SDLK_LEFT
SDLK_RIGHT

SDLK_w
SDLK_a
SDLK_s
SDLK_d

SDLK_SPACE
SDLK_RETURN
SDLK_ESCAPE
```

---

## Подключение геймпада

Количество подключенных устройств:

```c
int count = SDL_NumJoysticks();
```

Проверка поддержки Game Controller API:

```c
if (SDL_IsGameController(0))
{
    // устройство поддерживается
}
```

Открытие первого геймпада:

```c
SDL_GameController* controller =
    SDL_GameControllerOpen(0);
```

---

## Проверка успешного открытия

```c
if (!controller)
{
    SDL_Log("Controller not found");
}
```

---

## Закрытие контроллера

После завершения программы контроллер необходимо закрыть:

```c
SDL_GameControllerClose(controller);
```

---

## События геймпада

SDL автоматически генерирует события:

```c
SDL_CONTROLLERBUTTONDOWN
SDL_CONTROLLERBUTTONUP
SDL_CONTROLLERAXISMOTION
```

---

## Нажатие кнопки

Пример обработки кнопки A:

```c
if (event.type == SDL_CONTROLLERBUTTONDOWN)
{
    if (event.cbutton.button ==
        SDL_CONTROLLER_BUTTON_A)
    {
        SDL_Log("Button A");
    }
}
```

---

## Аналоговые стики

Для аналоговых стиков используется событие:

```c
SDL_CONTROLLERAXISMOTION
```

Пример:

```c
if (event.type == SDL_CONTROLLERAXISMOTION)
{
    Sint16 value = event.caxis.value;

    SDL_Log("Axis value: %d", value);
}
```

---

## Типичный цикл обработки ввода

```c
SDL_Event event;

while (SDL_PollEvent(&event))
{
    switch (event.type)
    {
        case SDL_QUIT:
            running = 0;
            break;

        case SDL_KEYDOWN:
            break;

        case SDL_KEYUP:
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            break;

        case SDL_CONTROLLERBUTTONUP:
            break;
    }
}
```

---

## Практическое задание

Создайте приложение, которое:

1. Перемещает квадрат по экрану клавишами WASD.
2. Поддерживает управление стрелками.
3. Позволяет управлять объектом с помощью геймпада.
4. Завершает приложение по нажатию клавиши Escape.
5. Выводит сообщения о нажатых кнопках в консоль.

---

## Что получилось

После завершения урока вы научитесь:

* Работать с системой событий SDL.
* Обрабатывать клавиатуру.
* Подключать игровые контроллеры.
* Получать состояние кнопок и стиков.
* Реализовывать управление игроком на ПК.

