#include "StdAfx.h"
#include "CJGW_UiLibViewResoverDialog.h"

namespace JGW
{
    CCJGWUiLibViewResoverDialog::CCJGWUiLibViewResoverDialog(void):mpParentContainer(NULL),mpSubContainer(NULL),mpParentPaintUi(NULL),mAsyncWndMessage(m_hWnd)
    {
    }


    CCJGWUiLibViewResoverDialog::~CCJGWUiLibViewResoverDialog(void)
    {
        CloseSubWndPlugin();
		mZQLoadSubPlugin.DestroySubPlugin();
    }

    void CCJGWUiLibViewResoverDialog::OnCloseWindow()
    {
        CloseSubWndPlugin();
    }

    void CCJGWUiLibViewResoverDialog::CloseSubWndPlugin()
    {
        mMapRegMsg.clear();
        if ( mpSubContainer && mpParentContainer )
        {
            m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGWUiLibViewResoverDialog::OnEventCustomMessage,0);
            KillTimer(GetHWND(),WM_TIME_FIFO_EVENT);
            mpEmptySubContainer->SetDelayedDestroy(false);
            mpEmptySubContainer->Remove(mpSubContainer);
            mpEmptySubContainer->RemoveAll();       
            Log4WD(_T("CCJGWUiLibViewResoverDialog::CloseSubWnd Remove mpSubContainer"));
            mpParentContainer = NULL;
            mpSubContainer = NULL;
        }
        mZQLoadSubPlugin.CloseSubPluginView();
        if (NULL != m_hWnd && ::IsWindow(m_hWnd))
        {
            Close(IDCANCEL);//! ::SetWindowLongPtr(GetHWND(), GWLP_USERDATA,LPARAM(NULL));
            Log4WD(_T("CCJGWUiLibViewResoverDialog::CloseSubWnd Close Window"));
            DestroyWindow();
        }
    }

    LPCTSTR CCJGWUiLibViewResoverDialog::GetWindowClassName() const
    {
        return _T("CZQUiLibViewResoverDialog_UiLib");
    }

    void CCJGWUiLibViewResoverDialog::Notify(TNotifyUI& msg)
    {
        if ( mpZQUiLibViewResolverImpl ) mpZQUiLibViewResolverImpl->OnNotifySubWndPlugin( &msg );
    }

    bool CCJGWUiLibViewResoverDialog::InitWindow()
    {
        mpEmptySubContainer =  static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("emptysubLayout")));
        Log4WD_F(_T("CCJGWUiLibViewResoverDialog::InitWindow mpEmptySubContainer:%d"),mpEmptySubContainer);
        _ASSERT(mpEmptySubContainer);
        if ( !mpEmptySubContainer ) goto INITSUBWND_FAIL;

        mpZQUiLibViewResolverImpl = mZQLoadSubPlugin.LoadSubPluginSubWndImpl( mpPluginInfo );
        if ( !mpZQUiLibViewResolverImpl ) goto INITSUBWND_FAIL;

        if ( CreateDuiLibSubWnd() )
        {
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGWUiLibViewResoverDialog::OnEventCustomMessage,0);
            return true;
        }
        else
        {
            MessageBox(GetHWND(),L"¼ÓÔØ´´½¨ÊÔÍ¼Ê§°Ü£¡£¡£¡",L"³õÊ¼»¯Ê§°Ü",MB_ICONERROR);
        }

