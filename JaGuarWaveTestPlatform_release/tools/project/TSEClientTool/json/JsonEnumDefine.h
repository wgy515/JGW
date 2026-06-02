#pragma once
#include <string>
#define JSON_CONTENT_SEPARATOR "----"

namespace JGW
{
    enum ActionJsonEnum
    {
        /// <summary>
        /// 未知
        /// </summary>
        Unknown_AJE = -1,
        /// <summary>
        /// 客户端或者服务端退出
        /// </summary>
        Exit_AJE = 0,
        /// <summary>
        /// socket 标识
        /// </summary>
        Flags_AJE = 1,
        /// <summary>
        /// 测试序列状态变化
        /// </summary>
        TestStatusChange_AJE = 2,
        /// <summary>
        /// 测试索引变化
        /// </summary>
        TestIndexChange_AJE = 3,
        /// <summary>
        /// 测试相关log
        /// </summary>
        TestLog_AJE,
        /// <summary>
        /// 测试控制相关命令
        /// </summary>
        ControlCmd_AJE
    } ;

    /*enum TestStatusEnum
    {
        /// <summary>
        /// 初始化测试状态
        /// </summary>
        InitTestStatus = 0,
        /// <summary>
        /// 准备运行的测试状态
        /// </summary>
        RunTestStatus,
        /// <summary>
        ///正在测试的状态
        /// </summary>
        RuningTestStatus, 
        /// <summary>
        /// 测试失败
        /// </summary>
        FailTestStatus,
        /// <summary>
        /// 测试成功
        /// </summary>
        PassTestStatus
    };*/

    enum TSELogTypeEnum
    {
        /// <summary>
        /// 
        /// </summary>
        Debug_TSE = 0,
        /// <summary>
        /// 
        /// </summary>
        Info_TSE,
        /// <summary>
        /// 
        /// </summary>
        Error_TSE,
        /// <summary>
        /// 
        /// </summary>
        UserDefine_TSE
    };

    enum TSETestControlEnum
    {
        Unknown_TCE = -1,
        Fail_TCE = 0,
        PASS_TCE,
        Init_TCE,
        Start_TCE,
        Stop_TCE,
        Exit_TCE,
        MAX_TCE
    };
    //StartFlag[] = L"{D261BC0C-45F3-498D-AEE8-5E730691CF35}";
    //szPassFlag[] = L"{A6A28E32-E91C-40D0-A79B-06CC8B441601}";
    //szFailFlag[] = L"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}";
    //szExitFlag[] = L"{65FDA37A-8DFE-4540-9611-255A5C823649}";
    const std::string gstrTestGuids[] = {
        "{95DD4B2F-F186-4EE2-A221-B15EC0D70CE2}" //! Unknown
        ,"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}" //! FAIL
        ,"{A6A28E32-E91C-40D0-A79B-06CC8B441601}" //! PASS
        ,"{0AC2981A-4474-45FE-AAF7-694A9EB95D73}" //! Init
        ,"{D261BC0C-45F3-498D-AEE8-5E730691CF35}" //! Start
        ,"{65FDA37A-8DFE-4540-9611-255A5C823649}" //! stop
        ,"{F32BCFBA-A54D-4A59-B401-423756785EEF}" //! Exit
    };
    /*enum UserDefineLogTypeEnum 
    {
        /// <summary>
        /// 未知
        /// </summary>
        UnknownUserDefineLogType = -1,
        /// <summary>
        /// html file path
        /// </summary>
        HtmlFilePath = 0,
        /// <summary>
        /// 
        /// </summary>
        ShowContentInfo = 1,
        /// <summary>
        /// 
        /// </summary>
        ShowTestInfoToRichEdit = 2
    };*/

}