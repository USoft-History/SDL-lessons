# Урок 0.3 — Организация сборки SDL2 проекта для Linux и PlayStation 2

В этом уроке мы разделим систему сборки на несколько Makefile-файлов и создадим удобную структуру для поддержки нескольких платформ.

На данный момент проект поддерживает:

* Linux
* PlayStation 2 Homebrew

В будущем по аналогии можно добавить:

* Windows
* PSP
* PS Vita
* Android

## Структура проекта

```
sdl2-app-skeleton/
├── bin/
├── src/
│   └── main.c
├── Makefile
├── Makefile.linux
└── Makefile.ps2
```

## Главный Makefile

Файл Makefile является точкой входа для сборки проекта.

Он не выполняет компиляцию самостоятельно, а перенаправляет управление в платформозависимые Makefile.

```makefile

.PHONY: all linux ps2 clean

all: linux ps2

linux:
	$(MAKE) -f Makefile.linux

ps2:
	$(MAKE) -f Makefile.ps2

clean:
	$(MAKE) -f Makefile.linux clean
	$(MAKE) -f Makefile.ps2 clean

```

### Сборка всех платформ

```bash
make
```

Будут собраны:

```
bin/game_linux
bin/game_ps2.elf
```

### Сборка только Linux версии
```bash
make linux
```

#### Сборка только PS2 версии
```bash
make ps2
```

#### Очистка проекта
```bash
make clean
```

Команда вызывает очистку для всех платформ.


## Makefile.linux

Данный файл отвечает за сборку Linux-версии приложения.

```makefile
CC = gcc

CFLAGS = -O2 -Wall -DPLATFORM_LINUX `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

SRC = src/main.c
OUT = bin/game_linux
```

---

### Используемые параметры

```Makefile
CC = gcc
```

Компилятор языка C.

---

### Оптимизация

```Makefile
-O2
```

Включает оптимизацию кода.

---

### Предупреждения компилятора

```makefile
-Wall
```

Показывает основные предупреждения.

---

### Макрос платформы

```
-DPLATFORM_LINUX
```
Позволяет писать платформозависимый код:

```cpp
#ifdef PLATFORM_LINUX
    SDL_Log("Linux build");
#endif
```

---

### SDL2 Config

`sdl2-config --cflags`

Автоматически добавляет пути к заголовочным файлам SDL2.

`sdl2-config --libs`

Автоматически подключает необходимые SDL2-библиотеки.

---

### Результат сборки

После выполнения:

```bash
make linux
```

будет создан файл:

`bin/game_linux`

## Makefile.ps2

Данный файл отвечает за сборку PlayStation 2 Homebrew приложения.

### PS2DEV и PS2SDK

```makefile
PS2DEV ?= /usr/local/ps2dev 
PS2SDK ?= $(PS2DEV)/ps2sdk
```

Определяют пути к установленному тулчейну.

---

### Компилятор

```makefile
export PATH := $(PATH):$(PS2DEV)/ee/bin
```

Добавляет компилятор PS2 в PATH.

Используемый компилятор:

`mips64r5900el-ps2-elf-gcc`

---

### Макрос платформы

`EE_CFLAGS += -DPLATFORM_PS2`

Использование:

```cpp
#ifdef PLATFORM_PS2
    SDL_Log("PS2 build");
#endif
```

---

### Поиск исходников

```makefile
EE_SRCS := $(wildcard src/*.c)
```

Автоматически добавляет все .c файлы из каталога src.

---


### Заголовочные файлы
```makefile
EE_INCS += -I$(PS2SDK)/ports/include
EE_INCS += -I$(PS2DEV)/gsKit/include
```

Подключаются:

* SDL2
* gsKit

---

### Библиотеки

`EE_LIBS = -lSDL2_ttf -lfreetype -lpng -lz -lSDL2`

Дополнительно подключаются библиотеки:

* ps2_drivers
* gsKit
* dmaKit
* audsrv
* padx
* mtap
* mouse
* fileXio
* patches
* debug

---

### Система сборки PS2SDK

```makefile
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
```

Эти файлы содержат стандартную систему сборки PS2SDK.

---

### Результат сборки

После выполнения:

`make ps2`

будет создан файл:

`bin/game_ps2.elf`

---

### Добавление новых платформ

Текущая архитектура позволяет легко расширять проект.

Например, для Windows можно добавить:

`Makefile.windows`

и расширить главный Makefile:

`all: linux windows ps2`

После чего станет доступна команда:

`make windows`

---

## Итог

После выполнения урока вы получите универсальную систему сборки SDL2-проекта с разделением по платформам.

Такой подход позволяет поддерживать несколько платформ одновременно, не усложняя основной Makefile и сохраняя код проекта максимально чистым и удобным для сопровождения.