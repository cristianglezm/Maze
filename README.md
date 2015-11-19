# Maze #

Maze is a demo to see different graph traversal algorithms in action and generate mazes with differents algorithms.
Currently there is only one Algorithm to Generate the maze:

* Growing Tree Algorithm: Can be configured with some parameters.

	* Randomness: Choose the way that selects the next node to process its neighbors (between linear and random). 
	* Binomial: Choose the way that selects the neighbor to disconnect from the rest (between uniform and binomial).

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
* Clang 3.3 or newer (Xcode 5.0 or newer on OS X);
* Visual Studio 2010 or newer;

Maze requires the following dependencies:

* [SFML](http://sfml-dev.org) 2.1 or newer

Use CMake to build the project.

```
mkdir build && cd build
cmake -G"MinGW Makefiles" -DSLOW_SEARCH=TRUE -DPARALLEL_ISOLATE=FALSE -DMANHATTAN=TRUE ..
make
make install
```