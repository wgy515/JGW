// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "IPCGlobalVariable.h"
#include <vector>
#include <memory>
#include "CJGW_IPCRingBuffer.h"
#include <map>
#include "CJGW_ProcessSimplexCommunication.h"
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
#include <unordered_map>
//std::map<std::wstring,std::shared_ptr<JGW::CCJGW_IPCRingBuffer>> gmapIPCSimplexNameRingBuffer;

BOOL APIENTRY DllMain( HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

JGW::CCJGW_IPCGlobalVariable gIPCGlobalVariable;

JGW::IPCGlobalVariableInterface* GetIPCGlobalVariable()
{
    return &gIPCGlobalVariable;
}

unsigned int GetIPCGlobalVariableValue(const wchar_t* strGlobalVariableName,wchar_t* strGlobalVariableValue,unsigned int nGlobalVariableValueLen)
{
    std::wstring strGlobalVariableValueTemp = gIPCGlobalVariable.GetGlobalVariableValue(strGlobalVariableName);
    wcscpy_s(strGlobalVariableValue,nGlobalVariableValueLen,strGlobalVariableValueTemp.c_str());
    return strGlobalVariableValueTemp.length();
}

void PutIPCGlobalVariableNameValue(const wchar_t* strGlobalVariableName,const wchar_t* strGlobalVariableValue)
{
    gIPCGlobalVariable.PutGlobalVariableNameValue(strGlobalVariableName,strGlobalVariableValue);
}

bool ExistsIPCGlobalVariableName(const wchar_t* strGlobalVariableName)
{
    return gIPCGlobalVariable.ExistsGlobalVariableName(strGlobalVariableName);
}

void* CreateIPCSimplex(bool bIsTx,const wchar_t* strIPCSimplexName,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen)
{
#if 0
    std::map<std::wstring,std::shared_ptr<JGW::CCJGW_IPCRingBuffer>>::iterator it = gmapIPCSimplexNameRingBuffer.find(strIPCSimplexName);
    if (it == gmapIPCSimplexNameRingBuffer.end())
    {
        std::shared_ptr<JGW::CCJGW_IPCRingBuffer> ptrIPCRingBuffer(new JGW::CCJGW_IPCRingBuffer);
        if (!ptrIPCRingBuffer->CreateIPCRingBuffer(bIsTx,strIPCSimplexName))
        {
            return NULL;
        }
        gmapIPCSimplexNameRingBuffer.insert(std::make_pair<std::wstring,std::shared_ptr<JGW::CCJGW_IPCRingBuffer>>(strIPCSimplexName,ptrIPCRingBuffer));
        return ptrIPCRingBuffer.get();
    }
    return (void*)it->second.get();
#else
    JGW::CCJGW_IPCRingBuffer* pIPCRingBuffer = new JGW::CCJGW_IPCRingBuffer();
    if (!pIPCRingBuffer->CreateIPCRingBuffer(bIsTx,strIPCSimplexName,szMessageSeparator,nMessageSeparatorLen))
    {
        delete pIPCRingBuffer;
        return NULL;
    }
    return pIPCRingBuffer;
#endif
}

void TransmitIPCSimplexMessage(void* pICPSimplexContext,const char* strMessage)
{
    //JGW::CCJGW_IPCRingBuffer* pIPCRingBuffer = (JGW::CCJGW_IPCRingBuffer*)pICPSimplexContext;
    ((JGW::CCJGW_IPCRingBuffer*)pICPSimplexContext)->PutStringMessage(strMessage);
}

unsigned int ReceiveIPCSimplexMessage(void* pICPSimplexContext,char* pstrReceiveMessage,unsigned int nReceiveMessageLen)
{
    //JGW::CCJGW_IPCRingBuffer* pIPCRingBuffer = (JGW::CCJGW_IPCRingBuffer*)pICPSimplexContext;
    return ((JGW::CCJGW_IPCRingBuffer*)pICPSimplexContext)->PopStringMessage(pstrReceiveMessage,nReceiveMessageLen);
}

void ReleaseIPCSimplex(void* pICPSimplexContext)
{
    delete (JGW::CCJGW_IPCRingBuffer*)pICPSimplexContext;
}

std::map<std::wstring,std::shared_ptr<JGW::S_RING_BUFFER_INFO>> gmapNameRingBufferInfo;
JGW::CCJGW_CriticalSectionLock gCriticalSectionLock;

void* CreateProcessSimplexCommunication(bool bIsTx,unsigned int nProcessSimplexBufferLen,const wchar_t* strProcessSimplexName,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen)
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
    std::map<std::wstring,std::shared_ptr<JGW::S_RING_BUFFER_INFO>>::iterator it = gmapNameRingBufferInfo.find(strProcessSimplexName);
    if (it == gmapNameRingBufferInfo.end())
    {
        //! 
        std::shared_ptr<JGW::S_RING_BUFFER_INFO> ptrRingBufferInfo(new JGW::S_RING_BUFFER_INFO);
        ptrRingBufferInfo->mnRxReferenceCount = 0;
        ptrRingBufferInfo->mnTxReferenceCount = 0;
        ptrRingBufferInfo->mpRingBuffer = NULL;
        ptrRingBufferInfo->msRingBuffer_t.rpos = 0;
        ptrRingBufferInfo->msRingBuffer_t.size = 0;
        ptrRingBufferInfo->msRingBuffer_t.wpos = 0;
        if (bIsTx) ptrRingBufferInfo->mnTxReferenceCount++;
        else ptrRingBufferInfo->mnRxReferenceCount++;
        gmapNameRingBufferInfo.insert(std::make_pair<std::wstring,std::shared_ptr<JGW::S_RING_BUFFER_INFO>>(strProcessSimplexName,ptrRingBufferInfo));


        JGW::CCJGW_ProcessSimplexCommunication* pProcessSimplexCommunication = new JGW::CCJGW_ProcessSimplexCommunication(ptrRingBufferInfo);
        pProcessSimplexCommunication->CreateCommunication(bIsTx,&ptrRingBufferInfo->msRingBuffer_t,ptrRingBufferInfo->mpRingBuffer,nProcessSimplexBufferLen,szMessageSeparator,nMessageSeparatorLen);
        return pProcessSimplexCommunication;
    }
    else
    {
        if (bIsTx)
        {
            if (it->second->mnTxReferenceCount > 0) return NULL;

            JGW::CCJGW_ProcessSimplexCommunication* pProcessSimplexCommunication = new JGW::CCJGW_ProcessSimplexCommunication(it->second);
            it->second->mnTxReferenceCount++;
            pProcessSimplexCommunication->CreateCommunication(bIsTx,&it->second->msRingBuffer_t,it->second->mpRingBuffer,nProcessSimplexBufferLen,szMessageSeparator,nMessageSeparatorLen);
            return pProcessSimplexCommunication;
        }
        else
        {
            if (it->second->mnRxReferenceCount > 0) return NULL;

            JGW::CCJGW_ProcessSimplexCommunication* pProcessSimplexCommunication = new JGW::CCJGW_ProcessSimplexCommunication(it->second);
            it->second->mnRxReferenceCount++;
            pProcessSimplexCommunication->CreateCommunication(bIsTx,&it->second->msRingBuffer_t,it->second->mpRingBuffer,nProcessSimplexBufferLen,szMessageSeparator,nMessageSeparatorLen);
            return pProcessSimplexCommunication;
        }
    }
    return NULL;
}

