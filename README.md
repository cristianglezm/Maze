# Maze #

Maze is a demo to see different graph traversal algorithms in action and generate mazes with differents algorithms.
Currently there is only one Algorithm to Generate the maze:

* Growing Tree Algorithm: Can be configured with some parameters.

	* Randomness: Choose the way that selects the next node to process its neighbors (between linear and random). 
	* Binomial: Choose the way that selects the neighbor to disconnect from the rest (between uniform and binomial).

[Video](https://www.youtube.com/watch?v=AEI9HyGn89U)

Keys

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

* select the root directory of SFML

* mark the SLOW_SEARCH option if you want to see the progress of the Algorithms.

* press configure and generate Project.
