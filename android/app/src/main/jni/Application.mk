NDK_TOOLCHAIN_VERSION := 4.9
APP_PLATFORM := $(ANDROID_PLATFORM)
APP_STL := c++_shared
APP_ABI := $(TARGET_ARCH_ABI)
APP_CPPFLAGS += -std=c++17
ifeq ($(BUILD_TYPE), debug)
    APP_OPTIM := debug
    APP_DEBUG := true
    APP_MODULES := sfml-activity-d Maze
else
    APP_OPTIM := release
    APP_DEBUG := false
    APP_MODULES := sfml-activity Maze
endif
APP_NAME := Maze
