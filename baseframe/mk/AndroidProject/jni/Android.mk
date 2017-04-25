LOCAL_PATH := $(call my-dir)
MY_ROOT_PATH := $(LOCAL_PATH)/../../..

MY_CFLAGS := -Os \
-fvisibility=hidden \
-fira-region=one \
-fpic \
-fgcse-las \
-ftracer \
-frename-registers \
-fbranch-target-load-optimize2 \
-Wa,--strip-local-absolute \

MY_CPPFLAGS := -fvisibility-inlines-hidden

MY_CFLAGS 	+= -DANDROID_SYSTEM -DGL_GLEXT_PROTOTYPES
MY_CFLAGS	+= -DDEBUG
MY_CFLAGS 	+= -fvisibility=hidden 
MY_CFLAGS 	+= -ffunction-sections -fdata-sections

MY_CFLAGS 	+= -DUSE_LIB_JPEG 
# MY_CFLAGS 	+= -DUSE_LIB_HTTP 

#jpeg-turbo static lib
include $(CLEAR_VARS)

LOCAL_CFLAGS += -D__ARM_HAVE_NEON

EXTRA_DIST := $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/nasm_lt.sh \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jcclrmmx.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jcclrss2.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jdclrmmx.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jdclrss2.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jdmrgmmx.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jdmrgss2.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jcclrss2-64.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jdclrss2-64.asm \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jdmrgss2-64.asm  \
			 $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/CMakeLists.txt
 
