#pragma once
#include <JGW_CSTSEServerModule/IJGW_CSTSEServerModuleInterface.h>
#include <poco/Net/ServerSocket.h>
#include <map>
#include <poco/NotificationQueue.h>
#include <JGW_FoundationFunc/queue_buffer.hpp>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
    class CCJGW_CSTSEServerModule : public IJGW_CSTSEServerModuleInterface
    {
    public:
        static IJGW_CSTSEServerModuleInterface* GetCSTSEServerModule();
    private:
        CCJGW_CSTSEServerModule(void);
        ~CCJGW_CSTSEServerModule(void);
    private:
        //! 
        virtual bool StartCSTSEServer(int socketPort = 3008);
        //! 
        virtual void StopCSTSEServer();
        //!
        virtual std::shared_ptr<Poco::Net::StreamSocket> GetSocketToFlagString(const std::string& strFlag);
        //! 
        virtual void CloseCSTSESocket(std::shared_ptr<Poco::Net::StreamSocket> ptrSocket);
        //!
        static void ServerThreadFunc(void* param);
        //! 
        static void ClientThreadFunc(void* param);
    private:
        bool CheckStreamSocketConnect();
        std::string GetFlagStringToSocket(std::shared_ptr<Poco::Net::StreamSocket> ptrSocket);      
    private:
        bool mbStopped;
        CCJGW_CriticalSectionLock mCriticalSectionLock;
        Poco::Thread mServerThread;
        Poco::Thread mClientThread;
        Poco::Net::ServerSocket mServerSocket;
        queue_buffer<std::shared_ptr<Poco::Net::StreamSocket>> mQueueStreamSocket;
        std::map<std::string,std::shared_ptr<Poco::Net::StreamSocket>> mmapServerSocket;
    };
}

