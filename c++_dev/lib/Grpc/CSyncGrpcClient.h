#ifndef __CSYNC_GRPC_CLIENT__
#define __CSYNC_GRPC_CLIENT__
#include <string>
#include <memory>
#include <list>
#include <thread>
#include <grpc++/grpc++.h>
#include <grpc++/impl/codegen/sync_stream.h>
#include "CmfLog.h"
namespace Cmf
{
template <typename Service, typename Request, typename Response>
class CSynGrpcClient
{
    typedef ::grpc::Status (Service::Stub::*ONE_FUNC_ONE) (::grpc::ClientContext* contex, const Request& req, Response* res);
    typedef std::unique_ptr< ::grpc::ClientReader<Response> > (Service::Stub::*ONE_FUNC_MULTI) (::grpc::ClientContext* context, const Request& req);
    typedef std::unique_ptr< ::grpc::ClientWriter<Request> > (Service::Stub::*MULTI_FUNC_ONE) (::grpc::ClientContext* context, Response* res);
    typedef std::unique_ptr< ::grpc::ClientReaderWriter<Request, Response>> (Service::Stub::*MULTI_FUNC_MULTI) (::grpc::ClientContext* context);

public:
    explicit CSynGrpcClient(const std::string& target, ONE_FUNC_ONE func)
    : stub_m(Service::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()))),
      f1_m(func), f2_m(nullptr),  f3_m(nullptr),  f4_m(nullptr)
    {
        cmfLog(CLog::TRACE, "CSynGrpcClient create 1-1");
    }
    explicit CSynGrpcClient(const std::string& target, ONE_FUNC_MULTI func)
    : stub_m(Service::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()))),
      f1_m(nullptr), f2_m(func), f3_m(nullptr),  f4_m(nullptr)
    {
        cmfLog(CLog::TRACE, "CSynGrpcClient create 1-*");
    }
    explicit CSynGrpcClient(const std::string& target, MULTI_FUNC_ONE func)
    : stub_m(Service::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()))),
      f1_m(nullptr), f2_m(nullptr), f3_m(func),  f4_m(nullptr)
    {
        cmfLog(CLog::TRACE, "CSynGrpcClient create *-1");
    }
    explicit CSynGrpcClient(const std::string& target, MULTI_FUNC_MULTI func)
    : stub_m(Service::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()))),
      f1_m(nullptr), f2_m(nullptr), f3_m(nullptr),  f4_m(func)
    {
        cmfLog(CLog::TRACE, "CSynGrpcClient create *-*");
    }
    bool operator()(const Request& req, Response& res)
    {
        ::grpc::ClientContext context;
        if (f1_m)
        {
            ::grpc::Status status = (stub_m.get()->*f1_m)(&context, req, &res);
            cmfLog(CLog::DEBUG, "%s", status.ok() ? "gRPC Success" : "gRPC Fail");
            return status.ok() ? true:false;
        }
        cmfLog(CLog::ERROR, "wrong method call");
        return false;
    }
    bool operator()(const Request& req, std::list<Response>& reslist)
    {
        ::grpc::ClientContext context;
        if (f2_m)
        {
            Response res;
             std::unique_ptr< grpc::ClientReader<Response> > reader((stub_m.get()->*f2_m)(&context, req));
            while (reader->Read(&res))
            {
                reslist.push_back(res);
            }
            ::grpc::Status status = reader->Finish();
            cmfLog(CLog::DEBUG, "%s", status.ok() ? "gRPC Success" : "gRPC Fail");
            return status.ok() ? true:false;
        }
        cmfLog(CLog::ERROR, "wrong method call");
        return false;
    }
    bool operator()(const std::list<Request>& reqlist, Response& res)
    {
        ::grpc::ClientContext context;
        if (f3_m)
        {
            std::unique_ptr< grpc::ClientWriter<Request> > writer((stub_m.get()->*f3_m)(&context, &res));
            for (const auto& req : reqlist)
            {
                writer->Write(req);
            }
            writer->WritesDone();
            ::grpc::Status status = writer->Finish();
            cmfLog(CLog::DEBUG, "%s", status.ok() ? "gRPC Success" : "gRPC Fail");
            return status.ok() ? true:false;
        }
        cmfLog(CLog::ERROR, "wrong method call");
        return false;
    }
    bool operator()(const std::list<Request>& reqlist, std::list<Response>& reslist)
    {
        ::grpc::ClientContext context;
        if (f4_m)
        {
            Response res;
            std::unique_ptr< grpc::ClientReaderWriter<Request, Response> > stream((stub_m.get()->*f4_m)(&context));
            std::thread writer( [stream, reqlist]() {
                for (const auto& req : reqlist)
                {
                    stream->Writes(req);
                }
                stream->WritesDone();
            } );
            while (stream->Read(&res))
            {
                reslist.push_back(res);
            }
            writer.join();
            ::grpc::Status status = stream->Finish();
            cmfLog(CLog::DEBUG, "%s", status.ok() ? "gRPC Success" : "gRPC Fail");
            return status.ok() ? true:false;
        }
        cmfLog(CLog::ERROR, "wrong method call");
        return false;
    }
private:
    std::unique_ptr<typename Service::Stub> stub_m;
    ONE_FUNC_ONE f1_m;
    ONE_FUNC_MULTI f2_m;
    MULTI_FUNC_ONE f3_m;
     MULTI_FUNC_MULTI f4_m;
};
}
#endif // __CSYNC_GRPC_CLIENT__
