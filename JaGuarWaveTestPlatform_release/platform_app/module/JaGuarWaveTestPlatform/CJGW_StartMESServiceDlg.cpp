#include "StdAfx.h"
#include "CJGW_StartMESServiceDlg.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCJGW_StartMESServiceDlg::CCJGW_StartMESServiceDlg(HWND hParentHwnd /* = NULL */) : m_hParentHwnd(hParentHwnd)
    {
    }


    CCJGW_StartMESServiceDlg::~CCJGW_StartMESServiceDlg(void)
    {
    }

    UINT CCJGW_StartMESServiceDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

	void CCJGW_StartMESServiceDlg::OnCloseWindow()
	{
		m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_StartMESServiceDlg::OnEventCustomMessage,0);
	}

	void CCJGW_StartMESServiceDlg::OnNotify(TNotifyUI& msg)
	{

    }

    void CCJGW_StartMESServiceDlg::StartMESServiceApplication()
    {
        //! 检查MES服务是否已经加载成功
        if (-1 == JGW_GetProcessID2Name(MES_SERVICE_APPLICATION_NAME))
        {
            std::wstring strTemp = JGW_GetApplicationFolder();
            strTemp += MES_SERVICE_APPLICATION_NAME;
            if (NULL == JGW_CreateProcess(strTemp.c_str()))
            {
                mpMESTipInfo->SetText(GetString(E_MES_SERVICE_APPLICATION_FAILED_TO_START));
                mpMESTipInfo->SetTextColor(0xFF0000);
            }
            SetTimer(m_hWnd,WM_TIMER_CHECK_MES_APPLICATION_STATR,3000,NULL);
        }
        else
        {
            mpMESTipInfo->SetText(GetString(E_MES_SERVICE_APPLICATION_SUCCESSFULLY_TO_START));
            SetTimer(m_hWnd,WM_TIMER_CHECK_MES_APPLICATION_STATR,1000,NULL);
        }
    }

    void CCJGW_StartMESServiceDlg::OnInitWindow()
    {
        {
            mpMESTipInfo = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"mesTipInfoLab"));
            if (NULL == mpMESTipInfo) Close(IDCANCEL);
            mpMESTipInfo->SetText(GetString(E_MES_STARTING_SERVICE_PLEASE_WAIT));
            mpMESTipInfo->SetTextColor(0xFF0000);
        }

        {
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_StartMESServiceDlg::OnEventCustomMessage,0);
        }

        StartMESServiceApplication();   
    }

    int CCJGW_StartMESServiceDlg::DoModal()
    {
        //! m_hParentHwnd = hParentHwnd; 
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("start_mes_service_view.xml"),_T("MES Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("start_mes_service_view.xml"),_T("MES Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("start_mes_service_view.xml"),_T("MES Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();
      //  SetWindowPos(m_PaintManager.GetPaintWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
        return ShowModal();
    }

    bool CCJGW_StartMESServiceDlg::OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam )
    {
        UINT uMsg = pTEventUI->Type;
        switch ( uMsg )
        {
        case WM_TIMER:
            if (WM_TIMER_CHECK_MES_APPLICATION_STATR == pTEventUI->wParam) 
            { 
                KillTimer(m_hWnd,WM_TIMER_CHECK_MES_APPLICATION_STATR); 
                Close(IDOK);
            }
            break;
        }

        return true;
    }
}
