NDK_TOOLCHAIN_VERSION := 4.9
APP_PLATFORM := android-14
APP_STL := c++_shared
APP_ABI := armeabi-v7a
APP_CPPFLAGS += -std=c++17
ifeq ($(BUILD_TYPE), debug)
    APP_OPTIM := debug
    APP_MODULES := sfml-activity-d Maze
else
    APP_OPTIM := release
    APP_MODULES := sfml-activity Maze
endif
APP_NAME := Maze
