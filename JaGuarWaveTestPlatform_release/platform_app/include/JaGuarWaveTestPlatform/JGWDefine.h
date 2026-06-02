#pragma once
#include <string>
#include <vector>
#include <map>
#include "..\..\..\platform_include\JGW_MSG_ID_Define.h"

//! 重复加载插件次数
#define RELOAD_PLUGIN_COUNTS 3

#define USB_CONFIG_INI_FILE_NAME "USB_Config.ini"

#define GET_INFO_WORK_ORDER_NUMBER

typedef void* LPVOID ;
#if 0
//! 项目按键布局
typedef struct 
{
	std::wstring m_strImageHeight;
	std::wstring m_strImageWeight;
	std::wstring m_strLabelHeight;
	std::wstring m_strLabelTextColor;
	std::wstring m_strHeight;
	std::wstring m_strFontID;
}S_PROJECT_BUTTON_LAYOUT,*LPS_PROJECT_BUTTON_LAYOUT;
//!　按键图片
typedef struct 
{
	std::wstring mstrNormalImage; //! 
	std::wstring mstrHotImage;//! 热点图片
	std::wstring mstrPushedImage;//! 按下图片
	std::wstring mstrFocusedImage;//! 焦点图片
	std::wstring mstrDisabledImage;//! 禁用图片
}S_BUTTON_IMAGE_,*LPS_BUTTON_IMAGE_;

typedef std::vector<S_BUTTON_IMAGE_> _vecButtonImage;
//! 分组按键布局
typedef struct 
{
	std::wstring		m_strHeight;
	std::wstring		m_strFontID;
	std::wstring		m_strTextColor;
	S_BUTTON_IMAGE_		mSButtonImage;
}S_GROUP_BUTTON_LAYOUT,*LPS_GROUP_BUTTON_LAYOUT;

typedef struct  
{
    std::wstring mstrClsid;
    std::wstring mstrModuleName;
}S_POCO_CLASS_LOADER_INFO,*LPS_POCO_CLASS_LOADER_INFO;
//S_DEVICE_POOL_MODULE_INFO,*LPS_DEVICE_POOL_MODULE_INFO;

typedef S_POCO_CLASS_LOADER_INFO S_DEVICE_POOL_MODULE_INFO;
typedef LPS_POCO_CLASS_LOADER_INFO LPS_DEVICE_POOL_MODULE_INFO;

typedef struct  
{
	std::wstring mstrViewLoaderPluginName;
	std::map<int,std::wstring> mID_ClsidMap;
	//! std::wstring mstrViewLoaderPluginClsid;
}S_VIEW_LOAD_KEY_VALUE,LPS_VIEW_LOAD_KEY_VALUE;
//! 试图加载器信息
typedef struct 
{
	std::vector<S_VIEW_LOAD_KEY_VALUE> mvViewLoadKey_Value;
}S_VIEWLOADER_INFO,*LPS_VIEWRESOLVER_INFO;

//! 插件信息
typedef struct  
{
	//! 保存视图加载器信息
	LPS_VIEWRESOLVER_INFO mpViewLaoderInfo;
	int					mImageGroupID;
	int					mnViewLoadID; //! 解析器插件视图加载ID
    std::wstring		mStationID; //! 站点ID
	std::wstring		mstrClsid; //! 注册类名
	std::wstring		mstrModuleFolderName;//! 模块目录名称
	std::wstring		mstrStationName; //! 站点按钮名称
	std::wstring		mstrPluginName; //! 插件名称
}S_PLUGIN_INFO,*LPS_PLUGIN_INFO;

//typedef std::vector<S_SUB_WINDOW_INFO> S_PLUGIN_GROUP_VECTOR;
//! 插件组信息
struct S_PLUGIN_GROUP_INFO
{
	std::vector<S_PLUGIN_INFO>		mvSubWinInfo;
	std::wstring					mnGroupID;
	std::wstring					mstrGroupName;
};

typedef std::vector<S_PLUGIN_GROUP_INFO> _vplugininfo,*_vlpPlugininfo;
#endif
//! MES 模式，在线或者离线模式
enum E_MES_MODE
{
    E_NOTONLINE_MES = 0, //! 离线模式
    E_ONLINE_MES, //! 在线模式  
};
//! MES的相关状态
enum E_MES_STATUS
{
    E_MES_UNKNOWN_STATUS = 0,  //! 初始化状态
    E_MES_OK_STATUS = 1, //! 登录正常
    E_MES_TASK_ERROR_STATUS,
    //! E_MES_INIT_STATUS, //! MES初始化状态
    //! E_MES_STATUS_TASK_ERROR, //! 未选择订单
    E_MES_LOGIN_ERROR_STATUS, //! 登录错误
};
// enum E_MES_STATUS
// {
//     E_MES_STATUS_OK = 0, //! 登录正常
//     E_MES_INIT_STATUS, //! MES初始化状态
//     E_MES_STATUS_NOTONLINE, //! 离线模式
//     E_MES_STATUS_TASK_ERROR, //! 未选择订单
//     E_MES_LOGIN_ERROR, //! 登录错误
// };

