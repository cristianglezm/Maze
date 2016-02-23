LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Maze

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../src

LOCAL_SRC_FILES := ../../src/androidMain.cpp ../../src/Graph.cpp
LOCAL_SRC_FILES += ../../src/Tile.cpp ../../src/Menu.cpp ../../src/Button.cpp

LOCAL_LDLIBS := -llog

LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_CFLAGS += -Wall -std=c++11 -DANDROID

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)
