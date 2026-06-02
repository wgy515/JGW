#include "StdAfx.h"
#include "CTSEAPSMultiUpgradePluginView.h"
#include "AdapterCommon.h"
#include "CJGW_APSUpgradeConfigDlg.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_PluginViewLib/LogShowDuidlg.h>
#include <regex>
namespace JGW
{
    CCTSEAPSMultiUpgradePluginView::CCTSEAPSMultiUpgradePluginView(void) : mpsViewResolverParam(NULL),mpPluginPaintManagerUI(NULL),mLanSortConfig(msNetCardParam)
    {
        msPluginSkin.mstrSkinXmlPath = L"ap_multi_upgrade_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        msAPSMultiUpgradeConfig.mnDownloadTime = 100;
        msAPSMultiUpgradeConfig.mbIsDownloadFolder = false;
        msAPSMultiUpgradeConfig.mnDownloadTimeOut = 200;
        msAPSMultiUpgradeConfig.mstrDownloadFilePath = L"update.zip";
        msAPSMultiUpgradeConfig.mbSaveDownloadLog = false;
        msAPSMultiUpgradeConfig.mbTSEParamIsFileName = true;
        msAPSMultiUpgradeConfig.mstrHostIPAddr = L"192.168.213.252";
        msAPSMultiUpgradeConfig.mstrHostIPAddrAnsi = JGW_W2A(msAPSMultiUpgradeConfig.mstrHostIPAddr);
        msAPSMultiUpgradeConfig.mstrUpgradeVersion = L"";
        msAPSMultiUpgradeConfig.mstrCurrentWorkDir = JGW_GetApplicationFolder();
        msAPSMultiUpgradeConfig.mstrDownloadProcessPath = JGW_GetExecConfigFilePath(L"TSEAPSUpgradeTool.exe");
        mnIpAdapterInfoMemSize = sizeof(IP_ADAPTER_INFO);
        mpIpAdapterInfo = (PIP_ADAPTER_INFO)calloc(1,mnIpAdapterInfoMemSize);
        mnIfTableMemSize = sizeof(MIB_IFTABLE);
        mpIfTable = (MIB_IFTABLE*)calloc(1,mnIfTableMemSize);

        for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            msNetCardParam[i].mbNetCardStatusUpdateFlag = false;
            msNetCardParam[i].meNetCardStatus = ENETCARD_CONNECT_STATUS;
            msNetCardParam[i].mePrvFinishedDownloadingNetCardStatus = ENETCARD_CONNECT_STATUS;
            msNetCardParam[i].meDownloadStatus = E_IDLE_DOWNLOAD;
            msNetCardParam[i].mePrevDownloadStatus = E_INST_DOWNLOAD;
            msNetCardParam[i].mbIsFinishedDownloading = false;
            msNetCardParam[i].mnProgressPos = 0;
            msNetCardParam[i].mnIndex = i;
        }
    }


    CCTSEAPSMultiUpgradePluginView::~CCTSEAPSMultiUpgradePluginView(void)
    {
        if (mpIfTable)
        {
            free(mpIfTable);
            mpIfTable = NULL;
        }
        if (mpIpAdapterInfo)
        {
            free(mpIpAdapterInfo);
            mpIpAdapterInfo = NULL;
        }
    }

    void CCTSEAPSMultiUpgradePluginView::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        KillTimer(mMainHwnd,WM_TIMER_UPDATE_DEVICE_INFO);
        KillTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    bool CCTSEAPSMultiUpgradePluginView::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        {
            mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            mMainHwnd = mpPluginPaintManagerUI->GetPaintWindow();
            if ( !GetDuiContronlClass() ) return false;
        }
        {
            InitDownloadConfig();

            InitUIConfig();
        }
        {
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,
                WM_TIMER,&CCTSEAPSMultiUpgradePluginView::OnResponseTimer);
            SetTimer(mMainHwnd,
                WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,
                UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
            SetTimer(mMainHwnd,
                WM_TIMER_UPDATE_DEVICE_INFO,
                UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }
        return true;
    }

    void CCTSEAPSMultiUpgradePluginView::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if (pNotifyMsg->sType == DUI_MSGTYPE_CLICK)
        {
            if (m_pChooseImageBtn == pNotifyMsg->pSender)
            {
                OnChangeDownloadPathChooseBtn();
                InitUIConfig();
            }
            else if (m_pClearSortBtn == pNotifyMsg->pSender)
            {
                for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++) msNetCardParam[i].mstrDescription = L"";
                mLanSortConfig.ClearLanSortConfig();
            }
            else if (m_pConfigLanBtn == pNotifyMsg->pSender)
            {
                OnClickConfigBtn();
            }
            else
            {
                for ( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
                {
                    if ( pNotifyMsg->pSender == m_plogBtn[i] )
                    {
                        //std::wstring wstrDebug = g_strDownloadDubugLog[i];
                        std::unique_ptr<CLogShowDuidlg> ptrLogDlg( new CLogShowDuidlg(g_strDownloadDubugLog[i],g_strDownloadDubugLog[i],mMainHwnd) );
                        ptrLogDlg->DoModal();
                        return;
                    }
                }
            }
        }
    }

    bool CCTSEAPSMultiUpgradePluginView::GetDuiContronlClass()
    {
        bool bSuccess = true;
        wchar_t strProgressName[20] = {0},strStatusName[20] = {0},strComLabel[20] = {0},strlogBtn[20] = {0};

        for ( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            swprintf_s(strProgressName,20,L"DownLoadProgress%d",i + 1);
            swprintf_s(strStatusName,20,L"statusText%d",i + 1);
            swprintf_s(strComLabel,20,L"comLabel%d",i + 1);
            swprintf_s(strlogBtn,20,L"DownLoadLog%d",i + 1);

            m_pDownLoadProgress[i] = static_cast<CProgressUI*>(mpPluginPaintManagerUI->FindControl(strProgressName));
            m_pstatusText[i] = static_cast<CTextUI*>(mpPluginPaintManagerUI->FindControl(strStatusName));
            m_pcomLabel[i] = static_cast<CLabelUI*>(mpPluginPaintManagerUI->FindControl(strComLabel));
            m_plogBtn[i] = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(strlogBtn));

            bSuccess &= (NULL != m_pDownLoadProgress[i]);
            bSuccess &= (NULL != m_pstatusText[i]);
            bSuccess &= (NULL != m_pcomLabel[i]);
            bSuccess &= (NULL != m_plogBtn[i]);

            if( bSuccess )
            {
                m_pDownLoadProgress[i]->SetValue(0);
                m_pDownLoadProgress[i]->SetMaxValue(600);
                m_pcomLabel[i]->SetEnabled(false);
            }
        }
        m_pDownImagePathLabel = static_cast<CLabelUI*>(mpPluginPaintManagerUI->FindControl(L"downPathLabel"));
        m_pChooseImageBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"chooseBtn"));
        m_pClearSortBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"clearBtn"));
        m_pConfigLanBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"configLanBtn"));
        bSuccess &= (NULL != m_pDownImagePathLabel);
        bSuccess &= (NULL != m_pChooseImageBtn);
        bSuccess &= (NULL != m_pClearSortBtn);
        bSuccess &= (NULL != m_pConfigLanBtn);
        if( !bSuccess ) ::MessageBox( mpPluginPaintManagerUI->GetPaintWindow(),L"Lost skin control",L"error",MB_ICONERROR);

        return bSuccess;
    }

    void CCTSEAPSMultiUpgradePluginView::OnChangeDownloadPathChooseBtn()
    {
        if (!msAPSMultiUpgradeConfig.mbIsDownloadFolder)
        {
            std::wstring strProgEmmcFirehoseLitembnFilePath,strFile;
            if( !JGW_ChooseFile(strProgEmmcFirehoseLitembnFilePath,L"update*.bin;*.img\0update*.bin;*.img\0*.bin\0*.bin\0all file(*.*)\0*.*\0\0",mMainHwnd ) )  return ;
            m_pDownImagePathLabel->SetText(strProgEmmcFirehoseLitembnFilePath.c_str());
            JGW_ReplaceStringW(strProgEmmcFirehoseLitembnFilePath,JGW_GetApplicationFolder(),L"");
            mDownloadConfig.SetIniKeyValue(L"MAIN",L"DownLoadPath",strProgEmmcFirehoseLitembnFilePath.c_str());
            msAPSMultiUpgradeConfig.mstrDownloadFilePath = strProgEmmcFirehoseLitembnFilePath;
        }
        else
        {
            std::wstring strFolder;
            if (!JGW_ChooseFolder(strFolder,mMainHwnd)) return;
            m_pDownImagePathLabel->SetText(strFolder.c_str());
            mDownloadConfig.SetIniKeyValue(L"MAIN",L"DownLoadPath",strFolder.c_str());
            msAPSMultiUpgradeConfig.mstrDownloadFilePath = strFolder;
        }
    }

    void CCTSEAPSMultiUpgradePluginView::InitDownloadConfig()
    {
        mLanSortConfig.LoadLanSortConfig(JGW_GetConfigConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,LAN_SORT_FILE_NAME));
        mDownloadConfig.InitIniFilePath(JGW_GetCacheConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,L"down.ini").c_str());

        msAPSMultiUpgradeConfig.mstrDownloadFilePath = JGW_RealativePathToAbsPath(mDownloadConfig.GetIniKeyValue(L"MAIN",L"DownLoadPath",L"cache\\TSE_AP_MULTI_UPDATE\\update.zip").c_str());
        msAPSMultiUpgradeConfig.mnDownloadTime = mDownloadConfig.GetIniKeyIntValue(L"MAIN",L"DownLoadTime",100);
        msAPSMultiUpgradeConfig.mbSaveDownloadLog = mDownloadConfig.GetIniKeyBoolValue(L"MAIN",L"SaveDownloadLog",false);
        //msAPSMultiUpgradeConfig.mstrSaveDownloadLogFolder = mDownloadConfig.GetIniKeyValue(L"MAIN",L"SaveDownloadLogFolder",L"");
        /*if (msAPSMultiUpgradeConfig.mstrSaveDownloadLogFolder.empty() || !JGW_PathFileExists(msAPSMultiUpgradeConfig.mstrSaveDownloadLogFolder.c_str()))
        {*/
        //! Module Folder\\TestLog\\Module Folder\\suite name
        msAPSMultiUpgradeConfig.mstrSaveDownloadLogFolder = JGW_GetFormatWString(L"%sTestLog\\%s\\%s\\",JGW_GetApplicationFolder(),mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str(), mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName.c_str());
        //}
        msAPSMultiUpgradeConfig.mnDownloadTimeOut = mDownloadConfig.GetIniKeyIntValue(L"MAIN",L"DownLoadTimeOut",200);
        msAPSMultiUpgradeConfig.mstrHostIPAddr = mDownloadConfig.GetIniKeyValue(L"MAIN",L"HostIPAddr",L"192.168.213.252");
        msAPSMultiUpgradeConfig.mstrHostIPAddrAnsi = JGW_W2A(msAPSMultiUpgradeConfig.mstrHostIPAddr);
        msAPSMultiUpgradeConfig.mstrUpgradeVersion = mDownloadConfig.GetIniKeyValue(L"MAIN",L"UpgradeVersion",L"");
        msAPSMultiUpgradeConfig.mstrHostIPSeg = msAPSMultiUpgradeConfig.mstrHostIPAddr.substr(0,msAPSMultiUpgradeConfig.mstrHostIPAddr.find_last_of('.') + 1);
        msAPSMultiUpgradeConfig.mstrTSESuiteConfigFilePath = JGW_GetTSEConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
        msAPSMultiUpgradeConfig.mbTSEParamIsFileName = mDownloadConfig.GetIniKeyBoolValue(L"MAIN",L"TSEParamIsFileName",true);
        msAPSMultiUpgradeConfig.mbIsDownloadFolder = mDownloadConfig.GetIniKeyBoolValue(L"MAIN",L"IsDownLoadFolder",false);
    }

    void ScanDownloadFile(const wchar_t* filepath,const wchar_t* filename,void* lpData)
    {
        std::vector<std::wstring>* vDownloadFileList = (std::vector<std::wstring>*)lpData;
        std::wstring strTmep (filepath);
        strTmep += L"\\";
        strTmep += filename;
        vDownloadFileList->push_back(strTmep);
    }

    bool CCTSEAPSMultiUpgradePluginView::ParseDownloadFolderConfig()
    {
        msAPSMultiUpgradeConfig.mstrFolderTSEParam = L"";
        std::wstring strTseParam = mDownloadConfig.GetIniKeyValue(L"MAIN",L"TSE_Param",L"");
        if (strTseParam.empty()) return true;
        std::vector<std::wstring> vTSEParam,vDownloadFileList;
        JGW_ParserStrW(strTseParam.c_str(),L",",vTSEParam);

        int numFiles = 0;
        std::wstring strFolder(msAPSMultiUpgradeConfig.mstrDownloadFilePath);
        JGW_EraseLastAndFristTwoCharsW(strFolder,L'\\',L'/');
        JGW_TraverseFolder_VC(strFolder,numFiles,L"\\*.*",ScanDownloadFile,&vDownloadFileList);

        for (size_t i = 0;i < vTSEParam.size();i ++)
        {
            std::wstring strRegex = mDownloadConfig.GetIniKeyValue(L"MAIN",vTSEParam[i].c_str()),strFileName;
            std::wregex partten(strRegex,std::wregex::icase);
            bool result = false;
            for (size_t x = 0;x < vDownloadFileList.size();x ++)
            {
                strFileName = JGW_GetFileNameToFilePath(vDownloadFileList[x].c_str());
                if (regex_match(strFileName,partten))
                {
                    if (msAPSMultiUpgradeConfig.mbTSEParamIsFileName)
                    {
                        msAPSMultiUpgradeConfig.mstrFolderTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",vTSEParam[i].c_str(),strFileName.c_str());
                        JGW_FormatWString(strRegex,L"%s\\%s",strFolder.c_str(),strFileName.c_str());
                        if (!JGW_PathFileExists(strRegex.c_str()))
                        {
                            MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),strRegex.c_str(),L"File does not exist",MB_ICONERROR);
                            return false;
                        }
                    }
                    else
                    {
                        msAPSMultiUpgradeConfig.mstrFolderTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",vTSEParam[i].c_str(),vDownloadFileList[x].c_str());
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

    void CCTSEAPSMultiUpgradePluginView::SaveDownloadConfig()
    {
        std::wstring strTemp = msAPSMultiUpgradeConfig.mstrDownloadFilePath;
        JGW_ReplaceStringW(strTemp,JGW_GetApplicationFolder(),L"");

        mDownloadConfig.SetIniKeyValue(L"MAIN",L"DownLoadPath",strTemp.c_str());
        mDownloadConfig.SetIniKeyValue(L"MAIN",L"DownLoadTimeOut",msAPSMultiUpgradeConfig.mnDownloadTimeOut);
        mDownloadConfig.SetIniKeyValue(L"MAIN",L"DownLoadTime",msAPSMultiUpgradeConfig.mnDownloadTime);
        mDownloadConfig.SetIniKeyValue(L"MAIN",L"HostIPAddr",msAPSMultiUpgradeConfig.mstrHostIPAddr.c_str());
        mDownloadConfig.SetIniKeyValue(L"MAIN",L"UpgradeVersion",msAPSMultiUpgradeConfig.mstrUpgradeVersion.c_str());
        mDownloadConfig.SetIniKeyValue(L"MAIN",L"SaveDownloadLog",msAPSMultiUpgradeConfig.mbSaveDownloadLog);
    }

    void CCTSEAPSMultiUpgradePluginView::InitUIConfig()
    {
        if (!JGW_FileExistsToFilePath(msAPSMultiUpgradeConfig.mstrDownloadFilePath.c_str())) return ;

        if (msAPSMultiUpgradeConfig.mbIsDownloadFolder && !ParseDownloadFolderConfig()) return ;

        m_pDownImagePathLabel->SetText(msAPSMultiUpgradeConfig.mstrDownloadFilePath.c_str());

        for ( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            m_pDownLoadProgress[i]->SetMaxValue(msAPSMultiUpgradeConfig.mnDownloadTime);
        }

        if (!mMultiUpgreadManageThread.IsThreadRun())
        {
            mMultiUpgreadManageThread.OnInitManageThread(msNetCardParam,&msAPSMultiUpgradeConfig);
            mMultiUpgreadManageThread.CreateMessageThread();        
        }
    }

    void CCTSEAPSMultiUpgradePluginView::OnClickConfigBtn()
    {
        if (!GetAdapterInfo() || !GetIfTableInfo()) return ;
        CCJGW_APSUpgradeConfigDlg apsUpgradeConfigDlg(mMainHwnd,&msAPSMultiUpgradeConfig,mpIpAdapterInfo);
        if (IDOK == apsUpgradeConfigDlg.DoModal()) SaveDownloadConfig();
    }

    void CCTSEAPSMultiUpgradePluginView::OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == (int)wParam)
        {
            for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
            {
                UpdateDownloadInfo(i);
            }
        }
        else if (WM_TIMER_UPDATE_DEVICE_INFO == (int)wParam)
        {
            UpdateDeviceInfo();
        }
    }

    void CCTSEAPSMultiUpgradePluginView::UpdateDownloadInfo(int nIndex)
    {
        switch (msNetCardParam[nIndex].meDownloadStatus)
        {
        case E_IDLE_DOWNLOAD:
            if (msNetCardParam[nIndex].meDownloadStatus != msNetCardParam[nIndex].mePrevDownloadStatus)
            {
                msNetCardParam[nIndex].mnProgressPos = 0;
                m_pcomLabel[nIndex]->SetEnabled(false);
                m_pstatusText[nIndex]->SetText(_T("IDLE"));
                m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),L"#FF008080");
                //m_pDownLoadProgress[nIndex]->SetValue(0);
            }
            break;
        case E_INST_DOWNLOAD:
            if (msNetCardParam[nIndex].meDownloadStatus != msNetCardParam[nIndex].mePrevDownloadStatus)
            {
                m_pstatusText[nIndex]->SetText(_T("INST"));
                m_pcomLabel[nIndex]->SetEnabled(true);
                //m_pDownLoadProgress[nIndex]->SetValue(0);
                m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),INST_STATUS_BKCOLOR);
            }
            break;
        case E_BUSY_DOWNLOAD:
            if (msNetCardParam[nIndex].mnProgressPos < msAPSMultiUpgradeConfig.mnDownloadTime - 20) m_pDownLoadProgress[nIndex]->SetValue(++msNetCardParam[nIndex].mnProgressPos);
            else m_pDownLoadProgress[nIndex]->SetValue(msAPSMultiUpgradeConfig.mnDownloadTime - 20);
            if (msNetCardParam[nIndex].mnProgressPos > 1) break;
            m_pcomLabel[nIndex]->SetText(msNetCardParam[nIndex].mstrNetName.c_str());
            m_pcomLabel[nIndex]->SetEnabled(true);
            m_pstatusText[nIndex]->SetText(_T("BUSY"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),IDLE_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(PASS_PROCESS_COLOR);
            m_pChooseImageBtn->SetEnabled(false);
            m_pClearSortBtn->SetEnabled(false);
            m_pConfigLanBtn->SetEnabled(false);
            break;
        case E_ERROR_DOWNLOAD:
            if (msNetCardParam[nIndex].meDownloadStatus != msNetCardParam[nIndex].mePrevDownloadStatus)
            {
                msNetCardParam[nIndex].mnProgressPos = 0;
                m_pstatusText[nIndex]->SetText(_T("FAIL"));
                m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),FAIL_STATUS_DOWN);
                m_pDownLoadProgress[nIndex]->SetForeImage(FAIL_PROCESS_COLOR);
            }
            //! m_pDownLoadProgress[nIndex]->SetValue(m_pDownLoadProgress[nIndex]->GetValue() + 1);
            break;
        case E_SUCCESSFUL_DOWNLOAD:
            if (msNetCardParam[nIndex].meDownloadStatus != msNetCardParam[nIndex].mePrevDownloadStatus)
            {
                msNetCardParam[nIndex].mnProgressPos = 0;
                m_pstatusText[nIndex]->SetText(_T("PASS"));
                m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),PASS_STATUS_DOWN);
                m_pDownLoadProgress[nIndex]->SetValue( m_pDownLoadProgress[nIndex]->GetMaxValue());    
            }
            break;
        }
        msNetCardParam[nIndex].mePrevDownloadStatus = msNetCardParam[nIndex].meDownloadStatus;
    }



    LPS_NET_CARD_PARAM CCTSEAPSMultiUpgradePluginView::GetNetCardParamStruct(const std::wstring& strDescription)
    {
        for ( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            if (NULL == msNetCardParam[i].mstrDescription.compare(strDescription)) return &msNetCardParam[i];
        }

        for ( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            if (msNetCardParam[i].mstrDescription.empty()) return &msNetCardParam[i];
        }

        return NULL;
    }

    bool CCTSEAPSMultiUpgradePluginView::GetAdapterInfo()
    {
        unsigned long stSize = mnIpAdapterInfoMemSize;		
        //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
        int nRel = GetAdaptersInfo(mpIpAdapterInfo, &stSize);	

        //! 如果函数返回的是ERROR_BUFFER_OVERFLOW
        //! 则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
        //! 这也是说明为什么stSize既是一个输入量也是一个输出量
        if (ERROR_BUFFER_OVERFLOW == nRel)
        {
            free(mpIpAdapterInfo);	//释放原来的内存空间
            //! 重新申请内存空间用来存储所有网卡信息
            mpIpAdapterInfo = (PIP_ADAPTER_INFO)calloc(1,stSize);
            mnIpAdapterInfoMemSize = stSize;
            //! 再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
            nRel = GetAdaptersInfo(mpIpAdapterInfo, &stSize);
        }

        return (ERROR_SUCCESS == nRel);
    }

    bool CCTSEAPSMultiUpgradePluginView::GetIfTableInfo()
    {
        unsigned long dwSize = mnIfTableMemSize;
        DWORD dwRel = GetIfTable(mpIfTable, &dwSize, FALSE);
        if (ERROR_INSUFFICIENT_BUFFER == dwRel)	
        {
            free(mpIfTable);
            mpIfTable = (MIB_IFTABLE *)calloc(1,dwSize);	
            dwRel = GetIfTable(mpIfTable, &dwSize, FALSE);
        }
        return (ERROR_SUCCESS == dwRel);
    }

    bool CCTSEAPSMultiUpgradePluginView::InitNetCardParamToAdapterInfo()
    {
        PIP_ADAPTER_INFO pTempIpAdapterInfo = mpIpAdapterInfo;
        std::wstring strTempAddr,strTempDescription;

        while (pTempIpAdapterInfo)
        {
            strTempAddr = JGW_A2W_A(pTempIpAdapterInfo->IpAddressList.IpAddress.String);
            if (-1 == strTempAddr.find(msAPSMultiUpgradeConfig.mstrHostIPSeg)) { pTempIpAdapterInfo = pTempIpAdapterInfo->Next;continue;};

            strTempDescription = JGW_A2W_A(pTempIpAdapterInfo->Description);
            LPS_NET_CARD_PARAM psNetCardParam = GetNetCardParamStruct(strTempDescription);
            if (!psNetCardParam /*|| !psNetCardParam->mstrDescription.empty()*/) { pTempIpAdapterInfo = pTempIpAdapterInfo->Next;continue;};

            if (0 != psNetCardParam->mstrDescription.compare(strTempDescription))
            {
                psNetCardParam->mstrDescription = strTempDescription;
                mLanSortConfig.SaveLanSortConfig(psNetCardParam);
            }
            psNetCardParam->mstrAdapterName = JGW_A2W_A(pTempIpAdapterInfo->AdapterName);
            psNetCardParam->mstrNetName = JGW_GetNetcardDeviceNameToAdapterName(psNetCardParam->mstrAdapterName.c_str());
            psNetCardParam->mstrIPAddr = strTempAddr;
            if (psNetCardParam->mstrDescription.empty()) PDLOG4WW_F(L"AdapterInfo Description: %s,AdapterName: %s,IPAddr:%s",strTempDescription.c_str(),psNetCardParam->mstrAdapterName.c_str(),psNetCardParam->mstrIPAddr.c_str()); 
            psNetCardParam->mstrDescription = strTempDescription;
            pTempIpAdapterInfo = pTempIpAdapterInfo->Next;
        }

        for (unsigned int i = 0; i < mpIfTable->dwNumEntries; i++)
        {
            strTempDescription = JGW_A2W_A((const char*)mpIfTable->table[i].bDescr);
            for (int index = 0;index < DOWNLOAD_THREAD_MAX;index ++)
            {
                if (!msNetCardParam[index].mstrDescription.empty() && 0 == strTempDescription.compare(msNetCardParam[index].mstrDescription) && E_BUSY_DOWNLOAD != msNetCardParam[index].meDownloadStatus)
                {
                    //! 判断当前适配器网络是否处于连接状态
                    if (IF_OPER_STATUS_OPERATIONAL != mpIfTable->table[i].dwOperStatus)
                    {
                        if (ENETCARD_DISCONNCT_STATUS != msNetCardParam[index].meNetCardStatus) PDLOG4WW_F(L"AdapterInfo Description: %s,NetCardStatu: %s",strTempDescription.c_str(),L"DisConnect");
                        msNetCardParam[index].meNetCardStatus = ENETCARD_DISCONNCT_STATUS;
                        //! 记录第一次下载完成后的测试状态
                        if (msNetCardParam[index].mbIsFinishedDownloading)
                        {
                            msNetCardParam[index].mePrvFinishedDownloadingNetCardStatus = ENETCARD_DISCONNCT_STATUS;
                            msNetCardParam[index].mbIsFinishedDownloading = false;
                        }
                        if (msNetCardParam[index].meDownloadStatus != E_IDLE_DOWNLOAD && msNetCardParam[index].meDownloadStatus != E_BUSY_DOWNLOAD && ENETCARD_CONNECT_STATUS == msNetCardParam[index].mePrvFinishedDownloadingNetCardStatus)
                        {
                            if (msNetCardParam[index].meDownloadStatus == msNetCardParam[index].mePrevDownloadStatus)
                                msNetCardParam[index].meDownloadStatus = E_IDLE_DOWNLOAD;         
                        }
                        break;
                    }
                    else
                    {
                        //! 记录第一次下载完成后的测试状态
                        if (msNetCardParam[index].mbIsFinishedDownloading)
                        {
                            msNetCardParam[index].mePrvFinishedDownloadingNetCardStatus = ENETCARD_CONNECT_STATUS;
                            msNetCardParam[index].mbIsFinishedDownloading = false;
                        }
                        if (msNetCardParam[index].meDownloadStatus == E_IDLE_DOWNLOAD)
                        {
                            if (msNetCardParam[index].meDownloadStatus == msNetCardParam[index].mePrevDownloadStatus)
                                msNetCardParam[index].meDownloadStatus = E_INST_DOWNLOAD;
                        }
                    }
                    //! 更新状态的时候上报一次log
                    if (ENETCARD_CONNECT_STATUS != msNetCardParam[index].meNetCardStatus) PDLOG4WW_F(L"AdapterInfo Description: %s,NetCardStatu: %s",strTempDescription.c_str(),L"Connect");

                    //! if (msNetCardParam[index].mbNetCardStatusUpdateFlag) break;
                    if (ENETCARD_CONNECT_STATUS != msNetCardParam[index].meNetCardStatus) msNetCardParam[index].mbNetCardStatusUpdateFlag = true;
                    //! msNetCardParam[index].meNetCardStatus = ENETCARD_CONNECT_STATUS;

                    break;
                }
            }
        }

        return true;
    }

    void CCTSEAPSMultiUpgradePluginView::UpdateDeviceInfo()
    {
        if (!GetAdapterInfo() || !GetIfTableInfo()) return ;

        if (!InitNetCardParamToAdapterInfo()) return ;
    }
}