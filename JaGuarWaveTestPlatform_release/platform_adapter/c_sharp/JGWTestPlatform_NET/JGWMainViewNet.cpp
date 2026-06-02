#include "StdAfx.h"
#include "JGWMainViewNet.h"
#include <Windows.h>

namespace JGW
{
    JGWMainViewNet::JGWMainViewNet(void)
    {
    }


    JGWMainViewNet::~JGWMainViewNet(void)
    {
    }

    LPCTSTR JGWMainViewNet::GetWindowClassName() const
    {
        return L"UILIB_JGWMainViewNet";
    }

    UINT JGWMainViewNet::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    HWND JGWMainViewNet::DoModal(HWND hMaindWnd)
    {
       
        //MessageBox(m_hParentHwnd,NULL,L"LoadLayoutFile",NULL);
        m_hParentHwnd = hMaindWnd;
// #if defined USE_ZIP_RESOURCE_ENABLE
//         SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
//         CreateNoBorderStyleWindow(_T(""),_T("CalLogUISkin.xml"),_T("Log Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
// #elif defined USE_ZIP_FILE_ENABLE
//         SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
//         CreateNoBorderStyleWindow(_T(""),_T("CalLogUISkin.xml"),_T("Log Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
// #else
        //CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("test.xml"),_T("Log Windows"),m_hParentHwnd,UI_CLASSSTYLE_CHILD);
/*#endif*/
       // CenterWindow();

        //CreateDuiDialogWindow((HWND)NULL,_T("skin\\JaGuarWave"),_T("test.xml"),UI_CLASSSTYLE_CHILD);
        //Log4WE_F(L"Error %d HWND: %d",GetLastError(),GetHWND());
        //Subclass(GetHWND());
        //MoveWindow(GetHWND(),0,0,400,400,TRUE );
        //CenterWindow();
      //  Create();
//         LONG styleValue = ::GetWindowLong(GetHWND(), GWL_STYLE);
//         styleValue &= ~WS_POPUP;
//         styleValue &= ~WS_BORDER;
//         styleValue &= ~WS_DLGFRAME;
//         ::SetWindowLong(GetHWND(), GWL_STYLE, styleValue |WS_CHILD);

        //SetWindowLong(GetHWND(),,GWL_STYLE)
        ///ModifyStyle(GetHWND(), WS_POPUP | WS_BORDER | WS_DLGFRAME, WS_CHILD, NULL);
        //SetParent(GetHWND(), m_hParentHwnd);
        //MoveWindow(GetHWND(),0,0,400,400,TRUE ); // 将子进程移动到外壳进程
        m_hParentHwnd = hMaindWnd;  
        //m_strSkinFolder = _T("skin\\JaGuarWave");
        //m_strSkinFilePath = _T("test.xml");
        SetSkinFolder(L"skin\\JaGuarWave");
        SetSkinFileName(L"JaGuarWaveTestPlatform_NET.xml");

        m_bIsCreateDuiWnd = true;
        //Log4WE_F(L"LoadLayoutFile Create HWND : %d",GetHWND());
        //return NULL;
        return Create(m_hParentHwnd,L"JGWMainViewNet_WND",WS_CHILD | WS_VISIBLE ,0,0,0,200,400,0);

        //m_dwClassStyle = UI_CLASSSTYLE_CHILD;
        //Log4WE_F(L"LoadLayoutFile HWND : %d",GetHWND());
        //MessageBox(m_hParentHwnd,NULL,L"LoadLayoutFile",NULL);
//         Subclass(m_hParentHwnd);
//         Log4WE_F(L"LoadLayoutFile HWND : %d",GetHWND());
//          bool result = LoadLayoutFile();
//          Log4WE_F(L"LoadLayoutFile result : %d",result?1:0);
        //return ShowModal();
        //return 0;
    }
// 
//     void JGWMainViewNet::SubClassWindow(HWND hMaindWnd)
//     {
//         m_hParentHwnd = hMaindWnd;  
//         m_strSkinFolder = _T("skin\\JaGuarWave");
//         m_strSkinFileName = _T("test.xml");
//         m_bIsCreateDuiWnd = true;
//         m_dwClassStyle = UI_CLASSSTYLE_CHILD;
//         Subclass(m_hParentHwnd);
//     }

    void JGWMainViewNet::Notify(TNotifyUI& msg)
    {
         //dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("main.xml"),_T("JaGuarWave Factory Test Application"));
        if( msg.sType == DUI_MSGTYPE_CLICK )
        {
//             if (msg.pSender->GetName() == L"mesOkBtn")
//             {
                MessageBox(GetHWND(),msg.pSender->GetName(),msg.pSender->GetName(),NULL);
/*            }*/
        }
    }
}
