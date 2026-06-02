#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>
//! Log保存方式 当前程序目录下TestLog目录->测试名称->时间日期目录->SN->
namespace JGW
{
    class CTSE_QualcommQCN : public CTSE_TestBase
    {
    public:
        CTSE_QualcommQCN(void);
        ~CTSE_QualcommQCN(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run(void);
    private:
        bool BuildQCNPath();
        bool BackupQCN();
        bool RestoreQCN();
    private:
        bool mbIsReportLog;
        bool mbIsBackupQCN;
        std::string mstrQCNPath;
        std::string mstrSPC;
        std::wstring mstrSNEnvironment;
        std::wstring mstrQCNEnvironment;
    };
}