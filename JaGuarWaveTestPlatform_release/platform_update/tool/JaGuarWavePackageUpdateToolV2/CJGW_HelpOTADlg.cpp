#include "StdAfx.h"
#include "CJGW_HelpOTADlg.h"
#include "CJGW_OTASQLite.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_PackageConfigCommonFunc.h"

#include <JGW_CurlPlugin/CJGW_CurlFtp.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCJGW_HelpOTADlg::CCJGW_HelpOTADlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig) : CHelpPackageUIInterface(psPackageUpdateConfig)
    {
    }


    CCJGW_HelpOTADlg::~CCJGW_HelpOTADlg(void)
    {
    }

    bool CCJGW_HelpOTADlg::InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI)
    {
        mpPaintManagerUI = pPaintManagerUI;
        {
            bool result = true;
            FIND_UI(CComboBoxUI,OTAInstallVersionCombo,pPaintManagerUI);
            FIND_UI(CComboBoxUI,OTAOTAVersionCombo,pPaintManagerUI);
            FIND_UI(CButtonUI,OTADownloadOTABtn,pPaintManagerUI);
            FIND_UI(CEditUI,OTAOTAFolderEdt,pPaintManagerUI);
            FIND_UI(CEditUI,OTAVersionEdit,pPaintManagerUI);
            FIND_UI(CEditUI,OTANameEdit,pPaintManagerUI);
            FIND_UI(CEditUI,OTADesEdit,pPaintManagerUI);
            FIND_UI(CButtonUI,UpdateOTABtn,pPaintManagerUI);
            result &= CHECK_UI(OTAInstallVersionCombo);
            result &= CHECK_UI(OTAOTAVersionCombo);
            result &= CHECK_UI(OTADownloadOTABtn);
            result &= CHECK_UI(OTAOTAFolderEdt);
            result &= CHECK_UI(OTAVersionEdit);
            result &= CHECK_UI(OTANameEdit);
            result &= CHECK_UI(OTADesEdit);
            result &= CHECK_UI(UpdateOTABtn);

            FIND_UI(CComboBoxUI,AddOTAInstallVersionCombo,pPaintManagerUI);
            FIND_UI(CButtonUI,AddOTAFolderBtn,pPaintManagerUI);
            FIND_UI(CEditUI,AddOTAFolderEdt,pPaintManagerUI);
            FIND_UI(CEditUI,AddOTAVersionEdit,pPaintManagerUI);
            FIND_UI(CEditUI,AddOTANameEdit,pPaintManagerUI);
            FIND_UI(CEditUI,AddOTADesEdit,pPaintManagerUI);
            FIND_UI(CButtonUI,UploadAddOTABtn,pPaintManagerUI);
            result &= CHECK_UI(AddOTAInstallVersionCombo);
            result &= CHECK_UI(AddOTAFolderBtn);
            result &= CHECK_UI(AddOTAFolderEdt);
            result &= CHECK_UI(AddOTAVersionEdit);
            result &= CHECK_UI(AddOTANameEdit);
            result &= CHECK_UI(AddOTADesEdit);
            result &= CHECK_UI(UploadAddOTABtn);

            if (!result)
            {
                return false;
            }
            mpOTADownloadOTABtn->SetEnabled(false);
            mpUpdateOTABtn->SetEnabled(false);
            mpOTAOTAVersionCombo->SetEnabled(false);
            mpUploadAddOTABtn->SetEnabled(false);
        }
        {
            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsInstallVersion.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = mpsPackageUpdateConfig->mvsInstallVersion[index].mstrVersion;
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate((LPVOID)mpsPackageUpdateConfig->mvsInstallVersion[index].mnID);
                mpOTAInstallVersionCombo->Add(pListLabelEle);
            }

            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsInstallVersion.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = mpsPackageUpdateConfig->mvsInstallVersion[index].mstrVersion;
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate((LPVOID)mpsPackageUpdateConfig->mvsInstallVersion[index].mnID);
                mpAddOTAInstallVersionCombo->Add(pListLabelEle);
            }
        }
        return true;
    }

    void CCJGW_HelpOTADlg::OnNotifyHelpPackageUI(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (msg.pSender == mpOTAInstallVersionCombo)
            {
                while (mpOTAOTAVersionCombo->GetCount() > 1) mpOTAOTAVersionCombo->RemoveAt(1);
                mpOTAOTAVersionCombo->SetEnabled(false);
                mpOTADownloadOTABtn->SetEnabled(false);
                mpOTAOTAFolderEdt->SetText(L"");
                mpOTAVersionEdit->SetText(L"");
                mpOTANameEdit->SetText(L"");
                mpOTADesEdit->SetText(L"");
                mpUpdateOTABtn->SetEnabled(false);
                if (mpOTAInstallVersionCombo->GetCurSel() > 0)
                {
                    msOTAUpdateInfo.mnInstallVersionID = (int)mpOTAInstallVersionCombo->GetItemAt(mpOTAInstallVersionCombo->GetCurSel())->GetDate();
                    msOTAUpdateInfo.mstrInstallVersion = mpOTAInstallVersionCombo->GetText();
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_OTA_UPDATE_SELECT_INSTALLVERSION_ACTION_ID,NULL);
                }
            }
            else if (msg.pSender == mpAddOTAInstallVersionCombo)
            {
                mpUploadAddOTABtn->SetEnabled(false);
                mpAddOTAFolderBtn->SetEnabled(false);
                mpAddOTADesEdit->SetText(L"");
                mpAddOTANameEdit->SetText(L"");
                mpAddOTAVersionEdit->SetText(L"");
                mpAddOTAFolderEdt->SetText(L"");

                if (mpAddOTAInstallVersionCombo->GetCurSel() > 0)
                {
                    msOTAAddInfo.mnInstallVersionID = (int)mpAddOTAInstallVersionCombo->GetItemAt(mpAddOTAInstallVersionCombo->GetCurSel())->GetDate();
                    msOTAAddInfo.mstrInstallVersion = mpAddOTAInstallVersionCombo->GetText();
                    mpUploadAddOTABtn->SetEnabled(true);
                    mpAddOTAFolderBtn->SetEnabled(true);
                }
            }
            else if (msg.pSender == mpOTAOTAVersionCombo)
            {
                mpOTADownloadOTABtn->SetEnabled(false);
                if (mpOTAOTAVersionCombo->GetCurSel() > 0)
                {
                    mpOTADownloadOTABtn->SetEnabled(true);
                }
            }
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpOTADownloadOTABtn)
            {
                mpUpdateOTABtn->SetEnabled(false);
                if (mpOTAOTAVersionCombo->GetCurSel() > 0)
                {
                    S_OTA *psOTA = (S_OTA *)mpOTAOTAVersionCombo->GetItemAt(mpOTAOTAVersionCombo->GetCurSel())->GetDate();
                    msOTAUpdateInfo.mnOTAID = psOTA->mnID;
                    msOTAUpdateInfo.mstrVersion = psOTA->mstrVersion;
                    msOTAUpdateInfo.mstrFtpUrl = psOTA->mstrFTPUrl;
                    mpOTAOTAVersionCombo->SetEnabled(false);
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_OTA_UPDATE_DOWNLOAD_OTA_ACTION_ID,NULL);
                }
            }
            else if (msg.pSender == mpUpdateOTABtn)
            {
                mpOTAInstallVersionCombo->SetEnabled(false);
                mpUpdateOTABtn->SetEnabled(false);
                mpOTAOTAVersionCombo->SetEnabled(false);
                mpOTADownloadOTABtn->SetEnabled(false);
                msOTAUpdateInfo.mstrUpdateOTAVersion = CCJGW_PackageConfigCommonFunc::GetOTAVersion();
                //! msOTAUpdateInfo.mstrLocalUpdateOTAFolder = mpOTAOTAFolderEdt->GetText().GetData();
                msOTAUpdateInfo.mstrLocalUpdateOTAZipPath = CCJGW_PackageConfigCommonFunc::GetPackageOTAZipPath(msOTAUpdateInfo.mstrInstallVersion,msOTAUpdateInfo.mstrUpdateOTAVersion);
                msOTAUpdateInfo.mstrUpdateOTAName = mpOTANameEdit->GetText().GetData();
                msOTAUpdateInfo.mstrUpdateOTADes = mpOTADesEdit->GetText().GetData();


                PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_OTA_UPDATE_OTA_ACTION_ID,NULL);
            }
            else if (msg.pSender == mpAddOTAFolderBtn)
            {
                std::wstring strFolder;
                if (JGW_ChooseFolder(strFolder,mpPaintManagerUI->GetPaintWindow()))
                {
                    mpAddOTAFolderEdt->SetText(strFolder.c_str());
                }
            }
            else if (msg.pSender == mpUploadAddOTABtn)
            {
                mpAddOTAInstallVersionCombo->SetEnabled(false);
                mpAddOTAFolderBtn->SetEnabled(false);
                mpUploadAddOTABtn->SetEnabled(false);
                msOTAAddInfo.mstrLocalOTAFolder = mpAddOTAFolderEdt->GetText().GetData();
                msOTAAddInfo.mstrOTADes = mpAddOTADesEdit->GetText().GetData();
                msOTAAddInfo.mstrOTAName = mpAddOTANameEdit->GetText().GetData();
                msOTAAddInfo.mstrOTAVersion = CCJGW_PackageConfigCommonFunc::GetOTAVersion();
                msOTAAddInfo.mstrLocalOTAPackageZipPath = CCJGW_PackageConfigCommonFunc::GetPackageOTAZipPath(msOTAAddInfo.mstrInstallVersion.c_str(),msOTAAddInfo.mstrOTAVersion.c_str());
                msOTAAddInfo.mstrUploadFTPUrl = CCJGW_PackageConfigCommonFunc::GetFtpOTAPackagePath(
                    mpsPackageUpdateConfig->mstrFTPRootPath
                    ,msOTAAddInfo.mstrInstallVersion
                    ,msOTAAddInfo.mstrOTAVersion);
                mpAddOTAVersionEdit->SetText(msOTAAddInfo.mstrOTAVersion.c_str());
                PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_OTA_ADD_OTA_ACTION_ID,NULL);
            }
        }
    }

    void CCJGW_HelpOTADlg::CloseHelpPackageUI()
    {

    }

    void CCJGW_HelpOTADlg::OnHandleEventCustomMessage(TEventUI* pTEventUI)
    {
        if (pTEventUI->Type == WM_HWMD_OPERATION_COMPLETE_MSG)
        {
            OnRspOperationCompleteMsg((E_OTA_ASYNC_ACTION_ID)pTEventUI->wParam,pTEventUI->lParam == 1);
        }
    }

    void CCJGW_HelpOTADlg::OnRspOperationCompleteMsg(E_OTA_ASYNC_ACTION_ID eActionIndex,bool result)
    {
        if (eActionIndex == E_OTA_UPDATE_SELECT_INSTALLVERSION_ACTION_ID)
        {
            if (result)
            {
                while (mpOTAOTAVersionCombo->GetCount() > 1) mpOTAOTAVersionCombo->RemoveAt(1);
                for (size_t index = 0;index < msOTAUpdateInfo.mvsOTA.size();index++)
                {
                    CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                    std::wstring strTemp = msOTAUpdateInfo.mvsOTA[index].mstrVersion;
                    pListLabelEle->SetText(strTemp.c_str());
                    pListLabelEle->SetDate((LPVOID)&msOTAUpdateInfo.mvsOTA[index]);
                    mpOTAOTAVersionCombo->Add(pListLabelEle);
                }
                mpOTAOTAVersionCombo->SetEnabled(true);
            }
        }
        else if (eActionIndex == E_OTA_UPDATE_DOWNLOAD_OTA_ACTION_ID)
        {
            if (result)
            {
                S_OTA *psOTA = (S_OTA *)mpOTAOTAVersionCombo->GetItemAt(mpOTAOTAVersionCombo->GetCurSel())->GetDate();
                mpOTAOTAFolderEdt->SetText(msOTAUpdateInfo.mstrLocalUpdateOTAFolder.c_str());
                mpOTAVersionEdit->SetText(psOTA->mstrVersion.c_str());
                mpOTANameEdit->SetText(psOTA->mstrName.c_str());
                mpOTADesEdit->SetText(psOTA->mstrDes.c_str());;
                mpUpdateOTABtn->SetEnabled(true);
            }
        }
        else if (eActionIndex == E_OTA_UPDATE_OTA_ACTION_ID)
        {
            if (result)
            {
                MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The ota information is modified successfully.",L"Succeed",MB_OK);
            }
            else
            {
                mpOTAInstallVersionCombo->SetEnabled(true);
                mpUpdateOTABtn->SetEnabled(true);
                mpOTAOTAVersionCombo->SetEnabled(true);
                mpOTADownloadOTABtn->SetEnabled(true);
            }
        }
        else if (eActionIndex == E_OTA_ADD_OTA_ACTION_ID)
        {
            if (result)
            {
                mpAddOTAInstallVersionCombo->SetEnabled(false);
                mpAddOTAFolderBtn->SetEnabled(false);
                mpUploadAddOTABtn->SetEnabled(false);
                MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The ota information is add successfully.",L"Succeed",MB_OK);
            }
            else
            {
                mpAddOTAInstallVersionCombo->SetEnabled(true);
            }
        }
    }

    bool CCJGW_HelpOTADlg::OnHandleAsyncFuncMsg(int nActionIndex)
    {
        switch ((E_OTA_ASYNC_ACTION_ID)nActionIndex)
        {
        case E_OTA_UPDATE_SELECT_INSTALLVERSION_ACTION_ID:
            return OnRspOTAUpdateInstallVersionComboItemSelectMsg();
        case E_OTA_UPDATE_DOWNLOAD_OTA_ACTION_ID:
            return OnRspOTAUpdateDownloadOTABtnClickMsg();
        case E_OTA_UPDATE_OTA_ACTION_ID:
            return OnRspOTAUpdateOtaBtnClickMsg();
        case E_OTA_ADD_OTA_ACTION_ID:
            return OnRspOTAAddOtaBtnClickMsg();
        default:
            break;
        }
        return false;
    }

    bool CCJGW_HelpOTADlg::OnRspOTAUpdateInstallVersionComboItemSelectMsg()
    {
        CCJGW_OTASQLite otaSQLite;
        if (!otaSQLite.GetOTAInfosToInstallVersionID(mpsPackageUpdateConfig->mcSQLite,msOTAUpdateInfo.mnInstallVersionID,msOTAUpdateInfo.mvsOTA))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get OTA DB Fail",L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGW_HelpOTADlg::OnRspOTAUpdateDownloadOTABtnClickMsg()
    {
        //! DOWNLOAD FTPURL  mstrFtpUrl
        std::wstring strLocalOTAUpdateZipPath,strErrorMsg;
#ifndef DEBUG_HELP
        CCJGW_CurlFtp curlFtpDownload(mpsPackageUpdateConfig->mstrFTPHost,mpsPackageUpdateConfig->mstrUserName,mpsPackageUpdateConfig->mstrUserPassword,mpsPackageUpdateConfig->mnFTPPort);

        strLocalOTAUpdateZipPath = CCJGW_PackageConfigCommonFunc::GetPackageOTAZipPath(msOTAUpdateInfo.mstrInstallVersion,msOTAUpdateInfo.mstrVersion);
        if (!curlFtpDownload.DownloadCurlFtpFile(msOTAUpdateInfo.mstrFtpUrl,strLocalOTAUpdateZipPath,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Download ota update zip file Fail",L"Error",MB_ICONERROR);
            return false;
        }
#else
        strLocalOTAUpdateZipPath = L"D:\\Config20220608185138.zip";
#endif

        msOTAUpdateInfo.mstrLocalUpdateOTAFolder = CCJGW_PackageConfigCommonFunc::GetLocalOTAFolder(msOTAUpdateInfo.mstrInstallVersion,msOTAUpdateInfo.mstrVersion);

        if (!CCJGW_PackageConfigCommonFunc::DeCompressFile(msOTAUpdateInfo.mstrLocalUpdateOTAFolder,strLocalOTAUpdateZipPath,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGW_HelpOTADlg::OnRspOTAUpdateOtaBtnClickMsg()
    {
        //! 
        std::wstring strErrorMsg;
        if (!CCJGW_PackageConfigCommonFunc::CompressFile(msOTAUpdateInfo.mstrLocalUpdateOTAFolder
            ,msOTAUpdateInfo.mstrLocalUpdateOTAZipPath
            ,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }

        msOTAUpdateInfo.mstrUpdateFTPUrl = CCJGW_PackageConfigCommonFunc::GetFtpOTAPackagePath(
            mpsPackageUpdateConfig->mstrFTPRootPath
            ,msOTAUpdateInfo.mstrInstallVersion
            ,msOTAUpdateInfo.mstrUpdateOTAVersion);
#ifndef DEBUG_HELP
        if (!CCJGW_PackageConfigCommonFunc::UploadFTPFile(mpsPackageUpdateConfig
            ,msOTAUpdateInfo.mstrLocalUpdateOTAZipPath
            ,msOTAUpdateInfo.mstrUpdateFTPUrl
            ,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }
#endif

        CCJGW_OTASQLite otaSQLite;
        S_OTA sOTA;
        sOTA.mnID = msOTAUpdateInfo.mnOTAID;
        sOTA.mnInstallVersionID = msOTAUpdateInfo.mnInstallVersionID;
        //! sOTA.mbIsLatestVersion = true;
        sOTA.mstrDes = msOTAUpdateInfo.mstrUpdateOTADes;
        sOTA.mstrFTPUrl = msOTAUpdateInfo.mstrUpdateFTPUrl;
        sOTA.mstrMD5 = CCJGW_CryptFileMD5::GetFileMD5(msOTAUpdateInfo.mstrLocalUpdateOTAZipPath);
        sOTA.mstrName = msOTAUpdateInfo.mstrUpdateOTAName;
        sOTA.mstrVersion = msOTAUpdateInfo.mstrUpdateOTAVersion;

        if (!otaSQLite.UpdateOTADBToOTAInfo(mpsPackageUpdateConfig->mcSQLite,sOTA))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Update OTA DB Fail",L"Error",MB_ICONERROR);
            return false;
        }

        return true;
    }

    bool CCJGW_HelpOTADlg::OnRspOTAAddOtaBtnClickMsg()
    {
        if (msOTAAddInfo.mstrOTAName.empty())
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The OTA name cannot be empty",L"Error",MB_ICONERROR);
            return false;
        }

        std::wstring strErrorMsg;
        if (!CCJGW_PackageConfigCommonFunc::CompressFile(msOTAAddInfo.mstrLocalOTAFolder
            ,msOTAAddInfo.mstrLocalOTAPackageZipPath
            ,strErrorMsg))
        {
            return false;
        }

#ifndef DEBUG_HELP
        if (!CCJGW_PackageConfigCommonFunc::UploadFTPFile(mpsPackageUpdateConfig
            ,msOTAAddInfo.mstrLocalOTAPackageZipPath
            ,msOTAAddInfo.mstrUploadFTPUrl
            ,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }
#endif
        CCJGW_OTASQLite otaSQLite;
        S_OTA sOTA;
        sOTA.mnInstallVersionID = msOTAAddInfo.mnInstallVersionID;
        sOTA.mbIsLatestVersion = true;
        sOTA.mstrDes = msOTAAddInfo.mstrOTADes;
        sOTA.mstrFTPUrl = msOTAAddInfo.mstrUploadFTPUrl;
        sOTA.mstrMD5 = CCJGW_CryptFileMD5::GetFileMD5(msOTAAddInfo.mstrLocalOTAPackageZipPath);
        sOTA.mstrName = msOTAAddInfo.mstrOTAName;
        sOTA.mstrVersion = msOTAAddInfo.mstrOTAVersion;

        if (!otaSQLite.UpdateOTAIsLatestVersionToOTAInfo(mpsPackageUpdateConfig->mcSQLite,sOTA.mnInstallVersionID,false))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Update OTA IsLatestVersion Fail",L"Error",MB_ICONERROR);
            return false;
        }

        if (!otaSQLite.InsertOTADBToOTAInfo(mpsPackageUpdateConfig->mcSQLite,sOTA))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Add OTA DB Fail",L"Error",MB_ICONERROR);
            return false;
        }

        return true;
    }
}
