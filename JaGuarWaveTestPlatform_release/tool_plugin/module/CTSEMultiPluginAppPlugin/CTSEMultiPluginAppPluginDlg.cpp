#include "StdAfx.h"
#include "CTSEMultiPluginAppPluginDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCTSEMultiPluginAppPluginDlg::CCTSEMultiPluginAppPluginDlg(void) : meNumberOfMultipleTests(E_NUMBER_OF_MULTIPLE_TEST_2),mnMultipleTestRow(1),mnMultipleTestCol(2)
    {
        msPluginSkin.mstrSkinXmlPath = L"tse_multi_plugin_app_plugin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCTSEMultiPluginAppPluginDlg::~CCTSEMultiPluginAppPluginDlg(void)
    {

    }

    bool CCTSEMultiPluginAppPluginDlg::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        {
            mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            mpMainMultipleVerticalLayout = static_cast<CVerticalLayoutUI*>(mpPluginPaintManagerUI->FindControl(L"MainMultipleVerticalLayout"));
            if (NULL == mpMainMultipleVerticalLayout) return false;
            //! 上下文菜单栏（右键点击出现菜单栏）
            mpMainMultipleVerticalLayout->SetContextMenuUsed(true);
            //! 加载多路插件测试配置信息
            if (!LoadTSEMultiPluginAppConfig()) return false;
            // 响应发送的消息
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_MENUCLICK,&CCTSEMultiPluginAppPluginDlg::OnMenuClick);
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_PLUGIN_APP_HWND_MSG_TO_MAIN_HWND_MSG_ID,&CCTSEMultiPluginAppPluginDlg::OnPluginAppHwndMsg);
            ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID,&CCTSEMultiPluginAppPluginDlg::OnMainHwndToPluginAppMsg);
            //! 初始化多路测试视图
            InitTSEMultiPluginAppPluginView();
        }
        return true;
    }

    void CCTSEMultiPluginAppPluginDlg::OnMainHwndToPluginAppMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        //! 更新MES状态信息 => 菜单栏有调试模式，非调试模式配置
        if (WM_UPDATE_MES_STATUS_MSG_WPARAM == wParam)
        {
            for (std::map<int,CPluginAppWindowUI*>::iterator it = mmappPluginAppWindowUI.begin();it != mmappPluginAppWindowUI.end();++it)
            {
                it->second->SendPluginAppMsg(WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID,WM_UPDATE_MES_STATUS_MSG_WPARAM,lParam);
            }   
        }
        else if (WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM == wParam)
        {
            for (std::map<int,CPluginAppWindowUI*>::iterator it = mmappPluginAppWindowUI.begin();it != mmappPluginAppWindowUI.end();++it)
            {
                it->second->SendPluginAppMsg(WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID,WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM,lParam);
            }  
        }
    }

    void CCTSEMultiPluginAppPluginDlg::OnPluginAppHwndMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        //! JGW_PluginApp.exe发送自身窗口句柄消息到上层
        int nPluginID = wParam;
        if (mmappPluginAppWindowUI.end() != mmappPluginAppWindowUI.find(nPluginID))
        {
            mmappPluginAppWindowUI[nPluginID]->SetPluginAppWindow((HWND)lParam);
        }
    }

    void CCTSEMultiPluginAppPluginDlg::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if (pNotifyMsg->sType == DUI_MSGTYPE_MENU)
        {
            //! 鼠标右键点击消息
            if (pNotifyMsg->pSender == mpMainMultipleVerticalLayout)
            {
                OnContextMenuClick(pNotifyMsg);
            }
        }
    }

    void CCTSEMultiPluginAppPluginDlg::OnContextMenuClick(TNotifyUI* pNotifyMsg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = pNotifyMsg->ptMouse.x;
        point.y = pNotifyMsg->ptMouse.y;
        ClientToScreen(mpPluginPaintManagerUI->GetPaintWindow(), &point);

        mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"] = false;
        mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"] = false;
        mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"] = false;
        mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"] = false;
        switch(meNumberOfMultipleTests)
        {
        case E_NUMBER_OF_MULTIPLE_TEST_1:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"] = true;
            break;
        case E_NUMBER_OF_MULTIPLE_TEST_2:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"] = true;
            break;
        case E_NUMBER_OF_MULTIPLE_TEST_4:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"] = true;
            break;
        case E_NUMBER_OF_MULTIPLE_TEST_8:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"] = true;
            break;
        }

        pMenu->Init(NULL,_T("tse_multi_plugin_app_plugin_menu.xml"),point,mpPluginPaintManagerUI,&mpMenuCheckInfo);
    }

    void CCTSEMultiPluginAppPluginDlg::CloseSubWndPlugin()
    {
        UnInitTSEMultiPluginAppPluginView();
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    void CCTSEMultiPluginAppPluginDlg::OnMenuClick(const WPARAM& wParam,const LPARAM& lParam)
    {
#if 0
        const wchar_t* strName = (const wchar_t*)wParam;

        return ;
#else
        //! 取消已单机过的参数
        switch(meNumberOfMultipleTests)
        {
        case E_NUMBER_OF_MULTIPLE_TEST_1:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"] = false;
            break;
        case E_NUMBER_OF_MULTIPLE_TEST_2:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"] = false;
            break;
        case E_NUMBER_OF_MULTIPLE_TEST_4:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"] = false;
            break;
        case E_NUMBER_OF_MULTIPLE_TEST_8:
            mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"] = false;
            break;
        }
        //! 判断是有点击数据
        if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"])
        {
            if (!(1 == mnMultipleTestRow && 1 == mnMultipleTestCol))
            {
                mnMultipleTestRow = 1;
                mnMultipleTestCol = 1;
                InitTSEMultiPluginAppPluginView();
                mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",1);
            }
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_1;
        }
        else if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"])
        {
            if (!(1 == mnMultipleTestRow && 2 == mnMultipleTestCol))
            {
                mnMultipleTestRow = 1;
                mnMultipleTestCol = 2;
                InitTSEMultiPluginAppPluginView();
                mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",2);
            }
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_2;
        }
        else if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"])
        {
            if (!(2 == mnMultipleTestRow && 2 == mnMultipleTestCol))
            {
                mnMultipleTestRow = 2;
                mnMultipleTestCol = 2;
                InitTSEMultiPluginAppPluginView();
                mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",4);
            }
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_4;
        }
        else if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"])
        {
            if (!(2 == mnMultipleTestRow && 4 == mnMultipleTestCol))
            {
                mnMultipleTestRow = 2;
                mnMultipleTestCol = 4;
                InitTSEMultiPluginAppPluginView();
                mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",8);
            }
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_8;
        }
#endif
    }

    void CCTSEMultiPluginAppPluginDlg::UnInitTSEMultiPluginAppPluginView()
    {
        for (std::map<int,CPluginAppWindowUI*>::iterator it = mmappPluginAppWindowUI.begin();it != mmappPluginAppWindowUI.end();++it)
        {
            it->second->StopPluginAppExecute();
            //! 当前插件内部的控件，内部自动删除
            ((CVerticalLayoutUI*)it->second->GetParent())->RemoveAll();
        }
        mmappPluginAppWindowUI.clear();
        mpMainMultipleVerticalLayout->RemoveAll();
    }

    void CCTSEMultiPluginAppPluginDlg::InitTSEMultiPluginAppPluginView()
    {
        int nIndex = 0;
        //! 卸载多路测试视图
        UnInitTSEMultiPluginAppPluginView();

        //! 初始化多路测试视图
        for (int i = 0;i < mnMultipleTestRow;i++)
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

            for (int y = 0;y < mnMultipleTestCol;y++)
            {
#if 1
                CDialogBuilder builder1;
                CVerticalLayoutUI* pListContainerItem = (CVerticalLayoutUI*)(builder1.Create(_T("plugin_appp_window.xml"),(UINT)0,this,mpPluginPaintManagerUI));
                nIndex ++;
                if (NULL != pListContainerItem)
                {
                    //! 当前插件内部的控件，内部自动删除
                    pListContainerItem->SetDelayedDestroy(false);
                    CPluginAppWindowUI* pPluginAppWindowUI = (CPluginAppWindowUI*)pListContainerItem->GetItemAt(0);
                    if (pPluginAppWindowUI != NULL)
                    {
                        pPluginAppWindowUI->StartPluginAppExecute(nIndex,mpPluginPaintManagerUI->GetPaintWindow(),nIndex,mpsViewResolverParam->mpsCommonWndMsgInfo->mpsMesInfo->msMesConnectInfo.meMESModel == E_ONLINE_MES ? 1 : 0,&msMultiPluginAppConfig);
                        mmappPluginAppWindowUI[nIndex] = (pPluginAppWindowUI);
                    }
                    pHorizontalLayoutUI->Add(pListContainerItem);
#if 0
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
#endif

                } 
#endif
            }
            mpMainMultipleVerticalLayout->Add(pHorizontalLayoutUI);
        }
    }

    CControlUI* CCTSEMultiPluginAppPluginDlg::CreateControl(LPCTSTR pstrClass)
    {
        if(_tcscmp(pstrClass, DUI_CTR_PLUGINAPPWINDOWUI) == 0)
            return	new CPluginAppWindowUI();
        return NULL;
    }

    bool CCTSEMultiPluginAppPluginDlg::LoadTSEMultiPluginAppConfig()
    {
        std::wstring strConfigFilePath;

        JGW_FormatWString(strConfigFilePath,L"%s\\%s\\%s",JGW_GetExecConfigFilePath(L"config").c_str(),mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str(),TSE_MULTI_PLUGIN_APP_CONFIG_NAME);
        mConfigIni.InitIniFilePath(strConfigFilePath.c_str());

        msMultiPluginAppConfig.mstrStationID = mpsViewResolverParam->mpPluginInfo->mStationID;
        msMultiPluginAppConfig.mstrClsid = mConfigIni.GetIniKeyValue(L"MAIN",L"clsid");
        msMultiPluginAppConfig.mnViewLoaderID = mConfigIni.GetIniKeyIntValue(L"MAIN",L"viewloader_id",1);
        msMultiPluginAppConfig.mstrModuleFolder = mConfigIni.GetIniKeyValue(L"MAIN",L"module_folder");
        msMultiPluginAppConfig.mstrPluginName = mConfigIni.GetIniKeyValue(L"MAIN",L"name");
        msMultiPluginAppConfig.mstrStationName = mConfigIni.GetIniKeyValue(L"MAIN",L"station_name");
        std::string strStationNameA = JGW_W2A(msMultiPluginAppConfig.mstrStationName,CP_ACP);
        msMultiPluginAppConfig.mstrStationName = JGW_A2W(strStationNameA,CP_UTF8);
        msMultiPluginAppConfig.mstrSuiteName = mConfigIni.GetIniKeyValue(L"MAIN",L"suite_name");
        msMultiPluginAppConfig.mnNumberOfMultipleTests = mConfigIni.GetIniKeyIntValue(L"MAIN",L"NumberOfMultipleTests",2);

        switch (msMultiPluginAppConfig.mnNumberOfMultipleTests)
        {
        case 1:
            mnMultipleTestRow = 1;
            mnMultipleTestCol = 1;
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_1;
            break;
        case 2:
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_2;
            mnMultipleTestRow = 1;
            mnMultipleTestCol = 2;
            break;
        case 4:
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_4;
            mnMultipleTestRow = 2;
            mnMultipleTestCol = 2;
            break;
        case 8:
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_8;
            mnMultipleTestRow = 2;
            mnMultipleTestCol = 4;
            break;
        default:
            meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_2;
            mnMultipleTestRow = 2;
            mnMultipleTestCol = 2;
            break;
        }

        return true;
    }
}

