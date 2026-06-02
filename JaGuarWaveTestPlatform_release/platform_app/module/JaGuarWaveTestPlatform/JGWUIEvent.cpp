#include "stdafx.h"
#include "JGWUIEvent.h"
#include <JGW_FoundationFunc\JGW_FilePath.h>
#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
	CSubWndCreateMsgNotification::CSubWndCreateMsgNotification( LPS_SUB_WND_IMPL_PARAM pSubWndInfo ):m_pSubWndInfo(pSubWndInfo)
	{

	}

	LPS_SUB_WND_IMPL_PARAM CSubWndCreateMsgNotification::GetSubWndInfo() const
	{
		return m_pSubWndInfo;
	}


	CSubWndMsgOnNotifyNotification::CSubWndMsgOnNotifyNotification( TNotifyUI& msg ):mMsg(msg)
	{

	}

	TNotifyUI& CSubWndMsgOnNotifyNotification::GetTNotifyUIMsg() const
	{
		return mMsg;
	}

	CSubWndCustomeMessageNotification::CSubWndCustomeMessageNotification( const UINT& uMsg, WPARAM& wParam, LPARAM& lParam ):muMsg(uMsg),mwParam(wParam),mlParam(lParam)
	{

	}

	const UINT& CSubWndCustomeMessageNotification::GetMsgID() const
	{
		return muMsg;
	}

	const WPARAM& CSubWndCustomeMessageNotification::GetWParam() const
	{
		return mwParam;
	}

	const LPARAM& CSubWndCustomeMessageNotification::GetLParam() const
	{
		return mlParam;
	}

	CCJGWUIEvent::CCJGWUIEvent(void):mbResult(false),mpSubWndimpl(NULL)
	{
	}


	CCJGWUIEvent::~CCJGWUIEvent(void)
	{
		CloseSubPluginView();
	}

	//! del true
	void CCJGWUIEvent::CloseSubPluginView()
	{
		mbResult = false;
        mpSubWndimpl = NULL;
        Log4WI(L"CCJGWUIEvent::CloseSubPluginView");
		mLoadSubPlugin.CloseSubPluginView();   
	}

    void CCJGWUIEvent::DestroySubPlugin()
    {
        Log4WI(L"CCJGWUIEvent::DestroySubPlugin");
        mLoadSubPlugin.DestroySubPlugin();
    }

	void CCJGWUIEvent::HandleOnTNotifyEvent( const Poco::AutoPtr<CSubWndMsgOnNotifyNotification>& subWndMsgPtr )
	{
		if ( mpSubWndimpl ) mpSubWndimpl->OnMsgNotify( LPVOID(&subWndMsgPtr->GetTNotifyUIMsg()) );
	}

	void CCJGWUIEvent::HandleOnMainCustomeMessage( const Poco::AutoPtr<CSubWndCustomeMessageNotification>& subWndMsgPtr )
	{
		if ( mpSubWndimpl ) mpSubWndimpl->OnParentCustomMessage( subWndMsgPtr->GetMsgID(),subWndMsgPtr->GetWParam(),subWndMsgPtr->GetLParam() );
	}

	void CCJGWUIEvent::HandleSubCreateWndEvent( const Poco::AutoPtr<CSubWndCreateMsgNotification>& subWndMsgPtr )
	{
		LPS_SUB_WND_IMPL_PARAM pSubWndInfo = NULL;
		//! ÅÐ¶Ï´«²ÎÊÇ·ñOK
		{
			pSubWndInfo = subWndMsgPtr->GetSubWndInfo();
			Log4WD_F(_T("CCJGWUIEvent::HandleSubCreateWndEvent SubWndInfo:%d"),pSubWndInfo);
			if ( !pSubWndInfo ) return;

			if ( !pSubWndInfo->mpPluginInfo || !pSubWndInfo->mpMainPaintManager || !pSubWndInfo->mpMainParentContainer )
			{
				Log4WE_F(_T("CCJGWUIEvent::HandleSubCreateWndEvent PluginInfo:%d,PaintMangerUI:%d,ParentContainer:%d"),pSubWndInfo->mpPluginInfo,pSubWndInfo->mpMainPaintManager,pSubWndInfo->mpMainParentContainer);
				::MessageBox( pSubWndInfo->mpMainPaintManager?( (CPaintManagerUI*)pSubWndInfo->mpMainPaintManager )->GetPaintWindow():NULL,GetString(E_CHILDFORM_PARAM_IS_NOT_EMPTY),GetString(E_CHILDFORM_INIT_FAIL_TYPE),MB_ICONERROR );
				return ;
			}
		}
		
		//! create sub wnd
		{
			mpSubWndimpl = ( (S_PLUGIN_INFO*)pSubWndInfo->mpPluginInfo )->mnViewLoadID?(CJGWDuiiLlibSubWndImpl *)pSubWndInfo->mpViewResolverClassLoader: mLoadSubPlugin.LoadSubPluginSubWndImpl( (S_PLUGIN_INFO*)pSubWndInfo->mpPluginInfo );
			Log4WD_F(_T("CCJGWUIEvent::HandleSubCreateWndEvent SubWndimpl:%d"),mpSubWndimpl);
			if ( mpSubWndimpl ) mbResult = mpSubWndimpl->CreateSubWnd( pSubWndInfo );
			Log4WD_F(_T("CCJGWUIEvent::HandleSubCreateWndEvent mbResult:%d"),mbResult);
		}
	}

}