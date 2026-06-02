#pragma once

namespace JGW
{
    //! 监听器类，注册函数回调 
    /*
        测试线程继承CTSE_LogListenerInterface类，重写相关log函数，即可实现回调
    */
    class CTSE_LogListener
    {
    public:
        //! 调试信息 -- 主要用来调试使用 不会在界面上显示
        virtual void OnDebugLog(const wchar_t* strDebugLog) {}
        //! 错误信息 -- 显示错误的相关内容
        virtual void OnErrorLog(const wchar_t* strErrorLog) {}
        //! 测试信息 -- 显示测试相关信息  在界面上显示
        virtual void OnInfoLog(const wchar_t* strInfoLog) {}
        //! 自定义LOG信息
        virtual void OnUserDefinedLog(int id,const wchar_t* strCustomLog) {}
    };
}