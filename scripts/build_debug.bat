@echo off
cd ../bin
g++ -c -std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror -Wno-unused-variable -fext-numeric-literals -I../src ../src/*
g++ *.o -o geneak.exe
Rem -IC:/sfml/include
Rem -LC:/sfml/lib -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -lsfml-audio-d
