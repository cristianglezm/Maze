cmake_minimum_required(VERSION 3.1)

################################################################################
### User options.
################################################################################

option(BUILD_STATIC "TRUE to statically link to the standard libraries and dependencies, FALSE to use them as DLLs" OFF)
if(BUILD_STATIC)
	SET(SFML_STATIC_LIBRARIES TRUE)
	SET(BUILD_SHARED_LIBS FALSE)
	SET(SFML_USE_STATIC_STD_LIBS TRUE)
endif(BUILD_STATIC)

option(SLOW_SEARCH "Slows DFS,BFS, GBFS" FALSE)
if(SLOW_SEARCH)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSLOW_SEARCH")
endif()

option(PARALLEL_ISOLATE "uses parallel isolate" FALSE)
if(PARALLEL_ISOLATE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPARALLEL_ISOLATE")
endif(PARALLEL_ISOLATE)

option(EUCLIDEAN "Uses Euclidean Distance to calculate best route" TRUE)
if(EUCLIDEAN)
	SET(MANHATTAN FALSE)
	SET(CHEBYSHEV FALSE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEUCLIDEAN")
endif(EUCLIDEAN)

option(MANHATTAN "Uses Manhattan Distance to calculate best route" FALSE)
if(MANHATTAN)
	SET(EUCLIDEAN FALSE)
	SET(CHEBYSHEV FALSE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMANHATTAN")
endif(MANHATTAN)

option(CHEBYSHEV "Uses Chebysev Distance to calculate best route" FALSE)
if(CHEBYSHEV)
	SET(MANHATTAN FALSE)
	SET(EUCLIDEAN FALSE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCHEBYSHEV")
endif(CHEBYSHEV)


################################################################################
### Project files.
################################################################################

project(Maze)

SET(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

FILE(GLOB_RECURSE Maze_SOURCES "src/*.cpp")
FILE(GLOB_RECURSE Maze_AndroidMain "src/android*.cpp")
FILE(GLOB_RECURSE Maze_INCLUDES "src/*.hpp")
LIST(REMOVE_ITEM Maze_SOURCES ${Maze_AndroidMain})

if(WIN32)
	FILE(GLOB_RECURSE SFML_DEBUG_DLLS "${SFML_ROOT}/bin/sfml-*-d-2.dll")
	FILE(GLOB_RECURSE SFML_RELEASE_DLLS "${SFML_ROOT}/bin/*-2.dll")
	if(NOT SFML_DEBUG_DLLS STREQUAL "")
		LIST(REMOVE_ITEM SFML_RELEASE_DLLS ${SFML_DEBUG_DLLS})
	endif()
endif(WIN32)

add_executable(Maze ${Maze_SOURCES} ${Maze_INCLUDES})

################################################################################
### Testing
################################################################################

#enable_testing()

################################################################################
### Dependencies.
################################################################################

#sfml
find_package(SFML 2.1 COMPONENTS graphics window system network audio REQUIRED)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(Maze ${SFML_LIBRARIES})
endif(SFML_FOUND)

################################################################################
### Enable C++14 and warnings
################################################################################

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
if(WIN32)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -lpthread -static-libgcc -static-libstdc++")
endif(WIN32)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fexpensive-optimizations")
endif()

#################################################################################
### Installing Game
#################################################################################
install(TARGETS Maze RUNTIME DESTINATION .
			ARCHIVE DESTINATION .)
install(FILES LICENSE.txt README.md DESTINATION .)
if(NOT BUILD_STATIC AND WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		install(FILES ${SFML_DEBUG_DLLS} DESTINATION .)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		install(FILES ${SFML_RELEASE_DLLS} DESTINATION .)
	endif()
endif(NOT BUILD_STATIC AND WIN32)

install(DIRECTORY android/assets/data DESTINATION .)
