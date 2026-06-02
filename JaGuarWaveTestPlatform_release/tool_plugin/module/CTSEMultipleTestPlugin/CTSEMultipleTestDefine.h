#pragma once
#include <map>
#include <JGW_ReportDeviceInfo/CJGW_ReportDeviceInfoDefine.h>
#include "CJGW_TestLogLock.h"

//! tsepm.config E:\工具包\工厂测试工具包\NC700\JGWStart\JaGuarWaveTestPlatform_V20210701\exec\cache\NC700\upgrade_http_multiunit\tsepm.config
//#define PASS_PROCESS_COLOR	L"progress_fore.png"
//#define PASS_STATUS_DOWN	L"#ff00ff00"
//
///*#define IDLE_STATUS_DOWN	L"#ff808000"*/
//
//#define IDLE_STATUS_DOWN	L"#ffffff00"
//#define IDEL_PROCESS_COLOR  L"progress_fore_gray.png"
//
//#define FAIL_STATUS_DOWN	L"#ffff0000"
//#define FAIL_PROCESS_COLOR	L"progress_fore_read.png"

#define IDLE_PROCESS_IMG L"progress_fore.png"
#define IDLE_STATUS_BKCOLOR L"#ffffffa0"

#define BUSY_PROCESS_IMG L"slider_fg.png"
#define BUSY_PROCESS_IMG_STRING L"file='slider_fg.png' corner='5,5,5,5';"
#define BUSY_STATUS_BKCOLOR L"#ffa0c0ff"


#define FAIL_PROCESS_IMG L"progress_fore_read.png"
#define FAIL_STATUS_BKCOLOR L"#ffff0000"

#define PASS_PROCESS_IMG L"progress_fore.png"
#define PASS_STATUS_BKCOLOR L"#ff00ff00"

namespace JGW
{
    //! 下载状态
    enum E_TSEMP_TEST_STATUS
    {
        E_TSEMP_UNKNOWN,
        //! 下载初始化状态
        E_TSEMP_IDLE,       
        //! 设备当前插入状态
        E_TSEMP_INST, 
        //! 运行
        E_TSEMP_BUSY, 
        //! 错误
        E_TSEMP_FAIL,  
        //! 成功
        E_TSEMP_PASS       
    };

    typedef struct
    {
        //! ANSI 主机IP地址
        std::string mstrHostIPAddrAnsi;
        //！ 主机IP地址
        std::wstring mstrHostIPAddr;
        //！ 主机IP地址段
        std::wstring mstrHostIPSeg;
    }S_NETCARD_CONFIG,*PS_NETCARD_CONFIG;

    typedef struct
    {
        //! 适配器GUID
        std::wstring mstrNetGUID;
        //! 网卡IP地址
        std::wstring mstrIPAddr;
    }S_BIND_NETCARD_INFO,*PS_BIND_NETCARD_INFO;

    typedef struct
    {
        //! 端口号段信息
        std::wstring mstrComPort;
        //! ADB SERIAL NUMBER
        std::wstring mstrAdbSerialNumber;
    }S_BIND_USBDEV_INFO,*PS_BIND_USBDEV_INFO;

    enum E_GSTSE_ENVNAME
    {
        E_GSTSE_ISEUSEDEV = 0,
        E_GSTSE_BINARY_IMAGEISFOLDER,
        E_GSTSE_ISSAVETEST_LOG,
        E_GSTSE_SAVE_TESTLOG_FOLDER,
        E_GSTSE_BINARYIMAGEPATH,
        E_GSTSE_UPGRADE_VERSION,
        E_GSTSE_IP_ADDRESS,
        E_GSTSE_NETCARD_GUID,
        E_GSTSE_COM_PORT,
        E_GSTSE_ADB_SERIALNUMBER,
        E_GSTSE_SUITE_CONFIG_FILEPATH,
        E_GSTSE_HOST_ADDRESS
    };

