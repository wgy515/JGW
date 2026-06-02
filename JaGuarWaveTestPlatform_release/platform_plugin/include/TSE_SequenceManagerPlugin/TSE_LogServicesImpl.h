#pragma once
#include <string>
//! 注册回调
namespace JGW
{
    class CTSE_LogServicesImpl
    {
    public:
        //! 调试信息 -- 主要用来调试使用 不会在界面上显示
        virtual void DebugLog(const wchar_t* strDebugLog) = 0;
        //!
        virtual void DebugLogFormat(const wchar_t* strDebugLog,...) = 0;
        //! 调试信息 -- 主要用来调试使用 不会在界面上显示
        virtual void DebugLog(const char* strDebugLog) = 0;
        //!
        virtual void DebugLogFormat(const char* strDebugLog,...) = 0;
        //! 错误信息 -- 显示错误的相关内容
        virtual void ErrorLog(const wchar_t* strErrorLog) = 0;
        //!
        virtual void ErrorLogFormat(const wchar_t* strErrorLog,...) = 0;
        //! 错误信息 -- 显示错误的相关内容
        virtual void ErrorLog(const char* strErrorLog) = 0;
        //!
        virtual void ErrorLogFormat(const char* strErrorLog,...) = 0;
        //! 测试信息 -- 显示测试相关信息  在界面上显示
        virtual void InfoLog(const wchar_t* strInfoLog) = 0;
        //!
        virtual void InfoLogFormat(const wchar_t* strInfoLog,...) = 0;
        //! 测试信息 -- 显示测试相关信息  在界面上显示
        virtual void InfoLog(const char* strInfoLog) = 0;
        //!
        virtual void InfoLogFormat(const char* strInfoLog,...) = 0;
        //! 客户自定义Log
        virtual void UserDefinedLog(int id,const wchar_t* strCustomLog) = 0;
        //! 客户自定义Log
        virtual void UserDefinedLogFormat(int id,const wchar_t* strCustomLog,...) = 0;
    };
}