#include "StdAfx.h"
#include "CMultiDownloadFirmwareView.h"
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_PluginViewLib/LogShowDuidlg.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCMultiDownloadFirmwareView::CCMultiDownloadFirmwareView(void):m_bIsInitDownLoadPathOk(false),m_pSFManageThread(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"multi_download_fw_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }

    CCMultiDownloadFirmwareView::~CCMultiDownloadFirmwareView(void)
    {
    }

    void CCMultiDownloadFirmwareView::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        REMOVE_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,mpPluginPaintManagerUI->GetPaintWindow());
        KillTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
        //! KillTimer(mMainHwnd,WM_TIMER_SELECT_USB_DEVICE_INFO);
        if (m_pSFManageThread) delete m_pSFManageThread;
        m_pSFManageThread = NULL;
        while(!msDeviceMsgQueue.is_empty()) msDeviceMsgQueue.get()->Dec();

        if( mpsViewResolverParam->mpMapRegMsg ) mpsViewResolverParam->mpMapRegMsg->clear();
        if( mpsViewResolverParam->mpMainParentContainer ) mpsViewResolverParam->mpMainParentContainer->RemoveAll();
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }//! mpsViewResolverParam->mpMainParentContainer->SetEnabled(false);

    bool CCMultiDownloadFirmwareView::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        //! init
        {
            mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            m_pSFManageThread = new CSFManageThread(msPumaDownloadInfo.mstrDownloadPortSortConfigFilePath);
            mMainHwnd = mpPluginPaintManagerUI->GetPaintWindow();
            if ( !GetDuiContronlClass() ) return false;
        }
        //! 添加响应设备池消息
        {
            GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG( mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),guid,E_DIAG_MODE | E_DOWNLOADER_MODE);
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,&CCMultiDownloadFirmwareView::OnResponseDeviceMsg);
        }
        //! 添加消息响应
        {
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,
                WM_TIMER,&CCMultiDownloadFirmwareView::OnResponseTimer);
            //!ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_DEVICECHANGE,&CCMultiDownloadFirmwareView::OnReportedPumaDeviceMsg)
            SetTimer(mMainHwnd,
                WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,
                UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }
        //! 路径获取
        {
            if (!mDownloadConfig.InitIniFilePath(JGW_GetCacheConfigFilePath(
                mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName
                ,CONFIG_DOWNLOAD_NAME).c_str())) return false;
            msPumaDownloadInfo.mstrDownloadPortSortConfigFilePath = JGW_GetConfigConfigFilePath(
                mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,
                SORT_PORT_CONFIG_NAME);
        }
        //! 加载配置已经开启下载管理线程
        {
            LoadDownloadConfigFile();
            m_pAutoSwitchComCheckBox->SetCheck(msMultiDFWParam.m_bAutoSwitchComport);
            mpValidateImageComCheckBox->SetCheck(msMultiDFWParam.mbValidateImage);
            mpFormatPDBComCheckBox->SetCheck(msMultiDFWParam.mbFormatPDBImage);
            UpdateUIShow();
            StartMagagerThread();
        }

        return true;
    }


    void CCMultiDownloadFirmwareView::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if ( pNotifyMsg->sType == DUI_MSGTYPE_CLICK )
        {
            if (m_pClearSortBtn == pNotifyMsg->pSender)
            {
                if(m_pSFManageThread->ClearSortInfo())
                {
                    CloseSubWndPlugin();
                }		
            }
            else if (m_pChooseImageBtn == pNotifyMsg->pSender)
            {
                if( m_pSFManageThread->IsStartDownload() ) 
                {
                    MessageBox(mMainHwnd,GetString(E_CPE_DOWNLOAD_NOT_ALLOW_CHANGE_CONFIG),GetString(E_PROMPT_TYPE),MB_ICONERROR);
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
            if (m_pAutoSwitchComCheckBox == pNotifyMsg->pSender)
            {                  
                msMultiDFWParam.m_bAutoSwitchComport = m_pAutoSwitchComCheckBox->GetCheck();    
                mDownloadConfig.SetIniKeyValue(L"MAIN",L"AutoSwitchComport",msMultiDFWParam.m_bAutoSwitchComport);
            }
            else if ( mpValidateImageComCheckBox == pNotifyMsg->pSender  )
            {
                msMultiDFWParam.mbValidateImage = mpValidateImageComCheckBox->GetCheck();
                msPumaDownloadInfo.mbValidateImageDataInfo = msMultiDFWParam.mbValidateImage;
                mDownloadConfig.SetIniKeyValue(L"MAIN",L"ValidateFWImage",msMultiDFWParam.mbValidateImage);
                LoadDownloadConfigFile();
                UpdateUIShow();
            }
            else if (mpFormatPDBComCheckBox == pNotifyMsg->pSender)
            {
                msMultiDFWParam.mbFormatPDBImage = mpFormatPDBComCheckBox->GetCheck();    
                msPumaDownloadInfo.mbFormatPDBImage = msMultiDFWParam.mbFormatPDBImage;
                mDownloadConfig.SetIniKeyValue(L"MAIN",L"FormatPDB",msMultiDFWParam.mbFormatPDBImage);
                if (msPumaDownloadInfo.mbFormatPDBImage)
                {
                    msPumaDownloadInfo.mvPumaEraseImageDataInfo = &mPumaImageFmtData.GetPumaEraseAllImageDataInfo();
                }
                else
                {
                    msPumaDownloadInfo.mvPumaEraseImageDataInfo = &mPumaImageFmtData.GetPumaEraseImageDataInfo();
                }
            }
        }
    }

    bool CCMultiDownloadFirmwareView::GetDuiContronlClass()
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
        mpValidateImageComCheckBox = static_cast<CCheckBoxUI*>(mpPluginPaintManagerUI->FindControl(L"validateImageComCheckBox"));
        m_pAutoSwitchComCheckBox = static_cast<CCheckBoxUI*>(mpPluginPaintManagerUI->FindControl(L"autoSwitchComCheckBox"));
        mpFormatPDBComCheckBox = static_cast<CCheckBoxUI*>(mpPluginPaintManagerUI->FindControl(L"FormatPDBComCheckBox"));
        m_pDownImagePathLabel = static_cast<CLabelUI*>(mpPluginPaintManagerUI->FindControl(L"downPathLabel"));
        m_pChooseImageBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"chooseBtn"));
        m_pClearSortBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"clearBtn"));
        bSuccess &= (NULL != mpValidateImageComCheckBox);
        bSuccess &= (NULL != mpFormatPDBComCheckBox);
        bSuccess &= (NULL != m_pAutoSwitchComCheckBox);
        bSuccess &= (NULL != m_pDownImagePathLabel);
        bSuccess &= (NULL != m_pChooseImageBtn);
        bSuccess &= (NULL != m_pClearSortBtn);
        if( !bSuccess ) ::MessageBox( mpPluginPaintManagerUI->GetPaintWindow(),L"Lost skin control",L"error",MB_ICONERROR);

        return bSuccess;
    }

    void CCMultiDownloadFirmwareView::OnResponseTimer( const WPARAM& wParam,const LPARAM& lParam )
    {
        //! 防止页面频繁刷新
        static S_DOWNLOAD_PROGRESS_INFO veDownloadProgressInfo[DOWNLOAD_THREAD_MAX] = {E_IDLE_DOWNLOAD};

        if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == (int)wParam)
        {
            const S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo = NULL;
            for (int i = 0; i < DOWNLOAD_THREAD_MAX;i ++)
            {
                m_pSFManageThread->GetDownloadInfo(i,&psDownloadProgressInfo);
                if (!psDownloadProgressInfo) continue;
                //! 下载状态一致以及发送数据量相同
                if (psDownloadProgressInfo->meDownloadStatus == veDownloadProgressInfo[i].meDownloadStatus && psDownloadProgressInfo->mdw64SendCounts == veDownloadProgressInfo->mdw64SendCounts) continue;
                UpdateDownloadInfo(i,psDownloadProgressInfo);
                veDownloadProgressInfo[i].meDownloadStatus = psDownloadProgressInfo->meDownloadStatus;
                veDownloadProgressInfo[i].mdw64SendCounts = psDownloadProgressInfo->mdw64SendCounts;
            }
        }
        /*else if (WM_TIMER_SELECT_USB_DEVICE_INFO == (int)wParam)
        {
          //!  OnReportedPumaDeviceMsg(wParam,lParam);
        }*/
    }

    void CCMultiDownloadFirmwareView::UpdateDownloadInfo(int nIndex,const S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo)
    {
        TCHAR szComport[10] = {0};
        int pos = 0;
        switch (psDownloadProgressInfo->meDownloadStatus)
        {
        case E_IDLE_DOWNLOAD:
            m_pcomLabel[nIndex]->SetEnabled(false);
            m_pstatusText[nIndex]->SetText(_T("IDLE"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),L"#FF008080");
            m_pDownLoadProgress[nIndex]->SetForeImage(PASS_PROCESS_COLOR);
            m_pDownLoadProgress[nIndex]->SetValue(0);
            break;
        case E_BUSY_DOWNLOAD:
            m_pcomLabel[nIndex]->SetText(JGW_A2W(psDownloadProgressInfo->mstrComPort).c_str());
            m_pcomLabel[nIndex]->SetEnabled(true);
            m_pstatusText[nIndex]->SetText(_T("BUSY"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),IDLE_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(PASS_PROCESS_COLOR);
            /*m_pDownLoadProgress[nIndex ]->SetValue( 1 );*/
            m_pAutoSwitchComCheckBox->SetEnabled(false);
            mpValidateImageComCheckBox->SetEnabled(false);
            m_pChooseImageBtn->SetEnabled(false);
            m_pClearSortBtn->SetEnabled(false);
            pos = (int)psDownloadProgressInfo->mdw64SendCounts / (1024);
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

    void CCMultiDownloadFirmwareView::OnResponseDeviceMsg( const WPARAM& wParam,const LPARAM& lParam )
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
            if (msMultiDFWParam.m_bAutoSwitchComport ||  E_DOWNLOADER_MODE == lpdevicemsg->m_nDeviceMode)
            {
                msDeviceMsgQueue.put(lpdevicemsg);
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
    //! 
    void CCMultiDownloadFirmwareView::OnReportedPumaDeviceMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
#if 0
        std::vector<std::wstring> vstrPidVid;
        bool bNew = true,bInsert = false;
        GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
        TCHAR pDevBuf[512] = {0};
        PDEV_BROADCAST_DEVICEINTERFACE pDev = (PDEV_BROADCAST_DEVICEINTERFACE)pDevBuf;
        pDev->dbcc_classguid = guid;
        pDev->dbcc_devicetype  = DBT_DEVTYP_DEVICEINTERFACE;
        pDev->dbcc_size = 512;
        JGW_GetUsbEnumPidVidVectotToServiceName(_T(PUMA_USB_SERVIER),vstrPidVid);
        //! 检查当前PID VID是否新增
        for (std::vector<std::wstring>::iterator it = vstrPidVid.begin();
            it != vstrPidVid.end();
            ++ it)
        {
            bNew = true;
            for (std::vector<std::wstring>::iterator itstr = mvstrExitsPidVid.begin();
                itstr != mvstrExitsPidVid.end();
                ++ itstr)
            {
                //! 相等 则说明当前PID VID不是新增
                if (NULL == it->compare(itstr[0])) { bNew = false; break;}
            }
            if (bNew)
            {
                wcscpy_s(pDev->dbcc_name,260,it->c_str());
                mvstrExitsPidVid.push_back(it->c_str());
                ::SendMessage(mpsViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_DEVICECHANGE,DBT_DEVICEARRIVAL,LPARAM(pDev));         
            }
        }
        //! 删除已经移除的USB信息
        std::vector<std::wstring>::iterator itExitsPidVid = mvstrExitsPidVid.begin();
        while (itExitsPidVid != mvstrExitsPidVid.end())
        {
            bInsert = false;
            for (std::vector<std::wstring>::iterator itstr = vstrPidVid.begin();
                itstr != vstrPidVid.end();
                ++ itstr)
            {
                //! 相等，则说明当前USB处于插入状态
                if (NULL == itExitsPidVid->compare(itstr[0])) { bInsert = true; break;}
            }
            if (!bInsert)
            {
                wcscpy_s(pDev->dbcc_name,260,itExitsPidVid->c_str());
                itExitsPidVid = mvstrExitsPidVid.erase(itExitsPidVid);
                ::SendMessage(mpsViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_DEVICECHANGE,DBT_DEVICEREMOVECOMPLETE,LPARAM(pDev));              
                continue;
            }
            itExitsPidVid ++;
        }
#endif
    }

    void CCMultiDownloadFirmwareView::LoadDownloadConfigFile()
    {
        std::wstring strTemp,strDownloadPath;
        msMultiDFWParam.m_bAutoSwitchComport = false;
        msMultiDFWParam.mstrImageFilePath = "";
        msMultiDFWParam.mbValidateImage = false;
        msMultiDFWParam.mbFormatPDBImage = false;
        m_bIsInitDownLoadPathOk = false;
        //! 加载下载配置文件
        {
            //! 获取下载路径 区分相对路径和绝对路径
            strDownloadPath = mDownloadConfig.GetIniKeyValue(L"MAIN",L"DownLoadPath");
            msMultiDFWParam.mstrImageFilePath = JGW_W2A(strDownloadPath);

            JGW_ReplaceCharW(strDownloadPath,L'/',L'\\');
            if(PathIsRelative(strDownloadPath.c_str()) )
            {
                strTemp = JGW_GetApplicationFolder();
                strTemp += strDownloadPath;
                msMultiDFWParam.mstrImageFilePath = JGW_W2A(strTemp);
            }

            msMultiDFWParam.m_bAutoSwitchComport = mDownloadConfig.GetIniKeyBoolValue(L"MAIN",L"AutoSwitchComport"); 
            msMultiDFWParam.mbValidateImage = mDownloadConfig.GetIniKeyBoolValue(L"MAIN",L"ValidateFWImage");  
            msMultiDFWParam.mbFormatPDBImage = mDownloadConfig.GetIniKeyBoolValue(L"MAIN",L"FormatPDB");  
        }
        {
            if (!mPumaImageFmtData.FmtPumaImageFile(msMultiDFWParam.mstrImageFilePath.c_str(),msMultiDFWParam.mbValidateImage))
            {
                MessageBox(mMainHwnd,L"Load Fireware Fail",GetString(E_PROMPT_TYPE),MB_ICONERROR);
                return ;
            }
             m_pDownImagePathLabel->SetText(JGW_A2W(msMultiDFWParam.mstrImageFilePath).c_str());
            msPumaDownloadInfo.mbValidateImageDataInfo = msMultiDFWParam.mbValidateImage;
            msPumaDownloadInfo.mbFormatPDBImage = msMultiDFWParam.mbFormatPDBImage;
            msPumaDownloadInfo.mnImageFileSize = mPumaImageFmtData.GetImageFileSize();
            msPumaDownloadInfo.mpPumaImageDataInfo = &mPumaImageFmtData.GetPumaImageDataInfo();
            msPumaDownloadInfo.mpPumaValidateImageDataInfo = &mPumaImageFmtData.GetPumaValidateImageDataInfo();
            if (msPumaDownloadInfo.mbFormatPDBImage)
            {
                msPumaDownloadInfo.mvPumaEraseImageDataInfo = &mPumaImageFmtData.GetPumaEraseAllImageDataInfo();
            }
            else
            {
                msPumaDownloadInfo.mvPumaEraseImageDataInfo = &mPumaImageFmtData.GetPumaEraseImageDataInfo();
            }
            /// msPumaDownloadInfo.mvPumaEraseImageDataInfo = &mPumaImageFmtData.GetPumaEraseImageDataInfo();
        }
        m_bIsInitDownLoadPathOk = true;
    }

    void CCMultiDownloadFirmwareView::UpdateUIShow()
    {
        if (m_bIsInitDownLoadPathOk)
        {
            m_pDownImagePathLabel->SetText(JGW_A2W(msMultiDFWParam.mstrImageFilePath).c_str());
            for( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++ )
                m_pDownLoadProgress[i]->SetMaxValue(mPumaImageFmtData.GetDownloadBufCounts());
        }
        else
        {
            m_pDownImagePathLabel->SetText(L"");
        }
    }

    void CCMultiDownloadFirmwareView::StartMagagerThread()
    {
        if ( !m_bIsInitDownLoadPathOk || m_pSFManageThread->IsThreadRun() ) return ;
        m_pSFManageThread->InitThreadParam(mMainHwnd,&msPumaDownloadInfo,&msDeviceMsgQueue);
        m_pSFManageThread->CreateMessageThread();
        Sleep(250);
        //! SetTimer(mMainHwnd,WM_TIMER_SELECT_USB_DEVICE_INFO,1000,NULL);
    }

    void CCMultiDownloadFirmwareView::OnChangeDownloadPathChooseBtn()
    {
        std::wstring strProgEmmcFirehoseLitembnFilePath,strFile;
        if( !JGW_ChooseFile(strProgEmmcFirehoseLitembnFilePath,L"Firmware Image(image*.bin)\0image*.bin\0Firmware File(*.bin)\0*.bin\0all file(*.*)\0*.*\0\0",mMainHwnd ) )  return ;
        m_pDownImagePathLabel->SetText(strProgEmmcFirehoseLitembnFilePath.c_str());
        std::wstring strDownloadPath = m_pDownImagePathLabel->GetText().GetData();
        JGW_ReplaceStringW(strDownloadPath,JGW_GetApplicationFolder(),L"");
        mDownloadConfig.SetIniKeyValue(L"MAIN",L"DownLoadPath",strDownloadPath.c_str());
        {
            LoadDownloadConfigFile();

            UpdateUIShow();

            StartMagagerThread();
        }
    }
}
