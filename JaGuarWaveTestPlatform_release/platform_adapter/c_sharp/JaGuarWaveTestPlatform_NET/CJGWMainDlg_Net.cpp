#include "stdafx.h"
#include "CJGWMainDlg_Net.h"
#include <JaGuarWaveTestPlatform/CJGW_InputViewResolver.h>
#include <JaGuarWaveTestPlatform/CJGW_LogWindowsView.h>
#include <JaGuarWaveTestPlatform/CJGW_MESConfig.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include <poco/NObserver.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>


namespace JGW
{
    CCJGWMainDlg_Net::CCJGWMainDlg_Net():m_pSubWndLayoutUI(NULL),m_pViewResolverClass(NULL),m_hParentHwnd(NULL),mstrBackgroundColor(L"#FFCFECF8")
    {
        m_sCommonWndMsgInfo.mpsMesInfo = &msMesInfo;
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCreateMsgNotification>(mevent,&CCJGWUIEvent::HandleSubCreateWndEvent));
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCustomeMessageNotification>(mevent,&CCJGWUIEvent::HandleOnMainCustomeMessage));
        mNotifiCenter.addObserver(Poco::NObserver<CCJGWUIEvent,CSubWndMsgOnNotifyNotification>(mevent,&CCJGWUIEvent::HandleOnTNotifyEvent));
        GetMESDataBaseAdo()->SetMESServiceSocketConnect(&msMesInfo);
    }


    CCJGWMainDlg_Net::~CCJGWMainDlg_Net(void)
    {
        /*for (size_t i = 0;i < m_vSDeviceMsg.size();i ++)
        {
            delete m_vSDeviceMsg[i];
        }
        m_vSDeviceMsg.clear();*/
    }

    DUI_BEGIN_MESSAGE_MAP_B(CCJGWMainDlg_Net, CCUiLibWnd)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_SELECTCHANGED,L"",OnSelectChanged)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"showLogLayoutMenuElement",OnRspLogBtnClick)    
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"maximizeLogLayoutBtn",OnMaximizeLogLayoutBtnClick)
#if 0
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"startBtn",OnRspStartTestBtnClick)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"configBtn",OnRspConfigBtnClick)
#else
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_CLICK,L"startBtn",WM_HWND_TEST_SATRT_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_CLICK,L"configBtn",WM_HWND_CONFIG_BUTTON_CLICK_MSG)
        DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(DUI_MSGTYPE_SELECTCHANGED,L"clearTestRecOption",WM_HWND_CLEAR_TEST_RECORD_BUTTON_CLICK_MSG)
