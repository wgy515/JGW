#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_FTPUpdateFile : public CTSE_TestBase
    {
    public:
        CTSE_FTPUpdateFile(void);
        ~CTSE_FTPUpdateFile(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSNEnvironment;
        std::wstring mstrFTPListName;
        std::wstring mstrOperateID;
        std::wstring mstrComputeName;
        std::wstring mstrFTPUploadDirEnvironment; //! FTP上传目录
        std::wstring mstrUploadFileEnvironment; //! 上传文件环境变量
    };
}