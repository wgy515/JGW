// JaGuarWaveUpdateHelp.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include "CJaGuarwaveUpdateHelpDlg.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define JGW_Application_Instance_GUID "{C0E53FCD-423B-4E65-A6AD-6B0E6F3704ED}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave Automatic Updates Application"

void PareseCommandLineParam(JGW::S_UPDATE_HELP_CONFIG& sUpdateHelpConfig,LPTSTR lpCmdLine)
{
//     LPWSTR *szArgList;
//     int argCount;
//     szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
    std::vector<std::wstring> vTemp;
    std::wstring strTemp(lpCmdLine?lpCmdLine:L"");

    /*JGW::JGW_EraseLastAndFristChars(strTemp,'\"');*/
    if ( !strTemp.empty() && '\"' == strTemp.at(0) )
    {
        strTemp.erase(strTemp.begin());
    }
    if ( !strTemp.empty() && '\"' == strTemp.at(strTemp.length()-1) )
    {
        strTemp.erase(strTemp.end()-1);
    }

    JGW::JGW_ParserStrW(strTemp.c_str(),L"&&",vTemp);
    if (vTemp.size() >= 3)
    {
        sUpdateHelpConfig.mstrUpdateExecFolder = JGW::JGW_W2A(vTemp[0]);
        sUpdateHelpConfig.mstrUpdatePackFilePath = JGW::JGW_W2A(vTemp[1]);
        sUpdateHelpConfig.mstrUpdateVersion = JGW::JGW_W2A(vTemp[2]);
        sUpdateHelpConfig.mbAutoUpdate = true;
    }
    //! ota  config
    if (NULL != JGW::JGW_StrCaseStr(sUpdateHelpConfig.mstrUpdateVersion.c_str(),"config"))
    {
        sUpdateHelpConfig.mbIsConfigVersion = true;
    }

/*    LocalFree(szArgList);*/
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! STL本地化设置切回默认值
    if (!JGW::jgw_check_single_application_instance(_T(JGW_Application_Instance_GUID),NULL))
    {
        return -1;
    }

    JGW::S_UPDATE_HELP_CONFIG sUpdateHelpConfig;
    sUpdateHelpConfig.mbAutoUpdate = false;
    sUpdateHelpConfig.mbIsConfigVersion = false;
    PareseCommandLineParam(sUpdateHelpConfig,lpCmdLine);

    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJaGuarwaveUpdateHelpDlg* dlg = new JGW::CCJaGuarwaveUpdateHelpDlg(sUpdateHelpConfig);
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWaveUpdateHelp"),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWaveUpdateHelp.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWaveUpdateHelp"),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JAGUARWAVEUPDATEHELP);
        dlg->ShowModal(/*SW_HIDE*/);
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}