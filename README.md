<a id="totop"></a>
# EndlessRunner

Программа предназначена для операционной системы Windows 10 (x64);

Язык программирования C++, компилятор gcc;

# Сборка проекта

## 1. Установка необходимого програмного обеспечения

1. Ддя сборки проекта необходимо установить среду разработки [MSYS2](https://www.msys2.org/#installation)  
Установщик по окончании работы предлагает открыть MSYS2, если вы соглашаетесь убедитесь что вы используете окружение **UCRT64**;

2. После установки необходимо провести обновление командой:
```sh
pacman -Suy;
```

3. Следующим шагом установите компилятор gcc для окружения UCRT64 командой:
```sh
pacman -S mingw-w64-ucrt-x86_64-gcc
```

4. Установите утилиту make командой:
```sh
pacman -S mingw-w64-ucrt-x86_64-make`
```

## 2. Сборка
Для сборки проекта в окружении UCRT64 выполние следующие команды:
```sh
mingw32-make init
mingw32-make
```

*NOTE: Команда mingw32-make init создаёт коталог obj для файлов.*

### Сборка в VS Code
Для сборки проекта в VS Code необходимо:
1. Добавить в переменную среды PATH путь к UCRT64  
При установке MSYS2 по умолчанию это C:\msys64\ucrt64\bin
2. В терминале введите команды:
```sh
mingw32-make init
mingw32-make
```
<p align="right"> <a href="#totop">Back to top</a> </p>

# Разработчики

<a href="https://github.com/eaivanof"><img src="https://avatars.githubusercontent.com/u/192405069?v=4" width="100"></a>
<a href="https://github.com/Wangzy0606"><img src="https://avatars.githubusercontent.com/u/191434705?v=4" width="100"></a>

[![ТПУ ИШИТР](https://itr.tpu.ru/upload/constructor/38d/8e9blao3ih5jpfyttt3id51019dvvh1c.svg)](https://itr.tpu.ru/)

<p align="right"> <a href="#totop">Back to top</a> </p>