#endif
        DUI_END_MESSAGE_MAP_B()

        void CCJGWMainDlg_Net::OnCloseWindow()
    {
        Log4WI(L"CCJGWMainDlg_Net::OnCloseWindow");
        CloseSubPluginView();
        DestroySubPlugin();
        //if (m_lpDevicePoolImpl) m_lpDevicePoolImpl->CloseDevicePoolThread();

        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGWMainDlg_Net::OnEventCustomMessage,0);
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCreateMsgNotification>(mevent,&CCJGWUIEvent::HandleSubCreateWndEvent));
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndCustomeMessageNotification>(mevent,&CCJGWUIEvent::HandleOnMainCustomeMessage));
        mNotifiCenter.removeObserver(Poco::NObserver<CCJGWUIEvent,CSubWndMsgOnNotifyNotification>(mevent,&CCJGWUIEvent::HandleOnTNotifyEvent));

        DestoryMESDataBaseAdo();
        START_REMOTELOG_SOCKET(false);
        //::SetWindowLongPtr(GetHWND(), GWLP_USERDATA,NULL /*reinterpret_cast<LPARAM>(LPARAM(NULL))*/);
        delete this;
    }

    void CCJGWMainDlg_Net::OnRspLogBtnClick(TNotifyUI& msg)
    {
        OnResponceViewLogInfoMsg(mpLogHorizontalLayout->IsVisible()?0:1,NULL);
    }

    void CCJGWMainDlg_Net::CloseSubPluginView()
    {
        Log4WI(L"CCJGWMainDlg_Net::CloseSubPluginView");
        mevent.CloseSubPluginView();
        if (m_pViewResolverClass) m_pViewResolverClass->CloseSubWndPlugin();
        m_pSubWndLayoutUI->RemoveAll();
    }

    void CCJGWMainDlg_Net::DestroySubPlugin()
    {
        Log4WI(L"CCJGWMainDlg_Net::DestroySubPlugin");
        mevent.DestroySubPlugin();
        m_cLoadSubPlugin.DestroyClassPtr(mstrClsid,m_pViewResolverClass);
        m_pViewResolverClass = NULL;
    }

    void CCJGWMainDlg_Net::OnNotify(TNotifyUI& msg)
    {
        mNotifiCenter.postNotification(new CSubWndMsgOnNotifyNotification(msg));
    }

    void CCJGWMainDlg_Net::OnSelectChanged(TNotifyUI& msg)
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

    bool CCJGWMainDlg_Net::InitControlsUIClassPtr()
    {
        bool bSuccess = true;

        m_pSubWndLayoutUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("subWndHorizontalLayout")));
        mpLogHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("logHorizontalLayout")));
        m_pPluginVersionLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("pluginVersionLabel")));
        mpLogTabedt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("logTabedt")));
        mpTestInfoVerticalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("testInfoVerticalLayout")));
        mpMainVerticalLayoutUI = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("bg")));
        if (mpMainVerticalLayoutUI) mpMainVerticalLayoutUI->SetAttribute(L"bkcolor",mstrBackgroundColor.c_str());
        //! 隐藏测试视图
        if (mpTestInfoVerticalLayout) mpTestInfoVerticalLayout->SetVisible(false);

        bSuccess &= (NULL != m_pSubWndLayoutUI);
        bSuccess &= (NULL != mpLogHorizontalLayout);
        bSuccess &= (NULL != m_pPluginVersionLabel);
        bSuccess &= (NULL != mpLogTabedt);
        bSuccess &= (NULL != mpTestInfoVerticalLayout);

        if( !bSuccess )
        {
            ::MessageBox(FindWindow(NULL,NULL),_T("控件加载失败"),_T("视图初始化错误"),MB_ICONERROR);
            Close(IDCANCEL);
        }

        return bSuccess;
    }

    void CCJGWMainDlg_Net::OnInitWindow()
    {
        if (!InitControlsUIClassPtr()) return ;
        //! 响应事件消息
        {
            Log4WD(L"CCJGWMainDlg_Net::OnInitWindow");
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGWMainDlg_Net::OnEventCustomMessage,0);
        }
        //! load view config
        {
            if (!mcZQTViewLayoutConfig.LoadViewLayoutConfig(JGW::JGW_W2A_W(JGW_GetUIConfigFilePath()))) 
            {
                ::MessageBox(GetHWND(),_T("视图布局文件加载错误"),_T("视图初始化错误"),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            }
        }
        //! 加载视图加载器插件
        LoadViewResolverPlugin();
        //! 开启设备池
        //StartDevicePool();
    }

    CJGWDuiiLlibSubWndImpl* CCJGWMainDlg_Net::GetDuiLibSubWndImplPtr(int viewload_id)
    {
        S_PLUGIN_INFO sPluginInfo;
        for ( std::vector<S_VIEW_LOAD_KEY_VALUE>::iterator it = mcZQTViewLayoutConfig.GetUIProjectInfo()->msViewLoaderInfo.mvViewLoadKey_Value.begin();
            it != mcZQTViewLayoutConfig.GetUIProjectInfo()->msViewLoaderInfo.mvViewLoadKey_Value.end();
            ++ it )
        {
            if (it->mID_ClsidMap.end() != it->mID_ClsidMap.find(viewload_id))
            {
                mstrClsid = JGW_W2A((it->mID_ClsidMap.find(viewload_id))->second);
                Log4AD_F("GetDuiLibSubWndImplPtr clsid : %s",mstrClsid.c_str());
                return m_cLoadSubPlugin.CreateSubWndImpl(mstrClsid);
            }
        }
        return NULL;
    }

    bool CCJGWMainDlg_Net::OnResponseClickSubWindows()
    {
        Log4WD(L"OnClick Plugin View OnResponseClickSubWindows");
        //! 如果log视图打开则直接关闭
        OnResponceViewLogInfoMsg(NULL,NULL);
        //! 初始化
        m_sSubWndInfo.mpMainPaintManager = &m_PaintManager;
        m_sSubWndInfo.mpPluginInfo = &msPluginInfo;
        m_sSubWndInfo.mpWParam = &m_sCommonWndMsgInfo;
        m_sSubWndInfo.mpMainParentContainer = m_pSubWndLayoutUI;
        m_sSubWndInfo.mpViewResolverClassLoader = NULL;
        m_sSubWndInfo.mpMapRegMsg = NULL;

        msMesInfo.mstrOperationId = JGW_W2A(msPluginInfo.mStationID);
        if (msPluginInfo.mnViewLoadID)
        {
            m_pViewResolverClass = GetDuiLibSubWndImplPtr(msPluginInfo.mnViewLoadID);
            m_sSubWndInfo.mpViewResolverClassLoader = m_pViewResolverClass;
        }

        //         if (NULL != msMesInfo.mstrOperationId.compare("-1") 
        //             && E_NOTONLINE_MES != msMesInfo.msMesConnectInfo.meMESModel 
        //             && E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus
        //             )
        //         {
        //             CCJGW_MESConfigDlg mesConfigDlg(m_hWnd);
        //             mesConfigDlg.DoModal(&msMesInfo);
        //         }

        //OnUpdateMESUiInfo(NULL,NULL);

//         if (NULL != msMesInfo.mstrOperationId.compare("-1") 
//             && E_NOTONLINE_MES != msMesInfo.msMesConnectInfo.meMESStatus 
//             && E_MES_OK_STATUS != msMesInfo.msMesConnectInfo.meMESStatus)
//         {
//             ::MessageBox(GetHWND(),_T("请选择当前测试订单信息"),_T("未选择订单信息"),MB_ICONERROR);
//             return false;
//         }

        /*#ifdef _DEBUG*/
        Log4WD_F(_T("CCJGWMainDlg_Net::OnResponseClickSubWindows\t\n\t\tstationid:%s,\n\t\tImageGroupID:%d,\n\t\tUseViewLoader:%d,\n\t\tclsid:%s,\n\t\tModuleFolderName:%s,\n\t\tStationName:%s,\n\t\tPluginName:%s"),msPluginInfo.mStationID.c_str(), \
            msPluginInfo.mImageGroupID, \
            msPluginInfo.mnViewLoadID,\
            msPluginInfo.mstrClsid.c_str(),\
            msPluginInfo.mstrModuleFolderName.c_str(),\
            msPluginInfo.mstrStationName.c_str(),\
            msPluginInfo.mstrPluginName.c_str()\
            );
        /*#endif*/
        mNotifiCenter.postNotification(new CSubWndCreateMsgNotification(&m_sSubWndInfo));
        return mevent.GetHandleResult();
    }

    bool CCJGWMainDlg_Net::OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam )
    {
        switch (pTEventUI->Type)
        {
        case WM_TIMER: //! 定时器
            OnResponceTimeMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        //case WM_DEVICECHANGE: //! 设备插入或者拔出消息
        //    OnDeviceChange(pTEventUI->wParam,pTEventUI->lParam);
        //    break;
        //case WM_RESPONCE_DEVICEPOOL_MSG_DEFINE: //! 响应线程池发送过来的消息
        //    OnResponceDeviceMsg(pTEventUI->wParam,pTEventUI->lParam);
        //    break;
        case WM_RESPONCE_CLICK_SUB_WINDOW_MSG://! 响应点击子插件消息
            OnResponseClickSubWindows();
            break;
        case WM_RESPONCE_PLUGIN_VERSION_MSG_DEFINE://! 响应插件版本更新消息
            OnResponcePluginVersionMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_HWND_INPUT_COMMON_WINDOW_MSG: //! 响应通用输入框消息
            return IDCANCEL == OnResponceInputCommonWindowMsg(pTEventUI->wParam,pTEventUI->lParam);
        case WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS://! 响应关闭子测试插件
            CloseSubPluginView();
            break;
        case WM_HWND_VIEW_LOG_INFO://! 更新视图LOG信息
            OnResponceViewLogInfoMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;  
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

    void CCJGWMainDlg_Net::OnResponceTimeMsg( WPARAM wParam, LPARAM lParam )
    {
        if (WM_TIMER_CLICK_SUB_WINDOWS == (int)wParam) //! 初始化子插件
        {
            KillTimer(GetHWND(),WM_TIMER_CLICK_SUB_WINDOWS);
            DestroySubPlugin();
            if (!OnResponseClickSubWindows())
            {
                ::MessageBox(GetHWND(),_T("插件初始化错误"),_T("初始化失败"),MB_ICONERROR);
                CloseSubPluginView();
                DestroySubPlugin();
            }
        }
    }

    void CCJGWMainDlg_Net::LoadViewResolverPlugin()
    {
        S_PLUGIN_INFO sPluginInfo;

        Log4WD(L"CCJGWMainDlg_Net::LoadViewResolverPlugin");
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
                Log4WD_F(_T("CCJGWMainDlg_Net::InitWindow ViewLoaderPluginClsid:%s , ViewLoaderPluginName:%s"),sPluginInfo.mstrClsid.c_str(),sPluginInfo.mstrPluginName.c_str() );
            }
            /*#endif*/	
        }
    }

