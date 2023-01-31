# Maze #

[![Build Status](https://github.com/cristianglezm/Maze/actions/workflows/ci.yml/badge.svg)](https://github.com/cristianglezm/Maze/actions/workflows/ci.yml)

Maze is a demo to see different graph traversal algorithms in action and generate mazes with different algorithms.
Currently there is only one Algorithm to Generate the maze:

* Growing Tree Algorithm: Can be configured with some parameters.

	* Randomness: Choose the way that selects the next node to process its neighbours (between linear and random). 
	* Binomial: Choose the way that selects the neighbour to disconnect from the rest (between uniform and binomial).

[Download](https://bitbucket.org/cristian_glez_m/maze/downloads)

[Video](http://youtu.be/tLzJpE0kRS4)

Isometric View

![Isometric.png](https://bitbucket.org/repo/qoqboX/images/141456758-Isometric.png)

Top-down View

![Topdown.png](https://bitbucket.org/repo/qoqboX/images/3667644295-Topdown.png)

Keys

* A - A* search
* D - Depth-First Search
* B - Breadth-First Search
* G - Greedy Best First Search
* R - remove edges from a node
* M - Generate maze with the origin and / or destination

Building
===

Requires CMake and one of the following compilers:

* GCC 8.1 or newer;
* Clang 3.7 or newer [Not Tested];
* Visual Studio 2010 or newer [Not Tested];
* Android NDK and SDK, Gradle

Maze requires the following dependencies:

* [SFML](http://sfml-dev.org) 2.5.1 or newer

Use CMake to build the project.

```
mkdir build && cd build
cmake -G"MinGW Makefiles" -DMAZE_SLOW_SEARCH=TRUE -DMAZE_PARALLEL_ISOLATE=FALSE -DMAZE_MANHATTAN=TRUE ..
make
make install
```
Android
===
```
git clone https://github.com/SFML/SFML SFML
cd SFML
mkdir build && cd build && mkdir armeabi-v7a && cd armeabi-v7a
cmake -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_NDK=<android_ndk> \
        -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_ANDROID_STL_TYPE=c++_shared \ 
        -DCMAKE_BUILD_TYPE=Release -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang \ 
        -DCMAKE_INSTALL_PREFIX=<android_ndk>/sources/third_party/sfml ../..
make install
cd ../../..
git clone https://github.com/cristianglezm/Maze Maze
cd Maze && cd android
gradle build
[gradle installDebug]

```

You should replace <android_ndk> with the actual folder.
