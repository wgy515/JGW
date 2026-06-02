#pragma once
#include <map>
//！ 多路下载最大支持线程数
#define DOWNLOAD_THREAD_MAX			8
#define SORT_PORT_CONFIG_NAME		L"SortRecord.ini"

#define PASS_PROCESS_COLOR	L"progress_fore.png"
#define PASS_STATUS_DOWN	L"#ff00ff00"

/*#define IDLE_STATUS_DOWN	L"#ff808000"*/

#define IDLE_STATUS_DOWN	L"#ffffff00"
#define IDEL_PROCESS_COLOR  L"progress_fore_gray.png"

#define FAIL_STATUS_DOWN	L"#ffff0000"
#define FAIL_PROCESS_COLOR	L"progress_fore_read.png"

namespace JGW
{
    //! 下载状态
    enum E_DOWNLOAD_STATUS
    {
        //! E_INIT_DOWNLOAD = 0;
        E_IDLE_DOWNLOAD,            //! 下载初始化状态
        E_BUSY_DOWNLOAD,           //! 下载中
        E_ERROR_DOWNLOAD,           //! 下载错误
        E_SUCCESSFUL_DOWNLOAD       //! 下载成功
    };

    typedef struct  
    {
        size_t mnIndex; //! 多路下载线程索引
        size_t mnProgressPos; //! 下载进度
        E_DOWNLOAD_STATUS meDownloadStatus; //! 下载状态
        E_DOWNLOAD_STATUS mePrevDownloadStatus;
    }S_MULTI_DOWNLOAD_VIEW_PARAM,*LPS_MULTI_DOWNLOAD_VIEW_PARAM;

	typedef struct  
	{
		size_t mnDownLoadIndex; //! 下载ID

		int mnDonwloadComPort; //! 下载端 口
		int mnDonwloadSortIndex; //! 下载排序索引

		int mnSerialComPort; //! 下载串口
		int mnSerialSortIndex; //! 串口排序索引

		std::wstring mstrCPEIPAddr; //! CPE设备IP地址
	}S_MULTI_DOWNLOAD_SORT_CONFIG,*LPS_MULTI_DOWNLOAD_SORT_CONFIG;

    typedef struct
    {
        size_t mnDownloadTime; //! 预计下载时间
        size_t mnDownloadTimeOut; //! 预计下载超时时间
		std::wstring mstrDownloadPath;
        //! std::wstring mstrCPEIPAddr;
        std::wstring mstrTFtpdIPAddr; //! TFTPD服务器IP地址，默认当前主机IP地址
        //std::wstring mstrSBL;
        //std::wstring mstrCDT;
        //std::wstring mstrTZ;
        //std::wstring mstrDEV;
        //std::wstring mstrRPM;
        //std::wstring mstrBOOT;
		std::wstring mstrCurrentWorkDir;
		std::wstring mstrDownloadProcessPath;
		std::wstring mstrTSESuiteConfigFilePath;
        std::wstring mstrDownloadParam;
        std::map<std::wstring,std::wstring> mmapParams;
        S_MULTI_DOWNLOAD_SORT_CONFIG msMultiDownloadSortConfigs[DOWNLOAD_THREAD_MAX];
    }S_MULTI_DOWNLOAD_CONFIG,*LPS_MULTI_DOWNLOAD_CONFIG;

    extern std::wstring g_strDownloadDubugLog[DOWNLOAD_THREAD_MAX];
}