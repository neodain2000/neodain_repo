
SHARED_LIB_NAME := libneo.so

include lib.mk

CPPFLAGS += -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -lcrypt -g
# lists up all sub-directorys recursively
APP_SUB_DIRS := .\
				hdr \
				Lock \
				proto \

# user defined flags
CPPFLAGS += -O2
CPPFLAGS += -DHAVE_MEASURE_PERFORM -DABSTRACT_SOCK_ADDR
CPPFLAGS += -DGRACEFUL_CLOSE -DNEW_SCTP_HDR -DATOMIC_BUF_CNT -D__USE_CLI__
# log implementation setting
# if using spdlog

ALL_DEPENDENCIES := lib.mk
# Component level include directory is always added:
CPPFLAGS += -I./hdr -I./proto -I../include
SRC_FILES := $(foreach dir,$(APP_SUB_DIRS),$(wildcard $(dir)/*.c))
CPP_FILES := $(foreach dir,$(APP_SUB_DIRS),$(wildcard $(dir)/*.cc))
include $(MK_DIR)/make-common.mk
#---------------------------------
# target rules
#
.PHONY: clean proto 
all: proto
	$(Q)$(MAKE) $(SHARED_LIB_NAME)
proto:
	echo "make protocol buffers"
	$(Q)$(MAKE) -C proto
clean :
	$(Q)$(MAKE) -C proto clean
	-rm -rf $(DEP_DIR) $(OBJ_DIR) $(LIB_DIR)/$(SHARED_LIB_NAME)

ctags :
	ctags -R --exclude=hdr
samples :
	$(Q)$(MAKE) -C samples

include $(MK_DIR)/make-target.mk

