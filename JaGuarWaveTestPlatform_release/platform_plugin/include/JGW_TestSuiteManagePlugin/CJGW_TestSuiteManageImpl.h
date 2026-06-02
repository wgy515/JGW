#pragma once
#include <string>
#include "CJGW_TestSuiteManage_Enum_Define.h"
namespace JGW
{
    typedef void (_cdecl *jgw_suite_test_log_func)(int nIndex,int type,const wchar_t* strmsg);

    class CTSE_SequenceManageImpl
    {
    public:
        virtual ~CTSE_SequenceManageImpl(){}
        //! 初始化当前测试指针
        virtual bool InitTestSuiteManageInterface( HWND hMain,LPCTSTR strConfigFilePath ) = 0;
        //! strConfigFileBuf <?xml version="1.0" encoding="UTF-8" standalone="yes" ?><Test></Test>
        virtual bool InitTestSuiteManageInterfaceToXmlBuf( HWND hMain,std::string& strConfigFileBuf ) = 0;
        //! 执行所有测试项目根据接口序列类指针
        virtual bool ExecuteAllSuiteTest( bool bErrorStop = true ) = 0;
        //! 执行单个测试 根据测试ID 以及 接口序列类指针
        virtual bool ExecuteSingleSuiteTestToTestID( int nIndex ) = 0;
        //! 执行当前序列销毁动作
        virtual bool ExecuteSuiteTestFinalize( void ) = 0;
        //! 注册系统log事件句柄函数
        virtual void Register_System_Event_Handler_Fn( jgw_suite_test_log_func evtcb ) = 0;
        //! 设置环境变量( 所有suite dll共享一份变量 )
        virtual void SetSuiteEnvironmentVar( const wchar_t* envirname,const wchar_t* envirvalue ) = 0;
        //! 获取序列环境变量
        virtual std::wstring GetSuiteEnvironmentVar( const wchar_t* envirname ) = 0;
        //! 获取suite测试数量
        virtual size_t GetSuiteTestCounts( void ) = 0;
        //! 更新环境变量参数配置
        virtual void UpdateEnviromentParam( void ) = 0;
    };
}