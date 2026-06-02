#include "StdAfx.h"
#include "CJGW_PrsVendorRoamingToolDlg.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CefBrowserUI.h"

namespace JGW
{
    CCJGW_PrsVendorRoamingToolDlg::CCJGW_PrsVendorRoamingToolDlg(void) : mUIController(this)
    {
    }


    CCJGW_PrsVendorRoamingToolDlg::~CCJGW_PrsVendorRoamingToolDlg(void)
    {
    }

    void CCJGW_PrsVendorRoamingToolDlg::OnInitWindow()
    {
        if (!GetUIControl())
        {
            Close(IDCANCEL);
            ::MessageBox(GetHWND(),_T("控件加载失败"),_T("视图初始化错误"),MB_ICONERROR);
            return ;
        }

        {
            mpStartBtn->SetEnabled(false);
#if 0
            mpWkeBrowser->InitBrowser(WM_TIMER_WKE_BROWSER);
#endif
            m_PaintManager.GetEventSource() += MakeDelegate(&mUIController,&CCJGW_PrsVendorRoamingToolDlgController::OnEventCustomMessage,0);
        } 

        if (!mUIController.OnInit())
        {
            Close(IDCANCEL);
            ::MessageBox(GetHWND(),_T("配置文件初始化失败"),_T("初始化错误"),MB_ICONERROR);
            return ;
        }
        {
#if 1
            std::wstring strHtmlFile = JGW_RealativePathToAbsPath(mUIController.GetTestParams().mstrHtmlFile.c_str());
            std::wstring strEncodeHtmlFilePath,strDecodeHtmlFilePath;
            JGW_URLEncodeW(strHtmlFile,strEncodeHtmlFilePath);
            JGW_URLDecodeW(strEncodeHtmlFilePath,strDecodeHtmlFilePath);
            mpWkeBrowser->LoadFile(strEncodeHtmlFilePath.c_str()); 
            //! mpWkeBrowser->SettingMaxListCount(mUIController.GetTestParams().mnSettingMaxListCount);
#endif
        }
    }

    void CCJGW_PrsVendorRoamingToolDlg::OnCloseWindow()
    {
        mUIController.OnCloseWindow();
        m_PaintManager.GetEventSource() -= MakeDelegate(&mUIController,&CCJGW_PrsVendorRoamingToolDlgController::OnEventCustomMessage,0);
    }

    void CCJGW_PrsVendorRoamingToolDlg::OnNotify(TNotifyUI& msg)
    {
        mUIController.OnViewNotify(msg);
    }

    CControlUI* CCJGW_PrsVendorRoamingToolDlg::CreateControl(LPCTSTR pstrClass)
    {
        if(_tcscmp(pstrClass, DUI_CTR_WKEWINDOWLESSBROWSER) == 0)
            return	new CWkeWindowlessUI();
        else if (_tcscmp(pstrClass, CCefBrowserUIItemInterface) == 0)
            return new CCefBrowserUI(m_hWnd);
        return NULL;
    }

