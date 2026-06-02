#include "StdAfx.h"
#include "CMultipleTestPluginDlg.h"
//#include <JGW_ReportDeviceInfo/CJGW_ReportDeviceInfoExport.h>
#include <JGW_DevicePool/DevicePortDefine.h>
#include <JGW_DeviceQueueModule/CJGW_DeviceQueueImpl.h>
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CMultipleTestConfigDlg.h"
#include <regex>
#include <JGW_FoundationFunc/jgw_process_manage.h>

namespace JGW
{
    CCMultipleTestPluginDlg::CCMultipleTestPluginDlg(void) : mpsViewResolverParam(NULL),mpPluginPaintManagerUI(NULL),mpConfigBtn(NULL),mpChooseBtn(NULL),mpMainMultipleVerticalLayout(NULL),mpBinaryImagePathLabel(NULL),mnThreadIndex(0),mcMultipleTestManageThread(mvsMultipleTestThreadParam),mbInitMultipleTestOk(false)
    {
        msPluginSkin.mstrSkinXmlPath = L"ipq_multi_upgrade_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        // 双COM端口排序，双端口排序+网口IP自动叠加，单COM端口排序，网口排序）
        msMultipleConfig.msDeviceProgramInfo.meDeviceQueueProgram = E_SINGLE_PORT_QUEUE_PROGRAM;
        // 第一个端口设备模式
        msMultipleConfig.msDeviceProgramInfo.meFristDeviceMode = E_DOWNLOADER_MODE;
        // 第二个端口设备模式
        msMultipleConfig.msDeviceProgramInfo.meSecondDeviceMode = E_DEVICE_UNKNOWMODE;
        // 第二个端口设备模式
        msMultipleConfig.msDeviceProgramInfo.meThreeDeviceMode = E_DEVICE_UNKNOWMODE;
        //! 下载镜像是否是目录
        msMultipleConfig.mbBinaryImageIsFolder = false;
        //! 下载或者升级操作是否是文件名
        //bool mbDownloadUpgradeTSEParamIsFileName;
        //! 是否保存测试LOG
        msMultipleConfig.mbSaveTestLog = false;
        //! 多路测试几行
        msMultipleConfig.mnMultipleTestRow = 2;
        //！每行多少列
        msMultipleConfig.mnMultipleTestCol = 4;
        //! 下载时间
        msMultipleConfig.mnTestTimeSec = 200;
        //! 下载出现超时时间
        msMultipleConfig.mnTestTimeOutSec = 600;
        //！保存测试log目录
        //msMultipleConfig.mstrSaveTestLogFolder;
        //! 下载镜像文件路径 => 路径或者目录
        msMultipleConfig.mstrBinaryImagePath = L"";
        //! TFTPD32 IP地址
        msMultipleConfig.mstrTFTPDIPAddr = L"192.168.1.5";
        //! TFTPD32配置文件
        //msMultipleConfig.mstrTftpd32ConfigIni = L"Tftpd32\\tftpd32.ini";
        //! 升级或者下载版本
        msMultipleConfig.mstrUpgradeVersion = L"";
        //! TSE 测试程序文件当前工作目录
        msMultipleConfig.mstrTSEExecApplicationCurrentWorkDir = JGW_GetApplicationFolder();
        //! TSE 测试程序文件路径
        msMultipleConfig.mstrTSEExecApplicationFilePath = JGW_GetFormatWString(L"%s%s",JGW_GetApplicationFolder(),L"TSEAPSUpgradeTool.exe");
        //! 网卡设备相关配置
        msMultipleConfig.msNetCardConfig.mstrHostIPAddr = L"192.168.1.1";
        msMultipleConfig.msNetCardConfig.mstrHostIPAddrAnsi = JGW_W2A(msMultipleConfig.msNetCardConfig.mstrHostIPAddr);
        msMultipleConfig.msDeviceProgramInfo.mstrNetCardHostIPSeg = msMultipleConfig.msNetCardConfig.mstrHostIPAddr.substr(0,msMultipleConfig.msNetCardConfig.mstrHostIPAddr.find_last_of('.') + 1);
    }


