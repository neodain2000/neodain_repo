PROTOC = protoc
PROTOS_PATH = ./
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
PROTO_FILES := $(wildcard *.proto)
GRPC_PB_FILES := $(PROTO_FILES:.proto=.grpc.pb.cc)
PB_FILES := $(PROTO_FILES:.proto=.pb.cc)
#---------------------------------
# target rules
#
all: $(GRPC_PB_FILES) $(PB_FILES)
.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc : %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=./ --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=./ $<
clean :
	-rm -rf *.proto.pb.* *.pb.*