//const wchar_t gstrMESErrorStatus[][20] = {L" MES未知状态",L" MES在线模式",L" 未选择订单或者工单",L" 账号登录错误"};

const int gdwSaveAccountTimes[] = {1,2,4,8,12,24,-1};

//! MES账号信息
typedef struct
{
    E_MES_MODE meMESModel; //! 在线或者离线模式  DEF:默认在线模式
    E_MES_STATUS meMESStatus; //! MES操作相关状态
    //! E_MES_LOGIN_MODE meMESLoginMode;
    //! E_MES_STATUS meMESStatus;
    bool mbSaveAccount; //! 是否保存账号
    size_t mnMESServiceSocketPort; //! MES 服务SOCKET端口
    size_t mnMESConnectTimeOut; //! MES连接超时时间设置 S
    size_t mnMESSendTimeOut; //! MES发送超时 S
    size_t mnMESRecvTimeOut; //! MES接收超时 S
    size_t mnSaveAccountTimeStamp; //! 保存时间戳
    int mnSaveAccountTime; //! 以小时为单位
    std::string mstrUsername; //! MES用户名
    std::string mstrUserpwd; //! MES用户密码
    std::string mstrMESServiceIP; //! MES 服务端地址
    std::wstring mstrMESConfigFilePath;//! MES INI 配置文件路径
    std::wstring mstrMESWorkStageConfig;//! MES 站点相关信息配置文件路径
}S_MES_CONNECT_INFO,*LPS_MES_CONNECT_INFO;

typedef struct
{
    std::string mstrHost;
    std::string mstrUsername; //! MES用户名
    std::string mstrUserpwd; //! MES用户密码
}S_FTP_CONNECT_INFO,*PS_FTP_CONNECT_INFO;

typedef struct
{
    //! std::string mstrID;
    std::wstring mstrWorkStageName;
    std::wstring mstrWorkStageDes;
}S_MSE_WORKSTAGE,*LPS_MSE_WORKSTAGE;

typedef struct
{
    bool mbOpenCSTSEServerModule;
    int mnCSTSEServerSocketPort;
}S_CSTSE_SERVER_CONFIG;

typedef struct
{
    //! 程序退出的时候，强制关闭进程
    bool mbIsForceTerminateProcess;
    bool mbAutoUpdate;
	std::wstring mstrFTPLogBackGroundExec;
    std::string mstrTaskNumber;//! 订单号
    std::string mstrWorkOrderNumber; //! 工单号
    std::string mstrOperationId; //! 工序ID
    std::string mstrShiftID;//! 班次
    S_MES_CONNECT_INFO msMesConnectInfo;
    S_CSTSE_SERVER_CONFIG msCSTSEServerConfig;
    std::map<std::wstring,S_FTP_CONNECT_INFO> mmapFTPConnectInfo; //! ftp 连接信息List
    std::map<std::string,S_MSE_WORKSTAGE> mmapMESWorkStage; //! 站点ID 对应的站点休息
}S_MES_INFO,*LPS_MES_INFO;

typedef struct
{
	unsigned int muiDevicePoolThreadID;
    void* masync_message_class;
    void* mpPluginPaintManager;
    LPS_MES_INFO mpsMesInfo;//! MES相关信息结构体
}S_COMMON_WND_MSG_INFO,*LPS_COMMON_WND_MSG_INFO;

typedef struct  
{
    unsigned int mnShow;
    std::wstring mstrMenuElementName;
    std::wstring mstrMenuElementText;
    std::wstring mstrMenuElementExec;
}S_EXEC_MENUELEMENT_INFO;

// typedef struct
// {
// 	LPVOID	mpParentContainer;
// 	LPVOID	mpPaintMangerUI;
// 	LPVOID	mpPluginInfo;
// 	LPVOID	mpViewResolverClassLoader;
// }S_SUB_WND_INFO,*LPS_SUB_WND_INFO;
// #define GREEN	RGB(0, 125, 125)
// #define RED		RGB(250,0,0)
// #define BULE	RGB(125,125,125)
// #define BALCK	RGB(0,0,250)