//    void CCJGWMainDlg_Net::StartDevicePool()
//    {
//        if (m_lpDevicePoolImpl) return;
//
//        m_lpDevicePoolImpl = m_cLoadDevicePoolSubPlugin.LoadPocoClassLoader(*mcZQTViewLayoutConfig.GetDevicePoolModuleInfo());
//        Log4WD(L"CCJGWMainDlg_Net::StartDevicePool");
//#if 0
//        std::wstring wstrDevicePoolPluginPath;
//#if _DEBUG
//        JGW_FormatWString(wstrDevicePoolPluginPath,L"%s%s",JGW_GetApplicationFolder(),L"JGW_DevicePoolModule_d.dll");
//#else
//        JGW_FormatWString(wstrDevicePoolPluginPath,L"%s%s",JGW_GetApplicationFolder(),L"JGW_DevicePoolModule_d.dll");
//#endif
//        std::string strDevicePoolPluginPath = U( JGW::JGW_W2A(wstrDevicePoolPluginPath).c_str() );
//        try
//        {
//            m_cLoadDevicePoolSubPlugin.loadLibrary(strDevicePoolPluginPath);
//            if (!m_cLoadDevicePoolSubPlugin.isLibraryLoaded(strDevicePoolPluginPath)) return ;
//            m_lpDevicePoolImpl = &m_cLoadDevicePoolSubPlugin.instance("JGW::CCJGW_DevicePoolThread");
//        }
//        catch ( Poco::LibraryLoadException& e )
//        {
//            Log4AE_F( "CCJGWMainDlg_Net::StartDevicePool loadLibrary fail(%s)(%d)",e.what(), GetLastError());
//            ::MessageBoxA( FindWindow(NULL,NULL),strDevicePoolPluginPath.c_str()/*e.what()*/,"线程池加载失败",MB_ICONERROR );
//            return ;
//        }
//        catch (Poco::InvalidAccessException& e)
//        {
//            Log4AE_F( "CCJGWMainDlg_Net::StartDevicePool loadLibrary fail(%s)(%d)",e.what(), GetLastError());
//            ::MessageBoxA( FindWindow(NULL,NULL),strDevicePoolPluginPath.c_str()/*e.what()*/,"线程池加载失败",MB_ICONERROR );
//            return ;
//        }
//#endif
//        if ( !m_lpDevicePoolImpl ) return ;
//        //! 注册USB标记 防止重复注册
//        //! UPDATEREGUSBFLAGS();
//        //! 初始化设备消息起始大小
//
//        std::wstring strTemp (JGW_GetApplicationFolder());
//        strTemp += _T(USB_CONFIG_INI_FILE_NAME);
//        //! 开启设备池
//        if ( !m_lpDevicePoolImpl->StartDevicePoolThread(GetHWND(),strTemp.c_str()) ) 
//        {
//            ::MessageBox(GetHWND(),_T("设备池创建失败"),_T("错误"),MB_ICONERROR);
//            return ;
//        }
//        //! 注册线程设备PIDVID消息
//        GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
//        m_lpDevicePoolImpl->RegDevicePoolMsg(WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,m_hWnd,guid,E_ALL_MODE);
//
//        //! 保存线程池线程ID
//        m_sCommonWndMsgInfo.muiDevicePoolThreadID = m_lpDevicePoolImpl->GetDevicePoolThreadID();
//    }

    /*SDeviceMsg_Info* CCJGWMainDlg_Net::GetDeviceMsgInfoPtr()
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
    }*/

    //void CCJGWMainDlg_Net::OnDeviceChange(WPARAM wParam,LPARAM lParam)
    //{
    //    if( !m_lpDevicePoolImpl || !m_lpDevicePoolImpl->IsStartDevicePoolThread() || !lParam ) return ;

    //    SDeviceMsg_Info* lpdevicemsg = NULL;
    //    PDEV_BROADCAST_DEVICEINTERFACE lpdb = (DEV_BROADCAST_DEVICEINTERFACE*)lParam;
    //    if( DBT_DEVTYP_DEVICEINTERFACE != lpdb->dbcc_devicetype ) return ;
    //    switch (wParam)
    //    {
    //    case DBT_DEVNODES_CHANGED:
    //        break;
    //    case DBT_DEVICEARRIVAL:
    //        lpdevicemsg = GetDeviceMsgInfoPtr();
    //        if(!lpdevicemsg) return ;
    //        lpdevicemsg->InitDeviceMsg();
    //        lpdevicemsg->Add();
    //        lpdevicemsg->m_strDbcc_name = lpdb->dbcc_name;
    //        lpdevicemsg->m_nDeviceMsgType = DBT_DEVICEARRIVAL;
    //        memcpy(&lpdevicemsg->m_guid,&lpdb->dbcc_classguid,sizeof(GUID));
    //        Log4WD_F(L"CCJGWMainDlg_Net::OnDeviceChange DBT_DEVICEARRIVAL %s",lpdb->dbcc_name);
    //        m_lpDevicePoolImpl->SendDeviceMsgToDevicePool(lpdevicemsg);
    //        break;
    //    case DBT_DEVICEREMOVECOMPLETE:
    //        lpdevicemsg = GetDeviceMsgInfoPtr();
    //        if(!lpdevicemsg) return ;
    //        lpdevicemsg->InitDeviceMsg();
    //        lpdevicemsg->Add();
    //        lpdevicemsg->m_strDbcc_name = lpdb->dbcc_name;
    //        lpdevicemsg->m_nDeviceMsgType = DBT_DEVICEREMOVECOMPLETE;
    //        Log4WD_F(L"CCJGWMainDlg_Net::OnDeviceChange DBT_DEVICEREMOVECOMPLETE %s",lpdb->dbcc_name);
    //        memcpy(&lpdevicemsg->m_guid,&lpdb->dbcc_classguid,sizeof(GUID));
    //        m_lpDevicePoolImpl->SendDeviceMsgToDevicePool(lpdevicemsg);
    //        break;
    //    default:
    //        break;
    //    }
    //}

    //void CCJGWMainDlg_Net::OnResponceDeviceMsg( WPARAM& wParam,LPARAM& lParam )
    //{
    //    if( !wParam )return ;
    //    SDeviceMsg_Info* lpdevicemsg = (SDeviceMsg_Info*)wParam;

    //    //         if ( DBT_DEVICEARRIVAL == lpdevicemsg->m_nDeviceMsgType )
    //    //         {
    //    //             //! 	DEVICE_9008 = 1,DEVICE_9006 ,DEVICE_FACTORY,
    //    //             if ( E_DOWNLOADER_MODE == lpdevicemsg->m_nDeviceMode || E_DIAG_MODE == lpdevicemsg->m_nDeviceMode || E_MODEM_MODE == lpdevicemsg->m_nDeviceMode)
    //    //             {
    //    //                 m_pTipLabel->SetText(JGW_GetUsbFriendlyNameToPidVID(lpdevicemsg->m_strRegUsbPidVid.c_str()).c_str());
    //    //             }
    //    //             else
    //    //             {
    //    //                 m_pTipLabel->SetText(L"设备插入");
    //    //             }	
    //    //             m_pTipLabel->SetTextColor(YELLOW_COLOR);
    //    //         }
    //    //         else
    //    //         {
    //    //             m_pTipLabel->SetText(L"DisConnect");
    //    //             m_pTipLabel->SetTextColor(YELLOW_COLOR);
    //    //         }
    //    lpdevicemsg->Dec();
    //}

    void CCJGWMainDlg_Net::OnResponcePluginVersionMsg( WPARAM& wParam,LPARAM& lParam )
    {
        if (!wParam) return ;
        std::wstring strTemp( L"版本号: ");
        strTemp += (LPCTSTR)wParam;
        m_pPluginVersionLabel->SetText(strTemp.c_str());
        Log4WD_F(L"CCJGWMainDlg_Net::OnResponcePluginVersionMsg %s",(LPCTSTR)wParam);
    }

    LRESULT CCJGWMainDlg_Net::OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam )
    {
#if 1
        if ( !wParam || !lParam ) return 0;
        int dlgResult = 0;
        LPS_ALL_CONTROL_ATTR_SETTING	psAllControlAttrSetting = (LPS_ALL_CONTROL_ATTR_SETTING)wParam;
        std::vector<LPS_CONTROL_ATTR_SETTING>*	vGetControlAttrSetting = (std::vector<LPS_CONTROL_ATTR_SETTING>*)lParam;

        CCJGWInputViewResolver jgw( &psAllControlAttrSetting->m_sWindowAttr );

        for ( std::vector<LPS_CONTROL_ATTR_SETTING>::iterator it = psAllControlAttrSetting->m_vControlAttrSetting.begin();
            it != psAllControlAttrSetting->m_vControlAttrSetting.end();
            ++ it )
        {
            jgw.SetControlAttrSetting( it[0] );
        }

        if ( IDOK == jgw.DoModal() )
        {
            unUserData nRetData = {{0,0,0,0}};
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
            return IDOK;
        }
        return IDCANCEL;
#else
        return IDOK;
#endif
    }

    void CCJGWMainDlg_Net::OnMaximizeLogLayoutBtnClick(TNotifyUI& msg)
    {
        OnResponceViewLogInfoMsg(NULL,NULL);
        CCJGW_LogWindowsView logWindowsView;
        logWindowsView.DoModal();
    }

    void CCJGWMainDlg_Net::OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam)
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
    }

    void CCJGWMainDlg_Net::UpdateWin32WindowBackgroundColor(const wchar_t* color)
    {
        mstrBackgroundColor = color;
        if (mpMainVerticalLayoutUI) mpMainVerticalLayoutUI->SetAttribute(L"bkcolor",mstrBackgroundColor.c_str());
    }

    void CCJGWMainDlg_Net::UpdateMESInfo(bool mesOnline,const wchar_t* taskNumber, const wchar_t* workOrderNumber, const wchar_t* userName, const wchar_t* password, int shiftID)
    {
        msMesInfo.msMesConnectInfo.meMESModel = mesOnline?E_ONLINE_MES:E_NOTONLINE_MES;
        msMesInfo.mstrTaskNumber = JGW::JGW_W2A_W(taskNumber);
        msMesInfo.mstrWorkOrderNumber = JGW::JGW_W2A_W(workOrderNumber);
        msMesInfo.msMesConnectInfo.mstrUsername = JGW::JGW_W2A_W(userName);
        msMesInfo.msMesConnectInfo.mstrUserpwd = JGW::JGW_W2A_W(password);
        msMesInfo.mstrShiftID = (0 == shiftID)?"310001":"310002";
    }

    void CCJGWMainDlg_Net::UpdatePluginInfo(const wchar_t* stationID,const wchar_t* clsid,int viewloader_id,const wchar_t* moduleFolder,const wchar_t* pluginName,const wchar_t* station_name,int image_id)
    {
        msMesInfo.mstrOperationId = JGW::JGW_W2A_W(stationID);
        msMesInfo.msMesConnectInfo.meMESStatus = E_MES_OK_STATUS;
        msPluginInfo.mStationID  = stationID;
        msPluginInfo.mImageGroupID = image_id;
        msPluginInfo.mpViewLaoderInfo = &mcZQTViewLayoutConfig.GetUIProjectInfo()->msViewLoaderInfo;
        msPluginInfo.mstrClsid = clsid;
        msPluginInfo.mstrModuleFolderName = moduleFolder;
        msPluginInfo.mstrPluginName = pluginName;
        msPluginInfo.mstrStationName = station_name;
        msPluginInfo.mnViewLoadID = viewloader_id;

        SetTimer(GetHWND(),WM_TIMER_CLICK_SUB_WINDOWS,200,NULL);
    }

    void CCJGWMainDlg_Net::ShowLogControl()
    {
        OnResponceViewLogInfoMsg(mpLogHorizontalLayout->IsVisible()?0:1,NULL);
    }

    void CCJGWMainDlg_Net::ShowLogDialog()
    {
        TNotifyUI msg;
        OnMaximizeLogLayoutBtnClick(msg);
    }

    void CCJGWMainDlg_Net::StartRemoteLogSocket(bool bOpen)
    {
        START_REMOTELOG_SOCKET(bOpen);
    }

    void CCJGWMainDlg_Net::CloseWin32Window()
    {
        Close();
    }

    void CCJGWMainDlg_Net::UnLoadWin32Window()
    {

    }

    HWND CCJGWMainDlg_Net::LoadWin32Window(HWND hMainWnd,DWORD dwUsbDevicePoolThreadID)
    {
        LoadMESConfig(msMesInfo);
        m_hParentHwnd = hMainWnd;
        m_sCommonWndMsgInfo.muiDevicePoolThreadID = dwUsbDevicePoolThreadID;
#ifdef USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        return CreateNoBorderStyleWindow(_T(""),_T("JaGuarWaveTestPlatform_NET.xml"),_T("JaGuarWave ProductTest Tools Platform"),hMainWnd,WS_CHILD | WS_VISIBLE,0);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        return CreateNoBorderStyleWindow(_T(""),_T("JaGuarWaveTestPlatform_NET.xml"),_T("JaGuarWave ProductTest Tools Platform),hMainWnd,WS_CHILD | WS_VISIBLE,0);
#else
        return CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("JaGuarWaveTestPlatform_NET.xml"),_T("JaGuarWave ProductTest Tools Platform"),hMainWnd,WS_CHILD | WS_VISIBLE,0);
