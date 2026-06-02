#include "StdAfx.h"
#include "CJGW_PackageConfigCommonFunc.h"

#include <fstream>
#include <poco/Zip/Compress.h>
#include <poco/Zip/ZipArchive.h>
#include <poco/Zip/Decompress.h>
#include <poco/Zip/ZipLocalFileHeader.h>
#include <poco/Delegate.h>

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_CurlPlugin/CJGW_CurlFtp.h>

namespace JGW
{
    DWORD _errCnt = 0;
    CCJGW_PackageConfigCommonFunc::CCJGW_PackageConfigCommonFunc(void)
    {
    }


    CCJGW_PackageConfigCommonFunc::~CCJGW_PackageConfigCommonFunc(void)
    {
    }

    bool CCJGW_PackageConfigCommonFunc::CompressFile(const std::wstring& strCompressDirectory,const std::wstring& strCompressFilePath,std::wstring& strErrorMsg)
    {
        std::string strCompressDirectoryA = JGW_W2A(strCompressDirectory,CP_UTF8);
        std::string strCompressFilePathA = JGW_W2A(strCompressFilePath);
        strErrorMsg = L"";

        try
        {
            if (!JGW_PathFileExists(JGW_GetPathOfFile(strCompressFilePath.c_str()).c_str()))
            {
                JGW_CreateDirectory(strCompressFilePath.c_str(),false);
            }
            std::ofstream out(strCompressFilePathA, std::ios::binary);
            Poco::Zip::Compress c(out, true);

            Poco::Path theFile(strCompressDirectoryA);
            theFile.makeDirectory();
            c.addRecursive(theFile,Poco::Zip::ZipCommon::CL_NORMAL);
            Poco::Zip::ZipArchive a(c.close());

            return true;
        }
        catch (Poco::Exception& e)
        {
            strErrorMsg = JGW_A2W(e.what(),CP_UTF8);
        }
        return false;
    }

    void OnDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
    {
        ++_errCnt;
    }

    void OnDecompressOkCallback(const void* pSender,std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info)
    {
    }

