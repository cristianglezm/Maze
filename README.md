# Maze #

[![Build Status](https://travis-ci.org/cristianglezm/Maze.svg)](https://travis-ci.org/cristianglezm/Maze)

Maze is a demo to see different graph traversal algorithms in action and generate mazes with differents algorithms.
Currently there is only one Algorithm to Generate the maze:

* Growing Tree Algorithm: Can be configured with some parameters.

	* Randomness: Choose the way that selects the next node to process its neighbors (between linear and random). 
	* Binomial: Choose the way that selects the neighbor to disconnect from the rest (between uniform and binomial).

[Download](https://bitbucket.org/cristian_glez_m/maze/downloads)

[Video](http://youtu.be/tLzJpE0kRS4)

Isometric

![Isometric.png](https://bitbucket.org/repo/qoqboX/images/141456758-Isometric.png)

Topdown

![Topdown.png](https://bitbucket.org/repo/qoqboX/images/3667644295-Topdown.png)

Keys

* A - A* search
* D - Deep First Search
* B - Breadth First Search
* G - Greedy Best First Search
* R - remove edges from a node
* M - Generate maze with the origin and / or dest

Building
===

Requires CMake and one of the following compilers:

* GCC 4.8 or newer;
* Clang 3.7 or newer [Not Tested];
* Visual Studio 2010 or newer [Not Tested];

Maze requires the following dependencies:

* [SFML](http://sfml-dev.org) 2.3.2 or newer

Use CMake to build the project.

```
mkdir build && cd build
cmake -G"MinGW Makefiles" -DSLOW_SEARCH=TRUE -DPARALLEL_ISOLATE=FALSE -DMANHATTAN=TRUE ..
make
make install
```
Android
===
```
cd android
android update project --name Maze --target android-19 --path .
ndk-build
ant debug
```
