#include "StdAfx.h"
#include "TSE_LogMsg.h"

namespace JGW
{
    CTSE_LogMsg::CTSE_LogMsg(const std::string& strID) : mstrID(strID)
    {
    }


    CTSE_LogMsg::~CTSE_LogMsg(void)
    {
    }

    std::string CTSE_LogMsg::GetID()
    {
        return mstrID;
    }

    void CTSE_LogMsg::SetID(const std::string& strID)
    {
        mstrID = strID;
    }

    bool CTSE_LogMsg::ReadLog(std::string& strRead)
    {
        if (!mqueueLogMsg.empty())
        {
            mMutex.lock();
            strRead = mqueueLogMsg.front();
            mqueueLogMsg.pop();
            mMutex.unlock();
            return true;
        }
        return false;
    }

    void CTSE_LogMsg::PushLog(const std::string& strLog)
    {
        mMutex.lock();
        mqueueLogMsg.push(strLog);
        mMutex.unlock();
    }

}
