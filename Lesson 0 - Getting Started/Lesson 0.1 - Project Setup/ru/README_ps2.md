# Инструкция по развертыванию SDL2 окружения для PS2 Homebrew

В данном руководстве описан процесс настройки официального тулчейна ps2dev, ручной сборки недостающих графических библиотек и компиляции кроссплатформенного приложения на базе SDL2 под PlayStation 2.

## Шаг 0: Установка gcc/clang, make, cmake, patch, git, texinfo, flex, bison, gettext, wget, gsl, gmp, zlib, mpfr и mpc, если у вас их нет.

```bash
sudo apt -y install gcc g++ make cmake patch git texinfo flex bison gettext libgsl-dev libgmp3-dev libmpfr-dev libmpc-dev zlib1g-dev autopoint
```

## Шаг 1: Установка официального тулчейна (ps2dev)
Первым делом разворачивается базовый компилятор и SDK. Для автоматизации используется официальный репозиторий со скриптами сборки.

```bash

git clone https://github.com/ps2dev/ps2dev.git
cd ps2dev

```

Перед запуском компиляции обязательно экспортируются переменные окружения, чтобы скрипты знали, куда устанавливать тулчейн (требуются права на запись в выбранную директорию, обычно /usr/local/ps2dev):
```bash
export PS2DEV=/usr/local/ps2dev
sudo mkdir -p $PS2DEV
sudo chown -R $USER: $PS2DEV
```

```bash
export PS2SDK=$PS2DEV/ps2sdk
export PATH=$PATH:$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2SDK/bin:$PS2DEV/dvp/bin

```

Запуск полной сборки тулчейна и базовых портов:

```bash
./build-all.sh
```

Скрипт поочередно собирает 001-core-debian.sh, 002-ps2sdk.sh и 003-ps2sdk-ports.sh. На этом этапе разворачивается компилятор mips64r5900el-ps2-elf-gcc и базовая копия SDL2.

### Дальше нужно проверить доступен ли компилятор под ps2

Компиляторы должны лежать здесь:
```bash
/usr/local/ps2dev/ee/bin
```

```bash
$ ls /usr/local/ps2dev/ee/bin
mips64r5900el-ps2-elf-addr2line   mips64r5900el-ps2-elf-c++         mips64r5900el-ps2-elf-elfedit  mips64r5900el-ps2-elf-gcc-15.2.0  mips64r5900el-ps2-elf-gcc-ranlib  mips64r5900el-ps2-elf-gcov-tool      mips64r5900el-ps2-elf-gprof       mips64r5900el-ps2-elf-ld.bfd      mips64r5900el-ps2-elf-objcopy  mips64r5900el-ps2-elf-readelf     mips64r5900el-ps2-elf-strip       mips64r5900el-ps2-elf-ar         mips64r5900el-ps2-elf-c++filt     mips64r5900el-ps2-elf-g++         mips64r5900el-ps2-elf-gcc-ar      mips64r5900el-ps2-elf-gcov        mips64r5900el-ps2-elf-gdb         mips64r5900el-ps2-elf-gstack  mips64r5900el-ps2-elf-lto-dump    mips64r5900el-ps2-elf-objdump     mips64r5900el-ps2-elf-size
mips64r5900el-ps2-elf-as          mips64r5900el-ps2-elf-cpp         mips64r5900el-ps2-elf-gcc      mips64r5900el-ps2-elf-gcc-nm      mips64r5900el-ps2-elf-gcov-dump   mips64r5900el-ps2-elf-gdb-add-index  mips64r5900el-ps2-elf-ld          mips64r5900el-ps2-elf-nm          mips64r5900el-ps2-elf-ranlib 
mips64r5900el-ps2-elf-strings

```

