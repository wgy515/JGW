#include "StdAfx.h"
#include "CBoostDownloadView.h"
#include <JGW_DevicePool/DevicePortDefine.h>
#include <JGW_PluginViewLib/LogShowDuidlg.h>
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>

namespace JGW
{
    Cache_Config_Info					g_sCacheConfigInfo;

    CCBoostDownloadView::CCBoostDownloadView(void):mpsViewResolverParam(NULL)
        ,mpPluginPaintManagerUI(NULL)
        ,m_pSFManageThread(NULL)
        ,mMainHwnd(NULL)
        ,m_bIsInitDownLoadPathOk(false)
        ,mps_download_all_file_info(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"DownloadSkin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }

    CCBoostDownloadView::~CCBoostDownloadView(void)
    {
    }

    void CCBoostDownloadView::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        REMOVE_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,mpPluginPaintManagerUI->GetPaintWindow());
        KillTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);

        if (m_pSFManageThread) delete m_pSFManageThread;
        m_pSFManageThread = NULL;

        while(!mqueue_buffer.is_empty()) mqueue_buffer.get()->Dec();

        if( mpsViewResolverParam->mpMapRegMsg ) mpsViewResolverParam->mpMapRegMsg->clear();
        if( mpsViewResolverParam->mpMainParentContainer ) mpsViewResolverParam->mpMainParentContainer->RemoveAll();
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    bool CCBoostDownloadView::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
        if (!mpsViewResolverParam) return false;

        {
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            m_pSFManageThread = new CSFManageThread(g_sCacheConfigInfo.mstrDownloadPortSortConfigFilePath);
            mMainHwnd = mpPluginPaintManagerUI->GetPaintWindow();

            Log4WD_F(L"CGNetDownLoadViewDlg::InitSubWndPlugin mpsViewResolverParam->mpMapRegMsg:%d,mpPluginPaintManagerUI:%d,mpsViewResolverParam->mpMainParentContainer:%d,mpsViewResolverParam->mpPluginInfo:%d,mMainHwnd:%d,mpsViewResolverParam->mpsCommonWndMsgInfo:%d,m_pSFManageThread:%d",mpsViewResolverParam->mpMapRegMsg
                ,mpPluginPaintManagerUI
                ,mpsViewResolverParam->mpMainParentContainer
                ,mpsViewResolverParam->mpPluginInfo
                ,mMainHwnd
                ,mpsViewResolverParam->mpsCommonWndMsgInfo
                ,m_pSFManageThread
                );
            if ( !mpsViewResolverParam->mpMapRegMsg || !mpPluginPaintManagerUI || !mpsViewResolverParam->mpMainParentContainer || !mpsViewResolverParam->mpPluginInfo || !mMainHwnd || !mpsViewResolverParam->mpsCommonWndMsgInfo || !m_pSFManageThread ) return false;
            //! 获取DUI控件类指针

            if ( !GetDuiContronlClass() ) return false;
        }
        //! 添加响应设备池消息
        {
            GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG( mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),guid,/*E_DIAG_MODE |*/ E_DOWNLOADER_MODE);

            GUID adb_guid = GUID_ADB_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),adb_guid,E_ADB_MODE);

            /*REG_DEVICEPOOL_MSG( mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),guid,E_COM_MSG,E_DEVICE_ALL_MSG);*/
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,&CCBoostDownloadView::OnResponseDeviceMsg);
        }
        //! 添加消息响应
        {
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_TIMER,&CCBoostDownloadView::OnResponseTimer);
            SetTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }
        //! 路径获取
        {
            g_sCacheConfigInfo.m_strCurrentWorkDir = JGW_GetApplicationFolder();

            if (!mDownloadIniConfig.InitIniFilePath(JGW_GetCacheConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,CONFIG_DOWNLOAD_NAME).c_str())) return false;
            g_sCacheConfigInfo.mstrDownloadPortSortConfigFilePath = JGW_GetConfigConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,SORT_PORT_CONFIG_NAME);
            CCJGW_PeekNamePide::SetInitPideCMDExePath(JGW_GetExecConfigFilePath(L"cmd.exe"));
        }
        //! 加载配置已经开启下载管理线程
        {
            LoadIniConfigFile();

            UpdateUIShow();

            StartMagagerThread();
        }

        return true;
    }

    void CCBoostDownloadView::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if ( pNotifyMsg->sType == DUI_MSGTYPE_CLICK )
        {
            if ( m_pClearSortBtn == pNotifyMsg->pSender )
            {
                if( m_pSFManageThread->ClearSortInfo() )
                {
                    CloseSubWndPlugin();
                }		
            }
            else if ( m_pChooseFloderbtn == pNotifyMsg->pSender )
            {
                if( m_pSFManageThread->IsStartDownload() ) 
                {
                    MessageBox(mMainHwnd,L"已经开始下载，不允许修改配置，请重新打开工具，然后选择下载路径",L"访问错误",MB_ICONERROR);
                    return;
                }
                OnChangeDownloadPathChooseBtn();
            }
            else
            {
                for ( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
                {
                    if ( pNotifyMsg->pSender == m_plogBtn[i] )
                    {
                        std::wstring wstrDebug = g_strDownloadDubugLog[i];
                        std::unique_ptr<CLogShowDuidlg> ptrLogDlg( new CLogShowDuidlg(wstrDebug,wstrDebug,mMainHwnd) );
                         ptrLogDlg->DoModal();
                        return;
                    }
                }
            }
        }
        else if( pNotifyMsg->sType == DUI_MSGTYPE_SELECTCHANGED )
        {
            if( m_pResetAfterDownloadCheckBox == pNotifyMsg->pSender )
            {
                g_sCacheConfigInfo.m_bResetAfterDownload = m_pResetAfterDownloadCheckBox->GetCheck();
                mDownloadIniConfig.SetIniKeyValue(L"MAIN",L"ResetAfterDownload",g_sCacheConfigInfo.m_bResetAfterDownload);
            }
            else if ( m_pAutoSwitchComCheckBox == pNotifyMsg->pSender  )
            {
                g_sCacheConfigInfo.m_bAutoSwitchComport = m_pAutoSwitchComCheckBox->GetCheck();
                mDownloadIniConfig.SetIniKeyValue(L"MAIN",L"AutoSwitchComport",g_sCacheConfigInfo.m_bAutoSwitchComport);
            }
        }
        else if ( pNotifyMsg->sType == DUI_MSGTYPE_EDITREGEX )
        {
            if ( m_pModeEdt == pNotifyMsg->pSender )
            {
                mDownloadIniConfig.SetIniKeyValue(L"MAIN",L"ModeName",m_pModeEdt->GetText().GetData());
            }
        }
    }

    void CCBoostDownloadView::OnResponseDeviceMsg( const WPARAM& wParam,const LPARAM& lParam )
    {
        Log4WD(L"CGNetDownLoadViewDlg::OnResponseDeviceMsg");
        if( !wParam || !m_bIsInitDownLoadPathOk )return ;

        SDeviceMsg_Info* lpdevicemsg = (SDeviceMsg_Info*)wParam;
#ifdef _DEBUG
        OutputDebugString(lpdevicemsg->m_strDbcc_name.c_str());
        OutputDebugString(_T("\n"));
#endif
        if ( DBT_DEVICEARRIVAL == lpdevicemsg->m_nDeviceMsgType )
        {
            Log4WD_F(L"[OnDeviceChange]  DBT_DEVICEARRIVAL %s",lpdevicemsg->m_strDbcc_name.c_str());
            if (g_sCacheConfigInfo.m_bAutoSwitchComport ||  E_DOWNLOADER_MODE == lpdevicemsg->m_nDeviceMode)
            {
                mqueue_buffer.put(lpdevicemsg);
                return ;
            }
        }
        else if ( DBT_DEVICEREMOVECOMPLETE == lpdevicemsg->m_nDeviceMsgType )
        {
            if (E_DOWNLOADER_MODE == lpdevicemsg->m_nDeviceMode)
            {
                int nComPort = lpdevicemsg->m_nComPort;
                std::string strPort;
                const S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo = NULL;
                JGW_FormatString(strPort,"COM%d",nComPort);
                for ( int i = 0;i < 8;i ++)
                {
                    m_pSFManageThread->GetDownloadInfo(i,&psDownloadProgressInfo);
                    if (!psDownloadProgressInfo) break;
                    if (NULL == JGW_StrComparenoCaseStr(strPort.c_str(),psDownloadProgressInfo->mstrComPort.c_str()))
                    {
                        /*if (psDownloadProgressInfo->meDownloadStatus == E_SUCCESSFUL_DOWNLOAD 
                            || psDownloadProgressInfo->meDownloadStatus == E_ERROR_DOWNLOAD)
                        {
                            UpdateDownloadInfo(i,psDownloadProgressInfo);
                        }*/
                        m_pcomLabel[i]->SetEnabled(false);
                        break;
                    }
                }
            }
        }

        lpdevicemsg->Dec();
        return ;
    }

    void CCBoostDownloadView::OnResponseTimer( const WPARAM& wParam,const LPARAM& lParam )
    {
        //! 防止页面频繁刷新
        static S_DOWNLOAD_PROGRESS_INFO veDownloadProgressInfo[MAX_DOWNLOAD_THREAD_COUNT] = {E_IDLE_DOWNLOAD};

        if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == (int)wParam)
        {
            const S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo = NULL;
            for (int i = 0; i < MAX_DOWNLOAD_THREAD_COUNT;i ++)
            {
                m_pSFManageThread->GetDownloadInfo(i,&psDownloadProgressInfo);
                if (!psDownloadProgressInfo) continue;
                if (psDownloadProgressInfo->meDownloadStatus == veDownloadProgressInfo[i].meDownloadStatus && psDownloadProgressInfo->mdw64SendCounts == veDownloadProgressInfo->mdw64SendCounts) continue;
                UpdateDownloadInfo(i,psDownloadProgressInfo);
                veDownloadProgressInfo[i].meDownloadStatus = psDownloadProgressInfo->meDownloadStatus;
                veDownloadProgressInfo[i].mdw64SendCounts = psDownloadProgressInfo->mdw64SendCounts;
            }
        }
    }

    void CCBoostDownloadView::UpdateDownloadInfo(int nIndex,const S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo)
    {
        TCHAR szComport[10] = {0};
        int pos = 0;
        switch (psDownloadProgressInfo->meDownloadStatus)
        {
        case E_IDLE_DOWNLOAD:
            m_pcomLabel[nIndex]->SetEnabled(false);
            m_pstatusText[nIndex]->SetText(_T("IDLE"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),IDLE_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(PASS_PROCESS_COLOR);
            m_pDownLoadProgress[nIndex]->SetValue(0);
            break;
        case E_BUSY_DOWNLOAD:
            /*_stprintf_s(szComport,10,_T("COM%d"),m_pSFManageThread->msPortInfo[nIndex].mnPort);*/
            m_pcomLabel[nIndex]->SetText(JGW_A2W(psDownloadProgressInfo->mstrComPort).c_str());
            m_pcomLabel[nIndex]->SetEnabled(true);
            m_pstatusText[nIndex]->SetText(_T("BUSY"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),IDLE_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(PASS_PROCESS_COLOR);
            /*m_pDownLoadProgress[nIndex ]->SetValue( 1 );*/
            m_pAutoSwitchComCheckBox->SetEnabled(false);
            m_pResetAfterDownloadCheckBox->SetEnabled(false);
            pos = (int)psDownloadProgressInfo->mdw64SendCounts / (1024*1024);
            if (0 == pos || pos != m_pDownLoadProgress[nIndex]->GetValue())
                m_pDownLoadProgress[nIndex]->SetValue( pos?pos:10/*psDownloadProgressInfo->mdw64SendCounts / (1024*1024)*/ );
            break;
        case E_ERROR_DOWNLOAD:
            m_pstatusText[nIndex]->SetText(_T("FAIL"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),FAIL_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(FAIL_PROCESS_COLOR);
            m_pDownLoadProgress[nIndex]->SetValue(m_pDownLoadProgress[nIndex]->GetValue() + 1);
            break;
        case E_SUCCESSFUL_DOWNLOAD:
            m_pstatusText[nIndex]->SetText(_T("PASS"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),PASS_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetValue( m_pDownLoadProgress[nIndex]->GetMaxValue() );
            break;
        }
    }

    bool CCBoostDownloadView::GetDuiContronlClass()
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
                m_pDownLoadProgress[i]->SetMaxValue(PROCESS_MAX_VALUE);
                m_pcomLabel[i]->SetEnabled(false);
            }
        }

        m_pAutoSwitchComCheckBox = static_cast<CCheckBoxUI*>(mpPluginPaintManagerUI->FindControl(L"autoSwitchComCheckBox"));
        m_pResetAfterDownloadCheckBox = static_cast<CCheckBoxUI*>(mpPluginPaintManagerUI->FindControl(L"bResetAfterDownloadbox"));
        m_pModeEdt = static_cast<CEditUI*>(mpPluginPaintManagerUI->FindControl(L"modeledt"));
        m_pClearSortBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"clearBtn"));
        m_pCrcLabel = static_cast<CLabelUI*>(mpPluginPaintManagerUI->FindControl(L"crcLabel"));
        m_pDownPathLabel = static_cast<CLabelUI*>(mpPluginPaintManagerUI->FindControl(L"downPathLabel"));
        m_pChooseFloderbtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"chooseBtn"));
        bSuccess &= (NULL != m_pAutoSwitchComCheckBox);
        bSuccess &= (NULL != m_pResetAfterDownloadCheckBox);
        bSuccess &= (NULL != m_pModeEdt);
        bSuccess &= (NULL != m_pClearSortBtn);
        bSuccess &= (NULL != m_pCrcLabel);
        bSuccess &= (NULL != m_pDownPathLabel);
        bSuccess &= (NULL != m_pChooseFloderbtn);

        m_pProgrammerText = static_cast<CEditUI*>(mpPluginPaintManagerUI->FindControl(L"programmerText"));
        m_pRawprogramText = static_cast<CEditUI*>(mpPluginPaintManagerUI->FindControl(L"rawprogramText"));
        m_pPatchText = static_cast<CEditUI*>(mpPluginPaintManagerUI->FindControl(L"patchText"));
        bSuccess &= (NULL != m_pProgrammerText);
        bSuccess &= (NULL != m_pRawprogramText);
        bSuccess &= (NULL != m_pPatchText);

        if( !bSuccess ) ::MessageBox( mpPluginPaintManagerUI->GetPaintWindow(),L"Lost skin control",L"error",MB_ICONERROR);

        return bSuccess;
    }

    void CCBoostDownloadView::LoadIniConfigFile()
    {
        g_sCacheConfigInfo.ClearCacheConfigInfo();
        m_bIsInitDownLoadPathOk = false;
        //! 加载下载配置文件
        {
            std::wstring strTemp,strProgEmmcFirehoseLitembnFileNameTemp;
            //! 加载模块名称
            g_sCacheConfigInfo.m_strMode = mDownloadIniConfig.GetIniKeyValue(L"MAIN",L"ModeName");
            //! 获取下载路径 区分相对路径和绝对路径
            g_sCacheConfigInfo.m_strDownPath = mDownloadIniConfig.GetIniKeyValue(L"MAIN",L"DownLoadPath");
            JGW_ReplaceCharW(g_sCacheConfigInfo.m_strDownPath,L'/',L'\\');
            if( PathIsRelative(g_sCacheConfigInfo.m_strDownPath.c_str()) )
            {
                JGW_GetExecSubFolder(strTemp,L"cache" );
                strTemp += mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName;
                strTemp += L"\\";
                strTemp += g_sCacheConfigInfo.m_strDownPath;
                g_sCacheConfigInfo.m_strDownPath = strTemp;
            }
            if(g_sCacheConfigInfo.m_strDownPath.at(g_sCacheConfigInfo.m_strDownPath.size()-1) != L'\\')
                g_sCacheConfigInfo.m_strDownPath +=  _T("\\");
            //! 下载完成后重启
            g_sCacheConfigInfo.m_bResetAfterDownload = mDownloadIniConfig.GetIniKeyBoolValue(L"MAIN",L"ResetAfterDownload");

            g_sCacheConfigInfo.m_strMemoryname = mDownloadIniConfig.GetIniKeyValueA(L"MAIN",L"Memoryname");

            g_sCacheConfigInfo.m_bAutoSwitchComport = mDownloadIniConfig.GetIniKeyBoolValue(L"MAIN",L"AutoSwitchComport");

            g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath = mDownloadIniConfig.GetIniKeyValue(L"MAIN",L"ProgEmmcFirehoseLitembnFileName");
            //!			strProgEmmcFirehoseLitembnFileNameTemp = m_strDownLoadPath +  strValue;
            strcpy_s(g_sCacheConfigInfo.m_strProgEmmcFirehoseLitembnFilePath,512, w2a(g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath).c_str());

            g_sCacheConfigInfo.m_strRawprograms = mDownloadIniConfig.GetIniKeyValue(L"MAIN",L"Rawprograms");

            g_sCacheConfigInfo.m_strPatchs = mDownloadIniConfig.GetIniKeyValue(L"MAIN",L"Patchs");
        }
        //static CJGW_DownloadConfig firehos;
        //! check file is exits
        {
            s_download_config_info sDownloadConfigInfo;

            std::string strRawprograms = JGW_W2A(g_sCacheConfigInfo.m_strRawprograms);
            std::string strPatchs = JGW_W2A(g_sCacheConfigInfo.m_strPatchs);
            std::wstring strProgEmmcFirehoseLitembnFilePath = g_sCacheConfigInfo.m_strDownPath + g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath;

            sDownloadConfigInfo.mstrDownloadPath = JGW_W2A(g_sCacheConfigInfo.m_strDownPath);
            sDownloadConfigInfo.mbReset = g_sCacheConfigInfo.m_bResetAfterDownload;
            sDownloadConfigInfo.mstrArmPrgFilePath = JGW_W2A(strProgEmmcFirehoseLitembnFilePath);
            JGW_ParserStrA(strRawprograms.c_str(),",",sDownloadConfigInfo.mvstrRawProgram);
            JGW_ParserStrA(strPatchs.c_str(),",",sDownloadConfigInfo.mvstrPatch);
            if( sDownloadConfigInfo.mvstrRawProgram.empty() || sDownloadConfigInfo.mvstrPatch.empty() /*|| sDownloadConfigInfo.mvstrRawProgram.size() > 2*/ )
            {
                MessageBox(mMainHwnd,L"下载配置文件有误",L"选择下载配置文件为空",MB_ICONERROR);
                return ;
            }

            if (g_sCacheConfigInfo.m_strMemoryname.empty()) 
            {
                //! prog_emmc_firehose prog_ufs_firehose
                if ( !mdownloadConfig.LoadDonwloadConfig(&sDownloadConfigInfo,(NULL != JGW_WStrCaseWStr(g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath.c_str(),L"prog_emmc_firehose"))?"emmc":"ufs" ))
                {
                    MessageBoxA(mMainHwnd,mdownloadConfig.GetErrorMsg(),"error",MB_ICONERROR);
                    return ;
                }
            }
            else
            {
                if (!mdownloadConfig.LoadDonwloadConfig(&sDownloadConfigInfo,g_sCacheConfigInfo.m_strMemoryname.c_str()))
                {
                    MessageBoxA(mMainHwnd,mdownloadConfig.GetErrorMsg(),"error",MB_ICONERROR);
                    return ;
                }
            }

            mps_download_all_file_info = &mdownloadConfig.GetDownloadAllFileInfo();
            mps_download_all_file_info->mnFileSizeCounts /= 1024*1024;
            mps_download_all_file_info->mnFileSizeCounts += (int)(mps_download_all_file_info->mnFileSizeCounts * 0.2);
        }

        m_bIsInitDownLoadPathOk = true;
    }

    void CCBoostDownloadView::UpdateUIShow()
    {
        m_pAutoSwitchComCheckBox->SetCheck( g_sCacheConfigInfo.m_bAutoSwitchComport );
        m_pResetAfterDownloadCheckBox->SetCheck( g_sCacheConfigInfo.m_bResetAfterDownload );
        m_pModeEdt->SetText( g_sCacheConfigInfo.m_strMode.c_str() );

        if ( m_bIsInitDownLoadPathOk )
        {
            m_pDownPathLabel->SetText( g_sCacheConfigInfo.m_strDownPath.c_str() );
            m_pProgrammerText->SetText( JGW_A2W_A(g_sCacheConfigInfo.m_strProgEmmcFirehoseLitembnFilePath).c_str() );
            m_pRawprogramText->SetText( g_sCacheConfigInfo.m_strRawprograms.c_str() );
            m_pPatchText->SetText( g_sCacheConfigInfo.m_strPatchs.c_str() );
            for( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++ )
                m_pDownLoadProgress[i]->SetMaxValue( (int)mps_download_all_file_info->mnFileSizeCounts );

            m_pProgrammerText->SetTipValue(JGW_A2W_A(g_sCacheConfigInfo.m_strProgEmmcFirehoseLitembnFilePath).c_str());
            m_pRawprogramText->SetTipValue( g_sCacheConfigInfo.m_strRawprograms.c_str() );
            m_pPatchText->SetTipValue( g_sCacheConfigInfo.m_strPatchs.c_str() );
        }
        else
        {
            m_pDownPathLabel->SetText( L"" );
            m_pProgrammerText->SetText( L"" );
            m_pRawprogramText->SetText( L"" );
            m_pPatchText->SetText( L"" );
            m_pProgrammerText->SetTipValue( L"" );
            m_pRawprogramText->SetTipValue( L"" );
            m_pPatchText->SetTipValue( L"" );
        }
    }

    void CCBoostDownloadView::SaveIniConfigFile()
    {
        mDownloadIniConfig.SetIniKeyValue(L"MAIN",L"DownLoadPath",g_sCacheConfigInfo.m_strDownPath.c_str());

        mDownloadIniConfig.SetIniKeyValue(L"MAIN",L"ProgEmmcFirehoseLitembnFileName",g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath.c_str());
        mDownloadIniConfig.SetIniKeyValue(L"MAIN",L"Rawprograms",g_sCacheConfigInfo.m_strRawprograms.c_str());
        mDownloadIniConfig.SetIniKeyValue(L"MAIN",L"Patchs",g_sCacheConfigInfo.m_strPatchs.c_str());
    }

    void CCBoostDownloadView::OnChangeDownloadPathChooseBtn()
    {
        //! 选择ProgEmmcFireHose
        { //! prog_ufs_firehose_8996_ddr.elf
            std::wstring strProgEmmcFirehoseLitembnFilePath,strFile;
            if( !JGW_ChooseFile( strProgEmmcFirehoseLitembnFilePath,L"Programmer File(prog*firehose*.*)\0prog*firehose*.*\0Programmer File(prog_ufs_firehose*.elf)\0prog_ufs_firehose*.elf\0Programmer File(prog_emmc_firehose*.mbn)\0prog_emmc_firehose*.mbn\0all file(*.*)\0*.*\0\0",mMainHwnd ) )  return ;

            g_sCacheConfigInfo.m_strDownPath = JGW_GetPathOfFile(strProgEmmcFirehoseLitembnFilePath.c_str());
            g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath = JGW_GetFileNameToFilePath(strProgEmmcFirehoseLitembnFilePath.c_str());
            m_pProgrammerText->SetText(g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath.c_str());
            strcpy_s(g_sCacheConfigInfo.m_strProgEmmcFirehoseLitembnFilePath,512,w2a(g_sCacheConfigInfo.m_wstrProgEmmcFirehoseLitembnFilePath).c_str());
            m_pDownPathLabel->SetText( g_sCacheConfigInfo.m_strDownPath.c_str() );
        }
        //! rawprogram and patch
        {
            std::vector<std::wstring> _vRawProgram;
            std::vector<std::wstring> _vPatch;

            JGW_ChooseMultiSeleJGWile( _vRawProgram,L"rawprogram File(rawprogram*.xml)\0rawprogram*.xml\0all file(*.*)\0*.*\0\0",mMainHwnd ,OFN_EXPLORER | OFN_ALLOWMULTISELECT ,g_sCacheConfigInfo.m_strDownPath.size()?g_sCacheConfigInfo.m_strDownPath.c_str():L"c:\\" );

            JGW_ChooseMultiSeleJGWile( _vPatch,L"patch File(patch*.xml)\0patch*.xml\0all file(*.*)\0*.*\0\0",mMainHwnd ,OFN_EXPLORER | OFN_ALLOWMULTISELECT,g_sCacheConfigInfo.m_strDownPath.size()?g_sCacheConfigInfo.m_strDownPath.c_str():L"c:\\" );
            if( _vRawProgram.empty() || _vPatch.empty()/* || _vRawProgram.size() > 2 || _vPatch.size() > 2*/ || _vRawProgram.size() != _vPatch.size())
            {
                MessageBox(mMainHwnd,L"下载配置文件有误",L"选择下载配置文件为空",MB_ICONERROR);
                return;
            }
            g_sCacheConfigInfo.m_strRawprograms = L"";
            g_sCacheConfigInfo.m_strPatchs = L"";
            for ( size_t i = 0;i < _vRawProgram.size() /*&& i < 3*/;i ++)
            {
                if( !g_sCacheConfigInfo.m_strRawprograms.empty() ) g_sCacheConfigInfo.m_strRawprograms += L",";
                g_sCacheConfigInfo.m_strRawprograms += JGW_GetFileNameToFilePath( _vRawProgram.at(i).c_str() );

                if( !g_sCacheConfigInfo.m_strPatchs.empty() ) g_sCacheConfigInfo.m_strPatchs += L",";
                g_sCacheConfigInfo.m_strPatchs += JGW_GetFileNameToFilePath( _vPatch.at(i).c_str() );
            }
            m_pPatchText->SetText( g_sCacheConfigInfo.m_strPatchs.c_str() );
            m_pRawprogramText->SetText( g_sCacheConfigInfo.m_strRawprograms.c_str() );
        }
        {
            SaveIniConfigFile();

            LoadIniConfigFile();

            UpdateUIShow();

            StartMagagerThread();
        }
    }

    void CCBoostDownloadView::StartMagagerThread()
    {
        if ( !m_bIsInitDownLoadPathOk || m_pSFManageThread->IsThreadRun() ) return ;
        m_pSFManageThread->InitThreadParam(mMainHwnd,mps_download_all_file_info,&mqueue_buffer);
        m_pSFManageThread->CreateMessageThread();
        Sleep(250);
    }
}