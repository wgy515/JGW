#pragma once
#include <WinSock2.h>
#include <poco/Net/StreamSocket.h>
#include <string>
#include <memory>

namespace JGW
{
    class IJGW_CSTSEServerModuleInterface
    {
    public:     
        virtual ~IJGW_CSTSEServerModuleInterface() {}
        //! 
        virtual bool StartCSTSEServer(int socketPort = 3008) = 0;
        //! 
        virtual void StopCSTSEServer() = 0;
        //!
        virtual std::shared_ptr<Poco::Net::StreamSocket> GetSocketToFlagString(const std::string& strFlag) = 0;
        //!
        virtual void CloseCSTSESocket(std::shared_ptr<Poco::Net::StreamSocket> ptrSocket) = 0;
    };
}

#ifdef JGW_CSTSESERVERMODULE_EXPORTS
#define CSTSESERVERMODULE_API __declspec(dllexport)
#else
#define CSTSESERVERMODULE_API __declspec(dllimport)
#endif

#ifndef  JGW_CSTSESERVERMODULE_EXPORTS
#ifdef _DEBUG
#pragma comment(lib,"JGW_CSTSEServerModule_d.lib")
#else
#pragma comment(lib,"JGW_CSTSEServerModule.lib")
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif
    //CSTSESERVERMODULE_API JGW::IJGW_CSTSEServerModuleInterface* GetServerModule();
#ifdef __cplusplus
}
#endif