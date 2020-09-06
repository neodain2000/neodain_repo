#ifndef __CSYNC_GRPC_SERVER__
#define __CSYNC_GRPC_SERVER__
#include <string>
#include <memory>
#include <grpc++/grpc++.h>
#include <google/protobuf/text_format.h>
#include "CThread.h"
#include "CmfLog.h"
namespace Cmf
{
template <typename Service>
class CSyncGrpcServerThread : public Service, public CThread
{
public:
    CSyncGrpcServerThread(const std::string& address) : CThread("CSyncGrpcServerThreadThread", THREAD_SCHED_OTHER, 10, 1024*1024, false)
    {
        grpc::ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(this);
        server_m = builder.BuildAndStart();
    }

    ~CSyncGrpcServerThread()
    {
        server_m->Shutdown();
    }
    virtual void workFunc() override
    {
        server_m->Wait();
    }
private:
    std::unique_ptr<grpc::Server> server_m;
};
}
#endif // __CSYNC_GRPC_SERVER__
