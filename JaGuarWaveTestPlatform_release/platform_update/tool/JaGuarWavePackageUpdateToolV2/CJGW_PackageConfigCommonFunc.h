#pragma once
#include <string>
#include "JGWPackageUpdateDefine.h"
namespace JGW
{
    class CCJGW_PackageConfigCommonFunc
    {
    public:
        CCJGW_PackageConfigCommonFunc(void);
        ~CCJGW_PackageConfigCommonFunc(void);
    public:
        //! 压缩文件 strCompressDirectory : 压缩目录 strCompressFilePath ： 压缩文件路径
        static bool CompressFile(const std::wstring& strCompressDirectory,const std::wstring& strCompressFilePath,std::wstring& strErrorMsg);
        //! 解压文件
        static bool DeCompressFile(const std::wstring& strDeCompressDirectory,const std::wstring& strDeCompressFilePath,std::wstring& strErrorMsg);
        //! 上传FTP文件
        static bool UploadFTPFile(LPS_PACKAGE_UPDATE_CONFIG psFtpParam,const std::wstring& strLocalFilePath,const std::wstring& strRemoteFtpPath,std::wstring& strErrorMsg);
        //! 下载FTP文件
        //static bool DownloadFTP
    public:
        /*********************规则*********************************************/
        //! 
        static std::wstring GetConfigVersion();
        //!
        static std::wstring GetLocalConfigFolder(const std::wstring& strTaskNumber,const std::wstring& strConfigVersion);
        //! \exec_debug\tmp\DHW222310\version.zip
        static std::wstring GetPackageConfigZipPath(const std::wstring& strTaskNumber,const std::wstring& strConfigVersion);
        //! JGWPTP_Help\Project\项目信息\配置版本.zip
        static std::wstring GetFtpConfigPackagePath(const std::wstring& strFtpRootPath,const std::wstring& strProjectName,const std::wstring& strConfigVersion);
        //!
        static std::wstring GetOTAVersion();
        //! $$exec_folder$$\PackageOTA\InstallVersion\version
        static std::wstring GetLocalOTAFolder(const std::wstring& strInstallVersion,const std::wstring& strOTAVersion);
        //! \exec_debug\tmp\InstallVersion\version.zip
        static std::wstring GetPackageOTAZipPath(const std::wstring& strInstallVersion,const std::wstring& strOTAVersion);
        //! JGWPTP_Help\UpdateVersion\安装版本\配置版本.zip
        static std::wstring GetFtpOTAPackagePath(const std::wstring& strFtpRootPath,const std::wstring& strInstallVersion,const std::wstring& strOTAVersion);
    private:

    };

}

