# Урок 0.2 — Сборка первого SDL2-приложения для ПК и PlayStation 2

## Цель урока

Научиться собирать одно и то же SDL2-приложение для двух платформ:

* Linux (PC)
* PlayStation 2

После выполнения урока у вас будет:

* первый SDL2-проект;
* понимание процесса кроссплатформенной сборки;
* исполняемый файл для ПК;
* ELF-файл для PlayStation 2.

---

## Создание структуры проекта

Создаем каталог проекта:

```bash
mkdir sdl2-tutorial
cd sdl2-tutorial
```

Создаем каталоги для исходного кода и готовых сборок:

```bash
mkdir src
mkdir bin
```

Структура проекта должна выглядеть следующим образом:

```text
sdl2-tutorial
├── bin
└── src
    └── main.c
```

---

## Создание первого исходного файла

Создаем файл:

```bash
touch src/main.c
```

Открываем его в редакторе и добавляем следующий код:

```c
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Quit();
    return 0;
}
```

### Что делает программа

Функция:

```c
SDL_Init(SDL_INIT_VIDEO);
```

инициализирует видеоподсистему SDL2.

Функция:

```c
SDL_Quit();
```

завершает работу SDL2 и освобождает ресурсы.

На данном этапе программа ничего не отображает на экране. Наша задача — проверить, что SDL2 корректно подключена и проект успешно собирается.

---

# Сборка для ПК

Выполните команду:

```bash
gcc src/main.c -lSDL2 -o bin/pc_game
```

После успешной сборки появится файл:

```text
bin/pc_game
```

Запуск программы:

```bash
./bin/pc_game
```

Программа завершится сразу после запуска.

---

# Сборка для PlayStation 2

Для сборки используем кросс-компилятор PS2DEV.

Выполните команду:

```bash
/usr/local/ps2dev/ee/bin/mips64r5900el-ps2-elf-gcc \
-I/usr/local/ps2dev/ps2sdk/ports/include \
-I/usr/local/ps2dev/gsKit/include \
src/main.c \
-L/usr/local/ps2dev/ps2sdk/ports/lib \
-L/usr/local/ps2dev/ps2sdk/ee/lib \
-L/usr/local/ps2dev/gsKit/lib \
-lSDL2 \
-lps2_drivers \
-lgskit \
-ldmakit \
-laudsrv \
-lpadx \
-lmtap \
-lmouse \
-lfileXio \
-lpatches \
-ldebug \
-o bin/ps2_game.elf
```

После успешной сборки появится файл:

```text
bin/ps2_game.elf
```

---

## Запуск в PCSX2

Запустить ELF-файл можно через PCSX2:

**System → Run ELF**

или указать путь к файлу:

```text
bin/ps2_game.elf
```

Также приложение можно запускать на реальной PlayStation 2 через Free McBoot и другие homebrew-загрузчики.

---

## Итоги

В этом уроке мы:

* создали первый SDL2-проект;
* написали минимальное SDL2-приложение;
* собрали программу для Linux;
* собрали программу для PlayStation 2;
* получили два исполняемых файла из одного исходного кода.

Это демонстрирует главное преимущество SDL2 — возможность использовать один код для разных платформ.

---
