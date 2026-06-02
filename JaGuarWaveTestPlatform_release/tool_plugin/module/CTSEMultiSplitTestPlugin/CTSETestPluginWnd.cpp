#include "StdAfx.h"
#include "CTSETestPluginWnd.h"
#include "CTSETestPluginUI.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_TestRecord/JGW_TestRecord_Exports.h>
#include "CTSETestPluginTestThread.h"
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>
#include <fstream>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#include <JaGuarWaveTestPlatform/CJGW_InputViewResolver_Define.h>
//#include <WinBase.h>
#include "CJGW_InputViewResolver.h"

namespace JGW
{
    CCTSETestPluginWnd::CCTSETestPluginWnd(void) : mpTSETestPluginUI(NULL),m_bInit(false),mpTestRecordImpl(NULL),mpTestThread(NULL)/*,mstrSaveOutPutLog(L"c:\\iperf.txt")*/,mbIsSaveTestLog(false)
    {
        /************************************************************************/
        /* 
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_CLICK,L"startBtn",WM_HWND_TEST_SATRT_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_CLICK,L"configBtn",WM_HWND_CONFIG_BUTTON_CLICK_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_SELECTCHANGED,L"clearTestRecOption",WM_HWND_CLEAR_TEST_RECORD_BUTTON_CLICK_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_SELECTCHANGED,L"SaveTestLog",WM_HWND_SAVE_TEST_LOG_SELECTCHANGE_MSG)      
        */
        /************************************************************************/
    }


    CCTSETestPluginWnd::~CCTSETestPluginWnd(void)
    {
        //::OutputDebugString(L"CCTSETestPluginWnd::~CCTSETestPluginWnd\n");
    }

    void CCTSETestPluginWnd::SetModuleFolderSuitName(const std::wstring& strModuleFolder,const std::wstring& strSulteName)
    {
        mstrModuleFolder = strModuleFolder;
        mstrSuiteName = strSulteName;
    }

    void CCTSETestPluginWnd::OnInitWindow()
    {    
        {
			//! testInfoVerticalLayout 测试信息状态栏
            CVerticalLayoutUI* pTestInfoVerticalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("testInfoVerticalLayout")));
            if (pTestInfoVerticalLayout) pTestInfoVerticalLayout->SetVisible(true);

