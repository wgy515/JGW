// RKDeviceLayerQuery.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "RKUpgradeDll.h"
#include "DevicesAPI.h"
using namespace JGW;

//typedef enum
//{
//    //! 没有发现设备
//    RKUSB_NONE=0x0,
//    //! MASKROM设备
//    RKUSB_MASKROM=0x01,
//    //! LOADER设备
//    RKUSB_LOADER=0x02,
//    //! MSC设备
//    RKUSB_MSC=0x04,
//    //! ADB设备
//    RKUSB_ADB=0x08,
//    //! MTP设备
//    RKUSB_MTP=0x10,
//    //! UVC设备
//    RKUSB_UVC=0x20
//} RKUSB_TYPPE;

const wchar_t* GetRKUsbType(int nType)
{
    switch (nType)
    {
    case RKUSB_NONE:
        return L"None";
    case RKUSB_MASKROM:
        return L"MaskRom";
    case RKUSB_LOADER:
        return L"Loader";
    case RKUSB_MSC:
        return L"MSC";
    case RKUSB_ADB:
        return L"ADB";
    case RKUSB_MTP:
        return L"MTP";
    case RKUSB_UVC:
        return L"UVC"; 
    }
    return L"unknown";
}

int _tmain(int argc, _TCHAR* argv[])
{
    static INIT_DEV_INFO InitDevInfo;
    static INIT_LOG_INFO InitLogInfo;
    static INIT_CALLBACK_INFO InitCallbackInfo;

    /* UpgradeDll init */
    InitDevInfo.bScan4FsUsb = FALSE;
    InitDevInfo.emSupportDevice = 0;
    InitDevInfo.uiRockMscTimeout = 30;
    InitDevInfo.uiRockusbTimeout = 30;
    InitDevInfo.usRockMscPid = 0;
    InitDevInfo.usRockMscVid = 0;
    InitDevInfo.usRockusbPid = 0;
    InitDevInfo.usRockusbVid = 0;
    InitLogInfo.bLogEnable = TRUE;
    std::wstring mstrLogPath = std::wstring(JGW_GetApplicationFolder()) + L"log\\global\\";
    InitLogInfo.lpszLogPathName = (wchar_t*)(mstrLogPath.data());
    // 您可以将其设置为 ProgressPromptProc 来显示升级信息；
    InitCallbackInfo.pProgressPromptProc = NULL;//you can set it to ProgressPromptProc for showing upgrade info;
    // 您可以将其设置为 UpgradeStepPromptProc 来显示进度信息；
    InitCallbackInfo.pUpgradeStepPromptProc = NULL;//you can set it to UpgradeStepPromptProc for showing progress info;
    bool m_bUpgradeDllInitOK = (TRUE == RK_Initialize(InitDevInfo, InitLogInfo, InitCallbackInfo));
    if (!m_bUpgradeDllInitOK) 
    {
        std::wcout << (L"Initialize RKUpgrade dll failed!") << std::endl;
        return -1;
    }
    std::wcout << L"------------------------------------------------------------------------------------------" << std::endl;
    std::wcout << L"------------------------------------------------------------------------------------------" << std::endl;

    while (true)
    {
        PSTRUCT_DEVICE_DESC pDevs = NULL;
        int m_nDeviceCount = RK_ScanDevice(&pDevs);
        for (int i = 0;i < m_nDeviceCount;i++)
        {
            std::wcout << L"RK " << GetRKUsbType(pDevs[i].emUsbType) << L" Device ID : " << pDevs[i].dwLayer << std::endl;
        }
        
        //if (m_nDeviceCount > 0)
        //{
        //    std::wcout << std::endl;
        //    std::wcout << std::endl;
        //}
        //getchar();
        Sleep(1000);
    }
    RK_Uninitialize();
    
    return 0;
}