libsimd_SOURCES_DIST := $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jsimd_arm_neon.S \
                       $(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jsimd_arm.c 

LOCAL_SRC_FILES := $(libsimd_SOURCES_DIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(MY_ROOT_PATH)/src/commons/jpeg_src/simd \
                    $(MY_ROOT_PATH)/src/commons/jpeg_src
 
AM_CFLAGS := -march=armv7-a -mfpu=neon
AM_CCASFLAGS := -march=armv7-a -mfpu=neon
 
libjpeg_SOURCES_DIST := $(MY_ROOT_PATH)/src/commons/jpeg_src/jcapimin.c \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/jcapistd.c  \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/jccoefct.c  \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/jccolor.c  \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/jcdctmgr.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jchuff.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcinit.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcmainct.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcmarker.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcmaster.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcomapi.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcparam.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcphuff.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcprepct.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcsample.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jctrans.c   \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdapimin.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdapistd.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdatadst.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdatasrc.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdcoefct.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdcolor.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jddctmgr.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdhuff.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdinput.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdmainct.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdmarker.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdmaster.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdmerge.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdphuff.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdpostct.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdsample.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdtrans.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jerror.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jfdctflt.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jfdctfst.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jfdctint.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jidctflt.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jidctfst.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jidctint.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jidctred.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jquant1.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jquant2.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jutils.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jmemmgr.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jmemnobs.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jaricom.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jcarith.c  \
        $(MY_ROOT_PATH)/src/commons/jpeg_src/jdarith.c  \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/turbojpeg.c \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/transupp.c \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/jdatadst-tj.c \
		$(MY_ROOT_PATH)/src/commons/jpeg_src/jdatasrc-tj.c

LOCAL_SRC_FILES += $(libjpeg_SOURCES_DIST:$(LOCAL_PATH)/%=%)
 
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_C_INCLUDES := $(MY_ROOT_PATH)/src/commons/jpeg_src
 
LOCAL_CFLAGS := -DAVOID_TABLES  -O3 -fstrict-aliasing -fprefetch-loop-arrays  -DANDROID \
        -DANDROID_TILE_BASED_DECODE -DENABLE_ANDROID_NULL_CONVERT

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_STATIC_LIBRARY)
LOCAL_MODULE := libjpeg
include $(BUILD_STATIC_LIBRARY)


# include $(CLEAR_VARS)
# libjpegTurbo_SOURCES_DIST = 	$(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jsimd_arm_neon.S \
# 								$(MY_ROOT_PATH)/src/commons/jpeg_src/simd/jsimd_arm.c \
# 								$(MY_ROOT_PATH)/src/commons/jpeg_src/*.c 

# LOCAL_SRC_FILES := $(libsimd_SOURCES_DIST:$(LOCAL_PATH)/%=%)
# LOCAL_C_INCLUDES := $(MY_ROOT_PATH)/src/commons/jpeg_src/simd
# AM_CFLAGS := -march=armv7-a -mfpu=neon
# AM_CCASFLAGS := -march=armv7-a -mfpu=neon
# LOCAL_MODULE := libsimd
# include $(BUILD_STATIC_LIBRARY)

#commons static lib
include $(CLEAR_VARS)
MY_CPP_LIST		:= $(wildcard $(MY_ROOT_PATH)/src/commons/*.cpp)
# MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/commons/ghttplib/*.c)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES:= $(MY_ROOT_PATH)/inc/MessageCenter \
				   $(MY_ROOT_PATH)/inc/commons
LOCAL_CPPFLAGS 	+= $(MY_CPPFLAGS)
LOCAL_CFLAGS 	+= $(MY_CFLAGS)

LOCAL_MODULE := libcommons
include $(BUILD_STATIC_LIBRARY)

#MessageCenter static lib
include $(CLEAR_VARS)
MY_CPP_LIST		:= $(wildcard $(MY_ROOT_PATH)/src/MessageCenter/*.cpp)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES:= $(MY_ROOT_PATH)/inc/MessageCenter \
				   $(MY_ROOT_PATH)/inc/commons
LOCAL_CPPFLAGS 	+= $(MY_CPPFLAGS)
LOCAL_CFLAGS 	+= $(MY_CFLAGS)

LOCAL_MODULE := libMessageCenter
include $(BUILD_STATIC_LIBRARY)

#engine1.1 static lib
include $(CLEAR_VARS)
MY_CPP_LIST		:= $(wildcard $(MY_ROOT_PATH)/src/engine/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/cache/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/camera/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/common/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/HAL/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/HAL/Shader/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/platform/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/render/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/renderObj/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/scence/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/texure/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/anim/*.cpp)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES:= $(MY_ROOT_PATH)/inc/MessageCenter \
				   $(MY_ROOT_PATH)/inc/commons	\
				   $(MY_ROOT_PATH)/inc/engine 	\
				   $(MY_ROOT_PATH)/src/engine 	\
				   $(MY_ROOT_PATH)/src/engine/anim 	\
				   $(MY_ROOT_PATH)/src/engine/common 	\
				   $(MY_ROOT_PATH)/src/engine/camera 	\
				   $(MY_ROOT_PATH)/src/engine/scence 	\
				   $(MY_ROOT_PATH)/src/engine/renderObj 	\
				   $(MY_ROOT_PATH)/src/engine/texure 	\
				   $(MY_ROOT_PATH)/src/engine/HAL

LOCAL_CPPFLAGS 	+= $(MY_CPPFLAGS)
LOCAL_CFLAGS 	+= $(MY_CFLAGS)
LOCAL_CFLAGS 	+= -D_OPENGL_1_X_

LOCAL_MODULE := libEngine11
include $(BUILD_STATIC_LIBRARY)

#engine2.0 static lib
include $(CLEAR_VARS)
MY_CPP_LIST		:= $(wildcard $(MY_ROOT_PATH)/src/engine/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/cache/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/camera/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/common/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/HAL/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/HAL/Shader/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/platform/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/render/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/renderObj/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/scence/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/texure/*.cpp)
MY_CPP_LIST		+= $(wildcard $(MY_ROOT_PATH)/src/engine/anim/*.cpp)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES:= $(MY_ROOT_PATH)/inc/MessageCenter \
				   $(MY_ROOT_PATH)/inc/commons	\
				   $(MY_ROOT_PATH)/inc/engine 	\
				   $(MY_ROOT_PATH)/src/engine 	\
				   $(MY_ROOT_PATH)/src/engine/anim 	\
				   $(MY_ROOT_PATH)/src/engine/common 	\
				   $(MY_ROOT_PATH)/src/engine/camera 	\
				   $(MY_ROOT_PATH)/src/engine/scence 	\
				   $(MY_ROOT_PATH)/src/engine/renderObj 	\
				   $(MY_ROOT_PATH)/src/engine/texure 	\
				   $(MY_ROOT_PATH)/src/engine/HAL

LOCAL_CPPFLAGS 	+= $(MY_CPPFLAGS)
LOCAL_CFLAGS 	+= $(MY_CFLAGS)
LOCAL_CFLAGS 	+= -D_OPENGL_2_0_

LOCAL_MODULE := libEngine20
include $(BUILD_STATIC_LIBRARY)

##############其它配置
# LOCAL_CPPFLAGS += -fvisibility-inlines-hidden

# LOCAL_LDFLAGS  += -Wl,--exclude-libs=ALL \
# -Wl,-O2 \
# -Wl,--sort-common \
# -Wl,-x \
# -Wl,-X \
# -Wl,--enable-new-dtags \
# -Wl,--no-define-common

# LOCAL_LDFLAGS += -Wl,--gc-sections   


# LOCAL_LDLIBS 	:= -landroid -lEGL -ljnigraphics
# LOCAL_LDLIBS	+= -llog
# #LOCAL_LDLIBS += -lGLESv1_CM
# LOCAL_LDLIBS 	+= -lGLESv2 
