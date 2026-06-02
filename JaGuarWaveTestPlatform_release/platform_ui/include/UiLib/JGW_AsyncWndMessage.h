#pragma once
/*#include "ctf_msvc_lib.h"*/
#include <Windows.h>
#include <queue>
#include <functional>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
/*
异步窗口消息类
保存异步字符串
*/


namespace UiLib
{
    struct S_ASYNC_MESSAGE
    {
        UINT        mMsgID;
        WPARAM      wParam;
        LPARAM      lParam;
    };

    class UILIB_API CCJGW_AsyncWndMessage
    {
    public:
        CCJGW_AsyncWndMessage(HWND& hMainHwnd);
        ~CCJGW_AsyncWndMessage(void);
        //!
        HWND GetMainHwnd();
        //! 异步消息
        void PutAsyncMessage(UINT msgid,WPARAM wParam = NULL,LPARAM lParam = NULL);
        //! 异步字符串消息
        void PutAsyncStringMessage(UINT msgid,const wchar_t* strMsg,LPARAM lParam = NULL);
        //! 异步字符串消息
        void PutAsyncWParamStringMessage(UINT msgid,WPARAM wParam,const wchar_t* strMsg);
        //! 处理消息 只能是主线程调用
        void ExecuteAsyncMessage(std::function<void(std::queue<S_ASYNC_MESSAGE>*)> func);
        //! 处理消息  
        std::queue<S_ASYNC_MESSAGE>* ExecuteAsyncMessage(void);
    private:
        //! 清除当前Put消息队列 只能是测试线程调用
        void ClearCurrentAsyncMsgQueue();
        //! 销毁的时候调用
        void ClearAllAsyncMessage();
        //! 清除在执行区的异步消息  -- 只能是窗口线程调用
        void ClearExecuteAsyncMsgQueue();
    private:
        //! 执行区队列与缓冲区队列交换
        void SwapQueue();
    private:
        //!
        HWND& mhMaindHwnd;
        //! put 消息队列 当执行execute函数的时候，put队列和给get队列互换
        std::queue<S_ASYNC_MESSAGE>* mp_current_msg_queue;
        //! get 消息队列
        std::queue<S_ASYNC_MESSAGE>* mp_execute_msg_queue;
        //! 消息队列
        std::queue<std::wstring>* mp_msg_str_queue;
        //! 当前操作完成标识
        //volatile bool mbcomplete;
        //! 计数记录当前消息队列数量   
        JGW::CCJGW_CriticalSectionLock mCriticalSectionLock;
    };
}


