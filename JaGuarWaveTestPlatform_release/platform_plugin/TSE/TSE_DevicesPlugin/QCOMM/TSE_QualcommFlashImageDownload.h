#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_QualcommFlashImageDownload:public CTSE_TestBase
    {
    public:
        CTSE_QualcommFlashImageDownload(void);
        ~CTSE_QualcommFlashImageDownload(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Init(void);

        bool TSE_Run(void);

        bool TSE_Exit(void);
    private:
        void TSE_InitDownloadParam();
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
    };
}