    bool CCJGW_PrsVendorRoamingToolDlg::GetUIControl()
    {
        bool bSuccess = true;

        //mpWkeBrowser = static_cast<CWkeWindowlessUI*>(m_PaintManager.FindControl(L"wkeBrowser"));
        //mpWkeBrowser = static_cast<CWkeWindowlessUI*>(m_PaintManager.FindControl(L"wkeBrowser"));
        //! WkePrsStatusBrowser // CefPrsStatusBrowser
        CControlUI* pCControlUI = m_PaintManager.FindControl(L"WkePrsStatusBrowser");
        if (NULL != pCControlUI)
        {
            mpWkeBrowser = (CCJGWPrsStatusBrowserInterface*)((CWkeWindowlessUI*)pCControlUI);
            ((CWkeWindowlessUI*)pCControlUI)->InitBrowser(WM_TIMER_WKE_BROWSER);
        }

        pCControlUI = m_PaintManager.FindControl(L"CefPrsStatusBrowser");
        if (NULL != pCControlUI)
        {
            mpWkeBrowser = (CCJGWPrsStatusBrowserInterface*)((CCefBrowserUI*)pCControlUI);
        }

        mpIperfCommandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"IperfCommandEdt"));
        mpTerminalType = static_cast<CComboUI*>(m_PaintManager.FindControl(L"TerminalType"));
        mpProtocolType = static_cast<CComboUI*>(m_PaintManager.FindControl(L"ProtocolType"));
        mpIperfIPEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"IperfIPEdt"));
        mpSshIpAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"SshIpAddressEdt"));
        mpTestTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TestTimeEdt"));
        mpStartBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"StartBtn"));
        mpStopBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"StopBtn"));
        mpLogEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"LogEdt"));
        mpExtraParameterEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"ExtraParameterEdt"));
        mpUpdateConfigOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"UpdateConfigOption"));
        mpThreadCountEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ThreadCountEdt"));
        mpClientConfigHorizontalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"ClientConfigHorizontalLayout"));

        mpLogPrefixEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"LogPrefixEdt"));
        mpLogFolderBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"LogFolderBtn"));
        mpSaveIPQLogCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"SaveIPQLogCheckBox"));

        mpLogDebugTestCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"LogDebugTestCheckBox"));
        mpLogDebugBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"LogDebugBtn"));
		mpCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"closebtn"));
		bSuccess &= (NULL != mpCloseBtn);
        mpLogDebugFilePathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"LogDebugFilePathEdt"));
        mpLogDebugTestHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"LogDebugTestHorizontalLayout"));



        //mpSwitchTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("SwitchTabLayout")));

		mpApMacCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"ApMacCombo"));
		bSuccess &= (NULL != mpApMacCombo);
		mpApPrsRoamingInfoRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"ApPrsRoamingInfoRichEdit"));
		bSuccess &= (NULL != mpApPrsRoamingInfoRichEdit);

		mpStaMacCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"StaMacCombo"));
		bSuccess &= (NULL != mpStaMacCombo);
		mpConnectApMacEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ConnectApMacEdit"));
		bSuccess &= (NULL != mpConnectApMacEdit);
		mpStaPrsRoamingInfoRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"StaPrsRoamingInfoRichEdit"));
		bSuccess &= (NULL != mpStaPrsRoamingInfoRichEdit);
		mpStaPrsRoamingInfoVerticalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"StaPrsRoamingInfoVerticalLayout"));
		bSuccess &= (NULL != mpStaPrsRoamingInfoVerticalLayout);

        bSuccess &= (NULL != mpWkeBrowser);
        bSuccess &= (NULL != mpIperfCommandEdt);
        bSuccess &= (NULL != mpTerminalType);
        bSuccess &= (NULL != mpProtocolType);
        bSuccess &= (NULL != mpIperfIPEdt);
        bSuccess &= (NULL != mpSshIpAddressEdt);
        bSuccess &= (NULL != mpTestTimeEdt);
        bSuccess &= (NULL != mpStartBtn);
        bSuccess &= (NULL != mpStopBtn);
        bSuccess &= (NULL != mpLogEdt);
        bSuccess &= (NULL != mpExtraParameterEdt);
        bSuccess &= (NULL != mpUpdateConfigOption);
        bSuccess &= (NULL != mpThreadCountEdt);
        bSuccess &= (NULL != mpClientConfigHorizontalLayout);
        bSuccess &= (NULL != mpLogPrefixEdt);
        bSuccess &= (NULL != mpLogFolderBtn);
        bSuccess &= (NULL != mpSaveIPQLogCheckBox);
        bSuccess &= (NULL != mpLogDebugTestCheckBox);
        bSuccess &= (NULL != mpLogDebugBtn);
        bSuccess &= (NULL != mpLogDebugFilePathEdt);

        return bSuccess;
    }
}
