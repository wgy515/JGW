#pragma once
#include <string>

#define SORT_PORT_CONFIG_NAME		L"SortRecord.ini"
#define CONFIG_DOWNLOAD_NAME		L"down.ini"
#define DOWNLOAD_THREAD_MAX         8
#define MAX_DOWNLOAD_THREAD_COUNT   (8)
#define PASS_PROCESS_COLOR	        L"progress_fore.png"
#define PASS_STATUS_DOWN	        L"#ff00ff00"
#define PROCESS_MAX_VALUE			100 * 100
/*#define IDLE_STATUS_DOWN	        L"#ff808000"*/

#define IDLE_STATUS_DOWN	        L"#ffffff00"
#define IDEL_PROCESS_COLOR          L"progress_fore_gray.png"

#define FAIL_STATUS_DOWN	        L"#ffff0000"
#define FAIL_PROCESS_COLOR	        L"progress_fore_read.png"

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

    enum E_GET_WORKTHREAD_ERROR
    {
        E_NO_BIND_THREAD = 99998,
        E_GET_INSTACNCEID_ERROR = 99999
    };

    typedef struct 
    {
        //! 下载完成后是否重启
        bool mbResetAfterDownload;
        //! 自动切换下载端口
        bool mbAutoSwitchDownloadPort;
        //! UFS 需要
        bool mbHaveProvisionXml;
        //! 预计下载时间
        int mnDownloadTimeSec;
        //! 下载超时时间
        int mnDownloadTimeOutSec;
        //! TSE 配置文件路径
        std::wstring mstrTSESuiteConfigPath;
        //! 下载进程路径
        std::wstring mstrDownloadProcessPath;
        //! 当前EXEC程序路径
        std::wstring mstrCurrentWorkDir;
        //! 下载端口排序文件路径
        std::wstring mstrDownloadPortSortConfigFilePath;
        //! 当前项目名称
        std::wstring mstrModeName;
        //! 下载路径
        std::wstring mstrDownloadPath;
        //! UFS初始化配置文件
        std::wstring mstrProvisionXml;
        //! firehose
        std::wstring mstrProgFirehoseLitembnFileName;
        //! rawprograms 文件
        std::wstring mstrRawprograms;
        //! patch文件
        std::wstring mstrPacths;
    }S_TSE_MULTIUNIT_DOWNLOAD_CONFIG,*LPS_TSE_MULTIUNIT_DOWNLOAD_CONFIG;

    struct S_DOWNLOAD_PROGRESS_INFO
    {
        E_DOWNLOAD_STATUS       meDownloadStatus;
        //E_DOWNLOAD_STATUS       mePrevDownloadStatus;
        int                     mnComPort;
        int                     mnPos;
        size_t					mnIndex;
        LPS_TSE_MULTIUNIT_DOWNLOAD_CONFIG mpsTSEMDConfig;
        //DWORD64                 mdw64SendCounts;
    };

    extern std::wstring g_strDownloadDubugLog[DOWNLOAD_THREAD_MAX];

    const wchar_t g_szSerialNumberDevice[] = L"adb -s %s get-state\n";
    const wchar_t g_szGetSerialNumber[] = L"adb devices\n";
    const wchar_t g_szGetState[] = L"adb -s %s get-state\n";
    const wchar_t g_szADBDevices[] = L"adb -s %s devices\n";
    const wchar_t g_szRebootbooloader[] = L"adb -s %s reboot-bootloader\n";
    const wchar_t g_szFastbootReboot[] = L"fastboot -s %s reboot\n";
    const wchar_t g_szFastbootDevices[] = L"fastboot -s %s devices\n";
    const wchar_t g_szSelectOemInfo[] = L"fastboot -s %s oem device-info\n";
    const wchar_t g_szOemUnLock[] = L"fastboot -s %s oem unlock-fdl\n";
    const wchar_t g_szFastBootDownCmd[] = L"fastboot flash %s \"%s\" -s %s\n";
    const wchar_t g_szFasterasePartion[] = L"fastboot erase %s -s %s\n";
    const wchar_t g_szADBSwitchEDL[] = L"adb -s %s reboot edl\n";
}