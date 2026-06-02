#include "StdAfx.h"
#include "CJGW_ProcessSimplexCommunication.h"

#define MAX_MESSAGE_LENGTH (10 * 1024)
namespace JGW
{
    CCJGW_ProcessSimplexCommunication::CCJGW_ProcessSimplexCommunication(std::shared_ptr<S_RING_BUFFER_INFO> ptrsRingBuferInfo) : mnMessageSeparatorLen(2),mbIsTransmit(true),mptrsRingBuferInfo(ptrsRingBuferInfo)
    {
        mszMessageSeparator[0] = '\r';
        mszMessageSeparator[1] = '\n';
        mpData = (char*)malloc(MAX_MESSAGE_LENGTH + 1);
    }


    CCJGW_ProcessSimplexCommunication::~CCJGW_ProcessSimplexCommunication(void)
    {
        free(mpData);
        if (mbIsTransmit) mptrsRingBuferInfo->mnTxReferenceCount--;
        else mptrsRingBuferInfo->mnRxReferenceCount--;
    }

    bool CCJGW_ProcessSimplexCommunication::CreateCommunication(bool bIsTx,ringbuffer_t* pRingBuffer_t,void* pBuffer,unsigned int nBufferLen,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen)
    {
        mbIsTransmit = bIsTx;
        mnMessageSeparatorLen = nMessageSeparatorLen > RING_BUFFER_SEQARATOR_LENGTH ? RING_BUFFER_SEQARATOR_LENGTH : nMessageSeparatorLen;
        memcpy_s(mszMessageSeparator,RING_BUFFER_SEQARATOR_LENGTH,szMessageSeparator,mnMessageSeparatorLen);
        return mcRingBuffer.CreateRingBuffer(pRingBuffer_t,pBuffer,nBufferLen);
    }

    void CCJGW_ProcessSimplexCommunication::TransmitMessage(const std::string& strMessage)
    {
        if (mbIsTransmit) mcRingBuffer.WriteRingBuffer((unsigned char*)strMessage.c_str(),strMessage.length());
    }

    std::string CCJGW_ProcessSimplexCommunication::ReceiveMessage()
    {
        if (mbIsTransmit) return "";

        size_t len = mcRingBuffer.SearchRingBuffer((unsigned char*)"\r\n",2);
        if (len > 0)
        {
            len = len > MAX_MESSAGE_LENGTH ? MAX_MESSAGE_LENGTH : len;
            memset(mpData,0x00,MAX_MESSAGE_LENGTH);
            len = mcRingBuffer.ReadRingBuffer((unsigned char*)mpData,len);
            if (len > 0) mcRingBuffer.DrainRingBuffer(len);
            return mpData;
        }
        return "";
    }

    unsigned int CCJGW_ProcessSimplexCommunication::ReceiveMessage(char* strReceiveMessage,unsigned int nReceiveMessageLen)
    {
        if (mbIsTransmit) return 0;

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
