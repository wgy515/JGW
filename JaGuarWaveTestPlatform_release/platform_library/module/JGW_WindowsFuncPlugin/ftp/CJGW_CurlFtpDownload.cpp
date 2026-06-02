#if 0
#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_CurlFtpDownload.h>
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <vector>
namespace JGW
{
    //单条命令: curl [-options] <ftpUrl> -X "FTP命令"
    //多条命令: curl [-options] <ftpUrl> -Q "FTP命令" -Q "FTP命令"
    //# 
    //# 创建文件夹, 在 /aaDir/ 目录（目录必须以"/"结尾）下创建 bbDir 文件夹
    //#
    //    curl -u "user:passwd" ftp://192.168.0.100/aaDir/ -X "MKD bbDir"

    //# 
    //# 删除文件夹, 删除 /aaDir/ 目录下的 bbDir 文件夹（文件夹必须为空）
    //#
    //curl -u "user:passwd" ftp://192.168.0.100/aaDir/ -X "RMD bbDir"

    //# 
    //# 删除文件, 删除 /aaDir/ 目录下的 aa.txt 文件
    //#
    //curl -u "user:passwd" ftp://192.168.0.100/aaDir/ -X "DELE aa.txt"

    //#
    //# 重命名, 重命名需要连续执行两条命令, 使用两个 -Q 参数连续执行两条命令（必须先 RNFR, 后 RNTO）
    //#
    //curl -u "user:passwd" ftp://192.168.0.100/ -Q "RNFR OldPath" -Q "RNTO NewPath"

    //# 下载 FTP 指定文件 /aaDir/aa.txt, 以原文件名命名保存到当前目录 
    //    curl ftp://192.168.0.100/aaDir/aa.txt -u "user:passwd" -O

    //# 下载 FTP 指定文件 /aaDir/aa.txt, 以 bb.txt 命名保存
    //curl ftp://192.168.0.100/aaDir/aa.txt -u "user:passwd" -o bb.txt


    //# 上传 aa.txt 文件到 FTP 指定目录下（目录必须以"/"结尾）, 并以 原文件名 命名保存
    //curl ftp://192.168.0.100/aaDir/ -u "user:passwd" -T "aa.txt"

    //# 上传 aa.txt 文件到 FTP 指定目录下, 并以 bb.txt 命名保存
    //curl ftp://192.168.0.100/aaDir/bb.txt -u "user:passwd" -T "aa.txt"

    //# 同时上传多个文件
    //curl ftp://192.168.0.100/aaDir/ -u "user:passwd" -T "{aa.txt,bb.txt}"

    //# 查看 FTP 指定目录（目录必须以"/"结尾）下的文件列表 
    //curl ftp://192.168.0.100/aaDir/ -u "user:passwd"

    //# 查看 FTP 指定文件的内容（直接输出到终端） 
    //curl ftp://192.168.0.100/aaDir/aa.txt -u "user:passwd"

    //# 用户名 和 密码 的另一种写法（查看 FTP 服务器指定目录）
    //curl ftp://user:passwd@192.168.0.200/aaDir/

    CCJGW_CurlFtpDownload::CCJGW_CurlFtpDownload(const std::string& host, const std::string& username /* = "" */, const std::string& password /* = "" */,size_t nFtpPort) : mnFtpPort(nFtpPort)
    {
        strcpy_s(mstrHost,128,host.c_str());
        strcpy_s(mstrUserName,128,username.c_str());
        strcpy_s(mstrPassword,128,password.c_str());
    }

    
    CCJGW_CurlFtpDownload::~CCJGW_CurlFtpDownload(void)
    {
    }

    void CCJGW_CurlFtpDownload::UpdateHostUserNamePassword(const std::string& host,const std::string& username /* = "" */,const std::string& password /* = "" */,size_t nFtpPort /* = 22 */)
    {
        strcpy_s(mstrHost,128,host.c_str());
        strcpy_s(mstrUserName,128,username.c_str());
        strcpy_s(mstrPassword,128,password.c_str());
        mnFtpPort = nFtpPort;
    }


