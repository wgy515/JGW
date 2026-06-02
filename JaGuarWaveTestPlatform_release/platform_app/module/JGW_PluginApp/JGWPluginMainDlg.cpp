#include "StdAfx.h"
#include "JGWPluginMainDlg.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include <poco/NObserver.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "CJGW_InputViewResolver.h"
#include "CJGW_LogWindowsView.h"

namespace JGW
{
    CJGWPluginMainDlg::CJGWPluginMainDlg(int nPluginAppID,HWND hMainTestPlatformWindow,S_MES_INFO& sMesInfo,S_PLUGIN_INFO& sPluginInfo) : mhMainTestPlatformWindow(hMainTestPlatformWindow),msMesInfo(sMesInfo),msPluginInfo(sPluginInfo),m_pViewResolverClass(NULL),mnPluginAppID(nPluginAppID)
    {
        m_sCommonWndMsgInfo.mpsMesInfo = &msMesInfo;
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCreateMsgNotification>(mevent,&CCJGWUIEvent::HandleSubCreateWndEvent));
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCustomeMessageNotification>(mevent,&CCJGWUIEvent::HandleOnMainCustomeMessage));
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndMsgOnNotifyNotification>(mevent,&CCJGWUIEvent::HandleOnTNotifyEvent));
        GetMESDataBaseAdo()->SetMESServiceSocketConnect(&msMesInfo);
    }


    CJGWPluginMainDlg::~CJGWPluginMainDlg(void)
    {
    }

    DUI_BEGIN_MESSAGE_MAP_B(CJGWPluginMainDlg, CCUiLibWnd)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_SELECTCHANGED,L"",OnSelectChanged)
        //! DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"logShowBtn",OnRspLogBtnClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"showLogLayoutMenuElement",OnRspLogBtnClick)    
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"maximizeLogLayoutBtn",OnMaximizeLogLayoutBtnClick)
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

    void CJGWPluginMainDlg::OnCloseWindow()
    {
        CloseSubPluginView();

        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CJGWPluginMainDlg::OnEventCustomMessage,0);
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCreateMsgNotification>(mevent,&CCJGWUIEvent::HandleSubCreateWndEvent));
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCustomeMessageNotification>(mevent,&CCJGWUIEvent::HandleOnMainCustomeMessage));
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndMsgOnNotifyNotification>(mevent,&CCJGWUIEvent::HandleOnTNotifyEvent));

        DestoryMESDataBaseAdo();
    }

    void CJGWPluginMainDlg::OnFinalMessage(HWND hWnd)
    {
        CCUiLibWnd::OnFinalMessage(hWnd);
        DestroySubPlugin();
        PostQuitMessage(0);
        delete this;
    }

    void CJGWPluginMainDlg::OnRspLogBtnClick(TNotifyUI& msg)
    {
        OnResponceViewLogInfoMsg(mpLogHorizontalLayout->IsVisible()?0:1,NULL);
    }

    void CJGWPluginMainDlg::CloseSubPluginView()
    {
        Log4WI(L"CCJGWMainDlg::CloseSubPluginView");
        mevent.CloseSubPluginView();
        if (m_pViewResolverClass) m_pViewResolverClass->CloseSubWndPlugin();
        m_pSubWndLayoutUI->RemoveAll();
    }

    void CJGWPluginMainDlg::DestroySubPlugin()
    {
        Log4WI(L"CCJGWMainDlg::DestroySubPlugin");
        mevent.DestroySubPlugin();
        m_cLoadSubPlugin.DestroyClassPtr(mstrClsid,m_pViewResolverClass);
        m_pViewResolverClass = NULL;
    }


    void CJGWPluginMainDlg::OnNotify(TNotifyUI& msg)
    {
        if( msg.sType == DUI_MSGTYPE_CLICK )
        {
#if 0
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
#endif
        }
        else if (msg.sType == DUI_MSGTYPE_MOUSEENTER)
        {
            //if (msg.pSender->GetName() == L"menuProjectbtn")
            //{
            //    OnShowProjectMenu(msg.pSender);
            //} 
        }
        mNotifiCenter.postNotification(new CSubWndMsgOnNotifyNotification(msg));
    }

    void CJGWPluginMainDlg::OnSelectChanged(TNotifyUI& msg)
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
        else if (msg.pSender->GetName() == L"LogOption")
        {
            TNotifyUI msg;
            OnRspLogBtnClick(msg);     
        }
