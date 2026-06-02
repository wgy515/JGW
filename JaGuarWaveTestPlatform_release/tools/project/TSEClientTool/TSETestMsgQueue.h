#pragma once
#include <queue>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>

namespace JGW
{
    class CTSETestMsgQueue
    {
    public:
        CTSETestMsgQueue(void);
        ~CTSETestMsgQueue(void);
    public:
        //! 
        void PutAsyncMessage(const std::string& strLog);
        //! 
        std::queue<std::string>* GetExecuteQueue(void);
    private:
        //! 执行区队列与缓冲区队列交换
        void SwapQueue();
    private:
        //! put 消息队列 当执行execute函数的时候，put队列和给get队列互换
        std::queue<std::string>* mp_current_msg_queue;
        //! get 消息队列
        std::queue<std::string>* mp_execute_msg_queue;
        //! 
        CCJGW_CriticalSectionLock mCriticalSectionLock;
    };
}



