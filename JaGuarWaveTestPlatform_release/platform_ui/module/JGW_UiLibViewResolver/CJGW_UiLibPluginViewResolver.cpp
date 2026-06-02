#include "StdAfx.h"
#include "CJGW_UiLibPluginViewResolver.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#define RELOAD_PLUGIN_COUNTS 3

namespace JGW
{
    CCJGWUiLibPluginViewResolver::CCJGWUiLibPluginViewResolver(void):mpParentContainer(NULL)
        ,mpPaintManager(NULL)
        ,mpSubContainer(NULL)
        ,mpZQUiLibViewResolverImpl(NULL)
        ,mAsyncWndMessage(m_hWnd)
    {
    }

    CCJGWUiLibPluginViewResolver::~CCJGWUiLibPluginViewResolver(void)
    {
        CloseSubWndPlugin();
        mZQLoadSubPlugin.DestroySubPlugin();
    }

    void CCJGWUiLibPluginViewResolver::CloseSubWndPlugin()
    {
#if 0
        if ( mpSubContainer && mpParentContainer )
        {
            //! Log4WD_F(L"CloseSubWndPlugin : %s",mpPluginInfo->mstrPluginName.c_str());
            mpParentContainer->Remove(mpSubContainer);
            mMapRegMsg.clear();
            mpSubContainer = NULL;
            if (mpPaintManager) KillTimer(mpPaintManager->GetPaintWindow(),WM_TIME_FIFO_EVENT);
        }
        mZQLoadSubPlugin.CloseSubPluginView();
#else
        mZQLoadSubPlugin.CloseSubPluginView();
        if ( mpSubContainer && mpParentContainer )
        {
            //! Log4WD_F(L"CloseSubWndPlugin : %s",mpPluginInfo->mstrPluginName.c_str());
            mpParentContainer->Remove(mpSubContainer);
            mMapRegMsg.clear();
            mpSubContainer = NULL;
            if (mpPaintManager) KillTimer(mpPaintManager->GetPaintWindow(),WM_TIME_FIFO_EVENT);
        }
#endif
    }

    bool CCJGWUiLibPluginViewResolver::CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam )
    {
        mpPluginInfo = (LPS_PLUGIN_INFO)psSubWndImplParam->mpPluginInfo;
        mpParentContainer = (CContainerUI*)psSubWndImplParam->mpMainParentContainer;
        mpPaintManager = (CPaintManagerUI*)psSubWndImplParam->mpMainPaintManager;
        psSubWndImplParam->mpMapRegMsg = &mMapRegMsg;
        ((LPS_COMMON_WND_MSG_INFO)psSubWndImplParam->mpWParam)->mpPluginPaintManager = mpPaintManager;
        ((LPS_COMMON_WND_MSG_INFO)psSubWndImplParam->mpWParam)->masync_message_class = &mAsyncWndMessage;
        m_hWnd = mpPaintManager->GetPaintWindow();
       
        if ( !mpPluginInfo || !mpPaintManager || !mpParentContainer )
        {
            Log4WE_F(L"CreateSubWnd The subform initialization parameter cannot be null mpPluginInfo:%d,mpPaintUi:%d,mpParentContainer:%d",mpPluginInfo,mpPaintManager,mpParentContainer);
            return false;
        }
        Log4WD_F(L"CreateSubWnd : %s",mpPluginInfo->mstrPluginName.c_str());
        //! 加载当前子窗体接口
        mpZQUiLibViewResolverImpl = mZQLoadSubPlugin.LoadSubPluginSubWndImpl( mpPluginInfo );
        if ( !mpZQUiLibViewResolverImpl )
        {
            Log4WE_F( L"CreateSubWnd LoadSubPluginSubWndImpl cannot be null mstrPluginName:%s,mstrClsid:%s",mpPluginInfo->mstrPluginName.c_str(),mpPluginInfo->mstrClsid.c_str() );
            return false;
        }
        //! 创建子窗体视图
        return CreateDuiLibSubWnd( psSubWndImplParam );
    }

    bool CCJGWUiLibPluginViewResolver::CreateDuiLibSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam )
    {
        const S_PLUGIN_SKIN* const pPluginSkin = mpZQUiLibViewResolverImpl->GetSubWndPluginSkin();
        CDialogBuilder builder;

        Log4WD_F(L"CreateDuiLibSubWnd Plugin Version :%s  Plugin Skin :%s",pPluginSkin->mstrPluginVersions.c_str(),pPluginSkin->mstrSkinXmlPath.c_str());
        ::SendMessage( mpPaintManager->GetPaintWindow(),WM_RESPONCE_PLUGIN_VERSION_MSG_DEFINE,WPARAM(pPluginSkin->mstrPluginVersions.c_str()),NULL);
        mpSubContainer = builder.Create(pPluginSkin->mstrSkinXmlPath.c_str(),NULL,NULL,mpPaintManager,NULL);
        if ( !mpSubContainer )
        {
            Log4WE_F(L"CCJGWUiLibPluginViewResolver::CreateDuiLibSubWnd CDialogBuilder Create fail(mstrSkinXmlPath:%s)",pPluginSkin->mstrSkinXmlPath.c_str());
            return false;
        }
        mpParentContainer->Add(mpSubContainer);
        //! mpSubContainer->SetManager(mpPaintManeger,mpParentContainer);
        
        SetTimer(mpPaintManager->GetPaintWindow(),WM_TIME_FIFO_EVENT,FIFO_EVENT_INTERVAL_TIME,NULL);
        return mpZQUiLibViewResolverImpl->InitSubWndPlugin( (LPS_VIEW_RESOLVER_IMPL_PARAM)psSubWndImplParam );
    }


    void CCJGWUiLibPluginViewResolver::OnMsgNotify(LPVOID pNotifyMsg)
    {
        if ( mpZQUiLibViewResolverImpl ) mpZQUiLibViewResolverImpl->OnNotifySubWndPlugin( pNotifyMsg );
    }

    void CCJGWUiLibPluginViewResolver::OnParentCustomMessage( const UINT& uMsg,const WPARAM& wParam,const LPARAM& lParam )
    {
        if (WM_TIMER == uMsg && WM_TIME_FIFO_EVENT == (int)wParam) ExecuteFIFOEventMessage();
        //! mMapRegMsg
        if ( mMapRegMsg.end() != mMapRegMsg.find(uMsg) && mpZQUiLibViewResolverImpl )
        {
            LPOnResponceMsgFun OnResponceMsg = mMapRegMsg[uMsg];
            (mpZQUiLibViewResolverImpl->*OnResponceMsg)( wParam,lParam ) ;
        }
    }

    void CCJGWUiLibPluginViewResolver::ExecuteFIFOEventMessage()
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
                //! p_execute_msg_queue->pop();
            }
        });
    }
}