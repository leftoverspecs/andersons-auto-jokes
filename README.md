[![LICENSE](https://www.gnu.org/graphics/gplv3-88x31.png)](https://www.gnu.org/licenses/gpl-3.0.txt)

GGJ 2024
========

TODO: Describe game

Requirements
------------

 * [Python 3](https://www.python.org/)
 * [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5)
 * [SDL2-image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.2)
 * [SDL2-mixer](https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.6.3)
 * [GLEW](https://sourceforge.net/projects/glew/files/glew/2.2.0/)
 * [glm](https://github.com/g-truc/glm/releases/tag/0.9.9.8)
 * [CMake](https://cmake.org/)

Build on GNU/Linux
------------------

Install the required dependencies with your system's package manager. Then call

    $ mkdir build
    $ cd build
    $ cmake ..
    $ cmake --build .

Build on Windows
----------------

Download the prebuilt binaries and put them into a common folder. Then call

    > mkdir build
    > cd build
    > cmake -DCMAKE_PREFIX_PATH=<common-folder> -DGLEW_ROOT=<glew-folder> ..
    > cmake --build .
