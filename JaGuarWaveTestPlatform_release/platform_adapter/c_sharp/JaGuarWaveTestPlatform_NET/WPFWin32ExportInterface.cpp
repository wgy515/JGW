// JaGuarWaveTestPlatform_NET.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "resource.h"
#include <JaGuarWaveTestPlatform/CJGW_MESConfig.h>

#include "WPFWin32ExportInterface.h"
#include "CJGWMainDlg_Net.h"

#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

HMODULE ghModule = NULL;
static JGW::CCJGWMainDlg_Net* gpIWPFWin32WindowInterface = NULL;
S_MES_INFO sMesInfo;
bool LoadMESConfig(S_MES_INFO& sMesInfo);

void UpdateWin32WindowBackgroundColor(const wchar_t* color)
{
    gpIWPFWin32WindowInterface->UpdateWin32WindowBackgroundColor(color);
}

void CloseWin32Window()
{
    gpIWPFWin32WindowInterface->CloseWin32Window();
}

void UnLoadWin32Window()
{
    if (gpIWPFWin32WindowInterface)
    {
        CPaintManagerUI::Term();
        delete gpIWPFWin32WindowInterface;
        gpIWPFWin32WindowInterface = NULL;
    }
}

void UpdateMESInfo(bool mesOnline,const wchar_t* taskNumber, const wchar_t* workOrderNumber, const wchar_t* userName, const wchar_t* password, int shiftID)
{
    gpIWPFWin32WindowInterface->UpdateMESInfo(mesOnline,taskNumber,workOrderNumber,userName,password,shiftID);
//     sMesInfo.mstrTaskNumber = JGW::JGW_W2A_W(taskNumber);
//     sMesInfo.mstrWorkOrderNumber = JGW::JGW_W2A_W(workOrderNumber);
//     sMesInfo.msMesConnectInfo.mstrUsername = JGW::JGW_W2A_W(userName);
//     sMesInfo.msMesConnectInfo.mstrUserpwd = JGW::JGW_W2A_W(password);
//     sMesInfo.mstrShiftID = (0 == shiftID)?"310001":"310002";
}

void UpdatePluginInfo(const wchar_t* stationID,const wchar_t* clsid,int viewloader_id,const wchar_t* moduleFolder,const wchar_t* pluginName,const wchar_t* station_name,int image_id)
{
    gpIWPFWin32WindowInterface->UpdatePluginInfo(stationID,clsid,viewloader_id,moduleFolder,pluginName,station_name,image_id);
    //sMesInfo.mstrOperationId = JGW::JGW_W2A_W(operationID);
}

void ShowLogControl()
{
    gpIWPFWin32WindowInterface->ShowLogControl();
}

void ShowLogDialog()
{
    gpIWPFWin32WindowInterface->ShowLogDialog();
}

void StartRemoteLogSocket(bool bOpen)
{
    gpIWPFWin32WindowInterface->StartRemoteLogSocket(bOpen);
}

HWND LoadWin32Window(HWND hMainWnd,DWORD dwUsbDevicePoolThreadID)
{
    CPaintManagerUI::SetInstance(ghModule);
    gpIWPFWin32WindowInterface = new JGW::CCJGWMainDlg_Net();
    return gpIWPFWin32WindowInterface->LoadWin32Window(hMainWnd,dwUsbDevicePoolThreadID);
}


BOOL APIENTRY DllMain( HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    ghModule = hModule;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}








//bool LoadHelpConfig(JGW::S_JGW_HELP_CONFIG& sHelpConfig)
//{
//    std::wstring strFilePath(JGW::JGW_GetApplicationFolder());
//    JGW::JGW_EraseLastAndFristChars(strFilePath,'\\');
//    JGW::JGW_EraseAfterToRemoveCharW(strFilePath,L'\\');
//    strFilePath += L"help\\help.ini";
//
//    JGW::CCJGW_ConfigIni helpConfigIni;
//    helpConfigIni.InitIniFilePath(strFilePath.c_str());
//    sHelpConfig.mstrHelpServiceIPAddr = helpConfigIni.GetIniKeyValueA(L"help",L"Host",L"127.0.0.1");
//    sHelpConfig.mnSocketTimeoutSec = helpConfigIni.GetIniKeyIntValue(L"help",L"SocketTimeOutSec",50);
//    sHelpConfig.mnUpdateServiceSocketPort = helpConfigIni.GetIniKeyIntValue(L"help",L"SocketPort",3002);
//
//    return true;
//}