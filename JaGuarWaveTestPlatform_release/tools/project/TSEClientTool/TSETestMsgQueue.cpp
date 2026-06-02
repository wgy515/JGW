#include "StdAfx.h"
#include "TSETestMsgQueue.h"

namespace JGW
{
    CTSETestMsgQueue::CTSETestMsgQueue(void) : mp_current_msg_queue(new std::queue<std::string>)
        ,mp_execute_msg_queue(new std::queue<std::string>)
    {

    }

    CTSETestMsgQueue::~CTSETestMsgQueue(void)
    {
        if (mp_execute_msg_queue) delete mp_execute_msg_queue;
        if (mp_current_msg_queue) delete mp_current_msg_queue;
    }

    void CTSETestMsgQueue::PutAsyncMessage(const std::string& strLog)
    {
        JGW::CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
        mp_current_msg_queue->push(strLog);
    }

    std::queue<std::string>* CTSETestMsgQueue::GetExecuteQueue()
    {
        SwapQueue();
        return mp_execute_msg_queue;
    }

    void CTSETestMsgQueue::SwapQueue()
    {
        JGW::CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
        std::queue<std::string>* p_queue = mp_current_msg_queue;
        mp_current_msg_queue = mp_execute_msg_queue;
        mp_execute_msg_queue = p_queue;
    }
}