            bool bSuccse = true;
            mpLogHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"logHorizontalLayout"));
            mpTestInfoVerticalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"testInfoVerticalLayout"));   
            mpStartBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"startBtn"));
            mpConfigBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"configBtn"));
            mpTestStatusBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"testStatusBtn"));
            mpTestTimeBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"testTimeBtn"));
            mpSaveTestLog = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"SaveTestLog"));

            mpPassTextUi = static_cast<CTextUI*>(m_PaintManager.FindControl(L"passText"));
            mpFailTextUi = static_cast<CTextUI*>(m_PaintManager.FindControl(L"failText"));
            mpCountTextUi = static_cast<CTextUI*>(m_PaintManager.FindControl(L"countText"));
            mpPassRateTextUi = static_cast<CTextUI*>(m_PaintManager.FindControl(L"passRateText"));
            mpClearTestRecOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"clearTestRecOption"));
            mpLogOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"LogOption"));
            mpDoMainList = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(L"domainlist"));
            mpThroughtputTestEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"throughtputTestEdt"));
			mpLogTabedt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("logTabedt")));

            bSuccse &= (NULL != mpStartBtn);
            bSuccse &= (NULL != mpConfigBtn);
            bSuccse &= (NULL != mpTestInfoVerticalLayout);
            bSuccse &= (NULL != mpTestStatusBtn);
            bSuccse &= (NULL != mpTestTimeBtn);
            bSuccse &= (NULL != mpPassTextUi);
            bSuccse &= (NULL != mpFailTextUi);
            bSuccse &= (NULL != mpCountTextUi);
            bSuccse &= (NULL != mpPassRateTextUi);
            bSuccse &= (NULL != mpSaveTestLog);
            bSuccse &= (NULL != mpLogHorizontalLayout);
            bSuccse &= (NULL != mpClearTestRecOption);
            bSuccse &= (NULL != mpDoMainList);
            bSuccse &= (NULL != mpLogOption);
            bSuccse &= (NULL != mpThroughtputTestEdt);
			bSuccse &= (NULL != mpLogTabedt);

            if (!bSuccse)
            {
                Close(0);
                return;
            }
            mpLogHorizontalLayout->SetVisible(false);
        }
        {
            mstrSaveDownloadLogFolder = JGW_GetFormatWString(L"%sTestLog\\%s\\%s\\",JGW_GetApplicationFolder(),mstrModuleFolder.c_str(),mstrSuiteName.c_str());

            SetTimer(m_PaintManager.GetPaintWindow(),WM_TIME_FIFO_EVENT,FIFO_EVENT_INTERVAL_TIME,NULL);
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCTSETestPluginWnd::OnEventCustomMessage,0);

            ADD_REG_MSG((&mMapRegMsg),WM_TIMER,&CCTSETestPluginWnd::OnRspTimeMsg);
            //mMapRegMsg.insert(std::map<UINT,LPOnResponceMsgFun>::value_type(WM_TIMER,(LPOnResponceMsgFun)&CCTSETestPluginWnd::OnRspTimeMsg));
			//! 测试结果消息
            ADD_REG_MSG((&mMapRegMsg),WM_TEST_PASS_MSG,&CCTSETestPluginWnd::OnTestPassMsg);
            ADD_REG_MSG((&mMapRegMsg),WM_TEST_FAIL_MSG,&CCTSETestPluginWnd::OnTestFailMsg);
            //ADD_REG_MSG((&mMapRegMsg),WM_HWND_SHOW_LOG_LAYOUT_CLICK_MSG,&CCTSETestPluginWnd::OnShowLogLayoutClickMsg);
			//! 按键点击消息
            ADD_REG_MSG((&mMapRegMsg),WM_HWND_CONFIG_BUTTON_CLICK_MSG,&CCTSETestPluginWnd::OnConfigClickMsg);
        }
		{
			//! 加载测试记录实例索引
			mpTestRecordImpl = GetTestRecordInstanceToIndex();
			if (!mpTestRecordImpl)
			{
				Close(0);
				return ;
			}
			//! 加载测试记录配置
			if (!mpTestRecordImpl->LoadTestRecordConfig(mstrModuleFolder.c_str(),mstrSuiteName.c_str()))
			{
				Close(0);
				return;
			}
			//! 是否保存日志
			mpSaveTestLog->SetCheck(!mpTestRecordImpl->GetIsSaveTestLog());
			mpSaveTestLog->SetCheck(mpTestRecordImpl->GetIsSaveTestLog());
			mpSaveTestLog->SetEnabled(false);
			mbIsSaveTestLog = mpSaveTestLog->GetCheck();

			if (!InitSubWndPlugin(NULL))
			{
				Close(0);
				return ;
			}
			//! 初始化
			OnRspTestStatusChange(E_TEST_IDEL);
		}  
    }

    bool CCTSETestPluginWnd::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM ,WPARAM )
    {
        UINT uMsg = pTEventUI->Type;
        WPARAM wParam = pTEventUI->wParam;
        LPARAM lParam = pTEventUI->lParam;
        if (WM_TIMER == uMsg && WM_TIME_FIFO_EVENT == (int)wParam) ExecuteFIFOEventMessage();
        //! mMapRegMsg
        if ( mMapRegMsg.end() != mMapRegMsg.find(uMsg))
        {
            LPOnResponceMsgFun OnResponceMsg = mMapRegMsg[uMsg];
            (this->*OnResponceMsg)( wParam,lParam ) ;
        }

        return true;
    }

	LRESULT CCTSETestPluginWnd::HandleInterceptCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		switch (uMsg)
		{
		case WM_HWND_INPUT_COMMON_WINDOW_MSG: //! 响应通用输入框消息
			bHandled = TRUE;
			return OnResponceInputCommonWindowMsg(wParam,lParam);
		}

		return 0;
	}

	LRESULT CCTSETestPluginWnd::OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam )
	{
#if 1
		LPS_ALL_INPUT_UI_ATTR_SETTING psAllControlAttrSetting = (LPS_ALL_INPUT_UI_ATTR_SETTING)wParam;
		if (NULL == psAllControlAttrSetting)
		{
			return IDCANCEL;
		}

		CCJGWInputViewResolver jgw(psAllControlAttrSetting);
		if ( IDOK == jgw.DoModal() )
		{
			unUserData nRetData = {{0,0,0,0}};
#if 0
			for ( std::vector<LPS_CONTROL_ATTR_SETTING>::iterator it = vGetControlAttrSetting->begin();
				it != vGetControlAttrSetting->end();
				++ it )
			{
				for ( std::vector<S_ATTR_NAME_VALUE>::iterator itAttr = it[0]->mvsAttrNameValue.begin();
					itAttr != it[0]->mvsAttrNameValue.end();
					++ itAttr )
				{
					nRetData = jgw.GetControlAttr( it[0]->name.c_str(),itAttr->pstrName.c_str() );
					if ( NULL != nRetData.pcwchar ) itAttr->pstrValue = nRetData.pcwchar;
				}
			}
#else
			for ( std::vector<S_ATTR_NAME_VALUE>::iterator itAttr = psAllControlAttrSetting->m_sGetControlAttrSetting.mvsAttrNameValue.begin();
				itAttr != psAllControlAttrSetting->m_sGetControlAttrSetting.mvsAttrNameValue.end();
				++ itAttr )
			{
				nRetData = jgw.GetControlAttr( psAllControlAttrSetting->m_sGetControlAttrSetting.name.c_str(),itAttr->pstrName.c_str() );
				if ( NULL != nRetData.pcwchar ) itAttr->pstrValue = nRetData.pcwchar;
			}
#endif
			return IDOK;
		}
		return IDCANCEL;
#else
		return IDOK;
#endif
	}

    void CCTSETestPluginWnd::OnRspTimeMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_TEST_TIME == (int)wParam)
		{
			OnUpdateTestTime();
		}     
    }


    void CCTSETestPluginWnd::ExecuteFIFOEventMessage()
    {
		//! 执行同步消息
        mAsyncWndMessage.ExecuteAsyncMessage([&](std::queue<S_ASYNC_MESSAGE>* p_execute_msg_queue){
            while (!p_execute_msg_queue->empty())
            {
                S_ASYNC_MESSAGE async_message = p_execute_msg_queue->front();
                p_execute_msg_queue->pop();
                if ( mMapRegMsg.end() != mMapRegMsg.find(async_message.mMsgID) )
                {
                    LPOnResponceMsgFun OnResponceMsg = mMapRegMsg[async_message.mMsgID];
                    (this->*OnResponceMsg)( async_message.wParam,async_message.lParam ) ;
                }
            }
        });
    }

    void CCTSETestPluginWnd::OnNotify(TNotifyUI& msg)
    {
        if( msg.sType == DUI_MSGTYPE_CLICK )
        {
            if (mpStartBtn == msg.pSender)
            {
                OnTestStartClickMsg(NULL,NULL);
            }
            else if (mpConfigBtn == msg.pSender)
            {
				if (NULL != mpTestThread) mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
            }
        }
        else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            if (mpClearTestRecOption == msg.pSender)
            {
                OnClearRecordData();
                mpClearTestRecOption->Selected(false);
            }
            else if (mpLogOption ==  msg.pSender)
            {
                OnShowLogLayoutClickMsg(mpLogHorizontalLayout->IsVisible() ? 0 : 1,NULL);
                mpClearTestRecOption->Selected(false);
            }
			else
			{
				if (msg.pSender->GetName() == L"debugTab")
				{
					if (mpTestThread)
					{
						mpLogTabedt->SetText(((CCTSETestPluginTestThread*)mpTestThread)->GetDebufMessage().c_str());
						mpLogTabedt->EndDown();
					}

				}
				else if (msg.pSender->GetName() == L"errorTab")
				{
					if (mpTestThread)
					{
						mpLogTabedt->SetText(((CCTSETestPluginTestThread*)mpTestThread)->GetErrorMessage().c_str());
						mpLogTabedt->EndDown();  
					}

				}
				else if (msg.pSender->GetName() == L"htmlTab")
				{
					if (mpTestThread)
					{
						mpLogTabedt->SetText(GETPHLOGW().c_str());
						mpLogTabedt->EndDown();
					}
				}
			}
        }
    }

    void CCTSETestPluginWnd::OnCloseWindow()
    {
		//! 取消事件注册
		m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCTSETestPluginWnd::OnEventCustomMessage,0);
		KillTimer(m_PaintManager.GetPaintWindow(),WM_TIME_FIFO_EVENT);
		//! 终止测试线程
		if (mpTestThread)
		{
			mpTestThread->TerminateMessageThread();
			delete mpTestThread;
			mpTestThread = NULL;
		}
    }

    void CCTSETestPluginWnd::OnFinalMessage(HWND /*hWnd*/)
    {
        OnCloseWindow();
        delete this;
    }

	LONG gnTSETestPluginWndIndex = 0;
    void CCTSETestPluginWnd::Init(CCTSETestPluginUI* pTSETestPluginUI)
    {
        mpTSETestPluginUI = pTSETestPluginUI;
        m_bInit = true;

        if (NULL != mpTSETestPluginUI)
        {
			std::wstring strClassName;
			JGW_FormatWString(strClassName,L"%s_%d",_T("JaGuarWave Multiple Test Config"),(int)::InterlockedIncrement(&gnTSETestPluginWndIndex));

            //! RECT rcPos = mpTSETestPluginUI->GetPos();
            UINT uStyle = UI_WNDSTYLE_DIALOG;

#if defined USE_ZIP_RESOURCE_ENABLE
            SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
            CreateNoBorderStyleWindow(_T(""),_T("tse_multi_split_test.xml"),strClassName.c_str(),mpTSETestPluginUI->GetManager()->GetPaintWindow(),uStyle,WS_EX_STATICEDGE);
#elif defined USE_ZIP_FILE_ENABLE
            SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
            CreateNoBorderStyleWindow(_T(""),_T("tse_multi_split_test.xml"),strClassName.c_str(),mpTSETestPluginUI->GetManager()->GetPaintWindow(),uStyle,WS_EX_STATICEDGE);
#else
			//! WS_EX_APPWINDOW 任务栏中有图标显示
            CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("tse_multi_split_test.xml"),strClassName.c_str(),mpTSETestPluginUI->GetManager()->GetPaintWindow(),uStyle,WS_EX_STATICEDGE);
#endif
            //	HWND   hWnd = CreateWindow(_T("#32770"), _T("WndMediaDisplay"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), (HMENU)0, NULL, NULL);
            //Create(mpTSETestPluginUI->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
        }
    }

    void CCTSETestPluginWnd::OnTestStartClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
		//! 取消log布局显示
        OnShowLogLayoutClickMsg(0,NULL);
		//! 将当前测试状态更改为BUSY状态
        OnRspTestStatusChange(E_TEST_BUSY);
    }

    void CCTSETestPluginWnd::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    void CCTSETestPluginWnd::OnTestPassMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        OnRspTestStatusChange(E_TEST_PASS);
        PostMessage(WM_HWND_TEST_PASS_MSG,WPARAM(0),NULL);
    }

    void CCTSETestPluginWnd::OnTestFailMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
		//! 测试失败状态
        OnRspTestStatusChange(E_TEST_FAIL);
        PostMessage(WM_HWND_TEST_FAIL_MSG,WPARAM(0),NULL);
		//! 显示LOG布局
        OnShowLogLayoutClickMsg(1,NULL);
    }

    void CCTSETestPluginWnd::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        //! 隐藏log显示信息
        if (0 == (int)wParam)
        {
            //! 已经隐藏
            if (!mpLogHorizontalLayout->IsVisible()) return;
            mpLogHorizontalLayout->SetVisible(false);
        }
        else
        {
            COptionUI* pError =  (COptionUI*)m_PaintManager.FindControl(L"errorTab");
            COptionUI* debugTab =  (COptionUI*)m_PaintManager.FindControl(L"debugTab");
            debugTab->Selected(true);
            pError->Selected(true);
            mpLogHorizontalLayout->SetVisible(true);
        }
        if (mpDoMainList)
        {
            mpDoMainList->SetNeedEndDown(true);
        }
    }


    bool CCTSETestPluginWnd::OnRspTestStatusChange(E_TEST_STATUS_TYPE eTestStatusType)
    {
        switch (eTestStatusType)
        {
        case E_TEST_IDEL:
            OnTestIDELStatus();
            break;
        case E_TEST_BUSY:
            OnTestBusyStatus();
            OnBusyTestStatus();
            break;
        case E_TEST_FAIL:
            OnTestFailStatus();
            OnFailTestStatus();    
            break;
        case E_TEST_PASS:
            OnTestPassStatus();
            OnPassTestStatus();
            break;
        }
        return true;
    }

    void CCTSETestPluginWnd::OnTestIDELStatus()
    {
		//! 测试状态
        mpTestInfoVerticalLayout->SetBkColor(IDLE_BK_COLOR);
        mpStartBtn->SetEnabled(true);
        mpStartBtn->SetText(L"Start");

        mdwTestTime = 0;
        mpTestTimeBtn->SetText(L"00:00");
        //(static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"MainTestHorizontalLayout")))->SetBkColor(IDLE_BK_COLOR);  
        mpTestStatusBtn->SetTextColor(IDLE_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(IDLE_BK_COLOR);
        mpTestStatusBtn->SetText(L"IDLE");    

        //! update test record
        mnTestPassCount = mpTestRecordImpl->GetTestRecordPassCount();
        mnTestCounts = mpTestRecordImpl->GetTestRecordCount();

        UpdateTestRecordView();
    }

    void CCTSETestPluginWnd::OnTestBusyStatus()
    {
        CLEARPLOGW();
        mdwTestTime = 0;
        mpTestTimeBtn->SetText(L"00:00");
        mpStartBtn->SetEnabled(false);
        mpTestStatusBtn->SetTextColor(BUSY_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(BUSY_BK_COLOR);
        //(static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"MainTestHorizontalLayout")))->SetBkColor(BUSY_BK_COLOR);  
        mpTestInfoVerticalLayout->SetBkColor(BUSY_BK_COLOR);
        mpTestStatusBtn->SetText(L"BUSY");
        SetTimer(m_PaintManager.GetPaintWindow(),WM_TIMER_UPDATE_TEST_TIME,1000,NULL);
    }

    void CCTSETestPluginWnd::OnTestFailStatus()
    {
        mpStartBtn->SetEnabled(true);
        mpStartBtn->SetFocus();
        mpTestStatusBtn->SetTextColor(FAIL_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(FAIL_BK_COLOR);
        mpTestInfoVerticalLayout->SetBkColor(FAIL_BK_COLOR);
        //(static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"MainTestHorizontalLayout")))->SetBkColor(FAIL_BK_COLOR);  
        mpTestStatusBtn->SetText(L"FAIL");
        KillTimer(m_PaintManager.GetPaintWindow(),WM_TIMER_UPDATE_TEST_TIME);
        mpTestRecordImpl->UpdateTestRecordToTestFail();
        mnTestCounts ++;
        UpdateTestRecordView();
    }

    void CCTSETestPluginWnd::OnTestPassStatus()
    {
        mpStartBtn->SetEnabled(true);
        mpStartBtn->SetFocus();
        mpTestStatusBtn->SetTextColor(PASS_TEXT_COLOR);
        mpTestStatusBtn->SetBkColor(PASS_BK_COLOR);
        mpTestInfoVerticalLayout->SetBkColor(PASS_BK_COLOR);
        //(static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"MainTestHorizontalLayout")))->SetBkColor(PASS_BK_COLOR);  
        mpTestStatusBtn->SetText(L"PASS");
        KillTimer(m_PaintManager.GetPaintWindow(),WM_TIMER_UPDATE_TEST_TIME);
        mpTestRecordImpl->UpdateTestRecordToTestPass();
        mnTestCounts ++;
        mnTestPassCount ++;
        UpdateTestRecordView();
    }

    void CCTSETestPluginWnd::UpdateTestRecordView()
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

    void CCTSETestPluginWnd::OnUpdateTestTime()
    {
        std::wstring sztime(_T(""));
        mdwTestTime++;
        if (mdwTestTime > 60 * 60) mdwTestTime = 0;
        JGW::FormatWString(sztime,L"%02d:%02d",mdwTestTime / 60,mdwTestTime % 60);
        mpTestTimeBtn->SetText(sztime.c_str());
    }

    void CCTSETestPluginWnd::OnClearRecordData()
    {
        mnTestCounts = 0;
        mnTestPassCount = 0;
        UpdateTestRecordView();
        mpTestRecordImpl->ClearTestRecord();
    }

    /**************************************************************************************/

    bool CCTSETestPluginWnd::InitSubWndPlugin( LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam )
    {
        {
            mpDoMainList->SetAttribute(L"itemalign",L"left");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);
            mpDoMainList->GetHeader()->RemoveAll();

            mpDoMainList->InsertColumn(0,_T("Index"),60);
            mpDoMainList->InsertColumn(1,_T("Test Project"),0);
            mpDoMainList->InsertColumn(2,_T("Content"),200);
            mpDoMainList->InsertColumn(3,_T("Time"),80);
            mpDoMainList->InsertColumn(4,_T("P/F"),40);
        }
        //! Init Param
        {
            msMultiSplitTestConfig.mstrSuiteXmlConfigPath = JGW_GetTSEConfigFilePath(mstrModuleFolder,mstrSuiteName);
        }

        {
            ADD_REG_MSG((&mMapRegMsg),WM_HWND_TSE_SHOW_CONTENT,&CCTSETestPluginWnd::OnTSEShowTestContent);
            ADD_REG_MSG((&mMapRegMsg),WM_HWND_TEST_LIST_MSG,&CCTSETestPluginWnd::OnTestListMsg);
            ADD_REG_MSG((&mMapRegMsg),WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCTSETestPluginWnd::OnTSEInitFail);
            ADD_REG_MSG((&mMapRegMsg),WM_HWND_RICHEDIT_APPEND_TEST_MSG,&CCTSETestPluginWnd::OnAppendRichEditTest);
            ADD_REG_MSG((&mMapRegMsg),WM_COPYDATA,&CCTSETestPluginWnd::OnTSEAdapterLog);

        }
        //! Init Test Thread
        {
            if (!mpTestThread) mpTestThread = new CCTSETestPluginTestThread(m_PaintManager.GetPaintWindow());
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(&mAsyncWndMessage),LPARAM(&msMultiSplitTestConfig));
        }   

        return true;
    }

    void CCTSETestPluginWnd::OnNotifySubWndPlugin( LPVOID pMsg )
    {

    }

    bool CCTSETestPluginWnd::OnBusyTestStatus()
    {
        //! if (bUpdateLogTime) KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"Is Save Test Log : %s LF\r...",mbIsSaveTestLog ? L"true" : L"false");

        mstrTestLogStream.str(L"");
        mstrTestLogStream.clear();
        mstrPrevTestProjectName = L"";
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        mpThroughtputTestEdt->SetText(L"");
        OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        //JGW_DeleteFile(mstrSaveOutPutLog.c_str());
        //! SetTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO,1000,NULL);
        return true;
    }

    bool CCTSETestPluginWnd::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        /*
        if (!JGW_DeleteFile(mstrSaveOutPutLog.c_str())) 
        {
        JGW_KillProcessToName(L"adb.exe");
        Sleep(1000);
        JGW_DeleteFile(mstrSaveOutPutLog.c_str());
        }
        */
#ifdef SHOW_TESTPROJECT_INFO
        if (!mstrPrevTestProjectName.empty())
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"%s => FAIL",mstrPrevTestProjectName.c_str());
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
            mstrPrevTestProjectName = L"";
        }
