#include "StdAfx.h"
#include "CJGW_APSUpgradeConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
/*#include <regex>*/
namespace JGW
{
    CCJGW_APSUpgradeConfigDlg::CCJGW_APSUpgradeConfigDlg(HWND hParentHwnd,LPS_APS_MULTI_UPGRADE_CONFIG psAPSMultiUpgradeConfig,PIP_ADAPTER_INFO pTempIpAdapterInfo) : m_hParentHwnd(hParentHwnd),mpsAPSMultiUpgradeConfig(psAPSMultiUpgradeConfig),mpMainList(NULL),mpIpAdapterInfo(pTempIpAdapterInfo)
    {
    }


    CCJGW_APSUpgradeConfigDlg::~CCJGW_APSUpgradeConfigDlg(void)
    {
    }

    void CCJGW_APSUpgradeConfigDlg::OnCloseWindow()
    {
        for (int i = 0;i < mpMainList->GetCount();i ++)
        {
            CListContainerElementUIEx* pListTextElementUi = mpMainList->GetCListContainerElementUIEx(i);
            ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->OnNotify -= MakeDelegate(this,&CCJGW_APSUpgradeConfigDlg::OnSetMsgBtnClick,_T("click"));
            ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(1)->OnNotify -= MakeDelegate(this,&CCJGW_APSUpgradeConfigDlg::OnChangeMsgBtnClick,_T("click"));
        }
    }

