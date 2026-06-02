#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
//#include <JaGuarWaveHelpV2/CJaGuarWaveHelpV2Define.h>

namespace JGW
{
    typedef struct
    {
        //! 是否更新DB日志参数配置文件
        bool mbIsUpdateDBLogConfig;
        //! 调试模式  开启远程调试并自动上传更新数据到MES系统 TDO:
        int mnDebugModeMs;
        //！ 上传log目录
        int mnUploadLogSec;
        //! 上传TestLog目录
        int mnUploadTestLogSec;
        //! 是否上传$$_EXEC_FOLDER_$$\\log的日志文件
        bool mbIsUploadLog;
        //! 是否上传$$_EXEC_FOLDER_$$\\TestLog的日志文件
        bool mbIsUploadTestLog;
        //! $$_Custom_$$ 对应的相对路径，不为空则开始监听$$_EXEC_FOLDER_$$\\$$_Custom_$$目录的日志文件
        std::wstring mstrCustomLogDir;
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
        //!
        std::wstring mstrIntsallTaskNumber;
        //！安装包版本
        std::wstring mstrInstallVersion;
        //! 更新包版本
        std::wstring mstrUpdateVersion;
        //! 配置版本
        std::wstring mstrConfigVersion;
    }S_JGW_HELP_V2_VERSION_CONFIG,*LPS_JGW_HELP_V2_VERSION_CONFIG;


    //! $$_EXEC_FOLDER_$$ : 当前程序所在目录
    // 通过FTP上传LOG配置信息
    typedef struct
    {
        //! 是否更新了数据库LOG配置
        //bool mbIsUpdateDBLogConfig;
        //! 是否上传$$_EXEC_FOLDER_$$\\log的日志文件
        //bool mbIsUploadLog;
        //! 是否上传$$_EXEC_FOLDER_$$\\TestLog的日志文件
        //bool mbIsUploadTestLog;
        //! 上传$$_EXEC_FOLDER_$$\\log的日志文件间隔时间秒为单位 同时也是上传$$_EXEC_FOLDER_$$\\$$_Custom_$$的日志文件间隔时间秒为单位
        //size_t mnUploadLogSecond;
        //! 上传$$_EXEC_FOLDER_$$\\TestLog的日志文件间隔时间秒为单位
        //size_t mnUploadTestLogSecond;
        //! FTP port 默认26
        size_t mnPort;
        //! help.ini / 默认 : 192.168.8.10
        std::string mstrHost;
        //! FTP 根目录 : /JGWPTP_Help/
        std::wstring mstrRootPath;
        //! FTP log根目录 : /JaGuarWaveTestPlatform_Log/
        std::wstring mstrLogRootPath;
        //! FTP用户名 : 默认加密字串 lKNG7Im5x0ith8DsldhiTw==
        std::string mstrUserName;
        //! FTP用户密码 : 默认加密字串 WWC7r11A4ezrNs6aEyHcIg==
        std::string mstrUserPassword;
        //! $$_Custom_$$ 对应的相对路径，不为空则开始监听$$_EXEC_FOLDER_$$\\$$_Custom_$$目录的日志文件
        //! std::wstring mstrCustomLogDir;
    }S_FTP_LOG_INFO,*LPS_FTP_LOG_INFO;

    typedef struct
    {
        //! 已经下载过数据库DB文件
        bool mbIsDownloadDB;
        //! DB文件更新时间 === 上次更新时间
        long mlHelpDBFtpUpdateTime;
        //! 是否已经打开上传定时器,自动LOOP使用
        bool mbIsOpenUploadTimer;

        //! 更新服务SOCKET端口
        size_t mnUpdateServiceSocketPort;
        //! SOCKET 超时时间
        size_t mnSocketTimeoutSec;
        //! 127.0.0.1
        //std::string mstrHelpServiceIPAddr;
        //！检查软件自动更新时间间隔 300s
        size_t mnCheckSoftwareUpdateIntervalTimeSec;
        //! 订单号
        //std::wstring mstrTaskNumber;
        //! 
        S_FTP_LOG_INFO msFTPlogInfo;
        //
        S_TASK_SQL msTaskSQL;
        //! 软件相关版本信息
        S_JGW_HELP_V2_VERSION_CONFIG msVersionConfig;
        //! 下载线程参数信息
        S_JGW_HELP_V2_DOWNLOAD_THREAD_PARAM msDownloadThreadParam;
    }S_HELP_V3_CONFIG;
}
