# Lesson 5.1 - PC Controller Input

В этом уроке мы научимся работать с пользовательским вводом на ПК с помощью SDL2.

SDL предоставляет единый API для работы с:

* Клавиатурой
* Мышью
* Событиями окна

После изучения урока вы сможете:

* Обрабатывать нажатия клавиш.
* Обрабатывать события от мыши.
* Реагировать на отпускание клавиш.
* Использовать систему событий SDL.

---

Для того чтобы вам обрабатывать клавиши нужно:

1. Взять за основу урок `Lesson 1 - Application Loop Fundamentals`, скопировать из этого урока sdl2-app в вашу рабочую папку

2. Изменить цикл `while (SDL_PollEvent(&event))`:

**Было:**
```cpp
while (SDL_PollEvent(&event))
{
    if (event.type == SDL_QUIT)
    {
        running = false;
    }
}
```

**Стало:**
```cpp

while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYDOWN:
        SDL_Log("KEYDOWN Processing");
        break;

      case SDL_KEYUP:
        SDL_Log("KEYUP Processing");
        break;
      }
    }

```

3. Для более ясной картины давайте добавим обработку клавишь. 

Добавим закрытие окна по нажатию `ESCAPE`, и залогируем нажатие кнопки `SPACE`:

```cpp

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
          running = false;
        }

        if (event.key.keysym.sym == SDLK_SPACE)
        {
          SDL_Log("SPACE pressed");
        }

        break;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_SPACE)
        {
          SDL_Log("SPACE released");
        }
        break;
      }
    }

```

4. Еще давайте добавим обработку мыши:

```cpp

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
          running = false;
        }

        if (event.key.keysym.sym == SDLK_SPACE)
        {
          SDL_Log("SPACE pressed");
        }

        break;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_SPACE)
        {
          SDL_Log("SPACE released");
        }
        break;

        // ---- Mouse ----

      case SDL_MOUSEMOTION:
        SDL_Log(
            "Mouse Position: %d, %d",
            event.motion.x,
            event.motion.y);
        break;

      case SDL_MOUSEBUTTONDOWN:
        SDL_Log(
            "Mouse Button Down: %d",
            event.button.button);
        break;

      case SDL_MOUSEBUTTONUP:
        SDL_Log(
            "Mouse Button Up: %d",
            event.button.button);
        break;

      case SDL_MOUSEWHEEL:
        SDL_Log(
            "Mouse Wheel: %d",
            event.wheel.y);
        break;

      }
    }

```

На этом все у нас теперь есть заготовка работы с клавиатурой и мышкой :)  

---

В `sdl2-app` урока реализована утилита которая выводит код и название клавиши на экран, и пишет в консоль события движения и нажатия на мышку.  

`Это будет работать только на PC!`


## Что получилось

После завершения урока вы научитесь:

* Работать с системой событий SDL.
* Обрабатывать клавиатуру.
* Обрабатывать мышку

