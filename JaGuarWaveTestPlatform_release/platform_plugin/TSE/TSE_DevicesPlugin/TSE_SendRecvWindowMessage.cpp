#include "StdAfx.h"
#include "TSE_SendRecvWindowMessage.h"

namespace JGW
{
    CTSE_SendRecvWindowMessage::CTSE_SendRecvWindowMessage(void) : mdwMessageID(WM_USER),mbCheckMessageResult(false),mnCheckTimeOutSec(5)
    {
    }


    CTSE_SendRecvWindowMessage::~CTSE_SendRecvWindowMessage(void)
    {
    }

    const wchar_t* CTSE_SendRecvWindowMessage::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SendRecvWindowMessage\",\
                \"TestName\": \"发送接收窗口消息\",\
                \"MessageID\": \"休眠时间毫秒\",\
                \"CheckTimeOutSec\": \"检查超时时间，秒为单位\",\
                \"CheckMessageResult\": \"是否检查消息结果\"\
                }";
    }

    bool CTSE_SendRecvWindowMessage::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"MessageID"))
        {
            mdwMessageID = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckMessageResult"))
        {
            mbCheckMessageResult = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckTimeOutSec"))
        {
            mnCheckTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SendRecvWindowMessage::TSE_Run()
    {
        //! 发送窗口消息，并等待窗口反馈
        SendMessage(GetGlobalMainHwnd(),mdwMessageID,WPARAM(::GetCurrentThreadId()),mdwMessageID);
        //! 检查消息结果，msg.message 是发送的消息ID，msg.wParam 是当前线程ID msg.lParam 消息结果 1 : true 0 : false
        if (mbCheckMessageResult)
        {
            MSG msg = {0};
            for (size_t index = 0;index < 5 * mnCheckTimeOutSec;index++)
            {
                if (FALSE != PeekMessage(&msg,NULL,WM_USER,WM_USER + 10000,PM_REMOVE))
                {
                    //! 接收到当前线程消息,MSG ID WPARAM
                    if (msg.message == mdwMessageID && msg.wParam == mdwMessageID)
                    {
                        return 1 == msg.lParam;
                    }
                }
                Sleep(200);
            }
            LogE(L"Error : Recv Window Message TimeOut!!!");
            return false;
        }

        return true;
    }

}
