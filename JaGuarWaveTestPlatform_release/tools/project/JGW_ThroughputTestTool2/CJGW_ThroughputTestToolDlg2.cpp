#include "StdAfx.h"
#include "CJGW_ThroughputTestToolDlg2.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    //! F:\project\git_code_debug\FTM-tools\JaGuarWaveTestPlatform\bin\debug\exec_debug\html\echarts-2.2.1\doc\example\line2.html
    CCJGW_ThroughputTestToolDlg2::CCJGW_ThroughputTestToolDlg2(void) : mUIController(this)
    {

    }


    CCJGW_ThroughputTestToolDlg2::~CCJGW_ThroughputTestToolDlg2(void)
    {
    }

    void CCJGW_ThroughputTestToolDlg2::OnInitWindow()
    {
        if (!GetUIControl())
        {
            Close(IDCANCEL);
            ::MessageBox(GetHWND(),_T("控件加载失败"),_T("视图初始化错误"),MB_ICONERROR);
            return ;
        }

        {
            mpStartBtn->SetEnabled(false);
            mpWkeBrowser->InitBrowser(WM_TIMER_WKE_BROWSER);
            m_PaintManager.GetEventSource() += MakeDelegate(&mUIController,&CCJGW_ThroughputTestTool2Controller::OnEventCustomMessage,0);
        } 

        if (!mUIController.OnInit())
        {
            Close(IDCANCEL);
            ::MessageBox(GetHWND(),_T("配置文件初始化失败"),_T("初始化错误"),MB_ICONERROR);
            return ;
        }
        {
            std::wstring strHtmlFile = JGW_RealativePathToAbsPath(mUIController.GetTestParams().mstrHtmlFile.c_str());
            std::wstring strEncodeHtmlFilePath,strDecodeHtmlFilePath;
            JGW_URLEncodeW(strHtmlFile,strEncodeHtmlFilePath);
            JGW_URLDecodeW(strEncodeHtmlFilePath,strDecodeHtmlFilePath);
            mpWkeBrowser->LoadFile(strEncodeHtmlFilePath.c_str()); 
            mpWkeBrowser->SettingMaxListCount(mUIController.GetTestParams().mnSettingMaxListCount);
        }
    }

    void CCJGW_ThroughputTestToolDlg2::OnCloseWindow()
    {
        mUIController.OnCloseWindow();
        m_PaintManager.GetEventSource() -= MakeDelegate(&mUIController,&CCJGW_ThroughputTestTool2Controller::OnEventCustomMessage,0);
    }

    void CCJGW_ThroughputTestToolDlg2::OnNotify(TNotifyUI& msg)
    {
        mUIController.OnViewNotify(msg);
    }

    CControlUI* CCJGW_ThroughputTestToolDlg2::CreateControl(LPCTSTR pstrClass)
    {
        if(_tcscmp(pstrClass, DUI_CTR_WKEWINDOWLESSBROWSER) == 0)
            return	new CWkeWindowlessUI();
        return NULL;
    }

    bool CCJGW_ThroughputTestToolDlg2::GetUIControl()
    {
        bool bSuccess = true;

        mpWkeBrowser = static_cast<CWkeWindowlessUI*>(m_PaintManager.FindControl(L"wkeBrowser"));
        mpIperfCommandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"IperfCommandEdt"));
        mpTerminalType = static_cast<CComboUI*>(m_PaintManager.FindControl(L"TerminalType"));
        mpProtocolType = static_cast<CComboUI*>(m_PaintManager.FindControl(L"ProtocolType"));
        mpIperfIPEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"IperfIPEdt"));
        mpSshIpAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"SshIpAddressEdt"));
        mpCellphoneIPEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"CellphoneIPEdt"));
        mpTestTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TestTimeEdt"));
        mpStartBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"StartBtn"));
        mpStopBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"StopBtn"));
        mpLogEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"LogEdt"));
        mpExtraParameterEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"ExtraParameterEdt"));
        mpUpdateConfigOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"UpdateConfigOption"));
        mpThreadCountEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ThreadCountEdt"));
        mpClientConfigHorizontalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"ClientConfigHorizontalLayout"));
        mpNetCardInfoComBo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"NetCardInfoComBo"));
        mpCheckUseCellphoneIP = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"CheckUseCellphoneIP"));
        mpLogPrefixEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"LogPrefixEdt"));
        mpLogFolderBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"LogFolderBtn"));
        mpSaveIPQLogCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"SaveIPQLogCheckBox"));

        bSuccess &= (NULL != mpWkeBrowser);
        bSuccess &= (NULL != mpIperfCommandEdt);
        bSuccess &= (NULL != mpTerminalType);
        bSuccess &= (NULL != mpProtocolType);
        bSuccess &= (NULL != mpIperfIPEdt);
        bSuccess &= (NULL != mpSshIpAddressEdt);
        bSuccess &= (NULL != mpCellphoneIPEdt);
        bSuccess &= (NULL != mpTestTimeEdt);
        bSuccess &= (NULL != mpStartBtn);
        bSuccess &= (NULL != mpStopBtn);
        bSuccess &= (NULL != mpLogEdt);
        bSuccess &= (NULL != mpExtraParameterEdt);
        bSuccess &= (NULL != mpUpdateConfigOption);
        bSuccess &= (NULL != mpThreadCountEdt);
        bSuccess &= (NULL != mpClientConfigHorizontalLayout);
        bSuccess &= (NULL != mpNetCardInfoComBo);
        bSuccess &= (NULL != mpCheckUseCellphoneIP);
        bSuccess &= (NULL != mpLogPrefixEdt);
        bSuccess &= (NULL != mpLogFolderBtn);
        bSuccess &= (NULL != mpSaveIPQLogCheckBox);

        return bSuccess;
    }
}