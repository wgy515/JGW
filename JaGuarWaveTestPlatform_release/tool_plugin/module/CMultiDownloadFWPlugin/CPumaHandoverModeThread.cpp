#include "StdAfx.h"
#include "CPumaHandoverModeThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "PumaDownload_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_DevicePool/DevicePortDefine.h>

namespace JGW
{
    CCPumaHandoverModeThread::CCPumaHandoverModeThread(void)
    {
    }


    CCPumaHandoverModeThread::~CCPumaHandoverModeThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCPumaHandoverModeThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(SWITCH_EDL_MODE_THREAD_MSG,&CCPumaHandoverModeThread::OnHandoverDownloadMode)
    END_MYTHREAD_MESSAGE_MAP()

    void ClearDevice(SDeviceMsg_Info* lpDevice)
    {
        lpDevice->Dec();
    }


    void CCPumaHandoverModeThread::OnHandoverDownloadMode(WPARAM wParam,LPARAM lParam)
    {
        if(!wParam || !lParam) return ;
        PS_PUMA_DOWNLOAD_INFO psPumaDownloadInfo = (PS_PUMA_DOWNLOAD_INFO)lParam;
        SDeviceMsg_Info* lpdevice = (SDeviceMsg_Info*)wParam;
        std::shared_ptr<SDeviceMsg_Info> ps(lpdevice,ClearDevice);
        std::wstring mstrComPort;
        std::string strRead;

        JGW_FormatWString(mstrComPort,L"COM%d",lpdevice->m_nComPort);
        if (!mcscp.OpenSerialComPort(mstrComPort)) return ;
        std::string strCmd;
        //! 1048576 100000
        //! 665872
        //! erase b f8000000 a2910
        if (psPumaDownloadInfo->mbFormatPDBImage)
        {
            strCmd = eraseChipCmd;
            //! JGW_FormatString(strCmd,eraseChipCmd,FlashMemoryBase,psPumaDownloadInfo->mnImageFileSize);
        }
        else
        {
            JGW_FormatString(strCmd,"erase %c %lx %lx\n",'b',FlashMemoryBase,psPumaDownloadInfo->mnImageFileSize);
        }

        mcscp.ReadRespCommandToWriteCommand(strCmd.c_str(),strRead);
        mcscp.WriteCommand(resetcmd);
        mcscp.CloseSerialComPort();
    }
}