void TransmitProcessSimplexMessage(void* pProcessSimplexContext,const char* strMessage)
{
    ((JGW::CCJGW_ProcessSimplexCommunication*)pProcessSimplexContext)->TransmitMessage(strMessage);
}

unsigned int ReceiveProcessSimplexMessage(void* pProcessSimplexContext,char* pstrReceiveMessage,unsigned int nReceiveMessageLen)
{
    return ((JGW::CCJGW_ProcessSimplexCommunication*)pProcessSimplexContext)->ReceiveMessage(pstrReceiveMessage,nReceiveMessageLen);
}

void ReleaseProcessSimplex(void* pProcessSimplexContext)
{
    delete (JGW::CCJGW_ProcessSimplexCommunication*)pProcessSimplexContext;
}

void ReleaseProcessSimplexRingBuffer(const wchar_t* strProcessSimplexName)
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
    std::map<std::wstring,std::shared_ptr<JGW::S_RING_BUFFER_INFO>>::iterator it = gmapNameRingBufferInfo.find(strProcessSimplexName);
    if (0 == it->second->mnRxReferenceCount && 0 == it->second->mnTxReferenceCount)
    {
        gmapNameRingBufferInfo.erase(it);
    }
}

std::unordered_map<std::wstring,std::wstring> gmapGlobalVariableNameValue;
JGW::CCJGW_CriticalSectionLock gGlobalVariableCriticalSectionLock;

