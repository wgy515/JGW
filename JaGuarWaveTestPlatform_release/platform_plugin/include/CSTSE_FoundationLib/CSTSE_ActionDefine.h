#pragma once

#ifndef CSTSE_FOUNDATION_LIB_EXPORTS
#   define CSTSE_FOUNDATION_LIB_IMPORTS
#endif

#ifdef CSTSE_FOUNDATION_LIB_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"CSTSE_FoundationLib_d.lib")
#   else
#       pragma comment(lib,"CSTSE_FoundationLib.lib")
#   endif
#endif
/*
CSTSE => Client/Server TSE (采用服务端程序启动客户端测试进程，通过管道通信相关命令，管道的相关命令采用加密处理)

1、client 启动参数，或者紧接着Server端发送 CST_COMMAND_PARAM command param => GetCommandLineW
2、client 返回init tse result => CST_COMMAND_PARAM,true,error msg
3、Server => GET_TSE_INFO (suite count,suite_index_info)
4、Server => UPDATE ENV/GET ENV => Client => result => Server
5、Server => Execute all/single => client => result => Server
6、TSE_Enverionment Client <=> Server



#pragma once
//! 字符串加密处理
namespace JGW
{
//! CST 操作类型
enum E_CST_Action_Type
{
//! 未知操作类型
ECST_UnknownAction = -1,
//! 根据命令参数初始化TSE进程
ECST_COMMAND_PARAM = 0,
//! 更新或者读取环境变量对应参数
ECST_EnverionmentAction = 1,
//! 执行CST测试命令
ECST_ExecuteAction,
//! 测试状态更改
ECST_TestStatusChangeAction,
//! 测试索引更改
ECST_TestIndexChangeAction,
//! 测试log
ECST_TestLog,
//! kill掉当前进程
ECST_ExitProcess
};

enum E_CST_EnverionmentOpr
{
//! 读取
E_CST_Enverionment_Get = 0,
//! 更新
E_CST_Enverionment_Update 
};

}
*/
namespace JGW
{
    enum ECSTSE_ACTION
    {
        ECSTSE_ACTION_Unknown = -1,
        //! 根据命令参数初始化TSE进程 
        ECSTSE_ACTION_COMMAND_PARAM_INIT_TSE = 0,
        //! 
        ECSTSE_ACTION_RESPONSE_COMMAND_PARAM_INIT_TSE = 0,
        //! GET_TSE_INFO (suite count,suite_index_info)
        ECSTSE_ACTION_GET_TSE_INFO,
        //! 
        ECSTSE_ACTION_RESPONSE_GET_TSE_INFO,
        //! UPDATE OR GET Enverionment Key-Value
        ECSTSE_ACTION_Enverionment,
        //!
        ECSTSE_ACTION_RESPONSE_Enverionment,
        //! TSE => TEST
        ECSTSE_ACTION_TSE_TEST,
        //!
        ECSTSE_ACTION_RESPONSE_TSE_TEST,
        //! log
        ECSTSE_ACTION_TEST_LOG,
        //!
        //ECSTSE_ACTION_RESPONSE_TEST_LOG,
        //! TestStatusChange
        ECSTSE_ACTION_TEST_STATUS_CHANGE,
        //!
        /// ECSTSE_ACTION_RESPONSE_TEST_STATUS_CHANGE,
        //! TestIndexChange
        ECSTSE_ACTION_TEST_INDEX_CHANGE,
        //! 
        ///ECSTSE_ACTION_RESPONSE_TEST_INDEX_CHANGE
        //! 回应请求相关参数
        //! ECSTSE_ACTION_RESPONSE = 1
    };

    enum ECSTSE_TSE_TEST_LOG_TYPE
    {
        //! 
        ECSTSE_TSE_TEST_LOG_TYPE_DEBUG = 0,
        //! 
        ECSTSE_TSE_TEST_LOG_TYPE_INFO,
        //! 
        ECSTSE_TSE_TEST_LOG_TYPE_ERROR,
        //! 
        ECSTSE_TSE_TEST_LOG_TYPE_USERDEFINE
    };

    enum ECSTSE_TSE_TEST_TYPE
    {
        //! auto test
        ECSTSE_TSET_TYPE_AUTO = 0,
        //! config view test
        ECSTSE_TSET_TYPE_ConfigView = 1,
        //! normal
        ECSTSE_TSET_TYPE_NORMAL
    };
}