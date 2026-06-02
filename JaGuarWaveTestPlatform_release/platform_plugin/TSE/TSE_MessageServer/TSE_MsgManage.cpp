#include "StdAfx.h"
#include "TSE_MsgManage.h"
#include <TSE_MSFoundationLib/TSE_MSLogJson.h>
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>


namespace JGW
{
    CTSE_MsgManage::CTSE_MsgManage(void)
    {
    }


    CTSE_MsgManage::~CTSE_MsgManage(void)
    {
        CloseMsgManage();
    }

    void CTSE_MsgManage::CloseMsgManage()
    {
        for (std::map<std::string,std::shared_ptr<CTSE_ControlMsg>>::iterator it = mmapIDControlMsg.begin();
            it != mmapIDControlMsg.end();
            ++ it)
        {
            it->second->CloseControlMsg();
        }
        mmapIDControlMsg.clear();
        mmapIDLogMsg.clear();
    }

    void CTSE_MsgManage::HandleMsg(Poco::Net::SocketAddress socketAddress,const std::string& strJsonLog)
    {
        std::string strID,strMsg;
        bool encrypt = false,isLog = true;

        if (CTSE_MSLogJson::GetMSLogToJson(strID,strMsg,encrypt,isLog,strJsonLog))
        {
            //! 如果已经加密则进行解密在传递
            if (encrypt)
            {
                strMsg = CCJGW_CryptPassword::DecryptPassword(strMsg);
            }
            if(strMsg.empty())
            {
                return ;
            }
            if (isLog)
            {
                std::map<std::string,std::shared_ptr<CTSE_LogMsg>>::iterator it = mmapIDLogMsg.find(strID);
                if (it == mmapIDLogMsg.end())
                {
                    std::shared_ptr<CTSE_LogMsg> ptrLogMsg (new CTSE_LogMsg(strID));
                    ptrLogMsg->PushLog(strMsg);
                    mmapIDLogMsg.insert(std::make_pair<std::string,std::shared_ptr<CTSE_LogMsg>>(strID,ptrLogMsg));
                }
                else
                {
                    it->second->PushLog(strMsg);
                }
            }
            else
            {
                //TSEControlTypeEnum eControlType;
                //if (!CTSE_MSLogJson::GetMSControlType(eControlType,strMsg))
                //{
                //    return;
                //}
                ////! 收到退出信息删除Control Msg
                //if (TSEControlExitApplicationType == eControlType)
                //{
                //    std::map<std::string,std::shared_ptr<CTSE_ControlMsg>>::iterator it = mmapIDControlMsg.find(strID);
                //    if (it != mmapIDControlMsg.end())
                //    {
                //        mmapIDControlMsg.erase(it);
                //    }
                //    return ;
                //}

                std::map<std::string,std::shared_ptr<CTSE_ControlMsg>>::iterator it = mmapIDControlMsg.find(strID);
                if (it == mmapIDControlMsg.end())
                {
                    std::shared_ptr<CTSE_ControlMsg> ptrControlMsg (new CTSE_ControlMsg(socketAddress,strID));
                    ptrControlMsg->PushControlMsg(strMsg);
                    //ptrControlMsg->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlRegResultType));
                    mmapIDControlMsg.insert(std::make_pair<std::string,std::shared_ptr<CTSE_ControlMsg>>(strID,ptrControlMsg));
                }
                else
                {
                    //it->second->SetSocketAddress(socketAddress);
                    it->second->PushControlMsg(strMsg);
                }
            }     
        }
    }

    std::shared_ptr<CTSE_LogMsg> CTSE_MsgManage::GetLogMsgClassToID(const std::string& strID)
    {
        std::map<std::string,std::shared_ptr<CTSE_LogMsg>>::iterator it = mmapIDLogMsg.find(strID);
        if (it != mmapIDLogMsg.end())
        {
            return it->second;
        }
        return NULL;
    }

    std::shared_ptr<CTSE_ControlMsg> CTSE_MsgManage::GetControlMsgClassToID(const std::string& strID)
    {
        std::map<std::string,std::shared_ptr<CTSE_ControlMsg>>::iterator it = mmapIDControlMsg.find(strID);
        if (it != mmapIDControlMsg.end())
        {
            return it->second;
        }
        return NULL;
    }
}
