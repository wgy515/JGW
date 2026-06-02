#include "StdAfx.h"
#include "CJGW_WlanTestDevToolDlg.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGW_WlanTestDevToolDlg::CCJGW_WlanTestDevToolDlg(void)
    {
    }


    CCJGW_WlanTestDevToolDlg::~CCJGW_WlanTestDevToolDlg(void)
    {
    }

    void CCJGW_WlanTestDevToolDlg::OnInitWindow()
    {
        mpWiFiTestLogPathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"WiFiTestLogPathEdt"));
        mpSelectWiFiTestLogPathEdtBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"SelectWiFiTestLogPathEdtBtn"));

        mpWiFiTemplatePathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"WiFiTemplatePathEdt"));
        mpSelectWiFiTemplatePathEdtBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"SelectWiFiTemplatePathEdtBtn"));

        mpSaveTemplatePathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"SaveTemplatePathEdt"));
        mpSelectSaveTemplatePathEdtBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"SelectSaveTemplatePathEdtBtn"));
        mpTemperatureComBo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"TemperatureComBo"));
        mpExecuteBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"ExecuteBtn"));

        if (NULL == mpWiFiTestLogPathEdt || NULL == mpSelectWiFiTestLogPathEdtBtn 
            || NULL == mpSaveTemplatePathEdt || NULL == mpSelectSaveTemplatePathEdtBtn
            || NULL == mpTemperatureComBo || NULL == mpExecuteBtn
            || NULL == mpWiFiTemplatePathEdt || NULL == mpSelectWiFiTemplatePathEdtBtn)
        {
            MessageBox(m_hWnd,L"初始化UI异常",L"错误",MB_ICONERROR);
            Close(IDCANCEL);
            return;
        }

        m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_WlanTestDevToolDlg::OnEventCustomMessage,0);
        mpWlanTestDevThread = new CCJGW_WlanTestDevThread();
        mpWlanTestDevThread->CreateMessageThread(0,0,true);

        msWiFiTestTemplateParam.mstrExcelParseIniFilePath = JGW_GetConfigConfigFilePath(L"",L"excel_config.ini");
        msWiFiTestTemplateParam.mstrExcelTemplateFilePath = JGW_GetConfigConfigFilePath(L"",L"wifi_template.xls");
        mpWiFiTemplatePathEdt->SetText(msWiFiTestTemplateParam.mstrExcelTemplateFilePath.c_str());
    }

    bool CCJGW_WlanTestDevToolDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_TEST_PASS_MSG:
            mpExecuteBtn->SetEnabled(true);
            break;
        default:
            break;
        }
        return true;
    }

    void CCJGW_WlanTestDevToolDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpSelectWiFiTestLogPathEdtBtn)
            {
                OnSelectWiFiTestLogPathBtnClick();
            }
            else if (msg.pSender == mpSelectSaveTemplatePathEdtBtn)
            {
                OnSelectSaveTemplateLogPathBtnClick();
            }
            else if (msg.pSender == mpExecuteBtn)
            {
                OnExecuteBtnClick();
            }
            else if (msg.pSender == mpSelectWiFiTemplatePathEdtBtn)
            {
                OnSelectWiFiTemplatePathEdtBtnClick();
            }
        }
    }

    void CCJGW_WlanTestDevToolDlg::OnCloseWindow()
    {
        if (mpWlanTestDevThread)
        {
            mpWlanTestDevThread->TerminateMessageThread();
            delete mpWlanTestDevThread;
            mpWlanTestDevThread = NULL;
        }
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_WlanTestDevToolDlg::OnEventCustomMessage,0);
    }

    void CCJGW_WlanTestDevToolDlg::OnSelectWiFiTestLogPathBtnClick()
    {
        std::wstring strOpenFilePath;
        if (!JGW_ChooseFile(strOpenFilePath,L"csv File(*.csv)\0*.csv\0all file(*.*)\0*.*\0\0",m_hWnd))
        {
            return ;
        }
        msWiFiTestTemplateParam.mstrWiFiTestLogPath = strOpenFilePath;
        mpWiFiTestLogPathEdt->SetText(strOpenFilePath.c_str());
    }

    void CCJGW_WlanTestDevToolDlg::OnSelectWiFiTemplatePathEdtBtnClick()
    {
        std::wstring strSaveFilePath;
        if (!JGW_SaveFileName(strSaveFilePath,L"excel File(*.xls,*.xlsx)\0*.xlsx;*.xls\0all file(*.*)\0*.*\0\0",m_hWnd,L".xls"))
        {
            return ;
        }
        msWiFiTestTemplateParam.mstrExcelTemplateFilePath = strSaveFilePath;
        mpWiFiTemplatePathEdt->SetText(msWiFiTestTemplateParam.mstrExcelTemplateFilePath.c_str());
    }

    void CCJGW_WlanTestDevToolDlg::OnSelectSaveTemplateLogPathBtnClick()
    {
        std::wstring strSaveFilePath;
        if (!JGW_SaveFileName(strSaveFilePath,L"excel File(*.xls,*.xlsx)\0*.xlsx;*.xls\0all file(*.*)\0*.*\0\0",m_hWnd,L".xls"))
        {
            return ;
        }
        msWiFiTestTemplateParam.mstrSaveTemplatePath = strSaveFilePath;
        mpSaveTemplatePathEdt->SetText(strSaveFilePath.c_str());
    }

    void CCJGW_WlanTestDevToolDlg::OnExecuteBtnClick()
    {
        if (msWiFiTestTemplateParam.mstrSaveTemplatePath.empty() || msWiFiTestTemplateParam.mstrWiFiTestLogPath.empty())
        {
            MessageBox(m_hWnd,L"请选择对应参数文件",L"错误",MB_ICONERROR);
            return ;
        }
        msWiFiTestTemplateParam.mnTemperatureIndex = mpTemperatureComBo->GetCurSel();
        mpExecuteBtn->SetEnabled(false);
        msWiFiTestTemplateParam.mhMainWindow = m_hWnd;
        mpWlanTestDevThread->PostThreadMessage(WM_THREAD_TEST_START_MSG,WPARAM(&msWiFiTestTemplateParam),LPARAM(&mAsyncWndMessage));
    }
}
