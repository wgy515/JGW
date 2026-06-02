#include "StdAfx.h"
#include "CJGWHelpConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_ConfigSQLite.h"
#include <JGW_CurlPlugin/CJGW_CurlFtp.h>
#include <poco/Delegate.h>
#include <fstream>
#include <poco/Zip/Compress.h>
#include <poco/Zip/ZipArchive.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include "CJGW_ProjectSQLite.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_PackageConfigCommonFunc.h"

namespace JGW
{
    CCJGWHelpConfigDlg::CCJGWHelpConfigDlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig) : CHelpPackageUIInterface(psPackageUpdateConfig)
    {
        msAddConfigUiInfo.mnProjectID = -1;
        msUpdateConfigUiInfo.mnConfigID = -1;
        msUpdateConfigUiInfo.mnProjectID = -1;
        msUpdateConfigUiInfo.mnTaskID = -1;
    }


    CCJGWHelpConfigDlg::~CCJGWHelpConfigDlg(void)
    {
    }

    bool CCJGWHelpConfigDlg::InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI)
    {
        //! init ui
        {
            bool result = true;

            mpPaintManagerUI = pPaintManagerUI;
            FIND_UI(CComboBoxUI,ConfigTaskCombo,mpPaintManagerUI);
            result &= CHECK_UI(ConfigTaskCombo);
            FIND_UI(CButtonUI,DownloadConfigBtn,mpPaintManagerUI);
            result &= CHECK_UI(DownloadConfigBtn);
            FIND_UI(CEditUI,ConfigFolderLabel,mpPaintManagerUI);
            result &= CHECK_UI(ConfigFolderLabel);
            FIND_UI(CEditUI,ConfigNameEdit,mpPaintManagerUI);
            result &= CHECK_UI(ConfigNameEdit);
            FIND_UI(CEditUI,ConfigDesEdit,mpPaintManagerUI);
            result &= CHECK_UI(ConfigDesEdit);
            FIND_UI(CEditUI,ConfigVersionEdit,mpPaintManagerUI);
            result &= CHECK_UI(ConfigVersionEdit);
            FIND_UI(CButtonUI,ConfigUpdateBtn,mpPaintManagerUI);
            result &= CHECK_UI(ConfigUpdateBtn);

            FIND_UI(CComboBoxUI,AddConfigProjectCombo,mpPaintManagerUI);
            result &= CHECK_UI(AddConfigProjectCombo);
            FIND_UI(CEditUI,AddConfigFolderLabel,mpPaintManagerUI);
            result &= CHECK_UI(AddConfigFolderLabel);
            FIND_UI(CButtonUI,AddConfigPackageFolderBtn,mpPaintManagerUI);
            result &= CHECK_UI(AddConfigPackageFolderBtn);
            FIND_UI(CEditUI,AddConfigVersionEdit,mpPaintManagerUI);
            result &= CHECK_UI(AddConfigVersionEdit);
            FIND_UI(CEditUI,AddConfigNameEdit,mpPaintManagerUI);
            result &= CHECK_UI(AddConfigNameEdit);
            FIND_UI(CEditUI,AddConfigDesEdit,mpPaintManagerUI);
            result &= CHECK_UI(AddConfigDesEdit);
            FIND_UI(CButtonUI,UploadAddConfigPackageBtn,mpPaintManagerUI);
            result &= CHECK_UI(UploadAddConfigPackageBtn);
            if (!result)
            {
                return false;
            }
            mpConfigUpdateBtn->SetEnabled(false);
            mpDownloadConfigBtn->SetEnabled(false);
            mpAddConfigPackageFolderBtn->SetEnabled(false);
            mpUploadAddConfigPackageBtn->SetEnabled(false);
        }
        //! init task project
        {
            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsTask.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = JGW_A2W(mpsPackageUpdateConfig->mvsTask[index].mstrName,CP_UTF8);
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate(&mpsPackageUpdateConfig->mvsTask[index]);
                mpConfigTaskCombo->Add(pListLabelEle);
            }

            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsProject.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                pListLabelEle->SetText(mpsPackageUpdateConfig->mvsProject[index].mstrName.c_str());
                pListLabelEle->SetDate(&mpsPackageUpdateConfig->mvsProject[index]);
                mpAddConfigProjectCombo->Add(pListLabelEle);
            }
        }

        return true;
    }

    bool CCJGWHelpConfigDlg::OnHandleAsyncFuncMsg(int nActionIndex)
    {
        switch ((E_CONFIG_ASYNC_ACTION_ID)nActionIndex)
        {
        case E_SELECT_TASK_ASYNC_ACTION_ID:
            return OnRspConfigTaskComboItemSelectMsg();
        case E_DOWNLOAD_CONFIG_ASYNC_ACTION_ID:
            return OnRspDownloadConfigClickMsg();
        case E_UPDATE_CONFIG_ASYNC_ACTION_ID:
            return OnRspUpdateConfigClickMsg();
        case E_ADD_CONFIG_ASYNC_ACTION_ID:
            return OnRspUploadAddConfigPackageClickMsg();
        }
        return false;
    }

    void CCJGWHelpConfigDlg::OnNotifyHelpPackageUI(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (msg.pSender == mpConfigTaskCombo)
            {
                mpConfigUpdateBtn->SetEnabled(false);
                mpDownloadConfigBtn->SetEnabled(false);
                mpConfigDesEdit->SetText(L"");
                mpConfigNameEdit->SetText(L"");
                mpConfigFolderLabel->SetText(L"");
                mpConfigVersionEdit->SetText(L"");

                if (mpConfigTaskCombo->GetCurSel() > 0)
                {
                    S_TASK *psTask = (S_TASK *)mpConfigTaskCombo->GetItemAt(mpConfigTaskCombo->GetCurSel())->GetDate();
                    msUpdateConfigUiInfo.mnTaskID = psTask->mnID;
                    msUpdateConfigUiInfo.mstrTaskName = mpConfigTaskCombo->GetItemAt(mpConfigTaskCombo->GetCurSel())->GetText().GetData();
                    msUpdateConfigUiInfo.mnConfigID = psTask->mnConfigID;
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_SELECT_TASK_ASYNC_ACTION_ID,NULL);
                }
            }
            else if (msg.pSender == mpAddConfigProjectCombo)
            {
                mpAddConfigPackageFolderBtn->SetEnabled(false);
                mpUploadAddConfigPackageBtn->SetEnabled(false);
                msAddConfigUiInfo.mnProjectID = -1;
                if (mpAddConfigProjectCombo->GetCurSel() > 0)
                {
                    S_PROJECT *psProject = (S_PROJECT *)mpAddConfigProjectCombo->GetItemAt(mpAddConfigProjectCombo->GetCurSel())->GetDate();
                    msAddConfigUiInfo.mnProjectID = psProject->mnID;
                    msAddConfigUiInfo.mstrProjectName = psProject->mstrName;
                    mpAddConfigPackageFolderBtn->SetEnabled(true);
                    mpUploadAddConfigPackageBtn->SetEnabled(true);
                }
            }
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpDownloadConfigBtn == msg.pSender)
            {
                mpConfigUpdateBtn->SetEnabled(false);
                mpDownloadConfigBtn->SetEnabled(false);
                PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_DOWNLOAD_CONFIG_ASYNC_ACTION_ID,NULL);
            }
            else if (mpConfigUpdateBtn == msg.pSender)
            {
                mpConfigUpdateBtn->SetEnabled(false);
                mpDownloadConfigBtn->SetEnabled(false);
                mpConfigTaskCombo->SetEnabled(false);

                msUpdateConfigUiInfo.mstrConfigFolder = mpConfigFolderLabel->GetText().GetData();
                //! msUpdateConfigUiInfo.mstrVersion = mpConfigVersionEdit->GetText().GetData();
                msUpdateConfigUiInfo.mstrConfigName = mpConfigNameEdit->GetText().GetData();
                msUpdateConfigUiInfo.mstrConfigDes = mpConfigDesEdit->GetText().GetData();
                msUpdateConfigUiInfo.mstrLocalUpdateConfigPackageZipPath = CCJGW_PackageConfigCommonFunc::GetPackageConfigZipPath(msUpdateConfigUiInfo.mstrTaskName,msUpdateConfigUiInfo.mstrUpdateConfigVersion);
                PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_UPDATE_CONFIG_ASYNC_ACTION_ID,NULL);
            }
            else if (mpAddConfigPackageFolderBtn == msg.pSender)
            {
                //! 选择目录
                OnRspSelectConfigPackageFolderClickMsg();
            }
            else if (mpUploadAddConfigPackageBtn == msg.pSender)
            {
                mpUploadAddConfigPackageBtn->SetEnabled(false);
                mpAddConfigPackageFolderBtn->SetEnabled(false);
                mpAddConfigProjectCombo->SetEnabled(false);
                msAddConfigUiInfo.mstrConfigFolder = mpAddConfigFolderLabel->GetText().GetData();
                //msAddConfigUiInfo.mstrVersion = mpAddConfigVersionEdit->GetText().GetData();
                msAddConfigUiInfo.mstrConfigName = mpAddConfigNameEdit->GetText().GetData();
                msAddConfigUiInfo.mstrConfigDes = mpAddConfigDesEdit->GetText().GetData();
                msAddConfigUiInfo.mstrVersion = CCJGW_PackageConfigCommonFunc::GetConfigVersion();
                mpAddConfigVersionEdit->SetText(msAddConfigUiInfo.mstrVersion.c_str());
                msAddConfigUiInfo.mstrLocalConfigPackageZipPath = CCJGW_PackageConfigCommonFunc::GetPackageConfigZipPath(msAddConfigUiInfo.mstrProjectName,msAddConfigUiInfo.mstrVersion);
                PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_ADD_CONFIG_ASYNC_ACTION_ID,NULL);
            }
        }
    }

    bool CCJGWHelpConfigDlg::OnRspConfigTaskComboItemSelectMsg()
    {
        //! F:\project\git_code_debug\FTM-tools\JaGuarWaveTestPlatform\bin\debug\exec_debug\PackageConfig\DHW206996C\Config20220608185138
        msUpdateConfigUiInfo.mstrUpdateConfigVersion = CCJGW_PackageConfigCommonFunc::GetConfigVersion();
        msUpdateConfigUiInfo.mstrConfigFolder = CCJGW_PackageConfigCommonFunc::GetLocalConfigFolder(msUpdateConfigUiInfo.mstrTaskName,msUpdateConfigUiInfo.mstrUpdateConfigVersion);
        JGW_DeleteDirectoryW(msUpdateConfigUiInfo.mstrConfigFolder.c_str());

        //! 获取订单对应的配置信息
        CCJGW_ConfigSQLite configSQLite;
        if (!configSQLite.GetConfigDBToConfigID(mpsPackageUpdateConfig->mcSQLite,msUpdateConfigUiInfo.mnConfigID,msConfig))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Config DB Fail",L"Error",MB_ICONERROR);
            return false;
        }
        //! 获取CONFIG对应的项目信息
        S_PROJECT sProject;
        CCJGW_ProjectSQLite projectSQLite;
        if (!projectSQLite.GetProjectDBToProjectID(mpsPackageUpdateConfig->mcSQLite,msConfig.mnProjectID,sProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Project DB Fail",L"Error",MB_ICONERROR);
            return false;
        }

        msUpdateConfigUiInfo.mnProjectID = msConfig.mnProjectID;
        msUpdateConfigUiInfo.mstrProjectName = sProject.mstrName;
        return true;
    }

    void CCJGWHelpConfigDlg::CloseHelpPackageUI()
    {

    }

    void CCJGWHelpConfigDlg::OnHandleEventCustomMessage(TEventUI* pTEventUI)
    {
        if (pTEventUI->Type == WM_HWMD_OPERATION_COMPLETE_MSG)
        {
            OnRspOperationCompleteMsg((E_CONFIG_ASYNC_ACTION_ID)pTEventUI->wParam,pTEventUI->lParam == 1);
        }
    }

    void CCJGWHelpConfigDlg::OnRspOperationCompleteMsg(E_CONFIG_ASYNC_ACTION_ID eActionIndex,bool result)
    {
        if (E_SELECT_TASK_ASYNC_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                mpConfigFolderLabel->SetText(msUpdateConfigUiInfo.mstrConfigFolder.c_str());
                mpConfigVersionEdit->SetText(msConfig.mstrVersion.c_str());
                mpConfigNameEdit->SetText(msConfig.mstrName.c_str());
                mpConfigDesEdit->SetText(msConfig.mstrDes.c_str());
                mpDownloadConfigBtn->SetEnabled(true);
                mpConfigUpdateBtn->SetEnabled(false);
            }
            else
            {
                mpDownloadConfigBtn->SetEnabled(false);
                mpConfigUpdateBtn->SetEnabled(false);
            }
        }
        else if (E_DOWNLOAD_CONFIG_ASYNC_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                mpDownloadConfigBtn->SetEnabled(false);
                mpConfigUpdateBtn->SetEnabled(true);
            }
            else
            {
                mpDownloadConfigBtn->SetEnabled(true);
                mpConfigUpdateBtn->SetEnabled(false);
            }
        }
        else if (E_UPDATE_CONFIG_ASYNC_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                mpDownloadConfigBtn->SetEnabled(false);
                mpConfigUpdateBtn->SetEnabled(false);
                mpConfigVersionEdit->SetText(msUpdateConfigUiInfo.mstrUpdateConfigVersion.c_str());
                MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The configuration information is modified successfully.",L"Succeed",MB_OK);
            }
            else
            {
                mpDownloadConfigBtn->SetEnabled(true);
                mpConfigUpdateBtn->SetEnabled(false);
            }
        }
        else if (E_ADD_CONFIG_ASYNC_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                mpUploadAddConfigPackageBtn->SetEnabled(false);
                mpAddConfigPackageFolderBtn->SetEnabled(false);
                mpAddConfigProjectCombo->SetEnabled(false);
                MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The new configuration information has been uploaded successfully.",L"Succeed",MB_OK);
            }
            else
            {
                mpAddConfigProjectCombo->SetEnabled(true);
                mpAddConfigPackageFolderBtn->SetEnabled(true);
                mpUploadAddConfigPackageBtn->SetEnabled(true);
            }
        }
    }


    bool CCJGWHelpConfigDlg::OnRspDownloadConfigClickMsg()
    {
        /// download help db
        std::wstring strLocalConfigUpdateZipPath,strErrorMsg;
#ifndef DEBUG_HELP
        strLocalConfigUpdateZipPath = CCJGW_PackageConfigCommonFunc::GetPackageConfigZipPath(msUpdateConfigUiInfo.mstrTaskName,msConfig.mstrVersion);
        CCJGW_CurlFtp curlFtpDownload(mpsPackageUpdateConfig->mstrFTPHost,mpsPackageUpdateConfig->mstrUserName,mpsPackageUpdateConfig->mstrUserPassword,mpsPackageUpdateConfig->mnFTPPort);

        if (!curlFtpDownload.DownloadCurlFtpFile(msConfig.mstrFTPUrl,strLocalConfigUpdateZipPath,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Download Config update zip file Fail",L"Error",MB_ICONERROR);
            return false;
        }
#else
        strLocalConfigUpdateZipPath = L"D:\\Config20220608185138.zip";
#endif
        if (!CCJGW_PackageConfigCommonFunc::DeCompressFile(msUpdateConfigUiInfo.mstrConfigFolder,strLocalConfigUpdateZipPath,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGWHelpConfigDlg::OnRspUpdateConfigClickMsg()
    {
        std::wstring strErrorMsg;
        if (!CCJGW_PackageConfigCommonFunc::CompressFile(msUpdateConfigUiInfo.mstrConfigFolder
            ,msUpdateConfigUiInfo.mstrLocalUpdateConfigPackageZipPath
            ,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }

        msUpdateConfigUiInfo.mstrUpdateFtpUrl = CCJGW_PackageConfigCommonFunc::GetFtpConfigPackagePath(
            mpsPackageUpdateConfig->mstrFTPRootPath
            ,msUpdateConfigUiInfo.mstrProjectName
            ,msUpdateConfigUiInfo.mstrUpdateConfigVersion);
#ifndef DEBUG_HELP
        if (!UploadFTPPackageConfig(msUpdateConfigUiInfo.mstrUpdateFtpUrl,msUpdateConfigUiInfo.mstrLocalUpdateConfigPackageZipPath))
        {
            return false;
        }
#endif
        if (!UpdatePackageConfigSQLite())
        {
            return false;
        }
        return true;
    }

    bool CCJGWHelpConfigDlg::UploadFTPPackageConfig(const std::wstring& strUploadFtpUrl,const std::wstring& strLocalFilePath)
    {
        //! JGWPTP_Help\Project\项目信息\订单\配置版本.zip
        std::wstring strErrorMsg;

        if (!CCJGW_PackageConfigCommonFunc::UploadFTPFile(mpsPackageUpdateConfig
            ,strLocalFilePath
            ,strUploadFtpUrl
            ,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGWHelpConfigDlg::UpdatePackageConfigSQLite()
    {
        S_CONFIG sConfig;
        sConfig.mnID = msUpdateConfigUiInfo.mnConfigID;
        sConfig.mnProjectID = msUpdateConfigUiInfo.mnProjectID;
        sConfig.mstrName = msUpdateConfigUiInfo.mstrConfigName;
        sConfig.mstrDes = msUpdateConfigUiInfo.mstrConfigDes;
        sConfig.mstrVersion = msUpdateConfigUiInfo.mstrUpdateConfigVersion;
        sConfig.mstrFTPUrl = msUpdateConfigUiInfo.mstrUpdateFtpUrl;
        sConfig.mstrMD5 = CCJGW_CryptFileMD5::GetFileMD5(msUpdateConfigUiInfo.mstrLocalUpdateConfigPackageZipPath);

        CCJGW_ConfigSQLite configSQLite;
        if (!configSQLite.UpdateConfigDBToConfigID(mpsPackageUpdateConfig->mcSQLite,msConfig.mnID,sConfig))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Update Config DB Fail",L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGWHelpConfigDlg::OnRspUploadAddConfigPackageClickMsg()
    {
        if (msAddConfigUiInfo.mnProjectID < 1)
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Please select project information",L"Error",MB_ICONERROR);
            return false;
        }
        if (msAddConfigUiInfo.mstrConfigName.empty())
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Enter the configuration name information",L"Error",MB_ICONERROR);
            return false;
        }

        if (msAddConfigUiInfo.mstrConfigFolder.empty())
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Select the configuration project path",L"Error",MB_ICONERROR);
            return false;
        }

        std::wstring strErrorMsg;
        if (!CCJGW_PackageConfigCommonFunc::CompressFile(msAddConfigUiInfo.mstrConfigFolder
            ,msAddConfigUiInfo.mstrLocalConfigPackageZipPath
            ,strErrorMsg))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
            return false;
        }

        msAddConfigUiInfo.mstrUploadFtpUrl = CCJGW_PackageConfigCommonFunc::GetFtpConfigPackagePath(
            mpsPackageUpdateConfig->mstrFTPRootPath
            ,msAddConfigUiInfo.mstrProjectName
            ,msAddConfigUiInfo.mstrVersion);
#ifndef DEBUG_HELP
        if (!UploadFTPPackageConfig(msAddConfigUiInfo.mstrUploadFtpUrl,msAddConfigUiInfo.mstrLocalConfigPackageZipPath))
        {
            return false;
        }
#endif
        if (!AddPackageConfigToSQLite())
        {
            return false;
        }

        return true;
    }

    bool CCJGWHelpConfigDlg::AddPackageConfigToSQLite()
    {
        S_CONFIG sConfig;
        sConfig.mnProjectID = msAddConfigUiInfo.mnProjectID;
        sConfig.mstrName = msAddConfigUiInfo.mstrConfigName;
        sConfig.mstrDes = msAddConfigUiInfo.mstrConfigDes;
        sConfig.mstrVersion = msAddConfigUiInfo.mstrVersion;
        sConfig.mstrFTPUrl = msAddConfigUiInfo.mstrUploadFtpUrl;
        sConfig.mstrMD5 = CCJGW_CryptFileMD5::GetFileMD5(msAddConfigUiInfo.mstrLocalConfigPackageZipPath);

        CCJGW_ConfigSQLite configSQLite;
        if (!configSQLite.AddConfigDBToConfigInfo(mpsPackageUpdateConfig->mcSQLite,sConfig))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Add Config DB Fail",L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGWHelpConfigDlg::OnRspSelectConfigPackageFolderClickMsg()
    {
        std::wstring strFolder;
        if (JGW_ChooseFolder(strFolder,mpPaintManagerUI->GetPaintWindow()))
        {
            mpAddConfigFolderLabel->SetText(strFolder.c_str());
        }
        return true;
    }
}
