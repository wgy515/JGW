#pragma once
#include <string>
#include <queue>
#include <poco/Mutex.h>
#include <Poco/Net/Net.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <TSE_MSFoundationLib/CTSE_ControlMsgBase.h>

namespace JGW
{
    class CTSE_ControlMsg : public CTSE_ControlMsgBase
    {
    public:
        CTSE_ControlMsg(Poco::Net::SocketAddress socketAddress,const std::string& strID);
        ~CTSE_ControlMsg(void);
    public:
        void CloseControlMsg();

        bool ReadControlMsg(std::string& strRead);
        /// 读取log，无数据返回false

        //void SetSocketAddress(Poco::Net::SocketAddress socketAddress);

        bool WriteControlMsg(const std::string& strWrite);
        /// 写控制信息到UDP客户端

        void PushControlMsg(const std::string& strLog);
        /// push log到缓冲区中
    private:
        Poco::Net::DatagramSocket _udpSocket;
        Poco::Net::SocketAddress mSocketAddress;
        std::string mstrID;
        std::queue<std::string> mqueueControlMsg;
        Poco::Mutex mMutex;
    };
}


