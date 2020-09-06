##############################
#
# TOP Level Makefile
#
SUBDIRS = Neo
#LIB_SUBDIRS = lib/alarmLib lib/statLib lib/trcLib lib/sixwindLib CTMH/CtmhCmw/DecEnc
#LIB_SUBDIRS = lib/Lock lib/Thread
include $(MK_DIR)/make-common.mk
all: proto
	$(Q)$(MAKE) apps
#	$(Q)$(MAKE) lib
#lib: $(LIB_SUBDIRS)
#   $(Q)$(MAKE) apps
apps: $(SUBDIRS)
proto:
	echo "make protocol buffers"
	$(Q)$(MAKE) -C proto

##############################
#
# etc
#
.PHONY: clean app proto
clean:
	-$(Q)for dir in $(SUBDIRS) proto; do $(MAKE) -C $$dir clean; done
include $(MK_DIR)/make-target.mk