| Утилита                               | Назначение                               |
| ------------------------------------- | ---------------------------------------- |
| `mips64r5900el-ps2-elf-gcc`           | Компилятор C                             |
| `mips64r5900el-ps2-elf-g++`           | Компилятор C++                           |
| `mips64r5900el-ps2-elf-cpp`           | Препроцессор C                           |
| `mips64r5900el-ps2-elf-c++`           | Ссылка на компилятор C++                 |
| `mips64r5900el-ps2-elf-gcc-15.2.0`    | Конкретная версия GCC                    |
| `mips64r5900el-ps2-elf-as`            | Ассемблер                                |
| `mips64r5900el-ps2-elf-ld`            | Линкер                                   |
| `mips64r5900el-ps2-elf-ld.bfd`        | Альтернативный запуск линкера BFD        |
| `mips64r5900el-ps2-elf-ar`            | Создание статических библиотек `.a`      |
| `mips64r5900el-ps2-elf-ranlib`        | Индексация библиотек `.a`                |
| `mips64r5900el-ps2-elf-gcc-ranlib`    | Версия ranlib от GCC                     |
| `mips64r5900el-ps2-elf-gcc-ar`        | Версия ar от GCC                         |
| `mips64r5900el-ps2-elf-nm`            | Просмотр символов объекта или библиотеки |
| `mips64r5900el-ps2-elf-gcc-nm`        | Версия nm от GCC                         |
| `mips64r5900el-ps2-elf-objdump`       | Дизассемблирование ELF и OBJ файлов      |
| `mips64r5900el-ps2-elf-objcopy`       | Конвертация объектных файлов             |
| `mips64r5900el-ps2-elf-size`          | Размер секций ELF                        |
| `mips64r5900el-ps2-elf-strings`       | Извлечение строк из бинарника            |
| `mips64r5900el-ps2-elf-strip`         | Удаление отладочной информации           |
| `mips64r5900el-ps2-elf-readelf`       | Подробный просмотр структуры ELF         |
| `mips64r5900el-ps2-elf-elfedit`       | Редактирование ELF-заголовков            |
| `mips64r5900el-ps2-elf-addr2line`     | Преобразование адреса в строку исходника |
| `mips64r5900el-ps2-elf-c++filt`       | Расшифровка C++-символов                 |
| `mips64r5900el-ps2-elf-gdb`           | Отладчик                                 |
| `mips64r5900el-ps2-elf-gdb-add-index` | Добавление индекса для GDB               |
| `mips64r5900el-ps2-elf-gprof`         | Профилирование производительности        |
| `mips64r5900el-ps2-elf-gcov`          | Покрытие кода тестами                    |
| `mips64r5900el-ps2-elf-gcov-dump`     | Просмотр данных gcov                     |
| `mips64r5900el-ps2-elf-gcov-tool`     | Обработка данных gcov                    |
| `mips64r5900el-ps2-elf-gstack`        | Просмотр стека процесса                  |
| `mips64r5900el-ps2-elf-lto-dump`      | Информация о Link Time Optimization      |

Если все есть так как у меня значит все установилось правильно

Запускаем компилятор:

```bash

/usr/local/ps2dev/ee/bin/mips64r5900el-ps2-elf-gcc

mips64r5900el-ps2-elf-gcc: fatal error: no input files
compilation terminated.

```

значит у нас компилятор нормально запустился и ждет от нас исходные файлы

---



## Шаг 2: Ручная сборка графического движка gsKit
В современных версиях SDL2 для PS2 в качестве бэкенда рендеринга используется библиотека gsKit (низкоуровневая обертка над графическим синтезатором Graphics Synthesizer). Если автоматический скрипт её пропустил, её необходимо собрать вручную.

```bash

cd ~/projects/ps2  # или твоя рабочая папка
git clone https://github.com/ps2dev/gsKit.git
cd gsKit

# Настройка окружения и зависимостей внутри репозитория
./setup.sh

# Сборка и установка в систему через CMake
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$PS2DEV/ps2sdk/ps2sdk.cmake
cmake --build build --target install

```

Результат: статические библиотеки libgskit.a и libdmakit.a устанавливаются по пути /usr/local/ps2dev/gsKit/lib/.


💡 Важные инсайты, полученные в процессе:
libps2_drivers.a — критически важная библиотека из ps2sdk-ports. Новые версии SDL2 прячут в неё внутренние вызовы init_audio_driver и init_joystick_driver. Без неё линкер упадет с ошибкой undefined reference.

Регистр имен — библиотека gsKit при сборке через CMake устанавливается в нижнем регистре (libgskit.a), поэтому в Makefile пишется строго -lgskit, а не -lgsKit.

**Переменные окружения вроде $PS2DEV должны быть экспортированы в терминале перед любой командой make(или добавлен экспорт в сам makefile), иначе Makefile не сможет подтянуть глобальные файлы конфигурации Makefile.eeglobal.**
