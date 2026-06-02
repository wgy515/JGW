#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>

namespace JGW
{
	class CCJGW_LANConfigDlg : public CCUiLibWnd
	{
	public:
		CCJGW_LANConfigDlg(HWND hParentHwnd,int& operid,std::wstring& strIPAddress,std::wstring& strBroadcaseAddressEdt);
		~CCJGW_LANConfigDlg(void);
		int					DoModal();
	private:
		UILIB_GetWindowClassName(CCJGW_MESConfigDlg);
		void				OnInitWindow();
		virtual UINT		GetClassStyle() const;
		void				OnNotify(TNotifyUI& msg);
		void				OnCloseWindow();
	private:
		HWND				m_hParentHwnd;
		CButtonUI*          mpOkOptionUI;
		CButtonUI*          mpCancelOptionUI;

		CEditUI* mpIPAddressEdt;
		CEditUI* mpBroadcaseAddressEdt;
		CComboBoxUI* mpOperCom;

		int& mnOperID; //! 0 : 自动获取 1: 手动获取
		std::wstring& mstrIPAddress;
		std::wstring& mstrBroadcaseAddressEdt;
	};
}