#pragma once
#include <string>
#include "TSE_LogMsg.h"
#include "TSE_ControlMsg.h"
#include <map>
#include <memory>
#include <poco/Mutex.h>

namespace JGW
{
    class CTSE_MsgManage
    {
    public:
        CTSE_MsgManage(void);
        ~CTSE_MsgManage(void);
    public:
        void HandleMsg(Poco::Net::SocketAddress socketAddress,const std::string& strJsonLog);

        /// 处理接收到Json log msg 
        /// {\"encrypt\":0,\"id\":\"123456\",\"msg\":\"55566666\"}
        void CloseMsgManage();

        std::shared_ptr<CTSE_LogMsg> GetLogMsgClassToID(const std::string& strID);
        /// 获取LOG MSG CLASS

        std::shared_ptr<CTSE_ControlMsg> GetControlMsgClassToID(const std::string& strID);
        /// 获取CONTROL MSG CLASS
    private:
        std::map<std::string,std::shared_ptr<CTSE_LogMsg>> mmapIDLogMsg;
        std::map<std::string,std::shared_ptr<CTSE_ControlMsg>> mmapIDControlMsg;
    };
}