#include "stdafx.h"
#include "help/CJGW_HelpTipDlg.h"
#include "JGWMainDlg.h"
#include "CJGW_InputViewResolver.h"
#include "CJGW_LogWindowsView.h"
#include "CJGW_MESConfigDlg.h"
#include "CJGW_StartMESServiceDlg.h"
#include "CJGW_IperfServerDlg.h"
#include "CJGW_MESConfig.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include <poco/NObserver.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
//#include <JGW_CSTSEServerModule\IJGW_CSTSEServerModuleInterface.h>

#include "../../include/JaGuarWaveTestPlatformHelp/CCJGW_HelpThreadImpl.h"


namespace JGW
{
    CCJGWMainDlg::CCJGWMainDlg(S_MES_INFO& sMesInfo,S_JGW_HELP_CONFIG& sHelpConfig):m_pSubWndLayoutUI(NULL),m_lpDevicePoolImpl(NULL),m_pViewResolverClass(NULL),mnOldProjectIndex(0),mbOpenHelpTipDlg(false),msMesInfo(sMesInfo),msHelpConfig(sHelpConfig),m_pClickPSender(NULL)
    {
        m_sCommonWndMsgInfo.mpsMesInfo = &msMesInfo;
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCreateMsgNotification>(mevent,&CCJGWUIEvent::HandleSubCreateWndEvent));
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCustomeMessageNotification>(mevent,&CCJGWUIEvent::HandleOnMainCustomeMessage));
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndMsgOnNotifyNotification>(mevent,&CCJGWUIEvent::HandleOnTNotifyEvent));
        GetMESDataBaseAdo()->SetMESServiceSocketConnect(&msMesInfo);
    }


    CCJGWMainDlg::~CCJGWMainDlg(void)
    {
        for (size_t i = 0;i < m_vSDeviceMsg.size();i ++)
        {
            delete m_vSDeviceMsg[i];
        }
        m_vSDeviceMsg.clear();
    }

    DUI_BEGIN_MESSAGE_MAP_B(CCJGWMainDlg, CCUiLibWnd)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_SELECTCHANGED,L"",OnSelectChanged)
        //! DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"logShowBtn",OnRspLogBtnClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"showLogLayoutMenuElement",OnRspLogBtnClick)    
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"maximizeLogLayoutBtn",OnMaximizeLogLayoutBtnClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"viewMenuBtn",OnViewMenuClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"SettingMenuBtn",OnSettingMenuClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"systemMenuBtn",OnSytemMenuClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"toolBtn",OnToolMenuClick)
#if 0
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"startBtn",OnRspStartTestBtnClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"configBtn",OnRspConfigBtnClick)
#else
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_CLICK,L"startBtn",WM_HWND_TEST_SATRT_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_CLICK,L"configBtn",WM_HWND_CONFIG_BUTTON_CLICK_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_SELECTCHANGED,L"clearTestRecOption",WM_HWND_CLEAR_TEST_RECORD_BUTTON_CLICK_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_SELECTCHANGED,L"SaveTestLog",WM_HWND_SAVE_TEST_LOG_SELECTCHANGE_MSG)
#endif
        DUI_END_MESSAGE_MAP_B()

    void CCJGWMainDlg::OnCloseWindow()
    {
        Log4WI(L"CCJGWMainDlg::OnCloseWindow");
       /* if (msMesInfo.msCSTSEServerConfig.mbOpenCSTSEServerModule)
        {
            GetServerModule()->StopCSTSEServer();
        }*/
        StopHelpModuleThread();
        CloseSubPluginView();
        
        if (m_lpDevicePoolImpl) m_lpDevicePoolImpl->CloseDevicePoolThread();

        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGWMainDlg::OnEventCustomMessage,0);
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCreateMsgNotification>(mevent,&CCJGWUIEvent::HandleSubCreateWndEvent));
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCustomeMessageNotification>(mevent,&CCJGWUIEvent::HandleOnMainCustomeMessage));
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndMsgOnNotifyNotification>(mevent,&CCJGWUIEvent::HandleOnTNotifyEvent));

        if (m_pTimerLabel) KillTimer(GetHWND(),WM_TIMER_UPDATE_TIME);
        int width = m_pPrejectVerticalLayoutUI->GetWidth();
        if (mnOldProjectIndex != mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex
            || m_pPrejectVerticalLayoutUI->GetSelProjectIndex() != mcZQTViewLayoutConfig.GetUIProjectInfo()->mnGroupIndex
            || width != _ttoi(mcZQTViewLayoutConfig.GetUIProjectInfo()->msProjectButtonLayout.m_strWidth.c_str()))
        {
            mcZQTViewLayoutConfig.GetUIProjectInfo()->mnGroupIndex = m_pPrejectVerticalLayoutUI->GetSelProjectIndex();
            if (0 < width)
                mcZQTViewLayoutConfig.GetUIProjectInfo()->msProjectButtonLayout.m_strWidth = JGW_GetFormatWString(L"%d",width);
            mcZQTViewLayoutConfig.SaveViewLayoutConfig(JGW_W2A_W(JGW_GetUIConfigFilePath()));
        }

        //! 保存配置文件
        CCJGW_MESConfig::SaveMESIniConfig(&msMesInfo);
        DestoryMESDataBaseAdo();
        START_REMOTELOG_SOCKET(false);
        
    }

    void CCJGWMainDlg::OnFinalMessage(HWND hWnd)
    {
        CCUiLibWnd::OnFinalMessage(hWnd);
        DestroySubPlugin();
        PostQuitMessage(0);
        delete this;
    }

    void CCJGWMainDlg::OnRspLogBtnClick(TNotifyUI& msg)
    {
        OnResponceViewLogInfoMsg(mpLogHorizontalLayout->IsVisible()?0:1,NULL);
    }

    void CCJGWMainDlg::CloseSubPluginView()
    {
        Log4WI(L"CCJGWMainDlg::CloseSubPluginView");
        mevent.CloseSubPluginView();
        if (m_pViewResolverClass) m_pViewResolverClass->CloseSubWndPlugin();
        m_pSubWndLayoutUI->RemoveAll();
    }

    void CCJGWMainDlg::DestroySubPlugin()
    {
        Log4WI(L"CCJGWMainDlg::DestroySubPlugin");
        mevent.DestroySubPlugin();
        m_cLoadSubPlugin.DestroyClassPtr(mstrClsid,m_pViewResolverClass);
        m_pViewResolverClass = NULL;
    }

    void CCJGWMainDlg::OnNotify(TNotifyUI& msg)
    {
        if( msg.sType == DUI_MSGTYPE_CLICK )
        {
            if (m_pPrejectVerticalLayoutUI->IsPrjectButtonClick(msg.pSender))
            {	//! 加载子窗口
                m_pClickPSender = msg.pSender;
                CloseSubPluginView();
                mpTestInfoVerticalLayout->SetVisible(false);
                m_pPrejectVerticalLayoutUI->SetVisible(false);
                //! 实现窗口动画，以及等待上一个视图结束
                SetTimer(GetHWND(),WM_TIMER_CLICK_SUB_WINDOWS,200,NULL);
                return ;
            }
        }
        else if (msg.sType == DUI_MSGTYPE_MOUSEENTER)
        {
            if (msg.pSender->GetName() == L"menuProjectbtn")
            {
                OnShowProjectMenu(msg.pSender);
            } 
        }
        mNotifiCenter.postNotification(new CSubWndMsgOnNotifyNotification(msg));
    }

    void CCJGWMainDlg::OnSelectChanged(TNotifyUI& msg)
    {
        if (msg.pSender->GetName() == L"debugTab")
        {
            meLogClickLevel = JGWPrintLog_Debug;
            mpLogTabedt->SetText(GETPDLOGW().c_str());
            mpLogTabedt->EndDown();
        }
        else if (msg.pSender->GetName() == L"errorTab")
        {
            meLogClickLevel = JGWPrintLog_Error;
            mpLogTabedt->SetText(GETPELOG4W().c_str());
            mpLogTabedt->EndDown();  
        }
        else if (msg.pSender->GetName() == L"htmlTab")
        {
            meLogClickLevel = JGWPrintLog_Html;
            mpLogTabedt->SetText(GETPHLOGW().c_str());
            mpLogTabedt->EndDown();
        }
#if 1
        else if (msg.pSender->GetName() == L"clearTestRecOption")
        {
            ((COptionUI*)msg.pSender)->Selected(false);
        }
#endif
    }

    bool CCJGWMainDlg::InitControlsUIClassPtr()
    {
        bool bSuccess = true;
        //! projectBtn1
        mpToolTitleLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("toolTitleLab")));
        mpOperationIDEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("operationIDEdt")));
        mpWrokOrederNumberEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("wrokOrederNumberEdt")));
        mpUserNameEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("uersEdt")));
        mpshiftEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("shiftIDEdt")));
        m_pTipLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("tipLabel")));
        m_pMesInfoLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("testinfolab")));
        m_pTimerLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("timeLabel")));
        if (m_pTimerLabel) SetTimer( GetHWND(),WM_TIMER_UPDATE_TIME,1000,NULL );
        bSuccess &= (NULL != m_pTipLabel);
        bSuccess &= (NULL != mpToolTitleLabel);
        bSuccess &= (NULL != mpOperationIDEdt);
        bSuccess &= (NULL != mpWrokOrederNumberEdt);
        bSuccess &= (NULL != mpshiftEdt);
        bSuccess &= (NULL != mpUserNameEdt);
        bSuccess &= (NULL != m_pMesInfoLabel);

        m_pPrejectVerticalLayoutUI = static_cast<CUIVerticalLayoutUIProjectBtn*>(m_PaintManager.FindControl(_T("projectVerLayout")));
        m_pSubWndLayoutUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("subWndHorizontalLayout")));
        mpLogHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("logHorizontalLayout")));
        m_pPluginVersionLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("pluginVersionLabel")));
        mpLogTabedt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("logTabedt")));
        mpTestInfoVerticalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("testInfoVerticalLayout")));
        m_pProjectNameLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("projectNameLabel")));
        mpLeftFrameVerticalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("leftFrameVerticalLayout")));
        //! 隐藏测试视图
        if (mpTestInfoVerticalLayout) mpTestInfoVerticalLayout->SetVisible(false);
        mpStatusBarLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("statusBarLayout")));
        m_pAutoUpdateServiceStatusLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("autoUpdateStatusLabel")));
        bSuccess &= (NULL != m_pPrejectVerticalLayoutUI);
        bSuccess &= (NULL != m_pSubWndLayoutUI);
        bSuccess &= (NULL != mpLogHorizontalLayout);
        bSuccess &= (NULL != m_pPluginVersionLabel);
        bSuccess &= (NULL != mpLogTabedt);
        bSuccess &= (NULL != mpTestInfoVerticalLayout);
        bSuccess &= (NULL != mpStatusBarLayout);
        bSuccess &= (NULL != m_pProjectNameLabel);
        bSuccess &= (NULL != mpLeftFrameVerticalLayout);
        bSuccess &= (NULL != m_pAutoUpdateServiceStatusLabel);

        if( !bSuccess )
        {
            ::MessageBox(GetHWND(),GetString(E_CONTROL_FAILED_TOLOAD),GetString(E_VIEW_INIT_ERROR),MB_ICONERROR);
            Close(IDCANCEL);
        }

        return bSuccess;
    }

    void CCJGWMainDlg::OnInitWindow()
    {
        if (!InitControlsUIClassPtr()) return ;
        //! 响应事件消息
        {
            Log4WD(L"CCJGWMainDlg::OnInitWindow");
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGWMainDlg::OnEventCustomMessage,0);
        }
        //! 加载MES 更新服务
        {
            if (!GetHelpModuleThread()->IsThreadRun()) m_pAutoUpdateServiceStatusLabel->SetBkImage(L"errorRedDot.png");
            SetMainWindowsHwnd(m_hWnd);
        }
        //::ShowWindow(GetHWND(), SW_SHOWMAXIMIZED);
        //! load view config
        {
            if (!mcZQTViewLayoutConfig.LoadViewLayoutConfig(JGW::JGW_W2A_W(JGW_GetUIConfigFilePath()))) 
            {
                ::MessageBox(GetHWND(),GetString(E_VIEW_LAYOUT_FILE_LOADING_ERROR),GetString(E_VIEW_INIT_ERROR),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            }
            mnOldProjectIndex = mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex;
            mpLeftFrameVerticalLayout->SetAttribute(L"width",mcZQTViewLayoutConfig.GetUIProjectInfo()->msProjectButtonLayout.m_strWidth.c_str());
            m_pPrejectVerticalLayoutUI->InitPluginGroupView(mcZQTViewLayoutConfig.GetUIProjectInfo());
        }
        //! 加载视图加载器插件
        LoadViewResolverPlugin();
        //! 开启设备池
        StartDevicePool();
        //! start cs tse server module
        /*if (msMesInfo.msCSTSEServerConfig.mbOpenCSTSEServerModule)
        {
            GetServerModule()->StartCSTSEServer(msMesInfo.msCSTSEServerConfig.mnCSTSEServerSocketPort);
        }*/

        //! 发送最大化命令按钮
        SetTimer(GetHWND(),WM_TIMER_MAX_EVENT,200,NULL);
#if 0
        //! 
        if (E_NOTONLINE_MES == msMesInfo.msMesConnectInfo.meMESModel) return;
        //! 初始化MES 服务
        SetTimer(GetHWND(),WM_TIMER_INIT_MES_SERVICE,1000,NULL);
#endif
    }
