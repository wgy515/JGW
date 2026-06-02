#include "StdAfx.h"
#include "CJGW_MESConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_MESConfig.h"
#include <JaGuarWaveTestPlatformHelp/CCJGW_HelpThreadImpl.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

namespace JGW
{
    CCJGW_MESConfigDlg::CCJGW_MESConfigDlg(HWND hParentHwnd /* = NULL */) : m_hParentHwnd(hParentHwnd),mpsMesInfo(NULL)
    {
    }


    CCJGW_MESConfigDlg::~CCJGW_MESConfigDlg(void)
    {
    }

	void CCJGW_MESConfigDlg::OnCloseWindow()
	{
		m_PaintManager.RemovePreMessageFilter(this);
	}

    UINT CCJGW_MESConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    int CCJGW_MESConfigDlg::DoModal(LPS_MES_INFO psMesInfo)
    {
        mpsMesInfo = psMesInfo;
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("mes_config.xml"),_T("MES Confif"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("mes_config.xml"),_T("MES Confif"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("mes_config.xml"),_T("MES Confif"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_MESConfigDlg::OnInitWindow()
    {
        bool bSucceseful = true;
		m_PaintManager.AddPreMessageFilter(this);
        mpOkOptionUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"mesOkBtn"));
        mpCancelOptionUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"mesCancelBtn"));
#ifdef GET_INFO_WORK_ORDER_NUMBER
        mpWorkOrderNumberEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"workOrderNumberEdt"));
#else
        mpTaskNumberEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"taskNumberEdt"));
#endif
        mpUserNameEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"userNameEdt"));
        mpUserPasswordEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"userPasswordEdt"));
        mpBarcodeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"barcodeEdt"));
        mpClassCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"classCombo"));
        mpSaveAccountCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"saveAccountCheckBox"));
        mpDurationTimeCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"durationTimeCombo"));
        mpStationCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"stationCombo"));
        mpMonoComboBoxUI = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"MonoComboBoxUI"));
        bSucceseful &= (NULL != mpOkOptionUI);
        bSucceseful &= (NULL != mpCancelOptionUI);
#ifdef GET_INFO_WORK_ORDER_NUMBER
        bSucceseful &= (NULL != mpWorkOrderNumberEdt);
#else
        bSucceseful &= (NULL != mpTaskNumberEdt);
