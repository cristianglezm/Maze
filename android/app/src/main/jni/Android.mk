LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Maze

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../src

LOCAL_SRC_FILES := ../../../../../src/androidMain.cpp ../../../../../src/Graph.cpp
LOCAL_SRC_FILES += ../../../../../src/Tile.cpp ../../../../../src/Menu.cpp ../../../../../src/Button.cpp

ifeq ($(BUILD_TYPE), debug)
    LOCAL_LDLIBS := -llog
    LOCAL_SHARED_LIBRARIES := sfml-system-d
    LOCAL_SHARED_LIBRARIES += sfml-window-d
    LOCAL_SHARED_LIBRARIES += sfml-graphics-d
    LOCAL_SHARED_LIBRARIES += sfml-audio-d
    LOCAL_SHARED_LIBRARIES += sfml-network-d
    LOCAL_SHARED_LIBRARIES += sfml-activity-d
    LOCAL_SHARED_LIBRARIES += openal
    LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main-d
    LOCAL_CFLAGS += -DDEBUG
else
    LOCAL_LDLIBS := -llog
    LOCAL_SHARED_LIBRARIES := sfml-system
    LOCAL_SHARED_LIBRARIES += sfml-window
    LOCAL_SHARED_LIBRARIES += sfml-graphics
    LOCAL_SHARED_LIBRARIES += sfml-audio
    LOCAL_SHARED_LIBRARIES += sfml-network
    LOCAL_SHARED_LIBRARIES += sfml-activity
    LOCAL_SHARED_LIBRARIES += openal
    LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main
    LOCAL_CFLAGS += -DNDEBUG -O3
endif
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_CFLAGS += -Wall -std=c++17 -DANDROID

include $(BUILD_SHARED_LIBRARY)

$(call import-module, third_party/sfml)
