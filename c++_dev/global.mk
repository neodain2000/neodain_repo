################################
# global scope make rule
################################
# for Preprocessor
CPPFLAGS += -O2
# for C/C++ compiler
CFLAGS +=
# for C++ compiler
CXXFLAGS += -std=c++11
# for linker
LDFLAGS += -L$(LIB_DIR)
LDFLAGS += -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed
#EPG_CDR_VER := Rel8
EPG_CDR_VER := Rel13 
