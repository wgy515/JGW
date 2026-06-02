#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <map>

namespace JGW
{
    typedef struct
    {
        //! 调试模式  开启远程调试并自动上传更新数据到MES系统 TDO:
        int mnDebugModeMs;
        //！ 上传log目录
        int mnUploadLogSec;
        //! 上传TestLog目录
        int mnUploadTestLogSec;
    }S_TASK_CONFIG_SQL,*PS_TASK_CONFIG_SQL;

    typedef struct
    {
        //! 项目名称
        std::wstring mstrName;
        //! 项目描述
        std::wstring mstrDescription;
    }S_PROJECT_SQL,*PS_PROJECT_SQL;


    typedef struct
    {
        //! 配置名称
        std::wstring mstrName;
        //!
        std::wstring mstrVersion;
        //! 配置详细描述
        std::wstring mstrDescription;
        //! 配置MD5
        std::wstring mstrMD5;
        //! File FTP URL
        std::wstring mstrFTPUrl;
        //! 外键 + 项目信息
        S_PROJECT_SQL msProjectSQL;
    }S_CONFIG_SQL,*PS_CONFIG_SQL;

    typedef struct
    {
        //! 安装版本名称
        std::wstring mstrName;
        //! 安装版本详细描述
        std::wstring mstrDescription;
        //！安装版本
        std::wstring mstrVersion;
        //! 配置MD5
        std::wstring mstrMD5;
        //! File FTP URL
        std::wstring mstrFTPUrl;
    }S_INSTALL_VERSION_SQL,*PS_INSTALL_VERSION_SQL;

    typedef struct
    {
        //! 是否是最新版本
        bool mbIsLatestVersion;
        //! 安装版本名称
        std::wstring mstrName;
        //! 安装版本详细描述
        std::wstring mstrDescription;
        //！安装版本
        std::wstring mstrVersion;
        //! 配置MD5
        std::wstring mstrMD5;
        //! File FTP URL
        std::wstring mstrFTPUrl;
        //! 外键 安装包相关信息
        S_INSTALL_VERSION_SQL msInstallVersion;
    }S_OTA_SQL,PS_OTA_SQL;


    typedef struct  
    {
        //!
        int mnInstallVersionID;
        //!
        int mnOTAID;
        //!
        int mnConfigID;
        //! 
        int mnTaskConfigID;
        //! 订单号
        std::wstring mstrTaskNumber;
        //! 订单描述
        std::wstring mstrDescription;
        //! 安装版本
        S_INSTALL_VERSION_SQL msInstallVersion;
        //！OTA版本相关信息 包含安装版本
        S_OTA_SQL msOTA;
        //! 配置信息
        S_CONFIG_SQL msConfig;
        //! 订单配置信息
        S_TASK_CONFIG_SQL msTaskConfig;
    }S_TASK_SQL,*PS_TASK_SQL;

    typedef struct  
    {
        bool mbIsUpdateTaskNumber;
        void* mpParam;
        boost::shared_ptr<boost::asio::ip::tcp::socket> mClientSock;
        std::wstring mstrExecFolder;
        std::wstring mstrTaskNumber;
        std::string mstrJsonCommand;
    } S_JGW_HELP_V2_DOWNLOAD_THREAD_PARAM,*LPS_JGW_HELP_V2_DOWNLOAD_THREAD_PARAM;

    //! version.ini 配置信息
    typedef struct  
    {
        //！安装包版本
        std::wstring mstrInstallVersion;
        //! 更新包版本
        std::wstring mstrUpdateVersion;
        //! 配置版本
        std::wstring mstrConfigVersion;
    }S_JGW_HELP_V2_VERSION_CONFIG,*LPS_JGW_HELP_V2_VERSION_CONFIG;


    typedef struct 
    {
        //!
        bool mbIsDownloadDB;
        //! 是否已经打开上传定时器
        bool mbIsOpenUploadTimer;
        //!
        long mlHelpDBFtpUpdateTime;
        //! 上传时间间隔
        size_t mnUploadIntervalTimeSec;

        //! 更新服务SOCKET端口
        size_t mnUpdateServiceSocketPort;
        //! SOCKET 超时时间
        size_t mnSocketTimeoutSec;
        //! 
        std::string mstrHelpServiceIPAddr;

        //！检查软件自动更新时间间隔
        size_t mnCheckSoftwareUpdateIntervalTimeSec;
        

        //! FTP端口
        size_t mnFTPPort;
        //! FTP HOST
        std::string mstrFTPHost;
        std::string mstrUserName;
        std::string mstrUserPassword;
     
        //! std::wstring mstrTaskNumber;
        std::wstring mstrFTPRootPath;
        std::wstring mstrFTPLogRootPath;  

        S_TASK_SQL msTaskSQL;
        //! 软件相关版本信息
        S_JGW_HELP_V2_VERSION_CONFIG msVersionConfig;
        //! 下载线程参数信息
        S_JGW_HELP_V2_DOWNLOAD_THREAD_PARAM msDownloadThreadParam;
    } S_JGW_HELP_V2_CONFIG,*LPS_JGW_HELP_V2_CONFIG;

}