    //! TSE Envrionment Name Array
    const wchar_t gsTSEEnvrionmentName[][36] ={
        L"TSE_IS_USEDEVICE",L"TSE_BINARY_IMAGEISFOLDER",L"TSE_ISSAVETEST_LOG",
        L"TSE_SAVE_TESTLOG_FOLDER",L"TSE_BINARYIMAGEPATH",L"TSE_UPGRADE_VERSION",
        L"TSE_IP_ADDRESS",L"TSE_NETCARD_GUID",L"TSE_COM_PORT",L"TSE_ADB_SERIALNUMBER",
        L"TSE_SUITE_CONFIG_FILEPATH",L"TSE_HOST_ADDRESS"
    };

    typedef struct  
    {
        //! 是否是下载或者升级进程
        bool mbIsDownloadUpgrade;
        //! 是否是USB设备或者网卡设备，true USB设备，false 网卡设备
        bool mbIsUsbDevice;
        //! 下载镜像是否是目录
        bool mbBinaryImageIsFolder;
        //! 下载或者升级操作是否是文件名
        bool mbDownloadUpgradeTSEParamIsFileName;
        //! 是否保存测试LOG
        bool mbSaveTestLog;
        //! 是否清空排序
        bool mbClearSortIndex;
        //! 多路测试几行
        int mnMultipleTestRow;
        //！每行多少列
        int mnMultipleTestCol;
        //! 下载时间
        size_t mnDownloadTimeSec;
        //! 下载出现超时时间
        size_t mnDownloadTimeOutSec;
        //！保存测试log目录
        std::wstring mstrSaveTestLogFolder;
        //! 下载镜像文件路径 => 路径或者目录
        std::wstring mstrBinaryImagePath;
        //! 升级或者下载版本
        std::wstring mstrUpgradeVersion;
        //! TSE 测试程序文件路径
        std::wstring mstrTSEExecApplicationFilePath;
        //! TSE 测试程序文件当前工作目录
        std::wstring mstrTSEExecApplicationCurrentWorkDir;
        //! TSE 配置文件路径
        std::wstring mstrTSESuiteConfigFilePath;
        //! 下载或者升级TSE参数 => 涉及文件相关信息
        std::wstring mstrDownloadUpgradeFileTSEParam;
        //! TSE 原始参数
        std::wstring mstrSourceTSEParam;
        //! TSE 原始参数 对应的参数常量
        std::map<E_GSTSE_ENVNAME,std::wstring> mmapeSourceTSEParam;
        //! 网卡设备相关配置
        S_NETCARD_CONFIG msNetCardConfig;
    }S_MULTIPLE_CONFIG,*PS_MULTIPLE_CONFIG;


    typedef struct
    {
        //! 网卡状态更新标识
        //! bool mbNetCardStatusUpdateFlag;
        //! 完成TSE 相关测试
        bool mbIsFinishedTSEMultipleTest;
        //! 测试状态
        E_TSEMP_TEST_STATUS meTSEMPTestStatus;
        //!
        E_TSEMP_TEST_STATUS mePrevTSEMPTestStatus;
        //!
        LPVOID mpMultipleTestThread;
        //! 多路线程排序索引  >= 1
        int mnSortIndex;
        //! 插件通用配置
        PS_MULTIPLE_CONFIG mpsMultipleConfig;
        //! TSE 配置文件路径
        std::wstring mstrTSESuiteConfigFilePath;
        //! TSE 测试LOG
        CCJGW_TestLogLock mstrTSEMultipleTestLog;
        //! TSE全局环境变量参数 => TSE_PARAM=XX TSE_PARAM1=XX等相关格式
        std::wstring mstrTSEParam;
        //! 绑定的网卡相关信息
        //S_BIND_NETCARD_INFO msBindNetCardInfo;
        //! 绑定的USB设备相关信息
        S_BIND_USBDEV_INFO msBindUsbDevInfo;
        //! 绑定的网卡相关信息
        PS_NETCARD_INFO mpsBindNetCardInfo;
    }S_TSE_MULTIPLE_TEST_THREAD_PARAM,*PS_TSE_MULTIPLE_TEST_THREAD_PARAM;
    //! ParamName => 
    //! ParamValue =>
}