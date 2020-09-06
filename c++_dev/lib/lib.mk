CPPFLAGS += -DBOOST_ALL_DYN_LINK -D__LOG_IMP_SPDLOG__ -I$(NEODAIN_DIR)/lib/hdr
LDLIBS += -lboost_system -lpthread -lgrpc++ -lgrpc -lprotobuf -pthread -lboost_filesystem -lboost_iostreams -lz
