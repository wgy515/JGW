#pragma once
#include <JGW_CSharpModule/CJGW_CSharpCommunicationBase.h>
namespace JGW
{
    class CJGW_PipeCommunication : public CJGW_CSharpCommunicationBase
    {
    public:
        CJGW_PipeCommunication(void);
        ~CJGW_PipeCommunication(void);
    private:
        //! strHost = "\\\\.\\Pipe\\test"
        virtual bool Create(const std::string& strHost,size_t port);
        virtual bool Connect();
        virtual bool IsConnect();
        virtual int Read(char* buf,size_t len);
        virtual bool Write(char* buf,size_t len);
        virtual void DisConnect();
    private:
        bool mbConnected;
        HANDLE mhPipe;
    };

}