#endif
    }

    bool CCJGWMainDlg_Net::LoadMESConfig(S_MES_INFO& msMesInfo)
    {
        msMesInfo.msMesConnectInfo.meMESModel = E_ONLINE_MES;
        msMesInfo.msMesConnectInfo.meMESStatus = E_MES_UNKNOWN_STATUS;
        msMesInfo.msMesConnectInfo.mstrMESConfigFilePath = JGW::JGW_GetApplicationFolder();
        msMesInfo.msMesConnectInfo.mstrMESConfigFilePath += L"jgw_mes_config.data";
        msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig = JGW::JGW_GetApplicationFolder();
        msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig += L"jgw_mes_workstage.xml";
        Log4WD_F(L"MES Config File Path: %s,MES Work Stage Config File Path: %s",msMesInfo.msMesConnectInfo.mstrMESConfigFilePath.c_str(),msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig.c_str());

        JGW::CCJGW_MESConfig config;
        config.LoadMESWorkStageXmlConfig(msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig,&msMesInfo);
        if (!config.LoadMESIniConfig(msMesInfo.msMesConnectInfo.mstrMESConfigFilePath,&msMesInfo))
        {
            ::MessageBox(NULL,_T("MES配置文件加载失败(jgw_mes_config.data)"),_T("MES Config"),MB_ICONERROR);
            return false;
        }
        return true;
    }

}