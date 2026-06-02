#pragma once

#define TEST_SUITE_MANAGE_PLUGIN_NAME "TSE_SequenceManagerPlugin.dll"
#define TEST_SUITE_MANAGE_CLASS_NAME "CTSE_SequenceManageExport"

#ifdef _DEBUG
#define HELP_SERVICE_APPLICATION_NAME  L"JaGuarWaveHelpV2_d.exe"
#else
#define HELP_SERVICE_APPLICATION_NAME  L"JaGuarWaveHelpV2.exe"
#endif

#define IDR_ZIPRES1                     129

//#define BLACK_COLOR 0x000000
//#define RED_COLOR 0xff0000
//#define GREEN_COLOR 0x00ff00
//#define BLUE_COLOR 0x0000ff
//#define YELLOW_COLOR 0xffff00

#define IDLE_TEXT_COLOR 0x00FFFBF0
#define IDLE_BK_COLOR 0xFFC0C0C0
#define BUSY_TEXT_COLOR 0x000000FF
#define BUSY_BK_COLOR 0xFFFFFF00
#define FAIL_TEXT_COLOR 0x00FFFBF0
#define FAIL_BK_COLOR 0xFFFF0000
#define PASS_TEXT_COLOR 0x00FFFFFF
#define PASS_BK_COLOR 0xFF008000


//! 异步消息池主窗口处理的定时器的事件间隔  MS
#define FIFO_EVENT_INTERVAL_TIME    100
//! 下载进度条更新时间间隔 
#define UPDATE_DOWNLOAD_PROGRESS_TIME 1000
//! 更新LOG信息时间
#define UPDATE_LOG_INFO_INTERVAL_TIME 500
//! 定时器
#define WM_TIMER_BASE 100
//! 响应子窗口点击定时器
#define WM_TIMER_CLICK_SUB_WINDOWS                  WM_TIMER_BASE
//! 更新时间显示定时器
#define WM_TIMER_UPDATE_TIME						(WM_TIMER_BASE + 1)
//! 更新测试时间定时器
#define WM_TIMER_UPDATE_TEST_TIME					(WM_TIMER_BASE + 2)
//! 模拟最大化事件 - > 
#define WM_TIMER_MAX_EVENT							(WM_TIMER_BASE + 3)
//! 先入先出事件处理 -> 异步消息池定时更新定时器ID
#define WM_TIME_FIFO_EVENT                          (WM_TIMER_BASE + 4)
//! 重测定时器 TDO: 有没有必要
#define WM_TIMER_RETEST_EVENT                       (WM_TIMER_BASE + 5)
//! 更新下载界面进度条
#define WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT     (WM_TIMER_BASE + 6)
//! 更新INFO信息
#define WM_TIMER_UPDATE_LOG_INFO                    (WM_TIMER_BASE + 7)
//!
#define WM_TIMER_WKE_BROWSER                        (WM_TIMER_BASE + 8)
//! 初始化MES Service
#define WM_TIMER_INIT_MES_SERVICE                   (WM_TIMER_BASE + 9)
//! 检查MES Service应用程序是否已经开启
#define WM_TIMER_CHECK_MES_APPLICATION_STATR        (WM_TIMER_BASE + 10)
//!
#define WM_TIMER_UPDATE_DEVICE_INFO                 (WM_TIMER_BASE + 11)
//! 自动下载更新定时器
#define WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP     (WM_TIMER_BASE + 12)
//! 检查自动更新 Service应用程序是否已经开启
#define WM_TIMER_CHECK_HELP_APPLICATION_STATR        (WM_TIMER_BASE + 13)
//! 自动上传LOG定时器
#define WM_TIMER_AUTO_UPLOAD_HELP                    (WM_TIMER_BASE + 14)
//! 上报station状态信息
#define WM_TIMER_REPORT_STATION_STATUS              (WM_TIMER_BASE + 15)      
//! 上报流量等相关信息，默认按照1s的速度上报
#define WM_TIMER_REPORT_TRAFFIC_SPEED_INFO          (WM_TIMER_BASE + 16)

//! 开启线程池消息
#define START_DEVICE_POOL_MSG	(WM_USER + 5)
//! 注册设备池消息 WPARAM 为CRegMsgType指针不能为空 
#define REG_DEVICEPOOL_MSG_DEFINE	(WM_USER + 6)
//! 添加设备池消息  WPARAM 为SDeviceMsg_Info结构体指针不能为空（已分配内存）
#define ADD_DEVICEPOOL_MSG_DEFINE	(WM_USER + 7)
//! 移除设备池消息 WPARAM 为需要移除窗口句柄 
#define REMOVE_DEVICEPOOL_MSG_DEFINE	(WM_USER + 8)
//! 移除当前插件注册的消息
#define REMOVE_PLUGIN_REG_DEVICEPOLL_DEFINE (WM_USER + 9)


