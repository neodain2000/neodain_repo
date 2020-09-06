# set local build as a default target
#ifndef NEODAIN_DIR
NEODAIN_DIR = $(shell readlink -f .)
MK_DIR = $(NEODAIN_DIR)/mk
BUILD_TARGET = local
export NEODAIN_DIR MK_DIR BUILD_TARGET
#endif

RHEL_XTOOLS = /localdisk2/epc/buildtools/x-tools/x86_64-rhel73-linux-gnu
ASN1_DIR = libs/asn1/ossasn1_linux64_trial_v10/ossasn1/linux-x86-64.trial/10.4.0/samples/sample

TARGET_BUILD = 0
include $(MK_DIR)/make-common.mk

all: apps
.PHONY: apps lib prepare tags
apps : lib 
	$(Q)$(MAKE) -C $@

lib : prepare
	$(Q)$(MAKE) -C $@

prepare:

tags:
	ctags -R ./
##############################
#
# etc
#
.PHONY: clean
clean: lib_clean apps_clean
	-@rm -rf $(TARGET_PATH)
%_clean:
	-$(Q)[ ! -d $(@:_clean=) ] || $(MAKE) -C $(@:_clean=) clean
include $(MK_DIR)/make-target.mk

