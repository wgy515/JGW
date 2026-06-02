#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <map>
namespace JGW
{
    class CTSE_StreamingDownloadUserPartitions : public CTSE_TestBase
    {
    public:
        CTSE_StreamingDownloadUserPartitions(void);
        ~CTSE_StreamingDownloadUserPartitions(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbOverridePrtnTable;
        bool mbSkipGoToDownload;
        int mnNumberOfPartitionFiles;
        std::wstring mstrPartitionTableFileName;
        std::wstring mstrDownloadFolderPathEnv;
        std::wstring mstrResourceContextEnv;
        std::map<std::string,std::string> mmapParameters;
    };
}


