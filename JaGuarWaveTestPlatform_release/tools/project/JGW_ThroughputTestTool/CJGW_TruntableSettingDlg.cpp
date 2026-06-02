#include "StdAfx.h"
#include "CJGW_TruntableSettingDlg.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_TruntableSettingDlg::CCJGW_TruntableSettingDlg(S_IperfToolConfig& sIperfToolConfig,SCLLibHelper& cSCLLibHelper,HWND hParentHwnd /* = NULL */)
        : mhParentHwnd(hParentHwnd),mcSCLLibHelper(cSCLLibHelper),msIperfToolConfig(sIperfToolConfig),mnPosition(1)
    {
    }


    CCJGW_TruntableSettingDlg::~CCJGW_TruntableSettingDlg(void)
    {
    }

    void CCJGW_TruntableSettingDlg::OnInitWindow()
    {
        //! init control ptr
        {
            bool bSuccess = true;
            mpVelocityEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"velocityEdit"));
            mpAccelerationEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"accelerationEdit"));
            mpDecelerationEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"decelerationEdit"));
            mpPositionEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"positionEdit"));
            mpStartBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"startBtn"));
            mpStopBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"stopBtn"));
            mpApplyBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"applyBtn"));
            bSuccess &= (NULL != mpVelocityEdit);
            bSuccess &= (NULL != mpAccelerationEdit);
            bSuccess &= (NULL != mpDecelerationEdit);
            bSuccess &= (NULL != mpPositionEdit);
            bSuccess &= (NULL != mpStartBtn);
            bSuccess &= (NULL != mpStopBtn);
            bSuccess &= (NULL != mpApplyBtn);
            if (!bSuccess)
            {
                Close();
                return ;
            }
            mpStopBtn->SetEnabled(false);
        }
        //!
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"%.0f",msIperfToolConfig.msTrunTableParam.mfVelocity);
            mpVelocityEdit->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"%.0f",msIperfToolConfig.msTrunTableParam.mfAcceleration);
            mpAccelerationEdit->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"%.0f",msIperfToolConfig.msTrunTableParam.mfDeceleration);
            mpDecelerationEdit->SetText(strTemp.c_str());
        }
    }

    void CCJGW_TruntableSettingDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpStartBtn)
            {
                OnStartPositionRotate();
            }
            else if (msg.pSender == mpStopBtn)
            {
                OnStopPositionRotate();
            }
            else if (msg.pSender == mpApplyBtn)
            {
                UpdateConfigParam();
                Close();
            }
        }
    }

    void CCJGW_TruntableSettingDlg::OnCloseWindow()
    {

    }

    int CCJGW_TruntableSettingDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("turntable_setting.xml"),_T("Turntable Setting"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\IperfTool.zip");
        CreateNoBorderStyleWindow(_T(""),_T("turntable_setting.xml"),_T("Turntable Setting"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\IperfTool"),_T("turntable_setting.xml"),_T("Turntable Setting"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }
    //! FTSER2K FTDIBUS\COMPORT&VID_0403&PID_6001
    int CCJGW_TruntableSettingDlg::FindTruntableComPort()
    {
        std::wstring strComPort;
        strComPort = JGW_GetComPortToUsbServiceName(L"FTSER2K",L"VID_0403");
        if (strComPort.empty()) return 0;
        JGW_ReplaceStringW(strComPort,L"COM",L"");
        return _ttoi(strComPort.c_str());
    }

    void CCJGW_TruntableSettingDlg::UpdateConfigParam()
    {
        mnPosition = _ttoi(mpPositionEdit->GetText().GetData());
        msIperfToolConfig.msTrunTableParam.mfAcceleration = _ttof(mpAccelerationEdit->GetText().GetData());
        msIperfToolConfig.msTrunTableParam.mfDeceleration = _ttof(mpDecelerationEdit->GetText().GetData());
        msIperfToolConfig.msTrunTableParam.mfVelocity = _ttof(mpVelocityEdit->GetText().GetData());
    }

    void CCJGW_TruntableSettingDlg::OnStartPositionRotate()
    {
        UpdateConfigParam();
        if (!mcSCLLibHelper.IsOpen()) return ;
        mpStartBtn->SetEnabled(true);
        mpStopBtn->SetEnabled(true);
        //! 32
        mcSCLLibHelper.RelMove(TURNTABLE_NODE_ID,mnPosition * PER_ANGLE_TURN,&msIperfToolConfig.msTrunTableParam.mfVelocity,&msIperfToolConfig.msTrunTableParam.mfAcceleration,&msIperfToolConfig.msTrunTableParam.mfDeceleration);

    }

    void CCJGW_TruntableSettingDlg::OnStopPositionRotate()
    {
        mcSCLLibHelper.WriteStopAndKill(TURNTABLE_NODE_ID);
    }
}