#pragma once
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>

namespace JGW
{
    enum E_TSE_INTEROP_TEST_LOG_TYPE
    {
        E_INTEROP_DEBUG_TSE_LOG = 0,
        E_INTEROP_INFO_TSE_LOG = 1,
        E_INTEROP_ERROR_TSE_LOG = 2,
        E_INTEROP_USERDEFINE_TSE_LOG = 3
    };

    class CTSE_TestLog : public CTSE_LogListener
    {
    public:
        //! 调试信息 -- 主要用来调试使用 不会在界面上显示
        virtual void OnDebugLog(const wchar_t* strDebugLog);
        //! 错误信息 -- 显示错误的相关内容
        virtual void OnErrorLog(const wchar_t* strErrorLog);
        //! 测试信息 -- 显示测试相关信息  在界面上显示
        virtual void OnInfoLog(const wchar_t* strInfoLog);
        //! 自定义LOG信息
        virtual void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
    };
}