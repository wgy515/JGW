#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_FTM_ConnectSocket : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_ConnectSocket(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run(void);
    private:
        size_t mSocketPort;
        //! true : 则是从配置中获取的IP地址 false: 从全局变量中获取IP地址
        bool mbIsConfigSocketIP;
        std::string mSocketIP;
    };
}

