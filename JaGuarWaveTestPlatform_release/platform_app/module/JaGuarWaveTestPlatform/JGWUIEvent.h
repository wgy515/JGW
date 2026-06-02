#pragma once
#include <UiLib/CDuiDelegateDlg.h>
#include <poco/Notification.h>
#include <JaGuarWaveTestPlatform/JGWLoadSubPlugin.h>

namespace JGW
{
	//! 创建窗口消息通知
	class CSubWndCreateMsgNotification:public Poco::Notification
	{
	public:
		CSubWndCreateMsgNotification( LPS_SUB_WND_IMPL_PARAM pSubWndInfo );

		LPS_SUB_WND_IMPL_PARAM GetSubWndInfo() const;
	private:
		LPS_SUB_WND_IMPL_PARAM m_pSubWndInfo;;
	};

	class CSubWndMsgOnNotifyNotification:public Poco::Notification
	{
	public:
		CSubWndMsgOnNotifyNotification( TNotifyUI& msg );

		TNotifyUI& GetTNotifyUIMsg() const;
	private:
		TNotifyUI& mMsg;
	};

	class CSubWndCustomeMessageNotification:public Poco::Notification
	{
	public:
		CSubWndCustomeMessageNotification( const UINT& uMsg, WPARAM& wParam, LPARAM& lParam );

		const UINT& GetMsgID() const;
		const WPARAM& GetWParam() const;
		const LPARAM& GetLParam() const;
	private:
		const UINT& muMsg;
		const WPARAM& mwParam;
		const LPARAM& mlParam;
	};


	/*
	相应UI点击事件消息，界面和逻辑分离
	*/
	class CCJGWUIEvent
	{
	public:
		CCJGWUIEvent(void);
		~CCJGWUIEvent(void);
		//! 响应创建子窗体事件
		void HandleSubCreateWndEvent( const Poco::AutoPtr<CSubWndCreateMsgNotification>& subWndMsgPtr );
		//! 分发Notify消息
		void HandleOnTNotifyEvent( const Poco::AutoPtr<CSubWndMsgOnNotifyNotification>& subWndMsgPtr );
		//! 分发main custome msg
		void HandleOnMainCustomeMessage( const Poco::AutoPtr<CSubWndCustomeMessageNotification>& subWndMsgPtr );
		//! 获取响应函数调用返回值
		bool GetHandleResult(){ return mbResult;}
		//! 关闭子窗体
		void CloseSubPluginView();
        //! 卸载子插件
        void DestroySubPlugin();
	private:
		CJGWDuiiLlibSubWndImpl*						mpSubWndimpl;
		CJGWLoadSubPlugin<CJGWDuiiLlibSubWndImpl>	mLoadSubPlugin;
		bool										mbResult;
	};
}
