ARCH =
CROSS_COMPILE =
CC_VERSION =
CPUCFLAGS =
CPPFLAGS += -I$(CROSS_SYSROOT)/usr/local/include
LDFLAGS += -L$(CROSS_SYSROOT)/usr/lib64 -L$(CROSS_SYSROOT)/usr/local/lib
export ARCH CROSS_COMPILE CC_VERSION CPUCFLAGS
