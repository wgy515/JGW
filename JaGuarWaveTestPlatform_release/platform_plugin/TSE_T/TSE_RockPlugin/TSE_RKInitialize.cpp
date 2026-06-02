#include "StdAfx.h"
#include "TSE_RKInitialize.h"
#include "RKUpgradeDll.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_RKInitialize::CTSE_RKInitialize(void) : mbRKInitialize(false)
    {
    }


    CTSE_RKInitialize::~CTSE_RKInitialize(void)
    {
        RK_Uninitialize();
    }

    const wchar_t* CTSE_RKInitialize::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RKInitialize\",\
                \"TestName\": \"RK Initialize\"\
                }";
    }

    bool CTSE_RKInitialize::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        return true;
    }

    bool CTSE_RKInitialize::TSE_Run()
    {
        if (mbRKInitialize) return true;

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
        mstrLogPath = std::wstring(JGW_GetApplicationFolder()) + L"log\\global\\";
        InitLogInfo.lpszLogPathName = (wchar_t*)(mstrLogPath.data());
        // 您可以将其设置为 ProgressPromptProc 来显示升级信息；
        InitCallbackInfo.pProgressPromptProc = NULL;//you can set it to ProgressPromptProc for showing upgrade info;
        // 您可以将其设置为 UpgradeStepPromptProc 来显示进度信息；
        InitCallbackInfo.pUpgradeStepPromptProc = NULL;//you can set it to UpgradeStepPromptProc for showing progress info;
        bool m_bUpgradeDllInitOK = (TRUE == RK_Initialize(InitDevInfo, InitLogInfo, InitCallbackInfo));
        if (!m_bUpgradeDllInitOK) 
        {
            LogE(L"Initialize RKUpgrade dll failed!");
            return false;
        }
        mbRKInitialize = true;
        return true;
    }
}
