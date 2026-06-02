// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include <poco/ClassLibrary.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
#include "TSE_InitializeWlanTester.h"
#include "TSE_ConnectDutUsingUserTransportDLL.h"
#include "TSE_DutLog.h"
#include "TSE_WlanLoadDut.h"
#include "TSE_DisconnectDut.h"
#include "TSE_WlanXtalCalInit.h"
#include "TSE_WlanXtalCalibration.h"
#include "TSE_WlanTxCalibrationFWControlled.h"
#include "TSE_IPQ40xxTxCalTest.h"
#include "TSE_IPQ40xxRxCalTest.h"
#include "TSE_IPQ40xxSetUpRx.h"
#include "TSE_WlanRxCalibrationFWControlled.h"
#include "TSE_WlanMemCommit.h"
#include "TSE_WlanSetOtpMAC.h"
#include "TSE_SetPhoneResource.h"
#include "TSE_QCA639xTxTest.h"
#include "JGW_Phone.h"
#include "TSE_GPSTest.h"
#include "TSE_IPQ40xxGuLossCal.h"
#include "TSE_WlanSetPhyRFMode.h"
#include "TSE_AutoDetectChip.h"
#include "TSE_QCA639xRxTest.h"
#include "TSE_QCA639xSetUpDutTx.h"

#include "TSE_IPQ807xConnectDutUsingQUTS.h"
#include "TSE_IPQ807xDisConnectDut.h"
#include "TSE_IPQ807xWlanSetInstance.h"
#include "TSE_IPQ807xWlanSetLocalMac.h"
#include "TSE_IPQ807xWlanXtalCalInit2.h"
#include "TSE_IPQ807xWlanXtalCalibration2.h"
#include "TSE_IPQ807xWlanTxCalibrationFWControlled2.h"
#include "TSE_IPQ807xWlanRxCalibrationFWControlled2.h"
#include "TSE_IPQ807xWlanNoiseFloorCalGenBDF.h"
#include "TSE_IPQ807xBoardDataGetSize.h"
#include "TSE_IPQ807xWlanSaveBDF2.h"
#include "TSE_IPQ807xWlanSetDutTxMacAddressBssId.h"
#include "TSE_IPQ807xConnectDut.h"
#include "TSE_IPQ807xTxTest.h"
#include "TSE_IPQ807xRxTest.h"
#include "TSE_ClearWiFiTestRecord.h"
#include "TSE_InitWiFiTestRecord.h"
#include "TSE_SaveWiFiTestRecord.h"
#include "TSE_ParseWiFiTestLog.h"
#include "TSE_IPQ807xWlanSetDutTxMacAddressBssId.h"

using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_GlobalResources)
	POCO_EXPORT_CLASS(CTSE_InitializeWlanTester)
	POCO_EXPORT_CLASS(CTSE_ConnectDutUsingUserTransportDLL)
	POCO_EXPORT_CLASS(CTSE_DutLog)
	POCO_EXPORT_CLASS(CTSE_WlanLoadDut)
	POCO_EXPORT_CLASS(CTSE_DisconnectDut)
	POCO_EXPORT_CLASS(CTSE_WlanXtalCalInit)
	POCO_EXPORT_CLASS(CTSE_WlanXtalCalibration)
	POCO_EXPORT_CLASS(CTSE_WlanTxCalibrationFWControlled)
	POCO_EXPORT_CLASS(CTSE_IPQ40xxTxCalTest)
	POCO_EXPORT_CLASS(CTSE_IPQ40xxRxCalTest)
    POCO_EXPORT_CLASS(CTSE_IPQ40xxSetUpRx)
    POCO_EXPORT_CLASS(CTSE_WlanRxCalibrationFWControlled)
	POCO_EXPORT_CLASS(CTSE_WlanMemCommit)
	POCO_EXPORT_CLASS(CTSE_WlanSetOtpMAC)
    POCO_EXPORT_CLASS(CTSE_GPSTest)
    POCO_EXPORT_CLASS(CTSE_IPQ40xxGuLossCal)
    POCO_EXPORT_CLASS(CTSE_SetPhoneResource)
    POCO_EXPORT_CLASS(CTSE_QCA639xTxTest)
    POCO_EXPORT_CLASS(CTSE_QCA639xRxTest)
    POCO_EXPORT_CLASS(CTSE_WlanSetPhyRFMode)
    POCO_EXPORT_CLASS(CTSE_AutoDetectChip)
    POCO_EXPORT_CLASS(CTSE_QCA639xSetUpDutTx)
    POCO_EXPORT_CLASS(CTSE_IPQ807xConnectDutUsingQUTS)
    POCO_EXPORT_CLASS(CTSE_IPQ807xDisConnectDut)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanSetInstance)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanSetLocalMac)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanSetDutTxMacAddressBssId)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanXtalCalInit2)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanXtalCalibration2)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanTxCalibrationFWControlled2)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanRxCalibrationFWControlled2)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanNoiseFloorCalGenBDF)
    POCO_EXPORT_CLASS(CTSE_IPQ807xBoardDataGetSize)
    POCO_EXPORT_CLASS(CTSE_IPQ807xWlanSaveBDF2)
    POCO_EXPORT_CLASS(CTSE_IPQ807xConnectDut)
    POCO_EXPORT_CLASS(CTSE_IPQ807xTxTest)
    POCO_EXPORT_CLASS(CTSE_IPQ807xRxTest)
    POCO_EXPORT_CLASS(CTSE_ClearWiFiTestRecord)
    POCO_EXPORT_CLASS(CTSE_InitWiFiTestRecord)
    POCO_EXPORT_CLASS(CTSE_SaveWiFiTestRecord)
    POCO_EXPORT_CLASS(CTSE_ParseWiFiTestLog)
POCO_END_MANIFEST

CJGW_Phone* gpPhone = NULL;

namespace JGW
{
    CJGW_Phone* GetPhoneInstance()
    {
        if (NULL == gpPhone)
        {
            gpPhone = new CJGW_Phone();
        }
        return gpPhone;
    }
}

void pocoUninitializeLibrary()
{
    if (gpPhone)
    {
        delete gpPhone;
        gpPhone = NULL;
    }
}



#ifdef _DEBUG
#pragma comment(lib,"TSE_FoundationLib_d.lib")
#else
#pragma comment(lib,"TSE_FoundationLib.lib")
#endif
BOOL APIENTRY DllMain( HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifdef IS_ENCRYPT_MODULE
        return JGW::EncryptModuleIsLoaded(hModule) ? TRUE : FALSE;
#endif  
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

