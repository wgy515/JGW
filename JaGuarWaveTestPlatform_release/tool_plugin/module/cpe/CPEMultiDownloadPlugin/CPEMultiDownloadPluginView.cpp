#include "StdAfx.h"
#include "CPEMultiDownloadPluginView.h"
#include <JGW_DevicePool/DevicePortDefine.h>
#include <JGW_PluginViewLib/LogShowDuidlg.h>
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include "CPEDownloadConfigDlg.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCPEMultiDownloadPluginView::CCPEMultiDownloadPluginView(void) : mMultiDownloadManageThread(mMultiDownloadSortConfig), m_bIsInitDownLoadPathOk(false),mbInsertedDevice(false)
    {
        msPluginSkin.mstrSkinXmlPath = L"ap_multi_upgrade_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;

        msMultiDownloadConfig.mstrCurrentWorkDir = JGW_GetApplicationFolder();
        msMultiDownloadConfig.mstrDownloadProcessPath = JGW_GetExecConfigFilePath(L"TSEAPSUpgradeTool.exe");

        msMultiDownloadConfig.mnDownloadTime = 100;
        msMultiDownloadConfig.mnDownloadTimeOut = 200;
        CCJGW_SyncSocket syncSocket;
        //msMultiDownloadConfig.mstrCPEIPAddr = L"192.168.1.22";
        std::string hostName;
        msMultiDownloadConfig.mstrTFtpdIPAddr = JGW_A2W(syncSocket.GetHostIP(hostName));

        msMultiDownloadConfig.mstrDownloadParam = L"TSE_SAHARA_FIREHOSE_IMAGE_PARAM=sbl1_flashless.mbn,TZ_MBN=tz.mbn,DEVCFG_MBN=devcfg_noac.mbn,RPM_MBN=rpm.mbn,BOOT_MBN=openwrt-ipq807x-u-boot.mbn,CDT_DDR4=cdt_DDR4.bin";
        //msMultiDownloadConfig.mstrSBL = L"sbl1_flashless.mbn";
        //msMultiDownloadConfig.mstrCDT = L"cdt-AP-HK09_512M16_DDR4.bin";
        //msMultiDownloadConfig.mstrTZ = L"tz.mbn";
        //msMultiDownloadConfig.mstrDEV = L"devcfg_noac.mbn";
        //msMultiDownloadConfig.mstrRPM = L"rpm.mbn";
        //msMultiDownloadConfig.mstrBOOT = L"openwrt-ipq807x-u-boot.mbn";

        for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            msMultiDownloadViewParam[i].meDownloadStatus = E_IDLE_DOWNLOAD;
            msMultiDownloadViewParam[i].mePrevDownloadStatus = E_SUCCESSFUL_DOWNLOAD;
            msMultiDownloadViewParam[i].mnProgressPos = 0;
            msMultiDownloadViewParam[i].mnIndex = i + 1;

            msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex = i + 1;
            msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort = i + 1;
            msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex = -1;

            msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort = i + 1;
            msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex = -1;
            //! 默认IP地址
            msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr = JGW_GetFormatWString(L"192.168.1.%d",i + 102);
        }
    }


    CCPEMultiDownloadPluginView::~CCPEMultiDownloadPluginView(void)
    {
    }

    void CCPEMultiDownloadPluginView::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        REMOVE_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,mpPluginPaintManagerUI->GetPaintWindow());
        KillTimer(mMainHwnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    bool CCPEMultiDownloadPluginView::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        {
            mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            mMainHwnd = mpPluginPaintManagerUI->GetPaintWindow();
            if ( !GetDuiContronlClass() ) return false;
        }
        //! 加载下载配置
        {
            JGW_KillProcessToName(L"tftpd32.exe");
            //! 加载排序配置文件
            std::wstring strConfig = JGW_GetConfigConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,SORT_PORT_CONFIG_NAME);
            mMultiDownloadSortConfig.InitDownloadPortSortConfig(&msMultiDownloadConfig,strConfig);

            std::wstring strDownloadConfig = JGW_GetCacheConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,L"down.ini");
            mMultiDownloadConfig.LoadMultiDownloadConfig(&msMultiDownloadConfig,strDownloadConfig);
            m_pDownImagePathLabel->SetText(msMultiDownloadConfig.mstrDownloadPath.c_str());

            msMultiDownloadConfig.mstrTSESuiteConfigFilePath = JGW_GetTSEConfigFilePath(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName);

            //! 初始化多路下载配置参数
            mMultiDownloadManageThread.OnInitManageThread(msMultiDownloadViewParam,&msMultiDownloadConfig,&mqueue_buffer);

            CheckDownloadPathConfig();
        }
        //! 下载状态信息监测
        {
            GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG( mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),guid,E_DIAG_MODE | E_DOWNLOADER_MODE);

            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,&CCPEMultiDownloadPluginView::OnResponseDeviceMsg);

            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,
                WM_TIMER,&CCPEMultiDownloadPluginView::OnResponseTimer);
            //! 每隔1秒中更新下载视图信息
            SetTimer(mMainHwnd,
                WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,
                UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }

        return true;
    }

    void CCPEMultiDownloadPluginView::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if (pNotifyMsg->sType == DUI_MSGTYPE_CLICK)
        {
            if (m_pChooseImageBtn == pNotifyMsg->pSender)
            {
                OnChangeDownloadPathChooseBtn();
            }
            else if (m_pClearSortBtn == pNotifyMsg->pSender)
            {
                for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
                {
                    msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex = i + 1;
                    msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort = i + 1;
                    msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex = -1;

                    msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort = i + 1;
                    msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex = -1;
                    //! 默认IP地址
                    msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr = JGW_GetFormatWString(L"192.168.1.%d",i + 102);
                }
                mMultiDownloadSortConfig.SaveMultiDownloadSortConfig();
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
                        std::unique_ptr<CLogShowDuidlg> ptrLogDlg( new CLogShowDuidlg(g_strDownloadDubugLog[i],g_strDownloadDubugLog[i],mMainHwnd) );
                        ptrLogDlg->DoModal();
                        return;
                    }
                }
            }
        }
    }

    void CCPEMultiDownloadPluginView::OnClickConfigBtn()
    {
        //! 如果设备已经插入则不允许修改配置
        if (mbInsertedDevice)
        {
            MessageBox(mMainHwnd,GetString(E_CPE_DOWNLOAD_NOT_ALLOW_CHANGE_CONFIG),GetString(E_PROMPT_TYPE),MB_ICONERROR);
            return ;
        }
        bool restore = m_bIsInitDownLoadPathOk;
        m_bIsInitDownLoadPathOk = false;
        S_MULTI_DOWNLOAD_CONFIG sMultiDownloadConfig;
        sMultiDownloadConfig.mnDownloadTime = msMultiDownloadConfig.mnDownloadTime;
        sMultiDownloadConfig.mnDownloadTimeOut = msMultiDownloadConfig.mnDownloadTimeOut;
        sMultiDownloadConfig.mstrDownloadPath = msMultiDownloadConfig.mstrDownloadPath;
        sMultiDownloadConfig.mstrTFtpdIPAddr = msMultiDownloadConfig.mstrTFtpdIPAddr;
        //sMultiDownloadConfig.mstrSBL = msMultiDownloadConfig.mstrSBL;
        //sMultiDownloadConfig.mstrCDT = msMultiDownloadConfig.mstrCDT;
        //sMultiDownloadConfig.mstrTZ = msMultiDownloadConfig.mstrTZ;
        //sMultiDownloadConfig.mstrDEV = msMultiDownloadConfig.mstrDEV;
        //sMultiDownloadConfig.mstrRPM = msMultiDownloadConfig.mstrRPM;
        //sMultiDownloadConfig.mstrBOOT = msMultiDownloadConfig.mstrBOOT;
        sMultiDownloadConfig.mstrDownloadParam = msMultiDownloadConfig.mstrDownloadParam;;
        sMultiDownloadConfig.mstrCurrentWorkDir = msMultiDownloadConfig.mstrCurrentWorkDir;
        sMultiDownloadConfig.mstrDownloadProcessPath = msMultiDownloadConfig.mstrDownloadProcessPath;
        sMultiDownloadConfig.mstrTSESuiteConfigFilePath = msMultiDownloadConfig.mstrTSESuiteConfigFilePath;
        for (int i = 0;i < DOWNLOAD_THREAD_MAX;i++)
        {
            sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort = msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort;
            sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex = msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex;
            sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex = msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex;
            sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort = msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort;
            sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex = msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex;
            sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr = msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr;
        }

        CCPEDownloadConfigDlg downloadConfigDlg(mMainHwnd,sMultiDownloadConfig);
        if (IDOK == downloadConfigDlg.DoModal(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID))
        {
            msMultiDownloadConfig.mnDownloadTime = sMultiDownloadConfig.mnDownloadTime;
            msMultiDownloadConfig.mnDownloadTimeOut = sMultiDownloadConfig.mnDownloadTimeOut;
            msMultiDownloadConfig.mstrDownloadPath = sMultiDownloadConfig.mstrDownloadPath;
            msMultiDownloadConfig.mstrTFtpdIPAddr = sMultiDownloadConfig.mstrTFtpdIPAddr;
            //msMultiDownloadConfig.mstrSBL = sMultiDownloadConfig.mstrSBL;
            //msMultiDownloadConfig.mstrCDT = sMultiDownloadConfig.mstrCDT;
            //msMultiDownloadConfig.mstrTZ = sMultiDownloadConfig.mstrTZ;
            //msMultiDownloadConfig.mstrDEV = sMultiDownloadConfig.mstrDEV;
            //msMultiDownloadConfig.mstrRPM = sMultiDownloadConfig.mstrRPM;
            //msMultiDownloadConfig.mstrBOOT = sMultiDownloadConfig.mstrBOOT;
            msMultiDownloadConfig.mstrDownloadParam = sMultiDownloadConfig.mstrDownloadParam;
            msMultiDownloadConfig.mstrCurrentWorkDir = sMultiDownloadConfig.mstrCurrentWorkDir;
            msMultiDownloadConfig.mstrDownloadProcessPath = sMultiDownloadConfig.mstrDownloadProcessPath;
            msMultiDownloadConfig.mstrTSESuiteConfigFilePath = sMultiDownloadConfig.mstrTSESuiteConfigFilePath;
            for (int i = 0;i < DOWNLOAD_THREAD_MAX;i++)
            {
                msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort = sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadComPort;
                msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex = sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDonwloadSortIndex;
                msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex = sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnDownLoadIndex;
                msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort = sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort;
                msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex = sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialSortIndex;
                msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr = sMultiDownloadConfig.msMultiDownloadSortConfigs[i].mstrCPEIPAddr;
            }
            SaveDownloadConfig();
            CheckDownloadPathConfig();
        }
        else
        {
            m_bIsInitDownLoadPathOk = restore;
        }
    }

    void CCPEMultiDownloadPluginView::SaveDownloadConfig()
    {
        mMultiDownloadConfig.SaveMultiDownloadConfig(&msMultiDownloadConfig);
        mMultiDownloadSortConfig.SaveMultiDownloadSortConfig();
    }

    void CCPEMultiDownloadPluginView::OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == (int)wParam)
        {
            for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
            {
                UpdateDownloadInfo(i);
            }
        }
    }

    void CCPEMultiDownloadPluginView::OnResponseDeviceMsg( const WPARAM& wParam,const LPARAM& lParam )
    {
        Log4WD(L"CGNetDownLoadViewDlg::OnResponseDeviceMsg");
        if( !wParam || !m_bIsInitDownLoadPathOk )return ;

        mbInsertedDevice = true;
        SDeviceMsg_Info* lpdevicemsg = (SDeviceMsg_Info*)wParam;
#ifdef _DEBUG
        OutputDebugString(lpdevicemsg->m_strDbcc_name.c_str());
        OutputDebugString(_T("\n"));
#endif
        //! 只处理9008下载模式请求
        if ( DBT_DEVICEARRIVAL == lpdevicemsg->m_nDeviceMsgType )
        {
            Log4WD_F(L"[OnDeviceChange]  DBT_DEVICEARRIVAL %s",lpdevicemsg->m_strDbcc_name.c_str());
            if (/*mpsTseMultiunitDownloadConfig->mbAutoSwitchDownloadPort || */ E_DOWNLOADER_MODE == lpdevicemsg->m_nDeviceMode)
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
                for ( int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
                {
                    if (nComPort == msMultiDownloadConfig.msMultiDownloadSortConfigs[i].mnSerialComPort)
                    {
                        m_pcomLabel[i]->SetEnabled(false);
                        //! m_pcomLabel[i]->SetText(msMultiDownloadConfig.msMultiDownloadSortConfigs[i].)
                        break;
                    }
                }
            }
        }

        lpdevicemsg->Dec();
        return ;
    }

    void CCPEMultiDownloadPluginView::UpdateDownloadInfo(int nIndex)
    {
        if (E_BUSY_DOWNLOAD != msMultiDownloadViewParam[nIndex].meDownloadStatus && msMultiDownloadViewParam[nIndex].meDownloadStatus == msMultiDownloadViewParam[nIndex].mePrevDownloadStatus)
        {
            return ;
        }
        std::wstring strComPort;
        switch (msMultiDownloadViewParam[nIndex].meDownloadStatus)
        {
        case E_IDLE_DOWNLOAD:
            msMultiDownloadViewParam[nIndex].mnProgressPos = 0;
            m_pcomLabel[nIndex]->SetEnabled(false);
            if (msMultiDownloadConfig.msMultiDownloadSortConfigs[nIndex].mnDonwloadComPort > 0)
            {
                JGW_FormatWString(strComPort,L"COM%d-COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[nIndex].mnDonwloadComPort,msMultiDownloadConfig.msMultiDownloadSortConfigs[nIndex].mnSerialComPort);
                m_pcomLabel[nIndex]->SetText(strComPort.c_str());
            }
            else
            {
                m_pcomLabel[nIndex]->SetText(L"");
            }
            break;
        case E_BUSY_DOWNLOAD:
            if (msMultiDownloadViewParam[nIndex].mnProgressPos < msMultiDownloadConfig.mnDownloadTime - 1) m_pDownLoadProgress[nIndex]->SetValue(++msMultiDownloadViewParam[nIndex].mnProgressPos);
            else m_pDownLoadProgress[nIndex]->SetValue(msMultiDownloadConfig.mnDownloadTime - 1);
            if (msMultiDownloadViewParam[nIndex].mnProgressPos > 1) break;
            //JGW_FormatWString(strComPort,L"COM%d",msMultiDownloadConfig.msMultiDownloadSortConfigs[nIndex].mnSerialComPort);
            //m_pcomLabel[nIndex]->SetText(strComPort.c_str());
            m_pcomLabel[nIndex]->SetEnabled(true);
            m_pstatusText[nIndex]->SetText(_T("BUSY"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),IDLE_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(PASS_PROCESS_COLOR);
            m_pChooseImageBtn->SetEnabled(false);
            m_pClearSortBtn->SetEnabled(false);
            m_pConfigLanBtn->SetEnabled(false);
            break;
        case E_ERROR_DOWNLOAD:
            msMultiDownloadViewParam[nIndex].mnProgressPos = 0;
            m_pstatusText[nIndex]->SetText(_T("FAIL"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),FAIL_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetForeImage(FAIL_PROCESS_COLOR);
            //! m_pDownLoadProgress[nIndex]->SetValue(m_pDownLoadProgress[nIndex]->GetValue() + 1);
            break;
        case E_SUCCESSFUL_DOWNLOAD:
            msMultiDownloadViewParam[nIndex].mnProgressPos = 0;
            m_pstatusText[nIndex]->SetText(_T("PASS"));
            m_pstatusText[nIndex]->SetAttribute(_T("bkcolor"),PASS_STATUS_DOWN);
            m_pDownLoadProgress[nIndex]->SetValue( m_pDownLoadProgress[nIndex]->GetMaxValue() );
            break;
        }
        msMultiDownloadViewParam[nIndex].mePrevDownloadStatus = msMultiDownloadViewParam[nIndex].meDownloadStatus;
    }


    bool CCPEMultiDownloadPluginView::GetDuiContronlClass()
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

    void CCPEMultiDownloadPluginView::CheckDownloadPathConfig()
    {
        m_bIsInitDownLoadPathOk = false;
        std::wstring strPath = JGW_RealativePathToAbsPath(msMultiDownloadConfig.mstrDownloadPath.c_str());
        msMultiDownloadConfig.mmapParams.clear();
        std::vector<std::wstring> vParam;
        std::wstring strKey,strValue,strDownloadFilePath;
        JGW_ParserStrW(msMultiDownloadConfig.mstrDownloadParam.c_str(),L",",vParam);
        for (size_t i = 0;i < vParam.size();i ++)
        {
            std::vector<std::wstring> vTemp;
            JGW_ParserStrW(vParam[i].c_str(),L"=",vTemp);
            if (2 != vTemp.size())
            {
                MessageBox(mMainHwnd,msMultiDownloadConfig.mstrDownloadParam.c_str(),L"Config Download File Param Error",MB_ICONERROR);
                m_pDownImagePathLabel->SetText(L"");
                return ;
            }
            strKey = vTemp[0];
            strValue = vTemp[1];
            JGW_EraseLastAndFristTwoCharsW(strKey);
            JGW_EraseLastAndFristChars(strKey,' ');
            JGW_EraseLastAndFristTwoCharsW(strValue);
            JGW_EraseLastAndFristChars(strValue,' ');
            strDownloadFilePath = JGW_RealPathToAbsPathBasePath(strValue.c_str(),strPath.c_str());

            if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(strDownloadFilePath.c_str()))
            {
                MessageBox(mMainHwnd,strDownloadFilePath.c_str(),L"Missing download file",MB_ICONERROR);
                m_pDownImagePathLabel->SetText(L"");
                return ;
            }
            msMultiDownloadConfig.mmapParams[strKey] = strValue;
        }
        /*if (!msMultiDownloadConfig.mstrSBL.empty())
        {
        downloadFilePath = strPath + L"\\" + msMultiDownloadConfig.mstrSBL;
        if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(downloadFilePath.c_str()))
        {
        MessageBox(mMainHwnd,downloadFilePath.c_str(),L"下载文件缺失",MB_ICONERROR);
        m_pDownImagePathLabel->SetText(L"");
        return ;
        }
        }

        if (!msMultiDownloadConfig.mstrBOOT.empty())
        {
        downloadFilePath = strPath + L"\\" + msMultiDownloadConfig.mstrBOOT;
        if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(downloadFilePath.c_str()))
        {
        MessageBox(mMainHwnd,downloadFilePath.c_str(),L"下载文件缺失",MB_ICONERROR);
        m_pDownImagePathLabel->SetText(L"");
        return ;
        }
        }

        if (!msMultiDownloadConfig.mstrCDT.empty())
        {
        downloadFilePath = strPath + L"\\" + msMultiDownloadConfig.mstrCDT;
        if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(downloadFilePath.c_str()))
        {
        MessageBox(mMainHwnd,downloadFilePath.c_str(),L"下载文件缺失",MB_ICONERROR);
        m_pDownImagePathLabel->SetText(L"");
        return ;
        }
        }

        if (!msMultiDownloadConfig.mstrDEV.empty())
        {
        downloadFilePath = strPath + L"\\" + msMultiDownloadConfig.mstrDEV;
        if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(downloadFilePath.c_str()))
        {
        MessageBox(mMainHwnd,downloadFilePath.c_str(),L"下载文件缺失",MB_ICONERROR);
        m_pDownImagePathLabel->SetText(L"");
        return ;
        }
        }

        if (!msMultiDownloadConfig.mstrRPM.empty())
        {
        downloadFilePath = strPath + L"\\" + msMultiDownloadConfig.mstrRPM;
        if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(downloadFilePath.c_str()))
        {
        MessageBox(mMainHwnd,downloadFilePath.c_str(),L"下载文件缺失",MB_ICONERROR);
        m_pDownImagePathLabel->SetText(L"");
        return ;
        }
        }

        if (!msMultiDownloadConfig.mstrTZ.empty())
        {
        downloadFilePath = strPath + L"\\" + msMultiDownloadConfig.mstrTZ;
        if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(downloadFilePath.c_str()))
        {
        MessageBox(mMainHwnd,downloadFilePath.c_str(),L"下载文件缺失",MB_ICONERROR);
        m_pDownImagePathLabel->SetText(L"");
        return ;
        }
        }*/
        //! => mstrDownloadPath;
        mMultiDownloadConfig.SettingTftp32BaseDirectory(strPath.c_str());

        m_bIsInitDownLoadPathOk = true;
        m_pDownImagePathLabel->SetText(strPath.c_str());

        if (!mMultiDownloadManageThread.IsThreadRun())
        {
            mMultiDownloadManageThread.CreateMessageThread();
            Sleep(250);
        }
    }

    void CCPEMultiDownloadPluginView::OnChangeDownloadPathChooseBtn()
    {
        //! 如果设备已经插入则不允许修改配置
        if (mbInsertedDevice)
        {
            MessageBox(mMainHwnd,GetString(E_CPE_DOWNLOAD_NOT_ALLOW_CHANGE_CONFIG),GetString(E_PROMPT_TYPE),MB_ICONERROR);
            return ;
        }

        std::wstring strFolder;
        if (JGW_ChooseFolder(strFolder,mMainHwnd))
        {
            msMultiDownloadConfig.mstrDownloadPath = strFolder;
            CheckDownloadPathConfig();
            mMultiDownloadConfig.SaveMultiDownloadConfig(&msMultiDownloadConfig);
        }
    }
}

