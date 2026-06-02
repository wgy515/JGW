#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <map>
#include "CJGW_HelpUpdateDefine.h"
#define SOCKET_TX_BUF_MAX           (4 * 1024)
#define SOCKET_RX_BUF_MAX           (4 * 1024)

#define DEFAULT_INSTALL_FILE_PATH "C:\\JaGuarWaveTestPlatformInstall\\JaGuarWaveTestPlatform_Setup.exe"

#define DEFAULT_UPDATE_FILE_PATH "C:\\JaGuarWaveTestPlatformInstall\\update.zip"
namespace JGW
{

    typedef struct  
    {
        void* mpParam;
        boost::shared_ptr<boost::asio::ip::tcp::socket> mClientSock;
        std::wstring mstrExecFolder;
        std::wstring mstrTaskNumber;
        std::string mstrJsonCommand;
    } S_JGW_HELP_DOWNLOAD_THREAD_PARAM,*LPS_JGW_HELP_DOWNLOAD_THREAD_PARAM;


    typedef struct
    {
        //! std::vector<std::string> mvUploadIgnoreFilePath;
        //! key size_t 是文件目录大小 value  => key std::string file path  value md5
        //! std::map<size_t,std::map<std::string,std::string>> mmapUploadIgnoreFilePath;
        std::map<std::wstring,std::wstring> mmapUploadIgnoreFilePath;
        //! key std::string file path  value md5
        std::map<std::wstring,std::wstring> mvUploadFilePath;
    }S_UPLOAD_FILEPATH_PARAM,*LPS_UPLOAD_FILEPATH_PARAM;

    //! version.ini 配置信息
    typedef struct  
    {
        std::wstring mstrInstallVersion;
        std::wstring mstrUpdateVersion;
    }S_JGW_HELP_VERSION_CONFIG,*LPS_JGW_HELP_VERSION_CONFIG;

    //! update.ini配置信息
    typedef struct  
    {
        std::wstring mstrInstallVersion; //! 安装软件版本，为空的时候下载最新的软件版本
        std::wstring mstrInstallFileName; //! 安装软件版本位于安装目录下载地址
        std::wstring mstrInstallFileMD5; //! 安装文件的校验码
        std::wstring mstrUpdateVersion; //! 基于基础版本上面增加001
        std::wstring mstrUpdatePack; //! 更新包名称
        std::wstring mstrUpdatePackMD5; //! md5主要用于包校验
    }S_JGW_HELP_UPDATE_CONFIG,*LPS_JGW_HELP_UPDATE_CONFIG;

    //! install.ini配置信息
    typedef struct
    {
        std::wstring mstrNewsInstallFTPFileName;
        std::wstring mstrNewsInstallVersion;
        std::wstring mstrNewsInstallMD5;
    }S_JGW_HELP_INSTALL_CONFIG,*LPS_JGW_HELP_INSTALL_CONFIG;

    //! upload.ini配置信息 当未找到upload.ini 配置的时候，后台每隔2分钟下载查找下upload.ini
    typedef struct
    {
        bool mbUploadTestLog;
        bool mbUploadLog;
        std::wstring mstrCustomDir; //! 用于自定义上传目录 抓取其他的log
    }S_JGW_HELP_UPLOAD_CONFIG,*LPS_JGW_HELP_UPLOAD_CONFIG;

    typedef struct 
    {
        //! 是否已经打开上传定时器
        bool mbIsOpenUploadTimer;
        //! 更新服务SOCKET端口
        size_t mnUpdateServiceSocketPort;
        //! SOCKET 超时时间
        size_t mnSocketTimeoutSec;
        //! FTP端口
        size_t mnFTPPort;
        //！检查软件自动更新时间间隔
        size_t mnCheckSoftwareUpdateIntervalTimeSec;
        //! 上传时间间隔
        size_t mnUploadIntervalTimeSec;
        std::string mstrFTPHost;
        std::string mstrUserName;
        std::string mstrUserPassword;
        std::string mstrHelpServiceIPAddr;
        std::wstring mstrTaskNumber;
        std::wstring mstrFTPRootPath;
        std::wstring mstrFTPLogRootPath;  
        S_JGW_HELP_VERSION_CONFIG msVersionConfig;
        S_JGW_HELP_INSTALL_XML_CONFIG msInstallXMLConfig;
        S_JGW_HELP_UPDATE_XML_CONFIG msUpdateXMLConfig;
        S_JGW_HELP_DOWNLOAD_THREAD_PARAM msDownloadThreadParam;
    } S_JGW_HELP_CONFIG,*LPS_JGW_HELP_CONFIG;

    enum E_JGW_HELP_AUTO_UPDATE_TYPE
    {
        E_HELP_RSP_ERROR_MSG = -3, //! 发送错误的相关消息
        E_HELP_RSP_FTP_CONNECT_TYPE = -2, //! FTP连接异常或者错误消息
        E_HELP_RSP_HEARTBEAT_TYPE = -1, //! 心跳包请求回应
        E_HELP_REQ_CHECK_SOFTWARE_UPDATE_TYPE = 0, //! 请求检查软件更新消息类型 软件更新分为安装包更新和更新包自动更新
        E_HELP_RSP_DOWNLOAD_TYPE = 1, //! 服务端回应下载更新消息
#if 0
        E_HELP_REQ_UPDATE_TYPE = 2, //! 客户端请求程序更新消息
#endif
        E_HELP_RSP_UPDATE_TYPE = 3, //! 客户端回应程序更新消息
    };
}