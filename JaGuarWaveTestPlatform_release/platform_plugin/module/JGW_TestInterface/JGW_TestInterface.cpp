// JGW_TestInterface.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <JGW_TestInterface/JGW_TestInterface.h>
#include "CJGW_TestView.h"

bool InitTestInterface(CPaintManagerUI* pPaintManagerUI,const wchar_t* strModuleFolder,const wchar_t* strRecordConfigName)
{
    JGW::CCJGW_TestView* pJGWTestView = &JGW::CCJGW_TestView::GetInstanceTestView();
    return pJGWTestView->InitTestView(pPaintManagerUI,strModuleFolder,strRecordConfigName);
}

void ChangeTestStatus(JGW::E_TEST_STATUS_TYPE eTestStatusType)
{
    JGW::CCJGW_TestView* pJGWTestView = &JGW::CCJGW_TestView::GetInstanceTestView();
    pJGWTestView->OnChangeTestStatus(eTestStatusType);
}

void UpdateTestTime()
{
    JGW::CCJGW_TestView* pJGWTestView = &JGW::CCJGW_TestView::GetInstanceTestView();
    pJGWTestView->OnUpdateTestTime();
}

void ClearRecordData()
{
    JGW::CCJGW_TestView* pJGWTestView = &JGW::CCJGW_TestView::GetInstanceTestView();
    pJGWTestView->OnClearRecordData();
}

bool SaveTestLogSelectChangedMsg()
{
    JGW::CCJGW_TestView* pJGWTestView = &JGW::CCJGW_TestView::GetInstanceTestView();
    return pJGWTestView->OnSaveTestLogSelectChangedMsg();
}