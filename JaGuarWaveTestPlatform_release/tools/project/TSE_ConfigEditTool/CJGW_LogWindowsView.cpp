#include "StdAfx.h"
#include "CJGW_LogWindowsView.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGW_LogWindowsView::CCJGW_LogWindowsView(HWND hParentHwnd /* = NULL */):m_hParentHwnd(hParentHwnd)
    {
       
    }


    CCJGW_LogWindowsView::~CCJGW_LogWindowsView(void)
    {
        if (mpWkeBrowser)
        {
            wkeShutdown();
            ::CoUninitialize();
        } 
    }

    UINT CCJGW_LogWindowsView::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }


    LPCTSTR CCJGW_LogWindowsView::GetWindowClassName() const
    {
        return L"CCJGW_LogWindowsView";
    }


    int CCJGW_LogWindowsView::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("tse_log.xml"),_T("Log Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("tse_log.xml"),_T("Log Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
#else
        CreateNoBorderStyleWindow(_T("skin\\TSEConfigEditTool"),_T("tse_log.xml"),_T("Log Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
#endif
        CenterWindow();

        return ShowModal();
    }

    CControlUI* CCJGW_LogWindowsView::CreateControl(LPCTSTR pstrClass)
    {
        if(_tcscmp(pstrClass, DUI_CTR_WKEWINDOWLESSBROWSER) == 0)
            return	new CWkeWindowlessUI();
        return NULL;
    }

    void CCJGW_LogWindowsView::OnNotify(TNotifyUI& msg)
    {
#if 1
        if ( msg.sType == DUI_MSGTYPE_CLICK)
        {
            if ( msg.pSender == m_pdebugOption )
            {
                CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
                if( pControl ) 	{if( 0 != pControl->GetCurSel() ) pControl->SelectItem(0);}
                else return;
                m_plogedt->SetText(GETPDLOGW().c_str());
            }

            if ( msg.pSender == m_perrorOption)
            {
                CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
                if( pControl ) 	{if( 0 != pControl->GetCurSel() ) pControl->SelectItem(0);}
                else return;
                m_plogedt->SetText(GETPELOG4W().c_str());
            }
            if ( msg.pSender->GetName() == _T("htmlOption") )
            {    
                m_strHtmlPath = GETPHLOGW();
                if ( m_strHtmlPath.empty() ) 
                {
                    m_strHtmlPath = JGW_GetApplicationFolder();
                    m_strHtmlPath += L"empty.html";
                }
                CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
                if( pControl ) 	{if( 1 != pControl->GetCurSel() ) pControl->SelectItem(1);}
                else return;
#if 1 
                CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("ActiveXDemo1")));
                if( pActiveXUI ) 
                {
                    IWebBrowser2* pWebBrowser = NULL;
                    pActiveXUI->SetDelayCreate(false);              // 相当于界面设计器里的DelayCreate属性改为FALSE，在duilib自带的FlashDemo里可以看到此属性为TRUE             
                    pActiveXUI->CreateControl(CLSID_WebBrowser);    // 相当于界面设计器里的Clsid属性里填入{8856F961-340A-11D0-A96B-00C04FD705A2}，建议用CLSID_WebBrowser，如果想看相应的值，请见<ExDisp.h>
                    pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);

                    if( pWebBrowser != NULL ) 
                    {
                        //pWebBrowser->Navigate(L"https://code.google.com/p/duilib/",NULL,NULL,NULL,NULL);  
                        pWebBrowser->Navigate((BSTR)m_strHtmlPath.c_str(),NULL,NULL,NULL,NULL);  // 由于谷歌时不时被墙，所以换成反应快的网站
                        pWebBrowser->Release();
                    }
                }
                else
                {
                    if (!mpWkeBrowser) return ;
                    mpWkeBrowser->InitBrowser(WM_TIMER_WKE_BROWSER);
                    mpWkeBrowser->LoadFile(m_strHtmlPath.c_str()); 
                }
#endif
            }
        }
#endif
    }

    void CCJGW_LogWindowsView::OnInitWindow()
    {
        //closebtn
#if 1
        m_plogedt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"logedt"));
        m_perrorOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"errorOption"));
        m_pdebugOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"debugOption"));
        mpWkeBrowser = static_cast<CWkeWindowlessUI*>(m_PaintManager.FindControl(L"wkeBrowser"));
        if (mpWkeBrowser)
        {
            ::CoInitialize(NULL);
            wkeInit();
        }
        m_plogedt->SetText(GETPELOG4W().c_str());
#endif
    }
}

