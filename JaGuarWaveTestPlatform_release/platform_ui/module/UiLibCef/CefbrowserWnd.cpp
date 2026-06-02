#include "StdAfx.h"
#include "CefbrowserWnd.h"

namespace JGW
{
    CCefbrowserWnd::CCefbrowserWnd(const RECT &rect) : m_rect(rect),m_iBrowserID(-1)
    {
    }


    CCefbrowserWnd::~CCefbrowserWnd(void)
    {
    }

    void CCefbrowserWnd::Init()
    {

    }
    CDuiString CCefbrowserWnd::GetSkinFile()
    {
        return _T("CefbrowserLine.xml");
    }
    LPCTSTR CCefbrowserWnd::GetWindowClassName(void) const
    {
        return CEFBROWSER_LINE_WND;
    }

    void CCefbrowserWnd::InitWindow()
    {
        ::SetWindowPos(*this, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
        Init();
    }

    void CCefbrowserWnd::OnFinalMessage(HWND hWnd)
    {
        WindowImplBase::OnFinalMessage(hWnd);
    }

    LRESULT CCefbrowserWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        //----------------------------------------
        //屏蔽Alt + F4 系统关闭消息
        //----------------------------------------
        if (WM_SYSCOMMAND == uMsg && SC_CLOSE == wParam)
        {
            bHandled = TRUE;
            return 0;
        }


        return WindowImplBase::OnSysCommand(uMsg, wParam, lParam, bHandled);
    }

    LRESULT CCefbrowserWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = TRUE;
        return WindowImplBase::OnClose(uMsg, wParam, lParam, bHandled);
    }

    LRESULT CCefbrowserWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT ret = 0;
        BOOL bHandle = TRUE;
        if (uMsg == WM_KEYDOWN && wParam == VK_RETURN)
            return ret;

        switch (uMsg)
        {
        case WM_SIZE:        
            {
                ret = OnSize(uMsg, wParam, lParam, bHandle);
                OnSizeChange();
                break; 
            }
        case WM_SETBROWSER_ID:
            {
                m_iBrowserID = (INT32)lParam;
                //duilibCef::RegisterCppFunc(m_iBrowserID, JSCallCppFunc_Name[JS_CALL_CPP_COUNT_REFRESH]);
                //duilibCef::RegisterCppFunc(m_iBrowserID, JSCallCppFunc_Name[JS_CALL_CPP_COUNT_SWITCH]);
                break;
            }
        case WM_LOAD_CEF:
            {
                OnLoadCEFWnd();
                break;
            }
        case WM_EXIT_THREAD:
            {
                duilibCef::CloseBrowserByID(m_iBrowserID);
                ::SendMessage(GetHWND(), WM_CLOSE, 0, 0);
                break;
            }
        case WM_CLOSE:
            {
                ret = OnClose(uMsg, wParam, lParam, bHandle);
                break;
            }
        case MESSAGE_JSCALLCPP_SHOWDATA:
            {
                const std::wstring* pWstrJsCallCppData = (std::wstring*)(lParam);
                m_pRichEditjsCallcppData->SetText(pWstrJsCallCppData->c_str());
                if (NULL!=pWstrJsCallCppData)
                {
                    delete pWstrJsCallCppData;
                    pWstrJsCallCppData = NULL;
                }
                break;
            }
        case MESSAGE_CPPCALLJS_SHOWDATA:
            {
                const std::wstring* pWstrCppCallJsData = (std::wstring*)(lParam);
                m_pRichEditcppCalljsData->SetText(pWstrCppCallJsData->c_str());
                if (NULL != pWstrCppCallJsData)
                {
                    delete pWstrCppCallJsData;
                    pWstrCppCallJsData = NULL;
                }
                break;
            }
#if 0
        case MESSAGE_CEFCALLERROR_SHOWDATA:
            {
                const std::wstring* pWstrCEFCallErrorData = (std::wstring*)(lParam);
                //std::wstring wstrTableText = L"初始化失败     ";
                std::wstring wstrTableTitle = L"错误";
                UseTable(En_TABLE_OK, *pWstrCEFCallErrorData, wstrTableTitle);
                if (NULL != pWstrCEFCallErrorData)
                {
                    delete pWstrCEFCallErrorData;
                    pWstrCEFCallErrorData = NULL;
                }
                break;
            }
#endif
        default:
            bHandle = FALSE;
        }
        if (bHandle) return ret;

        return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
    }



    UINT CCefbrowserWnd::UseTable(const INT32 &ichoose, const std::wstring& strText, const std::wstring& wstrTitle/* = L"警告"*/)
    {
        UINT ret = 0;

        CTableWnd *pTableWnd = new CTableWnd(ichoose, strText, wstrTitle);  //确认弹窗
        if (NULL == pTableWnd)
            return ret;

        pTableWnd->Create(GetHWND(), _T("tableWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);


        //!< 提示弹窗界面居中
        pTableWnd->CenterWindow();

        ret = pTableWnd->ShowModal();
        if (NULL != pTableWnd)
        {
            delete pTableWnd;
            pTableWnd = NULL;
        }
        return ret;
    }

    void CCefbrowserWnd::chgWndPos(RECT rect)
    {
        //LOG_INFORMATION("[CMainWnd][InitWindow]rectCefLayout.left = %d, top=%d, right=%d, bottom=%d", (INT32)(rect.left), (INT32)(rect.top), (INT32)(rect.right), (INT32)(rect.bottom));
        if (rect.left != rect.right
            && rect.top != rect.bottom)
        {
            MoveWindow(GetHWND(),
                rect.left,
                rect.top,
                1000,
                500,
                true);

            ::PostMessage(GetHWND(), WM_PAINT, 0, 0);
        }
    }
    void CCefbrowserWnd::OnLoadCEFWnd()
    {
        bool ret = false;
        RECT rtVerticalLayoutCEF = m_pCVerticalLayoutCEF->GetPos();

        std::wstring wstrCurrentPath = L"";

        wstrCurrentPath = CUtility::getInstance()->GetFullPathW(false);

        std::wstring  wstrCurrentHtmlPath = wstrCurrentPath + L"html\\" + CEFLINE_HTML;

        HWND hwnd = GetHWND();
        ret = duilibCef::LoadCef(hwnd,wstrCurrentHtmlPath, rtVerticalLayoutCEF,true);
        if (ret)
        {
            //cefOperation::RegisterCppFunc(m_iBrowserID,JSCallCppFunc_Name[JS_CALL_CPP_COUNT_REFRESH]);
            //cefOperation::RegisterCppFunc(m_iBrowserID,JSCallCppFunc_Name[JS_CALL_CPP_COUNT_SWITCH]);

            /*cefOperation::RegCallBkFunForJsCallCppBk((CCefCallBackFunc::JsCallCppBKFunc));
            cefOperation::RegCallBkFunForCppCallJsBk((CCefCallBackFunc::CppCallJsBKFunc));
            cefOperation::RegCallBkFunForErrorInfo((CCefCallBackFunc::FuncErrorBk));
            cefOperation::RegCallBkFunForBrowserId(FuncCefBrowserID);
            */
        }
    }

    LRESULT CCefbrowserWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {

        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return WindowImplBase::OnSize(uMsg, wParam, lParam, bHandled);
    }
    void CCefbrowserWnd::OnSizeChange()
    {
        if ((-1) != m_iBrowserID)
        {
            RECT rtVerticalLayoutCEF = m_pCVerticalLayoutCEF->GetPos();
            duilibCef::MoveBrowserWindow(m_iBrowserID, rtVerticalLayoutCEF);
        }
    }
}