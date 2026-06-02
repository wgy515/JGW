#include "StdAfx.h"
#include "CJGW_CSTSEServerModule.h"
using namespace Poco::Net;
namespace JGW
{
    CCJGW_CSTSEServerModule::CCJGW_CSTSEServerModule(void) : mbStopped(false)
    {
    }


    CCJGW_CSTSEServerModule::~CCJGW_CSTSEServerModule(void)
    {
    }

    IJGW_CSTSEServerModuleInterface* CCJGW_CSTSEServerModule::GetCSTSEServerModule()
    {
        static CCJGW_CSTSEServerModule gCSTSEServerModule;
        return &gCSTSEServerModule;
    }

    bool CCJGW_CSTSEServerModule::StartCSTSEServer(int socketPort /* = 3008 */)
    {
        try
        {
            IPAddress wildcardAddr;
            SocketAddress address(wildcardAddr, socketPort);
            mServerSocket.impl()->bind(address, true);
            mServerSocket.impl()->listen(64);
            mbStopped = false;
            mmapServerSocket.clear();
            mServerThread.start(&CCJGW_CSTSEServerModule::ServerThreadFunc,this);
            mClientThread.start(&CCJGW_CSTSEServerModule::ClientThreadFunc,this);

            return true;
        }
        catch (...)
        {
        }
        return false;
    }

    void CCJGW_CSTSEServerModule::ServerThreadFunc(void* param)
    {
        CCJGW_CSTSEServerModule* pServerThread = (CCJGW_CSTSEServerModule*)param;
        while (!pServerThread->mbStopped)
        {
            Poco::Timespan timeout(250000);
            try
            {
                if (pServerThread->mServerSocket.poll(timeout, Socket::SELECT_READ))
                {
                    try
                    {
                        // StreamSocket ss = _socket.acceptConnection();
                        std::shared_ptr<StreamSocket> ptrStreamSocket(new StreamSocket(pServerThread->mServerSocket.acceptConnection()));
                        // enabe nodelay per default: OSX really needs that
                        ptrStreamSocket->setNoDelay(true);
                        pServerThread->mQueueStreamSocket.put(ptrStreamSocket);
                    }
                    catch (Poco::Exception&)
                    {
                        //ErrorHandler::handle(exc);
                    }
                    catch (std::exception&)
                    {
                        //ErrorHandler::handle(exc);
                    }
                    catch (...)
                    {
                        //ErrorHandler::handle();
                    }
                }
            }
            catch (Poco::Exception&)
            {
                //ErrorHandler::handle(exc);
                // possibly a resource issue since poll() failed;
                // give some time to recover before trying again
                Poco::Thread::sleep(50); 
            }
        }
    }

    void CCJGW_CSTSEServerModule::ClientThreadFunc(void* param)
    {
        CCJGW_CSTSEServerModule* pServerThread = (CCJGW_CSTSEServerModule*)param;
        while (!pServerThread->mbStopped)
        {
            if (pServerThread->mQueueStreamSocket.is_empty())
            {
                pServerThread->CheckStreamSocketConnect();
                Poco::Thread::sleep(500); 
                continue;
            }
            std::shared_ptr<Poco::Net::StreamSocket> ptrStreamSocket = pServerThread->mQueueStreamSocket.get();
            std::string strFlag = pServerThread->GetFlagStringToSocket(ptrStreamSocket);
            if (!strFlag.empty())
            {
                CCJGW_CriticalSectionAutoLock lock(pServerThread->mCriticalSectionLock);
                pServerThread->mmapServerSocket[strFlag.c_str()] = ptrStreamSocket;
            }
            Poco::Thread::sleep(50); 
            continue;
        }
    }

    bool CCJGW_CSTSEServerModule::CheckStreamSocketConnect()
    {
        CCJGW_CriticalSectionAutoLock lock(mCriticalSectionLock);
        std::map<std::string,std::shared_ptr<Poco::Net::StreamSocket>>::iterator it = mmapServerSocket.begin();
        while (it != mmapServerSocket.end())
        {
            try
            {
                it->second->sendBytes("\0", 1);
                ++ it; 
                continue;
            }
            catch (...)
            {

            }
            CloseCSTSESocket(it->second);
            it = mmapServerSocket.erase(it); 
        }
        return true;
    }

    std::string CCJGW_CSTSEServerModule::GetFlagStringToSocket(std::shared_ptr<Poco::Net::StreamSocket> ptrSocket)
    {
        Poco::Timespan timeout(10 * 1000); 
        try
        {
            if (ptrSocket->poll(timeout, Socket::SELECT_READ))
            {
                char buffer[1024];
                int n = ptrSocket->receiveBytes(buffer, sizeof(buffer));
                buffer[n] = '\0';
                ptrSocket->sendBytes(buffer, n);
                buffer[n] = '\0';
                return buffer;
            } 
        }
        catch (...)
        {
            //std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
        }

        try
        {
            ptrSocket->close();
        }
        catch (...)
        {

        }

        return "";
    }

    std::shared_ptr<StreamSocket> CCJGW_CSTSEServerModule::GetSocketToFlagString(const std::string& strFlag)
    {
        CCJGW_CriticalSectionAutoLock lock(mCriticalSectionLock);
        std::map<std::string,std::shared_ptr<Poco::Net::StreamSocket>>::iterator it = mmapServerSocket.find(strFlag);
        if (it != mmapServerSocket.end())
        {
            mmapServerSocket.erase(it);
            return it->second;
        }
        std::shared_ptr<StreamSocket> ptrSocket(NULL);
        return ptrSocket;
    }

    void CCJGW_CSTSEServerModule::StopCSTSEServer()
    {
        mbStopped = true;
        try
        {
            mServerSocket.close();
        }
        catch (...)
        {

        }
        CCJGW_CriticalSectionAutoLock lock(mCriticalSectionLock);
        std::map<std::string,std::shared_ptr<Poco::Net::StreamSocket>>::iterator it = mmapServerSocket.begin();
        while (it != mmapServerSocket.end())
        {
            CloseCSTSESocket(it->second);
            ++ it; 
        }
        mmapServerSocket.clear();
    }

    void CCJGW_CSTSEServerModule::CloseCSTSESocket(std::shared_ptr<StreamSocket> ptrSocket)
    {
        try
        {
            ptrSocket->shutdown();
            ptrSocket->close();
        }
        catch (...)
        {

        }
    }
}
