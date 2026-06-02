#include "StdAfx.h"
#include "PackageUpdateThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include <JGW_CurlPlugin/CJGW_CurlFtp.h>
#include <JaGuarWaveHelp/CJGW_InstallXmlConfig.h>
#include <JaGuarWaveHelp/CJGW_UpdateXmlConfig.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <poco/Path.h>
#include <poco/File.h>
#include <poco/Zip/Compress.h>
#include <poco/Zip/ZipArchive.h>
#include <fstream>
#include <JGW_FoundationFunc/CUtf8String.hpp>
namespace JGW
{
    CPackageUpdateThread::CPackageUpdateThread(S_PACKAGEUPDATE_INFO& sPackageUpdateInfo) : msPackageUpdateInfo(sPackageUpdateInfo)
    {
    }


    CPackageUpdateThread::~CPackageUpdateThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CPackageUpdateThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_PARSE_PROGRAMFILES_MSG,&CPackageUpdateThread::OnParseProgramFilesMsg)
        ON_MYTHREAD_MESSGAE(WM_THREAD_UPLOAD_PACKAGEUPDATEFILE_MSG,&CPackageUpdateThread::OnUploadPackageUpdateFilesMsg)
    END_MYTHREAD_MESSAGE_MAP()

    void CPackageUpdateThread::OnParseProgramFilesMsg(WPARAM wParam,LPARAM lParam)
    {
        if (!KillProgramProcess())
        {
            MessageBox(msPackageUpdateInfo.mhMainWnd,L"Closing the ongoing installation directory process failed",L"Closing process failed",MB_ICONERROR);
            msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
            return ;
        }

        if (!GetInstallPackageInformation())
        {
            MessageBox(msPackageUpdateInfo.mhMainWnd,L"Check that you have selected the correct installation package directory",L"Failed to get installation package version information",MB_ICONERROR);
            msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
            return ;
        }

        if (!CheckInstallPackageIsLatestVersion())
        {
            MessageBox(msPackageUpdateInfo.mhMainWnd,L"Checking the latest software version failed",L"Checking version failed",MB_ICONERROR);
            msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
            return ;
        }

        if (!GetModifiedFileToProgramFilesMD5())
        {
            MessageBox(msPackageUpdateInfo.mhMainWnd,L"Failed to obtain program directory file MD5 information",L"Get Program File MD5 Fail",MB_ICONERROR);
            msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
            return ;
        }
        msPackageUpdateInfo.mbIsParseProgramFiles = true;
        msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
    }

    bool CPackageUpdateThread::KillProgramProcess()
    {
        std::wstring strKillProcessBatFilePath;
        JGW_FormatWString(strKillProcessBatFilePath,L"%s\\kill_process.bat",msPackageUpdateInfo.mstrProgramFiles.c_str());

        if (!JGW_FileExistsToFilePath(strKillProcessBatFilePath.c_str()))
        {
            return false;
        }
#if 0
        CCJGW_PeekNamePide cPeekNamePide;
        cPeekNamePide.SendDosCmds(strKillProcessBatFilePath.c_str(),L"",120 * 1000);
        std::wstring strRead;
        cPeekNamePide.ReadDosCmd(strRead);
#else
        ShellExecute(NULL,L"open",strKillProcessBatFilePath.c_str(),NULL,msPackageUpdateInfo.mstrProgramFiles.c_str(),SW_NORMAL);
        /*for (int i = 0;i < 6 * 60;i++)
        {
            if (-1 != JGW_GetProcessID2Name(_T("cmd.exe"))) return true;
            Sleep(1 * 1000);
        }*/
#endif
        return true;
    }

    bool CPackageUpdateThread::GetInstallPackageInformation()
    {
        std::wstring strProgramVersionFilePath,strMesConfigFilePath,strHelpConfigFilePath;

        JGW_FormatWString(strProgramVersionFilePath,L"%s\\exec\\version.ini",msPackageUpdateInfo.mstrProgramFiles.c_str());
        JGW_FormatWString(strMesConfigFilePath,L"%s\\exec\\jgw_mes_config.data",msPackageUpdateInfo.mstrProgramFiles.c_str());
        JGW_FormatWString(strHelpConfigFilePath,L"%s\\exec\\help.ini",msPackageUpdateInfo.mstrProgramFiles.c_str());
        if (!JGW_FileExistsToFilePath(strProgramVersionFilePath.c_str())
            || !JGW_FileExistsToFilePath(strMesConfigFilePath.c_str())
            || !JGW_FileExistsToFilePath(strHelpConfigFilePath.c_str()))
        {
            mstrErrorMsg = L"Failed to get the config file of the program";
            return false;
        }

        {
            CCJGW_ConfigIni versionConfigInif;
            versionConfigInif.InitIniFilePath(strProgramVersionFilePath.c_str());
            msPackageUpdateInfo.mstrInstallVersion = versionConfigInif.GetIniKeyValue(L"install",L"version");
            msPackageUpdateInfo.mstrCurrentUpdateVersion = versionConfigInif.GetIniKeyValue(L"update",L"version");
            if (msPackageUpdateInfo.mstrInstallVersion.empty())
            {
                mstrErrorMsg = L"Failed to get the installed version of the program";
                return false;
            }
            msPackageUpdateInfo.mstrUpdateVersion = L"Version"/*msPackageUpdateInfo.mstrInstallVersion*/ + JGW_GetCurrentTimeStampYMDHMS();
            /*if (msPackageUpdateInfo.mstrCurrentUpdateVersion.empty())
            {
            msPackageUpdateInfo.mstrUpdateVersion = msPackageUpdateInfo.mstrInstallVersion + L"001";
            }
            else
            {
            int index = 0;
            if (msPackageUpdateInfo.mstrCurrentUpdateVersion.size() >= 3)
            {
            index = _ttoi(msPackageUpdateInfo.mstrCurrentUpdateVersion.substr(msPackageUpdateInfo.mstrCurrentUpdateVersion.size() - 3,3).c_str());
            }
            JGW_FormatWString(msPackageUpdateInfo.mstrUpdateVersion,L"%s\\%03d",msPackageUpdateInfo.mstrInstallVersion.c_str(),index);
            }*/
        }

        {
            CCJGW_ConfigIni mesConfigIni;
            mesConfigIni.InitIniFilePath(strMesConfigFilePath.c_str());
            msPackageUpdateInfo.mstrTaskNumber = mesConfigIni.GetIniKeyValue(L"MESClient",L"TaskNumber");
            if (msPackageUpdateInfo.mstrTaskNumber.empty())
            {
                mstrErrorMsg = L"Failed to get the task number";
                return false;
            }
        }
        {
            CCJGW_ConfigIni helpConfigIni;
            CCJGW_CryptPassword cryptPassword;
            helpConfigIni.InitIniFilePath(strHelpConfigFilePath.c_str());
            msPackageUpdateInfo.msUpdateFtpServerInfo.mnFTPPort = helpConfigIni.GetIniKeyIntValue(L"FTP",L"Port",26);
            msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPHost = helpConfigIni.GetIniKeyValueA(L"FTP",L"Host",L"192.168.8.10");
            msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPRootPath = helpConfigIni.GetIniKeyValue(L"FTP",L"RootPath",L"/JaGuarWaveTestPlatform_Version/");

            msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPLogRootPath = helpConfigIni.GetIniKeyValue(L"FTP",L"LogRootPath",L"/JaGuarWaveTestPlatform_Version/");

            std::wstring strUserName = helpConfigIni.GetIniKeyValue(L"FTP",L"UserName",L"lKNG7Im5x0ith8DsldhiTw==");
            std::wstring strUserPassword = helpConfigIni.GetIniKeyValue(L"FTP",L"Password",L"WWC7r11A4ezrNs6aEyHcIg==");
            msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserName = cryptPassword.DecryptPassword(JGW_W2A(strUserName,CP_UTF8));
            msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserPassword = cryptPassword.DecryptPassword(JGW_W2A(strUserPassword,CP_UTF8));
        }
        return true;
    }

    bool CPackageUpdateThread::CheckInstallPackageIsLatestVersion()
    {
        std::wstring strFTPInstallVersion,strFTPUpdateVersion,strTempInstallVersion,strTempUpdateVersion,strErrorMsg;

        JGW_FormatWString(strFTPInstallVersion,L"%s/Install/install.xml",msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPRootPath.c_str());
        JGW_FormatWString(strFTPUpdateVersion,L"%s/Update/%s/update.xml",msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPRootPath.c_str(),msPackageUpdateInfo.mstrTaskNumber.c_str());
        JGW_FormatWString(strTempInstallVersion,L"%s\\temp_install.xml",JGW_GetApplicationFolder());
        JGW_FormatWString(strTempUpdateVersion,L"%s\\temp_update.xml",JGW_GetApplicationFolder());

        CCJGW_CurlFtp curlFtpDownload(msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPHost,msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserName,msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserPassword,msPackageUpdateInfo.msUpdateFtpServerInfo.mnFTPPort);
        
        if (!curlFtpDownload.DownloadCurlFtpFile(strFTPInstallVersion,strTempInstallVersion,strErrorMsg))
        {
            mstrErrorMsg = L"Download Install Xml File Fail";
            return false;
        }

        bool bUpdateXmlExist = false;
        bool bDownloadFile = curlFtpDownload.CheckPathCurlFtpExist(strFTPUpdateVersion,strErrorMsg);
        if (bDownloadFile && !curlFtpDownload.DownloadCurlFtpFile(strFTPUpdateVersion,strTempUpdateVersion,strErrorMsg))
        {
            mstrErrorMsg = L"Download Update Xml File Fail";
            return false;
        }

        /*if (bUpdateXmlExist && !bDownloadFile)
        {
            mstrErrorMsg = L"Download Update Xml File Fail";
            return false;
        }*/

        CCJGW_InstallXmlConfig installXmlConfig;
        if (!installXmlConfig.LoadInstallXmlConfig(strTempInstallVersion,msPackageUpdateInfo.msInstallXmlConfig))
        {
            mstrErrorMsg = L"Load Install Xml File Fail";
            return false;
        }

        //! update xml file exist
        if (bDownloadFile && bUpdateXmlExist)
        {
            CCJGW_UpdateXmlConfig updateXmlConfig;
            if (!updateXmlConfig.LoadUpdateXmlConfig(strTempUpdateVersion,msPackageUpdateInfo.msUpdateXmlConfig))
            {
                mstrErrorMsg = L"Load Update Xml File Fail";
                return false;
            }

            //! 为最新安装版本,则检查更新版本
            if (NULL == msPackageUpdateInfo.mstrInstallVersion.compare(msPackageUpdateInfo.msInstallXmlConfig.msNewInstallConfig.mstrVersion))
            {
                msPackageUpdateInfo.mbIsNewInstallVersion = true;
                //! 没有需要更新的版本则直接返回true
                if (msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.empty())
                {
                    return true;
                }
                //! 检查更新版本信息
                for (std::vector<S_JGW_HELP_UPDATE_PACKAGE>::iterator it = msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.begin();
                    it != msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.end();
                    ++ it)
                {
                    //! 已经更新
                    if (NULL == it->mstrInstallVersion.compare(L"*") || NULL == it->msUpdateInfo.mstrVersion.compare(msPackageUpdateInfo.mstrCurrentUpdateVersion))
                    {
                        return true;
                    }
                } 
                mstrErrorMsg = L"Please update the current software version";
                return false;
            }
            else //! 不为最新版本
            {
                msPackageUpdateInfo.mbIsNewInstallVersion = false;
                //! 需要更新当前安装版本
                if (msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.empty())
                {
                    mstrErrorMsg = L"Please update the current software install version";
                    return false;
                }
                for (std::vector<S_JGW_HELP_UPDATE_PACKAGE>::iterator it = msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.begin();
                    it != msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.end();
                    ++ it)
                {
                    //! 如果安装版本一致，则检测更新版本 *
                    if (NULL == msPackageUpdateInfo.mstrInstallVersion.compare(it->mstrInstallVersion) 
                        /*|| NULL == it->mstrInstallVersion.compare("*")*/)
                    {
                        //! 如果更新版本不一致，则提示需要更新
                        if (NULL == msPackageUpdateInfo.mstrUpdateVersion.compare(it->msUpdateInfo.mstrVersion))
                        {
                            return true;
                        }
                    }
                }
                mstrErrorMsg = L"Please update the current software version";
                return false;
            }
        }
        //! update xml not file exist
        if (!bUpdateXmlExist)
        {
            //! 当前版本不为最新版本，则重新下载最新版本
            if (NULL != msPackageUpdateInfo.mstrInstallVersion.compare(msPackageUpdateInfo.msInstallXmlConfig.msNewInstallConfig.mstrVersion))
            {
                mstrErrorMsg = L"Please update the current software version";
                return false;
            }
            msPackageUpdateInfo.mbIsNewInstallVersion = true;
        }

        return true;
    }

    void CPackageUpdateThread::LoadUpdatePackageIgnoreFile(std::vector<std::wstring>& vUpdatePackageIgnore)
    {
        std::wifstream inputFile;
        std::wstring strDefaultUpdatePackageIgnoreFilePath,strTemp;

        JGW_FormatWString(strDefaultUpdatePackageIgnoreFilePath,L"%supdate.package.ignore",JGW_GetApplicationFolder());
        inputFile.imbue( std::locale("") );
        inputFile.open(strDefaultUpdatePackageIgnoreFilePath);
        if (!inputFile.is_open()) return;

        wchar_t str[512] = {0};
        while (inputFile.getline(str,512))
        {
            strTemp = str;
            JGW_EraseLastAndFristChars(strTemp,' ');
            JGW_ReplaceStringW(strTemp,L"\\",L"/");
            vUpdatePackageIgnore.push_back(strTemp);
        }
        inputFile.close();
    }

    void ScanPuamThroughputTestResultFile(const wchar_t* filepath,const wchar_t* filename,void* lpData)
    {
        PS_MD5FILE_PARAM psMD5FileParam = (PS_MD5FILE_PARAM)lpData;
        std::wstring strTmep (filepath);
        strTmep += L"/";
        strTmep += filename;
        std::wstring strMD5 = CCJGW_CryptFileMD5::GetFileMD5(strTmep);
        std::wstring strFolder = psMD5FileParam->mstrProgramFiles + L"/",strIgnoreFile;
        
        for (size_t i = 0;i < psMD5FileParam->mvIgnoreFiles.size();i ++)
        {
            JGW_FormatWString(strIgnoreFile,L"%s/%s",psMD5FileParam->mstrProgramFiles.c_str(),psMD5FileParam->mvIgnoreFiles[i].c_str());
            if (0 == _wcsnicmp(strIgnoreFile.c_str(),strTmep.c_str(),strIgnoreFile.length()))
            {
                return ;
            }
        }
        JGW_ReplaceStringW(strTmep,strFolder,L"");
        psMD5FileParam->mmapFileMD5.insert(std::make_pair<std::wstring,std::wstring>(strTmep,strMD5));
    }

    bool CPackageUpdateThread::GetModifiedFileToProgramFilesMD5()
    {
#if 1
        std::wstring strMD5ListFtpPath,strMDListFilePath,strErrorMsg;

        JGW_FormatWString(strMD5ListFtpPath,L"%s/Install/%s",msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPRootPath.c_str(),msPackageUpdateInfo.msInstallXmlConfig.msNewInstallConfig.mstrFileName.c_str());
        JGW_EraseAfterToRemoveCharW(strMD5ListFtpPath,L'/');
        strMD5ListFtpPath += L"md5.list";
        //! 如果未下载MD5 LIST FILE则说明
        JGW_FormatWString(strMDListFilePath,L"%stemp_md5.list",JGW_GetApplicationFolder());
        CCJGW_CurlFtp curlFtpDownload(msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPHost,msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserName,msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserPassword,msPackageUpdateInfo.msUpdateFtpServerInfo.mnFTPPort);
        if (!curlFtpDownload.DownloadCurlFtpFile(strMD5ListFtpPath,strMDListFilePath,strErrorMsg))
        {
            mstrErrorMsg = L"Download MD5 List File Fail";
            return true;
        }

        CCJGW_ConfigIni md5ListConfigIni;
        md5ListConfigIni.InitIniFilePath(strMDListFilePath.c_str());

        int numFiles = 0;
        S_MD5FILE_PARAM sMD5FileParam;
        sMD5FileParam.mstrProgramFiles = msPackageUpdateInfo.mstrProgramFiles;
        std::wstring strFolder(msPackageUpdateInfo.mstrProgramFiles),strTemp;
        JGW_EraseLastAndFristTwoCharsW(strFolder,L'\\',L'/');
        LoadUpdatePackageIgnoreFile(sMD5FileParam.mvIgnoreFiles);
        JGW_TraverseFolder_VC(strFolder,numFiles,L"\\*.*"/*"/ *QSEQ*.html"*/,ScanPuamThroughputTestResultFile,&sMD5FileParam);
        for (std::map<std::wstring,std::wstring>::iterator it = sMD5FileParam.mmapFileMD5.begin();
            it != sMD5FileParam.mmapFileMD5.end();
            ++ it)
        {
            strTemp = md5ListConfigIni.GetIniKeyValue(L"LIST",it->first.c_str());
            if (strTemp.empty())
            {
                msPackageUpdateInfo.mvUpdateFilePath.push_back(it->first);
                continue;
            }
            if (NULL != strTemp.compare(it->second))
            {
                msPackageUpdateInfo.mvUpdateFilePath.push_back(it->first);
                continue;
            }
        }
#endif
        return true;
    }

    void CPackageUpdateThread::OnUploadPackageUpdateFilesMsg(WPARAM wParam,LPARAM lParam)
    {
        if (!CompressPackageUpdateFiles())
        {
            MessageBox(msPackageUpdateInfo.mhMainWnd,L"Failed to compress the update package",L"Update failed",MB_ICONERROR);
            msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
            return ;
        }

        if (!CreateUpdateXmlFile())
        {
            MessageBox(msPackageUpdateInfo.mhMainWnd,L"Failed to create update xml",L"Update failed",MB_ICONERROR);
            msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
            return ;
        }

        if (!UploadCompressPackageUpdateFiles())
        {
            MessageBox(msPackageUpdateInfo.mhMainWnd,L"Failed to upload update package",L"Update failed",MB_ICONERROR);
            msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
            return ;
        }

        MessageBox(msPackageUpdateInfo.mhMainWnd,L"The update package was uploaded successfully",L"Update failed",MB_OK);
        msPackageUpdateInfo.mbIsParseProgramFiles = false;
        msPackageUpdateInfo.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_OPERATION_COMPLETE_MSG);
    }

    bool CPackageUpdateThread::CompressPackageUpdateFiles()
    {
        JGW_FormatWString(msPackageUpdateInfo.mstrTempUpdatePackageFilePath,L"%stemp_update_package.zip",JGW_GetApplicationFolder());
        JGW_DeleteFile(msPackageUpdateInfo.mstrTempUpdatePackageFilePath.c_str());

        std::ofstream out;
        out.imbue( std::locale("") );
        out.open(JGW_W2A(msPackageUpdateInfo.mstrTempUpdatePackageFilePath).c_str(),std::ios::binary);
        if (!out.is_open()) 
        {
            mstrErrorMsg = L"Create Package Update File Failed";
            return false;
        }

        Poco::Zip::Compress c(out, true);
        std::wstring strAbsFilePath,strFolder = msPackageUpdateInfo.mstrProgramFiles,strUpdateFile;
        int numFiles = 0;
        JGW_ReplaceStringW(strFolder,L"\\",L"/");

        for (size_t i = 0;i < msPackageUpdateInfo.mvUpdateFilePath.size();i ++)
        {
            strUpdateFile = msPackageUpdateInfo.mvUpdateFilePath[i];
            JGW_FormatWString(strAbsFilePath,L"%s/%s",strFolder.c_str(),strUpdateFile.c_str());
            Poco::Path theFile(JGW_W2A(strAbsFilePath,CP_UTF8));
            Poco::Path fileName(JGW_W2A(strUpdateFile,CP_UTF8));
            try
            {
                c.addFile(theFile, fileName);
                //!inputFile << it->second << " " << it->first << std::endl;
                numFiles ++;
            }
            catch (Poco::Exception& e)
            {
                mstrErrorMsg = JGW_GetFormatWString(L"Compress File(%s) Fail(%s)",strAbsFilePath.c_str(),JGW_A2W_A(e.what()).c_str());
                return false;
            }
        }
        try
        {
            Poco::Zip::ZipArchive a(c.close());
            if (0 == numFiles) return false;
            return true;
        }
        catch (Poco::Exception& e)
        {
            mstrErrorMsg = JGW_GetFormatWString(L"Compress File Fail(%s)",JGW_A2W_A(e.what()).c_str());
        }
        return true;
    }

    bool CPackageUpdateThread::CreateUpdateXmlFile()
    {
        JGW_FormatWString(msPackageUpdateInfo.mstrTempUpdateXmlFilePath,L"%stemp_update.xml",JGW_GetApplicationFolder());
        JGW_DeleteFile(msPackageUpdateInfo.mstrTempUpdateXmlFilePath);

        S_JGW_HELP_UPDATE_XML_CONFIG sUpdateXmlConfig;
        sUpdateXmlConfig.mbVersieLimiet = true;
        //! 判断当前版本是否是最新版本
        if (0 == msPackageUpdateInfo.msInstallXmlConfig.msNewInstallConfig.mstrVersion.compare(msPackageUpdateInfo.mstrInstallVersion))
        {
            //! 
            S_JGW_HELP_UPDATE_PACKAGE sHelpUpdatePackage;
            //! 
            sHelpUpdatePackage.mstrInstallVersion = L"*";
            sHelpUpdatePackage.msUpdateInfo.mstrVersion = msPackageUpdateInfo.mstrUpdateVersion;
            sHelpUpdatePackage.msUpdateInfo.mstrUpdatePack = msPackageUpdateInfo.mstrUpdateVersion + L".zip";
            sHelpUpdatePackage.msUpdateInfo.mstrUpdatePackMD5 = CCJGW_CryptFileMD5::GetFileMD5(msPackageUpdateInfo.mstrTempUpdatePackageFilePath);
            sUpdateXmlConfig.mvsHelpUpdatePackage.push_back(sHelpUpdatePackage);
            //! 
            sHelpUpdatePackage.mstrInstallVersion = msPackageUpdateInfo.mstrInstallVersion;
            sHelpUpdatePackage.msUpdateInfo.mstrVersion = msPackageUpdateInfo.mstrUpdateVersion;
            sHelpUpdatePackage.msUpdateInfo.mstrUpdatePack = msPackageUpdateInfo.mstrUpdateVersion + L".zip";
            sHelpUpdatePackage.msUpdateInfo.mstrUpdatePackMD5 = CCJGW_CryptFileMD5::GetFileMD5(msPackageUpdateInfo.mstrTempUpdatePackageFilePath);
            sUpdateXmlConfig.mvsHelpUpdatePackage.push_back(sHelpUpdatePackage);

            for (size_t i = 0;i < msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.size();i ++)
            {
                //! 排序默认更新配置，以及相同安装版本
                if (0 == msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i].mstrInstallVersion.compare(L"*")
                    || 0 == msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i].mstrInstallVersion.compare(msPackageUpdateInfo.mstrInstallVersion))
                {
                    continue;
                }
                sUpdateXmlConfig.mvsHelpUpdatePackage.push_back(msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i]);
            }
        }
        else
        {
            for (size_t i = 0;i < msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage.size();i ++)
            {
                //! 相同安装版本 => 更新相关配置
                if (0 == msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i].mstrInstallVersion.compare(msPackageUpdateInfo.mstrInstallVersion))
                {
                    msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i].mstrInstallVersion = msPackageUpdateInfo.mstrInstallVersion;
                    msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i].msUpdateInfo.mstrVersion = msPackageUpdateInfo.mstrUpdateVersion;
                    msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i].msUpdateInfo.mstrUpdatePack = msPackageUpdateInfo.mstrUpdateVersion + L".zip";
                    msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i].msUpdateInfo.mstrUpdatePackMD5 = CCJGW_CryptFileMD5::GetFileMD5(msPackageUpdateInfo.mstrTempUpdatePackageFilePath);
                }
                sUpdateXmlConfig.mvsHelpUpdatePackage.push_back(msPackageUpdateInfo.msUpdateXmlConfig.mvsHelpUpdatePackage[i]);
            }
        }

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlDeclaration* pDeclaration= new TiXmlDeclaration("1.0","UTF-8",""); 
        ptrXmlDoc->LinkEndChild(pDeclaration);  

        TiXmlElement* pRoot = new TiXmlElement("update");
        ptrXmlDoc->LinkEndChild(pRoot); 
        SetElementAttributeValue(pRoot,"versielimiet",sUpdateXmlConfig.mbVersieLimiet ? L"1" : L"0");

        for (size_t i = 0;i < sUpdateXmlConfig.mvsHelpUpdatePackage.size();i ++)
        {
            TiXmlElement* pPackageNode = new TiXmlElement("package");
            SetElementAttributeValue(pPackageNode,"install_version",sUpdateXmlConfig.mvsHelpUpdatePackage[i].mstrInstallVersion);
            TiXmlElement* pUpdateNode = new TiXmlElement("update");
            SetElementAttributeValue(pUpdateNode,"version",sUpdateXmlConfig.mvsHelpUpdatePackage[i].msUpdateInfo.mstrVersion);
            SetElementAttributeValue(pUpdateNode,"updatepack",sUpdateXmlConfig.mvsHelpUpdatePackage[i].msUpdateInfo.mstrUpdatePack);
            SetElementAttributeValue(pUpdateNode,"updatepackmd5",sUpdateXmlConfig.mvsHelpUpdatePackage[i].msUpdateInfo.mstrUpdatePackMD5);
            pPackageNode->LinkEndChild(pUpdateNode);
            pRoot->LinkEndChild(pPackageNode);
        }
        return ptrXmlDoc->SaveFile(JGW_W2A(msPackageUpdateInfo.mstrTempUpdateXmlFilePath));
    }

    bool CPackageUpdateThread::UploadCompressPackageUpdateFiles()
    {
        std::wstring strFTPUpdateXmlPath,strFTPUpdatePackageFilePath,strErrorMsg,strFTPUpdateFolder;

        JGW_FormatWString(strFTPUpdateFolder,L"%s/Update/%s",msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPRootPath.c_str());
        JGW_FormatWString(strFTPUpdateXmlPath,L"%s/Update/%s/update.xml",msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPRootPath.c_str(),msPackageUpdateInfo.mstrTaskNumber.c_str());
        JGW_FormatWString(strFTPUpdatePackageFilePath,L"%s/Update/%s/%s.zip",msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPRootPath.c_str(),msPackageUpdateInfo.mstrTaskNumber.c_str(),msPackageUpdateInfo.mstrUpdateVersion.c_str());

        CCJGW_CurlFtp curlFtpDownload(msPackageUpdateInfo.msUpdateFtpServerInfo.mstrFTPHost,msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserName,msPackageUpdateInfo.msUpdateFtpServerInfo.mstrUserPassword,msPackageUpdateInfo.msUpdateFtpServerInfo.mnFTPPort);
        
        curlFtpDownload.CreateDirectoryCurlFtpFile(strFTPUpdateFolder,strErrorMsg);
        if (!curlFtpDownload.UploadCurlFtpFile(strFTPUpdatePackageFilePath,msPackageUpdateInfo.mstrTempUpdatePackageFilePath,strErrorMsg))
        {
            mstrErrorMsg = L"Upload Update Package File Fail";
            return false;
        }


        if (!curlFtpDownload.UploadCurlFtpFile(strFTPUpdateXmlPath,msPackageUpdateInfo.mstrTempUpdateXmlFilePath,strErrorMsg))
        {
            mstrErrorMsg = L"Upload Update Package File Fail";
            return false;
        }

        return true;
    }
}

