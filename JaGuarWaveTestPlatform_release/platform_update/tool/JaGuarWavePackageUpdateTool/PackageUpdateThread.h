#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>

namespace JGW
{
    class CPackageUpdateThread : public CCMessageThread
    {
    public:
        CPackageUpdateThread(S_PACKAGEUPDATE_INFO& sPackageUpdateInfo);
        ~CPackageUpdateThread(void);
    private:
        void OnParseProgramFilesMsg(WPARAM wParam,LPARAM lParam);
        //!
        bool KillProgramProcess();
        //! 获取安装包版本相关信息，版本、更新版本、订单号、更新服务器FTP相关信息、
        bool GetInstallPackageInformation();
        //! 检查当前订单或者版本是否为最新软件版本 1、下载当前订单对应的软件相关信息、下载MD5
        bool CheckInstallPackageIsLatestVersion();
        //! 遍历程序目录的MD5值，并且比对服务器MD5值
        //! bool CheckProgramFilesMD5();
        //! 遍历程序目录的MD5值，并且比对服务器MD5值.获取修改过文件信息，比对程序包的MD5值
        bool GetModifiedFileToProgramFilesMD5(); 
    private:
        //! 
        void OnUploadPackageUpdateFilesMsg(WPARAM wParam,LPARAM lParam);
        //! 压缩安装包更新文件
        bool CompressPackageUpdateFiles();
        //!
        bool CreateUpdateXmlFile();
        //! 上传压缩包文件
        bool UploadCompressPackageUpdateFiles();
    private:
        void LoadUpdatePackageIgnoreFile(std::vector<std::wstring>& vUpdatePackageIgnore);
    protected:   
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        S_PACKAGEUPDATE_INFO& msPackageUpdateInfo;
        std::wstring mstrErrorMsg;
    };
}


