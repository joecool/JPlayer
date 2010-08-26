LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libjplayer_jni
LOCAL_SRC_FILES := source/cc_co_androidzoo_jplayer_Player.cpp 

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/include \
    frameworks/base/core/jni \
    $(PV_INCLUDES) \
    $(JNI_H_INCLUDE) 

LOCAL_SHARED_LIBRARIES := \
	libandroid_runtime libnativehelper libui libcutils libutils libbinder libsonivox libdl libCore

LOCAL_LDLIBS += -ldl -lpthread

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := libCore
LOCAL_SRC_FILES := source/player.cpp

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/include \
    external/tremor/Tremor \
    frameworks/base/core/jni \
    $(PV_INCLUDES) \
    $(JNI_H_INCLUDE)

#LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := \
        libandroid_runtime libnativehelper libui libcutils libutils libbinder libsonivox libdl 

LOCAL_LDLIBS += -ldl -lpthread

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