#endif
    }

    void CJGWPluginMainDlg::OnInitWindow()
    {
        //! InitControlsUIClassPtr
        {
            bool bSuccess = true;

            mpTitleLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("TitleLabel")));
            mpLogOption =  static_cast<COptionUI*>(m_PaintManager.FindControl(_T("LogOption")));
            m_pSubWndLayoutUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("subWndHorizontalLayout")));
            mpLogHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("logHorizontalLayout")));
            mpLogTabedt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("logTabedt")));
            mpTestInfoVerticalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("testInfoVerticalLayout")));
            //! 隐藏测试视图
            if (mpTestInfoVerticalLayout) mpTestInfoVerticalLayout->SetVisible(false);
            //if (mpLogHorizontalLayout) mpLogHorizontalLayout->SetVisible(false);
            bSuccess &= (NULL != mpTitleLabel);
            bSuccess &= (NULL != mpLogOption);
            bSuccess &= (NULL != m_pSubWndLayoutUI);
            bSuccess &= (NULL != mpLogHorizontalLayout);
            bSuccess &= (NULL != mpLogTabedt);
            bSuccess &= (NULL != mpTestInfoVerticalLayout);

            if( !bSuccess )
            {
                ::MessageBox(GetHWND(),GetString(E_CONTROL_FAILED_TOLOAD),GetString(E_VIEW_INIT_ERROR),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            } 
        }
        {
            SetParent(GetHWND(),mhMainTestPlatformWindow);
            //Subclass()
        }
        //! 响应事件消息
        {
            Log4WD(L"CJGWPluginMainDlg::OnInitWindow");
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CJGWPluginMainDlg::OnEventCustomMessage,0);
        }
        //! 加载视图加载器插件
        LoadViewResolverPlugin();
        //! 发送最大化命令按钮
        SetTimer(GetHWND(),WM_TIMER_MAX_EVENT,200,NULL);
        //!
        ::SendMessage(mhMainTestPlatformWindow,WM_PLUGIN_APP_HWND_MSG_TO_MAIN_HWND_MSG_ID,mnPluginAppID,LPARAM(GetHWND()));
    }

 
    CJGWDuiiLlibSubWndImpl* CJGWPluginMainDlg::GetDuiLibSubWndImplPtr(int viewload_id)
    {
#if 0
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
#else
        //mstrClsid = msPluginInfo.mstrClsid;
        return m_cLoadSubPlugin.CreateSubWndImpl("JGW::CCJGWUiLibPluginViewResolver");
#endif
    }
  
    bool CJGWPluginMainDlg::OnResponseClickSubWindows(LPVOID lpData)
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

        //if (NULL != msMesInfo.mstrOperationId.compare("-1") 
        //    && E_NOTONLINE_MES != msMesInfo.msMesConnectInfo.meMESModel 
        //    && E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus
        //    )
        //{
        //    CCJGW_MESConfigDlg mesConfigDlg(m_hWnd);
        //    mesConfigDlg.DoModal(&msMesInfo);
        //}

        //OnUpdateMESUiInfo(NULL,NULL);

        //if (NULL != msMesInfo.mstrOperationId.compare("-1") 
        //    && E_NOTONLINE_MES != msMesInfo.msMesConnectInfo.meMESStatus 
        //    && E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus)
        //{
        //    ::MessageBox(GetHWND(),GetString(E_SELECT_CURRENT_ORDER_INFO),GetString(E_NOT_SELECTED_TASK),MB_ICONERROR);
        //    return false;
        //}

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

    bool CJGWPluginMainDlg::OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam )
    {
        switch (pTEventUI->Type)
        {
        case WM_TIMER: //! 定时器
            OnResponceTimeMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_DEVICECHANGE: //! 设备插入或者拔出消息
            //OnDeviceChange(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_RESPONCE_DEVICEPOOL_MSG_DEFINE: //! 响应线程池发送过来的消息
            //OnResponceDeviceMsg(pTEventUI->wParam,pTEventUI->lParam);
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
            //OnMenuElementClick(pTEventUI);    
            break;
        case WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS://! 响应关闭子测试插件
            CloseSubPluginView();
            break;
        case WM_HWND_UPDATE_MES_INFO: //! 更新MES信息
            //OnUpdateMESUiInfo(NULL,NULL);
            break;
        case WM_HWND_VIEW_LOG_INFO://! 更新视图LOG信息
            OnResponceViewLogInfoMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_HWND_RSP_HELP_DOWNLOAD_MSG:
        case WM_HWND_RSP_HELP_UPDATE_MSG:
        case WM_HWND_RSP_HELP_CONNECT_ERROR_MSG:
        case WM_HWND_RSP_HELP_CLOSE_MSG:
            //OnRspHelpMsg(pTEventUI->Type);
            break;
        case WM_HWND_HELP_TIP_CLISE_MSG:
            //mbOpenHelpTipDlg = false;
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
        case WM_MAIN_HWND_TO_PLUGIN_APP_MSG_ID:
            {
                if (WM_UPDATE_MES_STATUS_MSG_WPARAM == pTEventUI->wParam)
                {
                    if (1 == pTEventUI->lParam) msMesInfo.msMesConnectInfo.meMESModel = E_ONLINE_MES;
                    else msMesInfo.msMesConnectInfo.meMESModel = E_NOTONLINE_MES;
                }
                else if (WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM == pTEventUI->wParam)
                {
                    START_REMOTELOG_SOCKET(1 == pTEventUI->lParam ? true : false);
                }
            }
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

    void CJGWPluginMainDlg::OnResponcePluginVersionMsg( WPARAM& wParam,LPARAM& lParam )
    {
        if (!wParam) return ;
        std::wstring strTemp( L"Version: ");
        strTemp += (LPCTSTR)wParam;
        mpTitleLabel->SetText(strTemp.c_str());
        Log4WD_F(L"CCJGWMainDlg::OnResponcePluginVersionMsg %s",(LPCTSTR)wParam);
    }


    void CJGWPluginMainDlg::OnResponceTimeMsg( WPARAM wParam, LPARAM lParam )
    {
        if (WM_TIMER_CLICK_SUB_WINDOWS == (int)wParam) //! 初始化子插件
        {
            KillTimer(GetHWND(),WM_TIMER_CLICK_SUB_WINDOWS);
            //m_pPrejectVerticalLayoutUI->SetVisible(true);
            DestroySubPlugin();
            if (!OnResponseClickSubWindows(LPVOID(&msPluginInfo)))
            {
                ::MessageBox(GetHWND(),GetString(E_PLUGIN_INITIALIZATION_ERROR),GetString(E_INITIALIZATION_FAILUER),MB_ICONERROR);
                CloseSubPluginView();
                DestroySubPlugin();
            }
            else
            {
                //LPS_PLUGIN_INFO psPluginInfo = (LPS_PLUGIN_INFO)(m_pClickPSender->GetDate());
                //if (!psPluginInfo->mstrInitMenuExec.empty())
                //{
                //    TEventUI sTEventUI;
                //    sTEventUI.wParam = WPARAM(new wchar_t[psPluginInfo->mstrInitMenuExec.size() + 1]);
                //    wcscpy_s((wchar_t*)sTEventUI.wParam,psPluginInfo->mstrInitMenuExec.size() + 1,psPluginInfo->mstrInitMenuExec.c_str());
                //    mstrClickMenuName = L"toolBtn";
                //    OnMenuElementClick(&sTEventUI);
                //}  
            }
        }
        else if (WM_TIMER_UPDATE_TIME == (int)wParam) //! 更新测试时间
        {
            //OnUpdateTime();
        }
        else if (WM_TIMER_MAX_EVENT == (int)wParam) //! 延时最大化窗口
        {
#if 0
            KillTimer(GetHWND(),WM_TIMER_MAX_EVENT);
            m_pPrejectVerticalLayoutUI->SetSelProjectIndex(mcZQTViewLayoutConfig.GetUIProjectInfo()->mnGroupIndex);
            m_pProjectNameLabel->SetText(mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo.size() > mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex?mcZQTViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo[mcZQTViewLayoutConfig.GetUIProjectInfo()->mnProjectIndex].mstrProjectName.c_str():L"Project");
            CControlUI* pbtnMax = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
            if (!pbtnMax) return ;
            long lparam = MAKELONG(pbtnMax->GetX(),pbtnMax->GetY());
            SendMessage(WM_LBUTTONDOWN,NULL,LPARAM(lparam));
            SendMessage(WM_LBUTTONUP,NULL,LPARAM(lparam));
#endif
            KillTimer(GetHWND(),WM_TIMER_MAX_EVENT);
            //! 打开欢迎界面
            PostMessage( WM_RESPONCE_CLICK_SUB_WINDOW_MSG,WPARAM(&msPluginInfo),NULL );
        }
        else if (WM_TIMER_INIT_MES_SERVICE == (int)wParam) //! 延时处理MES 服务
        {
            //! KillTimer(GetHWND(),WM_TIMER_INIT_MES_SERVICE);
            //! StartMESService();
        }
    }

    void CJGWPluginMainDlg::LoadViewResolverPlugin()
    {
        std::wstring strViewLoaderPluginName = L"JGW_UiLibViewResolver.dll";

#if 0
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
#else
        S_PLUGIN_INFO sPluginInfo;
        sPluginInfo.mstrPluginName = strViewLoaderPluginName;
        m_cLoadSubPlugin.LoadSubWndPlugin( &sPluginInfo );
        //mstrClsid = msPluginInfo.mstrClsid;
        //m_cLoadSubPlugin.LoadSubWndPlugin( &msPluginInfo );
        //Log4WD_F(_T("CCJGWMainDlg::InitWindow ViewLoaderPluginClsid:%s , ViewLoaderPluginName:%s"),msPluginInfo.mstrClsid.c_str(),msPluginInfo.mstrPluginName.c_str() );
#endif
    }


    void CJGWPluginMainDlg::OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam)
    {
        //! 隐藏log显示信息
        if (0 == (int)wParam)
        {
            //! 已经隐藏
            if (!mpLogHorizontalLayout->IsVisible()) return;
            mpLogHorizontalLayout->SetVisible(false);
        }
        else
        {
            COptionUI* pError =  (COptionUI*)m_PaintManager.FindControl(L"errorTab");
            COptionUI* debugTab =  (COptionUI*)m_PaintManager.FindControl(L"debugTab");
            debugTab->Selected(true);
            pError->Selected(true);
            mpLogHorizontalLayout->SetVisible(true);
        }
        PostMessage(WM_HWND_SHOW_LOG_LAYOUT_CLICK_MSG,WPARAM(mpLogHorizontalLayout->IsVisible()?0:1));
    }



    LRESULT CJGWPluginMainDlg::OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam )
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

    void CJGWPluginMainDlg::OnMaximizeLogLayoutBtnClick(TNotifyUI& msg)
    {
        OnResponceViewLogInfoMsg(NULL,NULL);
        CCJGW_LogWindowsView logWindowsView;
        logWindowsView.DoModal();
    }
    

}