    UINT CCJGW_APSUpgradeConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    int CCJGW_APSUpgradeConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("aps_upgrade_config.xml"),_T("JaGuarWave APS Upgrade Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("aps_upgrade_config.xml"),_T("JaGuarWave APS Upgrade Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("aps_upgrade_config.xml"),_T("JaGuarWave APS Upgrade Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_APSUpgradeConfigDlg::OnInitWindow()
    {
        bool bSucceseful = true;

        mpOkOptionUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"OkBtn"));
        mpCancelOptionUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"CancelBtn"));
        mpUpgradeVersionEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"UpgradeVersionEdt"));
        mpUpgradeTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"UpgradeTimeEdt"));
        mpHostIPAddrEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"HostIPAddrEdt"));
        mpMainList = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(L"domainlist"));
        mpDownloadLogPathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"DownloadLogPathEdt"));
        //mpSelectDownloadLogPathButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"SelectDownloadLogPathButton"));
        mpSaveDownloadLogComCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"SaveDownloadLogComCheckBox"));

        bSucceseful &= (NULL != mpOkOptionUI);
        bSucceseful &= (NULL != mpCancelOptionUI);
        bSucceseful &= (NULL != mpUpgradeVersionEdt);
        bSucceseful &= (NULL != mpUpgradeTimeEdt);
        bSucceseful &= (NULL != mpHostIPAddrEdt);
        bSucceseful &= (NULL != mpMainList);
        bSucceseful &= (NULL != mpDownloadLogPathEdt);
        //bSucceseful &= (NULL != mpSelectDownloadLogPathButton);
        bSucceseful &= (NULL != mpSaveDownloadLogComCheckBox);

        if (!bSucceseful)
        {
            Close(IDCANCEL);
            return ;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"%d",mpsAPSMultiUpgradeConfig->mnDownloadTime);
        mpHostIPAddrEdt->SetText(mpsAPSMultiUpgradeConfig->mstrHostIPAddr.c_str());
        mpUpgradeVersionEdt->SetText(mpsAPSMultiUpgradeConfig->mstrUpgradeVersion.c_str());
        mpUpgradeTimeEdt->SetText(strTemp.c_str());

        //mpSelectDownloadLogPathButton->SetEnabled(mpsAPSMultiUpgradeConfig->mbSaveDownloadLog);
        mpSaveDownloadLogComCheckBox->SetCheck(mpsAPSMultiUpgradeConfig->mbSaveDownloadLog);
        mpDownloadLogPathEdt->SetText(mpsAPSMultiUpgradeConfig->mstrSaveDownloadLogFolder.c_str());

        AddDefaultList();
    }

    void CCJGW_APSUpgradeConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpOkOptionUI == msg.pSender)
            {
                mpsAPSMultiUpgradeConfig->mstrHostIPAddr = mpHostIPAddrEdt->GetText().GetData();
                mpsAPSMultiUpgradeConfig->mstrUpgradeVersion = mpUpgradeVersionEdt->GetText().GetData();
                mpsAPSMultiUpgradeConfig->mnDownloadTime = _ttoi(mpUpgradeTimeEdt->GetText().GetData());
                mpsAPSMultiUpgradeConfig->mbSaveDownloadLog = mpSaveDownloadLogComCheckBox->GetCheck();
                mpsAPSMultiUpgradeConfig->mstrSaveDownloadLogFolder = mpDownloadLogPathEdt->GetText().GetData();
                mpsAPSMultiUpgradeConfig->mstrHostIPAddrAnsi = JGW_W2A(mpsAPSMultiUpgradeConfig->mstrHostIPAddr);
                mpsAPSMultiUpgradeConfig->mstrHostIPSeg = mpsAPSMultiUpgradeConfig->mstrHostIPAddr.substr(0,mpsAPSMultiUpgradeConfig->mstrHostIPAddr.find_last_of('.') + 1);
                Close(IDOK);
            }
            else if (mpCancelOptionUI == msg.pSender)
            {
                Close(IDCANCEL);
            }
        }
        else if (msg.sType == DUI_MSGTYPE_EDITREGEX)
        {
            bool bResult = (msg.wParam == IDYES)?true:false;
            mmapCheckEditRegexResult[msg.pSender] = bResult;
        }
    }

    void CCJGW_APSUpgradeConfigDlg::AddDefaultList()
    {
        CListContainerElementUIEx* lplsitContainerItem = NULL;
        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;
        PIP_ADAPTER_INFO pTempIpAdapterInfo = mpIpAdapterInfo;
        std::wstring strApdateName,strNetName,strDes,strIPAddr;

        while (pTempIpAdapterInfo)
        {
            CDialogBuilder builder1;
            lplsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(_T("aps_upgrade_config_lit_item.xml"),(UINT)0,NULL,&m_PaintManager));
            if( !lplsitContainerItem ) continue;
            if( lplsitContainerItem != NULL ) mpMainList->InsertItem(mpMainList->GetCount(), 30, lplsitContainerItem);

            strApdateName = JGW_A2W_A(pTempIpAdapterInfo->AdapterName);
            strNetName = JGW_GetNetcardDeviceNameToAdapterName(strApdateName.c_str());
            strDes = JGW_A2W_A(pTempIpAdapterInfo->Description);

            ((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(0))->GetItemAt(0)->SetText(strNetName.c_str());
            ((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText(strDes.c_str());

            if (!JGW_GetAdpateDHCPStatus(strApdateName.c_str()))
            {
                strIPAddr = JGW_GetStaticIPAddrToAdpaterName(strApdateName.c_str());
                ((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(strIPAddr.c_str());
            }
            CEditUI* pEdit = (CEditUI*)(((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(2))->GetItemAt(0));
            mmapCheckEditRegexResult[pEdit] = true;
            mmapApdaters[pEdit] = strApdateName;
            pEdit->SetReadOnly(true);

            ((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(3))->GetItemAt(0)->OnNotify += MakeDelegate(this,&CCJGW_APSUpgradeConfigDlg::OnSetMsgBtnClick,_T("click"));
            ((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetDate((LPVOID)mpMainList->GetCount());
            ((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(3))->GetItemAt(2)->OnNotify += MakeDelegate(this,&CCJGW_APSUpgradeConfigDlg::OnChangeMsgBtnClick,_T("click"));
            ((CHorizontalLayoutUI*)lplsitContainerItem->GetItemAt(3))->GetItemAt(2)->SetDate((LPVOID)mpMainList->GetCount());
            pTempIpAdapterInfo = pTempIpAdapterInfo->Next;
        }
    }

    bool CCJGW_APSUpgradeConfigDlg::ReadCMDProcessPipeEOF(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec /* = 10 */)
    {
        if (!pProcessPipe) return false;
        strRead = L"";
        int readSize = 0;
        jgw_timer tm;
        std::wstring strTemp;

        while(tm.elapsed() < nTimeOutSec)
        {
            readSize = pProcessPipe->recv(strTemp);
            if( -1 == readSize )
            {
                PELOG4WW(_T("error: process closed!!!"));
                return false;
            }
            else if ( readSize > 0 )
            {
                //trim::ctrim(strTemp);  //! JGW_ReplaceStringW(strTemp,_T("\r"),_T("\t")); //! JGW_ReplaceStringW(strTemp,_T("\n"),_T("\t"));
                if( strTemp.length() <= 0 ) continue;
                strRead += strTemp;
                PDLOG4WW(strTemp.c_str());
                if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
                {
                    return true;
                }
            }
        }
        pProcessPipe->CloseProcessPipe();
        GetProcessPipe();
        return false;
    }

    CCJGW_ProcessPipe* CCJGW_APSUpgradeConfigDlg::GetProcessPipe()
    {
        if (!mProcessPipe.CheckProcessIsRun())
        {
            std::wstring strApplicationName = L"cmd.exe",strApplicationFilePath;
            strApplicationFilePath = JGW_RealativePathToAbsPath(strApplicationName.c_str());
            if (!mProcessPipe.InitProcessPipe(strApplicationFilePath.c_str(),L"",JGW_GetApplicationFolder())) { PELOG4WW(L"Failed to initialize CMD process"); return NULL;}
            ReadCMDProcessPipeEOF(&mProcessPipe,strApplicationName);
        }
        return &mProcessPipe;
    }

    //! netsh interface ip set address "本地连接 6" static 192.168.213.5 255.255.255.0
    bool CCJGW_APSUpgradeConfigDlg::OnSetMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
    {
        std::wstring strIPAddr,strNetName,strCommand,strRead;
        int i = (int)pTNotifyUI->pSender->GetDate();
        CListContainerElementUIEx* pListTextElementUi = mpMainList->GetCListContainerElementUIEx(i - 1);
        if (!pListTextElementUi) return false;

        CEditUI* pEdit = (CEditUI*)(((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(2))->GetItemAt(0));
        strIPAddr = pEdit->GetText().GetData();
        
        if (!mmapCheckEditRegexResult[pEdit]) return false;

        strNetName = ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(0))->GetItemAt(0)->GetText().GetData();
        JGW_FormatWString(strCommand,L"netsh interface ip set address \"%s\" static %s 255.255.255.0",strNetName.c_str(),strIPAddr.c_str());

        CCJGW_ProcessPipe* pProcessPipe = GetProcessPipe();
        pProcessPipe->timed_send(strCommand.c_str(),20);
        ReadCMDProcessPipeEOF(pProcessPipe,strRead);
        if (strRead.at(strRead.length() - 1) == _T('>'))
        {
            JGW_EraseFristToRemoveCharW(strRead,L'\n');
            JGW_EraseAfterToRemoveCharW(strRead,L'\n'); 
        }    
        JGW_EraseLastAndFristTwoCharsW(strRead);

        if (strRead.empty())
        {
            JGW_SetAdpaterDHCPStatus(mmapApdaters[pEdit].c_str());
            pEdit->SetReadOnly(true);
            ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->SetEnabled(false);
            ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(2)->SetEnabled(true);
        }
        else 
        {
            MessageBox(m_hWnd,strRead.c_str(),L"error",MB_ICONERROR);
        }

        return true;
    }

    bool CCJGW_APSUpgradeConfigDlg::OnChangeMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
    {
        int i = (int)pTNotifyUI->pSender->GetDate();
        CListContainerElementUIEx* pListTextElementUi = mpMainList->GetCListContainerElementUIEx(i - 1);
        if (!pListTextElementUi) return false;
        ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->SetEnabled(true);
        ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(2)->SetEnabled(false);

        CEditUI* pEdit = (CEditUI*)(((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(2))->GetItemAt(0));
        pEdit->SetReadOnly(false);
        pEdit->SetFocus();

        return true;
    }
}