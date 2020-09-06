################################################################################
# Command definitions
#
################################################################################
MKDIR = mkdir -p
# If quiet is set, only print short version of command
cmd = @$(if $($(quiet)cmd_$(1)),echo '  $($(quiet)cmd_$(1))' &&) $(cmd_$(1))
#-------------------------------------------------------------------------------
# Remove files (assumes the rm_files variable is set):
quiet_cmd_rmfiles = $(if $(wildcard $(rm_files)),CLEAN   $(wildcard $(rm_files)))
	cmd_rmfiles = $(if $(rm_files), rm -rf $(rm_files))
#-------------------------------------------------------------------------------
# Create a subdirectory (recursively):
quiet_cmd_mkdir = MKDIR $(call quiet-strip,$@)
	cmd_mkdir = $(MKDIR) $@
# Generate dependency file for a .cc file:
#define make-depend
#       $(CXX) -MM \
#        -MF $2 \
#        -MP \
#        -MT $3 \
#        -MT $2 \
#        $(CPPFLAGS) \
#        $(CXXFLAGS) \
#        $(TARGET_ARCH) \
#        $1
#endef
define quiet-strip
$(shell echo $1 | sed -e 's/.*target\//$$TARGET_PATH\//g')
endef
#-------------------------------------------------------------------------------
# Build depend files
quiet_cmd_depend = DEP $(call quiet-strip,$@) 
	cmd_depend = $(call make-depend,$<,$@,$(addprefix $(OBJ_DIR)/,$(subst .c,.o,$(subst .cc,.o,$(subst .cpp,.o,$(notdir $<))))))
#-------------------------------------------------------------------------------
# Build .o files from .c
quiet_cmd_cc_o_c = CC $(call quiet-strip,$@)
	cmd_cc_o_c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(DEP_FLAGS) $(OUTPUT_OPTION) $<
#-------------------------------------------------------------------------------
# Build .o files from .cc
quiet_cmd_cxx_o_c = C++ $(call quiet-strip,$@)
	cmd_cxx_o_c = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(DEP_FLAGS) $(OUTPUT_OPTION) $<
ifeq ($(CLANG_BUILD),1)
# override short print
quiet_cmd_cc_o_c = CLANG $(call quiet-strip,$@)
quiet_cmd_cxx_o_c = CLANG++ $(call quiet-strip,$@)
endif
#-------------------------------------------------------------------------------
# Link an executable from .o and lib files
# To be called as $(call cmd,link,<object-files>)
# So that $2 represents here all the <object-files>.
quiet_cmd_link = LINK $(call quiet-strip,$@) 
	cmd_link = $(CXX) $(LDFLAGS) $(TARGET_ARCH) $2 $(LOADLIBES) $(LDLIBS) -o $@
#-------------------------------------------------------------------------------
# Create an archive (lib) from .o files
quiet_cmd_ar = AR $(call quiet-strip,$@) 
	cmd_ar = $(AR) $(ARFLAGS) $@ $^
quiet_cmd_all_ar = AR $(call quiet-strip,$@) 
	cmd_all_ar = cd $(ALL_OBJ_DIR);\
		$(AR) $(ARFLAGS) $@ $(subst $(ALL_OBJ_DIR),.,$(strip $^)); \
		cd -
#-------------------------------------------------------------------------------
# Create a shared library from .o files
quiet_cmd_link_shared = LINK-SO $(call quiet-strip,$@)
	cmd_link_shared = $(CXX) $(LDFLAGS) $(TARGET_ARCH) $(EXTRA_LDLIBS) $^ -o $@ $(SHARED_LDFLAGS);
quiet_cmd_all_link_shared = ALINK-SO $(call quiet-strip,$@) 
	cmd_all_link_shared = cd $(ALL_OBJ_DIR); \
		$(CXX) $(LDFLAGS) $(TARGET_ARCH) $(EXTRA_LDLIBS) $(subst $(ALL_OBJ_DIR),.,$(strip $^)) -o $@ $(SHARED_LDFLAGS);\
		cd -
#-------------------------------------------------------------------------------
# Run ln -sf
quiet_cmd_ln = LN $(call quiet-strip,$@)
	cmd_ln = ln -sf

################################################################################
# Make targets
#
################################################################################
.PHONY: $(EXE_NAME) $(SHARED_LIB_NAME) $(STATIC_LIB_NAME) $(SUBDIRS) $(LIB_SUBDIRS)
$(EXE_NAME): $(DEP_DIR) $(OBJ_DIR) $(BIN_DIR) $(BIN_DIR)/$(EXE_NAME)
$(SHARED_LIB_NAME): $(DEP_DIR) $(OBJ_DIR) $(LIB_DIR) $(LIB_DIR)/$(SHARED_LIB_NAME)
$(STATIC_LIB_NAME): $(DEP_DIR) $(OBJ_DIR) $(LIB_DIR) $(LIB_DIR)/$(STATIC_LIB_NAME)
$(SUBDIRS):
	$(Q)$(MAKE) -C $@
$(LIB_SUBDIRS):
	$(Q)$(MAKE) -C $@
#############################################################
# Dependency management
# #
# # Don't include or remake dependencies when doing clean
# #
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
ifdef dep_files 
	-include $(dep_files)
endif
endif
#--------------- ----------------------------------------------------------------
# Generating .o from .c .C .cc or .cpp (compiling):
$(OBJ_DIR)/%.o: %.cc $(ALL_DEPENDENCIES)
	$(call cmd,cxx_o_c)
$(OBJ_DIR)/%.o: %.cpp $(ALL_DEPENDENCIES)
	$(call cmd,cxx_o_c)
$(OBJ_DIR)/%.o: %.c $(ALL_DEPENDENCIES)
	$(call cmd,cc_o_c)
#-------------------------------------------------------------------------------
# Generating dependency file from a lot of possible c or c++ file endings
#$(DEP_DIR)/%.d: %.cc $(DEP_DIR)
#   $(call cmd,depend)
#
#$(DEP_DIR)/%.d: %.cpp $(DEP_DIR)
#   $(call cmd,depend)
#
#$(DEP_DIR)/%.d: %.c $(DEP_DIR)
#   $(call cmd,depend)
#-------------------------------------------------------------------------------
# Generating directories
$(DEP_DIR):
	$(call cmd,mkdir)
$(OBJ_DIR):
	$(call cmd,mkdir)
$(BIN_DIR):
	$(call cmd,mkdir)
$(LIB_DIR):
	$(call cmd,mkdir)
#-------------------------------------------------------------------------------
ifdef SHARED_LIB_NAME
$(LIB_DIR)/$(SHARED_LIB_NAME): $(obj_files) $(SUBDIRS)
	$(call cmd,link_shared,$(obj_files))
endif

ifdef STATIC_LIB_NAME
$(LIB_DIR)/$(STATIC_LIB_NAME): $(obj_files) $(SUBDIRS)
	$(call cmd,ar,$(obj_files))
endif
ifdef EXE_NAME
$(BIN_DIR)/$(EXE_NAME): $(obj_files) $(SUBDIRS)
	$(call cmd,link,$(obj_files))
endif
################################################################################
# FORCE target to allow forcing any target to be re-made
#
FORCE:
