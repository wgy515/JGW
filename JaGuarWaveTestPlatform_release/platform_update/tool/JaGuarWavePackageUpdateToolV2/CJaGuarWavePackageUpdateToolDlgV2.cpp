#include "StdAfx.h"
#include "CJaGuarWavePackageUpdateToolDlgV2.h"
#include "CJGWHelpConfigDlg.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "CJGW_UpdateThread.h"
#include "CJGW_HelpTaskDlg.h"
#include "CJGW_HelpProjectDlg.h"
#include "CJGW_HelpOTADlg.h"

#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
namespace JGW
{
    CCJaGuarWavePackageUpdateToolDlgV2::CCJaGuarWavePackageUpdateToolDlgV2(void) : mpModuleContainer(NULL),mpHelpPackageUIInterface(NULL),mpMessageThread(NULL)
    {
        msPackageUpdateConfig.mstrFTPHost = "192.168.8.10";
        msPackageUpdateConfig.mnFTPPort = 26;
        msPackageUpdateConfig.mstrFTPRootPath = L"/JGWPTP_Help/";
        msPackageUpdateConfig.mstrFTPLogRootPath = L"/JaGuarWaveTestPlatform_Log/";
        CCJGW_CryptPassword cryptPassword;
        msPackageUpdateConfig.mstrUserName = cryptPassword.DecryptPassword("lKNG7Im5x0ith8DsldhiTw==");
        msPackageUpdateConfig.mstrUserPassword = cryptPassword.DecryptPassword("WWC7r11A4ezrNs6aEyHcIg==");
    }


    CCJaGuarWavePackageUpdateToolDlgV2::~CCJaGuarWavePackageUpdateToolDlgV2(void)
    {
    }

    bool CCJaGuarWavePackageUpdateToolDlgV2::OnHandleAsyncFuncMsg(int nActionIndex)
    {
        if (NULL != mpHelpPackageUIInterface)
        {
            return mpHelpPackageUIInterface->OnHandleAsyncFuncMsg(nActionIndex);
        }
        return false;
    }

