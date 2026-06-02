#include "StdAfx.h"
#include "CTSEMultipleTestPluginDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_ReportDeviceInfo/CJGW_ReportDeviceInfoExport.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <regex>
#include "CTSEMultipleTestThread.h"
#include "CTSEMultipleTestConfigDlg.h"
namespace JGW
{
    CCTSEMultipleTestPluginDlg::CCTSEMultipleTestPluginDlg(void) : mbInitMultipleTestOk(false),mcTSEMultipleTestManageThread(mvsMultipleTestThreadParam)
    {
        msPluginSkin.mstrSkinXmlPath = L"ipq_multi_upgrade_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        msMultipleConfig.mnMultipleTestCol = 4;
        msMultipleConfig.mnMultipleTestRow = 2;
        msMultipleConfig.mnDownloadTimeSec = 200;
        msMultipleConfig.mnDownloadTimeOutSec = 10 * 60;
    }


    CCTSEMultipleTestPluginDlg::~CCTSEMultipleTestPluginDlg(void)
    {
        mcTSEMultipleTestManageThread.TerminateMessageThread();
        for (size_t i = 0;i < mvsMultipleTestThreadParam.size();i ++)
        {
            CCTSEMultipleTestThread* pTSEMultipleTestThread = reinterpret_cast<CCTSEMultipleTestThread*>(mvsMultipleTestThreadParam[i].mpMultipleTestThread);
            if (NULL != mvsMultipleTestThreadParam[i].mpMultipleTestThread && pTSEMultipleTestThread)
            {
                delete pTSEMultipleTestThread;
            }
        }
        mvsMultipleTestThreadParam.clear();
    }

    bool CCTSEMultipleTestPluginDlg::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        //! Init UI
        {
            mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            mpMainMultipleVerticalLayout = static_cast<CVerticalLayoutUI*>(mpPluginPaintManagerUI->FindControl(L"MainMultipleVerticalLayout"));
            mpChooseBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"chooseBtn"));
            mpConfigLanBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"configBtn"));
            mpBinaryImagePathLabel = static_cast<CLabelUI*>(mpPluginPaintManagerUI->FindControl(L"downPathLabel"));

            if (NULL == mpMainMultipleVerticalLayout || NULL == mpChooseBtn || NULL == mpConfigLanBtn || NULL == mpBinaryImagePathLabel || !LoadTSEMultipleTestConfig())
            {
                return false;
            }
        }

        {
            //! 初始化多路测试视图
            InitMultipleView();  
            //! 
            StartNetCardDeviceThread(500);

            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_TIMER,&CCTSEMultipleTestPluginDlg::OnResponseTimer);
            SetTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }

        mcTSEMultipleTestManageThread.CreateMessageThread();
        //! 检查初始化多路测试是否OK
        CheckInitMultipleTestIsOk();

        return true;
    }

    void CCTSEMultipleTestPluginDlg::CheckInitMultipleTestIsOk()
    {
        mbInitMultipleTestOk = false;
        RemoveNetCardDeviceMsg(mpPluginPaintManagerUI->GetPaintWindow());
        mpBinaryImagePathLabel->SetText(L"");
        mcTSEMultipleTestManageThread.StopTSEMultipleTestManageThread();
        //StartNetCardDeviceThread(500);
        //! 如果是下载升级线程则检测下载路径等方面配置
        if (msMultipleConfig.mbIsDownloadUpgrade)
        {
            if (!JGW_PathFileExists(msMultipleConfig.mstrBinaryImagePath.c_str()))
            {
                MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),msMultipleConfig.mstrBinaryImagePath.c_str(),L"The file or directory does not exist",MB_ICONERROR);
                goto INIT_ERROR;
            }

            if (!JGW_PathFileExists(msMultipleConfig.mstrTSESuiteConfigFilePath.c_str()))
            {
                MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),msMultipleConfig.mstrTSESuiteConfigFilePath.c_str(),L"The file or directory does not exist",MB_ICONERROR);
                goto INIT_ERROR;
            }


            if (msMultipleConfig.mbBinaryImageIsFolder && !ParseDownloadFolderConfig())
            {
                goto INIT_ERROR;
            }
            mpBinaryImagePathLabel->SetText(msMultipleConfig.mstrBinaryImagePath.c_str());
        }
        else
        {
            mpBinaryImagePathLabel->SetText(msMultipleConfig.mstrTSESuiteConfigFilePath.c_str());
            if (!JGW_PathFileExists(msMultipleConfig.mstrTSESuiteConfigFilePath.c_str()))
            {
                MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),msMultipleConfig.mstrTSESuiteConfigFilePath.c_str(),L"The file or directory does not exist",MB_ICONERROR);
                goto INIT_ERROR;
            }
        }
        mcTSEMultipleTestManageThread.StartTSEMultipleTestManageThread(&msMultipleConfig);
        mbInitMultipleTestOk = true; 
        RegNetCardDeviceMsg(mpPluginPaintManagerUI->GetPaintWindow(),msMultipleConfig.msNetCardConfig.mstrHostIPSeg.c_str());
        ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_HWND_NETCARD_CHANGE_MSG,&CCTSEMultipleTestPluginDlg::OnResponseNetCardChangeMSG);
        return ;
