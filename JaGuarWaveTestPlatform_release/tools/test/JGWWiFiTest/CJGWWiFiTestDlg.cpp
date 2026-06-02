#include "StdAfx.h"
#include "CJGWWiFiTestDlg.h"
#include "JGWWiFiTestConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <fstream>

namespace JGW
{
    CCJGWWiFiTestDlg::CCJGWWiFiTestDlg(void):m_dwTime(0),m_nListItemHeight(30)
    {
    }


    CCJGWWiFiTestDlg::~CCJGWWiFiTestDlg(void)
    {
    }

    void CCJGWWiFiTestDlg::OnInitWindow()
    {
        //! UI
        {
            bool bSuccess = true;

            m_pListCtrlUi = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(L"domainlist"));
            m_pStartBtnUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"startBtn"));
            m_pTestStatusBtnUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"testStatusBtn"));
            m_pTestTimeBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"testTimeBtn"));
            m_pTestInfoText = static_cast<CTextUI*>(m_PaintManager.FindControl(L"testInfo"));
            bSuccess &= (NULL != m_pListCtrlUi);
            bSuccess &= (NULL != m_pStartBtnUI);
            bSuccess &= (NULL != m_pTestStatusBtnUI);
            bSuccess &= (NULL != m_pTestTimeBtn);
            bSuccess &= (NULL != m_pTestInfoText);

            if ( !bSuccess )
            {
                MessageBox(m_PaintManager.GetPaintWindow(),_T("Æ¤·ô¿Ø¼þÈ±Ê§"),_T("³õÊ¼»¯Ê§°Ü"),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            }
        }
        //! INIT UI 
        {
            int nWidth1 = m_pListCtrlUi->GetPos().right - m_pListCtrlUi->GetPos().left;
            m_pListCtrlUi->SetAttribute(L"itemalign",L"center");
            m_pListCtrlUi->GetList()->RemoveAll();
            m_pListCtrlUi->GetHeader()->SetScaleHeader(true);

            m_pListCtrlUi->GetHeader()->GetItemAt( 0 )->SetText(_T("²âÊÔÏî"));
            m_pListCtrlUi->GetHeader()->GetItemAt( 1 )->SetText(_T("Min"));
            m_pListCtrlUi->GetHeader()->GetItemAt( 2 )->SetText(_T("Value"));
            m_pListCtrlUi->InsertColumn(3,_T("Max"),0);
            m_pListCtrlUi->InsertColumn(4,_T("P/F"),0);

            m_pListCtrlUi->GetHeader()->GetItemAt( 0 )->SetScale( 30 );
            m_pListCtrlUi->GetHeader()->GetItemAt( 1 )->SetScale( 20 );
            m_pListCtrlUi->GetHeader()->GetItemAt( 2 )->SetScale( 20 );
            m_pListCtrlUi->GetHeader()->GetItemAt( 3 )->SetScale( 20 );
            m_pListCtrlUi->GetHeader()->GetItemAt( 4 )->SetScale( 10 );
        }
        {
            JGWWiFiTestConfig wifiTestConfig;
            std::string strFilePath;
            std::string strCacheConfigFilePath = "wifi_param.xml";
            std::string strConfigFilePath = "wifi_param.xml";

            JGW_GetModuleWorkPathA(strFilePath);
            strCacheConfigFilePath = strFilePath + "cache\\JGWWiFiTest\\wifi_param.xml";
            strConfigFilePath = strFilePath + "config\\JGWWiFiTest\\wifi_config.xml";

            if (!wifiTestConfig.LoadWiFiTestConfigXML(m_sWiFiTestConfig,strConfigFilePath)
                || !wifiTestConfig.LoadWiFiTestCacheXML(m_sWiFiTestConfig,strCacheConfigFilePath))
            {
                MessageBox(m_PaintManager.GetPaintWindow(),_T("ÅäÖÃÎÄ¼þ¼ÓÔØÊ§°Ü!!!"),_T("³õÊ¼»¯Ê§°Ü"),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            }

        }

        {
            if (!m_cJGWWiFiTestThread.CreateMessageThread(0,0,true))
            {
                MessageBox(m_PaintManager.GetPaintWindow(),_T("²âÊÔÏß³Ì¿ªÆôÊ§°Ü!!!"),_T("³õÊ¼»¯Ê§°Ü"),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            }
            m_cJGWWiFiTestThread.PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(this),LPARAM(&m_sWiFiTestConfig));
        }

        m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGWWiFiTestDlg::OnEventCustomMessage,0);
    }

    void CCJGWWiFiTestDlg::OnNotify(TNotifyUI& msg)
    {
        if(msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == m_pStartBtnUI)
            {
                StartWiFiTest();
            }
        }
    }

    void CCJGWWiFiTestDlg::OnCloseWindow()
    {
        m_cJGWWiFiTestThread.TerminateMessageThread();
        KillTimer(GetHWND(),WM_TIMER_UPDATE_TEST_TIME);
    }

    bool CCJGWWiFiTestDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_TIMER:
            OnUpdateTestTimeMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_TEST_PASS_MSG:
            OnWiFiTestResult(true);
            break;
        case WM_TEST_FAIL_MSG:
            OnWiFiTestResult(false);
            break;
        case WM_HWND_TEST_LIST_INFO_OK_MSG:
        case WM_HWND_TEST_LIST_INFO_ERROR_MSG:
            OnUpdateTestListMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        }
        return true;
    }

    void CCJGWWiFiTestDlg::StartWiFiTest()
    {
        m_pStartBtnUI->SetEnabled(false);
        SetTimer(GetHWND(),WM_TIMER_UPDATE_TEST_TIME,1000,NULL);
        m_dwTime = 0;
        m_pListCtrlUi->HomeUp();
        m_pListCtrlUi->SetAllUnSelectedItem();
        m_pListCtrlUi->RemoveAllShow();
        m_pListCtrlUi->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        m_cJGWWiFiTestThread.PostThreadMessage(WM_TEST_THREAD_MSG_RUN);
        m_pTestInfoText->SetText(_T("ÕýÔÚ²âÊÔÖÐ.........."));
        m_pTestStatusBtnUI->SetTextColor(BULE_BK_COLOR);
        m_pTestStatusBtnUI->SetText(_T("BUSY"));    
    }

    void CCJGWWiFiTestDlg::OnUpdateTestTimeMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_TEST_TIME != (int)wParam) return ;
        std::wstring sztime(_T(""));
        m_dwTime++;
        if (m_dwTime > 60 * 60) m_dwTime = 0;
        FormatWString(sztime,L"%02d:%02d",m_dwTime / 60,m_dwTime % 60);
        m_pTestTimeBtn->SetText(sztime.c_str());
    }

    void CCJGWWiFiTestDlg::OnWiFiTestResult(bool bResult)
    {
        m_pStartBtnUI->SetEnabled(true);
        KillTimer(GetHWND(),WM_TIMER_UPDATE_TEST_TIME);
        if (!bResult)
        {
            m_pTestInfoText->SetText(_T("²âÊÔÊ§°Ü£¡£¡£¡"));
            m_pListCtrlUi->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
            m_pTestInfoText->SetTextColor(0xff0000);
            m_pTestStatusBtnUI->SetTextColor(RED_BK_COLOR);
            m_pTestStatusBtnUI->SetText(_T("FAIL"));
        }
        else
        {
            m_pTestInfoText->SetText(_T("²âÊÔ³É¹¦...."));
            m_pTestInfoText->SetTextColor(0x00ff00);
            m_pTestStatusBtnUI->SetTextColor(PASS_COLOR);
            m_pTestStatusBtnUI->SetText(_T("PASS"));
            OnSaveTest();
        }
    }

    void CCJGWWiFiTestDlg::OnUpdateTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        static std::vector<std::wstring> _vListCtrlMsg;
        LPCTSTR strListCtrlMsg = (LPCTSTR)wParam;
        bool bInsertItem = ((int)lParam == 1);
        _vListCtrlMsg.clear();
        CListTextElementUI* m_pListTextElementUi = NULL;

        JGW_ParserStrW(strListCtrlMsg,_T("@"),_vListCtrlMsg);
        if ( bInsertItem )
        {
            m_pListTextElementUi = m_pListCtrlUi->InsertItem( m_pListCtrlUi->GetList()->GetCount(),_vListCtrlMsg.at(0).c_str(),m_nListItemHeight,m_PaintManager);
            m_pListCtrlUi->SetNeedEndDown(true);
        }
        else
        {
            m_pListTextElementUi = m_pListCtrlUi->GetListTextElementItemClass( m_pListCtrlUi->GetList()->GetCount() - 1);
        }
        if( !m_pListTextElementUi ) return ;


        for (size_t i = 0;i < _vListCtrlMsg.size();i ++)
        {
            m_pListTextElementUi->SetText(i,_vListCtrlMsg.at(i).c_str());
        }

        if ( std::wstring::npos != _vListCtrlMsg.at(_vListCtrlMsg.size() - 1).find(_T('F')) )
        {
            m_pListTextElementUi->Select();
            m_pListCtrlUi->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        }    
    } 

    void CCJGWWiFiTestDlg::OnSaveTest()
    {
        std::wofstream fileStream;
        std::wstring strExcelContent;
        CListTextElementUI* pListTextElementUi = NULL;

        std::wstring strTemp,strFilePath;
        JGW_GetExecSubFolder(strTemp,L"log");
        JGW_FormatWString(strFilePath,L"%s%d.lof",strTemp.c_str(),GetTickCount());
        //! GetTickCount
        fileStream.imbue( std::locale("chs") );
        fileStream.open( strFilePath.c_str(),std::ios_base::out | std::ios_base::trunc );

        if (!fileStream.is_open()) return ;

        fileStream << L"²âÊÔÏî\tMin\tValue\tMax\tP/F" << std::endl;

        for (int i = 0;i < m_pListCtrlUi->GetList()->GetCount();i ++)
        {
            pListTextElementUi = (CListTextElementUI*)m_pListCtrlUi->GetList()->GetItemAt(i);

            for (int index = 0;index < 5;index ++)
            {
                fileStream << pListTextElementUi->GetText(index) << L"\t";
            }
            fileStream <<  std::endl;
        }
//         std::vector<std::wstring>& vdata_col = mvSelectExcelColumnData[_T(DATA_COLUMN_NAME)];
//         std::vector<std::wstring>& vcounts_col = mvSelectExcelColumnData[_T(COUNTS_COLUMN_NAME)];
//         for (size_t i = 0;i < vdata_col.size();i++)
//         {
//             fileStream << vcounts_col.at(i) << L"\t" << vdata_col.at(i) << std::endl;
//         }



        fileStream.flush();
        fileStream.close();
        return ;
    }
}

