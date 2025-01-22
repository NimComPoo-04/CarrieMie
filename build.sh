#!/bin/sh

gcc -Wall -Wextra -ggdb -std=c11 -O0 -DDEBUG -I libs/raylib-5.5_linux_amd64/include -I libs/raygui-4.0/src/ *.c -L libs/raylib-5.5_linux_amd64/lib -l:libraylib.a -lm -o game.exe
./game.exe