INIT_ERROR:
        mpBinaryImagePathLabel->SetText(L"");
        return ;
    }

    bool CCTSEMultipleTestPluginDlg::LoadTSEMultipleTestConfig()
    {
        std::wstring strTSEMultipleTestConfigPath = JGW_GetCacheConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
        mcTSEMultipleTestConfig.InitIniFilePath(strTSEMultipleTestConfigPath.c_str());
        msMultipleConfig.mbIsUsbDevice = mcTSEMultipleTestConfig.GetIniKeyBoolValue(L"MAIN",L"IsUsbDevice",0);

        msMultipleConfig.mbIsDownloadUpgrade = mcTSEMultipleTestConfig.GetIniKeyBoolValue(L"MAIN",L"IsDownloadUpgrade",1);
        msMultipleConfig.mbBinaryImageIsFolder = mcTSEMultipleTestConfig.GetIniKeyBoolValue(L"MAIN",L"BinaryImageIsFolder",0);
        msMultipleConfig.mbSaveTestLog = mcTSEMultipleTestConfig.GetIniKeyBoolValue(L"MAIN",L"SaveTestLog",1);
        msMultipleConfig.mbDownloadUpgradeTSEParamIsFileName = mcTSEMultipleTestConfig.GetIniKeyBoolValue(L"MAIN",L"DownloadUpgradeTSEParamIsFileName",1);
        msMultipleConfig.mnMultipleTestRow = mcTSEMultipleTestConfig.GetIniKeyIntValue(L"MAIN",L"MultipleTestRow",1);
        msMultipleConfig.mnMultipleTestCol = mcTSEMultipleTestConfig.GetIniKeyIntValue(L"MAIN",L"MultipleTestCol",1);
        msMultipleConfig.mnDownloadTimeSec = mcTSEMultipleTestConfig.GetIniKeyIntValue(L"MAIN",L"DownloadTimeSec",200);
        msMultipleConfig.mnDownloadTimeOutSec = mcTSEMultipleTestConfig.GetIniKeyIntValue(L"MAIN",L"DownloadTimeOutSec",600);
        msMultipleConfig.mstrSaveTestLogFolder = mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"SaveTestLogFolder");
        if (msMultipleConfig.mstrSaveTestLogFolder.empty())
        {
            msMultipleConfig.mstrSaveTestLogFolder = JGW_GetFormatWString(L"%sTestLog\\%s\\%s\\",JGW_GetApplicationFolder(),mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str(), mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName.c_str());
        }
        std::wstring strBasePath = JGW_GetCacheConfigFolder(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName);
        msMultipleConfig.mstrTSESuiteConfigFilePath = JGW_RealPathToAbsPathBasePath(mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"TSESuiteConfigFilePath").c_str(),strBasePath.c_str());
        msMultipleConfig.mstrBinaryImagePath = mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"BinaryImagePath");
        msMultipleConfig.mstrUpgradeVersion = mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"UpgradeVersion");
        msMultipleConfig.mstrTSEExecApplicationFilePath = JGW_RealativePathToAbsPath(mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"TSEExecApplicationFilePath").c_str());
        msMultipleConfig.mstrTSEExecApplicationCurrentWorkDir = JGW_GetFileFolderToFilePath(msMultipleConfig.mstrTSEExecApplicationFilePath.c_str());
        msMultipleConfig.msNetCardConfig.mstrHostIPAddr = mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"HostIPAddr",L"192.168.1.1");
        msMultipleConfig.msNetCardConfig.mstrHostIPAddrAnsi = JGW_W2A(msMultipleConfig.msNetCardConfig.mstrHostIPAddr);
        msMultipleConfig.msNetCardConfig.mstrHostIPSeg = msMultipleConfig.msNetCardConfig.mstrHostIPAddr.substr(0,msMultipleConfig.msNetCardConfig.mstrHostIPAddr.find_last_of('.') + 1);
        msMultipleConfig.mstrSourceTSEParam = mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"SourceTSEParam");

        std::wstring strSortRecordConfigPath = JGW_GetConfigConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName + L"sort.config");
        mcSortRecordConfig.LoadSortRecordConfig(strSortRecordConfigPath);
        return true;
    }

    void ScanDownloadFile(const wchar_t* filepath,const wchar_t* filename,void* lpData)
    {
        std::vector<std::wstring>* vDownloadFileList = (std::vector<std::wstring>*)lpData;
        std::wstring strTmep (filepath);
        strTmep += L"\\";
        strTmep += filename;
        vDownloadFileList->push_back(strTmep);
    }


    bool CCTSEMultipleTestPluginDlg::ParseDownloadFolderConfig()
    {
        msMultipleConfig.mstrDownloadUpgradeFileTSEParam = L"";
        std::wstring strTseParam = mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",L"TSE_Param",L"");
        if (strTseParam.empty()) return true;
        std::vector<std::wstring> vTSEParam,vDownloadFileList;
        JGW_ParserStrW(strTseParam.c_str(),L",",vTSEParam);

        int numFiles = 0;
        std::wstring strFolder(msMultipleConfig.mstrBinaryImagePath);
        JGW_EraseLastAndFristTwoCharsW(strFolder,L'\\',L'/');
        JGW_TraverseFile_VC(strFolder,numFiles,L"\\*.*",ScanDownloadFile,&vDownloadFileList);

        for (size_t i = 0;i < vTSEParam.size();i ++)
        {
            std::wstring strRegex = mcTSEMultipleTestConfig.GetIniKeyValue(L"MAIN",vTSEParam[i].c_str()),strFileName;
            std::wregex partten(strRegex,std::wregex::icase);
            bool result = false;
            for (size_t x = 0;x < vDownloadFileList.size();x ++)
            {
                strFileName = JGW_GetFileNameToFilePath(vDownloadFileList[x].c_str());
                if (regex_match(strFileName,partten))
                {
                    if (msMultipleConfig.mbDownloadUpgradeTSEParamIsFileName)
                    {
                        msMultipleConfig.mstrDownloadUpgradeFileTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",vTSEParam[i].c_str(),strFileName.c_str());
                        JGW_FormatWString(strRegex,L"%s\\%s",strFolder.c_str(),strFileName.c_str());
                        if (!JGW_PathFileExists(strRegex.c_str()))
                        {
                            MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),strRegex.c_str(),L"File does not exist",MB_ICONERROR);
                            return false;
                        }
                    }
                    else
                    {
                        msMultipleConfig.mstrDownloadUpgradeFileTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",vTSEParam[i].c_str(),vDownloadFileList[x].c_str());
                    }
                    result = true;
                    break;
                }
            }
            if (!result)
            {
                JGW_FormatWString(strFileName,L"Find %s File Fail,Regex:%s",vTSEParam[i].c_str(),strRegex.c_str());
                MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),strFileName.c_str(),L"error",MB_ICONERROR);
                return false;
            }
        }
        return true;
    }

    void CCTSEMultipleTestPluginDlg::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if (pNotifyMsg->sType == DUI_MSGTYPE_CLICK)
        {
            if (mpChooseBtn == pNotifyMsg->pSender)
            {
                OnChangeDownloadPathChooseBtn();
                CheckInitMultipleTestIsOk();
            }
            else if (mpConfigLanBtn == pNotifyMsg->pSender)
            {
                OnClickConfigBtn();
            }
        }
    }

    void CCTSEMultipleTestPluginDlg::OnClickConfigBtn()
    {
        size_t row = msMultipleConfig.mnMultipleTestRow,col = msMultipleConfig.mnMultipleTestCol;
        bool bBinaryImageIsFolder = msMultipleConfig.mbBinaryImageIsFolder;
        msMultipleConfig.mbClearSortIndex = false;
        CCTSEMultipleTestConfigDlg multipleTestConfigDlg(mpPluginPaintManagerUI->GetPaintWindow(),&msMultipleConfig);
        if (IDOK == multipleTestConfigDlg.DoModal()) 
        {
            if (msMultipleConfig.mbClearSortIndex)
            {
                ClearNetCardSortConfig();
                mcSortRecordConfig.ClearSortRecordConfig();
                ClearTestLogUI();
            }
            SaveDownloadConfig(row,col,bBinaryImageIsFolder);
        }
    }

    void CCTSEMultipleTestPluginDlg::ClearTestLogUI()
    {
        for (size_t i = 0;i < mvMultipleTestView.size();i ++)
        {
            mvMultipleTestView[i].mpLogRickEdit->SetText(L"");
        }
    }

    void CCTSEMultipleTestPluginDlg::SaveDownloadConfig(size_t row,size_t col,bool bBinaryImageIsFolder)
    {
        if (bBinaryImageIsFolder != msMultipleConfig.mbBinaryImageIsFolder)
        {
            mbInitMultipleTestOk = false;
            mpBinaryImagePathLabel->SetText(L"");
        }

        mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"IsDownloadUpgrade",msMultipleConfig.mbIsDownloadUpgrade ? 1 : 0);
        mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"IsUsbDevice",msMultipleConfig.mbIsUsbDevice ? 1 : 0);
        mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"BinaryImageIsFolder",msMultipleConfig.mbBinaryImageIsFolder ? 1 : 0);
        mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"SaveTestLog",msMultipleConfig.mbSaveTestLog ? 1 : 0);
        mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"DownloadTimeSec",msMultipleConfig.mnDownloadTimeSec);
        mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"DownloadTimeOutSec",msMultipleConfig.mnDownloadTimeOutSec);

        if (msMultipleConfig.mnMultipleTestRow != row || msMultipleConfig.mnMultipleTestCol != col)
        {
            InitMultipleView();
            mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"MultipleTestRow",msMultipleConfig.mnMultipleTestRow);
            mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"MultipleTestCol",msMultipleConfig.mnMultipleTestCol);
        }
        //mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"SaveTestLogFolder",msMultipleConfig.mnDownloadTimeOutSec);
    }

    void CCTSEMultipleTestPluginDlg::OnChangeDownloadPathChooseBtn()
    {
        if (!msMultipleConfig.mbBinaryImageIsFolder)
        {
            std::wstring strProgEmmcFirehoseLitembnFilePath,strFile;
            if(!JGW_ChooseFile(strProgEmmcFirehoseLitembnFilePath,L"update*.bin;*.img\0update*.bin;*.img\0*.bin\0*.bin\0all file(*.*)\0*.*\0\0",mpPluginPaintManagerUI->GetPaintWindow()) )  return ;
            mpBinaryImagePathLabel->SetText(strProgEmmcFirehoseLitembnFilePath.c_str());
            JGW_ReplaceStringW(strProgEmmcFirehoseLitembnFilePath,JGW_GetApplicationFolder(),L"");
            mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"BinaryImagePath",strProgEmmcFirehoseLitembnFilePath.c_str());
            msMultipleConfig.mstrBinaryImagePath = strProgEmmcFirehoseLitembnFilePath;
        }
        else
        {
            std::wstring strFolder;
            if (!JGW_ChooseFolder(strFolder,mpPluginPaintManagerUI->GetPaintWindow())) return;
            mpBinaryImagePathLabel->SetText(strFolder.c_str());
            mcTSEMultipleTestConfig.SetIniKeyValue(L"MAIN",L"BinaryImagePath",strFolder.c_str());
            msMultipleConfig.mstrBinaryImagePath = strFolder;
        }
    }

    void CCTSEMultipleTestPluginDlg::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        ClearStartMsgNotifyClick();
        RemoveNetCardDeviceMsg(mpPluginPaintManagerUI->GetPaintWindow());
        CloseNetCardDeviceThread();
        mcTSEMultipleTestManageThread.StopTSEMultipleTestManageThread();
        mcTSEMultipleTestManageThread.ExitThread();
        for (size_t i = 0;i < mvsMultipleTestThreadParam.size();i ++)
        {
            CCTSEMultipleTestThread* pTSEMultipleTestThread = reinterpret_cast<CCTSEMultipleTestThread*>(mvsMultipleTestThreadParam[i].mpMultipleTestThread);
            if (NULL != mvsMultipleTestThreadParam[i].mpMultipleTestThread && pTSEMultipleTestThread)
            {
                pTSEMultipleTestThread->ExitThread();
            }
        }
        //! REMOVE_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,mpPluginPaintManagerUI->GetPaintWindow());
        //! KillTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    void CCTSEMultipleTestPluginDlg::OnResponseNetCardChangeMSG(const WPARAM& wParam,const LPARAM& lParam)
    {
        PS_NETCARD_INFO psNetCardInfo = reinterpret_cast<PS_NETCARD_INFO>(wParam);
        if (NULL == psNetCardInfo) return ;
        int nSortIndex = mcSortRecordConfig.GetSortRecordIndexToSortIndex(psNetCardInfo->mnSortIndex);
        if (nSortIndex < (int)mvsMultipleTestThreadParam.size() && nSortIndex > 0)
        {
            mvsMultipleTestThreadParam[nSortIndex - 1].mpsBindNetCardInfo = psNetCardInfo;
            //! 当前程序正在运行得时候，取消网卡状态消息上报
            if (E_TSEMP_BUSY == mvsMultipleTestThreadParam[nSortIndex - 1].meTSEMPTestStatus)
            {
                return ;
            }

            if (E_NETCARD_PHYSICAL_DISCONNECT == psNetCardInfo->meNetCardPhysicalState)
            {
                mvsMultipleTestThreadParam[nSortIndex - 1].mstrTSEMultipleTestLog.AppendText(JGW_GetFormatWString(L"%s NetCard Device Physical DisConnect\n",psNetCardInfo->mstrAdapterName.c_str()));
                mvsMultipleTestThreadParam[nSortIndex - 1].meTSEMPTestStatus = E_TSEMP_IDLE;
                //mvsMultipleTestThreadParam[nSortIndex - 1].mbNetCardStatusUpdateFlag = false;
            }
            else
            {
                mvsMultipleTestThreadParam[nSortIndex - 1].mstrTSEMultipleTestLog.AppendText(JGW_GetFormatWString(L"%s NetCard Device Physical Connect\n",psNetCardInfo->mstrAdapterName.c_str()));
                mvsMultipleTestThreadParam[nSortIndex - 1].mstrTSEMultipleTestLog.AppendText(L"");
                mvsMultipleTestThreadParam[nSortIndex - 1].meTSEMPTestStatus = E_TSEMP_INST;
                //mvsMultipleTestThreadParam[nSortIndex - 1].mbNetCardStatusUpdateFlag = true;
            }
        }
    }

    void CCTSEMultipleTestPluginDlg::ClearStartMsgNotifyClick()
    {
#if 0
        for (int i = 0;i < mpMainMultipleVerticalLayout->GetCount();i ++)
        {
            CHorizontalLayoutUI* pHorizontalLayout = (CHorizontalLayoutUI*)mpMainMultipleVerticalLayout->GetItemAt(i);
            for (int y = 0;y < pHorizontalLayout->GetCount();y ++)
            {
                ((CHorizontalLayoutUI*)pHorizontalLayout->GetItemAt(0))->GetItemAt(4)->OnNotify -= MakeDelegate(this,&CCTSEMultipleTestPluginDlg::OnStartMsgBtnClick,_T("click"));
            }
        }
#else
        /*for (size_t i = 0;i < mvMultipleTestView.size();i ++)
        {
        mvMultipleTestView[i].mpStartButton->OnNotify -= MakeDelegate(this,&CCTSEMultipleTestPluginDlg::OnStartMsgBtnClick,_T("click"));
        }*/
#endif
    }

    void CCTSEMultipleTestPluginDlg::OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == (int)wParam && mbInitMultipleTestOk)
        {
            for (size_t i = 0;i < mvMultipleTestView.size();i ++)
            {
                UpdateMultipleSingleView(mvsMultipleTestThreadParam[i],mvMultipleTestView[i]);
            }
        }
    }

    void CCTSEMultipleTestPluginDlg::UpdateMultipleSingleView(S_TSE_MULTIPLE_TEST_THREAD_PARAM& sThreadParam,S_MULTIPLE_VIEW_INFO& sMultipleViewInfo)
    {
        std::wstring strTemp;
        switch (sThreadParam.meTSEMPTestStatus)
        {
        case E_TSEMP_IDLE:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                //sMultipleViewInfo.mpStartButton->SetText(L"Start");
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),IDLE_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),IDLE_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusLabel->SetText(L"IDLE");
            }
            break;
        case E_TSEMP_INST:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                //sMultipleViewInfo.mpStartButton->SetText(L"Start");
                sMultipleViewInfo.mpStatusLabel->SetText(L"INST"); 
            }
            break;
        case E_TSEMP_BUSY:
            //! 更新Process pos
            if (sMultipleViewInfo.mnProcessPos++ + 20 >= (int)msMultipleConfig.mnDownloadTimeSec)
            {
                sMultipleViewInfo.mpDownloadProcess->SetValue(msMultipleConfig.mnDownloadTimeSec - 20);
            }
            else
            {
                sMultipleViewInfo.mpDownloadProcess->SetValue(sMultipleViewInfo.mnProcessPos);
            }
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                if (mpConfigLanBtn->IsEnabled()) mpConfigLanBtn->SetEnabled(false);
                if (mpChooseBtn->IsEnabled()) mpChooseBtn->SetEnabled(false);
                if (sMultipleViewInfo.mpDownloadProcess->GetMaxValue() != msMultipleConfig.mnDownloadTimeSec) sMultipleViewInfo.mpDownloadProcess->SetMaxValue(msMultipleConfig.mnDownloadTimeSec);

                //sMultipleViewInfo.mpStartButton->SetText(L"Stop");
                sMultipleViewInfo.mnProcessPos = 0;
                //sThreadParam.mstrTSEMultipleTestLog.ClearText();
                sMultipleViewInfo.mpLogRickEdit->SetText(L"");
                //! JGW_GetFormatWString(L"file='%s' corner='5,5,5,5';",BUSY_PROCESS_IMG).c_str()
                sMultipleViewInfo.mpDownloadProcess->SetForeImage(BUSY_PROCESS_IMG_STRING);
                sMultipleViewInfo.mpStatusLabel->SetText(L"BUSY");
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),BUSY_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),BUSY_STATUS_BKCOLOR);
                if (NULL != sThreadParam.mpsBindNetCardInfo)
                {
                    JGW_FormatWString(strTemp,L"%s - %s",sThreadParam.mpsBindNetCardInfo->mstrIPAddr.c_str(),sThreadParam.mpsBindNetCardInfo->mstrAdapterName.c_str());
                    sMultipleViewInfo.mpDeviceDesTextUI->SetText(strTemp.c_str());
                }  
            }
            break;
        case E_TSEMP_FAIL:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                //sMultipleViewInfo.mpStartButton->SetText(L"Start");
                sMultipleViewInfo.mpStatusLabel->SetText(L"FAIL");
                sMultipleViewInfo.mpDownloadProcess->SetForeImage(FAIL_PROCESS_IMG);
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),FAIL_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),FAIL_STATUS_BKCOLOR);
            }  
            break;
        case E_TSEMP_PASS:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                sMultipleViewInfo.mpDownloadProcess->SetValue(msMultipleConfig.mnDownloadTimeSec);
                //sMultipleViewInfo.mpStartButton->SetText(L"Start");
                sMultipleViewInfo.mpStatusLabel->SetText(L"PASS");
                sMultipleViewInfo.mpDownloadProcess->SetForeImage(PASS_PROCESS_IMG);
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),PASS_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),PASS_STATUS_BKCOLOR);
            }
            break;
        }
        if (sThreadParam.mstrTSEMultipleTestLog.IsHaveCurrntTestLog())
        {
            sMultipleViewInfo.mpLogRickEdit->AppendText(sThreadParam.mstrTSEMultipleTestLog.GetCurrntTestLog().c_str());
            sMultipleViewInfo.mpLogRickEdit->EndDown();
        }
        sThreadParam.mePrevTSEMPTestStatus = sThreadParam.meTSEMPTestStatus;
    }

    void CCTSEMultipleTestPluginDlg::InitMultipleView()
    {
        ClearStartMsgNotifyClick();
        mpMainMultipleVerticalLayout->RemoveAll();
        mvMultipleTestView.clear();
        mvsMultipleTestThreadParam.clear();
        int mnThreadIndex = 0;

        for (int i = 0;i < msMultipleConfig.mnMultipleTestRow;i++)
        {
#if 0
            CHorizontalLayoutUI* pHorizontalLayoutUI = new CHorizontalLayoutUI();
            pHorizontalLayoutUI->SetBorderSize(2);
            pHorizontalLayoutUI->SetBorderColor(0xffffff);
            pHorizontalLayoutUI->SetName(L"debug");
            RECT padding = {5,5,5,5};
            pHorizontalLayoutUI->SetPadding(padding);
            //! <HorizontalLayout enabled="true" bordersize="2" bordercolor="#ffffff" padding="5,5,5,5"></HorizontalLayout>
#else
            CDialogBuilder builder;
            CHorizontalLayoutUI* pHorizontalLayoutUI = (CHorizontalLayoutUI*)(builder.Create(L"<Window><HorizontalLayout enabled=\"true\" bordersize=\"2\" bordercolor=\"#ffffff\" padding=\"5,5,5,5\"></HorizontalLayout></Window>",(UINT)0,NULL,mpPluginPaintManagerUI));
#endif
            if (NULL == pHorizontalLayoutUI)
            {
                continue;
            }

            for (int y = 0;y < msMultipleConfig.mnMultipleTestCol;y++)
            {
                CDialogBuilder builder1;
                CVerticalLayoutUI* pListContainerItem = (CVerticalLayoutUI*)(builder1.Create(_T("multiple_single_test.xml"),(UINT)0,NULL,mpPluginPaintManagerUI));
                if (NULL != pListContainerItem)
                {
                    S_MULTIPLE_VIEW_INFO sMultipleViewInfo = {0};
                    S_TSE_MULTIPLE_TEST_THREAD_PARAM sTSEMultipleTestThreadParam;
                    mnThreadIndex ++;
                    sTSEMultipleTestThreadParam.mbIsFinishedTSEMultipleTest = true;
                    sTSEMultipleTestThreadParam.mnSortIndex = mnThreadIndex;
                    sTSEMultipleTestThreadParam.meTSEMPTestStatus = E_TSEMP_IDLE;
                    sTSEMultipleTestThreadParam.mePrevTSEMPTestStatus = E_TSEMP_UNKNOWN;
                    sTSEMultipleTestThreadParam.mpsMultipleConfig = &msMultipleConfig;
                    sTSEMultipleTestThreadParam.mpsBindNetCardInfo = NULL;
                    sTSEMultipleTestThreadParam.mpMultipleTestThread = NULL;
                    //sTSEMultipleTestThreadParam.mbNetCardStatusUpdateFlag = false;

                    sMultipleViewInfo.mpIndexLabel = (CLabelUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(0))->GetItemAt(0);
                    sMultipleViewInfo.mpDownloadProcess = (CProgressUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(0))->GetItemAt(2);

                    //sMultipleViewInfo.mpStartButton = (CButtonUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(0))->GetItemAt(4);
                    sMultipleViewInfo.mpStatusHorizontal = (CHorizontalLayoutUI*)pListContainerItem->GetItemAt(1);
                    sMultipleViewInfo.mpStatusLabel = (CLabelUI*)sMultipleViewInfo.mpStatusHorizontal->GetItemAt(0);
                    sMultipleViewInfo.mpDeviceDesTextUI = (CTextUI*)sMultipleViewInfo.mpStatusHorizontal->GetItemAt(1);
                    sMultipleViewInfo.mpLogRickEdit = (CRichEditUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(2))->GetItemAt(0);
                    sMultipleViewInfo.mpIndexLabel->SetText(JGW_GetFormatWString(L"%d",mvMultipleTestView.size() + 1).c_str());
                    sMultipleViewInfo.mnProcessPos = 0;
                    sMultipleViewInfo.mpDownloadProcess->SetValue(0);
                    sMultipleViewInfo.mpDownloadProcess->SetForeImage(IDLE_PROCESS_IMG);

                    /*sMultipleViewInfo.mpStartButton->SetDate((LPVOID)mvMultipleTestView.size());
                    sMultipleViewInfo.mpStartButton->OnNotify += MakeDelegate(this,&CCTSEMultipleTestPluginDlg::OnStartMsgBtnClick,_T("click"));*/
                    UpdateMultipleSingleView(sTSEMultipleTestThreadParam,sMultipleViewInfo);
                    pHorizontalLayoutUI->Add(pListContainerItem);
                    mvsMultipleTestThreadParam.push_back(sTSEMultipleTestThreadParam);
                    mvMultipleTestView.push_back(sMultipleViewInfo);
                } 
            }
            mpMainMultipleVerticalLayout->Add(pHorizontalLayoutUI);
        }
    }

    bool CCTSEMultipleTestPluginDlg::OnStartMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
    {
        size_t nCurrentTestIndex = (size_t)pTNotifyUI->pSender->GetDate();
        if (nCurrentTestIndex >= mvMultipleTestView.size() || nCurrentTestIndex < 0) 
        {
            return false;
        }
        /*mvMultipleTestView[nCurrentTestIndex].mpDownloadProcess->SetValue(10 * nCurrentTestIndex);
        UpdateMultipleSingleView((E_TSEMP_TEST_STATUS)nCurrentTestIndex,mvMultipleTestView[nCurrentTestIndex]);*/
        return true;
    }
}