#if 0
    void CCJGWMainDlg::LoadMESTaskInfo()
    {
        Log4WD_F(L"CCJGWMainDlg::LoadMESTaskInfo Userpwd : %s",msMesInfo.msMesConnectInfo.mstrUserpwd.empty()?L"":L"*******");
//         if (msMesInfo.msMesConnectInfo.mstrUserpwd.empty())
//         {
//             msMesInfo.msMesConnectInfo.meMESStatus = E_MES_LOGIN_ERROR;
//             msMesInfo.msMesConnectInfo.mbSaveAccount = false;
//             goto MES_ERROR;
//         }

        if (!GetMESDataBaseAdo()->LoginMES())
        {
            msMesInfo.msMesConnectInfo.meMESStatus = E_MES_LOGIN_ERROR_STATUS;
            msMesInfo.msMesConnectInfo.mbSaveAccount = false;
            goto MES_ERROR;
        }
        //! GetMESDataBaseAdo()->LoginMES();
        //! 则重新获取订单信息 加载订单显示UI  判断是否已经保存订单信息
        if (!msMesInfo.mstrTaskNumber.empty())
        {
            //! 获取订单信息失败的时候报错 并且提示选择订单信息
            msMesInfo.msMesConnectInfo.meMESStatus = E_MES_OK_STATUS;
            Log4WD_F(L"LoginMES Task Number %s Is Successful",msMesInfo.mstrTaskNumber.c_str());
        }
        else
        {
            Log4WE(L"LoginMES Task Number Login Empty");
            msMesInfo.msMesConnectInfo.meMESStatus = E_MES_TASK_ERROR_STATUS;
        }
MES_ERROR:
        OnUpdateMESUiInfo(NULL,NULL);
    }

    void CCJGWMainDlg::StartMESService()
    {
        Log4WD_F(L"Start MES Service");
        LoadMESTaskInfo();
    }

