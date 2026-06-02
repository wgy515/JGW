#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>

namespace JGW
{
    class CTSE_QualcommFlashImageDownloadV2 : public CTSE_TestBase
    {
    public:
        CTSE_QualcommFlashImageDownloadV2(void);
        ~CTSE_QualcommFlashImageDownloadV2(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Init(void);

        bool TSE_Run(void);
    private:
        bool mbIgnore;
        //! 命令发送或者接收超时时间
        int mnTimeOut;
        //! 发送给Fh_Loader_Cmd
        std::wstring mDesFh_Loader_Cmd;
        //! 原来的命令，处理后的命令
        std::wstring mstrSrcCmd;
        //! 
        std::wstring mstrCommandOk;

        CTSE_CheckTestResult mCheckTestResult;
    };
}

