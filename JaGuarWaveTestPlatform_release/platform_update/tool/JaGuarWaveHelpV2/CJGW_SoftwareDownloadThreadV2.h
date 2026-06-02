#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JaGuarWaveHelpV2/CJaGuarWaveHelpV2Define.h>
#include <JGW_WindowsFuncPlugin/CJGW_CurlFtpDownload.h>

namespace JGW
{
    class CCJGW_UpdateServiceThread;
    /*
    * 1、在FTP根目录下下载help.db => 从SQLITE help.db获取对应数据信息
    * 2、比对安装包版本 => 版本不一致，下载安装包版本重新安装
    * 3、比对更新包版本 => 版本不一致，下载更新包
    * 4、比对配置版本 =>  版本不一致，下载配置更新包
    */
    class CCJGW_SoftwareDownloadThreadV2 : public CCMessageThread
    {
    public:
        CCJGW_SoftwareDownloadThreadV2(S_JGW_HELP_V2_CONFIG& sHelpV2Config);
        ~CCJGW_SoftwareDownloadThreadV2(void);
    private:
        //! 软件版本更新，分为安装包、更新包更新以及配置版本
        void OnCheckSoftwareVersionUpdate(WPARAM wParam,LPARAM lParam);
    private:
        //!
        bool LoadHelpDB();
        //! 
        bool CheckInstallVersion(bool& bUpdate);
        //! 
        bool CheckOTAVersion(bool& bUpdate);
        //! 
        bool CheckConfigVersion(bool& bUpdate);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool mbInstallUpdate; //! 是否更新安装包
        CCJGW_UpdateServiceThread* mpHelpServiceThread;
        S_JGW_HELP_V2_CONFIG& msHelpV2Config;
        CCJGW_CurlFtp mcCurlFtpDownload;
    };
}


