@echo off
cd ..
mkdir bin
cd bin
g++ -c -std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror -Wno-unused-variable -fext-numeric-literals -IC:/sfml/include -I../src ../src/*
g++ *.o -o geneak.exe -LC:/sfml/lib -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -lsfml-audio-d
