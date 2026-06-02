#pragma once
#include "CJGW_RingBuffer.h"
#include <string>
#include "CJGW_SharedMemory.h"

namespace JGW
{
    //! 进程间通信(Inter Process Communication)
    class CCJGW_IPCRingBuffer
    {
    public:
        CCJGW_IPCRingBuffer();
        ~CCJGW_IPCRingBuffer(void);
    public:
        /// @brief 创建IPC环形缓冲区
        /// @param bWrite 是否是写入
        /// @param strIPCName 共享内存名称
        /// @return true : 成功 false : 失败
        bool CreateIPCRingBuffer(bool bWrite,const std::wstring& strIPCName,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen);
    public:
        /// @brief 写入消息到IPC环形缓冲区
        /// @param strMessage 消息
        void PutStringMessage(const std::string& strMessage);
        /// @brief 获取IPC环形缓冲区消息
        std::string PopStringMessage();
        /// @brief 获取IPC环形缓冲区消息
        unsigned int PopStringMessage(char* strReceiveMessage,unsigned int nReceiveMessageLen);
    private:
        //! 是否是写入缓冲区，只能当单边缓冲区
        bool mbWrite;
        //! 读取消息缓冲区
        char* mpData;
        unsigned int mnMessageSeparatorLen;
        unsigned char mszMessageSeparator[RING_BUFFER_SEQARATOR_LENGTH];
        CCJGW_SharedMemory mcRingBufferSharedMemory;
        CCJGW_SharedMemory mcRingBufferStructSharedMemory;
        CCJGW_RingBuffer mcRingBuffer;
    };
}
