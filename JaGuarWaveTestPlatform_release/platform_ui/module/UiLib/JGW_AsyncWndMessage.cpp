#include "StdAfx.h"
#include <UiLib\JGW_AsyncWndMessage.h>
/*#include <boost/detail/atomic_count.hpp>*/

namespace UiLib
{
   volatile UINT mnput_count(0);

    CCJGW_AsyncWndMessage::CCJGW_AsyncWndMessage(HWND& hMainHwnd):mp_msg_str_queue(new std::queue<std::wstring>)
        ,mp_execute_msg_queue(new std::queue<S_ASYNC_MESSAGE>),mhMaindHwnd(hMainHwnd)
    {
        mp_current_msg_queue = new std::queue<S_ASYNC_MESSAGE>;
    }

    CCJGW_AsyncWndMessage::~CCJGW_AsyncWndMessage(void)
    {
        if (mp_execute_msg_queue) delete mp_execute_msg_queue;
        if (mp_current_msg_queue) delete mp_current_msg_queue;
        if (mp_msg_str_queue) delete mp_msg_str_queue;
    }

    HWND CCJGW_AsyncWndMessage::GetMainHwnd()
    {
        return mhMaindHwnd;
    }

    void CCJGW_AsyncWndMessage::PutAsyncMessage(UINT msgid,WPARAM wParam,LPARAM lParam)
    {
        S_ASYNC_MESSAGE sAsyncMessage = {msgid,wParam,lParam};
        JGW::CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
        mp_current_msg_queue->push(sAsyncMessage);
        ++mnput_count;
    }

    void CCJGW_AsyncWndMessage::PutAsyncWParamStringMessage(UINT msgid,WPARAM wParam,const wchar_t* strMsg)
    {
        JGW::CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
        mp_msg_str_queue->push(strMsg);
        S_ASYNC_MESSAGE sAsyncMessage = {msgid,wParam,LPARAM(mp_msg_str_queue->back().c_str())};
        mp_current_msg_queue->push(sAsyncMessage);
        ++mnput_count;
    }

    void CCJGW_AsyncWndMessage::PutAsyncStringMessage(UINT msgid,const wchar_t* strMsg,LPARAM lParam)
    {
        JGW::CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
        mp_msg_str_queue->push(strMsg);
        S_ASYNC_MESSAGE sAsyncMessage = {msgid,WPARAM(mp_msg_str_queue->back().c_str()),lParam};
        mp_current_msg_queue->push(sAsyncMessage);
        ++mnput_count;
    }
    //! 交换异步窗口消息
    void CCJGW_AsyncWndMessage::SwapQueue()
    {
        JGW::CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
        std::queue<S_ASYNC_MESSAGE>* p_queue = mp_current_msg_queue;
        mp_current_msg_queue = mp_execute_msg_queue;
        mp_execute_msg_queue = p_queue;
    }

    void CCJGW_AsyncWndMessage::ClearCurrentAsyncMsgQueue()
    {
        while (!mp_current_msg_queue->empty()) mp_current_msg_queue->pop();
    }

    void CCJGW_AsyncWndMessage::ClearExecuteAsyncMsgQueue()
    {
        while (!mp_execute_msg_queue->empty()) mp_execute_msg_queue->pop();
    }

    void CCJGW_AsyncWndMessage::ClearAllAsyncMessage()
    {
        ClearCurrentAsyncMsgQueue();
        ClearExecuteAsyncMsgQueue();
        while (!mp_msg_str_queue->empty()) mp_msg_str_queue->pop();
    }

    std::queue<S_ASYNC_MESSAGE>* CCJGW_AsyncWndMessage::ExecuteAsyncMessage(void)
    {
        SwapQueue();
        //! 当消息数量小于字符串数量的时候 删除字符串数量
        while ( mnput_count < mp_msg_str_queue->size() ) mp_msg_str_queue->pop();
        size_t i = mp_execute_msg_queue->size();
        //! 将已经取走的队列移除put消息区域
        while (i-- > 0) --mnput_count;

        return mp_execute_msg_queue;
    }

    void CCJGW_AsyncWndMessage::ExecuteAsyncMessage(std::function<void(std::queue<S_ASYNC_MESSAGE>*)> func)
    {
        SwapQueue();
        //! 当消息数量小于字符串数量的时候 删除字符串数量
        while ( mnput_count < mp_msg_str_queue->size() ) mp_msg_str_queue->pop();

        size_t i = mp_execute_msg_queue->size();
         //! 将已经取走的队列移除put消息区域
        while (i-- > 0) --mnput_count;

        func(mp_execute_msg_queue);

        while (!mp_execute_msg_queue->empty()) mp_execute_msg_queue->pop();
    }
}

