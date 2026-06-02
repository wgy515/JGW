#pragma once
#include "CJGW_RingBuffer.h"
#include <memory>
namespace JGW
{
    typedef struct  
    {
        //! TX通讯引用计数，只能引用一次
        volatile int mnTxReferenceCount;
        //! RX通讯引用计数，只能引用一次
        volatile int mnRxReferenceCount;
        //! TRX共用环形缓冲区
        char* mpRingBuffer;
        //! 环形缓冲区结构体
        JGW::ringbuffer_t msRingBuffer_t;
    }S_RING_BUFFER_INFO;

    class CCJGW_ProcessSimplexCommunication
    {
    public:
        CCJGW_ProcessSimplexCommunication(std::shared_ptr<S_RING_BUFFER_INFO> ptrsRingBuferInfo);
        ~CCJGW_ProcessSimplexCommunication(void);
    public:
        bool CreateCommunication(bool bIsTx,ringbuffer_t* pRingBuffer_t,void* pBuffer,unsigned int nBufferLen,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen);
    public:
        /// @brief 写入消息到IPC环形缓冲区
        /// @param strMessage 消息
        void TransmitMessage(const std::string& strMessage);
        /// @brief 获取IPC环形缓冲区消息
        std::string ReceiveMessage();
        /// @brief 获取IPC环形缓冲区消息
        unsigned int ReceiveMessage(char* strReceiveMessage,unsigned int nReceiveMessageLen);
    public:
        bool mbIsTransmit;
        char* mpData;
        unsigned int mnMessageSeparatorLen;
        unsigned char mszMessageSeparator[RING_BUFFER_SEQARATOR_LENGTH];
        std::shared_ptr<S_RING_BUFFER_INFO> mptrsRingBuferInfo;
        CCJGW_RingBuffer mcRingBuffer;
    };
}