#endif
        bSucceseful &= (NULL != mpUserNameEdt);
        bSucceseful &= (NULL != mpUserPasswordEdt);
        bSucceseful &= (NULL != mpBarcodeEdt);
        bSucceseful &= (NULL != mpClassCombo);
        bSucceseful &= (NULL != mpSaveAccountCheckBox);
        bSucceseful &= (NULL != mpDurationTimeCombo);
        bSucceseful &= (NULL != mpStationCombo);
        bSucceseful &= (NULL != mpMonoComboBoxUI);

        if (!bSucceseful)
        {
            Close(IDCANCEL);
            return ;
        }

        InitMESInfoUI();

        CheckLoginIsExpired();

        {
            //! mpStationCombo
            for (std::map<std::string,S_MSE_WORKSTAGE>::iterator it = mpsMesInfo->mmapMESWorkStage.begin();
                it != mpsMesInfo->mmapMESWorkStage.end();
                ++ it)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                pListLabelEle->SetText(it->second.mstrWorkStageName.c_str());
                mpStationCombo->Add(pListLabelEle);
                pListLabelEle->SetName(JGW_A2W(it->first).c_str());
                if (NULL == it->first.compare(mpsMesInfo->mstrOperationId)) mpStationCombo->SelectItem(mpStationCombo->GetCount() - 1);
            }
        }
    }
	//!白班:310001 晚班:310002
	void CCJGW_MESConfigDlg::InitMESInfoUI()
	{
		mpUserNameEdt->SetText(JGW_A2W(mpsMesInfo->msMesConnectInfo.mstrUsername).c_str());
		mpUserPasswordEdt->SetText(JGW_A2W(mpsMesInfo->msMesConnectInfo.mstrUserpwd).c_str());
#ifdef GET_INFO_WORK_ORDER_NUMBER
        mpWorkOrderNumberEdt->SetText(JGW_A2W(mpsMesInfo->mstrWorkOrderNumber).c_str());
#else
        mpTaskNumberEdt->SetText(JGW_A2W(mpsMesInfo->mstrTaskNumber).c_str());
#endif
		mpSaveAccountCheckBox->SetCheck(mpsMesInfo->msMesConnectInfo.mbSaveAccount);

		if (NULL == JGW_StrComparenoCaseStr("310001",mpsMesInfo->mstrShiftID.c_str()) 
			|| NULL == JGW_StrComparenoCaseStr(JGW_W2A_W(GetString(E_DAY_SHIFT_TYPE)).c_str()/*"白班"*/,mpsMesInfo->mstrShiftID.c_str()))
		{
			mpClassCombo->SelectItem(0);
		}
		else
		{
			mpClassCombo->SelectItem(1);
		}
	}

    void CCJGW_MESConfigDlg::CheckLoginIsExpired()
    {
        if (-1 == mpsMesInfo->msMesConnectInfo.mnSaveAccountTime
            || !mpsMesInfo->msMesConnectInfo.mbSaveAccount)
        {
            mpSaveAccountCheckBox->SetEnabled(true);
            mpDurationTimeCombo->SetEnabled(true);
            return ;
        }
        //! 登录时间已经过期
        int diffTime = static_cast<int>(JGW_DiffTime(mpsMesInfo->msMesConnectInfo.mnSaveAccountTimeStamp + mpsMesInfo->msMesConnectInfo.mnSaveAccountTime * 3600,JGW_GetTimeOfDay()));
        if (diffTime < 0)
        {
            mpSaveAccountCheckBox->SetEnabled(true);
            mpDurationTimeCombo->SetEnabled(true);
            mpUserPasswordEdt->SetText(L"");
            return ;
        }
        else
        {
            mpSaveAccountCheckBox->SetEnabled(false);
            mpDurationTimeCombo->SetEnabled(false);
        }
    }

	LRESULT CCJGW_MESConfigDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& )
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_RETURN:
				GetTaskNumberToBarcode();
				return TRUE;
			case VK_ESCAPE:
                mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_TASK_ERROR_STATUS;
				Close(IDCANCEL);
				return TRUE;
			default:
				break;
			}
		}
		return FALSE;
	}

	void CCJGW_MESConfigDlg::GetTaskNumberToBarcode()
	{
		if (8 > mpBarcodeEdt->GetText().GetLength()) return ;

        std::string strOperationId = JGW_W2A(mpStationCombo->GetItemAt(mpStationCombo->GetCurSel())->GetName().GetData());
        if (NULL == strOperationId.compare("-1"))
        {
            MessageBox(m_hWnd,GetString(E_MES_BARCODE_STATIONID),GetString(E_MES_SELECT_STATION_ID),MB_ICONERROR);
            return ;
        }
        mpsMesInfo->mstrOperationId = strOperationId;
        mpsMesInfo->msMesConnectInfo.mstrUsername = JGW_W2A_W(mpUserNameEdt->GetText().GetData());
        mpsMesInfo->msMesConnectInfo.mstrUserpwd = JGW_W2A_W(mpUserPasswordEdt->GetText().GetData());
        std::string strBarcode = JGW_W2A_W(mpBarcodeEdt->GetText().GetData()),strTaskNumber,strWorkOrderNumber;
        //!
        if (E_MONO_Pack == mpMonoComboBoxUI->GetCurSel())
        {
            if (!GetMESDataBaseAdo()->GetTestWorkOrderNumberToIMEI(strBarcode,strTaskNumber,strWorkOrderNumber))
            {
                ::MessageBox(GetHWND(),GetMESDataBaseAdo()->GetMESErrorMsgW().c_str(),GetString(E_MES_GET_TASKNUMBER_FAIL),MB_ICONERROR);
                //return;
            }		
        }
        else
        {
            //! 
            if (GetMESDataBaseAdo()->GetTestWorkOrderNumberToPCBASN(strBarcode,strTaskNumber,strWorkOrderNumber,(E_MONO_TYPE)mpMonoComboBoxUI->GetCurSel()))
            {
#ifdef GET_INFO_WORK_ORDER_NUMBER
                mpsMesInfo->mstrTaskNumber = strTaskNumber;
                mpWorkOrderNumberEdt->SetText(JGW_A2W(strWorkOrderNumber).c_str());
#else
                mpsMesInfo->mstrWorkOrderNumber = strWorkOrderNumber;
                mpTaskNumberEdt->SetText(JGW_A2W(strTaskNumber).c_str());
#endif
                return;
            }

            //! 如果通过号段，就直接查询号段对应的订单号
            if (15 == strBarcode.length() && ('1' <= strBarcode[0] && '9' >= strBarcode[0]))
            {
                if (!GetMESDataBaseAdo()->GetTestWorkOrderNumberToIMEI(strBarcode,strTaskNumber,strWorkOrderNumber))
                {
                    ::MessageBox(GetHWND(),GetMESDataBaseAdo()->GetMESErrorMsgW().c_str(),GetString(E_MES_GET_IMEI_TASKNUMBER_FAIL),MB_ICONERROR);
                    //return;
                }		
            }
            else if (12 == strBarcode.length())
            {
                if (!GetMESDataBaseAdo()->GetTestWorkOrderNumberToIMEI(strBarcode,strTaskNumber,strWorkOrderNumber))
                {
                    ::MessageBox(GetHWND(),GetMESDataBaseAdo()->GetMESErrorMsgW().c_str(),GetString(E_MES_GET_IMEI_TASKNUMBER_FAIL),MB_ICONERROR);
                    //return;
                }		
            }
            else
            {
                if (!GetMESDataBaseAdo()->GetTestWorkOrderNumberToPCBASN(strBarcode,strTaskNumber,strWorkOrderNumber))
                {
                    ::MessageBox(GetHWND(),GetMESDataBaseAdo()->GetMESErrorMsgW().c_str(),GetString(E_MES_GET_SN_TASKNUMBER_FAIL),MB_ICONERROR);
                    //return;
                }
            }
        }
#ifdef GET_INFO_WORK_ORDER_NUMBER
        mpsMesInfo->mstrTaskNumber = strTaskNumber;
        mpWorkOrderNumberEdt->SetText(JGW_A2W(strWorkOrderNumber).c_str());
#else
        mpsMesInfo->mstrWorkOrderNumber = strWorkOrderNumber;
        mpTaskNumberEdt->SetText(JGW_A2W(strTaskNumber).c_str());
#endif
    }

    void CCJGW_MESConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpOkOptionUI == msg.pSender)
            {
				OnResponseClickOKMsg();	
            }
            else if (mpCancelOptionUI == msg.pSender)
            {
#ifdef GET_INFO_WORK_ORDER_NUMBER
                if (mpsMesInfo->mstrWorkOrderNumber.empty()) mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_TASK_ERROR_STATUS;
#else
				if (mpsMesInfo->mstrTaskNumber.empty()) mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_TASK_ERROR_STATUS;
#endif
                Close(IDCANCEL);
            }
        }
    }

	void CCJGW_MESConfigDlg::OnResponseClickOKMsg()
	{
        mpsMesInfo->msMesConnectInfo.mstrUsername = JGW_W2A_W(mpUserNameEdt->GetText().GetData());
        mpsMesInfo->msMesConnectInfo.mstrUserpwd = JGW_W2A_W(mpUserPasswordEdt->GetText().GetData());
        mpsMesInfo->mstrShiftID = (0 == mpClassCombo->GetCurSel())?"310001":"310002";
        //! 如果订单发生变化，则重新连接服务器,断开更新连接服务器
#ifdef GET_INFO_WORK_ORDER_NUMBER
        if (NULL != mpsMesInfo->mstrWorkOrderNumber.compare(JGW_W2A_W(mpWorkOrderNumberEdt->GetText().GetData()))) StopHelpModuleThread();
        mpsMesInfo->mstrWorkOrderNumber = JGW_W2A_W(mpWorkOrderNumberEdt->GetText().GetData());
        //! 验证是否账号密码输入是否正确
        if (!GetMESDataBaseAdo()->LoginMES())
        {
            mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_LOGIN_ERROR_STATUS;
            ::MessageBox(GetHWND(),GetMESDataBaseAdo()->GetMESErrorMsgW().c_str(),GetString(E_MES_ACCOUNT_LOGIN_ERROR),MB_ICONERROR);
            return ;
        }

        //! GetMESDataBaseAdo()->LoginMES();
        if (mpsMesInfo->mstrWorkOrderNumber.empty())
        {
            ::MessageBoxA(GetHWND(),"当前工单号为空","工单号为空",MB_ICONERROR);
            mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_TASK_ERROR_STATUS;
            return ;
        }
        std::string strOldOperationId = mpsMesInfo->mstrOperationId;
        mpsMesInfo->mstrOperationId = "011";
        if (!GetMESDataBaseAdo()->GetTaskNameToWorkOrderNumber(mpsMesInfo->mstrWorkOrderNumber,mpsMesInfo->mstrTaskNumber) || mpsMesInfo->mstrTaskNumber.empty())
        {
            mpsMesInfo->mstrOperationId = strOldOperationId;
            ::MessageBox(GetHWND(),GetString(E_MES_GET_WorkOrderNumber_TASKNUMBER_FAIL),GetString(E_MES_GET_TASKNUMBER_FAIL),MB_ICONERROR);
            mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_TASK_ERROR_STATUS;
            return ;
        }
        mpsMesInfo->mstrOperationId = strOldOperationId;
#else
        if (NULL != mpsMesInfo->mstrTaskNumber.compare(JGW_W2A_W(mpTaskNumberEdt->GetText().GetData()))) StopHelpModuleThread();
		mpsMesInfo->mstrTaskNumber = JGW_W2A_W(mpTaskNumberEdt->GetText().GetData());

        //! 验证是否账号密码输入是否正确
        if (!GetMESDataBaseAdo()->LoginMES())
        {
            mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_LOGIN_ERROR_STATUS;
            ::MessageBoxA(GetHWND(),GetMESDataBaseAdo()->GetMESErrorMsg().c_str(),"账号登录错误",MB_ICONERROR);
            return ;
        }

        //! GetMESDataBaseAdo()->LoginMES();
        if (mpsMesInfo->mstrTaskNumber.empty())
        {
            ::MessageBoxA(GetHWND(),"当前订单号为空","订单号为空",MB_ICONERROR);
            mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_TASK_ERROR_STATUS;
            return ;
        }
#endif
        mpsMesInfo->msMesConnectInfo.meMESStatus = E_MES_OK_STATUS;
        if (mpSaveAccountCheckBox->IsEnabled())
        {
            mpsMesInfo->msMesConnectInfo.mbSaveAccount = mpSaveAccountCheckBox->GetCheck();
            if (mpsMesInfo->msMesConnectInfo.mbSaveAccount)
            {
                mpsMesInfo->msMesConnectInfo.mnSaveAccountTime = gdwSaveAccountTimes[mpDurationTimeCombo->GetCurSel()];
                mpsMesInfo->msMesConnectInfo.mnSaveAccountTimeStamp = JGW_GetTimeOfDay();
            }
        }

        CCJGW_MESConfig::SaveMESIniConfig(mpsMesInfo);
        Close(IDOK);
    }
}