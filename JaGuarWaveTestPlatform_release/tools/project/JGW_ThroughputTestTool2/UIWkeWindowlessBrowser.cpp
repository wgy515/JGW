#include "StdAfx.h"
#include "UIWkeWindowlessBrowser.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#pragma comment(lib,"wke.lib")
namespace JGW
{
    CWkeWindowlessUI::CWkeWindowlessUI(void):m_webView(NULL)
    {
    }


    CWkeWindowlessUI::~CWkeWindowlessUI(void)
    {
    }

    LPCTSTR CWkeWindowlessUI::GetClass() const
    {
        return _T("WkeWindowlessUI");
    }

    LPVOID CWkeWindowlessUI::GetInterface(LPCTSTR pstrName)
    {
        if( _tcscmp(pstrName, DUI_CTR_WKEWINDOWLESSBROWSER) == 0 ) return static_cast<CWkeWindowlessUI*>(this);
        return CControlUI::GetInterface(pstrName);
    }


    void CWkeWindowlessUI::DoEvent(TEventUI& event)
    {
        RECT rc = GetPos();
        POINT pt = {event.ptMouse.x - rc.left, event.ptMouse.y - rc.top};

#if 1
        switch (event.Type)
        {
        case UIEVENT_MOUSEENTER:
        case UIEVENT_MOUSELEAVE:
        case UIEVENT_MOUSEMOVE:
            {
                unsigned int flags = 0;
                if (event.wParam & MK_CONTROL)
                    flags |= WKE_CONTROL;
                if (event.wParam & MK_SHIFT)
                    flags |= WKE_SHIFT;
                if (event.wParam & MK_LBUTTON)
                    flags |= WKE_LBUTTON;
                if (event.wParam & MK_MBUTTON)
                    flags |= WKE_MBUTTON;
                if (event.wParam & MK_RBUTTON)
                    flags |= WKE_RBUTTON;
                m_webView->mouseEvent(WKE_MSG_MOUSEMOVE, pt.x, pt.y, flags);		
            }		
            break;
        case UIEVENT_BUTTONDOWN:
            m_webView->mouseEvent(WKE_MSG_LBUTTONDOWN, pt.x, pt.y, event.wKeyState);	
            break;
        case UIEVENT_BUTTONUP:
            m_webView->mouseEvent(WKE_MSG_LBUTTONUP, pt.x, pt.y, event.wKeyState);				
            break;
        case UIEVENT_RBUTTONDOWN:
            m_webView->mouseEvent(WKE_MSG_RBUTTONDOWN, pt.x, pt.y, event.wKeyState);
            break;
        case UIEVENT_RBUTTONUP:
            m_webView->mouseEvent(WKE_MSG_RBUTTONUP, pt.x, pt.y, event.wKeyState);
            break;
        case UIEVENT_DBLCLICK:
            m_webView->mouseEvent(WKE_MSG_LBUTTONDBLCLK, pt.x, pt.y, event.wKeyState);			
            break;
        case UIEVENT_SCROLLWHEEL:
            m_webView->mouseWheel(pt.x, pt.y, event.wParam == SB_LINEUP ? 120 : -120, event.wKeyState);		
            break;
        case UIEVENT_KEYDOWN:
            m_webView->keyDown(event.chKey, event.lParam, false);		
            break;
        case UIEVENT_KEYUP:
            m_webView->keyUp(event.chKey, event.lParam, false);		
            break;	
        case UIEVENT_CHAR:
            {
                unsigned int charCode = event.chKey;
                unsigned int flags = 0;
                if (HIWORD(event.lParam) & KF_REPEAT)
                    flags |= WKE_REPEAT;
                if (HIWORD(event.lParam) & KF_EXTENDED)
                    flags |= WKE_EXTENDED;

                //flags = HIWORD(lParam);

                m_webView->keyPress(charCode, flags, false);
            }
            break;
        case UIEVENT_SETFOCUS:
            m_webView->focus();
            break;
        case UIEVENT_KILLFOCUS:
            m_webView->unfocus();	
            break;	
        case UIEVENT_SETCURSOR:
            return;		
        case UIEVENT_CONTEXTMENU:
            {
                unsigned int flags = 0;
                if (event.wParam & MK_CONTROL)
                    flags |= WKE_CONTROL;
                if (event.wParam & MK_SHIFT)
                    flags |= WKE_SHIFT;
                m_webView->contextMenuEvent(pt.x, pt.y, flags);		
                break;	
            }
        case UIEVENT_TIMER:
            if(event.wParam == mnTimerID) {			
                //! Invalidate();
                m_webView->tick();
            }
            break;
        case UIEVENT_IME_STARTCOMPOSITION:
            {
                wkeRect caret = m_webView->getCaret();

                CANDIDATEFORM form;
                form.dwIndex = 0;
                form.dwStyle = CFS_EXCLUDE;
                form.ptCurrentPos.x = caret.x ;
                form.ptCurrentPos.y = caret.y + caret.h;
                form.rcArea.top = caret.y + m_rcPaint.top;
                form.rcArea.bottom = caret.y + caret.h +m_rcPaint.top;
                form.rcArea.left = caret.x +m_rcPaint.left;
                form.rcArea.right = caret.x + caret.w +m_rcPaint.left;

                HIMC hIMC = ImmGetContext(m_pManager->GetPaintWindow());
                ImmSetCandidateWindow(hIMC, &form);
                ImmReleaseContext(m_pManager->GetPaintWindow(), hIMC);
            }
            break;
        default: break;
        }
        CControlUI::DoEvent(event);
#else

        bool handled = true;
        if (!m_webView)
        {
            return CControlUI::DoEvent(event);
        }
        if (event.Type == UIEVENT_TIMER)
        {
            m_webView->tick();
        }

        else if (event.Type == UIEVENT_BUTTONDOWN)
        {
            m_webView->focus();
            unsigned int flags = 0;

            if (event.wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (event.wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (event.wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (event.wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (event.wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            //flags = wParam;
            handled = m_webView->mouseEvent(WM_LBUTTONDOWN, pt.x, pt.y, flags);
        }else if (event.Type == UIEVENT_BUTTONUP)
        {
            unsigned int flags = 0;

            if (event.wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (event.wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (event.wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (event.wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (event.wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            handled = m_webView->mouseEvent(WM_LBUTTONUP, pt.x, pt.y, flags);
        }else if (event.Type == UIEVENT_MOUSEMOVE)
        {
            unsigned int flags = 0;

            if (event.wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (event.wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (event.wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (event.wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (event.wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            m_webView->mouseEvent(WKE_MSG_MOUSEMOVE, pt.x, pt.y, flags);
        }else if (event.Type == UIEVENT_RBUTTONDOWN)
        {
            m_webView->focus();
            unsigned int flags = 0;

            if (event.wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (event.wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (event.wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (event.wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (event.wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            handled = m_webView->mouseEvent(WM_RBUTTONDOWN, pt.x, pt.y, flags);
        }
        else if (event.Type == UIEVENT_RBUTTONUP)
        {
            int x = GET_X_LPARAM(event.lParam);
            int y = GET_Y_LPARAM(event.lParam);

            unsigned int flags = 0;

            if (event.wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (event.wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (event.wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (event.wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (event.wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;
            x -=m_rcPaint.left;
            y -=m_rcPaint.top;
            handled = m_webView->mouseEvent(WM_RBUTTONUP, x, y, flags);
        }
        else if (event.Type == UIEVENT_SCROLLWHEEL)
        {
            //             POINT pt ;
            //             pt.x = event.ptMouse.x;
            //             pt.y = event.ptMouse.y;
            //             pt.x -= m_rcPaint.left;
            //             pt.y -= m_rcPaint.top;
            //             ScreenToClient(m_pManager->GetPaintWindow(), &pt);

            int delta = GET_WHEEL_DELTA_WPARAM(event.wParam);

            unsigned int flags = 0;

            if (event.wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (event.wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (event.wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (event.wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (event.wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            handled = m_webView->mouseWheel(pt.x,pt.y, delta, flags);
        }else if (event.Type == UIEVENT_KEYDOWN)
        {
            unsigned int virtualKeyCode = event.wParam;
            unsigned int flags = 0;
            if (HIWORD(event.lParam) & KF_REPEAT)
                flags |= WKE_REPEAT;
            if (HIWORD(event.lParam) & KF_EXTENDED)
                flags |= WKE_EXTENDED;

            //flags = HIWORD(lParam);

            handled = m_webView->keyDown(virtualKeyCode, flags, false);
        }else if (event.Type == UIEVENT_KEYUP)
        {
            unsigned int virtualKeyCode = event.wParam;
            unsigned int flags = 0;
            if (HIWORD(event.lParam) & KF_REPEAT)
                flags |= WKE_REPEAT;
            if (HIWORD(event.lParam) & KF_EXTENDED)
                flags |= WKE_EXTENDED;

            //flags = HIWORD(lParam);

            handled = m_webView->keyUp(virtualKeyCode, flags, false);
        }else if (event.Type == UIEVENT_CHAR)
        {
            unsigned int charCode = event.chKey;
            unsigned int flags = 0;
            if (HIWORD(event.lParam) & KF_REPEAT)
                flags |= WKE_REPEAT;
            if (HIWORD(event.lParam) & KF_EXTENDED)
                flags |= WKE_EXTENDED;

            //flags = HIWORD(lParam);

            handled = m_webView->keyPress(charCode, flags, false);
        }else if (event.Type == UIEVENT_IME_STARTCOMPOSITION)
        {
            wkeRect caret = m_webView->getCaret();

            CANDIDATEFORM form;
            form.dwIndex = 0;
            form.dwStyle = CFS_EXCLUDE;
            form.ptCurrentPos.x = caret.x ;
            form.ptCurrentPos.y = caret.y + caret.h;
            form.rcArea.top = caret.y + m_rcPaint.top;
            form.rcArea.bottom = caret.y + caret.h +m_rcPaint.top;
            form.rcArea.left = caret.x +m_rcPaint.left;
            form.rcArea.right = caret.x + caret.w +m_rcPaint.left;

            HIMC hIMC = ImmGetContext(m_pManager->GetPaintWindow());
            ImmSetCandidateWindow(hIMC, &form);
            ImmReleaseContext(m_pManager->GetPaintWindow(), hIMC);
        }
        else if (event.Type ==UIEVENT_SETFOCUS)
        {
            m_webView->focus();
        }
        else if (event.Type == UIEVENT_KILLFOCUS)
        {
            m_webView->unfocus();
        }

        if (!handled)
        {
            // CControlUI::DoEvent(event);
        }
#endif
    }


    void CWkeWindowlessUI::PaintBkImage(HDC hDC)
    {
        CControlUI::PaintStatusImage(hDC);
        CDuiRect rect(m_rcItem);

        m_webView->paint(hDC,rect.left,rect.top,rect.GetWidth(),rect.GetHeight(),0,0,true);
    }

    void CWkeWindowlessUI::SetPos(RECT rc)
    {
        CControlUI::SetPos(rc);
        CDuiRect rect(m_rcItem);
        if (m_webView)
        {
            //! jsEvalW(m_webView->globalExec(),L"updateName()");
            m_webView->resize(rect.GetWidth(),rect.GetHeight());
            m_webView->tick();
        }

    }
    void CWkeWindowlessUI::InitBrowser(UINT nTimerID)
    {
        m_webView = wkeCreateWebView();
        m_webView->setTransparent(false);


        // 			wkeClientHandler handler;
        // 			handler.onTitleChanged = NULL;
        // 			handler.onURLChanged = NULL;
        // 			m_webView->setClientHandler(&handler);


        m_webView->setBufHandler((_wkeBufHandler*)this);

        CDuiRect rect(m_rcItem);
        m_webView->resize(rect.GetWidth(),rect.GetHeight());


        //	::SetTimer(m_hWnd,nTimerID,50,NULL);
        mnTimerID = nTimerID;
        m_pManager->SetTimer(this,nTimerID,50);

    }
    void CWkeWindowlessUI::onBufUpdated (const HDC hdc,int x, int y, int cx, int cy)
    {
        this->Invalidate();
    }


    void CWkeWindowlessUI::LoadUrl(LPCTSTR szUrl)
    {
        m_webView->loadURL(szUrl);
    }

    void CWkeWindowlessUI::LoadFile(LPCSTR szFile)
    {
        m_webView->loadFile(szFile);
    }

    void CWkeWindowlessUI::LoadFile(LPCTSTR szFile)
    {
        m_webView->loadFile(szFile);
    }

    /*void CWkeWindowlessUI::UpdateBpsInfo(size_t nIndex,double speed)
    {
    std::wstring strTemp;
    JGW_FormatWString(strTemp,L"addData(%d,%.2lf)",nIndex,speed);
    jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }

    void CWkeWindowlessUI::UpdateBpsInfo(size_t nIndex,int angle,double speed)
    {
    std::wstring strTemp;
    JGW_FormatWString(strTemp,L"addDataAngle(%d,%d,%.2lf)",nIndex,angle,speed);
    jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }

    void CWkeWindowlessUI::UpdateBpsInfo(size_t nIndex,const std::wstring& angle,double speed)
    {
    std::wstring strTemp;
    JGW_FormatWString(strTemp,L"addDataAngle(%d,%s,%.2lf)",nIndex,angle.c_str(),speed);
    jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }*/

    void CWkeWindowlessUI::UpdateAngleMbps(const std::wstring& angle,double speed)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"addAngleMbps(%s,%.2lf)",angle.c_str(),speed);
        jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }

    void CWkeWindowlessUI::UpdateAngleMbps(int angle,double speed)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"addAngleMbps(%d,%.2lf)",angle,speed);
        jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }

    void CWkeWindowlessUI::ClearBpsInfo()
    {
        jsEvalW(m_webView->globalExec(),L"clearData()");
    }

    void CWkeWindowlessUI::UpdateThrouphputRateSpeed(size_t index,const std::wstring& strRate,const std::wstring& strSpeed)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"UpdateThrouphputRateSpeed(%u,%s,%s)",index,strRate.c_str(),strSpeed.c_str());
        jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }

    void CWkeWindowlessUI::UpdateThrouphputRateSpeed(size_t index,double fRate,double fSpeed)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"UpdateThrouphputRateSpeed(%u,%.2lf,%.2lf)",index,fRate,fSpeed);
        jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }
    
    void CWkeWindowlessUI::UpdateThrouphputRateSpeed(size_t index,float fRate,float fSpeed)
    {
        //jsEvalW(m_webView->globalExec(),L"ClearThrouphputRateSpeed()");
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"UpdateThrouphputRateSpeed(%u,%.2f,%.2f)",index,fRate,fSpeed);
        jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }

    void CWkeWindowlessUI::ClearThrouphputRateSpeed()
    {
        jsEvalW(m_webView->globalExec(),L"ClearThrouphputRateSpeed()");
    }
    
    void CWkeWindowlessUI::SettingMaxListCount(int nMaxListCount)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"SettingMaxListCount(%d)",nMaxListCount);
        jsEvalW(m_webView->globalExec(),strTemp.c_str());
    }

    //void CWkeWindowlessUI::UpdateAngleMbps(const std::wstring& angle,double speed)
    //{
    //    std::wstring strTemp;
    //    JGW_FormatWString(strTemp,L"addAngleMbps(%s,%.2lf)",angle.c_str(),speed);
    //    jsEvalW(m_webView->globalExec(),strTemp.c_str());
    //}

    //void CWkeWindowlessUI::UpdateAngleMbps(int angle,double speed)
    //{
    //    std::wstring strTemp;
    //    JGW_FormatWString(strTemp,L"addAngleMbps(%d,%.2lf)",angle,speed);
    //    jsEvalW(m_webView->globalExec(),strTemp.c_str());
    //}

    //void CWkeWindowlessUI::ClearBpsInfo()
    //{
    //    jsEvalW(m_webView->globalExec(),L"clearData()");
    //}
}