//! 清楚设备池排序消息
#define CLEAR_SORT_DEVICEPOOL_MSG_DEFINE	(WM_USER + 10)

//! 响应设备池消息(插件窗口)  设备池发送设备插入拨出消息给窗口句柄消息ID
#define WM_RESPONCE_DEVICEPOOL_MSG_DEFINE			(WM_USER + 11)
//! 响应插件版本修改信息 由插件发送给主窗口
#define WM_RESPONCE_PLUGIN_VERSION_MSG_DEFINE		(WM_USER + 12)
//! 
//! #define WM_IMEIBURN_RESPONCE_DEVICEPOOL_MSG_DEFINE	(WM_USER + 13)
//! 响应子窗体点击消息 测试项目点击消息
#define WM_RESPONCE_CLICK_SUB_WINDOW_MSG			(WM_USER + 14)

#define WND_COM_USB_MSG				(WM_USER + 63 )

#define MANAGE_THREAD_START_MSG		(WM_USER + 50)
#define MANAGE_THREAD_END_MSG		(WM_USER + 51)

#define BUSY_DOWN_STATUS_WND_MSG	(WM_USER + 52)
#define PASS_DOWN_STATUS_WND_MSG	(WM_USER + 53)
#define FAIL_DOWN_STATUS_WND_MSG	(WM_USER + 54)
#define IDEL_DOWN_STATUS_WND_MSG	(WM_USER + 55)
#define ADD_PORCESS_DOWN_STATUS_WND_MSG	(WM_USER + 59)

#define DOWN_LOAD_THREAD_INIT_MSG		(WM_USER + 57)
#define DOWN_LOAD_THREAD_RUN_MSG		(WM_USER + 58)
//! 切换fastboot信息
#define SWITCH_FASTOOT_MODE_THREAD_MSG			(WM_USER + 60)
//! 切换下载模式9008
#define SWITCH_DOWNLOAD_MODE_THREAD_MSG			(WM_USER + 61)
//! EDL切换信息
#define SWITCH_EDL_MODE_THREAD_MSG				(WM_USER + 62)

#define THREAD_MSG_USB_INSERT					(WM_USER + 150)

//! 测试线程初始化消息
#define WM_TEST_THREAD_MSG_INIT					(WM_USER + 1000)
//! 测试线程开始测试消息
#define WM_TEST_THREAD_MSG_RUN					(WM_USER + 1001)
//! 测试线程
#define WM_TEST_THREAD_MSG_CLEANUP              (WM_USER + 1002)
//! 显示或者隐藏log信息
#define WM_HWND_VIEW_LOG_INFO                   (WM_USER + 1003)
//！当前线程测试成功消息
#define WM_TEST_PASS_MSG                        (WM_USER + 1004)
//! 当前线程测试失败消息
#define WM_TEST_FAIL_MSG                        (WM_USER + 1005)
//! 线程重测消息
#define WM_TEST_THREAD_MSG_RE_TEST              (WM_USER + 1008)
//! 线程开始测试消息
#define WM_THREAD_TEST_START_MSG                (WM_USER + 1009)
//! 测试线程加载配置消息
#define WM_TEST_THREAD_LOAD_CONFIG_MSG          (WM_USER + 1012)
//! 测试线程加载配置响应消息 WPARAM 1 成功 0: 失败
#define WM_WND_MSG_TO_RESPONSE_TEST_THREAD_LOAD_CONFIG_RESULT_MSG   (WM_USER + 1013)
//! 校准测试项目状态消息
#define WM_WND_CAL_TEST_PROJECT_STATUS_MSG      (WM_USER + 1014)
//! 校准测试信息 wParam 测试信息
#define WM_WND_CAL_TEST_INFO_MSG                (WM_USER + 1015)
//! TDO 未使用
//! #define WM_HWND_TEST_LIST_MSG						(WM_USER + 1016)
//! HWND 窗口开始测试消息 
#define WM_HWND_TEST_SATRT_MSG                  (WM_USER + 1017)
//!　TDO未使用
//! #define WM_HWND_TEST_INFO_MSG						(WM_USER + 1018)
//! HWND 窗口通用输入对话框消息
#define WM_HWND_INPUT_COMMON_WINDOW_MSG         (WM_USER + 1019)
//! HWND 测试列表信息成功消息
#define WM_HWND_TEST_LIST_INFO_OK_MSG           (WM_USER + 1020)
//! HWND 测试列表信息失败消息
#define WM_HWND_TEST_LIST_INFO_ERROR_MSG        (WM_USER + 1021)
//! 窗口点击配置按钮
#define WM_HWND_CONFIG_BUTTON_CLICK_MSG         (WM_USER + 1022)
//! 窗口点击清除测试记录按钮
#define WM_HWND_CLEAR_TEST_RECORD_BUTTON_CLICK_MSG  (WM_USER + 1023)
//!
#define WM_HWND_TEST_LIST_MSG						(WM_USER + 1024)
//! 
#define WM_HWND_RICHEDIT_APPEND_TEST_MSG            (WM_USER + 1025)
//! 关闭子窗体消息
#define WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS            (WM_USER + 1026)