    void CCJaGuarWavePackageUpdateToolDlgV2::OnInitWindow()
    {
        mpConfigOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"ConfigOption"));
        mpOTAOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"OTAOption"));
        mpModuleContentHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"ModuleContentHorizontalLayout"));
        mpMainHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"MainHorizontalLayout"));
        mpLogRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"LogRichEdit"));
        mpTaskOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"TaskOption"));
        mpCommitHelpDBBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"CommitHelpDBBtn"));
        mpProjectOption = static_cast<COptionUI*>(m_PaintManager.FindControl(L"ProjectOption"));

        if (NULL == mpConfigOption 
            || NULL == mpOTAOption
            || NULL == mpModuleContentHorizontalLayout
            || NULL == mpMainHorizontalLayout
            || NULL == mpLogRichEdit
            || NULL == mpTaskOption
            || NULL == mpCommitHelpDBBtn
            || NULL == mpProjectOption)
        {
            MessageBox(GetHWND(),L"Init Skin Fail",L"Error",MB_ICONERROR);
            Close();
            return;
        }
        LoadHelpConfig();
        mpMainHorizontalLayout->SetEnabled(false);
        //! 
        m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJaGuarWavePackageUpdateToolDlgV2::OnEventCustomMessage,0);
        OnAppendRichEditTest(WPARAM(L"Start download help db ..."),NULL);

        mpMessageThread = new CCJGW_UpdateThread(this);
        mpMessageThread->CreateMessageThread(0,0,true);
        mpMessageThread->PostThreadMessageW(WM_TEST_THREAD_MSG_INIT,WPARAM(&mAsyncWndMessage),LPARAM(&msPackageUpdateConfig));
        msPackageUpdateConfig.mnThreadID = mpMessageThread->m_dThreadID;
    }

    void CCJaGuarWavePackageUpdateToolDlgV2::LoadHelpConfig()
    {
        std::wstring strFilePath(JGW_GetApplicationFolder());
        strFilePath += L"help.ini";

        if (JGW_FileExistsToFilePath(strFilePath.c_str()))
        {
            CCJGW_ConfigIni helpConfigIni;
            CCJGW_CryptPassword cryptPassword;

            helpConfigIni.InitIniFilePath(strFilePath.c_str());
            msPackageUpdateConfig.mnFTPPort = helpConfigIni.GetIniKeyIntValue(L"FTP",L"Port",26);
            msPackageUpdateConfig.mstrFTPHost = helpConfigIni.GetIniKeyValueA(L"FTP",L"Host",L"192.168.8.10");
            msPackageUpdateConfig.mstrFTPRootPath = helpConfigIni.GetIniKeyValue(L"FTP",L"RootPath",L"/JaGuarWaveTestPlatform_Version/");

            msPackageUpdateConfig.mstrFTPLogRootPath = helpConfigIni.GetIniKeyValue(L"FTP",L"LogRootPath",L"/JaGuarWaveTestPlatform_Version/");

            std::string strUserName = helpConfigIni.GetIniKeyValueA(L"FTP",L"UserName",L"lKNG7Im5x0ith8DsldhiTw==");
            std::string strUserPassword = helpConfigIni.GetIniKeyValueA(L"FTP",L"Password",L"WWC7r11A4ezrNs6aEyHcIg==");
            msPackageUpdateConfig.mstrUserName = cryptPassword.DecryptPassword(strUserName);
            msPackageUpdateConfig.mstrUserPassword = cryptPassword.DecryptPassword(strUserPassword);
        }
    }


    void CCJaGuarWavePackageUpdateToolDlgV2::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            if (msg.pSender == mpConfigOption)
            {
                ClearModuleContainer();
                BuildModuleContainer(L"HelpConfig.xml");
                mpHelpPackageUIInterface = new CCJGWHelpConfigDlg(&msPackageUpdateConfig);
            }
            else if (msg.pSender == mpOTAOption)
            {
                ClearModuleContainer();
                BuildModuleContainer(L"OTAConfig.xml");
                mpHelpPackageUIInterface = new CCJGW_HelpOTADlg(&msPackageUpdateConfig);
            }
            else if (msg.pSender == mpTaskOption)
            {
                ClearModuleContainer();
                BuildModuleContainer(L"TaskConfig.xml");
                mpHelpPackageUIInterface = new CCJGW_HelpTaskDlg(&msPackageUpdateConfig);
            }
            else if (msg.pSender == mpProjectOption)
            {
                ClearModuleContainer();
                BuildModuleContainer(L"ProjectConfig.xml");
                mpHelpPackageUIInterface = new CCJGW_HelpProjectDlg(&msPackageUpdateConfig);
                
            }
            if (msg.pSender == mpProjectOption 
                || msg.pSender == mpTaskOption
                || msg.pSender == mpOTAOption
                || msg.pSender == mpConfigOption)
            {
                if (!mpHelpPackageUIInterface->InitHelpPackageUI(&m_PaintManager))
                {
                    MessageBox(GetHWND(),L"初始化Config UI失败",L"错误",MB_ICONERROR);
                    ClearModuleContainer();
                }
            }
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpCommitHelpDBBtn)
            {
                OnCommitHelpDBClickMsg();
            }
        }
        if (NULL != mpHelpPackageUIInterface)
        {
            mpHelpPackageUIInterface->OnNotifyHelpPackageUI(msg);
        }
    }

    void CCJaGuarWavePackageUpdateToolDlgV2::OnCloseWindow()
    {
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJaGuarWavePackageUpdateToolDlgV2::OnEventCustomMessage,0);
        ClearModuleContainer();
        if (NULL != mpMessageThread)
        {
            mpMessageThread->TerminateMessageThread();
            delete mpMessageThread;
            mpMessageThread = NULL;
        }
    }


    bool CCJaGuarWavePackageUpdateToolDlgV2::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        UINT uMsg = pTEventUI->Type;
        switch (uMsg)
        {
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
            OnAppendRichEditTest(pTEventUI->wParam,pTEventUI->lParam);
            return false;
        case WM_TEST_PASS_MSG:
            mpMainHorizontalLayout->SetEnabled(true);
            return false;
        case WM_HWMD_OPERATION_COMPLETE_MSG:
            if (NULL != mpHelpPackageUIInterface)
            {
                mpHelpPackageUIInterface->OnHandleEventCustomMessage(pTEventUI);
            }
            if (!mpMainHorizontalLayout->IsEnabled())
            {
                if (0 == JGW_WStrComparenoCaseWStr(L"FTP_UPLOAD",(const wchar_t*)pTEventUI->wParam))
                {
                    mpMainHorizontalLayout->SetEnabled(true);
                    if (1 == pTEventUI->lParam)
                    {
                        MessageBox(GetHWND(),L"FTP上传成功",L"正确",MB_OK);
                    }
                }
            }
            return false;
        }
        return true;
    }

    void CCJaGuarWavePackageUpdateToolDlgV2::OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (NULL != mpLogRichEdit)
        {
            std::wstring strTemp((const wchar_t*)wParam);
            JGW_EraseLastAndFristTwoCharsW(strTemp);
            mpLogRichEdit->AppendText(strTemp.c_str());
            mpLogRichEdit->AppendText(L"\r\n");
            mpLogRichEdit->EndDown();
        }
    }

    void CCJaGuarWavePackageUpdateToolDlgV2::BuildModuleContainer(const std::wstring& strSkinXmlPath)
    {
        CDialogBuilder builder;
        mpModuleContainer = builder.Create(strSkinXmlPath.c_str(),NULL,NULL,&m_PaintManager,NULL);
        if (!mpModuleContainer)
        {
            MessageBox(GetHWND(),L"加载皮肤控件失败",L"错误",MB_ICONERROR);
            return;
        }
        mpModuleContentHorizontalLayout->Add(mpModuleContainer);
    }

    void CCJaGuarWavePackageUpdateToolDlgV2::ClearModuleContainer()
    {
        if (NULL != mpHelpPackageUIInterface)
        {
            mpHelpPackageUIInterface->CloseHelpPackageUI();
            delete mpHelpPackageUIInterface;
            mpHelpPackageUIInterface = NULL;
        }

        mpModuleContentHorizontalLayout->RemoveAll();
        if (NULL != mpModuleContainer)
        {
            mpModuleContainer = NULL;
        }
    }

    void CCJaGuarWavePackageUpdateToolDlgV2::OnCommitHelpDBClickMsg()
    {
        static std::wstring strFtpUploadPath,strLocalFilePath;
        JGW_FormatWString(strFtpUploadPath,L"%s/help.db",msPackageUpdateConfig.mstrFTPRootPath.c_str());
        JGW_FormatWString(strLocalFilePath,L"%s\\temp.db",JGW_GetApplicationFolder());
        mpMainHorizontalLayout->SetEnabled(false);
        mpMessageThread->PostThreadMessage(WM_THREAD_FTP_UPLOAD_MSG,WPARAM(strFtpUploadPath.c_str()),LPARAM(strLocalFilePath.c_str()));
    }
}
