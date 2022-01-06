# Karta2D
Simple 2D physics engine I decided to build from scratch! Let's see where this journey takes me.

## Dependencies
### SDL2
Download and install latest [SDL2 development libraries](https://www.libsdl.org/download-2.0.php).

## Building project
Make sure you have latest [CMake release](https://cmake.org/download/).

1. Clone repository in a directory of your choice
2. Extract the SDL2 development libraries in any directory, e.g.:
```
C:\Libraries\SDL2-2.0.18
```
4. Open a terminal and configure CMake project in a folder `build`, set the CMake variable `SDL2_PATH` to your SDL2 development libraries path
```
C:\Dev\Projects\karta2d>cmake -S Karta2D -B build -D SDL2_PATH=C:\Libraries\SDL2-2.0.18
```
5. Build the CMake project
```
C:\Dev\Projects\karta2d>cmake --build build
```
6. You can either copy-paste the `SDL2.dll` next to the Visual Studio project executable or set the path as an environment variable
```
C:\Libraries\SDL2-2.0.18\lib\x64
C:\Libraries\SDL2-2.0.18\lib\x86
```
