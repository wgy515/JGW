// InputSNDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <JGW_PluginViewLib/LogShowDuidlg.h>
/*#include "RFCalInterface.h"*/

// CInputSNDlg 对话框
std::wstring CLogShowDuidlg::g_strHtmlPath(L"www.baidu.com");

CLogShowDuidlg::CLogShowDuidlg(std::wstring& strDubugLog,std::wstring& strErrorLog,HWND hParentHwnd /* = NULL */,std::wstring& strHtmlPath)
	:m_hParentHwnd(hParentHwnd)
	,m_strDubugLog(strDubugLog)
	,m_strErrorLog(strErrorLog)
	,m_pCloseBtn(NULL)
	,m_plogedt(NULL)
	,m_pMinBtn(NULL)
	,m_pMaxBtn(NULL)
	,m_pRestoreBtn(NULL)
	,m_pdebugOption(NULL)
	,m_perrorOption(NULL)
	,m_strHtmlPath(strHtmlPath)
{
}

// BEGIN_MYWND_MESSAGE_MAP(CLogShowDuidlg,CDuiDialog)
// END_MYWND_MESSAGE_MAP()

CLogShowDuidlg::~CLogShowDuidlg()
{
}

UINT CLogShowDuidlg::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG;
}


LPCTSTR CLogShowDuidlg::GetWindowClassName() const
{
	return L"CLogShowDuidlg";
}


int CLogShowDuidlg::DoModal()
{
#if defined USE_ZIP_RESOURCE_ENABLE
	SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
	CreateDuiDialogWindow(m_hParentHwnd,_T(""),_T("CalLogUISkin.xml"),UI_WNDSTYLE_FRAME);
#elif defined USE_ZIP_FILE_ENABLE
	SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
	CreateDuiDialogWindow(m_hParentHwnd,_T(""),_T("CalLogUISkin.xml"),UI_WNDSTYLE_FRAME);
#else
	CreateDuiDialogWindow(m_hParentHwnd,_T("skin\\JaGuarWave"),_T("CalLogUISkin.xml"),UI_WNDSTYLE_FRAME);
#endif
	CenterWindow();

	return ShowModal();
}

void CLogShowDuidlg::Notify(TNotifyUI& msg)
{
	if ( msg.sType == DUI_MSGTYPE_CLICK)
	{
		if ( m_pCloseBtn == msg.pSender )
		{
			Close(IDCANCEL);
			return;
		}
		if( msg.pSender == m_pCloseBtn /*|| msg.pSender == m_pExitBtn*/ )
		{
			Close(IDCANCEL);//PostQuitMessage(0);		
			return ;
		}

		if ( msg.pSender == m_pdebugOption )
		{
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
			if( pControl ) 	{if( 0 != pControl->GetCurSel() ) pControl->SelectItem(0);}
			else return;
			m_plogedt->SetText( m_strDubugLog.c_str() );
		}

		if ( msg.pSender == m_perrorOption)
		{
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
			if( pControl ) 	{if( 0 != pControl->GetCurSel() ) pControl->SelectItem(0);}
			else return;
			m_plogedt->SetText( m_strErrorLog.c_str() );
		}
		if ( msg.pSender->GetName() == _T("htmlOption") )
		{
			if ( m_strHtmlPath.empty() ) return;
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
			if( pControl ) 	{if( 1 != pControl->GetCurSel() ) pControl->SelectItem(1);}
			else return;
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
		}

		if (msg.pSender == m_pMaxBtn)
		{
			SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);
			return;
		}
		if (msg.pSender == m_pMinBtn)
		{
			SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
			return;
		}

		if (msg.pSender == m_pRestoreBtn)
		{
			SendMessage(WM_SYSCOMMAND,SC_RESTORE);
			return;
		}
	}
}

void CLogShowDuidlg::OnBnClickedOK()
{
}

bool CLogShowDuidlg::InitWindow()
{
	//closebtn
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(CLOSEBTN));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(MAXBTN));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(MINBTN));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(RESTORBTN));
	m_plogedt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"logedt"));

	m_perrorOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"errorOption"));
	m_pdebugOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"debugOption"));


	m_plogedt->SetText( m_strErrorLog.c_str());

	return true;
}
