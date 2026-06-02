#include "StdAfx.h"
#include "CJGW_TestView.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_TestView::CCJGW_TestView(void):mpPaintManagerUI(NULL),mdwTestTime(0),mpTestRecordImpl(NULL),mnTestPassCount(0),mnTestCounts(0)
    {
    }


    CCJGW_TestView::~CCJGW_TestView(void)
    {
        KillTimer(mpPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_TEST_TIME);
    }

    bool CCJGW_TestView::InitTestView(CPaintManagerUI* pPaintManagerUI,const wchar_t* strFolderName,const wchar_t* strRecordConfigName)
    {
        Log4WD_F(L"CCJGW_TestView::InitTestView pPaintManagerUI: 0x%08X,FolderName: %s",pPaintManagerUI,strFolderName);
        if (!pPaintManagerUI) return false;
        mpPaintManagerUI = pPaintManagerUI;
        //mstrFolderName = strFolderName;
        //! Init Test view
        {
            CVerticalLayoutUI* pTestInfoVerticalLayout = static_cast<CVerticalLayoutUI*>(mpPaintManagerUI->FindControl(_T("testInfoVerticalLayout")));
            if (pTestInfoVerticalLayout) pTestInfoVerticalLayout->SetVisible(true);

            bool bSuccse = true;
            mpTestInfoVerticalLayout = static_cast<CHorizontalLayoutUI*>(mpPaintManagerUI->FindControl(L"testInfoVerticalLayout"));   
            mpStartBtn = static_cast<CButtonUI*>(mpPaintManagerUI->FindControl(L"startBtn"));
            mpTestStatusBtn = static_cast<CButtonUI*>(mpPaintManagerUI->FindControl(L"testStatusBtn"));
            mpTestTimeBtn = static_cast<CButtonUI*>(mpPaintManagerUI->FindControl(L"testTimeBtn"));
            mpSaveTestLog = static_cast<CCheckBoxUI*>(mpPaintManagerUI->FindControl(L"SaveTestLog"));

            mpPassTextUi = static_cast<CTextUI*>(mpPaintManagerUI->FindControl(L"passText"));
            mpFailTextUi = static_cast<CTextUI*>(mpPaintManagerUI->FindControl(L"failText"));
            mpCountTextUi = static_cast<CTextUI*>(mpPaintManagerUI->FindControl(L"countText"));
            mpPassRateTextUi = static_cast<CTextUI*>(mpPaintManagerUI->FindControl(L"passRateText"));

            bSuccse &= (NULL != mpStartBtn);
            bSuccse &= (NULL != mpTestInfoVerticalLayout);
            bSuccse &= (NULL != mpTestStatusBtn);
            bSuccse &= (NULL != mpTestTimeBtn);
            bSuccse &= (NULL != mpPassTextUi);
            bSuccse &= (NULL != mpFailTextUi);
            bSuccse &= (NULL != mpCountTextUi);
            bSuccse &= (NULL != mpPassRateTextUi);
            bSuccse &= (NULL != mpSaveTestLog);

            if (!bSuccse) return false;
        }

        mpTestRecordImpl = GetTestRecordInstance();
        if (!mpTestRecordImpl) return false;

        if (!mpTestRecordImpl->LoadTestRecordConfig(strFolderName,strRecordConfigName)) return false;
        mpSaveTestLog->SetCheck(!mpTestRecordImpl->GetIsSaveTestLog());
        mpSaveTestLog->SetCheck(mpTestRecordImpl->GetIsSaveTestLog());
        mpSaveTestLog->SetEnabled(false);

        return true;
    }

    bool CCJGW_TestView::OnSaveTestLogSelectChangedMsg()
    {
        mpTestRecordImpl->SetIsSaveTestLog(mpSaveTestLog->GetCheck());
        return mpSaveTestLog->GetCheck();
    }

    bool CCJGW_TestView::OnChangeTestStatus(E_TEST_STATUS_TYPE eTestStatusType)
    {
        switch (eTestStatusType)
        {
        case E_TEST_IDEL:
            OnTestIDELStatus();
            break;
        case E_TEST_BUSY:
            OnTestBusyStatus();
            break;
        case E_TEST_FAIL:
            OnTestFailStatus();
            break;
        case E_TEST_PASS:
            OnTestPassStatus();
            break;
        }

        return true;
    }

    void CCJGW_TestView::OnTestIDELStatus()
    {
        mpTestInfoVerticalLayout->SetBkColor(IDLE_BK_COLOR);
        mpStartBtn->SetEnabled(true);
        mpStartBtn->SetText(L"Start");
        mdwTestTime = 0;
        mpTestTimeBtn->SetText(L"00:00");
        //(static_cast<CHorizontalLayoutUI*>(mpPaintManagerUI->FindControl(L"MainTestHorizontalLayout")))->SetBkColor(IDLE_BK_COLOR);  
        mpTestStatusBtn->SetTextColor(IDLE_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(IDLE_BK_COLOR);
        mpTestStatusBtn->SetText(L"IDLE");    
       
        //! update test record
        mnTestPassCount = mpTestRecordImpl->GetTestRecordPassCount();
        mnTestCounts = mpTestRecordImpl->GetTestRecordCount();

        UpdateTestRecordView();
    }

    void CCJGW_TestView::OnTestBusyStatus()
    {
        CLEARPLOGW();
        mdwTestTime = 0;
        mpTestTimeBtn->SetText(L"00:00");
        mpStartBtn->SetEnabled(false);
        mpTestStatusBtn->SetTextColor(BUSY_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(BUSY_BK_COLOR);
        //(static_cast<CHorizontalLayoutUI*>(mpPaintManagerUI->FindControl(L"MainTestHorizontalLayout")))->SetBkColor(BUSY_BK_COLOR);  
        mpTestInfoVerticalLayout->SetBkColor(BUSY_BK_COLOR);
        mpTestStatusBtn->SetText(L"BUSY");
        SetTimer(mpPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_TEST_TIME,1000,NULL);
    }

    void CCJGW_TestView::OnTestFailStatus()
    {
        mpStartBtn->SetEnabled(true);
        mpStartBtn->SetFocus();
        mpTestStatusBtn->SetTextColor(FAIL_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(FAIL_BK_COLOR);
        mpTestInfoVerticalLayout->SetBkColor(FAIL_BK_COLOR);
        //(static_cast<CHorizontalLayoutUI*>(mpPaintManagerUI->FindControl(L"MainTestHorizontalLayout")))->SetBkColor(FAIL_BK_COLOR);  
        mpTestStatusBtn->SetText(L"FAIL");
        KillTimer(mpPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_TEST_TIME);
        mpTestRecordImpl->UpdateTestRecordToTestFail();
        mnTestCounts ++;
        UpdateTestRecordView();
    }

    void CCJGW_TestView::OnTestPassStatus()
    {
        mpStartBtn->SetEnabled(true);
        mpStartBtn->SetFocus();
        mpTestStatusBtn->SetTextColor(PASS_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(PASS_BK_COLOR);
        mpTestInfoVerticalLayout->SetBkColor(PASS_BK_COLOR);
        //(static_cast<CHorizontalLayoutUI*>(mpPaintManagerUI->FindControl(L"MainTestHorizontalLayout")))->SetBkColor(PASS_BK_COLOR);  
        mpTestStatusBtn->SetText(L"PASS");
        KillTimer(mpPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_TEST_TIME);
        mpTestRecordImpl->UpdateTestRecordToTestPass();
        mnTestCounts ++;
        mnTestPassCount ++;
        UpdateTestRecordView();
    }

    void CCJGW_TestView::UpdateTestRecordView()
    {
        std::wstring strTemp;
        float fTemp;
        JGW_FormatWString(strTemp,L"%d",mnTestCounts);
        mpCountTextUi->SetText(strTemp.c_str());

        JGW_FormatWString(strTemp,L"%d",mnTestPassCount);
        mpPassTextUi->SetText(strTemp.c_str());

        JGW_FormatWString(strTemp,L"%d",mnTestCounts - mnTestPassCount);
        mpFailTextUi->SetText(strTemp.c_str());

        if (0 == mnTestCounts) fTemp = 0;
        else fTemp = (mnTestPassCount * 100.00f / mnTestCounts);
        JGW_FormatWString(strTemp,L"%.2f%%",fTemp);
        mpPassRateTextUi->SetText(strTemp.c_str());
    }

    void CCJGW_TestView::OnUpdateTestTime()
    {
        std::wstring sztime(_T(""));
        mdwTestTime++;
        if (mdwTestTime > 60 * 60) mdwTestTime = 0;
        JGW::FormatWString(sztime,L"%02d:%02d",mdwTestTime / 60,mdwTestTime % 60);
        mpTestTimeBtn->SetText(sztime.c_str());
    }

    void CCJGW_TestView::OnClearRecordData()
    {
        mnTestCounts = 0;
        mnTestPassCount = 0;
        UpdateTestRecordView();
        mpTestRecordImpl->ClearTestRecord();
    }
}