INITSUBWND_FAIL:
        Close(IDCANCEL);
        return false;
    }

    bool CCJGWUiLibViewResoverDialog::CreateDuiLibSubWnd()
    {
        const S_PLUGIN_SKIN* const pPluginSkin = mpZQUiLibViewResolverImpl->GetSubWndPluginSkin();
        CDialogBuilder builder;

        ::SendMessage( mpParentPaintUi->GetPaintWindow(),WM_RESPONCE_PLUGIN_VERSION_MSG_DEFINE,WPARAM(pPluginSkin->mstrPluginVersions.c_str()),NULL);
        Log4WD_F(_T("CCJGWUiLibViewResoverDialog::CreateDuiLibSubWnd pPluginSkin:%d"),pPluginSkin);
        mpSubContainer = builder.Create(pPluginSkin->mstrSkinXmlPath.c_str(),NULL,NULL,&m_PaintManager);
        //! mpSubContainer->SetManager(&m_PaintManager,mpParentContainer);
        if ( !mpSubContainer ) return false;	
        mpEmptySubContainer->Add(mpSubContainer);
        mpsSubWndImplParam->mpMapRegMsg = &mMapRegMsg;
        ((LPS_COMMON_WND_MSG_INFO)mpsSubWndImplParam->mpWParam)->mpPluginPaintManager = &m_PaintManager;
        ((LPS_COMMON_WND_MSG_INFO)mpsSubWndImplParam->mpWParam)->masync_message_class = &mAsyncWndMessage;

        return mpZQUiLibViewResolverImpl->InitSubWndPlugin( (LPS_VIEW_RESOLVER_IMPL_PARAM)mpsSubWndImplParam );
    }


    void CCJGWUiLibViewResoverDialog::ReSizeHwnd()
    {
        if ( mpParentContainer )
		{
			MoveWindow(GetHWND(),mpParentContainer->GetPos().left,mpParentContainer->GetPos().top
				,mpParentContainer->GetPos().right - mpParentContainer->GetPos().left
				,mpParentContainer->GetPos().bottom - mpParentContainer->GetPos().top,TRUE );
		}
			/*MoveWindow(GetHWND(),mpParentContainer->GetPos().left + 5 ,mpParentContainer->GetPos().top + 2
			,mpParentContainer->GetPos().right - mpParentContainer->GetPos().left - 10
			,mpParentContainer->GetPos().bottom - mpParentContainer->GetPos().top - 10,TRUE );*/
    }

    RECT rect;
    bool CCJGWUiLibViewResoverDialog::CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam )
    {
        mpsSubWndImplParam = psSubWndImplParam;
        _ASSERT(mpsSubWndImplParam);
        mpPluginInfo = (LPS_PLUGIN_INFO)psSubWndImplParam->mpPluginInfo;
        mpParentPaintUi = (CPaintManagerUI*)psSubWndImplParam->mpMainPaintManager;
        mpParentContainer = (CHorizontalLayoutUI*)psSubWndImplParam->mpMainParentContainer;
        _ASSERT(mpPluginInfo);
        _ASSERT(mpParentPaintUi);
        _ASSERT(mpParentContainer);

        rect = mpParentContainer->GetPos();
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateDuiDialogWindow(mpParentPaintUi->GetPaintWindow(),_T(""),_T("empty.xml"),UI_WNDSTYLE_CHILD);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateDuiDialogWindow(mpParentPaintUi->GetPaintWindow(),_T(""),_T("empty.xml"),UI_WNDSTYLE_CHILD);
#else
        CreateDuiDialogWindow(mpParentPaintUi->GetPaintWindow(),_T("skin\\JaGuarWave"),_T("empty.xml"),UI_WNDSTYLE_CHILD);
#endif
        ReSizeHwnd();

        SetTimer(GetHWND(),WM_TIME_FIFO_EVENT,FIFO_EVENT_INTERVAL_TIME,NULL);

        return true;
    }

    void CCJGWUiLibViewResoverDialog::OnPaint(HDC hDC, const RECT* rcPaint)
    {
        if ( rect.bottom != mpParentContainer->GetPos().bottom || rect.top != mpParentContainer->GetPos().top
            || rect.left != mpParentContainer->GetPos().left || rect.right != mpParentContainer->GetPos().right )
        {
            ReSizeHwnd();
            rect = mpParentContainer->GetPos();
        }
    }

    void CCJGWUiLibViewResoverDialog::OnMsgNotify(LPVOID pNotifyMsg)
    {
        if ( mpZQUiLibViewResolverImpl ) mpZQUiLibViewResolverImpl->OnNotifySubWndPlugin( pNotifyMsg );
    }

    void CCJGWUiLibViewResoverDialog::OnParentCustomMessage(const UINT& uMsg,const WPARAM& wParam,const LPARAM& lParam)
    {
        if (uMsg == WM_ACTIVATE )
        {
            rect.left = 0;
        }
//         if (WM_HWND_TEST_SATRT_MSG == uMsg 
//             || WM_HWND_CONFIG_BUTTON_CLICK_MSG == uMsg
//             || WM_HWND_CLEAR_TEST_RECORD_BUTTON_CLICK_MSG == uMsg
//             || (WM_TIMER == uMsg &&WM_TIMER_UPDATE_TEST_TIME == (int)wParam))
//         {
//             if ( mMapRegMsg.end() != mMapRegMsg.find(uMsg) && mpZQUiLibViewResolverImpl )
//             {
//                 LPOnResponceMsgFun OnResponceMsg = mMapRegMsg[uMsg];
//                 (mpZQUiLibViewResolverImpl->*OnResponceMsg)( wParam,lParam ) ;
//             }
//         }
    }


    bool CCJGWUiLibViewResoverDialog::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        if (WM_PAINT == pTEventUI->Type) OnPaint( NULL,NULL);
        if (mpZQUiLibViewResolverImpl && WM_TIMER == pTEventUI->Type && WM_TIME_FIFO_EVENT == (int)pTEventUI->wParam) ExecuteFIFOEventMessage();
        if ( mMapRegMsg.end() != mMapRegMsg.find(pTEventUI->Type) && mpZQUiLibViewResolverImpl )
        {
            LPOnResponceMsgFun OnResponceMsg = mMapRegMsg[pTEventUI->Type];
            (mpZQUiLibViewResolverImpl->*OnResponceMsg)( pTEventUI->wParam,pTEventUI->lParam ) ;
        }
        return true;
    }

    void CCJGWUiLibViewResoverDialog::ExecuteFIFOEventMessage()
    {
        mAsyncWndMessage.ExecuteAsyncMessage([&](std::queue<S_ASYNC_MESSAGE>* p_execute_msg_queue){
            while (!p_execute_msg_queue->empty())
            {
                S_ASYNC_MESSAGE async_message = p_execute_msg_queue->front();
                p_execute_msg_queue->pop();
                if ( mMapRegMsg.end() != mMapRegMsg.find(async_message.mMsgID) )
                {
                    LPOnResponceMsgFun OnResponceMsg = mMapRegMsg[async_message.mMsgID];
                    (mpZQUiLibViewResolverImpl->*OnResponceMsg)( async_message.wParam,async_message.lParam ) ;
                }
                //p_execute_msg_queue->pop();
            }
        });
    }
}
