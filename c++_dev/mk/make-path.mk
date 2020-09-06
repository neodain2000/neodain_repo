# $(warning Building user component)
# in this case we create the BUILD_SUBPATH as the subdirectory under the
# model directory
AWK			:= awk
BUILD_SUBPATH := $(shell \
	pwd -P | $(AWK) '{match($$0,regex); print substr($$0,RLENGTH+1)}' regex="$(NEODAIN_DIR)/")
#$(warning NEODAIN_DIR=$(NEODAIN_DIR))
#$(warning BUILD_SUBPATH=$(BUILD_SUBPATH))
OBJ_DIR		:= $(NEODAIN_DIR)/target/$(BUILD_TARGET)/obj/$(BUILD_SUBPATH)
DEP_DIR		:= $(NEODAIN_DIR)/target/$(BUILD_TARGET)/dep/$(BUILD_SUBPATH)
LIB_DIR		:= $(NEODAIN_DIR)/target/$(BUILD_TARGET)/lib
BIN_DIR		:= $(NEODAIN_DIR)/target/$(BUILD_TARGET)/bin
export OBJ_DIR DEP_DIR LIB_DIR BIN_DIR