#endif
        if (mbIsSaveTestLog) 
        {
            //bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",false,mpThroughtputTestEdt->GetText().GetData());
            bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",false,mstrTestLogStream.str());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        //! KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
        return true;
    }

    bool CCTSETestPluginWnd::OnPassTestStatus()
    {
        //! KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
#ifdef SHOW_TESTPROJECT_INFO
        if (!mstrPrevTestProjectName.empty())
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"%s => PASS",mstrPrevTestProjectName.c_str());
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
            mstrPrevTestProjectName = L"";
        }
#endif
        if (mbIsSaveTestLog)
        {
            bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",true,mpThroughtputTestEdt->GetText().GetData());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);

        return true;
    }

    std::wstring CCTSETestPluginWnd::GetSingleTestTime(double dTimer)
    {
        std::wstring strTimer;
        if (dTimer < 100)
        {
            FormatWString(strTimer,L"%.2lf ms",dTimer);
        }
        else if (dTimer >= 100 && dTimer < 1000 * 1000)
        {
            FormatWString(strTimer,L"%.2lf s",dTimer/1000.00);
        }
        else
        {
            FormatWString(strTimer,L"%.1lf min",dTimer/(1000.00 * 60.00));
        }
        return strTimer;
    }

    void CCTSETestPluginWnd::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        _ASSERT(wParam);
        _ASSERT(lParam);
        DWORD nRgn = (WPARAM)lParam;
        LPCTSTR strTestProject = (LPCTSTR)wParam;
        CListTextElementUI* pListTextElementUi = NULL;
        int  nIndex = mpDoMainList->GetCount();
        std::wstring strTemp;

        switch ( nRgn )
        {
        case BALCK:
            FormatWString(strTemp,L"%d",nIndex+1);
            pListTextElementUi = mpDoMainList->InsertItem( mpDoMainList->GetList()->GetCount(),strTemp.c_str(),30);
            mpDoMainList->SetNeedEndDown(true);
            pListTextElementUi->SetText(0,strTemp.c_str());
            pListTextElementUi->SetText(1,strTestProject);
            pListTextElementUi->SetText(2,L"");
            pListTextElementUi->SetText(3,L"---");
            pListTextElementUi->SetText(4,L"---");
            pListTextElementUi->Select(false);
#ifdef SHOW_TESTPROJECT_INFO
            if (nIndex >= 1 && !mstrPrevTestProjectName.empty() && bUpdateLogTime)
            {
                JGW_FormatWString(strTemp,L"%s => PASS",mstrPrevTestProjectName.c_str());
                OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
                mstrPrevTestProjectName = L"";
            }
            if (bUpdateLogTime)
            {
                JGW_FormatWString(strTemp,L"%s => Runing",strTestProject);
                OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
                mstrPrevTestProjectName = strTestProject;
            }
#endif
            m_cHiperTime.Start();
            break;
        case RED:
            pListTextElementUi = mpDoMainList->GetListTextElementItemClass(nIndex - 1);
            if(!pListTextElementUi) return ;
            m_cHiperTime.Stop();    
            //! pListTextElementUi->SetText(1,strTestProject);
            pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            pListTextElementUi->SetText(4,L"F");
            pListTextElementUi->Select(true);
            break;
        case GREEN:
            pListTextElementUi = mpDoMainList->GetListTextElementItemClass(nIndex-1);
            if(!pListTextElementUi) return ;
            m_cHiperTime.Stop();
            pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            pListTextElementUi->SetText(4,L"P");
            pListTextElementUi->Select(false);
            break;
        }  
    }

    void CCTSETestPluginWnd::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(m_PaintManager.GetPaintWindow(),GetString(E_TSE_INITFAIL_CHECKCONFIG_LOG),L"TSE INTI",MB_ICONERROR);
        //PostMessage(WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
		Close(0);
    }

    void CCTSETestPluginWnd::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListTextElementUI* pListTextElementUi = mpDoMainList->GetListTextElementItemClass(mpDoMainList->GetCount() - 1);
        if(!pListTextElementUi) return ;
        pListTextElementUi->SetText(2,(LPCTSTR)wParam);
    }

    void CCTSETestPluginWnd::OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam)
    {
        //IsBadStringPtr((const wchar_t*)wParam,-1);
        std::wstring strTemp((const wchar_t*)wParam);
        //std::string log = JGW_W2A(strTemp,CP_UTF8);
        //strTemp = JGW_A2W(log,CP_UTF8);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        mpThroughtputTestEdt->AppendText(strTemp.c_str());
        mpThroughtputTestEdt->AppendText(L"\r\n");
        mpThroughtputTestEdt->EndDown();

        mstrTestLogStream << strTemp << std::endl;
    }

    void CCTSETestPluginWnd::OnTimerUpdateLogInfo()
    {
#if 0
        std::wifstream mFileStream;
        std::wstring strTemp;

        if (msTSEThroughputTestConfigParam.mnTestTime > msTSEThroughputTestConfigParam.mnTestTimeIndex)
            JGW_FormatWString(strTemp,L"%d.0-%s%d.0",msTSEThroughputTestConfigParam.mnTestTimeIndex,msTSEThroughputTestConfigParam.mnTestTimeIndex + 1 < 10?L" ":L"",msTSEThroughputTestConfigParam.mnTestTimeIndex + 1);
        else 
        {
            JGW_FormatWString(strTemp,L"0.0-%d.0",msTSEThroughputTestConfigParam.mnTestTimeIndex);
            //KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
            bUpdateLogTime = true;
        }

        mFileStream.open(mstrSaveOutPutLog.c_str());
        if (!mFileStream.is_open()) return ;
        mFileStream.seekg(msTSEThroughputTestConfigParam.mnSeekgLog);

        while (mFileStream.getline(szReadBuf,READ_BUFFER_MAX_LEN))
        {
            if (NULL != wcsstr(szReadBuf,strTemp.c_str()))
            {
                if (msTSEThroughputTestConfigParam.mbMultThread && !wcsstr(szReadBuf,L"[SUM]")) continue;
                msTSEThroughputTestConfigParam.mnSeekgLog = mFileStream.tellg();
                OnAppendRichEditTest(WPARAM(szReadBuf),NULL);

                //! update 
//                 if (msTSEThroughputTestConfigParam.mnTestTime > msTSEThroughputTestConfigParam.mnTestTimeIndex ++)
//                 {
//                     std::vector<std::wstring> vstrTemp;
//                     JGW_ParserStrW(szReadBuf,L" ",vstrTemp);

                    /*if (msTSEThroughputTestConfigParam.mbMultThread)
                        mpView->mpWkeBrowser->UpdateBpsInfo(msTSEThroughputTestConfigParam.mnTestTimeIndex,_ttof(vstrTemp[5].c_str()));
                    else 
                        mpView->mpWkeBrowser->UpdateBpsInfo(msTSEThroughputTestConfigParam.mnTestTimeIndex,_ttof(vstrTemp[6].c_str()));                 
                    break;*/
/*                } */
            } 
            else
            {
                msTSEThroughputTestConfigParam.mnSeekgLog = mFileStream.tellg();
                OnAppendRichEditTest(WPARAM(szReadBuf),NULL);
            }
        }
        mFileStream.close();
#endif
    }
    /// JSON数据
    void CCTSETestPluginWnd::OnTSEAdapterLog(const WPARAM& wParam,const LPARAM& lParam)
    {
        COPYDATASTRUCT* pCopyDateStruct = (COPYDATASTRUCT*)lParam;
        if (0 == pCopyDateStruct->cbData)
        {
            return;
        }
#if 1
        ((CCTSETestPluginTestThread*)mpTestThread)->UpdateCopyDataMessage((char*)pCopyDateStruct->lpData);
#else
        CCJGW_FTM_Json ftmJson;
        if (ftmJson.FromJsonToString((char*)pCopyDateStruct->lpData))
        {
            CCJGW_CopyDataTestLogJson testLog;
            testLog.FromJosn(ftmJson);
            switch ((CopyDataLogTypePluginDomain)testLog.LogType)
            {
            case Debug_COPYDATA:
            case Info_COPYDATA:
                PDLOG4WW(testLog.Message.c_str());
                break;
            case Warn_COPYDATA:
            case Error_COPYDATA:
                OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                PELOG4WW(testLog.Message.c_str());
                break;
            case QIAWindowHanlde_COPYDATA:
                ((CCTSEThroughputTestThread*)mpTestThread)->UpdateQIAWindowHandle(_ttoi(testLog.Message.c_str()));
                break;
            case CheckQIAIsRunning_COPYDATA:
                ((CCTSEThroughputTestThread*)mpTestThread)->UpdateCheckQIAIsRunningFlag(1);
                break;
            case QIAXttTreeTestComplete_COPYDATA:
                ((CCTSEThroughputTestThread*)mpTestThread)->UpdateQiaTestRessult(1 == _ttoi(testLog.Message.c_str()));
                break;
            case UserDefine_COPYDATA:
                switch ((CopyDataUserDefineIDType)testLog.UserDefineID)
                {
                case E_COPYDATA_USER_DEFINE_HTMLFILEPATH:
                    PHLOG4WW(testLog.Message.c_str());
                    OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                    ((CCTSEThroughputTestThread*)mpTestThread)->AddHtmlTestLogging(testLog.Message);
                    break;
                case E_COPYDATA_TSE_SHOW_CONTENT_INFO:
                    OnTSEShowTestContent(WPARAM(testLog.Message.c_str()),NULL);
                    break;
                case E_COPYDATA_TSE_SHOW_TEST_INFO_TO_RICHEDIT:
                    OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                    break;
                case E_COPYDATA_TSE_SHOW_IDEL_BUSY_LIST_TEST_STATUS:
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),BALCK);
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),BULE);
                    break;
                case E_COPYDATA_TSE_SHOW_FAIL_LIST_TEST_STATUS:
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),RED);
                    break;
                case E_COPYDATA_TSE_SHOW_PASS_LIST_TEST_STATUS:
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),GREEN);
                    break;
                }

                break;
            }

        }
#endif
    }





}

