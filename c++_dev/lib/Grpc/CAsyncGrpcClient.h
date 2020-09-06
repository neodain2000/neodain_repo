#ifndef __CASYNC_GRPC_CLIENT__
#define __CASYNC_GRPC_CLIENT__
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <grpc++/grpc++.h>
#include "CThread.h"
#include "CmfLog.h"
using namespace std;
namespace Cmf
{
template <typename Service, typename Request, typename Response>
class CAsynGrpcClient
{
    typedef unique_ptr< ::grpc::ClientAsyncResponseReader<Response> >(Service::Stub::*FUNC)(::grpc::ClientContext* contex, const Request& req, ::grpc::CompletionQueue* cq);
    typedef function<void (int index, const grpc::Status& status)> CB_FUNC;
    public:
    explicit CAsynGrpcClient(FUNC func, CB_FUNC cb_func=nullptr) : func_m(func), cbfunc_m(cb_func)
    {
        cmfLog(CLog::TRACE, "CAsynGrpcClient create");
    }
    explicit CAsynGrpcClient(const string& target, FUNC func, CB_FUNC cb_func=nullptr) : func_m(func), cbfunc_m(cb_func)
    {
        targets_m.push_back(target);
        makeStubs(target);
        cmfLog(CLog::TRACE, "CAsynGrpcClient create");
    }
    ~CAsynGrpcClient()
    {
        cq_m.Shutdown();
        cmfLog(CLog::TRACE, "CAsynGrpcClient delete");
    }
    void operator()(const Request& req, Response& res, int msec=2000)
    {
        vector<unique_ptr<grpc::ClientAsyncResponseReader<Response>>> rpcs;
        vector<unique_ptr<grpc::ClientContext>> contexts;
        size_t count = stubs_m.size();
        cmfLog(CLog::TRACE, "Target count = %d\n", count);

        cmfLog_if(count==0, CLog::ERROR, "There is no target to send RPC");
        cmfLog_if(count!=1, CLog::ERROR, "Response Data will be overwritten");
        for (size_t i=0; i<count; i++)
        {
            grpc::ClientContext* pContext = new grpc::ClientContext();
            if (msec>0)
            {
                chrono::system_clock::time_point deadline = chrono::system_clock::now() + chrono::milliseconds(msec);
                pContext->set_deadline(deadline);
            }
            contexts.emplace_back(pContext);
            rpcs.emplace_back((stubs_m[i].get()->*func_m)(pContext, req, &cq_m));
            rpcs[i]->Finish(&res, statues_m[i].get(), (void*)i);
        }
        void* got_tag;
        bool ok = false;
        for (size_t i=0; i<count; i++)
        {
            cq_m.Next(&got_tag, &ok);

            const size_t index = size_t(got_tag);
            if (index < count)
            {
                const auto& status = *(statues_m[index].get());
                if (status.ok())
                {
                    cmfLog(CLog::DEBUG, "gRPC  OK(%d)", index);
                }
                else
                {
                    cmfLog(CLog::ERROR, "gRPC NOK(%d) : %s[%d]", index, status.error_message().c_str(), status.error_code());
                }
                if (cbfunc_m)
                {
                    cbfunc_m(index, status);
                }
            }
        }
    }
    void operator()(const Request& req, vector<Response>& reslist, int msec=2000)
    {
        vector<unique_ptr<grpc::ClientAsyncResponseReader<Response>>> rpcs;
        vector<unique_ptr<grpc::ClientContext>> contexts;

        size_t count = stubs_m.size();
        cmfLog_if(count==0, CLog::ERROR, "There is no target to send RPC");
        rpcs.reserve(count);
        reslist.reserve(count);
        for (size_t i=0; i<count; i++)
        {
            grpc::ClientContext* pContext = new grpc::ClientContext();
            if (msec>0)
            {
                chrono::system_clock::time_point deadline = chrono::system_clock::now() + chrono::milliseconds(msec);
                pContext->set_deadline(deadline);
            }
            contexts.emplace_back(pContext);
            rpcs.emplace_back((stubs_m[i].get()->*func_m)(pContext, req, &cq_m));
            reslist.push_back(Response());
            rpcs[i]->Finish(&reslist[i], statues_m[i].get(), (void*)i);
        }
        void* got_tag;
        bool ok = false;
        for (size_t i=0; i<count; i++)
        {
            cq_m.Next(&got_tag, &ok);
            const size_t index = size_t(got_tag);
            if (index < count)
            {
                const auto& status = *(statues_m[index].get());
                if (status.ok())
                {
                    cmfLog(CLog::DEBUG, "gRPC  OK(%d)", index);
                }
                else
                {
                    cmfLog(CLog::ERROR, "gRPC NOK(%d) : %s[%d]", index, status.error_message().c_str(), status.error_code());
                }
                if (cbfunc_m)
                {
                    cbfunc_m(index, status);
                }
            }
        }
    }
    void addTarget(const string& target)
    {
        makeStubs(target);
    }
    void addChannel(shared_ptr<::grpc::Channel> channel)
    {
        makeStubs(channel);
    }
private:
    void makeStubs(const string& target)
    {
        makeStubs(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
    }
    void makeStubs(shared_ptr<::grpc::Channel> channel)
    {
        stubs_m.emplace_back(Service::NewStub(channel));
        statues_m.emplace_back(new grpc::Status());
    }
    FUNC func_m;
    CB_FUNC cbfunc_m;
    chrono::milliseconds timeout_m;
    grpc::CompletionQueue cq_m;

    vector<string> targets_m;
    vector<unique_ptr<typename Service::Stub>> stubs_m;
    vector<unique_ptr<grpc::Status>> statues_m;
};
}
#endif // __CASYNC_GRPC_CLIENT__
