#include "StdAfx.h"
#include <JGW_QMSLFuncPlugin/CJGW_UsbModeHandoverThread.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGW_UsbModeHandoverThread::CCJGW_UsbModeHandoverThread(void)
    {
    }


    CCJGW_UsbModeHandoverThread::~CCJGW_UsbModeHandoverThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_UsbModeHandoverThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(SWITCH_EDL_MODE_THREAD_MSG,&CCJGW_UsbModeHandoverThread::OnQcommUsbHandoverEdl)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_UsbModeHandoverThread::OnQcommUsbHandoverEdl(WPARAM wParam,LPARAM lParam)
    {
        SDeviceMsg_Info* pDeviceMsgInfo= (SDeviceMsg_Info*)wParam;
        mUsbModeHandover.QcommUsbModeHandoverEdl(pDeviceMsgInfo);
    }
}