unsigned int GetProcessGlobalVariableValue(const wchar_t* strGlobalVariableName,wchar_t* strGlobalVariableValue,unsigned int nGlobalVariableValueLen)
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(gGlobalVariableCriticalSectionLock);

    std::unordered_map<std::wstring,std::wstring>::iterator it = gmapGlobalVariableNameValue.find(strGlobalVariableName);
    if (it == gmapGlobalVariableNameValue.end())
    {
        return 0;
    }
    wcscpy_s(strGlobalVariableValue,nGlobalVariableValueLen,it->second.c_str());

    return it->second.length();
}

void PutProcessGlobalVariableNameValue(const wchar_t* strGlobalVariableName,const wchar_t* strGlobalVariableValue)
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(gGlobalVariableCriticalSectionLock);
    gmapGlobalVariableNameValue[strGlobalVariableName] = strGlobalVariableValue;
}

bool ExistsProcessGlobalVariableName(const wchar_t* strGlobalVariableName)
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(gGlobalVariableCriticalSectionLock);
    return gmapGlobalVariableNameValue.end() != gmapGlobalVariableNameValue.find(strGlobalVariableName);
}





















#if 0
class ProcessSimplexRingBufferInfo
{
public:
    JGW::ringbuffer_t msRingBuffer_t;
    char* mpRingBuffer;


    ProcessSimplexRingBufferInfo() : mpRingBuffer(NULL),mnMessageSeparatorLen(2)
    {
        memset(mszMessageSeparator,0x00,RING_BUFFER_SEQARATOR_LENGTH);
        mszMessageSeparator[0] = '\r';
        mszMessageSeparator[1] = '\n';
    }

    ~ProcessSimplexRingBufferInfo()
    {
        if (NULL != mpRingBuffer)
        {
            delete[] mpRingBuffer;
            mpRingBuffer = NULL;
        }
    }
};
#endif

#if 0
std::map<std::wstring,std::shared_ptr<ProcessSimplexRingBufferInfo>> gmapNameProcessSimplexCommunication;

void* CreateProcessSimplexCommunication(bool bIsTx,unsigned int nProcessSimplexBufferLen,const wchar_t* strProcessSimplexName,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen)
{
    JGW::CCJGW_RingBuffer *pRingBuffer = new JGW::CCJGW_RingBuffer();
    std::map<std::wstring,std::shared_ptr<ProcessSimplexRingBufferInfo>>::iterator it = gmapNameProcessSimplexCommunication.find(std::wstring(strProcessSimplexName));
    if (it == gmapNameProcessSimplexCommunication.end())
    {
        std::shared_ptr<ProcessSimplexRingBufferInfo> ptrProcessSimplexCommunication(new ProcessSimplexRingBufferInfo);

        pRingBuffer->WriteRingBuffer(szMessageSeparator)
            gmapNameProcessSimplexCommunication.insert(std::make_pair<std::wstring,std::shared_ptr<JGW::CCJGW_ProcessSimplexCommunication>>(strProcessSimplexName,ptrProcessSimplexCommunication));
        return ptrProcessSimplexCommunication.get();
    }
    return pRingBuffer;
}

void TransmitProcessSimplexMessage(void* pProcessSimplexContext,const char* strMessage)
{
    ((JGW::CCJGW_ProcessSimplexCommunication*)pProcessSimplexContext)->TransmitMessage(strMessage);
}

unsigned int ReceiveProcessSimplexMessage(void* pProcessSimplexContext,char* pstrReceiveMessage,unsigned int nReceiveMessageLen)
{
    return ((JGW::CCJGW_ProcessSimplexCommunication*)pProcessSimplexContext)->ReceiveMessage(pstrReceiveMessage,nReceiveMessageLen);
}
#endif
//void ReleaseProcessSimplex(void* pProcessSimplexContext)
//{
//    for (std::map<std::wstring,std::shared_ptr<JGW::CCJGW_ProcessSimplexCommunication>>::iterator it = gmapNameProcessSimplexCommunication.begin();it != gmapNameProcessSimplexCommunication.end();)
//    {
//        if (pProcessSimplexContext == (void*)it->second.get())
//        {
//            it = gmapNameProcessSimplexCommunication.erase(it);
//        }
//        else
//        {
//            ++it;
//        }
//    }
//}





