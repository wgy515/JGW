#include "StdAfx.h"
#include "CWelcomViewDlg.h"

#define SUB_PLUGIN_VERSION L"Jaguar Wave Technology Home Page V1R01"
namespace JGW
{
    CCWelcomViewDlg::CCWelcomViewDlg(void)
    {
    }


    CCWelcomViewDlg::~CCWelcomViewDlg()
    {
        CloseSubWndPlugin();
    }

    bool CCWelcomViewDlg::CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam )
    {
        mpPluginInfo = (LPS_PLUGIN_INFO)psSubWndImplParam->mpPluginInfo;
        mpParentContainer = (CContainerUI*)psSubWndImplParam->mpMainParentContainer;
        mpPaintManeger = (CPaintManagerUI*)psSubWndImplParam->mpMainPaintManager;

        if ( !mpPluginInfo || !mpPaintManeger || !mpParentContainer )
        {
            //CTFLogWE_F(L"CCWelcomViewDlg::CreateSubWnd子窗体初始化参数不能为空 mpPluginInfo:%d,mpPaintUi:%d,mpParentContainer:%d",mpPluginInfo,mpPaintManeger,mpParentContainer);
            return false;
        }

        CDialogBuilder builder;

        ::SendMessage( mpPaintManeger->GetPaintWindow(),WM_RESPONCE_PLUGIN_VERSION_MSG_DEFINE,WPARAM(SUB_PLUGIN_VERSION),NULL);
        mpSubContainer = builder.Create( L"WelcomDlg.xml",NULL,NULL,mpPaintManeger,mpParentContainer );
        if ( !mpSubContainer )
        {
            //CTFLogWE_F(L"CCTFUiLibPluginViewResolver::CreateDuiLibSubWnd CDialogBuilder Create fail(mstrSkinXmlPath:%s)",L"WelcomDlg.xml");
            return false;
        }
        mpSubContainer->SetManager(mpPaintManeger,mpParentContainer);
        return true;
    }

    void CCWelcomViewDlg::OnMsgNotify(LPVOID pNotifyMsg)
    {

    }

    void CCWelcomViewDlg::CloseSubWndPlugin()
    {
        if ( mpSubContainer && mpParentContainer )
        {
            mpParentContainer->Remove(mpSubContainer);
            mpSubContainer = NULL;
        }
    }
}

