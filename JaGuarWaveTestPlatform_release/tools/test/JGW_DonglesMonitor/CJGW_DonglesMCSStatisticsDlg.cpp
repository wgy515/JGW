#include "StdAfx.h"
#include "CJGW_DonglesMCSStatisticsDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "resource.h"
namespace JGW
{
    CCJGW_DonglesMCSStatisticsDlg::CCJGW_DonglesMCSStatisticsDlg(ULONGLONG (&uMcsStatistics)[13],HWND hParentHwnd /* = NULL */) : muMcsStatistics(uMcsStatistics),m_hParentHwnd(hParentHwnd)
    {
    }


    CCJGW_DonglesMCSStatisticsDlg::~CCJGW_DonglesMCSStatisticsDlg(void)
    {
    }

    void CCJGW_DonglesMCSStatisticsDlg::OnCloseWindow()
    {
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_DonglesMCSStatisticsDlg::OnEventCustomMessage,0);
    }

    void CCJGW_DonglesMCSStatisticsDlg::OnInitWindow()
    {
        mpResetStatisticsBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"resetBtn"));
        std::wstring strTemp;
        for (int i = 0;i <= 12;i ++)
        {
            JGW_FormatWString(strTemp,L"mcs%d",i);
            mpMcsLab[i] = static_cast<CLabelUI*>(m_PaintManager.FindControl(strTemp.c_str()));
            if (NULL == mpMcsLab[i]) { Close(IDCANCEL); return ;}
        }
        {
            OnUpdateMCS();
        }

        {
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_DonglesMCSStatisticsDlg::OnEventCustomMessage,0);
        }
    }

    void CCJGW_DonglesMCSStatisticsDlg::OnUpdateMCS()
    {
        std::wstring strTemp;
        for (int i = 0;i <= 12;i ++)
        {
            JGW_FormatWString(strTemp,L"%I64u",muMcsStatistics[i]);
            mpMcsLab[i]->SetText(strTemp.c_str());
        }
    }

    int CCJGW_DonglesMCSStatisticsDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("mcs_counst.xml"),_T("MES Statistics"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\DonglesMonitor.zip");
        CreateNoBorderStyleWindow(_T(""),_T("mcs_counst.xml"),_T("MES Statistics"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\DonglesMonitor"),_T("mcs_counst.xml"),_T("MES Statistics"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
       //! CenterWindow();

        return ShowModal();
    }

    bool CCJGW_DonglesMCSStatisticsDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
         if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
         switch (pTEventUI->Type)
         {
         case WM_TIMER:
             if (WM_TIMER_UPDATE_TIME == (int)pTEventUI->wParam) OnUpdateMCS();
         }
         return true;
    }

    void CCJGW_DonglesMCSStatisticsDlg::OnNotify(TNotifyUI& msg)
    {
        if(msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpResetStatisticsBtn)
            {
                for (int i = 0;i <= 12;i ++)
                {
                    muMcsStatistics[i] = 0;
                }
                OnUpdateMCS();
            }
        }
    }
}
