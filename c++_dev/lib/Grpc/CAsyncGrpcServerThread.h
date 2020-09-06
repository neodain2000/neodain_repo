ifndef __CASYNC_GFUNC_SERVER__THREAD__
#define __CASYNC_GFUNC_SERVER__THREAD__
#include <string>
#include <memory>
#include <grpc++/grpc++.h>
#include <google/protobuf/text_format.h>
#include "CThread.h"
#include "CmfLog.h"
namespace Cmf
{
template <typename Service, typename Request, typename Response>
class CExecutor
{
public:
    typedef void(Service::*FUNC)(::grpc::ServerContext* context, Request* req, ::grpc::ServerAsyncResponseWriter<Response>* res, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    CExecutor(Service* service, grpc::ServerCompletionQueue* cq, FUNC func)
    : service_m(service), cq_m(cq), responder_m(&context_m), func_m(func)
    {
        (service_m->*func_m)(&context_m, &req, &responder_m, cq_m, cq_m, this);
        finished = false;
    }

    virtual ~CExecutor(){}
    bool work()
    {
        if (!finished)
        {
            if(Cmf::CCmf::Log() != 0 && Cmf::CCmf::Log()->checkFiltered(FRAMEWORKSVCID, CLog::DEBUG) == false)
            {
                Cmf::CCmf::Log()->write(0, CLog::DEBUG, FRAMEWORKSVCID,0,0,0,__FILE__,__LINE__, "Recieve Async gRPC Message");
                std::string message;
                google::protobuf::TextFormat::PrintToString(req, &message);
                Cmf::CCmf::Log()->write(0, CLog::DEBUG, FRAMEWORKSVCID,0,0,0,__FILE__,__LINE__, "%s", message.c_str());
            }
            vfWork(&req, &res);
            responder_m.Finish(res, grpc::Status::OK, this);
            finished = true;
        }
        else
        {
            delete this;
        }
        return finished;
    }
    virtual void vfWork(Request* req, Response* res) = 0;
private:
    Service* service_m;
    grpc::ServerCompletionQueue* cq_m;
    grpc::ServerContext context_m;
    grpc::ServerAsyncResponseWriter<Response> responder_m;
    FUNC func_m;
    Request req;
    Response res;
    bool finished;
};
template <typename Service, typename Executor>
class CAsyncGrpcServerThread final : public CThread
{
public:
    CAsyncGrpcServerThread(const std::string& address, typename Executor::FUNC func) : CThread("CAsyncGrpcServerThread", THREAD_SCHED_OTHER, 10, 1024*1024, false), func_m(func)
    {
        grpc::ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_m);
        cq_m = builder.AddCompletionQueue();
        server_m = builder.BuildAndStart();
    }
    ~CAsyncGrpcServerThread()
    {
        server_m->Shutdown();
        cq_m->Shutdown();
    }
    virtual void workFunc() override
    {
        new Executor(&service_m, cq_m.get(), func_m);
        void* tag;
        bool ok;
        while (true)
        {
            if (cq_m->Next(&tag, &ok) && ok)
            {
                if (static_cast<Executor*>(tag)->work())
                {
                {
                    new Executor(&service_m, cq_m.get(), func_m);
                }
            }
        }
    }
private:
    typename Executor::FUNC func_m;
    std::unique_ptr<grpc::Server> server_m;
    Service service_m;
    std::unique_ptr<grpc::ServerCompletionQueue> cq_m;
};
}
#endif // __CASYNC_GFUNC_SERVER__THREAD__
