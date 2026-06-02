// LanguageTranslationPlugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

const wchar_t* GetStringChs(E_STRING_TYPE eStringType)
{
    switch (eStringType)
    {
    case E_LOAD_MESCONFIG_FAIL:
        return L"MES配置文件加载失败";
    case E_PROMPT_TYPE:
        return L"提示";
    case E_APPLICATON_REPEAT_RUN_TYPE:
        return L"程序正在在运行中,是否等待程序正常退出!\n1、请等10S左右在进行打开程序\n2、在任务管理起中关闭此进程\n3、点击取消按钮，程序自动关闭前面实例";
    case E_PASSWORD_ENCRYPTION_FAILED_TYPE:
        return L"密码加密失败";
    case E_PASSWORD_DECRYPTION_FAILED_TYPE:
        return L"密码解密失败";
    case E_CHILDFORM_INIT_FAIL_TYPE:
        return L"子窗体初始化失败";
    case E_CONTROL_FAILED_TOLOAD:
        return L"控件加载失败";
    case E_VIEW_INIT_ERROR:
        return L"视图初始化错误";
    case E_VIEW_LAYOUT_FILE_LOADING_ERROR:
        return L"视图布局文件加载错误";
    case E_DAY_SHIFT_TYPE:
        return L"白班";
    case E_NIGHT_SHIFT_TYPE:
        return L"晚班";
    case E_GET_PLUGIN_INFO_BUTTON_NOY_AVAILABLE:
        return L"获取不到当前按钮对应的插件信息";
    case E_NOT_SELECTED_TASK:
        return L"未选择订单信息";
    case E_SELECT_CURRENT_ORDER_INFO:
        return L"请选择当前测试订单信息";
    case E_INITIALIZATION_FAILUER:
        return L"初始化失败";
    case E_PLUGIN_INITIALIZATION_ERROR:
        return L"插件初始化错误";
    case E_ACCOUNT_DEADLINE_PERMANENT:
        return L"当前账号有限期为：永久";
    case E_ACCOUNT_LOGIN_EXPIREAD_RELOGIN:
        return L"当前账号登录已经过期,请重新登录账号";
    case E_ACCOUNT_DEADLINE_SEC:
        return L"当前账号有限期还剩下：%d秒";
    case E_ACCOUNT_DEADLINE_MINUTE:
        return L"当前账号有限期还剩下：%d分";
    case E_ACCOUNT_DEADLINE_HOUR:
        return L"当前账号有限期还剩下：%d小时%d分";
    case E_DEVICE_POOL_CREATION_FAILED:
        return L"设备池创建失败";
    case E_MES_UNKNOWN_STATUS_TYPE:
        return L" MES未知状态";
    case E_MES_ONLINE_MODE_TYPE:
        return L" MES在线模式";
    case E_NO_ORDER_OR_WORKORDER_SELECTED:
        return L" 未选择订单或者工单";
    case E_MES_ACCOUNT_LOGIN_ERROR:
        return L" 账号登录错误";
    case E_MES_OFFLINE_MODE:
        return L" MES离线模式";
    case E_EXCEPTION_ERROR_TYPE:
        return L"异常";
    case E_HELP_SERVICE_CONNECTION_EXCEPTION:
        return L"升级服务连接出现异常";
    case E_HELP_CONNECT_FTP_ERROR:
        return L"无法连接到FTP服务器，请检查网络连接\n点击确认：退出当前程序，点击取消：忽略更新错误信息进入主程序";
    case E_HELP_TOOL_UPDATE_INTSALL_VERSION_TIP:
        return L"检测到工具有新版本需要更新（新版本更新的时候，先卸载掉旧版本，然后在进行新版本的安装）";
    case E_HELP_INSTALL_VERSION_MSGTIP:
        return L"安装版本更新";
    case E_HELP_TOOL_UPDATE_PACKAGE_VERSION_TIP:
        return L"检测到工具包有更新（工具包更新的时候，会关闭掉当前测试工具平台，然后进行工具包更新）";
    case E_HELP_PACKAGE_VERSION_MSGTIP:
        return L"工具包更新";
    case E_HELP_UPDATE_TIMEOUNT_TIP:
        return L"程序自动更新出现超时，请确认是否是下载速度过慢或者是其他异常导致!!!\n点击确认：退出当前程序，点击取消：继续等待程序下载更新";
    case E_HELP_UPDATE_TIMEOUNT_TIPMSG:
        return L"程序自动更新超时";
    case E_HELP_CLOSE_UPDATE_APPLICATION:
        return L"是否关闭当前程序，或者忽略当前程序自动更新\n点击中止：关闭当前程序，\n点击忽略：忽略当前程序自动更新\n点击重试：继续进行程序更新";
    case E_HELP_CLOSE_IGNORE_UPDATE_TIPMSG:
        return L"是否关闭或者忽略自动程序更新";
    case E_MES_SELECT_STATION_ID:
        return L"请选择站点";
    case E_MES_BARCODE_STATIONID:
        return L"请选择当前板号对应的站点信息!";
    case E_MES_NO_GET_TASKNUMBER:
        return L"未获取到当前订单号";
    case E_MES_GET_IMEI_TASKNUMBER_FAIL:
        return L"获取IMEI对应订单号失败";
    case E_MES_GET_SN_TASKNUMBER_FAIL:
        return L"获取SN对应订单号失败";
    case E_MES_GET_TASKNUMBER_FAIL:
        return L"获取订单号失败";
    case E_MES_GET_WorkOrderNumber_TASKNUMBER_FAIL:
        return L"根据当前工单号获取订单号失败";
    case E_MES_SERVICE_APPLICATION_FAILED_TO_START:
        return L"MES 服务应用程序开启失败";
    case E_MES_SERVICE_APPLICATION_SUCCESSFULLY_TO_START:
        return L"MES服务应用程序已经开启成功";
    case E_MES_STARTING_SERVICE_PLEASE_WAIT:
        return L"正在开启MES服务应用程序,请等待...";
    case E_INPUT_GRETERTHAN_EQUAL_LESSTHAN_CHARACTERS:
        return L"请输入大于等于%d位字符且小于等于%d位字符";
    case E_INPUT_PREFIX_NOTMATCH_PLEASE_REINPUT:
        return L"输入字符前缀不相符,请重新输入前缀为:%s的字符";
    case E_INPUT_IMEI_VERIFICATION_FAIL:
        return L"IMEI校验码生成失败";
    case E_INPUT_IMEI_CHECKCODE:
        return L"IMEI校验码为：%s";
    case E_MES_WORKORDERNUMBER_IS_EMPTY:
        return L"当前工单号为空";
    case E_CHILDFORM_PARAM_IS_NOT_EMPTY:
        return L"子窗体初始化参数不能为空";
    case E_HELP_SERVICE_APPLICATION_FAILED_TO_START:
        return L"自动更新服务应用程序开启失败";
    case E_HELP_SERVICE_APPLICATION_SUCCESSFULLY_TO_START:
        return L"自动更新服务应用程序已经开启成功";
    case E_HELP_STARTING_SERVICE_PLEASE_WAIT:
        return L"正在开启自动更新服务应用程序,请等待...";
    case E_MES_WARNING_OFFLINE_MODE:
        return L"警告:当前MES系统处于离线模式";
    case E_MES_WARNING_STATION_NOT_PROCESS:
        return L"警告:当前站点不经过MES流程";
    case E_MES_CONNECTING_DATA_SERVICE:
        return L"正在连接MES数据服务";
    case E_MES_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING:
        return L"连接MES数据服务失败，请检查MES服务是否已经开启";
    case E_MES_CONNECT_SUCCESSFULLY:
        return L"连接MES数据服务成功";
    case E_MES_CONNECT_SERVICE_FAILED:
        return L"与MES 服务通讯失败";
    case E_WIFI_CONNECTING_DATA_SERVICE:
        return L"正在连接WIFI服务";
    case E_WIFI_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING:
        return L"连接WIFI服务失败，请检查WIFI服务是否已经开启";
    case E_WIFI_CONNECT_SUCCESSFULLY:
        return L"连接WIFI服务成功";
    case E_CPE_DOWNLOAD_PORTHASSORTED_RESORT:
        return L"当前端口已经排序，如需排序请重新清空排序信息";
    case E_CPE_DOWNLOAD_PORT_IS_SORT_NEXTINDEX_SORT:
        return L"当前排序已经完成是否进入下一Index排序";
    case E_CPE_DOWNLOAD_INDEX_OUTOFRANGE_CHECK_ISSORT:
        return L"当前线程Index超过范围,请检查当前端口是否未进行排序";
    case E_CPE_DOWNLOAD_NOT_ALLOW_CHANGE_CONFIG:
        return L"下载程序运行过程中已经有设备插入，不允许在设备插入后在修改配置文件，请重新打开下载程序在修改配置文件";
    case E_TSE_INITFAIL_CHECKCONFIG_LOG:
        return L"初始化配置失败，请检查配置是否正常,详情见Log";
    case E_TSE_LOAD_MODULE_FAIL:
        return L"加载TSE模块失败";
    case E_XML_FAILED_PARSE_CONFIG:
        return L"解析XML配置文件失败(%s)";
    case E_TSE_XMLCONFIG_NO_TEST_PROJECT:
        return L"当前配置文件无测试项目";
    case E_TSE_TEST_INDEX_RULE:
        return L"请按照测试顺序测试，或者点击最后一项测试方法重置当前测试";
    case E_TSE_FILE_NOT_EXIST:
        return L"文件不存在";
    }
    return L"";
}