//     bool CCJGWMainDlg::InitMESConfig()
//     {
//         msMesInfo.msMesConnectInfo.meMESStatus = E_MES_INIT_STATUS;
//         msMesInfo.msMesConnectInfo.mstrMESConfigFilePath = JGW_GetApplicationFolder();
//         msMesInfo.msMesConnectInfo.mstrMESConfigFilePath += L"jgw_mes_config.data";
//         msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig = JGW_GetApplicationFolder();
//         msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig += L"jgw_mes_workstage.xml";
//         Log4WD_F(L"MES Config File Path: %s,MES Work Stage Config File Path: %s",msMesInfo.msMesConnectInfo.mstrMESConfigFilePath.c_str(),msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig.c_str());
// 
//         CCJGW_MESConfig config;
//         config.LoadMESWorkStageXmlConfig(msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig,&msMesInfo);
//         if (!config.LoadMESIniConfig(msMesInfo.msMesConnectInfo.mstrMESConfigFilePath,m_sCommonWndMsgInfo.mpsMesInfo))
//         {
//             ::MessageBox(GetHWND(),_T("MES配置文件加载失败(jgw_mes_config.data)"),_T("MES Config"),MB_ICONERROR);
//             Close(IDCANCEL);
//             return false;
//         }
//         return true;
//     }
#endif
    void CCJGWMainDlg::OnUpdateMESUiInfo(WPARAM wParam, LPARAM lParam)
    {
        if (E_NOTONLINE_MES == msMesInfo.msMesConnectInfo.meMESModel || 
            E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus)
        {
            mpToolTitleLabel->SetTextColor(0xff0000);
            mpToolTitleLabel->SetText(E_NOTONLINE_MES == msMesInfo.msMesConnectInfo.meMESModel?GetString(E_MES_OFFLINE_MODE):GetString((E_STRING_TYPE)(msMesInfo.msMesConnectInfo.meMESStatus + E_MES_UNKNOWN_STATUS_TYPE)));
            mpOperationIDEdt->SetText(L"");
            mpWrokOrederNumberEdt->SetText(L"");
            mpUserNameEdt->SetText(L"");
            mpshiftEdt->SetText(L"");
            return;
        }

        mpToolTitleLabel->SetTextColor(0x000000);
        std::wstring strMesInfoTip = GetString((E_STRING_TYPE)(msMesInfo.msMesConnectInfo.meMESStatus + E_MES_UNKNOWN_STATUS_TYPE));
        if (E_MES_OK_STATUS == msMesInfo.msMesConnectInfo.meMESStatus) 
        {
            strMesInfoTip += L"---";
            strMesInfoTip += JGW_A2W(msMesInfo.mstrTaskNumber);
        }
        mpToolTitleLabel->SetText(strMesInfoTip.c_str());
        //! 站点信息
        if (msMesInfo.mmapMESWorkStage.end() == msMesInfo.mmapMESWorkStage.find(msMesInfo.mstrOperationId))
        {
            mpOperationIDEdt->SetText(JGW_A2W(msMesInfo.mstrOperationId).c_str());
        }
        else
        {
            mpOperationIDEdt->SetText(msMesInfo.mmapMESWorkStage[msMesInfo.mstrOperationId].mstrWorkStageName.c_str());
        }
#ifdef GET_INFO_WORK_ORDER_NUMBER
        mpWrokOrederNumberEdt->SetText(JGW_A2W(msMesInfo.mstrWorkOrderNumber).c_str());
#else
        //! 订单信息
        mpWrokOrederNumberEdt->SetText(JGW_A2W(msMesInfo.mstrTaskNumber).c_str());
#endif // _DEBUG
        

        //! 用户名信息
        mpUserNameEdt->SetText(JGW_A2W(msMesInfo.msMesConnectInfo.mstrUsername).c_str());
        //! 班次信息
        if (NULL == msMesInfo.mstrShiftID.compare("310001"))
        {
            msMesInfo.mstrShiftID = "310001";
            mpshiftEdt->SetText(GetString(E_DAY_SHIFT_TYPE));
        }
        else
        {
            msMesInfo.mstrShiftID = "310002";
            mpshiftEdt->SetText(GetString(E_NIGHT_SHIFT_TYPE));
        }
    }

    CJGWDuiiLlibSubWndImpl* CCJGWMainDlg::GetDuiLibSubWndImplPtr(int viewload_id)
    {
        S_PLUGIN_INFO sPluginInfo;
        for ( std::vector<S_VIEW_LOAD_KEY_VALUE>::iterator it = mcZQTViewLayoutConfig.GetUIProjectInfo()->msViewLoaderInfo.mvViewLoadKey_Value.begin();
            it != mcZQTViewLayoutConfig.GetUIProjectInfo()->msViewLoaderInfo.mvViewLoadKey_Value.end();
            ++ it )
        {
            if (it->mID_ClsidMap.end() != it->mID_ClsidMap.find(viewload_id))
            {
                mstrClsid = JGW_W2A((it->mID_ClsidMap.find(viewload_id))->second);
                Log4WD_F(L"GetDuiLibSubWndImplPtr clsid : %s",JGW_A2W(mstrClsid).c_str());
                return m_cLoadSubPlugin.CreateSubWndImpl(mstrClsid);
            }
        }
        return NULL;
    }

    bool CCJGWMainDlg::OnResponseClickSubWindows(LPVOID lpData)
    {
        Log4WD(L"OnClick Plugin View OnResponseClickSubWindows");
        //! 如果log视图打开则直接关闭
        OnResponceViewLogInfoMsg(NULL,NULL);
        //! 初始化
        m_sSubWndInfo.mpMainPaintManager = &m_PaintManager;
        m_sSubWndInfo.mpPluginInfo = lpData;
        m_sSubWndInfo.mpWParam = &m_sCommonWndMsgInfo;
        m_sSubWndInfo.mpMainParentContainer = m_pSubWndLayoutUI;
        m_sSubWndInfo.mpViewResolverClassLoader = NULL;
        m_sSubWndInfo.mpMapRegMsg = NULL;

        if (!lpData)
        {
            ::MessageBox(GetHWND(),GetString(E_GET_PLUGIN_INFO_BUTTON_NOY_AVAILABLE),GetString(E_VIEW_INIT_ERROR),MB_ICONERROR);
            return false;
        }
        msMesInfo.mstrOperationId = JGW_W2A(LPS_PLUGIN_INFO(lpData)->mStationID);
        if (LPS_PLUGIN_INFO(lpData)->mnViewLoadID)
        {
            m_pViewResolverClass = GetDuiLibSubWndImplPtr(LPS_PLUGIN_INFO(lpData)->mnViewLoadID);
            m_sSubWndInfo.mpViewResolverClassLoader = m_pViewResolverClass;
        }

        if (NULL != msMesInfo.mstrOperationId.compare("-1") 
            && E_NOTONLINE_MES != msMesInfo.msMesConnectInfo.meMESModel 
            && E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus
            )
        {
            CCJGW_MESConfigDlg mesConfigDlg(m_hWnd);
            mesConfigDlg.DoModal(&msMesInfo);
        }

        OnUpdateMESUiInfo(NULL,NULL);

        if (NULL != msMesInfo.mstrOperationId.compare("-1") 
            && E_NOTONLINE_MES != msMesInfo.msMesConnectInfo.meMESStatus 
            && E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus)
        {
            ::MessageBox(GetHWND(),GetString(E_SELECT_CURRENT_ORDER_INFO),GetString(E_NOT_SELECTED_TASK),MB_ICONERROR);
            return false;
        }

/*#ifdef _DEBUG*/
        Log4WD_F(_T("CCJGWMainDlg::OnResponseClickSubWindows\t\n\t\tstationid:%s,\n\t\tImageGroupID:%d,\n\t\tUseViewLoader:%d,\n\t\tclsid:%s,\n\t\tModuleFolderName:%s,\n\t\tStationName:%s,\n\t\tPluginName:%s"),LPS_PLUGIN_INFO(lpData)->mStationID.c_str(), \
            LPS_PLUGIN_INFO(lpData)->mImageGroupID, \
            LPS_PLUGIN_INFO(lpData)->mnViewLoadID,\
            LPS_PLUGIN_INFO(lpData)->mstrClsid.c_str(),\
            LPS_PLUGIN_INFO(lpData)->mstrModuleFolderName.c_str(),\
            LPS_PLUGIN_INFO(lpData)->mstrStationName.c_str(),\
            LPS_PLUGIN_INFO(lpData)->mstrPluginName.c_str()\
            );
/*#endif*/
        mNotifiCenter.postNotification(new CSubWndCreateMsgNotification(&m_sSubWndInfo));
        return mevent.GetHandleResult();
    }

    bool CCJGWMainDlg::OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam )
    {
        switch (pTEventUI->Type)
        {
        case WM_TIMER: //! 定时器
            OnResponceTimeMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_DEVICECHANGE: //! 设备插入或者拔出消息
            OnDeviceChange(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_RESPONCE_DEVICEPOOL_MSG_DEFINE: //! 响应线程池发送过来的消息
            OnResponceDeviceMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_RESPONCE_CLICK_SUB_WINDOW_MSG://! 响应点击子插件消息
            OnResponseClickSubWindows(LPVOID(pTEventUI->wParam));
            break;
        case WM_RESPONCE_PLUGIN_VERSION_MSG_DEFINE://! 响应插件版本更新消息
            OnResponcePluginVersionMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_HWND_INPUT_COMMON_WINDOW_MSG: //! 响应通用输入框消息
            return IDCANCEL == OnResponceInputCommonWindowMsg(pTEventUI->wParam,pTEventUI->lParam);
        case WM_MENUCLICK: //! 响应菜单栏点击消息
            OnMenuElementClick(pTEventUI);    
            break;
        case WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS://! 响应关闭子测试插件
            CloseSubPluginView();
            break;
        case WM_HWND_UPDATE_MES_INFO: //! 更新MES信息
            OnUpdateMESUiInfo(NULL,NULL);
            break;
        case WM_HWND_VIEW_LOG_INFO://! 更新视图LOG信息
            OnResponceViewLogInfoMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
		case WM_PLUGIN_APP_TO_MAIN_HWND_MSG_ID:
			//!WM_PLUGIN_APP_TO_MAIN_HWND_MSG_ID WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM 1
			if (WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM == pTEventUI->wParam)
			{
				if (1 == pTEventUI->lParam && false == mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"])
				{
					START_REMOTELOG_SOCKET(true);
					mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"]  = true;
					WPARAM wParam = WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM;
					LPARAM lParam = mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"] ? 1 : 0;
					mNotifiCenter.postNotification(new CSubWndCustomeMessageNotification(WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID,wParam,lParam));
				}
				if (0 == pTEventUI->lParam && true == mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"])
				{
					START_REMOTELOG_SOCKET(false);
					mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"]  = false;
					WPARAM wParam = WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM;
					LPARAM lParam = mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"] ? 1 : 0;
					mNotifiCenter.postNotification(new CSubWndCustomeMessageNotification(WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID,wParam,lParam));
				}
			}
			break;
        case WM_HWND_RSP_HELP_DOWNLOAD_MSG:
        case WM_HWND_RSP_HELP_UPDATE_MSG:
        case WM_HWND_RSP_HELP_CONNECT_ERROR_MSG:
        case WM_HWND_RSP_HELP_CLOSE_MSG:
            OnRspHelpMsg(pTEventUI->Type);
            break;
        case WM_HWND_HELP_TIP_CLISE_MSG:
            mbOpenHelpTipDlg = false;
            break;
        case WM_HWND_TEST_PASS_MSG:
            //GetHelpModuleThread()->PostThreadMessage(WM_THREAD_HELP_UPLOAD_LOG_MSG,WPARAM(&msHelpConfig),LPARAM(m_hWnd));
            return true;
        case WM_HWND_TEST_FAIL_MSG:
            //GetHelpModuleThread()->PostThreadMessage(WM_THREAD_HELP_UPLOAD_LOG_MSG,WPARAM(&msHelpConfig),LPARAM(m_hWnd));
            return true;
        case WM_THREAD_HELP_UPLOAD_LOG_MSG:
            //GetHelpModuleThread()->PostThreadMessage(WM_THREAD_HELP_UPLOAD_LOG_MSG,WPARAM(&msHelpConfig),LPARAM(m_hWnd));
            return true;
#if 0
        case WM_HWND_TEST_SATRT_MSG: //! 主要由子窗口传递给父窗口
            {
                TNotifyUI msg;
                OnRspStartTestBtnClick(msg);
            }           
            return true;
#endif
        default:
            break;
        }
        mNotifiCenter.postNotification(new CSubWndCustomeMessageNotification(pTEventUI->Type,pTEventUI->wParam,pTEventUI->lParam));
        return true;
    }

    void CCJGWMainDlg::OnResponceTimeMsg( WPARAM wParam, LPARAM lParam )
    {
        if (WM_TIMER_CLICK_SUB_WINDOWS == (int)wParam && m_pClickPSender) //! 初始化子插件
        {
            KillTimer(GetHWND(),WM_TIMER_CLICK_SUB_WINDOWS);
            m_pPrejectVerticalLayoutUI->SetVisible(true);
            DestroySubPlugin();
            if (!OnResponseClickSubWindows(m_pClickPSender->GetDate()))
            {
                ::MessageBox(GetHWND(),GetString(E_PLUGIN_INITIALIZATION_ERROR),GetString(E_INITIALIZATION_FAILUER),MB_ICONERROR);
                CloseSubPluginView();
                DestroySubPlugin();
            }
            else
            {
                LPS_PLUGIN_INFO psPluginInfo = (LPS_PLUGIN_INFO)(m_pClickPSender->GetDate());
                if (!psPluginInfo->mstrInitMenuExec.empty())
                {
                    TEventUI sTEventUI;
                    sTEventUI.wParam = WPARAM(new wchar_t[psPluginInfo->mstrInitMenuExec.size() + 1]);
                    wcscpy_s((wchar_t*)sTEventUI.wParam,psPluginInfo->mstrInitMenuExec.size() + 1,psPluginInfo->mstrInitMenuExec.c_str());
                    mstrClickMenuName = L"toolBtn";
                    OnMenuElementClick(&sTEventUI);
                }  
            }
        }
        else if (WM_TIMER_UPDATE_TIME == (int)wParam) //! 更新测试时间
        {
            OnUpdateTime();
        }
        else if (WM_TIMER_MAX_EVENT == (int)wParam) //! 延时最大化窗口
        {
            KillTimer(GetHWND(),WM_TIMER_MAX_EVENT);
            m_pPrejectVerticalLayoutUI->SetSelProjectIndex(mcZQTViewLayoutConfig.GetUIProjectInfo()->mnGroupIndex);
            m_pProjectNameLabel->SetText(mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo.size() > mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex?mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex].mstrProjectName.c_str():L"Project");
            CControlUI* pbtnMax = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
            if (!pbtnMax) return ;
            long lparam = MAKELONG(pbtnMax->GetX(),pbtnMax->GetY());
            SendMessage(WM_LBUTTONDOWN,NULL,LPARAM(lparam));
            SendMessage(WM_LBUTTONUP,NULL,LPARAM(lparam));
            //! 打开欢迎界面
            PostMessage( WM_RESPONCE_CLICK_SUB_WINDOW_MSG,WPARAM(mcZQTViewLayoutConfig.GetWelcomPluginInfo()),NULL );
        }
        else if (WM_TIMER_INIT_MES_SERVICE == (int)wParam) //! 延时处理MES 服务
        {
            //! KillTimer(GetHWND(),WM_TIMER_INIT_MES_SERVICE);
            //! StartMESService();
        }
    }

    void CCJGWMainDlg::OnUpdateTime()
    {
        m_pTimerLabel->SetText(JGW_GetCurrentTimeStamp().c_str());
        if (E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus || E_ONLINE_MES != msMesInfo.msMesConnectInfo.meMESModel) { m_pMesInfoLabel->SetText(L""); return ;}
        if (!msMesInfo.msMesConnectInfo.mbSaveAccount) return ;
        CheckLoginIsExpired();
    }

    void CCJGWMainDlg::CheckLoginIsExpired()
    {
        if (-1 == msMesInfo.msMesConnectInfo.mnSaveAccountTime)
        {
            m_pMesInfoLabel->SetTextColor(0x000080);
            m_pMesInfoLabel->SetText(GetString(E_ACCOUNT_DEADLINE_PERMANENT));
            return ;
        }
        //! 登录时间已经过期
        int diffTime = (int)JGW_DiffTime(msMesInfo.msMesConnectInfo.mnSaveAccountTimeStamp + msMesInfo.msMesConnectInfo.mnSaveAccountTime * 3600,JGW_GetTimeOfDay());
        if (diffTime < 0)
        {
            msMesInfo.msMesConnectInfo.mbSaveAccount = false;
            msMesInfo.msMesConnectInfo.meMESStatus = E_MES_LOGIN_ERROR_STATUS;
            msMesInfo.msMesConnectInfo.mstrUserpwd = "";
            m_pMesInfoLabel->SetTextColor(0xff0000);
            m_pMesInfoLabel->SetText(GetString(E_ACCOUNT_LOGIN_EXPIREAD_RELOGIN)); 
        }
        else
        {
            m_pMesInfoLabel->SetTextColor(0x000080);
            if (diffTime < 60)
                m_pMesInfoLabel->SetText(JGW_GetFormatWString(GetString(E_ACCOUNT_DEADLINE_SEC),diffTime).c_str()); 
            else if (diffTime < 3600)
                m_pMesInfoLabel->SetText(JGW_GetFormatWString(GetString(E_ACCOUNT_DEADLINE_MINUTE),(diffTime)/60).c_str());
            else
                m_pMesInfoLabel->SetText(JGW_GetFormatWString(GetString(E_ACCOUNT_DEADLINE_HOUR),(diffTime)/3600,(diffTime % 3600) / 60).c_str());
        } 
    }

    void CCJGWMainDlg::LoadViewResolverPlugin()
    {
        S_PLUGIN_INFO sPluginInfo;

        Log4WD(L"CCJGWMainDlg::LoadViewResolverPlugin");
        for ( std::vector<S_VIEW_LOAD_KEY_VALUE>::iterator it =mcZQTViewLayoutConfig.GetUIProjectInfo()->msViewLoaderInfo.mvViewLoadKey_Value.begin();
            it != mcZQTViewLayoutConfig.GetUIProjectInfo()->msViewLoaderInfo.mvViewLoadKey_Value.end();
            ++ it )
        {
            sPluginInfo.mstrPluginName = it->mstrViewLoaderPluginName;
            m_cLoadSubPlugin.LoadSubWndPlugin( &sPluginInfo );
/*#ifdef _DEBUG*/
            for ( std::map<int,std::wstring>::iterator itid_clsid = it->mID_ClsidMap.begin();
                itid_clsid != it->mID_ClsidMap.end();
                ++ itid_clsid )
            {
                sPluginInfo.mstrClsid = itid_clsid->second;
                Log4WD_F(_T("CCJGWMainDlg::InitWindow ViewLoaderPluginClsid:%s , ViewLoaderPluginName:%s"),sPluginInfo.mstrClsid.c_str(),sPluginInfo.mstrPluginName.c_str() );
            }
/*#endif*/	
        }
    }

    void CCJGWMainDlg::StartDevicePool()
    {
        if (m_lpDevicePoolImpl) return;

        m_lpDevicePoolImpl = m_cLoadDevicePoolSubPlugin.LoadPocoClassLoader(*mcZQTViewLayoutConfig.GetDevicePoolModuleInfo());
        Log4WD(L"CCJGWMainDlg::StartDevicePool");
#if 0
        std::wstring wstrDevicePoolPluginPath;
#if _DEBUG
        JGW_FormatWString(wstrDevicePoolPluginPath,L"%s%s",JGW_GetApplicationFolder(),L"JGW_DevicePoolModule_d.dll");
#else
        JGW_FormatWString(wstrDevicePoolPluginPath,L"%s%s",JGW_GetApplicationFolder(),L"JGW_DevicePoolModule_d.dll");
#endif
        std::string strDevicePoolPluginPath = U( JGW::JGW_W2A(wstrDevicePoolPluginPath).c_str() );
        try
        {
            m_cLoadDevicePoolSubPlugin.loadLibrary(strDevicePoolPluginPath);
            if (!m_cLoadDevicePoolSubPlugin.isLibraryLoaded(strDevicePoolPluginPath)) return ;
            m_lpDevicePoolImpl = &m_cLoadDevicePoolSubPlugin.instance("JGW::CCJGW_DevicePoolThread");
        }
        catch ( Poco::LibraryLoadException& e )
        {
            Log4AE_F( "CCJGWMainDlg::StartDevicePool loadLibrary fail(%s)(%d)",e.what(), GetLastError());
            ::MessageBoxA( FindWindow(NULL,NULL),strDevicePoolPluginPath.c_str()/*e.what()*/,"线程池加载失败",MB_ICONERROR );
            return ;
        }
        catch (Poco::InvalidAccessException& e)
        {
            Log4AE_F( "CCJGWMainDlg::StartDevicePool loadLibrary fail(%s)(%d)",e.what(), GetLastError());
            ::MessageBoxA( FindWindow(NULL,NULL),strDevicePoolPluginPath.c_str()/*e.what()*/,"线程池加载失败",MB_ICONERROR );
            return ;
        }
#endif
        if ( !m_lpDevicePoolImpl ) return ;
        //! 注册USB标记 防止重复注册
        //! UPDATEREGUSBFLAGS();
        //! 初始化设备消息起始大小

        std::wstring strTemp (JGW_GetApplicationFolder());
        strTemp += _T(USB_CONFIG_INI_FILE_NAME);
        //! 开启设备池
        if ( !m_lpDevicePoolImpl->StartDevicePoolThread(GetHWND(),strTemp.c_str()) ) 
        {
            ::MessageBox(GetHWND(),GetString(E_DEVICE_POOL_CREATION_FAILED),_T("ERROR"),MB_ICONERROR);
            return ;
        }
        //! 注册线程设备PIDVID消息
        GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
        m_lpDevicePoolImpl->RegDevicePoolMsg(WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,m_hWnd,guid,E_ALL_MODE);

        //! 保存线程池线程ID
        m_sCommonWndMsgInfo.muiDevicePoolThreadID = m_lpDevicePoolImpl->GetDevicePoolThreadID();
    }

    SDeviceMsg_Info* CCJGWMainDlg::GetDeviceMsgInfoPtr()
    {
        for ( size_t i = 0;i < m_vSDeviceMsg.size();i ++)
        {
            if (m_vSDeviceMsg[i]->m_nRefCount <= 0 && !m_vSDeviceMsg[i]->m_bIsCache)
            {
                Log4WD_F(L"[GetDeviceMsgInfoPtr] %d",i);
                return m_vSDeviceMsg[i];
            }
        }

        SDeviceMsg_Info* psDeviceMsg = new SDeviceMsg_Info;
        m_vSDeviceMsg.push_back(psDeviceMsg);
        return psDeviceMsg;
    }

    void CCJGWMainDlg::OnDeviceChange(WPARAM wParam,LPARAM lParam)
    {
        if( !m_lpDevicePoolImpl || !m_lpDevicePoolImpl->IsStartDevicePoolThread() || !lParam ) return ;

        SDeviceMsg_Info* lpdevicemsg = NULL;
        PDEV_BROADCAST_DEVICEINTERFACE lpdb = (DEV_BROADCAST_DEVICEINTERFACE*)lParam;
        if( DBT_DEVTYP_DEVICEINTERFACE != lpdb->dbcc_devicetype ) return ;
        switch (wParam)
        {
        case DBT_DEVNODES_CHANGED:
            break;
        case DBT_DEVICEARRIVAL:
            lpdevicemsg = GetDeviceMsgInfoPtr();
            if(!lpdevicemsg) return ;
            lpdevicemsg->InitDeviceMsg();
            lpdevicemsg->Add();
            lpdevicemsg->m_strDbcc_name = lpdb->dbcc_name;
            lpdevicemsg->m_nDeviceMsgType = DBT_DEVICEARRIVAL;
            memcpy(&lpdevicemsg->m_guid,&lpdb->dbcc_classguid,sizeof(GUID));
            Log4WD_F(L"CCJGWMainDlg::OnDeviceChange DBT_DEVICEARRIVAL %s",lpdb->dbcc_name);
            m_lpDevicePoolImpl->SendDeviceMsgToDevicePool(lpdevicemsg);
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            lpdevicemsg = GetDeviceMsgInfoPtr();
            if(!lpdevicemsg) return ;
            lpdevicemsg->InitDeviceMsg();
            lpdevicemsg->Add();
            lpdevicemsg->m_strDbcc_name = lpdb->dbcc_name;
            lpdevicemsg->m_nDeviceMsgType = DBT_DEVICEREMOVECOMPLETE;
            Log4WD_F(L"CCJGWMainDlg::OnDeviceChange DBT_DEVICEREMOVECOMPLETE %s",lpdb->dbcc_name);
            memcpy(&lpdevicemsg->m_guid,&lpdb->dbcc_classguid,sizeof(GUID));
            m_lpDevicePoolImpl->SendDeviceMsgToDevicePool(lpdevicemsg);
            break;
        default:
            break;
        }
    }

    void CCJGWMainDlg::OnResponceDeviceMsg( WPARAM& wParam,LPARAM& lParam )
    {
        if( !wParam )return ;
        SDeviceMsg_Info* lpdevicemsg = (SDeviceMsg_Info*)wParam;

        if ( DBT_DEVICEARRIVAL == lpdevicemsg->m_nDeviceMsgType )
        {
            //! 	DEVICE_9008 = 1,DEVICE_9006 ,DEVICE_FACTORY,
            if ( E_DOWNLOADER_MODE == lpdevicemsg->m_nDeviceMode || E_DIAG_MODE == lpdevicemsg->m_nDeviceMode || E_MODEM_MODE == lpdevicemsg->m_nDeviceMode)
            {
                m_pTipLabel->SetText(JGW_GetUsbFriendlyNameToPidVID(lpdevicemsg->m_strRegUsbPidVid.c_str()).c_str());
            }
            else
            {
                m_pTipLabel->SetText(L"USB device is plugged in");
            }	
            m_pTipLabel->SetTextColor(BUSY_BK_COLOR);
        }
        else
        {
            m_pTipLabel->SetText(L"DisConnect");
            m_pTipLabel->SetTextColor(BUSY_BK_COLOR);
        }
        lpdevicemsg->Dec();
    }

    void CCJGWMainDlg::OnResponcePluginVersionMsg( WPARAM& wParam,LPARAM& lParam )
    {
        if (!wParam) return ;
        std::wstring strTemp( L"Version: ");
        strTemp += (LPCTSTR)wParam;
        m_pPluginVersionLabel->SetText(strTemp.c_str());
        Log4WD_F(L"CCJGWMainDlg::OnResponcePluginVersionMsg %s",(LPCTSTR)wParam);
    }

    LRESULT CCJGWMainDlg::OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam )
    {
#if 1
        LPS_ALL_INPUT_UI_ATTR_SETTING psAllControlAttrSetting = (LPS_ALL_INPUT_UI_ATTR_SETTING)wParam;
        if (NULL == psAllControlAttrSetting)
        {
            return IDCANCEL;
        }

        CCJGWInputViewResolver jgw(psAllControlAttrSetting);
        if ( IDOK == jgw.DoModal() )
        {
            unUserData nRetData = {{0,0,0,0}};
#if 0
            for ( std::vector<LPS_CONTROL_ATTR_SETTING>::iterator it = vGetControlAttrSetting->begin();
                it != vGetControlAttrSetting->end();
                ++ it )
            {
                for ( std::vector<S_ATTR_NAME_VALUE>::iterator itAttr = it[0]->mvsAttrNameValue.begin();
                    itAttr != it[0]->mvsAttrNameValue.end();
                    ++ itAttr )
                {
                    nRetData = jgw.GetControlAttr( it[0]->name.c_str(),itAttr->pstrName.c_str() );
                    if ( NULL != nRetData.pcwchar ) itAttr->pstrValue = nRetData.pcwchar;
                }
            }
#else
            for ( std::vector<S_ATTR_NAME_VALUE>::iterator itAttr = psAllControlAttrSetting->m_sGetControlAttrSetting.mvsAttrNameValue.begin();
                itAttr != psAllControlAttrSetting->m_sGetControlAttrSetting.mvsAttrNameValue.end();
                ++ itAttr )
            {
                nRetData = jgw.GetControlAttr( psAllControlAttrSetting->m_sGetControlAttrSetting.name.c_str(),itAttr->pstrName.c_str() );
                if ( NULL != nRetData.pcwchar ) itAttr->pstrValue = nRetData.pcwchar;
            }
#endif
            return IDOK;
        }
        return IDCANCEL;
#else
        return IDOK;
#endif
    }

    void CCJGWMainDlg::OnShowProjectMenu(CControlUI* pSend)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = pSend->GetX();
        point.y = pSend->GetY() + pSend->GetHeight();
        ClientToScreen(m_hWnd, &point);
        pMenu->Init(NULL,_T("project_menu.xml"),point,&m_PaintManager,&mpMenuCheckInfo);
        AddMenuElementToProjectNames(pMenu);
    }

    void CCJGWMainDlg::AddMenuElementToProjectNames(CMenuWnd* pMenu)
    {
        CMenuUI* rootMenu = pMenu->GetMenuUI();
        for (int i = 0;i < mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo.size();i ++)
        {
            CMenuElementUI* pTempMenu = new CMenuElementUI;
            pTempMenu->SetName(mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[i].mstrProjectMenuName.c_str());
            pTempMenu->SetText(mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[i].mstrProjectName.c_str());
            pTempMenu->SetIconSize(9,9);
            pTempMenu->SetIcon(L"right.png");
            pTempMenu->SetCheckItem(true);
            mpMenuCheckInfo[pTempMenu->GetName()] = false;
            if (i == mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex)
            {
                mpMenuCheckInfo[pTempMenu->GetName()] = true;
                pTempMenu->SetEnabled(false);
            }
            rootMenu->Add(pTempMenu);
        }
        pMenu->ResizeMenu();
    }

    void CCJGWMainDlg::OnSytemMenuClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(m_hWnd, &point);
        pMenu->Init(NULL,_T("system_menu.xml"),point,&m_PaintManager,&mpMenuCheckInfo);
        AddMenuElementToViewLayoutConfig(pMenu,msg.pSender->GetName().GetData());
    }

    void CCJGWMainDlg::OnToolMenuClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(m_hWnd, &point);
        pMenu->Init(NULL,_T("tool_menu.xml"),point,&m_PaintManager,&mpMenuCheckInfo);
        AddMenuElementToViewLayoutConfig(pMenu,msg.pSender->GetName().GetData());  
    }

    void CCJGWMainDlg::OnMaximizeLogLayoutBtnClick(TNotifyUI& msg)
    {
        OnResponceViewLogInfoMsg(NULL,NULL);
        CCJGW_LogWindowsView logWindowsView;
        logWindowsView.DoModal();
    }

    void CCJGWMainDlg::OnViewMenuClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(m_hWnd, &point);
        pMenu->Init(NULL,_T("view_menu.xml"),point,&m_PaintManager,&mpMenuCheckInfo);
        AddMenuElementToViewLayoutConfig(pMenu,msg.pSender->GetName().GetData());
    }

    void CCJGWMainDlg::OnSettingMenuClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(m_hWnd, &point);
        pMenu->Init(NULL,_T("setting_menu.xml"),point,&m_PaintManager,&mpMenuCheckInfo);
        AddMenuElementToViewLayoutConfig(pMenu,msg.pSender->GetName().GetData());
    }

    void CCJGWMainDlg::AddMenuElementToViewLayoutConfig(CMenuWnd* pMenu,const std::wstring& strMenuName)
    {
        mstrClickMenuName = strMenuName;
        auto itmap = mcZQTViewLayoutConfig.GetExecMeunInfos().find(strMenuName);
        if (mcZQTViewLayoutConfig.GetExecMeunInfos().end() == itmap) return ;
        CMenuUI* rootMenu = pMenu->GetMenuUI();
        for (auto it = itmap->second.begin();
            it != itmap->second.end();
            ++ it)
        {
            CMenuElementUI* pTempMenu = new CMenuElementUI;
            pTempMenu->SetName(it->mstrMenuElementName.c_str());
            pTempMenu->SetText(it->mstrMenuElementText.c_str());
            rootMenu->Add(pTempMenu);
        }
        pMenu->ResizeMenu();
    }

    void CCJGWMainDlg::OnMenuElementClick(TEventUI* pTEventUI)
    {
        const wchar_t* strName = (const wchar_t*)pTEventUI->wParam;
        if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("showLogLayoutMenuElement")))
        {
            TNotifyUI msg;
            OnRspLogBtnClick(msg);
        } 
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("showStatusBarMenuElement")))
        {
            mpStatusBarLayout->SetVisible(!mpStatusBarLayout->IsVisible());
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("startRemoteLogSocketMenuElement")))
        {
            START_REMOTELOG_SOCKET(mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"]);
            WPARAM wParam = WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM;
            LPARAM lParam = mpMenuCheckInfo[L"startRemoteLogSocketMenuElement"] ? 1 : 0;
            mNotifiCenter.postNotification(new CSubWndCustomeMessageNotification(WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID,wParam,lParam));
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,L"mesStartMenuElement"))
        {
            CCJGW_StartMESServiceDlg startMESServices(GetHWND());
            startMESServices.DoModal();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("mesConfigMenuElement")))
        {
            CCJGW_MESConfigDlg mesConfigDlg(m_hWnd);
            mesConfigDlg.DoModal(&msMesInfo);
            OnUpdateMESUiInfo(NULL,NULL);
        }
		else if (NULL == JGW_WStrComparenoCaseWStr(strName,L"mesDebugMenuElement"))
		{
			if (E_NOTONLINE_MES == msMesInfo.msMesConnectInfo.meMESModel)
				msMesInfo.msMesConnectInfo.meMESModel = E_ONLINE_MES;
			else
				msMesInfo.msMesConnectInfo.meMESModel = E_NOTONLINE_MES;
			OnUpdateMESUiInfo(NULL,NULL);
            WPARAM wParam = WM_UPDATE_MES_STATUS_MSG_WPARAM;
            LPARAM lParan = msMesInfo.msMesConnectInfo.meMESModel == E_ONLINE_MES ? 1 : 0;
            mNotifiCenter.postNotification(new CSubWndCustomeMessageNotification(WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID,wParam,lParan));
		}
        else
        {
            auto itmap = mcZQTViewLayoutConfig.GetExecMeunInfos().find(mstrClickMenuName);
            bool bResponseMenuMsg = false;
            if (mcZQTViewLayoutConfig.GetExecMeunInfos().end() != itmap) 
            {
                for (auto it = itmap->second.begin();
                    it != itmap->second.end();
                    ++ it)
                {
                    if (0 == it->mstrMenuElementName.compare(strName))
                    {
                        ShellExecute(m_hWnd,L"open",it->mstrMenuElementExec.c_str(),NULL,JGW_GetApplicationFolder(),it->mnShow);
                        bResponseMenuMsg = true;
                        break;
                    }
                }
            }  
            if (!bResponseMenuMsg)
            {
                for (int i = 0;i < mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo.size();i ++)
                {
                    if (0 == mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[i].mstrProjectMenuName.compare(strName))
                    {
                        mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex = i;
                        mcZQTViewLayoutConfig.GetUIProjectInfo()->mnGroupIndex = 0;
                        mcZQTViewLayoutConfig.GetUIProjectInfo()->mvAllPluginInfo.clear();
                        mcZQTViewLayoutConfig.GetUIProjectInfo()->mvAllPluginInfo.insert(mcZQTViewLayoutConfig.GetUIProjectInfo()->mvAllPluginInfo.begin(),mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[i].mvAllPluginInfo.begin(),mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[i].mvAllPluginInfo.end());
                        m_pProjectNameLabel->SetText(mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[i].mstrProjectName.c_str());
                        m_pPrejectVerticalLayoutUI->InitPluginGroupView(mcZQTViewLayoutConfig.GetUIProjectInfo());
                        m_pClickPSender = NULL;
                    }
                }
            }
        }

        delete [](const wchar_t*)pTEventUI->wParam;
    }

    void CCJGWMainDlg::OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam)
    {
        //! 隐藏log显示信息
        if (0 == (int)wParam)
        {
            //! 已经隐藏
            if (!mpLogHorizontalLayout->IsVisible()) return;
            mpLogHorizontalLayout->SetVisible(false);
            mpMenuCheckInfo[L"showLogLayoutMenuElement"] = false;
            m_pTipLabel->SetText(L"");
        }
        else
        {
            COptionUI* pError =  (COptionUI*)m_PaintManager.FindControl(L"errorTab");
            COptionUI* debugTab =  (COptionUI*)m_PaintManager.FindControl(L"debugTab");
            debugTab->Selected(true);
            pError->Selected(true);
            mpLogHorizontalLayout->SetVisible(true);
            mpMenuCheckInfo[L"showLogLayoutMenuElement"] = true;
            m_pTipLabel->SetTextColor(FAIL_BK_COLOR);
            m_pTipLabel->SetText(JGW_GETLASTERRORINFO.c_str());
        }
        PostMessage(WM_HWND_SHOW_LOG_LAYOUT_CLICK_MSG,WPARAM(mpLogHorizontalLayout->IsVisible()?0:1));
    }

    void CCJGWMainDlg::OnRspHelpMsg(int msg)
    {
        m_pAutoUpdateServiceStatusLabel->SetBkImage(L"successGreenDot.png");
        if (msg == WM_HWND_RSP_HELP_DOWNLOAD_MSG || msg == WM_HWND_RSP_HELP_UPDATE_MSG)
        {
            m_pAutoUpdateServiceStatusLabel->SetBkImage(L"successGreenDot.png");
            if (mbOpenHelpTipDlg) return ;
            RECT rcDlg = { 0 };
            ::GetWindowRect(m_hWnd, &rcDlg);
            CCJGW_HelpTipDlg helpTipDlg(msHelpConfig);
            mbOpenHelpTipDlg = true;
            helpTipDlg.DoModal(m_hWnd,(msg == WM_HWND_RSP_HELP_DOWNLOAD_MSG)?E_HELP_RSP_DOWNLOAD_TYPE:E_HELP_RSP_UPDATE_TYPE,rcDlg); 
        }
        else if (msg == WM_HWND_RSP_HELP_CONNECT_ERROR_MSG)
        {
            //! errorRedDot.png //! successGreenDot.png
            msHelpConfig.mstrTaskNumber = JGW_A2W(msMesInfo.mstrTaskNumber);
            m_pAutoUpdateServiceStatusLabel->SetBkImage(L"errorRedDot.png");
            JGW_KillProcessToName(HELP_SERVICE_APPLICATION_NAME);
            //MessageBox(m_hWnd,L"升级服务连接出现异常,正在重新进行连接",L"异常",MB_ICONERROR);
            GetHelpModuleThread()->PostThreadMessage(WM_THREAD_TEST_START_MSG,WPARAM(&msHelpConfig),LPARAM(m_hWnd));
        }
    }
}