    CCMultipleTestPluginDlg::~CCMultipleTestPluginDlg(void)
    {
    }

    bool CCMultipleTestPluginDlg::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        // 初始化UI
        {
            mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            mpMainMultipleVerticalLayout = static_cast<CVerticalLayoutUI*>(mpPluginPaintManagerUI->FindControl(L"MainMultipleVerticalLayout"));
            mpChooseBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"chooseBtn"));
            mpConfigBtn = static_cast<CButtonUI*>(mpPluginPaintManagerUI->FindControl(L"configBtn"));
            mpBinaryImagePathLabel = static_cast<CLabelUI*>(mpPluginPaintManagerUI->FindControl(L"downPathLabel"));

            if (NULL == mpMainMultipleVerticalLayout || NULL == mpChooseBtn || NULL == mpConfigBtn || NULL == mpBinaryImagePathLabel || !mcMultipleTestConfig.LoadMultipleTestConfig(mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,msMultipleConfig)) return false;  

            mpBinaryImagePathLabel->SetText(msMultipleConfig.mstrBinaryImagePath.c_str());

            JGW_KillProcessToName(L"tftpd32.exe");
        }
        //! 添加响应设备池消息
        {
            // COM 端口
            GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG( mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),guid,E_DIAG_MODE | E_DOWNLOADER_MODE);
            // ADB 端口
            GUID adb_guid = GUID_ADB_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),adb_guid,E_ADB_MODE);
            // MODEM 端口
            GUID modem_guid = GUID_MODEM_INTERFACE_USB_DEVICE;
            REG_DEVICEPOOL_MSG(mpsViewResolverParam->mpsCommonWndMsgInfo->muiDevicePoolThreadID,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,mpPluginPaintManagerUI->GetPaintWindow(),modem_guid,E_MODEM_MODE);
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,&CCMultipleTestPluginDlg::OnResponseDeviceMsg);
            // NETCARD 端口
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_HWND_NETCARD_CHANGE_MSG,&CCMultipleTestPluginDlg::OnResponseNetCardChangeMSG);
        }
        //! 初始化UI设计
        {
            InitMultipleTestView();
            mcMultipleTestManageThread.CreateMessageThread(0,0,true);
            // 定时器更新UI视图
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_TIMER,&CCMultipleTestPluginDlg::OnResponseTimer);
            SetTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }
        {
            //! 初始化设备排序模块
            if (!InitializationDeviceQueueModule(mpPluginPaintManagerUI->GetPaintWindow(),mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName,msMultipleConfig.msDeviceProgramInfo)) return false;
            //! 添加对设备排序事件信息函数回调
            AddChangeDeviceQueueMsgEvent(&CCMultipleTestPluginDlg::OnResponseDeviceQueueMsg,this);
        }

        {
            //! 检查多路测试配置是否初始化OK
            CheckMultipleTestConfigIsVaild();
            //! 检查多路测试初始化是否成功
            if (!mbInitMultipleTestOk) CheckInitMultipleTestIsOk(); 
        }

        return true;
    }

    void CCMultipleTestPluginDlg::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if (pNotifyMsg->sType == DUI_MSGTYPE_CLICK)
        {
            if (mpChooseBtn == pNotifyMsg->pSender)
            {
                OnChangeDownloadPathChooseBtn();
            }
            else if (mpConfigBtn == pNotifyMsg->pSender)
            {
                OnClickConfigBtn();
            }
        }
    }

    void CCMultipleTestPluginDlg::CloseSubWndPlugin()
    {
        if (IsCloseSubWnd()) return;
        //! 移除对设备排序事件信息函数回调
        RemoveChangeDeviceQueueMsgEvent(&CCMultipleTestPluginDlg::OnResponseDeviceQueueMsg);
        UnInitMultipleTestView();
        mcMultipleTestManageThread.StopTSEMultipleTestManageThread();
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    void CCMultipleTestPluginDlg::OnResponseDeviceMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        HandleDeviceQueueMsg(E_COM_MSG_TYPE,wParam,lParam);
    }

    void CCMultipleTestPluginDlg::OnResponseNetCardChangeMSG(const WPARAM& wParam,const LPARAM& lParam)
    {
        HandleDeviceQueueMsg(E_NETCARD_MSG_TYPE,wParam,lParam);
    }

    void CCMultipleTestPluginDlg::UnInitMultipleTestView()
    {
        for (size_t index = 0;index < mvsMultipleViewInfos.size();index++)
        {
            mvsMultipleViewInfos[index].mpStartButton->OnNotify -= MakeDelegate(this,&CCMultipleTestPluginDlg::OnStartMsgBtnClick,_T("click"));
        }
        mpMainMultipleVerticalLayout->RemoveAll();
        mvsMultipleViewInfos.clear();
        mnThreadIndex = 0;
    }

    void CCMultipleTestPluginDlg::InitMultipleTestView()
    {
        //! 卸载多路测试视图
        UnInitMultipleTestView();
        //! 初始化多路测试视图
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
            if (NULL == pHorizontalLayoutUI) continue;
            for (int y = 0;y < msMultipleConfig.mnMultipleTestCol;y++)
            {
                CDialogBuilder builder1;
                CVerticalLayoutUI* pListContainerItem = (CVerticalLayoutUI*)(builder1.Create(_T("multiple_test_single_item.xml"),(UINT)0,NULL,mpPluginPaintManagerUI));
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
                    sTSEMultipleTestThreadParam.mbIsNewDevicePlugIn = false;
                    sTSEMultipleTestThreadParam.mpMultipleTestThread = NULL;
                    sMultipleViewInfo.mnViewIndex = mnThreadIndex;
                    sMultipleViewInfo.mpIndexLabel = (CLabelUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(0))->GetItemAt(0);
                    sMultipleViewInfo.mpDownloadProcess = (CProgressUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(0))->GetItemAt(2);
                    //sMultipleViewInfo.mpStartButton = (CButtonUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(0))->GetItemAt(4);
                    sMultipleViewInfo.mpStatusHorizontal = (CHorizontalLayoutUI*)pListContainerItem->GetItemAt(1);
                    sMultipleViewInfo.mpStatusLabel = (CLabelUI*)sMultipleViewInfo.mpStatusHorizontal->GetItemAt(0);
                    //sMultipleViewInfo.mpDeviceDesTextUI = (CTextUI*)sMultipleViewInfo.mpStatusHorizontal->GetItemAt(1);
                    sMultipleViewInfo.mpStartButton = (CButtonUI*)sMultipleViewInfo.mpStatusHorizontal->GetItemAt(1);
                    sMultipleViewInfo.mpLogRickEdit = (CRichEditUI*)((CHorizontalLayoutUI*)pListContainerItem->GetItemAt(2))->GetItemAt(0);
                    sMultipleViewInfo.mpIndexLabel->SetText(JGW_GetFormatWString(L"%d",mvsMultipleViewInfos.size() + 1).c_str());
                    sMultipleViewInfo.mnProcessPos = 0;
                    sMultipleViewInfo.mpDownloadProcess->SetValue(0);
                    sMultipleViewInfo.mpDownloadProcess->SetForeImage(IDLE_PROCESS_IMG);

                    sMultipleViewInfo.mpStartButton->SetDate((LPVOID)mvsMultipleViewInfos.size());
                    sMultipleViewInfo.mpStartButton->OnNotify += MakeDelegate(this,&CCMultipleTestPluginDlg::OnStartMsgBtnClick,_T("click"));
                    UpdateMultipleSingleView(sTSEMultipleTestThreadParam,sMultipleViewInfo);
                    pHorizontalLayoutUI->Add(pListContainerItem);
                    mvsMultipleTestThreadParam.push_back(sTSEMultipleTestThreadParam);
                    mvsMultipleViewInfos.push_back(sMultipleViewInfo);
                } 
            }
            mpMainMultipleVerticalLayout->Add(pHorizontalLayoutUI);
        }
    }

    void CCMultipleTestPluginDlg::OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == (int)wParam && mbInitMultipleTestOk)
        {
            for (size_t i = 0;i < mvsMultipleViewInfos.size();i ++)
            {
                UpdateMultipleSingleView(mvsMultipleTestThreadParam[i],mvsMultipleViewInfos[i]);
            }
        }
    }

    bool CCMultipleTestPluginDlg::OnStartMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
    {
        // 检查是否初始化多路测试成功
        if (!mbInitMultipleTestOk) 
        {
            CheckInitMultipleTestIsOk(); 
            return false;
        }
        // 获取当前点击的开始按钮索引
        size_t nCurrentTestIndex = (size_t)pTNotifyUI->pSender->GetDate();
        if (nCurrentTestIndex >= mvsMultipleViewInfos.size() || nCurrentTestIndex < 0) 
        {
            return false;
        }
        // 初始化开始测试信息
        mvsMultipleViewInfos[nCurrentTestIndex].mpStartButton->SetEnabled(false);
        mvsMultipleTestThreadParam[nCurrentTestIndex].mbIsNewDevicePlugIn = true;

        return true;
    }

    void CCMultipleTestPluginDlg::OnClickConfigBtn()
    {
        size_t row = msMultipleConfig.mnMultipleTestRow,col = msMultipleConfig.mnMultipleTestCol;
        bool bBinaryImageIsFolder = msMultipleConfig.mbBinaryImageIsFolder;

        CCMultipleTestConfigDlg multipleTestConfigDlg(mpPluginPaintManagerUI->GetPaintWindow(),&msMultipleConfig);
        if (IDOK == multipleTestConfigDlg.DoModal()) 
        {
            //! 文件或者目录有修改重新加载
            if (bBinaryImageIsFolder != msMultipleConfig.mbBinaryImageIsFolder)
            {
                mbInitMultipleTestOk = false;
                mpBinaryImagePathLabel->SetText(L"");
            }
            // 多路测试行或者列有修改重新初始化多路测试视图
            if (msMultipleConfig.mnMultipleTestRow != row || msMultipleConfig.mnMultipleTestCol != col)
            {
                InitMultipleTestView();

            }
            mcMultipleTestConfig.SaveMultipleTestConfig(msMultipleConfig);
            CheckMultipleTestConfigIsVaild();
        }
    }

    void CCMultipleTestPluginDlg::OnChangeDownloadPathChooseBtn()
    {
        if (!msMultipleConfig.mbBinaryImageIsFolder)
        {
            std::wstring strProgEmmcFirehoseLitembnFilePath,strFile;
            if(!JGW_ChooseFile(strProgEmmcFirehoseLitembnFilePath,L"update*.bin;*.img\0update*.bin;*.img\0*.bin\0*.bin\0all file(*.*)\0*.*\0\0",mpPluginPaintManagerUI->GetPaintWindow()) )  return ;
            mpBinaryImagePathLabel->SetText(strProgEmmcFirehoseLitembnFilePath.c_str());
            JGW_ReplaceStringW(strProgEmmcFirehoseLitembnFilePath,JGW_GetApplicationFolder(),L"");
            mcMultipleTestConfig.GetConfigIniInstance().SetIniKeyValue(L"MAIN",L"BinaryImagePath",strProgEmmcFirehoseLitembnFilePath.c_str());
            msMultipleConfig.mstrBinaryImagePath = strProgEmmcFirehoseLitembnFilePath;
        }
        else
        {
            std::wstring strFolder;
            if (!JGW_ChooseFolder(strFolder,mpPluginPaintManagerUI->GetPaintWindow())) return;
            mpBinaryImagePathLabel->SetText(strFolder.c_str());
            mcMultipleTestConfig.GetConfigIniInstance().SetIniKeyValue(L"MAIN",L"BinaryImagePath",strFolder.c_str());
            msMultipleConfig.mstrBinaryImagePath = strFolder;
        }
        //! 检查当前配置是否有效
        CheckMultipleTestConfigIsVaild();
    }

    void CCMultipleTestPluginDlg::UpdateMultipleSingleView(S_TSE_MULTIPLE_TEST_THREAD_PARAM& sThreadParam,S_MULTIPLE_VIEW_INFO& sMultipleViewInfo)
    {
        switch (sThreadParam.meTSEMPTestStatus)
        {
        case E_TSEMP_IDLE:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                sMultipleViewInfo.mpStartButton->SetText(L"Start");
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),IDLE_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),IDLE_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusLabel->SetText(L"IDLE");
            }
            break;
        case E_TSEMP_INST:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                sMultipleViewInfo.mpStartButton->SetText(L"Start");
                sMultipleViewInfo.mpStatusLabel->SetText(L"INST"); 
            }
            break;
        case E_TSEMP_BUSY:
            //! 更新Process pos
            if (sMultipleViewInfo.mnProcessPos++ + 20 >= (int)msMultipleConfig.mnTestTimeSec)
            {
                sMultipleViewInfo.mpDownloadProcess->SetValue(msMultipleConfig.mnTestTimeOutSec - 20);
            }
            else
            {
                sMultipleViewInfo.mpDownloadProcess->SetValue(sMultipleViewInfo.mnProcessPos);
            }
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                if (mpConfigBtn->IsEnabled()) mpConfigBtn->SetEnabled(false);
                if (mpChooseBtn->IsEnabled()) mpChooseBtn->SetEnabled(false);
                if (sMultipleViewInfo.mpDownloadProcess->GetMaxValue() != msMultipleConfig.mnTestTimeSec) sMultipleViewInfo.mpDownloadProcess->SetMaxValue(msMultipleConfig.mnTestTimeSec);

                sMultipleViewInfo.mpStartButton->SetText(sThreadParam.msDeviceQueueInfo.mstrDes.c_str());
                sMultipleViewInfo.mpStartButton->SetEnabled(false);
                sMultipleViewInfo.mnProcessPos = 0;
                //sThreadParam.mstrTSEMultipleTestLog.ClearText();
                sMultipleViewInfo.mpLogRickEdit->SetText(L"");
                //! JGW_GetFormatWString(L"file='%s' corner='5,5,5,5';",BUSY_PROCESS_IMG).c_str()
                sMultipleViewInfo.mpDownloadProcess->SetForeImage(BUSY_PROCESS_IMG_STRING);
                sMultipleViewInfo.mpStatusLabel->SetText(L"BUSY");
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),BUSY_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),BUSY_STATUS_BKCOLOR);
            }
            break;
        case E_TSEMP_FAIL:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                sMultipleViewInfo.mpStartButton->SetText(L"Start");
                sMultipleViewInfo.mpStartButton->SetEnabled(true);

                sMultipleViewInfo.mpStatusLabel->SetText(L"FAIL");
                sMultipleViewInfo.mpDownloadProcess->SetForeImage(FAIL_PROCESS_IMG);
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),FAIL_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),FAIL_STATUS_BKCOLOR);
            }
            break;
        case E_TSEMP_PASS:
            if (sThreadParam.mePrevTSEMPTestStatus != sThreadParam.meTSEMPTestStatus)
            {
                sMultipleViewInfo.mpDownloadProcess->SetValue(msMultipleConfig.mnTestTimeSec);
                sMultipleViewInfo.mpStartButton->SetText(L"Start");
                sMultipleViewInfo.mpStartButton->SetEnabled(true);

                sMultipleViewInfo.mpStatusLabel->SetText(L"PASS");
                sMultipleViewInfo.mpDownloadProcess->SetForeImage(PASS_PROCESS_IMG);
                //sMultipleViewInfo.mpStatusLabel->SetAttribute(_T("bkcolor"),PASS_STATUS_BKCOLOR);
                sMultipleViewInfo.mpStatusHorizontal->SetAttribute(_T("bkcolor"),PASS_STATUS_BKCOLOR);
            }
            break;
        }
        // 检查当前测试LOG信息
        if (sThreadParam.mstrTSEMultipleTestLog.IsHaveCurrntTestLog())
        {
            sMultipleViewInfo.mpLogRickEdit->AppendText(sThreadParam.mstrTSEMultipleTestLog.GetCurrntTestLog().c_str());
            sMultipleViewInfo.mpLogRickEdit->EndDown();
        }
        sThreadParam.mePrevTSEMPTestStatus = sThreadParam.meTSEMPTestStatus;
    }

    void CCMultipleTestPluginDlg::OnResponseDeviceQueueMsg(LPS_DEVICE_QUEUE_INFO psDeviceQueueInfo,LPVOID pContext)
    {
        CCMultipleTestPluginDlg* pMultipleTestPluginDlg = (CCMultipleTestPluginDlg*)pContext;
        //! 检查上下文指针以及多路测试初始化结果
        if (NULL == pMultipleTestPluginDlg || !pMultipleTestPluginDlg->mbInitMultipleTestOk) return;
        //! 检查视图索引
        if (psDeviceQueueInfo->mnViewIndex >= 0)
        {
            if (pMultipleTestPluginDlg->mvsMultipleTestThreadParam.size() > (size_t)psDeviceQueueInfo->mnViewIndex)
            {
                //！复制排序的设备信息
                if (psDeviceQueueInfo->mbIsPlugInDevice)
                {
                    pMultipleTestPluginDlg->mvsMultipleTestThreadParam[psDeviceQueueInfo->mnViewIndex].msDeviceQueueInfo.mnViewIndex = psDeviceQueueInfo->mnViewIndex;
                    pMultipleTestPluginDlg->mvsMultipleTestThreadParam[psDeviceQueueInfo->mnViewIndex].msDeviceQueueInfo.mstrDes = psDeviceQueueInfo->mstrDes;
                    pMultipleTestPluginDlg->mvsMultipleTestThreadParam[psDeviceQueueInfo->mnViewIndex].msDeviceQueueInfo.mstrFristDevice = psDeviceQueueInfo->mstrFristDevice;
                    pMultipleTestPluginDlg->mvsMultipleTestThreadParam[psDeviceQueueInfo->mnViewIndex].msDeviceQueueInfo.mstrSecondDevice = psDeviceQueueInfo->mstrSecondDevice;
                    pMultipleTestPluginDlg->mvsMultipleTestThreadParam[psDeviceQueueInfo->mnViewIndex].msDeviceQueueInfo.mstrThreeDevice = psDeviceQueueInfo->mstrThreeDevice;
                }
                pMultipleTestPluginDlg->mvsMultipleTestThreadParam[psDeviceQueueInfo->mnViewIndex].msDeviceQueueInfo.mbIsPlugInDevice = psDeviceQueueInfo->mbIsPlugInDevice;
                pMultipleTestPluginDlg->mvsMultipleTestThreadParam[psDeviceQueueInfo->mnViewIndex].mbIsNewDevicePlugIn =psDeviceQueueInfo->mbIsPlugInDevice; 
            }
        }
    }

    void CCMultipleTestPluginDlg::CheckInitMultipleTestIsOk()
    {
        //MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),L"插件初始化异常，请检查配置",L"Error",MB_ICONERROR);
    }

    void ScanDownloadFile(const wchar_t* filepath,const wchar_t* filename,void* lpData)
    {
        std::vector<std::wstring>* vDownloadFileList = (std::vector<std::wstring>*)lpData;
        std::wstring strTmep (filepath);
        strTmep += L"\\";
        strTmep += filename;
        vDownloadFileList->push_back(strTmep);
    }

    void CCMultipleTestPluginDlg::CheckMultipleTestConfigIsVaild()
    {
        mbInitMultipleTestOk = false;
        mcMultipleTestManageThread.StopTSEMultipleTestManageThread();

        msMultipleConfig.mstrBinaryImagePath = JGW_RealativePathToAbsPath(msMultipleConfig.mstrBinaryImagePath.c_str());
        if (msMultipleConfig.mbBinaryImageIsFolder)
        {
            mcMultipleTestConfig.SettingTftp32BaseDirectory(msMultipleConfig.mstrBinaryImagePath.c_str());
        }
        else
        {
            mcMultipleTestConfig.SettingTftp32BaseDirectory(JGW_GetPathOfFile(msMultipleConfig.mstrBinaryImagePath.c_str()).c_str());
        }
        // SourceTSEParam => 将TSE_Param参数组名称以及对应值传递进TSE测试进程，以逗号做分隔 
        // TSE_Param => 文件名称正则表达式，匹配对应文件信息 以逗号做分隔 
        msMultipleConfig.mstrFormatTSEParam = L"";
        //! 1、检查TSE下载或者升级参数配置是否为空 如果为空则不检查文件有效性
        if (msMultipleConfig.mstrConfigTSEParam.empty()) 
        {
            mbInitMultipleTestOk = true;
            mcMultipleTestManageThread.StartTSEMultipleTestManageThread(&msMultipleConfig);
            return;
        }

        //! 2、当前下载升级文件是目录
        if (msMultipleConfig.mbBinaryImageIsFolder)
        {
            // 1 : 遍历当前文件目录
            int numFiles = 0;
            std::wstring strFileName,strRegex;
            std::vector<std::wstring> vTSEParam,vDownloadFileList;
            std::wstring strFolder(msMultipleConfig.mstrBinaryImagePath);
            JGW_EraseLastAndFristTwoCharsW(strFolder,L'\\',L'/');
            JGW_TraverseFile_VC(strFolder,numFiles,L"\\*.*",ScanDownloadFile,&vDownloadFileList);
            // 2 : TSE参数循环
            for (std::map<std::wstring,std::wstring>::iterator it = msMultipleConfig.mmapTSEParamValues.begin();
                it != msMultipleConfig.mmapTSEParamValues.end();
                ++ it)
            {
                strRegex = it->second;
                std::wregex partten(it->second,std::wregex::icase);
                bool result = false;
                for (size_t x = 0;x < vDownloadFileList.size();x ++)
                {
                    strFileName = JGW_GetFileNameToFilePath(vDownloadFileList[x].c_str());
                    if (regex_match(strFileName,partten))
                    {
                        // msMultipleConfig.mstrFormatTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->first.c_str(),vDownloadFileList[x].c_str());
                        msMultipleConfig.mstrFormatTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->first.c_str(),strFileName.c_str());
                        // 检查当前文件是否存在
                        JGW_FormatWString(strRegex,L"%s\\%s",strFolder.c_str(),strFileName.c_str());
                        if (!JGW_PathFileExists(strRegex.c_str()))
                        {
                            MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),strRegex.c_str(),L"File does not exist",MB_ICONERROR);
                            return;
                        }
                        result = true;
                        break;
                    }
                }
                if (!result)
                {
                    JGW_FormatWString(strFileName,L"Find %s File Fail,Regex:%s",it->first.c_str(),strRegex.c_str());
                    MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),strFileName.c_str(),L"error",MB_ICONERROR);
                    return;
                }
            }
            mbInitMultipleTestOk = true;
        }
        else
        {
            // 1: 检查固件路径是否为空以及固件是否存在
            if (msMultipleConfig.mstrBinaryImagePath.empty() || !JGW_FileExistsToFilePath(msMultipleConfig.mstrBinaryImagePath.c_str()))
            {
                MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),L"请选择下载或者升级文件",L"Error",MB_ICONERROR);
                mpBinaryImagePathLabel->SetText(L"");
                return;
            }
            //! 2: 正则表达式检查选择固件是否符合要求
            if (!msMultipleConfig.mmapTSEParamValues.empty())
            {
                std::wstring strFileName = JGW_GetFileNameToFilePath(msMultipleConfig.mstrBinaryImagePath.c_str());
                std::map<std::wstring,std::wstring>::iterator it = msMultipleConfig.mmapTSEParamValues.begin();
                std::wregex partten(it->second,std::wregex::icase);//! regex_search

                //msMultipleConfig.mstrFormatTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->first.c_str(),msMultipleConfig.mstrBinaryImagePath.c_str());
                msMultipleConfig.mstrFormatTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->first.c_str(),strFileName.c_str());

                if (!regex_match(strFileName,partten))
                {
                    MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),L"选择下载或者升级文件不匹配",L"Error",MB_ICONERROR);
                    mpBinaryImagePathLabel->SetText(L"");
                    return;
                }   
            }
            mbInitMultipleTestOk = true;
        }
        mcMultipleTestManageThread.StartTSEMultipleTestManageThread(&msMultipleConfig);
    }
}
