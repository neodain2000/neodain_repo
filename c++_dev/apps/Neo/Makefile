EXE_NAME := neodain
ifndef NEODAIN_DIR
NEODAIN_DIR = $(HOME)/Neodain
MK_DIR = $(NEODAIN_DIR)/mk
BUILD_TARGET = local
endif

# Set this variable to your location of the OSS ASN.1/C tools
OSSASN1 = $(NEODAIN_DIR)/libs/asn1/ossasn1/linux-x86-64.tgt/10.4.0.1/

#ifneq ("$(wildcard $(OSSASN1)/samples/common.mak)","")
#include $(OSSASN1)/samples/common.mak
#else
#$(warning File $$(OSSASN1)/samples/common.mak not found.)
#$(error Set OSSASN1 variable to your location of the OSS ASN.1/C tools.)
#endif

APP_SUB_DIRS := .\
				../proto\
				proto\
				msg\
				Cmd\

SRC_FILES := $(foreach dir,$(APP_SUB_DIRS),$(wildcard $(dir)/*.c))
CPP_FILES := $(foreach dir,$(APP_SUB_DIRS),$(wildcard $(dir)/*.cc))
LIBS_DIR := $(NEODAIN_DIR)/lib
HDR_DIR := $(NEODAIN_DIR)/hdr
LIB_HDR_DIR := $(NEODAIN_DIR)/lib/hdr 
APPS_DIR := $(NEODAIN_DIR)/apps
NEO_DIR := $(NEODAIN_DIR)/apps/Neo

EXTRA_CPPFLAGS += -I$(HDR_DIR)\
	-I$(LIBS_DIR)/proto\
	-I$(LIB_HDR_DIR)\
	-I$(APPS_DIR)/proto\
	-I$(NEO_DIR)/proto\
	-g
SUBDIRS :=
include $(NEODAIN_DIR)/lib/lib.mk
include $(NEODAIN_DIR)/apps/Neo/Neo.mk

#LDLIBS += -lcpprest -lrt -lcrypto -lssl -lafs -ltrc -lCDRmsg $(OSSTOED)
LDLIBS += -lneo -lcpprest -lrt -lcrypto -lssl
ALL_DEPENDENCIES := $(NEODAIN_DIR)/lib/lib.mk
include $(MK_DIR)/make-common.mk
#---------------------------------
# target rules
#
.PHONY: proto
all: proto
	$(Q)$(MAKE) $(EXE_NAME)
clean :
	$(Q)$(MAKE) -C proto clean
	-rm -rf $(DEP_DIR) $(OBJ_DIR) $(BIN_DIR)/$(EXE_NAME)
proto:
	echo "make protocol buffers"
	$(Q)$(MAKE) -C proto

include $(MK_DIR)/make-target.mk
