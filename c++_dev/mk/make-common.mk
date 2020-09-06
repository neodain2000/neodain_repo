#-------------------------------------------------------------------------------
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands
ifdef V
  ifeq ("$(origin V)", "command line")
    BUILD_VERBOSE = $(V)
  endif
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 0
endif
export BUILD_VERBOSE

ifdef CLANG
  ifeq ("$(origin CLANG)", "command line")
    CLANG_BUILD = $(CLANG)
  endif
endif
ifndef CLANG_BUILD
  CLANG_BUILD = 0
endif
export CLANG_BUILD

#-------------------------------------------------------------------------------
# Normally, we echo the whole command before executing it. By making
# that echo $($(quiet)$(cmd)), we now have the possibility to set
# $(quiet) to choose other forms of output instead, e.g.
#
#         quiet_cmd_cc_o_c = Compiling $(RELDIR)/$@
#         cmd_cc_o_c       = $(CC) $(c_flags) -c -o $@ $<
#
# If $(quiet) is empty, the whole command will be printed.
# If it is set to "quiet_", only the short version will be printed.
# If it is set to "silent_", nothing wil be printed at all, since
# the variable $(silent_cmd_cc_o_c) doesn't exist.
#
# A simple variant is to prefix commands with $(Q) - that's usefull
# for commands that shall be hidden in non-verbose mode.
#
#   $(Q)ln $@ :<
#
# If BUILD_VERBOSE equals 0 then the above command will be hidden.
# If BUILD_VERBOSE equals 1 then the above command is displayed.

ifeq ($(BUILD_VERBOSE),1)
  quiet =
  Q =
else
  quiet=quiet_
  Q = @
  MAKEFLAGS += --no-print-directory
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(MAKEFLAGS)),)
  quiet=silent_
endif

export quiet Q BUILD_VERBOSE

#-------------------------------------------------------------------------------
# To build with more strickt compiler warnings, use the W=1 flag on the make
# command line
ifdef W
  ifeq ("$(origin W)", "command line")
    BUILD_WARNINGS = $(W)
  endif
endif
ifndef BUILD_WARNINGS
  BUILD_WARNINGS = 0
endif


################################################################################
# Variables
#
################################################################################

include $(MK_DIR)/make-cross.mk
include $(MK_DIR)/make-path.mk

include $(NEODAIN_DIR)/global.mk

# Make variables (CC, etc...)
AS      = $(CROSS_COMPILE)as $(CPUCFLAGS)
LD      = $(CROSS_COMPILE)ld $(CPUCFLAGS)
CC      = $(CROSS_COMPILE)gcc$(CC_VERSION) $(CPUCFLAGS)
CXX     = $(CROSS_COMPILE)g++$(CC_VERSION)  $(CPUCFLAGS)
CPP     = $(CC) -E
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

MAKEARCH = $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
#$(info MAKEARCH = $(MAKEARCH))

ifeq ($(CLANG_BUILD),1)
CLANG_FLAGS +=
CC      = clang $(CLANG_FLAGS)
CXX     = clang++ $(CLANG_FLAGS)
endif

export AS LD CC CXX CPP AR NM STRIP OBJCOPY OBJDUMP MAKEARCH CLANG_FLAGS


SRC_FILES   += $(CPP_FILES)

#ARFLAGS     += rcs

LDFLAGS     += -g -lpthread -Wl,--export-dynamic

CPPFLAGS    += -c -Wall -D_GNU_SOURCE -fPIC

CPPFLAGS    += -Wcomment -Wnonnull \
               -Wextra -Wno-strict-aliasing \
               -Wcast-align -Wwrite-strings -Wno-unused-parameter \
               -Wno-format-security -Wno-ignored-qualifiers -Wno-unused-result

CXXFLAGS    += -Wsign-compare

SHARED_LDFLAGS  += -shared -fPIC -pthread -lpthread

CFLAGS      += $(EXTRA_CFLAGS)
CPPFLAGS    += $(EXTRA_CPPFLAGS)
LDFLAGS     += $(EXTRA_LDFLAGS)
LDLIBS      += $(EXTRA_LDLIBS)
SHARED_LDFLAGS  += $(EXTRA_SHARED_LDFLAGS)

# The compilation/link flags are passed to lower directories as well
export EXTRA_CFLAGS EXTRA_CPPFLAGS EXTRA_LDFLAGS EXTRA_LDLIBS

ALL_DEPENDENCIES += Makefile $(NEODAIN_DIR)/global.mk

# Convert all possible extensions to .o
oFilesTmp := $(SRC_FILES:.c=.o)
oFilesTmp := $(oFilesTmp:.cc=.o)
oFilesTmp := $(oFilesTmp:.cpp=.o)

# Convert all possible extensions to .d
dFilesTmp := $(SRC_FILES:.c=.d)
dFilesTmp := $(dFilesTmp:.cc=.d)
dFilesTmp := $(dFilesTmp:.cpp=.d)

################################################################################
# Generating object file list from SRC_FILES list:
# file1.c           --->  $(OBJ_DIR)/file1.o
# ../common/file2.c --->  $(OBJ_DIR)/file2.o
# ../config/file3.c --->  $(OBJ_DIR)/file3.o

obj_files := $(addprefix $(OBJ_DIR)/,$(notdir $(oFilesTmp)))

################################################################################
# Generating dependency file list
# file1.c           --->  $(DEP_DIR)/file1.d
# ../common/file2.c --->  $(DEP_DIR)/file2.d
# ../config/file3.c --->  $(DEP_DIR)/file3.d
dep_files :=$(addprefix $(DEP_DIR)/,$(notdir $(dFilesTmp) ))

DEP_FLAGS = -MMD -MF $(addprefix $(DEP_DIR)/,$(patsubst %.o,%.d,$(notdir $@)))

# Add the directories where the source files for different libraries are found
# into the 'vpath'.
vpath_list = $(dir $(SRC_FILES))

vpath %.c $(vpath_list)
vpath %.cc $(vpath_list)
vpath %.cpp $(vpath_list)
