#pragma once
#include <string>
#define DOWNLOAD_THREAD_MAX			8

#define PASS_PROCESS_COLOR	L"progress_fore.png"
#define PASS_STATUS_DOWN	L"#ff00ff00"

#define IDLE_STATUS_BKCOLOR	L"#FF008080"

#define INST_STATUS_BKCOLOR L"#ff808000"

#define IDLE_STATUS_DOWN	L"#ffffff00"
#define IDEL_PROCESS_COLOR  L"progress_fore_gray.png"

#define FAIL_STATUS_DOWN	L"#ffff0000"
#define FAIL_PROCESS_COLOR	L"progress_fore_read.png"

#define LAN_SORT_FILE_NAME L"LanSortConfig.ini"

#define dbg_debug(log_level,nIndex, fmt, ...) //! MyLog(log_level,nIndex, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)

#define dbg(log_level,nIndex, fmt, ...) MyLog(log_level,nIndex, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)

namespace JGW
{
    typedef struct  
    {
        //! 下载文件是否是目录
        bool mbIsDownloadFolder;
        //! TSE参数配置是文件名称
        bool mbTSEParamIsFileName;
        //! 是否保存下载log
        bool mbSaveDownloadLog;
        size_t mnDownloadTime;
        size_t mnDownloadTimeOut;
        std::string mstrHostIPAddrAnsi;
        std::wstring mstrDownloadFilePath;
        std::wstring mstrHostIPAddr;
        std::wstring mstrHostIPSeg;
        std::wstring mstrUpgradeVersion;
        std::wstring mstrSaveDownloadLogFolder;
        //! 下载进程路径
        std::wstring mstrDownloadProcessPath;
        //! 当前EXEC程序路径
        std::wstring mstrCurrentWorkDir;
        //!
        std::wstring mstrTSESuiteConfigFilePath;
        //!
        std::wstring mstrFolderTSEParam;
    }S_APS_MULTI_UPGRADE_CONFIG,*LPS_APS_MULTI_UPGRADE_CONFIG;

    //! 下载状态
    enum E_DOWNLOAD_STATUS
    {
        //! E_INIT_DOWNLOAD = 0;
        E_IDLE_DOWNLOAD,            //! 下载初始化状态
        E_INST_DOWNLOAD,
        E_BUSY_DOWNLOAD,           //! 下载中
        E_ERROR_DOWNLOAD,           //! 下载错误
        E_SUCCESSFUL_DOWNLOAD       //! 下载成功
    };

    //! LAN口状态
    enum E_NET_CARD_STATUS
    {
        ENETCARD_CONNECT_STATUS = 0, //!  连接状态
        ENETCARD_DISCONNCT_STATUS //! 非连接状态
    };

    typedef struct  
    {
        bool mbNetCardStatusUpdateFlag;
        bool mbIsFinishedDownloading;
        E_NET_CARD_STATUS meNetCardStatus;
        E_NET_CARD_STATUS mePrvFinishedDownloadingNetCardStatus; //! 下载完成后第一次网卡状态
        E_DOWNLOAD_STATUS mePrevDownloadStatus; //! 下载状态
        E_DOWNLOAD_STATUS meDownloadStatus; //! 下载状态
        size_t mnIndex;
        size_t mnProgressPos;
        std::wstring mstrIPAddr; //! IP 地址
        std::wstring mstrDescription;//! 描述信息
        std::wstring mstrAdapterName; //! 适配器名称
        std::wstring mstrNetName;
    }S_NET_CARD_PARAM,*LPS_NET_CARD_PARAM;

    enum LOG_TYPES
    {
        LOG_ERROR = 1,
        LOG_ALWAYS,
        LOG_WARN,
        LOG_INFO,
        LOG_DEBUG,  // usually only goes to log, unless VerboseLevel==LOG_DEBUG
        LOG_ONLY  // always only goes to log, such as hex dump
    };

    extern void MyLog (int log_level,int nIndex, const wchar_t *func_name, int line_number, const wchar_t *format, ...);

    extern std::wstring g_strDownloadDubugLog[DOWNLOAD_THREAD_MAX];
}