#define WM_HWND_TX_RX_SECTOR_MSG                    (WM_USER + 1027)

#define WM_HWND_UPDATE_DONGLES_INFO                 (WM_USER + 1028)
//! 初始化TSE 失败
#define WM_HWND_INIT_TSE_TEST_FAIL_MSG              (WM_USER + 1029)
//！
#define WM_HWND_UPDATE_RSSI_INFO                    (WM_USER + 1030)
//！
#define WM_HWND_UPDATE_MCS_INFO                     (WM_USER + 1031)
//! 更新MES相关信息
#define WM_HWND_UPDATE_MES_INFO                     (WM_USER + 1032)
//! TSE更新，在LIST后面添加测试相关信息
#define WM_HWND_TSE_SHOW_CONTENT					(WM_USER + 1033)
//! TSE 自动测试
#define WM_TEST_THREAD_MSG_AUTO_TEST				(WM_USER + 1034)
//! TSE 视图配置测试
#define WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST			(WM_USER + 1035)
//! TSE序列ID
#define WM_HWND_TSE_INDEX_MSG				        (WM_USER + 1036)
//! TSE序列ID
#define WM_TEST_THREAD_TSE_INDEX_MSG_RUN		    (WM_USER + 1037)
//! TSE序列ID
#define WM_HWND_TSE_INDEX_MSG_RUN_COMPLETE	        (WM_USER + 1038)
//! 重置TSE
#define WM_TEST_THREAD_RESET_TSE	                (WM_USER + 1039)
//! 重置TSE 完成
#define WM_HWND_RESET_TSE_COMPLETE	                (WM_USER + 1040)
//! 更新BPS 数据
#define WM_HWND_UPDATE_BPS_INFO                     (WM_USER + 1041)
//! 清空BPS数据信息
#define WM_HWND_CLEAR_BPS_INFO                      (WM_USER + 1042)
//! 线程开始测试消息
#define WM_THREAD_TEST_STOP_MSG                     (WM_USER + 1043)
//! 是否显示当前测试状态
#define WM_HWND_IS_SHOW_TEST_STATUS_WINDOW_MSG		(WM_USER + 1044)





/************************************************/
//! 获取LOG
#define WM_THREAD_GET_LOG_LAN_PROT                 (WM_USER + 1044)


/**********************************************/
//! help msg
//! 下载请求消息
#define WM_THREAD_HELP_REQ_DOWNLOAD_MSG                 (WM_USER + 1045)  
//! 下载回馈消息
//#define WM_THREAD_HELP_RSP_DOWNLOAD_MSG                 (WM_USER + 1046)
//! 请求窗口更新消息
//#define WM_HWND_HELP_REQ_UPDATE_MSG                     (WM_USER + 1047)
//! 请求线程更新消息
#define WM_THREAD_HELP_REQ_UPDATE_MSG                   (WM_USER + 1048)
//! 客户端响应下载的消息
#define WM_HWND_RSP_HELP_DOWNLOAD_MSG                   (WM_USER + 1049)
//! 客户端响应更新的消息
#define WM_HWND_RSP_HELP_UPDATE_MSG                     (WM_USER + 1050)
//! 客户端响应不需要更新消息
#define WM_HWND_RSP_HELP_CLOSE_MSG                      (WM_USER + 1051)
//! 客户端响应连接出现异常消息
#define WM_HWND_RSP_HELP_CONNECT_ERROR_MSG              (WM_USER + 1052)

//! 客户端更新提示窗口关闭消息
#define WM_HWND_HELP_TIP_CLISE_MSG                      (WM_USER + 1053)
//! 打开自动下载更新定时器消息
#define WM_HWND_OPEN_DOWNLOAD_HELP_TIMER_MSG            (WM_USER + 1054)
//! 打开自动更新定时器消息
#define WM_HWND_OPEN_UPDATE_HELP_TIMER_MSG              (WM_USER + 1055)
//! 通知线程上传LOG信息
#define WM_THREAD_HELP_UPLOAD_LOG_MSG                   (WM_USER + 1056)
//! 打开自动上传LOG定时器
#define WM_HWND_OPEN_UPLOADLOG_HELP_TIMER_MSG           (WM_USER + 1057)
//! 客户端 FTP 连接出现异常消息
#define WM_HWND_RSP_HELP_FTP_CONNECT_ERROR_MSG          (WM_USER + 1058)
//! 请求线程下载消息
#define WM_HWND_HELP_REQ_DOWNLOAD_MSG                   (WM_USER + 1059)
//! 程序更新的错误消息
#define WM_HWND_RSP_HELP_ERROR_MSG                      (WM_USER + 1060)

