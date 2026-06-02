#include "StdAfx.h"
#include "CTSETestPluginUI.h"
#include "CTSETestPluginWnd.h"
namespace JGW
{
    CCTSETestPluginUI::CCTSETestPluginUI(void) : m_pWindow(NULL)
    {
    }


    CCTSETestPluginUI::~CCTSETestPluginUI(void)
    {
        OutputDebugString(L"CCTSETestPluginUI::~CCTSETestPluginUI\n");
    }

    void CCTSETestPluginUI::SetModuleFolderSuitName(const std::wstring& strModuleFolder,const std::wstring& strSulteName)
    {
        mstrModuleFolder = strModuleFolder;
        mstrSuiteName = strSulteName;
    }

    void CCTSETestPluginUI::CloseTSETestPluginWnd()
    {
#if 0
        m_pWindow->Close(0);
#else
		if (IsWindow(mhWindow))
		{
			m_pWindow->Close(0);
		}
#endif
    }

    LPCTSTR CCTSETestPluginUI::GetClass() const
    {
        return _T("TSETestPluginUI");
    }

    LPVOID CCTSETestPluginUI::GetInterface(LPCTSTR pstrName)
    {
        if( _tcscmp(pstrName, _T("TSETestPlugin")) == 0 ) return static_cast<CCTSETestPluginUI*>(this);
        return CControlUI::GetInterface(pstrName);
    }

    void CCTSETestPluginUI::SetInternVisible(bool bVisible /* = true */)
    {
        CControlUI::SetInternVisible(bVisible);
        ::ShowWindow(m_pWindow->GetHWND(), bVisible?SW_SHOW:SW_HIDE); 
    }

    void CCTSETestPluginUI::DoInit()
    {
        m_pWindow = new CCTSETestPluginWnd();
        if(m_pWindow)
        {
            m_pWindow->SetModuleFolderSuitName(mstrModuleFolder,mstrSuiteName);
            m_pWindow->Init(this);
            m_pWindow->ShowWindow();
        }	
    }

    void CCTSETestPluginUI::SetPos(RECT rc)
    {
		mhWindow = m_pWindow->GetHWND();
        CControlUI::SetPos(rc);	

		//CDuiString strRect;
		//strRect.Format(L"SetPos : (l-r-t-b)%d-%d-%d-%d\n",rc.left,rc.right,rc.top,rc.bottom);
		//OutputDebugString(strRect.GetData());

		POINT point;
		point.x = rc.left;
		point.y = rc.top;
		//! 将客户坐标转化为屏幕坐标系
		if (TRUE == ::ClientToScreen(this->GetManager()->GetPaintWindow(), &point))
		{
			//strRect.Format(L"ClientToScreen : (x-y)%d-%d\n",point.x,point.y);
			//OutputDebugString(strRect.GetData());
		}
#if 1
        ::SetWindowPos(m_pWindow->GetHWND(), NULL, point.x,point.y/*rc.left, rc.top*/, rc.right - rc.left, 
            rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
#else
		::MoveWindow(m_pWindow->GetHWND(),rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,TRUE);
#endif
    }


}
