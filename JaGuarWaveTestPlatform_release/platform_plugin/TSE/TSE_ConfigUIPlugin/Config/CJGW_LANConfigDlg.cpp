#include "StdAfx.h"
#include "CJGW_LANConfigDlg.h"

namespace JGW
{
	CCJGW_LANConfigDlg::CCJGW_LANConfigDlg(HWND hParentHwnd,int& operid,std::wstring& strIPAddress,std::wstring& strBroadcaseAddressEdt) : mnOperID(operid)
		,mstrBroadcaseAddressEdt(strBroadcaseAddressEdt)
		,mstrIPAddress(strIPAddress),m_hParentHwnd(hParentHwnd)
	{
	}


	CCJGW_LANConfigDlg::~CCJGW_LANConfigDlg(void)
	{
	}

	void CCJGW_LANConfigDlg::OnCloseWindow()
	{

	}

	UINT CCJGW_LANConfigDlg::GetClassStyle() const
	{
		return UI_CLASSSTYLE_DIALOG;
	}

	int CCJGW_LANConfigDlg::DoModal()
	{
#if defined USE_ZIP_RESOURCE_ENABLE
		SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
		CreateNoBorderStyleWindow(_T(""),_T("lan_ip_config.xml"),_T("JaGuarWave LAN IP Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
		SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
		CreateNoBorderStyleWindow(_T(""),_T("lan_ip_config.xml"),_T("JaGuarWave LAN IP Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
		CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("lan_ip_config.xml"),_T("JaGuarWave LAN IP Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
		CenterWindow();

		return ShowModal();
	}

	void CCJGW_LANConfigDlg::OnInitWindow()
	{
		bool bSucceseful = true;

		mpOkOptionUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"OkBtn"));
		mpCancelOptionUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"CancelBtn"));
		mpIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ipAddressEdt"));
		mpBroadcaseAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"broadcastAddressEdt"));
		mpOperCom = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"operCombo"));

		bSucceseful &= (NULL != mpOkOptionUI);
		bSucceseful &= (NULL != mpCancelOptionUI);
		bSucceseful &= (NULL != mpIPAddressEdt);
		bSucceseful &= (NULL != mpBroadcaseAddressEdt);
		bSucceseful &= (NULL != mpOperCom);

		if (!bSucceseful)
		{
			Close(IDCANCEL);
			return ;
		}
		mpOperCom->SelectItem(mnOperID);
		mpIPAddressEdt->SetText(mstrIPAddress.c_str());
		mpBroadcaseAddressEdt->SetText(mstrBroadcaseAddressEdt.c_str());
		//! ×Ô¶¯
		if (0 == mnOperID)
		{
			mpIPAddressEdt->SetEnabled(false);
			mpBroadcaseAddressEdt->SetEnabled(false);
		}
	}

	void CCJGW_LANConfigDlg::OnNotify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (mpOkOptionUI == msg.pSender)
			{
				mstrBroadcaseAddressEdt = mpBroadcaseAddressEdt->GetText().GetData();
				mstrIPAddress = mpIPAddressEdt->GetText().GetData();
				mnOperID = mpOperCom->GetCurSel();
				Close(IDOK);
			}
			else if (mpCancelOptionUI == msg.pSender)
			{
				Close(IDCANCEL);
			}
		}
		else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
		{
			if (mpOperCom == msg.pSender)
			{
				mpIPAddressEdt->SetEnabled(1 == mpOperCom->GetCurSel()?true:false);
				mpBroadcaseAddressEdt->SetEnabled(1 == mpOperCom->GetCurSel()?true:false);
			}
		}
	}
}