#include "StdAfx.h"
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCJGW_CommanTestPlnView::CCJGW_CommanTestPlnView(void):mpViewResolverParam(NULL),mpPluginPaintManagerUI(NULL),mbIsShowLogHwnd(true),meTestStatusType(E_TEST_IDEL),mbIsShowTestResultStatus(false),mbIsSaveTestLog(false)
    {
    }


    CCJGW_CommanTestPlnView::~CCJGW_CommanTestPlnView(void)
    {
        //! if (mbIsShowTestResultStatus) mTestStatusView.CloseWindow();
    }

    void CCJGW_CommanTestPlnView::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        //if (mbIsShowTestResultStatus) mTestStatusView.CloseWindow();
        if( mpViewResolverParam->mpMapRegMsg ) mpViewResolverParam->mpMapRegMsg->clear();
        if( mpViewResolverParam->mpMainParentContainer ) mpViewResolverParam->mpMainParentContainer->RemoveAll();
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
        
    }

    bool CCJGW_CommanTestPlnView::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
         mpViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
         if (!mpViewResolverParam) return false;
         mpPluginPaintManagerUI = (CPaintManagerUI*)mpViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
         mstrSaveDownloadLogFolder = JGW_GetFormatWString(L"%sTestLog\\%s\\%s\\",JGW_GetApplicationFolder(),mpViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str(), mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName.c_str());
         //! init reg msg
         {
             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_TIMER,&CCJGW_CommanTestPlnView::OnRspTimeMsg);
             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_SATRT_MSG,&CCJGW_CommanTestPlnView::OnTestStartClickMsg);
             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_SHOW_LOG_LAYOUT_CLICK_MSG,&CCJGW_CommanTestPlnView::OnShowLogLayoutClickMsg);
             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_TEST_PASS_MSG,&CCJGW_CommanTestPlnView::OnTestPassMsg);
             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_TEST_FAIL_MSG,&CCJGW_CommanTestPlnView::OnTestFailMsg);

             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_CLEAR_TEST_RECORD_BUTTON_CLICK_MSG,&CCJGW_CommanTestPlnView::OnClearTestRecordClickMsg);
             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_CONFIG_BUTTON_CLICK_MSG,&CCJGW_CommanTestPlnView::OnConfigClickMsg);
			 ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_IS_SHOW_TEST_STATUS_WINDOW_MSG,&CCJGW_CommanTestPlnView::OnIsShowTestStatusWindowMsg);	
             ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_SAVE_TEST_LOG_SELECTCHANGE_MSG,&CCJGW_CommanTestPlnView::OnSaveTestLogSelectChangedMsg);	
         }
         //! init test info 
         {
             if (!InitTestInterface(mpViewResolverParam->mpMainPaintManager,mpViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str(),mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName.c_str()))
             {
                 Log4WE(L"InitTestInterface Fail");
                 return false;
             }
         }

         return OnRspTestStatusChange(E_TEST_IDEL);
    }


	void CCJGW_CommanTestPlnView::OnIsShowTestStatusWindowMsg(const WPARAM& wParam,const LPARAM& lParam)
	{
		mbIsShowTestResultStatus = (int)wParam == 0?false:true;
        mbIsShowTestResultStatus = false;
	}

    bool CCJGW_CommanTestPlnView::OnRspTestStatusChange(E_TEST_STATUS_TYPE eTestStatusType)
    {
        bool bResult = true;
        meTestStatusType = eTestStatusType;
        switch (eTestStatusType)
        {
        case E_TEST_IDEL:
            bResult = OnIdelTestStatus();
            break;
        case E_TEST_BUSY:         
            bResult = OnBusyTestStatus();
            //if (mbIsShowTestResultStatus) mTestStatusView.CloseWindow();
            break;
        case E_TEST_PASS:
            bResult = OnPassTestStatus();
            //if (mbIsShowTestResultStatus) mTestStatusView.DoModal(true,mpPluginPaintManagerUI->GetPaintWindow());
            break;
        case E_TEST_FAIL:
            if (mbIsShowLogHwnd) PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_HWND_VIEW_LOG_INFO,WPARAM(1),NULL);
            bResult = OnFailTestStatus();    
            //if (mbIsShowTestResultStatus) mTestStatusView.DoModal(false,mpPluginPaintManagerUI->GetPaintWindow());
            break;
        }
        ChangeTestStatus(eTestStatusType);
        return bResult;
    }

    void CCJGW_CommanTestPlnView::OnNotifySubWndPlugin(LPVOID pMsg)
    {

    }

    void CCJGW_CommanTestPlnView::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {

    }

    void CCJGW_CommanTestPlnView::OnSaveTestLogSelectChangedMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mbIsSaveTestLog = SaveTestLogSelectChangedMsg();
        ///mbIsSaveTestLog = (int)wParam == 0?false:true;   
    }

    void CCJGW_CommanTestPlnView::OnClearTestRecordClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        ClearRecordData();   
    }

    void CCJGW_CommanTestPlnView::OnTestStartClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_HWND_VIEW_LOG_INFO,WPARAM(0),NULL);
        CLEARPLOGW();
        OnRspTestStatusChange(E_TEST_BUSY);
    }

    void CCJGW_CommanTestPlnView::OnRspTimeMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_TEST_TIME == (int)wParam)
            UpdateTestTime();
    }

    void CCJGW_CommanTestPlnView::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {

    }

    void CCJGW_CommanTestPlnView::OnTestPassMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        OnRspTestStatusChange(E_TEST_PASS);
        PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_HWND_TEST_PASS_MSG,WPARAM(0),NULL);
    }

    void CCJGW_CommanTestPlnView::OnTestFailMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        OnRspTestStatusChange(E_TEST_FAIL);
        PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_HWND_TEST_FAIL_MSG,WPARAM(0),NULL);
    }
}