//！ 上报
#define WM_HWND_REPORT_INFO_MSG                         (WM_USER + 1061)

//! 保存log check事件
#define WM_HWND_SAVE_TEST_LOG_SELECTCHANGE_MSG  (WM_USER + 1062)
//! showLogLayoutMenuElement
#define WM_HWND_SHOW_LOG_LAYOUT_CLICK_MSG       (WM_USER + 1063)

//!
#define WM_HWND_NETCARD_CHANGE_MSG              (WM_USER + 1064)
//! 保存金机校准数据
#define WM_THREAD_SAVE_GU_CAL_DATA              (WM_USER + 1065)
//! 金机校准相关结果回馈到主窗体
#define WM_HWND_SAVE_GU_CAL_DATA                (WM_USER + 1066)



//! CSharp Adapter View  更新C#适配窗口大小
#define WM_RESIZE_CSHARP_ADAPTER_VIEW					(WM_USER + 1067)
//! 关闭C#适配窗口
#define WM_CLOSE_CSHARP_ADAPTER_VIEW					(WM_USER + 1068)
//! C#窗口句柄发送给C++窗口消息
#define WM_CSHARP_ADAPTER_VIEW_REPORT_HWND              (WM_USER + 1069)
////! c++ 窗口开始测试消息传递给C#窗口
//#define WM_START_TEST_MSG_TO_CSHARP_ADAPTER_VIEW		(WM_USER + 1070)
////! C++ 窗口点击CONFIG按钮消息传递给C#窗口
//#define WM_CLICK_CONFIG_MSG_TO_CSHARP_ADAPTER_VIEW		(WM_USER + 1071)
//! 发送给C#窗口显示输入对话框消息
#define WM_SHOW_INPUT_WINDOW_CSHARP_ADPATER				(WM_USER + 1070)


//! FTP下载消息 WPARAM : FTP下载路径 LPARAM : 本地保存路径
#define WM_THREAD_FTP_DOWNLOAD_MSG                      (WM_USER + 1071)
//! FTP上传消息 WPARAM : FTP上传路径 LPARAM : 本地上传文件路径
#define WM_THREAD_FTP_UPLOAD_MSG                        (WM_USER + 1072)
//! Operation complete 操作完成消息 WPARAM : 消息完成标识(字符串)
#define WM_HWMD_OPERATION_COMPLETE_MSG                  (WM_USER + 1073)
//! 
#define WM_THREAD_ASYNC_FUNC_MSG                        (WM_USER + 1074)
//!
#define WM_HWND_TEST_PASS_MSG                           (WM_USER + 1075)
//!
#define WM_HWND_TEST_FAIL_MSG                           (WM_USER + 1076)
//! 开启上传测试线程
#define WM_THREAD_START_UPDALODING_TEST_THREAD          (WM_USER + 1077)
//! UploadingTestLogsThreadLoop
#define WM_THREAD_UPLODING_TEST_LOG_LOOP                (WM_USER + 1078)
//! OnSaveRichEditTestLog
#define WM_HWMD_SAVE_RICHEDIT_TEST_LOG_MSG              (WM_USER + 1079)


//! PLUGIN msg => JaGuarWaveTestPlatform => JaGuarWaveTestPlatformHelp => JaGuarWaveHelpV3(通过PLUGIN 线程ID转发) => PLUGIN msg
//#define WM_HWND_MES_STATUS_CHANGE_MSG                   (WM_USER + 1080)

/***********************JGW_PluginApp 与 JaGuarWaveTestPlatform 通讯消息ID**********************************************************/
//! JaGuarWaveTestPlatform => PLUGIN msg
#define WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID               (WM_USER + 1080)
//! PLUGIN => JaGuarWaveTestPlatform msg
#define WM_PLUGIN_APP_TO_MAIN_HWND_MSG_ID               (WM_USER + 1081)
//! 插件向主窗口发送句柄信息
#define WM_PLUGIN_APP_HWND_MSG_TO_MAIN_HWND_MSG_ID      (WM_USER + 1082)
//! WPARAM === 更新MES状态信息
#define WM_UPDATE_MES_STATUS_MSG_WPARAM                         1
//! WPARAM === 开启远程日志信息
#define WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM                   2

#if 0
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_1                      2
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_2                      3     
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_3                      4     
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_4                      5     
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_5                      6     
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_6                      7    
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_7                      8    
//! 发送插件句柄到JaGuarWaveTestPlatform
#define WM_UPDATE_PLUGIN_APP_HWND_WPARAM_8                      9    
#endif
//! 发送CLOSE消息到子插件句柄
//#define WM_SEND_CLOSE_MSG_TO_PLUGIN_APP_WPARAM                  10