    bool CCJGW_PackageConfigCommonFunc::DeCompressFile(const std::wstring& strDeCompressDirectory,const std::wstring& strDeCompressFilePath,std::wstring& strErrorMsg)
    {
        //！解压Config文件
        JGW_CreateDirectory(strDeCompressDirectory.c_str(),true);
        std::string strDeCompressFilePathA = JGW_W2A(strDeCompressFilePath);
        std::string strDeCompressDirectoryA = JGW_W2A(strDeCompressDirectory,CP_UTF8);

        _errCnt = 0;
        strErrorMsg = L"";

        try
        {
            std::ifstream inp(strDeCompressFilePathA, std::ios::binary);
            Poco::Zip::Decompress dec(inp,Poco::Path(strDeCompressDirectoryA));
            dec.EError += Poco::delegate<std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(OnDecompressError);
            dec.EOk += Poco::delegate<std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>>(OnDecompressOkCallback);
            dec.decompressAllFiles();
            dec.EError -= Poco::delegate<std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(OnDecompressError);
            dec.EOk -= Poco::delegate<std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>>(OnDecompressOkCallback);
            if (_errCnt != 0)
            {
                JGW_FormatWString(strErrorMsg,L"Failed to decompress the file (%d)",_errCnt);
                return false;
            }
        }
        catch (Poco::Exception& e)
        {
            strErrorMsg = JGW_A2W(e.what(),CP_UTF8);
        }
        
        return true;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetConfigVersion()
    {
        std::wstring strConfigVersion;
        JGW_FormatWString(strConfigVersion,L"Config%s",JGW_GetCurrentTimeStampYMDHMS().c_str());
        return strConfigVersion;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetLocalConfigFolder(const std::wstring& strTaskNumber,const std::wstring& strConfigVersion)
    {
        std::wstring strLocalConfigFolder;
        JGW_FormatWString(strLocalConfigFolder,L"%sPackageConfig\\%s\\%s\\",JGW_GetApplicationFolder(),strTaskNumber.c_str(),strConfigVersion.c_str());
        return strLocalConfigFolder;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetPackageConfigZipPath(const std::wstring& strTaskNumber,const std::wstring& strConfigVersion)
    {
        std::wstring strPackageConfigZipPath;
        JGW_FormatWString(strPackageConfigZipPath,L"%stmp\\%s\\%s.zip",JGW_GetApplicationFolder(),strTaskNumber.c_str(),strConfigVersion.c_str());
        return strPackageConfigZipPath;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetFtpConfigPackagePath(const std::wstring& strFtpRootPath,const std::wstring& strProjectName,const std::wstring& strConfigVersion)
    {
        std::wstring strFtpConfigPackagePath;
        JGW_FormatWString(strFtpConfigPackagePath,L"%sProject/%s/%s.zip",strFtpRootPath.c_str(),strProjectName.c_str(),strConfigVersion.c_str());
        return strFtpConfigPackagePath;
    }

    bool CCJGW_PackageConfigCommonFunc::UploadFTPFile(LPS_PACKAGE_UPDATE_CONFIG psFtpParam,const std::wstring& strLocalFilePath,const std::wstring& strRemoteFtpPath,std::wstring& strErrorMsg)
    {
        CCJGW_CurlFtp curlFtpDownload(psFtpParam->mstrFTPHost,psFtpParam->mstrUserName,psFtpParam->mstrUserPassword,psFtpParam->mnFTPPort);
        if (!curlFtpDownload.UploadCurlFtpFile(strRemoteFtpPath,strLocalFilePath,strErrorMsg))
        {
            std::wstring strRemoteFtpFolder = JGW_GetFileFolderToFilePath(strRemoteFtpPath.c_str());
            if (!curlFtpDownload.CreateDirectoryCurlFtpFile(strRemoteFtpFolder,strErrorMsg))
            {
                curlFtpDownload.TraverseCreateFTPDirectory(strRemoteFtpFolder,strErrorMsg);
            }
            //! 重新上传FTP文件 并且上传完成后删除FTP文件
            if (curlFtpDownload.UploadCurlFtpFile(strRemoteFtpPath,strLocalFilePath,strErrorMsg))
            {
                return true;
            }
        }
        else
        {
            return true;
        }
        return false;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetOTAVersion()
    {
        std::wstring strConfigVersion;
        JGW_FormatWString(strConfigVersion,L"Version%s",JGW_GetCurrentTimeStampYMDHMS().c_str());
        return strConfigVersion;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetLocalOTAFolder(const std::wstring& strInstallVersion,const std::wstring& strOTAVersion)
    {
        std::wstring strLocalOTAFolder;
        JGW_FormatWString(strLocalOTAFolder,L"%sPackageOTA\\%s\\%s\\",JGW_GetApplicationFolder(),strInstallVersion.c_str(),strOTAVersion.c_str());
        return strLocalOTAFolder;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetPackageOTAZipPath(const std::wstring& strInstallVersion,const std::wstring& strOTAVersion)
    {
        std::wstring strLocalOTAZipPath;
        //! \exec_debug\tmp\InstallVersion\version.zip
        JGW_FormatWString(strLocalOTAZipPath,L"%stmp\\%s\\%s.zip",JGW_GetApplicationFolder(),strInstallVersion.c_str(),strOTAVersion.c_str());
        return strLocalOTAZipPath;
    }

    std::wstring CCJGW_PackageConfigCommonFunc::GetFtpOTAPackagePath(const std::wstring& strFtpRootPath,const std::wstring& strInstallVersion,const std::wstring& strOTAVersion)
    {
        std::wstring strFtpOTAPackagePath;
        JGW_FormatWString(strFtpOTAPackagePath,L"%sUpdateVersion/%s/%s.zip",strFtpRootPath.c_str(),strInstallVersion.c_str(),strOTAVersion.c_str());
        return strFtpOTAPackagePath;
    }
}

