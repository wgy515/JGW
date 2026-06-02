#include "StdAfx.h"
#include "CJGW_TrunTableRangeConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_TrunTableRangeConfigDlg::CCJGW_TrunTableRangeConfigDlg(S_TRUNTABLE_PARAM& sTruntableParam,HWND hParentHwnd /* = NULL */) : msTruntableParam(sTruntableParam),mhParentHwnd(hParentHwnd)
    {
    }


    CCJGW_TrunTableRangeConfigDlg::~CCJGW_TrunTableRangeConfigDlg(void)
    {
    }

    int CCJGW_TrunTableRangeConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("range_config.xml"),_T("Turntable Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\IperfTool.zip");
        CreateNoBorderStyleWindow(_T(""),_T("range_config.xml"),_T("Turntable Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\IperfTool"),_T("range_config.xml"),_T("Turntable Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_TrunTableRangeConfigDlg::OnInitWindow()
    {
        //! init control ptr
        {
            bool bSuccess = true;
            mpStartEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"startEdit"));
            mpEndEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"endEdit"));
            mpRangeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"stepEdit"));
            mpTipslab = static_cast<CEditUI*>(m_PaintManager.FindControl(L"tipslab"));
            bSuccess &= (NULL != mpStartEdt);
            bSuccess &= (NULL != mpEndEdt);
            bSuccess &= (NULL != mpRangeEdt);
            bSuccess &= (NULL != mpTipslab);

            if (!bSuccess)
            {
                Close();
                return ;
            }
            mpTipslab->SetText(L" 转台配置");
        }
        UpdateTrunTableParamUI();
    }

    void CCJGW_TrunTableRangeConfigDlg::OnCloseWindow()
    {

    }

    void CCJGW_TrunTableRangeConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"applyBtn")
            {
                msTruntableParam.mTurnTableAngleStart = _ttoi(mpStartEdt->GetText().GetData());
                msTruntableParam.mTurnTableAngleEnd = _ttoi(mpEndEdt->GetText().GetData());
                msTruntableParam.mTurnTableStep = _ttoi(mpRangeEdt->GetText().GetData());

                //if (msTruntableParam.mTurnTableStep <= 0) msTruntableParam.mTurnTableStep = 5;
                //if (msTruntableParam.mTurnTableAngleStart <= 0) msTruntableParam.mTurnTableAngleStart = 0;
                //if (msTruntableParam.mTurnTableAngleEnd <= 0) msTruntableParam.mTurnTableAngleEnd = 0;
                //if (msTruntableParam.mTurnTableAngleStart > msTruntableParam.mTurnTableAngleEnd) msTruntableParam.mTurnTableAngleStart = msTruntableParam.mTurnTableAngleEnd;
                //! 步进不能等于0
                if (0 == msTruntableParam.mTurnTableStep)
                {
                    msTruntableParam.mTurnTableStep = 5;
                }
                //! 如果步进大于0 则起始角度必须小于末尾角度
                if (msTruntableParam.mTurnTableStep > 0)
                {
                    if (msTruntableParam.mTurnTableAngleStart > msTruntableParam.mTurnTableAngleEnd) 
                    {
                        MessageBox(mhParentHwnd,L"当转台步进大于0，起始角度必须小于末尾角度",L"配置错误",MB_ICONERROR);
                        return ;
                    }
                }
                //! 如果步进小于0 则起始角度必须大于于末尾角度
                else
                {
                    if (msTruntableParam.mTurnTableAngleStart < msTruntableParam.mTurnTableAngleEnd) 
                    {
                        MessageBox(mhParentHwnd,L"当转台步进小于0，则起始角度必须大于于末尾角度",L"配置错误",MB_ICONERROR);
                        return ;
                    }
                }

                Close(IDOK);
            }
            else if (msg.pSender->GetName() == L"closebtn")
            {
                Close(IDOK);
            }
        }
    }

    void CCJGW_TrunTableRangeConfigDlg::UpdateTrunTableParamUI()
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"%d",msTruntableParam.mTurnTableAngleStart);
        mpStartEdt->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%d",msTruntableParam.mTurnTableAngleEnd);
        mpEndEdt->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%d",msTruntableParam.mTurnTableStep);
        mpRangeEdt->SetText(strTemp.c_str());
    }
}