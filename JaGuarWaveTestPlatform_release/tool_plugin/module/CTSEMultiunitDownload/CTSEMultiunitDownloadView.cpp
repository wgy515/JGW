#include "StdAfx.h"
#include "CTSEMultiunitDownloadView.h"
#include <JGW_DevicePool/DevicePortDefine.h>
#include <JGW_PluginViewLib/LogShowDuidlg.h>
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>

namespace JGW
{
    CCTSEMultiunitDownloadView::CCTSEMultiunitDownloadView(void):mpsViewResolverParam(NULL)
        ,mpPluginPaintManagerUI(NULL)
        ,m_pTSEMDManageThread(NULL)
        ,mMainHwnd(NULL)
        ,m_bIsInitDownLoadPathOk(false)
    {
        msPluginSkin.mstrSkinXmlPath = L"DownloadSkin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }

    CCTSEMultiunitDownloadView::~CCTSEMultiunitDownloadView(void)
    {
    }

    void CCTSEMultiunitDownloadView::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        mMultiunitDownloadConfig.SaveMultiunitDownloadConfig();
        REMOVE_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,mpPluginPaintManagerUI->GetPaintWindow());
        KillTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);

        if (m_pTSEMDManageThread) delete m_pTSEMDManageThread;
        m_pTSEMDManageThread = NULL;

        while(!mqueue_buffer.is_empty()) mqueue_buffer.get()->Dec();
        if( mpsViewResolverParam->mpMapRegMsg ) mpsViewResolverParam->mpMapRegMsg->clear();
        if( mpsViewResolverParam->mpMainParentContainer ) mpsViewResolverParam->mpMainParentContainer->RemoveAll();
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    bool CCTSEMultiunitDownloadView::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
        if (!mpsViewResolverParam) return false;

        {
            mpsTseMultiunitDownloadConfig = &mMultiunitDownloadConfig.GetSTSEMultiunitDownalodConfig();
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            m_pTSEMDManageThread = new CCTSEMDManageThread(mpsTseMultiunitDownloadConfig->mstrDownloadPortSortConfigFilePath);
            mMainHwnd = mpPluginPaintManagerUI->GetPaintWindow();
#if 0
            Log4WD_F(L"CGNetDownLoadViewDlg::InitSubWndPlugin mpsViewResolverParam->mpMapRegMsg:%d,mpPluginPaintManagerUI:%d,mpsViewResolverParam->mpMainParentContainer:%d,mpsViewResolverParam->mpPluginInfo:%d,mMainHwnd:%d,mpsViewResolverParam->mpsCommonWndMsgInfo:%d,m_pSFManageThread:%d",mpsViewResolverParam->mpMapRegMsg
                ,mpPluginPaintManagerUI
                ,mpsViewResolverParam->mpMainParentContainer
                ,mpsViewResolverParam->mpPluginInfo
                ,mMainHwnd
                ,mpsViewResolverParam->mpsCommonWndMsgInfo
                ,m_pTSEMDManageThread
                );
            if ( !mpsViewResolverParam->mpMapRegMsg || !mpPluginPaintManagerUI || !mpsViewResolverParam->mpMainParentContainer || !mpsViewResolverParam->mpPluginInfo || !mMainHwnd || !mpsViewResolverParam->mpsCommonWndMsgInfo || !m_pTSEMDManageThread ) return false;
#endif
            //! 获取DUI控件类指针
            if ( !GetDuiContronlClass() ) return false;
        }
        //! 添加响应设备池消息
        {
            GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG( mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),guid,E_DIAG_MODE | E_DOWNLOADER_MODE);
            /*REG_DEVICEPOOL_MSG( mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),guid,E_COM_MSG,E_DEVICE_ALL_MSG);*/
            GUID adb_guid = GUID_ADB_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),adb_guid,E_ADB_MODE);

            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,&CCTSEMultiunitDownloadView::OnResponseDeviceMsg);
            //! 添加消息响应 自动更新下载UI显示
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_TIMER,&CCTSEMultiunitDownloadView::OnResponseTimer);
            SetTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }
        
        //! 路径获取
        {
            //! 获取TSE 配置文件
            mpsTseMultiunitDownloadConfig->mstrTSESuiteConfigPath = JGW_GetTSEConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
            //! 获取下载 配置文件
            mstrIniConfigPath = JGW_GetCacheConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,CONFIG_DOWNLOAD_NAME);
            //! 
            if (!mMultiunitDownloadConfig.LoadMultiunitDownloadConfig(mstrIniConfigPath)) return false;
            //! 下载进程路径
            mpsTseMultiunitDownloadConfig->mstrCurrentWorkDir = JGW_GetApplicationFolder();
            mpsTseMultiunitDownloadConfig->mstrDownloadProcessPath = JGW_GetExecConfigFilePath(L"TSEDownload.exe");
            //! 初始化管道相关信息
            mpsTseMultiunitDownloadConfig->mstrDownloadPortSortConfigFilePath = JGW_GetConfigConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,SORT_PORT_CONFIG_NAME);
        }
        //! 加载配置已经开启下载管理线程
        {
            LoadIniConfigFile();

            UpdateUIShow();

            StartMagagerThread();
        }

        return true;
    }

    void CCTSEMultiunitDownloadView::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if ( pNotifyMsg->sType == DUI_MSGTYPE_CLICK )
        {
            if ( m_pClearSortBtn == pNotifyMsg->pSender )
            {
                if( m_pTSEMDManageThread->ClearSortInfo() )
                {
                    CloseSubWndPlugin();
                }		
            }
            else if ( m_pChooseFloderbtn == pNotifyMsg->pSender )
            {
                if( m_pTSEMDManageThread->IsStartDownload() ) 
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
                mpsTseMultiunitDownloadConfig->mbResetAfterDownload = m_pResetAfterDownloadCheckBox->GetCheck();
            }
            else if ( m_pAutoSwitchComCheckBox == pNotifyMsg->pSender  )
            {
                mpsTseMultiunitDownloadConfig->mbAutoSwitchDownloadPort = m_pAutoSwitchComCheckBox->GetCheck();
            }
        }
        else if ( pNotifyMsg->sType == DUI_MSGTYPE_EDITREGEX )
        {
            if ( m_pModeEdt == pNotifyMsg->pSender )
            {
                mpsTseMultiunitDownloadConfig->mstrModeName = m_pModeEdt->GetText().GetData();
            }
        }
    }

    void CCTSEMultiunitDownloadView::OnResponseDeviceMsg( const WPARAM& wParam,const LPARAM& lParam )
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
            if (mpsTseMultiunitDownloadConfig->mbAutoSwitchDownloadPort ||  E_DOWNLOADER_MODE == lpdevicemsg->m_nDeviceMode)
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
                S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo = NULL;
                for ( int i = 0;i < 8;i ++)
                {
                    m_pTSEMDManageThread->GetDownloadInfo(i,&psDownloadProgressInfo);
                    if (!psDownloadProgressInfo) break;
                    if (nComPort == psDownloadProgressInfo->mnComPort)
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

    void CCTSEMultiunitDownloadView::OnResponseTimer( const WPARAM& wParam,const LPARAM& lParam )
    {
        //! 防止页面频繁刷新
        static S_DOWNLOAD_PROGRESS_INFO veDownloadProgressInfo[MAX_DOWNLOAD_THREAD_COUNT] = {E_IDLE_DOWNLOAD};

        if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == (int)wParam)
        {
            S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo = NULL;
            for (int i = 0; i < MAX_DOWNLOAD_THREAD_COUNT;i ++)
            {
                m_pTSEMDManageThread->GetDownloadInfo(i,&psDownloadProgressInfo);
                if (!psDownloadProgressInfo) continue;
                if (psDownloadProgressInfo->meDownloadStatus == veDownloadProgressInfo[i].meDownloadStatus && psDownloadProgressInfo->meDownloadStatus != E_BUSY_DOWNLOAD) continue;
                UpdateDownloadInfo(i,psDownloadProgressInfo);
                veDownloadProgressInfo[i].meDownloadStatus = psDownloadProgressInfo->meDownloadStatus;
                veDownloadProgressInfo[i].mnPos = psDownloadProgressInfo->mnPos;
            }
        }
    }

    void CCTSEMultiunitDownloadView::UpdateDownloadInfo(int nIndex,S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo)
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
            _stprintf_s(szComport,10,_T("COM%d"),psDownloadProgressInfo->mnComPort);
            m_pcomLabel[nIndex]->SetText(szComport);
            m_pcomLabel[nIndex]->SetEnabled(true);
            m_pstatusText[nIndex]->SetText(_T("BUSY"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),IDLE_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(PASS_PROCESS_COLOR);
            /*m_pDownLoadProgress[nIndex ]->SetValue( 1 );*/
            m_pAutoSwitchComCheckBox->SetEnabled(false);
            m_pResetAfterDownloadCheckBox->SetEnabled(false);
            psDownloadProgressInfo->mnPos += 1;
            pos = (psDownloadProgressInfo->mnPos) /*/ (1024*1024)*/;
            if (m_pDownLoadProgress[nIndex]->GetMaxValue() - 10 > pos)
                m_pDownLoadProgress[nIndex]->SetValue(pos);
            else if (pos >= m_pDownLoadProgress[nIndex]->GetMaxValue() - 10)
                m_pDownLoadProgress[nIndex]->SetValue(m_pDownLoadProgress[nIndex]->GetMaxValue() - 10);
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

    bool CCTSEMultiunitDownloadView::GetDuiContronlClass()
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
                //m_pDownLoadProgress[i]->SetMaxValue(PROCESS_MAX_VALUE);
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

    void CCTSEMultiunitDownloadView::LoadIniConfigFile()
    {
        m_bIsInitDownLoadPathOk = false;
        if (!mMultiunitDownloadConfig.LoadMultiunitDownloadConfig(mstrIniConfigPath)) return;
        //! check file is exits
        {
            std::vector<std::wstring> vRawprograms,vPatchs;
            std::wstring strTemp;

            JGW_ParserStrW(mpsTseMultiunitDownloadConfig->mstrPacths.c_str(),L",",vPatchs);
            JGW_ParserStrW(mpsTseMultiunitDownloadConfig->mstrRawprograms.c_str(),L",",vRawprograms);

            strTemp = mpsTseMultiunitDownloadConfig->mstrDownloadPath + L"\\" + mpsTseMultiunitDownloadConfig->mstrProgFirehoseLitembnFileName;
            if (!JGW_FileExistsToFilePath(strTemp.c_str()))
            {
                MessageBox(mMainHwnd,strTemp.c_str(),L"下载文件不存在",MB_ICONERROR);
                return ;
            }

            if (mpsTseMultiunitDownloadConfig->mbHaveProvisionXml)
            {
                strTemp = mpsTseMultiunitDownloadConfig->mstrDownloadPath + L"\\" + mpsTseMultiunitDownloadConfig->mstrProvisionXml;
                if (!JGW_FileExistsToFilePath(strTemp.c_str()))
                {
                    MessageBox(mMainHwnd,strTemp.c_str(),L"下载文件不存在",MB_ICONERROR);
                    return ;
                }
            }
            

            strTemp = mpsTseMultiunitDownloadConfig->mstrDownloadProcessPath;
            if (!JGW_FileExistsToFilePath(strTemp.c_str()))
            {
                MessageBox(mMainHwnd,strTemp.c_str(),L"下载文件不存在",MB_ICONERROR);
                return ;
            }

            for (std::vector<std::wstring>::iterator it = vRawprograms.begin();
                it != vRawprograms.end();
                ++ it)
            { 
                strTemp = mpsTseMultiunitDownloadConfig->mstrDownloadPath + L"\\" + it[0];
                if (!JGW_FileExistsToFilePath(strTemp.c_str()))
                {
                    MessageBox(mMainHwnd,strTemp.c_str(),L"下载文件不存在",MB_ICONERROR);
                    return ;
                }
            }

            for (std::vector<std::wstring>::iterator it = vPatchs.begin();
                it != vPatchs.end();
                ++ it)
            { 
                strTemp = mpsTseMultiunitDownloadConfig->mstrDownloadPath + L"\\" + it[0];
                if (!JGW_FileExistsToFilePath(strTemp.c_str()))
                {
                    MessageBox(mMainHwnd,strTemp.c_str(),L"下载文件不存在",MB_ICONERROR);
                    return ;
                }
            }
        }

        m_bIsInitDownLoadPathOk = true;
    }

    void CCTSEMultiunitDownloadView::UpdateUIShow()
    {
        m_pAutoSwitchComCheckBox->SetCheck(mpsTseMultiunitDownloadConfig->mbAutoSwitchDownloadPort);
        m_pResetAfterDownloadCheckBox->SetCheck(mpsTseMultiunitDownloadConfig->mbResetAfterDownload);
        m_pModeEdt->SetText(mpsTseMultiunitDownloadConfig->mstrModeName.c_str() );

        if (m_bIsInitDownLoadPathOk)
        {
            m_pDownPathLabel->SetText(mpsTseMultiunitDownloadConfig->mstrDownloadPath.c_str() );
            m_pProgrammerText->SetText(mpsTseMultiunitDownloadConfig->mstrProgFirehoseLitembnFileName.c_str());
            m_pRawprogramText->SetText(mpsTseMultiunitDownloadConfig->mstrRawprograms.c_str());
            m_pPatchText->SetText(mpsTseMultiunitDownloadConfig->mstrPacths.c_str() );
            for( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++ ) m_pDownLoadProgress[i]->SetMaxValue(mpsTseMultiunitDownloadConfig->mnDownloadTimeSec/*100 * 100*/);
            m_pProgrammerText->SetTipValue(mpsTseMultiunitDownloadConfig->mstrProgFirehoseLitembnFileName.c_str());
            m_pRawprogramText->SetTipValue(mpsTseMultiunitDownloadConfig->mstrRawprograms.c_str());
            m_pPatchText->SetTipValue(mpsTseMultiunitDownloadConfig->mstrPacths.c_str());
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

    void CCTSEMultiunitDownloadView::OnChangeDownloadPathChooseBtn()
    {
        //! 选择ProgEmmcFireHose
        { //! prog_ufs_firehose_8996_ddr.elf
            std::wstring strProgEmmcFirehoseLitembnFilePath,strFile;
            if(!JGW_ChooseFile(strProgEmmcFirehoseLitembnFilePath,L"Programmer File(prog*firehose*.*)\0prog*firehose*.*\0Programmer File(prog_ufs_firehose*.elf)\0prog_ufs_firehose*.elf\0Programmer File(prog_emmc_firehose*.mbn)\0prog_emmc_firehose*.mbn\0all file(*.*)\0*.*\0\0",mMainHwnd))  return ;

            mpsTseMultiunitDownloadConfig->mstrDownloadPath = JGW_GetPathOfFile(strProgEmmcFirehoseLitembnFilePath.c_str());
            mpsTseMultiunitDownloadConfig->mstrProgFirehoseLitembnFileName = JGW_GetFileNameToFilePath(strProgEmmcFirehoseLitembnFilePath.c_str());
            m_pProgrammerText->SetText(mpsTseMultiunitDownloadConfig->mstrProgFirehoseLitembnFileName.c_str());
            m_pDownPathLabel->SetText(mpsTseMultiunitDownloadConfig->mstrDownloadPath.c_str());
        }
        //! provision xml
        {
            if (mpsTseMultiunitDownloadConfig->mbHaveProvisionXml)
            {
                std::vector<std::wstring> _vProvision;
                if(!JGW_ChooseMultiSelectFileNames(_vProvision,L"provision File(provision*.xml)\0provision*.xml\0all file(*.*)\0*.*\0\0",mMainHwnd,OFN_EXPLORER,mpsTseMultiunitDownloadConfig->mstrDownloadPath.size()?mpsTseMultiunitDownloadConfig->mstrDownloadPath.c_str():L"c:\\"))  return ;
                mpsTseMultiunitDownloadConfig->mstrProvisionXml = _vProvision.at(0);
            }    
        }
        //! rawprogram and patch
        {
            std::vector<std::wstring> _vRawProgram;
            std::vector<std::wstring> _vPatch;

            JGW_ChooseMultiSelectFileNames( _vRawProgram,L"rawprogram File(rawprogram*.xml)\0rawprogram*.xml\0all file(*.*)\0*.*\0\0",mMainHwnd ,OFN_EXPLORER | OFN_ALLOWMULTISELECT ,mpsTseMultiunitDownloadConfig->mstrDownloadPath.size()?mpsTseMultiunitDownloadConfig->mstrDownloadPath.c_str():L"c:\\" );

            JGW_ChooseMultiSelectFileNames( _vPatch,L"patch File(patch*.xml)\0patch*.xml\0all file(*.*)\0*.*\0\0",mMainHwnd ,OFN_EXPLORER | OFN_ALLOWMULTISELECT,mpsTseMultiunitDownloadConfig->mstrDownloadPath.size()?mpsTseMultiunitDownloadConfig->mstrDownloadPath.c_str():L"c:\\");
//             if( _vRawProgram.empty() || _vPatch.empty()/* || _vRawProgram.size() > 2 || _vPatch.size() > 2*/ || _vRawProgram.size() != _vPatch.size())
//             {
//                 MessageBox(mMainHwnd,L"下载配置文件有误",L"选择下载配置文件为空",MB_ICONERROR);
//                 return;
//             }

            mpsTseMultiunitDownloadConfig->mstrRawprograms = L"";
            mpsTseMultiunitDownloadConfig->mstrPacths = L"";
            for (size_t i = 0;i < _vRawProgram.size();i ++)
            {
                if (0 != i) mpsTseMultiunitDownloadConfig->mstrRawprograms += L",";
                mpsTseMultiunitDownloadConfig->mstrRawprograms += _vRawProgram.at(i);
            }
            for (size_t i = 0;i < _vPatch.size();i ++)
            {
                if (0 != i) mpsTseMultiunitDownloadConfig->mstrPacths += L",";
                mpsTseMultiunitDownloadConfig->mstrPacths += _vPatch.at(i);
            }

            m_pPatchText->SetText(mpsTseMultiunitDownloadConfig->mstrPacths.c_str());
            m_pRawprogramText->SetText(mpsTseMultiunitDownloadConfig->mstrRawprograms.c_str());
        }
        {
            mMultiunitDownloadConfig.SaveMultiunitDownloadConfig();

            LoadIniConfigFile();

            UpdateUIShow();

            StartMagagerThread();
        }
    }

    void CCTSEMultiunitDownloadView::StartMagagerThread()
    {
        if ( !m_bIsInitDownLoadPathOk || m_pTSEMDManageThread->IsThreadRun() ) return ;
        m_pTSEMDManageThread->InitThreadParam(mMainHwnd,mpsTseMultiunitDownloadConfig,&mqueue_buffer);
        m_pTSEMDManageThread->CreateMessageThread();
        Sleep(250);
    }
}