    bool CCJGW_CurlFtpDownload::DownloadCurlFTPFile(const std::string& strFTPFilePath,const std::string& strDownloadFileSavePath,size_t mnTimeOut /* = 6 * 60 */)
    {
        std::string strArg;
        JGW_FormatString(strArg," \"ftp://%s:%d%s\" -u \"%s:%s\" -o \"%s\"",mstrHost,mnFtpPort,strFTPFilePath.c_str(),mstrUserName,mstrPassword,strDownloadFileSavePath.c_str());
        return ExecuteCurlCommand(strArg,mnTimeOut);
    }
    //! curl ftp://192.168.0.100/aaDir/ -u "user:passwd" -T "aa.txt"
    bool CCJGW_CurlFtpDownload::UpdateCurlFTPFile(const std::string& strFTPFilePath,const std::string& strUpdateFilePath,size_t mnTimeOut /* = 6 * 60 */)
    {
        std::string strArg;
        JGW_FormatString(strArg," \"ftp://%s:%d%s\" -u \"%s:%s\" -T \"%s\"",mstrHost,mnFtpPort,strFTPFilePath.c_str(),mstrUserName,mstrPassword,strUpdateFilePath.c_str());
        return ExecuteCurlCommand(strArg,mnTimeOut);
    }
    //! curl -u "user:passwd" ftp://192.168.0.100/aaDir/ -X "MKD bbDir"
    bool CCJGW_CurlFtpDownload::CreateCurlFTPDirectory(const std::string& strFTPDir)
    {
        std::string strArg;
        JGW_FormatString(strArg," -u \"%s:%s\" \"ftp://%s:%d/\" -X \"MKD %s\"",mstrUserName,mstrPassword,mstrHost,mnFtpPort,strFTPDir.c_str());
        return ExecuteCurlCommand(strArg);
    }

    bool CCJGW_CurlFtpDownload::ExecuteCurlCommand(const std::string& strArg,std::wstring& strRead,size_t mnTimeOut /* = 3 * 60 */)
    {
        CCJGW_PeekNamePide cPeekNamePide;
        std::wstring strProcessPath = JGW_GetExecConfigFilePath(L"curl.exe");

        if(!cPeekNamePide.SendDosCmds(strProcessPath.c_str(),JGW_A2W(strArg).c_str(),mnTimeOut))
        {
            return false;
        }

        strRead = L"";
        if (!cPeekNamePide.ReadDosCmd(strRead))
        {
            return false;
        }

        return true;
    }

    bool CCJGW_CurlFtpDownload::CheckCurlFTPFileExist(bool& bFileExist,const std::string& strFTPFilePath,const std::string& strDownloadFileSavePath,size_t mnTimeOut /* = 10 * 60 */)
    {
        std::string strArg;
        JGW_FormatString(strArg," \"ftp://%s:%d%s\" -u \"%s:%s\" -o \"%s\"",mstrHost,mnFtpPort,strFTPFilePath.c_str(),mstrUserName,mstrPassword,strDownloadFileSavePath.c_str());

        std::wstring strTemp;
        ExecuteCurlCommand(strArg,strTemp,mnTimeOut);
        if (NULL == JGW_WStrCaseWStr(strTemp.c_str(),L"curl:"))
        {
            bFileExist = true;
            return true;
        }
        if (NULL != JGW_WStrCaseWStr(strTemp.c_str(),L"curl: (9)")
            || NULL != JGW_WStrCaseWStr(strTemp.c_str(),L"curl: (78)"))
        {
            bFileExist = false;
            return true;
        }
        bFileExist = true;
        return false;
    }

    bool CCJGW_CurlFtpDownload::ExecuteCurlCommand(const std::string& strArg,size_t mnTimeOut/* = 10 * 60*/)
    {
        CCJGW_PeekNamePide cPeekNamePide;
        std::wstring strProcessPath = JGW_GetExecConfigFilePath(L"curl.exe");

        if(!cPeekNamePide.SendDosCmds(strProcessPath.c_str(),JGW_A2W(strArg).c_str(),mnTimeOut))
        {
            return false;
        }

        std::wstring strRead;
        if (!cPeekNamePide.ReadDosCmd(strRead))
        {
            return false;
        }

        if (NULL == JGW_WStrCaseWStr(strRead.c_str(),L"curl:") || NULL != JGW_WStrCaseWStr(strRead.c_str(),L"257")
            /*|| NULL != JGW_WStrCaseWStr(strRead.c_str(),L"550")*/)
        {
            return true;
        }

        return false;
    }

    bool CCJGW_CurlFtpDownload::TraverseCreateFTPDirectory(const std::string& strFTPDir)
    {
        char path[MAX_PATH] = {0};
        std::vector<std::string> vstrPath;
        char cSaveChar;
        strcpy_s(path, MAX_PATH, strFTPDir.c_str());
        JGW_PathAddBackslashA(path);
        bool bResult = true;
        size_t i, nLen = strlen(path);

        for (i = 2; i < nLen; i++)
        {
            if (JGW_IsPathSlashA(path[i]))
            {
                cSaveChar = path[i];
                path[i] = 0;
                vstrPath.push_back(path);
                //bResult = CreateCurlFTPDirectory(path);
                path[i] = cSaveChar;
            }
        }

        std::string strArg;
        JGW_FormatString(strArg," -u \"%s:%s\" \"ftp://%s:%d/\"",mstrUserName,mstrPassword,mstrHost,mnFtpPort);
        for (size_t i = 0;i < vstrPath.size();i ++)
        {
            strArg += " -Q \"MKD ";
            strArg += vstrPath[i];
            strArg += "\"";
        }

        return ExecuteCurlCommand(strArg);
    }
}

#endif