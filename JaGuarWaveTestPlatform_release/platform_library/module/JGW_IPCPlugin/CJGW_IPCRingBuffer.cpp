#include "StdAfx.h"
#include "CJGW_IPCRingBuffer.h"

#define MEM_MAP_FILE_NAME_ROOT_RING_BUFFER_STRUCT L"Local\\3F847405-0AD3-4DB0-8CB0-63425827C60E-"
#define MEM_MAP_FILE_NAME_ROOT_RING_BUFFER L"Local\\3F847405-0AD3-4DB0-8CB0-63425827C60A-"
#define MAX_IPC_MESSAGE_LENGTH (10 * 1024)
namespace JGW
{
    CCJGW_IPCRingBuffer::CCJGW_IPCRingBuffer() : mbWrite(false),mnMessageSeparatorLen(2)
    {
        mpData = (char*)malloc(MAX_IPC_MESSAGE_LENGTH + 1);
        memset(mpData,0x00,MAX_IPC_MESSAGE_LENGTH + 1);
        mszMessageSeparator[0] = '\r';
        mszMessageSeparator[1] = '\n';
    }


    CCJGW_IPCRingBuffer::~CCJGW_IPCRingBuffer(void)
    {
        free(mpData);
    }

    bool CCJGW_IPCRingBuffer::CreateIPCRingBuffer(bool bWrite,const std::wstring& strIPCName,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen)
    {
        std::wstring strIPCRingBufferName(MEM_MAP_FILE_NAME_ROOT_RING_BUFFER),strIPCRingBufferStructName(MEM_MAP_FILE_NAME_ROOT_RING_BUFFER_STRUCT);

        mbWrite = bWrite;
        strIPCRingBufferStructName += strIPCName;
        strIPCRingBufferName += strIPCName;

        mnMessageSeparatorLen = nMessageSeparatorLen > RING_BUFFER_SEQARATOR_LENGTH ? RING_BUFFER_SEQARATOR_LENGTH : nMessageSeparatorLen;
        memcpy_s(mszMessageSeparator,RING_BUFFER_SEQARATOR_LENGTH,szMessageSeparator,mnMessageSeparatorLen);

        if (mbWrite)
        {
            //! 创建环形缓冲区共享内存BUFFER
            if (!mcRingBufferSharedMemory.CreateSharedMemory(strIPCRingBufferName,1024*1024))
            {
                return false;
            }
            //! 创建环形缓冲区共享内存结构体
            if (!mcRingBufferStructSharedMemory.CreateSharedMemory(strIPCRingBufferStructName,1024))
            {
                return false;
            }
            //! 将共享内存BUFFER以及共享内存结构体构造到环形缓冲区
            if (!mcRingBuffer.CreateRingBuffer((ringbuffer_t*)mcRingBufferStructSharedMemory.GetSharedMemoryBuffer(),mcRingBufferSharedMemory.GetSharedMemoryBuffer(),1024 * 1024))
            {
                return false;
            }
        }
        else
        {
            //! 打开现有环形缓冲区共享内存BUFFER
            if (!mcRingBufferSharedMemory.OpenSharedMemory(strIPCRingBufferName))
            {
                return false;
            }
            //! 打开环形缓冲区共享内存结构体
            if (!mcRingBufferStructSharedMemory.OpenSharedMemory(strIPCRingBufferStructName))
            {
                return false;
            }
            //! 共享内存结构体以及共享内存BUFFER构建环形缓冲区
            ringbuffer_t* pRingBuffer_t = (ringbuffer_t*)mcRingBufferStructSharedMemory.GetSharedMemoryBuffer();
            mcRingBuffer.CreateRingBuffer(pRingBuffer_t,mcRingBufferSharedMemory.GetSharedMemoryBuffer());
        }

        return true;
    }

    void CCJGW_IPCRingBuffer::PutStringMessage(const std::string& strMessage)
    {
        if (mbWrite) mcRingBuffer.WriteRingBuffer((unsigned char*)strMessage.c_str(),strMessage.length());
    }

    std::string CCJGW_IPCRingBuffer::PopStringMessage()
    {
        size_t len = mcRingBuffer.SearchRingBuffer((unsigned char*)"\r\n",2);
        if (len > 0)
        {
            len = len > MAX_IPC_MESSAGE_LENGTH ? MAX_IPC_MESSAGE_LENGTH : len;
            memset(mpData,0x00,MAX_IPC_MESSAGE_LENGTH);
            len = mcRingBuffer.ReadRingBuffer((unsigned char*)mpData,len);
            if (len > 0) mcRingBuffer.DrainRingBuffer(len);
            return mpData;
        }
        return "";
    }

    unsigned int CCJGW_IPCRingBuffer::PopStringMessage(char* strReceiveMessage,unsigned int nReceiveMessageLen)
    {
        size_t len = mcRingBuffer.SearchRingBuffer(mszMessageSeparator,mnMessageSeparatorLen);
        if (len > 0)
        {
            len = len > nReceiveMessageLen ? nReceiveMessageLen : len;
            memset(strReceiveMessage,0x00,nReceiveMessageLen);
            len = mcRingBuffer.ReadRingBuffer((unsigned char*)strReceiveMessage,len);
            if (len > 0) mcRingBuffer.DrainRingBuffer(len);
            return len;
        }
        return 0;
    }
}
