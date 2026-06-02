#pragma once
#ifdef LANGUAGETRANSLATIONPLUGIN_API
#undef LANGUAGETRANSLATIONPLUGIN_API
#endif

#ifdef LANGUAGETRANSLATIONPLUGIN_EXPORTS
#define LANGUAGETRANSLATIONPLUGIN_API __declspec(dllexport)
#else
#define LANGUAGETRANSLATIONPLUGIN_IMPORTS
#define LANGUAGETRANSLATIONPLUGIN_API __declspec(dllimport)
#endif

#ifdef LANGUAGETRANSLATIONPLUGIN_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"LanguageTranslationPlugin_d.lib")
#   else
#       pragma comment(lib,"LanguageTranslationPlugin.lib")
#   endif
#endif

enum E_STRING_TYPE
{
    E_LOAD_MESCONFIG_FAIL = 0,
    //! Prompt
    E_PROMPT_TYPE,
    //! Repeat operation
    E_APPLICATON_REPEAT_RUN_TYPE,
    //! Password encryption failed
    E_PASSWORD_ENCRYPTION_FAILED_TYPE,
    //! Password decryption failed
    E_PASSWORD_DECRYPTION_FAILED_TYPE,
    //! 子窗体初始化失败 Failed to initialize child form
    E_CHILDFORM_INIT_FAIL_TYPE,
    //! 控件加载失败 Control failed to load
    E_CONTROL_FAILED_TOLOAD,
    //! 视图初始化错误 View initialization error
    E_VIEW_INIT_ERROR,
    //! 视图布局文件加载错误 View layout file loading error
    E_VIEW_LAYOUT_FILE_LOADING_ERROR,
    //! 白班 day shift 
    E_DAY_SHIFT_TYPE,
    //! 晚班 Night shift 
    E_NIGHT_SHIFT_TYPE,
    //! 获取不到当前按钮对应的插件信息 The plug-in information corresponding to the current button is not available
    E_GET_PLUGIN_INFO_BUTTON_NOY_AVAILABLE,
    //! 未选择订单信息 Order information not selected
    E_NOT_SELECTED_TASK,
    //! 请选择当前测试订单信息 Please select the current test order information
    E_SELECT_CURRENT_ORDER_INFO,
    //! 初始化失败 Initialization failure
    E_INITIALIZATION_FAILUER,
    //! 插件初始化错误 Error in plug-in initialization
    E_PLUGIN_INITIALIZATION_ERROR,
    //! 当前账号有限期为：永久 The current account has a deadline of: permanent
    E_ACCOUNT_DEADLINE_PERMANENT,
    //! 当前账号登录已经过期,请重新登录账号 The current account login has expired, please login again
    E_ACCOUNT_LOGIN_EXPIREAD_RELOGIN,
    //! 当前账号有限期还剩下秒 Account Deadline
    E_ACCOUNT_DEADLINE_SEC,
    //! 当前账号有限期还剩下分 Account Deadline
    E_ACCOUNT_DEADLINE_MINUTE,
    //! 当前账号有限期还剩下时 Account Deadline
    E_ACCOUNT_DEADLINE_HOUR,
    //! 设备池创建失败 Device pool creation failed
    E_DEVICE_POOL_CREATION_FAILED,
    //! MES未知状态",L" MES在线模式",L" 未选择订单或者工单",L" 账号登录错误
    E_MES_UNKNOWN_STATUS_TYPE,
    //! 
    E_MES_ONLINE_MODE_TYPE,
    //! No order or work order selected
    E_NO_ORDER_OR_WORKORDER_SELECTED,
    //! Account login error
    E_MES_ACCOUNT_LOGIN_ERROR,
    //! off-line mode
    E_MES_OFFLINE_MODE,
    //! 异常 Exception
    E_EXCEPTION_ERROR_TYPE,
    //! 升级服务连接出现异常 An exception has occurred in the upgrade service connection
    E_HELP_SERVICE_CONNECTION_EXCEPTION,
    //! 无法连接到FTP服务器，请检查网络连接\n点击确认：退出当前程序，点击取消：忽略更新错误信息进入主程序
    E_HELP_CONNECT_FTP_ERROR,
    //！It is detected that there is a new version of the tool that needs to be updated (when the new version is updated, uninstall the old version first, and then install the new version)
    E_HELP_TOOL_UPDATE_INTSALL_VERSION_TIP,
    //! 版本更新
    E_HELP_INSTALL_VERSION_MSGTIP,
    //! 检测到工具包有更新
    E_HELP_TOOL_UPDATE_PACKAGE_VERSION_TIP,
    //! 工具包更新
    E_HELP_PACKAGE_VERSION_MSGTIP,
    //! 更新超时
    E_HELP_UPDATE_TIMEOUNT_TIP,
    //! 程序自动更新超时
    E_HELP_UPDATE_TIMEOUNT_TIPMSG,
    //! "是否关闭当前程序，或者忽略当前程序自动更新\n点击中止：关闭当前程序，\n点击忽略：忽略当前程序自动更新\n点击重试：继续进行程序更新
    E_HELP_CLOSE_UPDATE_APPLICATION,
    //! 是否关闭或者忽略自动程序更新
    E_HELP_CLOSE_IGNORE_UPDATE_TIPMSG,
    //! 请选择站点
    E_MES_SELECT_STATION_ID,
    //! Please select the site information corresponding to the current Barcode
    E_MES_BARCODE_STATIONID,
    //! 未获取到当前订单号段
    E_MES_NO_GET_TASKNUMBER,
    //！获取IMEI对应订单号失败
    E_MES_GET_IMEI_TASKNUMBER_FAIL,
    //！获取IMEI对应订单号失败
    E_MES_GET_SN_TASKNUMBER_FAIL,
    //! 获取订单号失败
    E_MES_GET_TASKNUMBER_FAIL,
    //! 当前工单号为空
    E_MES_WORKORDERNUMBER_IS_EMPTY,
    //! 根据当前工单号获取订单号失败
    E_MES_GET_WorkOrderNumber_TASKNUMBER_FAIL,
    //! MES 服务应用程序开启失败 The MES service application failed to start
    E_MES_SERVICE_APPLICATION_FAILED_TO_START,
    //! MES服务应用程序已经开启成功 The MES service application has been started successfully
    E_MES_SERVICE_APPLICATION_SUCCESSFULLY_TO_START,
    //! 正在开启MES服务应用程序,请等待... Starting the MES service application, please wait...
    E_MES_STARTING_SERVICE_PLEASE_WAIT,
    //! 请输入大于等于%d位字符且小于等于%d位字符" Please enter %d characters greater than or equal to and less than % D characters
    E_INPUT_GRETERTHAN_EQUAL_LESSTHAN_CHARACTERS,
    //! 输入字符前缀不相符,请重新输入前缀为:%s的字符 The input character prefix does not match. Please re-enter the character with the prefix %s
    E_INPUT_PREFIX_NOTMATCH_PLEASE_REINPUT,
    //! 无法计算当前号段校验值，请检查号段输入是否正确 The current segment verification value cannot be calculated. Please check whether the segment input is correct
    //! IMEI校验码生成失败
    E_INPUT_IMEI_VERIFICATION_FAIL,
    //! The current segment'S IMEI check code is: %s
    E_INPUT_IMEI_CHECKCODE,
    //! 子窗体初始化参数不能为空
    E_CHILDFORM_PARAM_IS_NOT_EMPTY,
    //! 自动更新服务应用程序开启失败
    E_HELP_SERVICE_APPLICATION_FAILED_TO_START,
    //! 自动更新服务应用程序已经开启成功
    E_HELP_SERVICE_APPLICATION_SUCCESSFULLY_TO_START,
    //! 正在开启自动更新服务应用程序,请等待
    E_HELP_STARTING_SERVICE_PLEASE_WAIT,
    //! 警告:当前MES系统处于离线模式
    E_MES_WARNING_OFFLINE_MODE,
    //! 警告:当前站点不经过MES流程 The current site does not go through the MES process
    E_MES_WARNING_STATION_NOT_PROCESS,
    //! Connecting to MES data Service
    E_MES_CONNECTING_DATA_SERVICE,
    //! 连接MES数据服务失败，请检查MES服务是否已经开启
    E_MES_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING,
    //! 连接MES数据服务成功
    E_MES_CONNECT_SUCCESSFULLY,
    //! 与MES 服务通讯失败 Communication with the MES service failed
    E_MES_CONNECT_SERVICE_FAILED,
    //! 正在连接WiFi服务...
    E_WIFI_CONNECTING_DATA_SERVICE,
    //! 连接WiFi服务失败，请检查WiFi服务是否已经开启
    E_WIFI_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING,
    //! 连接WiFi服务成功
    E_WIFI_CONNECT_SUCCESSFULLY,
    //！当前端口已经排序，请重新清空排序信息然，然后在进行排序 The current port has been sorted. Please clear the sorting information again and then sort again
    E_CPE_DOWNLOAD_PORTHASSORTED_RESORT,
    //! 当前排序已经完成是否进入下一Index排序 Whether the current sort has been completed to proceed to the next index sort
    E_CPE_DOWNLOAD_PORT_IS_SORT_NEXTINDEX_SORT,
    //! 当前线程Index超过范围,请检查当前端口是否未进行排序
    E_CPE_DOWNLOAD_INDEX_OUTOFRANGE_CHECK_ISSORT,
    //! 下载程序运行过程中已经有设备插入，不允许在设备插入后在修改配置文件，请重新打开下载程序在修改配置文件
    E_CPE_DOWNLOAD_NOT_ALLOW_CHANGE_CONFIG,
    //! 初始化配置失败，请检查配置是否正常,详情见Log
    E_TSE_INITFAIL_CHECKCONFIG_LOG,
    //! 加载TSE模块失败
    E_TSE_LOAD_MODULE_FAIL,
    //! 解析配置文件失败!!!(XML:%s) Failed to parse the configuration file
    E_XML_FAILED_PARSE_CONFIG,
    //! 当前配置文件无测试项目
    E_TSE_XMLCONFIG_NO_TEST_PROJECT,
    //! 请按照测试顺序测试，或者点击最后一项测试方法重置当前测试"
    E_TSE_TEST_INDEX_RULE,
    //! 文件不存在 ,file does not exist 
    E_TSE_FILE_NOT_EXIST
};

enum E_LOCAL_INDEX
{
    E_CHS_LOCAL = 0,
    E_EN_LOCAL 
};

#ifdef __cplusplus
extern "C" 
{
#endif
    //! 
    LANGUAGETRANSLATIONPLUGIN_API void InitLanguage(E_LOCAL_INDEX local);
    //! 
    LANGUAGETRANSLATIONPLUGIN_API const wchar_t* GetString(E_STRING_TYPE eStringType);
#ifdef __cplusplus
};
#endif