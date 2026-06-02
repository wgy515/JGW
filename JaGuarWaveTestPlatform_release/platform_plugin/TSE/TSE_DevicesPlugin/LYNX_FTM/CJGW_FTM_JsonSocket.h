#pragma once
#include "CJGW_FTM_ClientSocket.h"
#include "CJGW_FTM_Json.h"

namespace JGW
{
    class CCJGW_FTM_JsonSocket
    {
    public:
        static bool ReadJsonSocketToWriteSocket(const std::string& strWriteJson,size_t nReqCode,CCJGW_FTM_Json& ftmJson,size_t timeOutSec = 10,int retryCount = 3);
        static bool WriteJsonSocket(const std::string& strWriteJson,size_t timeOutSec = 10,int retryCount  = 3 );
    };
}