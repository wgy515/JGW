#include "StdAfx.h"
#include "CMultipleTestConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_DeviceQueueModule/CJGW_DeviceQueueImpl.h>
namespace JGW
{
    CCMultipleTestConfigDlg::CCMultipleTestConfigDlg(HWND hParentHwnd,PS_MULTIPLE_CONFIG psMultipleConfig) : mhParentHwnd(hParentHwnd),mpsMultipleConfig(psMultipleConfig)
    {
        mstrDownloadFileConfig = mpsMultipleConfig->mstrDownloadFileConfig;
        mstrTFTPDIPAddr = mpsMultipleConfig->mstrTFTPDIPAddr;
    }


    CCMultipleTestConfigDlg::~CCMultipleTestConfigDlg(void)
    {
    }

    int CCMultipleTestConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("multiple_test_config.xml"),_T("JaGuarWave Multiple Test Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("multiple_test_config.xml"),_T("JaGuarWave Multiple Test Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("multiple_test_config.xml"),_T("JaGuarWave Multiple Test Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    UINT CCMultipleTestConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    void CCMultipleTestConfigDlg::SetDeviceModeCombo(E_DEVICE_MODE& eDeviceMode,CComboUI* pDeviceModeCombo)
    {
        if (E_DEVICE_UNKNOWMODE == eDeviceMode) eDeviceMode = E_DIAG_MODE;
        switch (eDeviceMode)
        {
        case E_ADB_MODE:
            pDeviceModeCombo->SelectItem(0);
            break;
        case E_DIAG_MODE:
            pDeviceModeCombo->SelectItem(1);
            break;
        case E_MODEM_MODE:
            pDeviceModeCombo->SelectItem(2);
            break;
        case E_FASTBOOT_MODE:
            pDeviceModeCombo->SelectItem(3);
            break;
        case E_DOWNLOADER_MODE:
            pDeviceModeCombo->SelectItem(4);
            break;
        case E_LAN_MODE:
            pDeviceModeCombo->SelectItem(5);
            break;
        default:
            pDeviceModeCombo->SelectItem(1);
            break;
        }
    }

    E_DEVICE_MODE CCMultipleTestConfigDlg::GetDeviceModeCombo(CComboUI* pDeviceModeCombo)
    {
        int nCurSel = pDeviceModeCombo->GetCurSel();
        if (0 == nCurSel)
        {
            return E_ADB_MODE;
        }
        else if (1 == nCurSel)
        {
            return E_DIAG_MODE;
        }
        else if (2 == nCurSel)
        {
            return E_MODEM_MODE;
        }
        else if (3 == nCurSel)
        {
            return E_FASTBOOT_MODE;
        }
        else if (4 == nCurSel)
        {
            return E_DOWNLOADER_MODE;
        }
        return E_LAN_MODE;
    }

    void CCMultipleTestConfigDlg::OnInitWindow()
    {
        //! get ui ptr class
        {
            bool bSucceseful = true;

            mpIsSaveDownloadLogComCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"IsSaveDownloadLogComCheckBox"));
            mpIsFileCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"IsFileCheckBox"));
            mpDeviceQueueConfigBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"DeviceQueueConfigBtn"));

            mpTestTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TestTimeEdt"));
            mpTestTimeOutEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TestTimeOutEdt"));
            mpRowCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"RowCombo"));
            mpColumnCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"ColumnCombo"));

            mpDeviceQueueCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"DeviceQueueCombo"));
            mpHostIPAddrEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"HostIPAddrEdt"));

            mpFristDeviceModeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"FristDeviceModeCombo"));
            mpSecondDeviceModeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"SecondDeviceModeCombo"));

            mpFristDeviceModeLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"FristDeviceModeLabel"));
            mpSecondDeviceModeLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"SecondDeviceModeLabel"));


            mpUpgradeVersionEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"UpgradeVersionEdt"));
            mpLogPathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"LogPathEdt"));

            mpOkBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"OkBtn"));
            mpCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"CancelBtn"));
            mpClearDeviceQueueSortConfigBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"ClearDeviceQueueSortConfigBtn"));

            bSucceseful &= (NULL != mpIsSaveDownloadLogComCheckBox);
            bSucceseful &= (NULL != mpIsFileCheckBox);
            bSucceseful &= (NULL != mpDeviceQueueConfigBtn);
            bSucceseful &= (NULL != mpTestTimeEdt);
            bSucceseful &= (NULL != mpTestTimeOutEdt);
            bSucceseful &= (NULL != mpRowCombo);
            bSucceseful &= (NULL != mpColumnCombo);
            bSucceseful &= (NULL != mpDeviceQueueCombo);
            bSucceseful &= (NULL != mpHostIPAddrEdt);
            bSucceseful &= (NULL != mpFristDeviceModeCombo);
            bSucceseful &= (NULL != mpSecondDeviceModeCombo);
            bSucceseful &= (NULL != mpFristDeviceModeLabel);
            bSucceseful &= (NULL != mpSecondDeviceModeLabel);
            bSucceseful &= (NULL != mpUpgradeVersionEdt);
            bSucceseful &= (NULL != mpLogPathEdt);
            bSucceseful &= (NULL != mpOkBtn);
            bSucceseful &= (NULL != mpCancelBtn);
            bSucceseful &= (NULL != mpClearDeviceQueueSortConfigBtn);

            if (!bSucceseful)
            {
                Close(IDCANCEL);
                return ;
            }
        }

        // init config 
        {
            mpIsSaveDownloadLogComCheckBox->SetCheck(mpsMultipleConfig->mbSaveTestLog);
            mpIsFileCheckBox->SetCheck(!mpsMultipleConfig->mbBinaryImageIsFolder);

            mpTestTimeEdt->SetText(JGW_GetFormatWString(L"%u",mpsMultipleConfig->mnTestTimeSec).c_str());
            mpTestTimeOutEdt->SetText(JGW_GetFormatWString(L"%u",mpsMultipleConfig->mnTestTimeOutSec).c_str());
            mpRowCombo->SelectItem(mpsMultipleConfig->mnMultipleTestRow - 1);
            mpColumnCombo->SelectItem(mpsMultipleConfig->mnMultipleTestCol - 1);

            mpDeviceQueueCombo->SelectItem(mpsMultipleConfig->msDeviceProgramInfo.meDeviceQueueProgram);
            mpHostIPAddrEdt->SetText(mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.c_str());

            OnChangeDeviceQueueComboUI();
            //SetDeviceModeCombo(mpsMultipleConfig->meFristDeviceMode,mpFristDeviceModeCombo);
            //SetDeviceModeCombo(mpsMultipleConfig->meSecondDeviceMode,mpSecondDeviceModeCombo);
            //if (E_DEVICE_UNKNOWMODE == mpsMultipleConfig->meThreeDeviceMode) mpsMultipleConfig->meThreeDeviceMode = E_LAN_MODE;

            mpUpgradeVersionEdt->SetText(mpsMultipleConfig->mstrUpgradeVersion.c_str());
            mpLogPathEdt->SetText(mpsMultipleConfig->mstrSaveTestLogFolder.c_str());
        }
    }

    void CCMultipleTestConfigDlg::OnChangeDeviceQueueComboUI()
    {
        E_DEVICE_QUEUE_PROGRAM eDeviceQueueProgram = (E_DEVICE_QUEUE_PROGRAM)mpDeviceQueueCombo->GetCurSel();
        switch (eDeviceQueueProgram)
        {
        case E_SINGLE_PORT_QUEUE_PROGRAM:
            mpSecondDeviceModeLabel->SetVisible(false);
            mpSecondDeviceModeCombo->SetVisible(false);
            mpDeviceQueueConfigBtn->SetEnabled(false);
            SetDeviceModeCombo(mpsMultipleConfig->msDeviceProgramInfo.meFristDeviceMode,mpFristDeviceModeCombo);
            break;
        case E_DUAL_PORT_QUEUE_PROGRAM:
        case E_DUAL_PORT_NETCARD_QUEUE_PROGRAM:
            mpSecondDeviceModeLabel->SetVisible(true);
            mpSecondDeviceModeCombo->SetVisible(true);
            mpDeviceQueueConfigBtn->SetEnabled(true);
            SetDeviceModeCombo(mpsMultipleConfig->msDeviceProgramInfo.meFristDeviceMode,mpFristDeviceModeCombo);
            SetDeviceModeCombo(mpsMultipleConfig->msDeviceProgramInfo.meSecondDeviceMode,mpSecondDeviceModeCombo);
            break;
        case E_NETCARD_QUEUE_PROGRAM:
            mpSecondDeviceModeLabel->SetVisible(false);
            mpSecondDeviceModeCombo->SetVisible(false);
            mpDeviceQueueConfigBtn->SetEnabled(false);
            mpsMultipleConfig->msDeviceProgramInfo.meFristDeviceMode = E_LAN_MODE;
            SetDeviceModeCombo(mpsMultipleConfig->msDeviceProgramInfo.meFristDeviceMode,mpFristDeviceModeCombo);
            break;
        } 
    }

    void CCMultipleTestConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpOkBtn == msg.pSender)
            {
                mpsMultipleConfig->mbSaveTestLog = mpIsSaveDownloadLogComCheckBox->GetCheck();
                mpsMultipleConfig->mbBinaryImageIsFolder = !mpIsFileCheckBox->GetCheck();
                mpsMultipleConfig->mnTestTimeSec = _ttoi(mpTestTimeEdt->GetText().GetData());
                mpsMultipleConfig->mnTestTimeOutSec = _ttoi(mpTestTimeOutEdt->GetText().GetData());
                mpsMultipleConfig->mnMultipleTestRow = mpRowCombo->GetCurSel() + 1;
                mpsMultipleConfig->mnMultipleTestCol = mpColumnCombo->GetCurSel() + 1;

                mpsMultipleConfig->msDeviceProgramInfo.meDeviceQueueProgram = (E_DEVICE_QUEUE_PROGRAM)mpDeviceQueueCombo->GetCurSel();
                mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr = mpHostIPAddrEdt->GetText().GetData();
                mpsMultipleConfig->msNetCardConfig.mstrHostIPAddrAnsi = JGW_W2A(mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr);
                mpsMultipleConfig->msDeviceProgramInfo.mstrNetCardHostIPSeg = mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.substr(0,mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.find_last_of('.') + 1);

                //OnChangeDeviceQueueComboUI();
                mpsMultipleConfig->msDeviceProgramInfo.meFristDeviceMode = GetDeviceModeCombo(mpFristDeviceModeCombo);
                if (mpSecondDeviceModeCombo->IsVisible()) mpsMultipleConfig->msDeviceProgramInfo.meSecondDeviceMode = GetDeviceModeCombo(mpSecondDeviceModeCombo);
                //SetDeviceModeCombo(mpsMultipleConfig->meFristDeviceMode,mpFristDeviceModeCombo);
                //SetDeviceModeCombo(mpsMultipleConfig->meSecondDeviceMode,mpSecondDeviceModeCombo);
                //if (E_DEVICE_UNKNOWMODE == mpsMultipleConfig->meThreeDeviceMode) mpsMultipleConfig->meThreeDeviceMode = E_LAN_MODE;
                mpsMultipleConfig->mstrUpgradeVersion = mpUpgradeVersionEdt->GetText().GetData();
                mpsMultipleConfig->mstrSaveTestLogFolder = mpLogPathEdt->GetText().GetData();

                mpsMultipleConfig->mstrTFTPDIPAddr = mstrTFTPDIPAddr;
                if (0 != JGW_WStrComparenoCaseWStr(mpsMultipleConfig->mstrDownloadFileConfig.c_str(),mstrDownloadFileConfig.c_str()))
                {
                    // mpsMultipleConfig->mstrDownloadFileConfig = strDownloadFileConfig;
                    // mpsMultipleConfig->mmapTSEParamValues.clear();
                    // mpsMultipleConfig->mstrConfigTSEParam = L"";
                    //! 解析下载文件配置，格式
                    std::wstring strConfigTSEParam;
                    std::vector<std::wstring> vstrTemp,vstrTempKeyValue;
                    std::map<std::wstring,std::wstring> mapTSEParamValues;

                    JGW_ParserStrW(mstrDownloadFileConfig.c_str(),L",",vstrTemp);
                    for (size_t index = 0;index < vstrTemp.size();index++)
                    {
                        vstrTempKeyValue.clear();
                        JGW_ParserStrW(vstrTemp[index].c_str(),L"=",vstrTempKeyValue);
                        if (2 != vstrTempKeyValue.size())
                        {
                            MessageBox(mhParentHwnd,L"下载或升级文件配置错误",L"错误",MB_ICONERROR);
                            return ;
                        }
                        else
                        {
                            if (strConfigTSEParam.empty())
                            {
                                strConfigTSEParam += vstrTempKeyValue[0];
                            }
                            else
                            {
                                strConfigTSEParam += L",";
                                strConfigTSEParam += vstrTempKeyValue[0];
                            }
                            mapTSEParamValues.insert(std::make_pair<std::wstring,std::wstring>(vstrTempKeyValue[0],vstrTempKeyValue[1]));
                        }
                    }
                    mpsMultipleConfig->mstrDownloadFileConfig = mstrDownloadFileConfig;
                    mpsMultipleConfig->mmapTSEParamValues.clear();
                    mpsMultipleConfig->mmapTSEParamValues.insert(mapTSEParamValues.begin(),mapTSEParamValues.end());
                    mpsMultipleConfig->mstrConfigTSEParam = strConfigTSEParam;
                }
                Close(IDOK);
            }
            else if(mpCancelBtn == msg.pSender)
            {
                Close(IDCANCEL);
            }
            else if (mpClearDeviceQueueSortConfigBtn == msg.pSender)
            {
                ClearDeviceQueueSortConfig();
            }
            else if(mpDeviceQueueConfigBtn == msg.pSender)
            {
                S_DEVICEQUEUE_PROGRAM_INFO sDeviceQueueProgramInfo;
                //! 获取设备排序方案信息
                sDeviceQueueProgramInfo.meDeviceQueueProgram = (E_DEVICE_QUEUE_PROGRAM)mpDeviceQueueCombo->GetCurSel();
                // 
                sDeviceQueueProgramInfo.meFristDeviceMode = GetDeviceModeCombo(mpFristDeviceModeCombo);
                if (mpSecondDeviceModeCombo->IsVisible()) sDeviceQueueProgramInfo.meSecondDeviceMode = GetDeviceModeCombo(mpSecondDeviceModeCombo);
                sDeviceQueueProgramInfo.meThreeDeviceMode = E_DEVICE_UNKNOWMODE;
                // host ip seg
                std::wstring strHostIPAddr = mpHostIPAddrEdt->GetText().GetData();
                sDeviceQueueProgramInfo.mstrNetCardHostIPSeg = mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.substr(0,strHostIPAddr.find_last_of('.') + 1);
                OnResponseClickConfigMsg(m_PaintManager.GetPaintWindow(),sDeviceQueueProgramInfo,mstrTFTPDIPAddr,mstrDownloadFileConfig);
#if 0
                std::wstring strDownloadFileConfig = mpsMultipleConfig->mstrDownloadFileConfig;
                OnResponseClickConfigMsg(m_PaintManager.GetPaintWindow(),sDeviceQueueProgramInfo,mpsMultipleConfig->mstrTFTPDIPAddr,strDownloadFileConfig);
                if (0 != JGW_WStrComparenoCaseWStr(mpsMultipleConfig->mstrDownloadFileConfig.c_str(),strDownloadFileConfig.c_str()))
                {
                    // mpsMultipleConfig->mstrDownloadFileConfig = strDownloadFileConfig;
                    // mpsMultipleConfig->mmapTSEParamValues.clear();
                    // mpsMultipleConfig->mstrConfigTSEParam = L"";
                    //! 解析下载文件配置，格式
                    std::wstring strConfigTSEParam;
                    std::vector<std::wstring> vstrTemp,vstrTempKeyValue;
                    std::map<std::wstring,std::wstring> mapTSEParamValues;

                    JGW_ParserStrW(strDownloadFileConfig.c_str(),L",",vstrTemp);
                    for (size_t index = 0;index < vstrTemp.size();index++)
                    {
                        vstrTempKeyValue.clear();
                        JGW_ParserStrW(vstrTemp[index].c_str(),L"=",vstrTempKeyValue);
                        if (2 != vstrTempKeyValue.size())
                        {
                            MessageBox(mhParentHwnd,L"下载或升级文件配置错误",L"错误",MB_ICONERROR);
                            return ;
                        }
                        else
                        {
                            if (strConfigTSEParam.empty())
                            {
                                strConfigTSEParam += vstrTempKeyValue[0];
                            }
                            else
                            {
                                strConfigTSEParam += L",";
                                strConfigTSEParam += vstrTempKeyValue[0];
                            }
                            mapTSEParamValues.insert(std::make_pair<std::wstring,std::wstring>(vstrTempKeyValue[0],vstrTempKeyValue[1]));
                        }
                    }
                    mpsMultipleConfig->mstrDownloadFileConfig = strDownloadFileConfig;
                    mpsMultipleConfig->mmapTSEParamValues.clear();
                    mpsMultipleConfig->mmapTSEParamValues.insert(mapTSEParamValues.begin(),mapTSEParamValues.end());
                    mpsMultipleConfig->mstrConfigTSEParam = strConfigTSEParam;
                    Close(IDOK);
                }
#endif
            }
        }
        else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (msg.pSender == mpDeviceQueueCombo)
            {
                OnChangeDeviceQueueComboUI();
            }
        }
    }
}
