#include "StdAfx.h"
#include "CJGW_DeviceQueueSortConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_ReportDeviceInfo/CJGW_ReportDeviceInfoExport.h>
namespace JGW
{
    CCJGW_DeviceQueueSortConfigDlg::CCJGW_DeviceQueueSortConfigDlg(HWND hParentHwnd,const S_DEVICEQUEUE_PROGRAM_INFO& sDevoceQueueProgramInfo,std::wstring& strTFTPD32IPAddr,std::wstring& strDownloadFileConfig,CCJGW_DeviceQueueSortConfig& cDeviceQueueSortConfig) : mhParentHwnd(hParentHwnd),msDevoceQueueProgramInfo(sDevoceQueueProgramInfo),mstrTFTPD32IPAddr(strTFTPD32IPAddr),mstrDownloadFileConfig(strDownloadFileConfig),mnMaxSortIndex(8),mcDeviceQueueSortConfig(cDeviceQueueSortConfig),mnCurrentSortIndex(0)
    {
        mstrSkinFile = L"cpe_download_config.xml";
    }


    CCJGW_DeviceQueueSortConfigDlg::~CCJGW_DeviceQueueSortConfigDlg(void)
    {
    }

    int CCJGW_DeviceQueueSortConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),mstrSkinFile.c_str(),_T("JaGuarWave Device Queue Sort Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),mstrSkinFile.c_str()_T("JaGuarWave Device Queue Sort Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),mstrSkinFile.c_str(),_T("JaGuarWave Device Queue Sort Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_DeviceQueueSortConfigDlg::OnInitWindow()
    {
        if (!GetDuiContronlClass())
        {
            Close(IDCANCEL);
            return;
        }
        //! 根据下载配置参数，初始化配置界面
        InitDownloadConfigDlgToConfig();
    }

    UINT CCJGW_DeviceQueueSortConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    void CCJGW_DeviceQueueSortConfigDlg::CancelSort()
    {
        UILIB_GET(CancelSortBtn)->SetEnabled(false);
        for (int i = 0;i < UILIB_GET(DomainList)->GetCount();i ++)
        {
            CListContainerElementUIEx* pListTextElementUi = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
            if (msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_QUEUE_PROGRAM)
            {
                ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->SetEnabled(true);
                ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->SetText(L"Sort");
            }
            else if (msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_NETCARD_QUEUE_PROGRAM)
            {
                ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->SetEnabled(true);
                ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->SetText(L"Sort");
            }  
        }
        mnCurrentSortIndex = -1;
    }

    void CCJGW_DeviceQueueSortConfigDlg::OnNotify(TNotifyUI& msg)
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
                    mcDeviceQueueSortConfig.GetDeviceSortConfigToViewIndex(i).mstrThreeDevice = ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->GetText().GetData();
                }
                UpdateConfigToDownloadConfigDlg();
                Close(IDOK);
            }
            else if (UILIB_GET(ClearAllSortBtn) == msg.pSender)
            {
                mcDeviceQueueSortConfig.ClearDeviceQueueSortConfig();
                for (int i = 0;i < mnMaxSortIndex;i ++)
                {
                    S_DEVICESORT_CONFIG& sDeviceSortConfig = mcDeviceQueueSortConfig.GetDeviceSortConfigToViewIndex(i);
                    sDeviceSortConfig.mnFristDeviceSortIndex = -1;
                    sDeviceSortConfig.mnSecondDeviceSortIndex = -1;
                    sDeviceSortConfig.mnThreeDeviceSortIndex = -1;
                    sDeviceSortConfig.mstrFristDevice = L"";
                    sDeviceSortConfig.mstrSecondDevice = L"";
                    sDeviceSortConfig.mstrThreeDevice = JGW_GetFormatWString(L"%s.%d",msDevoceQueueProgramInfo.mstrNetCardHostIPSeg.c_str(),i + 102);
                }
                UpdateSortListView();
            }
        }
    }

    void CCJGW_DeviceQueueSortConfigDlg::UpdateConfigToDownloadConfigDlg()
    {
        mstrTFTPD32IPAddr = UILIB_GET(TftpdIpAddrEdt)->GetText().GetData();
        mstrDownloadFileConfig = UILIB_GET(DownloadFileParamEdt)->GetText().GetData();
    }

    void CCJGW_DeviceQueueSortConfigDlg::OnHandleDeviceMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,const LPARAM& lParam)
    {
        if(!wParam || mnCurrentSortIndex < 0) return;
        if (msDevoceQueueProgramInfo.meDeviceQueueProgram != E_DUAL_PORT_QUEUE_PROGRAM && msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_NETCARD_QUEUE_PROGRAM) return;

        CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->GetCListContainerElementUIEx(mnCurrentSortIndex);
        S_DEVICESORT_CONFIG sDeviceSortConfig = mcDeviceQueueSortConfig.GetDeviceSortConfigToViewIndex(mnCurrentSortIndex);
        if (E_COM_MSG_TYPE == eMsgType)
        {
            SDeviceMsg_Info* lpdevicemsg = (SDeviceMsg_Info*)(wParam);
            E_DEVICE_MODE eDeviceMode = (E_DEVICE_MODE)lpdevicemsg->m_nDeviceMode;
            if (msDevoceQueueProgramInfo.meFristDeviceMode == eDeviceMode)
            {
                sDeviceSortConfig.mnFristDeviceSortIndex = lpdevicemsg->m_nSortIndex;
                sDeviceSortConfig.mstrFristDevice = JGW_GetFormatWString(L"%d",lpdevicemsg->m_nComPort);
                if (eDeviceMode == E_ADB_MODE || eDeviceMode == E_FASTBOOT_MODE)
                {
                    sDeviceSortConfig.mstrFristDevice = lpdevicemsg->m_strSeralNumber;
                } 
                UpdateListContainerElementUI(mnCurrentSortIndex,plsitContainerItem,sDeviceSortConfig);
            }
            if (msDevoceQueueProgramInfo.meSecondDeviceMode == (E_DEVICE_MODE)lpdevicemsg->m_nDeviceMode)
            {
                sDeviceSortConfig.mnSecondDeviceSortIndex = lpdevicemsg->m_nSortIndex;
                sDeviceSortConfig.mstrSecondDevice = JGW_GetFormatWString(L"%d",lpdevicemsg->m_nComPort);
                if (eDeviceMode == E_ADB_MODE || eDeviceMode == E_FASTBOOT_MODE)
                {
                    sDeviceSortConfig.mstrSecondDevice = lpdevicemsg->m_strSeralNumber;
                } 
                UpdateListContainerElementUI(mnCurrentSortIndex,plsitContainerItem,sDeviceSortConfig);
            }
            lpdevicemsg->Dec();
        }
        else if (E_NETCARD_MSG_TYPE == eMsgType)
        {
            PS_NETCARD_INFO psNetCardInfo = reinterpret_cast<PS_NETCARD_INFO>(wParam);
            if (msDevoceQueueProgramInfo.meFristDeviceMode == E_LAN_MODE)
            {
                sDeviceSortConfig.mnFristDeviceSortIndex = psNetCardInfo->mnSortIndex;
                sDeviceSortConfig.mstrFristDevice = psNetCardInfo->mstrAdapterName;
                UpdateListContainerElementUI(mnCurrentSortIndex,plsitContainerItem,sDeviceSortConfig);
            }
            if (msDevoceQueueProgramInfo.meSecondDeviceMode == E_LAN_MODE)
            {
                sDeviceSortConfig.mnSecondDeviceSortIndex = psNetCardInfo->mnSortIndex;
                sDeviceSortConfig.mstrSecondDevice = psNetCardInfo->mstrAdapterName;
                UpdateListContainerElementUI(mnCurrentSortIndex,plsitContainerItem,sDeviceSortConfig);
            }
        }
    }

    bool CCJGW_DeviceQueueSortConfigDlg::GetDuiContronlClass()
    {
        bool bSucceseful = true;

        bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,TftpdIpAddrEdt);
        bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadFileParamEdt);

        bSucceseful &= UILIB_GETCONTROLPTR(CListTestCtrlUI,DomainList);
        bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,SaveBtn);
        bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,CancelBtn);
        bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,ClearAllSortBtn);
        bSucceseful &= UILIB_GETCONTROLPTR(CButtonUI,CancelSortBtn);
        bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadTimeEdt);
        bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadTimeOutEdt);
        bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadTimeLabel);
        bSucceseful &= UILIB_GETCONTROLPTR(CEditUI,DownloadTimeOutLabel);

        return bSucceseful;
    }

    std::wstring CCJGW_DeviceQueueSortConfigDlg::GetColumnNameToDeviceMode(E_DEVICE_MODE eDeviceMode)
    {
        switch (eDeviceMode)
        {
        case E_DIAG_MODE:
            return L"Diagnostics";
        case E_ADB_MODE:
            return L"ADB";
        case E_MODEM_MODE:
            return L"Modem";
        case E_FASTBOOT_MODE:
            return L"Fastboot";
        case E_DOWNLOADER_MODE:
            return L"Downloader";
        case E_LAN_MODE:
            return L"LAN";
        }
        return L"UnknowMode";
    }

    void CCJGW_DeviceQueueSortConfigDlg::UpdateListContainerElementUI(int index,CListContainerElementUIEx* plsitContainerItem,S_DEVICESORT_CONFIG& sDeviceSortConfig)
    {
        if (msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_QUEUE_PROGRAM)
        {
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(0))->GetItemAt(0)->SetText(JGW_GetFormatWString(L"%d",index + 1).c_str());
            if (-1 != sDeviceSortConfig.mnFristDeviceSortIndex)
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(sDeviceSortConfig.mstrFristDevice.c_str());
            }
            else
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(L"");
            }

            if (-1 != sDeviceSortConfig.mnSecondDeviceSortIndex)
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(sDeviceSortConfig.mstrSecondDevice.c_str());
            }
            else
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(L"");
            }
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetDate((LPVOID)(index));
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->OnNotify += MakeDelegate(this,&CCJGW_DeviceQueueSortConfigDlg::OnMsgSortBtnClick,_T("click"));
        }
        else if (msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_NETCARD_QUEUE_PROGRAM)
        {
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(0))->GetItemAt(0)->SetText(JGW_GetFormatWString(L"%d",index + 1).c_str());
            if (-1 != sDeviceSortConfig.mnFristDeviceSortIndex)
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(sDeviceSortConfig.mstrFristDevice.c_str());
            }
            else
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(L"");
            }

            if (-1 != sDeviceSortConfig.mnSecondDeviceSortIndex)
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(sDeviceSortConfig.mstrSecondDevice.c_str());
            }
            else
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(L"");
            }

            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(sDeviceSortConfig.mstrThreeDevice.c_str());
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->SetDate((LPVOID)(index));
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->OnNotify += MakeDelegate(this,&CCJGW_DeviceQueueSortConfigDlg::OnMsgSortBtnClick,_T("click"));
        }
    }

    void CCJGW_DeviceQueueSortConfigDlg::InitDownloadConfigDlgToConfig()
    {
        //UILIB_GET(CPEHostIPAddrEdt)->SetText(msMultiDownloadConfig.mstrCPEIPAddr.c_str());
        UILIB_GET(TftpdIpAddrEdt)->SetText(mstrTFTPD32IPAddr.c_str());
        UILIB_GET(DownloadFileParamEdt)->SetText(mstrDownloadFileConfig.c_str());
        UILIB_GET(CancelSortBtn)->SetEnabled(false);
        UILIB_GET(DownloadTimeEdt)->SetVisible(false);
        UILIB_GET(DownloadTimeOutEdt)->SetVisible(false);
        UILIB_GET(DownloadTimeLabel)->SetVisible(false);
        UILIB_GET(DownloadTimeOutLabel)->SetVisible(false);

        UILIB_GET(DomainList)->SetAttribute(L"itemalign",L"left");
        UILIB_GET(DomainList)->GetList()->RemoveAll();
        UILIB_GET(DomainList)->GetHeader()->SetScaleHeader(false);
        UILIB_GET(DomainList)->GetHeader()->RemoveAll();

        UILIB_GET(DomainList)->InsertColumn(0,_T("Index"),30);
        if (msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_QUEUE_PROGRAM)
        {
            UILIB_GET(DomainList)->InsertColumn(1,GetColumnNameToDeviceMode(msDevoceQueueProgramInfo.meFristDeviceMode).c_str(),0);
            UILIB_GET(DomainList)->InsertColumn(2,GetColumnNameToDeviceMode(msDevoceQueueProgramInfo.meSecondDeviceMode).c_str(),0);
            UILIB_GET(DomainList)->InsertColumn(3,_T(""),80);

            for (int i = 0;i < mnMaxSortIndex;i ++)
            {
                CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->InsertContainerItem(_T("dual_port_queue_program_item.xml"),40);
                if (!plsitContainerItem) continue;
                S_DEVICESORT_CONFIG& sDeviceSortConfig = mcDeviceQueueSortConfig.GetDeviceSortConfigToViewIndex(i);
                UpdateListContainerElementUI(i,plsitContainerItem,sDeviceSortConfig);
            }
        }
        else if (msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_NETCARD_QUEUE_PROGRAM)
        {
            UILIB_GET(DomainList)->InsertColumn(1,GetColumnNameToDeviceMode(msDevoceQueueProgramInfo.meFristDeviceMode).c_str(),0);
            UILIB_GET(DomainList)->InsertColumn(2,GetColumnNameToDeviceMode(msDevoceQueueProgramInfo.meSecondDeviceMode).c_str(),0);
            UILIB_GET(DomainList)->InsertColumn(3,_T("CPE IP"),150);
            UILIB_GET(DomainList)->InsertColumn(4,_T(""),80);

            for (int i = 0;i < mnMaxSortIndex;i ++)
            {
                CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->InsertContainerItem(_T("sigle_download_list_item.xml"),40);
                if (!plsitContainerItem) continue;
                S_DEVICESORT_CONFIG& sDeviceSortConfig = mcDeviceQueueSortConfig.GetDeviceSortConfigToViewIndex(i);
                sDeviceSortConfig.mstrThreeDevice = JGW_GetFormatWString(L"%s.%d",msDevoceQueueProgramInfo.mstrNetCardHostIPSeg.c_str(),i + 102);
                UpdateListContainerElementUI(i,plsitContainerItem,sDeviceSortConfig);
            }
        }
    }

    bool CCJGW_DeviceQueueSortConfigDlg::OnMsgSortBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
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
        int nItemIndex = (msDevoceQueueProgramInfo.meDeviceQueueProgram == E_DUAL_PORT_QUEUE_PROGRAM) ? 3 : 4;

        for (int i = 0;i < UILIB_GET(DomainList)->GetCount();i ++)
        {
            CListContainerElementUIEx* pListTextElementUi = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
            if (i != mnCurrentSortIndex) 
            {
                ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(nItemIndex))->GetItemAt(0)->SetEnabled(false);
            }
            else
            {
                ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(nItemIndex))->GetItemAt(0)->SetText(L"取消排序"); 
            }
        }
        return true;
    }

    void CCJGW_DeviceQueueSortConfigDlg::UpdateSortListView(int index /* = -1 */)
    {
        std::wstring strTemp;
        if (NULL != UILIB_GET(DomainList))
        {
            if (-1 == index)
            {
                for (int i = 0;i < mnMaxSortIndex;i ++)
                {
                    CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->GetCListContainerElementUIEx(i);
                    S_DEVICESORT_CONFIG& sDeviceSortConfig = mcDeviceQueueSortConfig.GetDeviceSortConfigToViewIndex(i);
                    UpdateListContainerElementUI(i,plsitContainerItem,sDeviceSortConfig);
                }
            }
            else
            {
                CListContainerElementUIEx* plsitContainerItem = UILIB_GET(DomainList)->GetCListContainerElementUIEx(index);
                S_DEVICESORT_CONFIG& sDeviceSortConfig = mcDeviceQueueSortConfig.GetDeviceSortConfigToViewIndex(index);
                UpdateListContainerElementUI(index,plsitContainerItem,sDeviceSortConfig);
            }
        }
    }

    void CCJGW_DeviceQueueSortConfigDlg::SaveSortListView(int index)
    {
        if (index >= 0 && index < mnMaxSortIndex)
        {
            mcDeviceQueueSortConfig.UpdateDeviceQueueSortConfig(index);
        }
    }
}
