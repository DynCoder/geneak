@echo off
cd ..
mkdir bin
cd bin
xcopy ..\src\resources\* . /y
g++ -c -std=c++14 -DNDEBUG -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror -Wno-unused-variable -fext-numeric-literals -IC:/sfml/include -I../src ../src/*.h
g++ -c -std=c++14 -DNDEBUG -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror -Wno-unused-variable -fext-numeric-literals -IC:/sfml/include -I../src ../src/*.cpp
g++ *.o -o lunaire.exe -LC:/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows
cd ..\scripts
