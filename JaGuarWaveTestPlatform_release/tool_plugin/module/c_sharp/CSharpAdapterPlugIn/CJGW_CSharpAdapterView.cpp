#include "StdAfx.h"
#include "CJGW_CSharpAdapterView.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    RECT rect = {0};
    CCJGW_CSharpAdapterView::CCJGW_CSharpAdapterView(void) : mhCSharpAdapterExecWindowHandle(NULL),mpViewResolverParam(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"empty.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCJGW_CSharpAdapterView::~CCJGW_CSharpAdapterView(void)
    {
    }

    void CCJGW_CSharpAdapterView::CloseSubWndPlugin()
    {
        mhCSharpAdapterExecWindowHandle = NULL;
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    bool CCJGW_CSharpAdapterView::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        HANDLE hPluginWnd; 
        mpViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
        CPaintManagerUI* pPaintManager = (CPaintManagerUI*)mpViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
        hPluginWnd = pPaintManager->GetPaintWindow();
        //! rect = mpViewResolverParam->mpMainParentContainer->GetPos();
        //! QiaSampleApp.exe
        std::wstring strApplicationFile = JGW_GetExecConfigFilePath(L"QiaSampleApp.exe");
        std::wstring strParam;
        JGW_FormatWString(strParam,L"%d",(int)hPluginWnd);
        ShellExecute(NULL,L"open",strApplicationFile.c_str(),strParam.c_str(),JGW_GetApplicationFolder(),SW_NORMAL);

        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_PAINT,&CCJGW_CSharpAdapterView::OnPaint);
        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_CSHARP_ADAPTER_VIEW_REPORT_HWND,&CCJGW_CSharpAdapterView::OnCsharpAdapterViewReportHwnd);


        return true;
    }

    void CCJGW_CSharpAdapterView::OnNotifySubWndPlugin(LPVOID pMsg)
    {

    }
    //! ¸üÐÂ
    void CCJGW_CSharpAdapterView::OnPaint(const WPARAM& wParam,const LPARAM& lParam)
    {
        if ( rect.bottom != mpViewResolverParam->mpMainParentContainer->GetPos().bottom || rect.top != mpViewResolverParam->mpMainParentContainer->GetPos().top
            || rect.left != mpViewResolverParam->mpMainParentContainer->GetPos().left || rect.right != mpViewResolverParam->mpMainParentContainer->GetPos().right )
        {
            ReSizeCSharpAdapterHwnd();
        }
    }

    void CCJGW_CSharpAdapterView::CloseCSharpAdapterHwnd()
    {
        if (NULL != mhCSharpAdapterExecWindowHandle)
        {
            SendMessage(mhCSharpAdapterExecWindowHandle,WM_CLOSE_CSHARP_ADAPTER_VIEW,NULL,NULL);
        }
    }

    void CCJGW_CSharpAdapterView::ReSizeCSharpAdapterHwnd()
    {
        if (NULL != mhCSharpAdapterExecWindowHandle)
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"%d,%d,%d,%d",mpViewResolverParam->mpMainParentContainer->GetPos().left + 5
                ,mpViewResolverParam->mpMainParentContainer->GetPos().top + 2
                ,mpViewResolverParam->mpMainParentContainer->GetPos().right - mpViewResolverParam->mpMainParentContainer->GetPos().left - 10
                ,mpViewResolverParam->mpMainParentContainer->GetPos().bottom - mpViewResolverParam->mpMainParentContainer->GetPos().top - 10);

            COPYDATASTRUCT copyData;
            copyData.lpData = (LPVOID)strTemp.c_str();
            copyData.cbData = strTemp.size() * 2;

            ::SendMessage(mhCSharpAdapterExecWindowHandle, WM_COPYDATA, NULL, (LPARAM)&copyData);

            //! SendMessage(mhCSharpAdapterExecWindowHandle,WM_RESIZE_CSHARP_ADAPTER_VIEW,WPARAM(strTemp.c_str()),NULL);
            /*
            MoveWindow(GetHWND(),mpViewResolverParam->mpMainParentContainer->GetPos().left + 5 ,mpViewResolverParam->mpMainParentContainer->GetPos().top + 2
            ,mpViewResolverParam->mpMainParentContainer->GetPos().right - mpViewResolverParam->mpMainParentContainer->GetPos().left - 10
            ,mpViewResolverParam->mpMainParentContainer->GetPos().bottom - mpViewResolverParam->mpMainParentContainer->GetPos().top - 10,TRUE );
            */
            rect = mpViewResolverParam->mpMainParentContainer->GetPos();
        }  
    }

    void CCJGW_CSharpAdapterView::OnCsharpAdapterViewReportHwnd(const WPARAM& wParam,const LPARAM& lParam)
    {
        mhCSharpAdapterExecWindowHandle = (HWND)wParam;
    }


}

