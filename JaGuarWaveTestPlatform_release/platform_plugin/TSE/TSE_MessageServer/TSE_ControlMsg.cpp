#include "StdAfx.h"
#include "TSE_ControlMsg.h"
#include <Poco/DeflatingStream.h>
#include <Poco/MemoryStream.h>
#include <Poco/StreamCopier.h>
#include <TSE_MSFoundationLib/TSE_MSLogJson.h>
#include <sstream>

namespace JGW
{
    CTSE_ControlMsg::CTSE_ControlMsg(Poco::Net::SocketAddress socketAddress,const std::string& strID) : mstrID(strID),mSocketAddress(socketAddress)
    {
    }


    CTSE_ControlMsg::~CTSE_ControlMsg(void)
    {
    }

    bool CTSE_ControlMsg::ReadControlMsg(std::string& strRead)
    {
        if (!mqueueControlMsg.empty())
        {
            mMutex.lock();
            strRead = mqueueControlMsg.front();
            mqueueControlMsg.pop();
            mMutex.unlock();
            return true;
        }
        return false;
    }

    void CTSE_ControlMsg::PushControlMsg(const std::string& strLog)
    {
        mMutex.lock();
        mqueueControlMsg.push(strLog);
        mMutex.unlock();
    }

    //int sendToUDPServer(const std::string& log, int flags /* = 0 */)
    //{
    //    std::ostringstream ostr;
    //    std::istringstream istr(log);
    //    //Poco::MemoryInputStream istr((const char*)log.c_str(),log.length());
    //    Poco::DeflatingOutputStream deflater(ostr);
    //    Poco::StreamCopier::copyStream(istr, deflater);
    //    deflater.close();

    //    return UDPClientBase::sendToUDPServer(ostr.str());
    //}

    bool CTSE_ControlMsg::WriteControlMsg(const std::string& strWrite)
    {   
        mMutex.lock();
        //Poco::Net::SocketAddress socketAddress (mSocketAddress);
        mMutex.unlock();

        std::string strMsg = CTSE_MSLogJson::GetMSLogJson(mstrID,strWrite,false,false);
        std::ostringstream ostr;
        std::istringstream istr(strMsg);
        Poco::DeflatingOutputStream deflater(ostr);
        Poco::StreamCopier::copyStream(istr, deflater);
        deflater.close();
        std::string msg = ostr.str();
        if (msg.length() > 0) 
        {
            return _udpSocket.sendTo((void*)(msg.c_str()),msg.length(),mSocketAddress) == msg.length();
        }
        return false;
    }

    //void CTSE_ControlMsg::SetSocketAddress(Poco::Net::SocketAddress socketAddress)
    //{
    //    //mMutex.lock();
    //    //mSocketAddress = socketAddress;
    //    //mMutex.unlock();
    //}

    void CTSE_ControlMsg::CloseControlMsg()
    {
        WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlExitApplicationType));
    }
}
