#include "StdAfx.h"
#include "CPEDownloadConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
#include <JGW_DevicePool/DevicePortDefine.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>


//#define UILIB_GETCONTROLPTR(class,name) \
//    {\
//        mp##name = static_cast<class*>(m_PaintManager.FindControl(_T(#name)));\
//        return NULL != mp##name;\
//    }


namespace JGW
{
	CCPEDownloadConfigDlg::CCPEDownloadConfigDlg(HWND hParentHwnd,S_MULTI_DOWNLOAD_CONFIG& sMultiDownloadConfig) : mhParentHwnd(hParentHwnd),msMultiDownloadConfig(sMultiDownloadConfig),mnCurrentSortIndex(-1)
	{
	}


	CCPEDownloadConfigDlg::~CCPEDownloadConfigDlg(void)
	{
	}

	void CCPEDownloadConfigDlg::OnCloseWindow()
	{
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCPEDownloadConfigDlg::OnEventCustomMessage,0);
		REMOVE_DEVICEPOOL_MSG(muiDevicePoolThreadID,m_PaintManager.GetPaintWindow());
		if (NULL != UILIB_GET(DomainList))
		{
			for (int i = 0;i < UILIB_GET(DomainList)->GetCount();i ++)
			{
				CListContainerElementUIEx* pListTextElementUi = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
				((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->OnNotify -= MakeDelegate(this,&CCPEDownloadConfigDlg::OnMsgSortBtnClick,_T("click"));
			}
		}
	}

	int CCPEDownloadConfigDlg::DoModal(int uiDevicePoolThreadID)
	{
		muiDevicePoolThreadID = uiDevicePoolThreadID;
#if defined USE_ZIP_RESOURCE_ENABLE
		SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
		CreateNoBorderStyleWindow(_T(""),_T("cpe_download_config.xml"),_T("JaGuarWave CPE Download Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
		SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
		CreateNoBorderStyleWindow(_T(""),_T("cpe_download_config.xml"),_T("JaGuarWave CPE Download Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
		CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("cpe_download_config.xml"),_T("JaGuarWave CPE Download Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
		CenterWindow();

		return ShowModal();
	}

	UINT CCPEDownloadConfigDlg::GetClassStyle() const
	{
		return UI_CLASSSTYLE_DIALOG;
	}

	void CCPEDownloadConfigDlg::OnInitWindow()
	{
		if (!GetDuiContronlClass())
		{
			Close(IDCANCEL);
			return;
		}
		//! 根据下载配置参数，初始化配置界面
		InitDownloadConfigDlgToConfig();
		//! 接收设备插入消息
		m_PaintManager.GetEventSource() += MakeDelegate(this,&CCPEDownloadConfigDlg::OnEventCustomMessage,0);
		GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
		REG_DEVICEPOOL_MSG(muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,m_PaintManager.GetPaintWindow(),guid,E_DIAG_MODE | E_DOWNLOADER_MODE);
	}

	void DecDeviceMsgCount(SDeviceMsg_Info* lpdevicemsg)
	{
		if (lpdevicemsg)
		{
			lpdevicemsg->Dec();
		}
	}

	bool CCPEDownloadConfigDlg::OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam )
	{
		if (pTEventUI->Type == WM_RESPONCE_DEVICEPOOL_MSG_DEFINE)
		{
			if(!pTEventUI->wParam || mnCurrentSortIndex < 0) return true;

			std::wstring strTemp;
			std::shared_ptr<SDeviceMsg_Info> ptrDeviceMsg((SDeviceMsg_Info*)pTEventUI->wParam,DecDeviceMsgCount);
			//SDeviceMsg_Info* lpdevicemsg = (SDeviceMsg_Info*)pTEventUI->wParam;
			//! 设备插入
			if (DBT_DEVICEARRIVAL == ptrDeviceMsg->m_nDeviceMsgType)
			{
                if (ptrDeviceMsg->m_nDeviceMode == E_DOWNLOADER_MODE || ptrDeviceMsg->m_nDeviceMode == E_DIAG_MODE)
                {
                    for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
                    {
                        if (i == mnCurrentSortIndex) continue;
                        if (ptrDeviceMsg->m_nSortIndex == msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex || ptrDeviceMsg->m_nSortIndex == msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex)
                        {
                            MessageBox(m_PaintManager.GetPaintWindow(),GetString(E_CPE_DOWNLOAD_PORTHASSORTED_RESORT),GetString(E_PROMPT_TYPE),MB_ICONERROR);
                            return true;
                        }
                    }
                }


				CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->GetCListContainerElementUIEx(mnCurrentSortIndex);
				if (ptrDeviceMsg->m_nDeviceMode == E_DOWNLOADER_MODE)
				{
					msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnDonwloadComPort = ptrDeviceMsg->m_nComPort;
					msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnDonwloadSortIndex = ptrDeviceMsg->m_nSortIndex;
					if (msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnDonwloadComPort > 0)
					{
						JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnDonwloadComPort);
						((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(strTemp.c_str());
					}	
				}
				else if (ptrDeviceMsg->m_nDeviceMode == E_DIAG_MODE)
				{
					msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnSerialComPort = ptrDeviceMsg->m_nComPort;
					msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnSerialSortIndex = ptrDeviceMsg->m_nSortIndex;
					if (msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnSerialComPort > 0)
					{
						JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnSerialComPort);
						((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(strTemp.c_str());
					}
				}
				//! 判断当前排序是否已经完成
				if (msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnSerialComPort > 0 
					&& msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnSerialSortIndex > 0
					&& msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnDonwloadComPort > 0
					&& msMultiDownloadConfig.msMultiDownloadSortConfigs[mnCurrentSortIndex].mnDonwloadSortIndex > 0)
				{
					if (IDOK == MessageBox(m_PaintManager.GetPaintWindow(),GetString(E_CPE_DOWNLOAD_PORT_IS_SORT_NEXTINDEX_SORT),GetString(E_PROMPT_TYPE),MB_OKCANCEL))
					{
						CancelSort();
					}
				}
			}
		}
		return true;
	}

	void CCPEDownloadConfigDlg::OnNotify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (UILIB_GET(CancelSortBtn) == msg.pSender)
			{
				CancelSort();
			}
			else if (UILIB_GET(CancelBtn) == msg.pSender)
			{
				Close(IDCANCEL);
			}
			else if (UILIB_GET(SaveBtn) == msg.pSender)
			{
				for (int i = 0;i < UILIB_GET(DomainList)->GetCount();i ++)
				{
					CListContainerElementUIEx* pListTextElementUi = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
					msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr = ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->GetText().GetData();
				}
                UpdateConfigToDownloadConfigDlg();
				Close(IDOK);
			}
			else if (UILIB_GET(ClearAllSortBtn) == msg.pSender)
			{
				for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
				{
					msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex = i + 1;
					msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort = -1;
					msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex = -1;

					msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort = -1;
					msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex = -1;
					//! 默认IP地址
					msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr = JGW_GetFormatWString(L"192.168.1.%d",i + 102);
				}
				UpdateSortListView();
			}
		}
	}

	void CCPEDownloadConfigDlg::CancelSort()
	{
		UILIB_GET(CancelSortBtn)->SetEnabled(false);
		for (int i = 0;i < UILIB_GET(DomainList)->GetCount();i ++)
		{
			CListContainerElementUIEx* pListTextElementUi = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
			((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->SetEnabled(true);
			((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->SetText(L"Sort");
		}
		mnCurrentSortIndex = -1;
	}

	bool CCPEDownloadConfigDlg::GetDuiContronlClass()
	{
		bool bSucceseful = true;

		//bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,CPEHostIPAddrEdt);
		bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,TftpdIpAddrEdt);
		//bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,SblEdt);
		//bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,CdtEdt);
		//bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,TzEdt);
		//bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DevEdt);
		//bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,RpmEdt);
		//bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,BootEdt);
        bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadFileParamEdt);

		bSucceseful &= UILIB_GETCONTROLPTR(CListTestCtrlUI,DomainList);
		bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,SaveBtn);
		bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,CancelBtn);
		bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,ClearAllSortBtn);
		bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,CancelSortBtn);
		bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadTimeEdt);
		bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadTimeOutEdt);

		return bSucceseful;
	}

	void CCPEDownloadConfigDlg::InitDownloadConfigDlgToConfig()
	{
		//UILIB_GET(CPEHostIPAddrEdt)->SetText(msMultiDownloadConfig.mstrCPEIPAddr.c_str());
		UILIB_GET(TftpdIpAddrEdt)->SetText(msMultiDownloadConfig.mstrTFtpdIPAddr.c_str());

        UILIB_GET(DownloadFileParamEdt)->SetText(msMultiDownloadConfig.mstrDownloadParam.c_str());
		//UILIB_GET(SblEdt)->SetText(msMultiDownloadConfig.mstrSBL.c_str());
		//UILIB_GET(CdtEdt)->SetText(msMultiDownloadConfig.mstrCDT.c_str());
		//UILIB_GET(TzEdt)->SetText(msMultiDownloadConfig.mstrTZ.c_str());
		//UILIB_GET(DevEdt)->SetText(msMultiDownloadConfig.mstrDEV.c_str());
		//UILIB_GET(RpmEdt)->SetText(msMultiDownloadConfig.mstrRPM.c_str());
		//UILIB_GET(BootEdt)->SetText(msMultiDownloadConfig.mstrBOOT.c_str());

		UILIB_GET(CancelSortBtn)->SetEnabled(false);
		UILIB_GET(DownloadTimeEdt)->SetText(JGW_GetFormatWString(L"%d",msMultiDownloadConfig.mnDownloadTime).c_str());
		UILIB_GET(DownloadTimeOutEdt)->SetText(JGW_GetFormatWString(L"%d",msMultiDownloadConfig.mnDownloadTimeOut).c_str());


		UILIB_GET(DomainList)->SetAttribute(L"itemalign",L"left");
		UILIB_GET(DomainList)->GetList()->RemoveAll();
		UILIB_GET(DomainList)->GetHeader()->SetScaleHeader(false);
		UILIB_GET(DomainList)->GetHeader()->RemoveAll();

		UILIB_GET(DomainList)->InsertColumn(0,_T("Index"),30);
		UILIB_GET(DomainList)->InsertColumn(1,_T("9008"),0);
		UILIB_GET(DomainList)->InsertColumn(2,_T("Serial Port"),0);
		UILIB_GET(DomainList)->InsertColumn(3,_T("CPE IP"),150);
		UILIB_GET(DomainList)->InsertColumn(4,_T(""),80);

		std::wstring strTemp;
		for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
		{
			CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->InsertContainerItem(_T("sigle_download_list_item.xml"),40);
			if (!plsitContainerItem) continue;
			JGW_FormatWString(strTemp,L"%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex);
			((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(0))->GetItemAt(0)->SetText(strTemp.c_str());
			//! 
			if (msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort > 0)
			{
				JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort);
				((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(strTemp.c_str());
			}

			if (msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort > 0)
			{
				JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort);
				((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(strTemp.c_str());
			}

			((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr.c_str());
			((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->SetDate((LPVOID)(i));
			((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->OnNotify += MakeDelegate(this,&CCPEDownloadConfigDlg::OnMsgSortBtnClick,_T("click"));
		}
	}

	void CCPEDownloadConfigDlg::UpdateSortListView(int index /* = -1 */)
	{
		std::wstring strTemp;
		if (NULL != UILIB_GET(DomainList))
		{
			if (-1 == index)
			{
				for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
				{
					CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
					if (msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort > 0)
					{
						JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort);
						((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(strTemp.c_str());
					}
					else
					{
						((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(L"");
					}

					if (msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort > 0)
					{
						JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort);
						((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(strTemp.c_str());
					}
					else
					{
						((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(L"");
					}
					((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr.c_str());
				}
			}
			else
			{
				CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->GetCListContainerElementUIEx(index);
				if (msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mnDonwloadComPort > 0)
				{
					JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mnDonwloadComPort);
					((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(strTemp.c_str());
				}

				if (msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mnSerialComPort > 0)
				{
					JGW_FormatWString(strTemp,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mnSerialComPort);
					((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(strTemp.c_str());
				}
				((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mstrCPEIPAddr.c_str());

			}
		}
	}

    void CCPEDownloadConfigDlg::SaveSortListView(int index)
    {
        if (index >= 0 && index < DOWNLOAD_THREAD_MAX)
        {
            std::wstring strTemp;
            CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->GetCListContainerElementUIEx(index);

            strTemp = ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->GetText();
            JGW_ReplaceStringW(strTemp,L"COM",L"");
            msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mnDonwloadComPort = _wtoi(strTemp.c_str());

            strTemp = ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->GetText();
            JGW_ReplaceStringW(strTemp,L"COM",L"");
            msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mnSerialComPort = _wtoi(strTemp.c_str());

            msMultiDownloadConfig.msMultiDownloadSortConfigs[index].mstrCPEIPAddr = ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->GetText();
        }

    }

	bool CCPEDownloadConfigDlg::OnMsgSortBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
	{
		//! 取消排序
        if (mnCurrentSortIndex >= 0)
		{
            SaveSortListView(mnCurrentSortIndex);
			CancelSort();
			UpdateSortListView();
			return false;
		}

		//! 开始排序
		mnCurrentSortIndex = (int)pTNotifyUI->pSender->GetDate();
		UILIB_GET(CancelSortBtn)->SetEnabled(true);
		for (int i = 0;i < UILIB_GET(DomainList)->GetCount();i ++)
		{
			CListContainerElementUIEx* pListTextElementUi = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
			if (i != mnCurrentSortIndex) 
			{
				((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->SetEnabled(false);
			}
			else
			{
				/*((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(1))->GetItemAt(0)->SetText(L"");
				((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(2))->GetItemAt(0)->SetText(L"");*/
				((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->SetText(L"取消排序"); 
			}
		}
		return true;
	}

	void CCPEDownloadConfigDlg::UpdateConfigToDownloadConfigDlg()
	{
		//msMultiDownloadConfig.mstrCPEIPAddr = UILIB_GET(CPEHostIPAddrEdt)->GetText().GetData();
		msMultiDownloadConfig.mstrTFtpdIPAddr = UILIB_GET(TftpdIpAddrEdt)->GetText().GetData();
        msMultiDownloadConfig.mstrDownloadParam = UILIB_GET(DownloadFileParamEdt)->GetText().GetData();
		//msMultiDownloadConfig.mstrSBL = UILIB_GET(SblEdt)->GetText().GetData();
		//msMultiDownloadConfig.mstrCDT = UILIB_GET(CdtEdt)->GetText().GetData();
		//msMultiDownloadConfig.mstrTZ = UILIB_GET(TzEdt)->GetText().GetData();
		//msMultiDownloadConfig.mstrDEV = UILIB_GET(DevEdt)->GetText().GetData();
		//msMultiDownloadConfig.mstrRPM = UILIB_GET(RpmEdt)->GetText().GetData();
		//msMultiDownloadConfig.mstrBOOT = UILIB_GET(BootEdt)->GetText().GetData();

		msMultiDownloadConfig.mnDownloadTime = _wtoi(UILIB_GET(DownloadTimeEdt)->GetText().GetData());
		msMultiDownloadConfig.mnDownloadTimeOut = _wtoi(UILIB_GET(DownloadTimeOutEdt)->GetText().GetData());
	}
}

