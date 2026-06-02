#include "StdAfx.h"
#include "CJaGuarwaveUpdateHelpDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
/*#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>*/
#include "CJGW_ConfigIni.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <fstream>
#include <poco/Delegate.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCJaGuarwaveUpdateHelpDlg::CCJaGuarwaveUpdateHelpDlg(S_UPDATE_HELP_CONFIG& sUpdateHelpConfig) : mpSelectFolderPath(NULL),mpFolderPathEdt(NULL),mpUpdateLogRichEdt(NULL),msUpdateHelpConfig(sUpdateHelpConfig),_errCnt(0)
    {
    }

    CCJaGuarwaveUpdateHelpDlg::~CCJaGuarwaveUpdateHelpDlg(void)
    {
    }

    void CCJaGuarwaveUpdateHelpDlg::OnInitWindow()
    {
        bool bSuccess = true;
        mpFolderPathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"folderPathEdt"));
        mpSelectFolderPath = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"selectFolderPathBtn"));
        mpUpdateLogRichEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"UpdateLogRichEdt"));

        bSuccess &= (NULL != mpFolderPathEdt);
        bSuccess &= (NULL != mpSelectFolderPath);
        bSuccess &= (NULL != mpUpdateLogRichEdt);

        if (bSuccess && msUpdateHelpConfig.mbAutoUpdate)
        {
             mpSelectFolderPath->SetEnabled(false);
             mpSelectFolderPath->SetText(JGW_A2W(msUpdateHelpConfig.mstrUpdateExecFolder).c_str());
             //! 关闭进程
             {
                 std::wstring strTemp(JGW_GetApplicationFolder()),strFilePath;
                 JGW_EraseLastAndFristChars(strTemp,'\\');
                 JGW_EraseAfterToRemoveCharW(strTemp,L'\\');
                 strFilePath = strTemp + L"kill_process.bat";
                 ShellExecute(NULL,L"open",strFilePath.c_str(),NULL,strTemp.c_str(),SW_NORMAL);
             }
             SetTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP,3000,NULL);
             m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJaGuarwaveUpdateHelpDlg::OnEventCustomMessage,0);
        }
    }

    void CCJaGuarwaveUpdateHelpDlg::OnCloseWindow()
    {
        KillTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP);
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJaGuarwaveUpdateHelpDlg::OnEventCustomMessage,0);
    }

    void CCJaGuarwaveUpdateHelpDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpSelectFolderPath)
            {
                OnRspSelectFolderBtnMsg();
            }
        }
    }

    bool CCJaGuarwaveUpdateHelpDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
        switch (pTEventUI->Type)
        {
        case WM_TIMER:
            OnTimerMsg(pTEventUI->wParam);
            break;
        default:
            break;
        }
        return true;
    }

    void CCJaGuarwaveUpdateHelpDlg::OnTimerMsg(int id)
    {
        if (WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP == id)
        {
            OnAppendRichEditTest(WPARAM(L"正在等待cmd进程终止..."),NULL);
            if (-1 != JGW_GetProcessID2Name(_T("cmd.exe"))) return;
            KillTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP);
            OnAutoUpdateTimer();
            if (_errCnt == 0)
            {
                ::MessageBox(m_hWnd,L"工具包更新完成，请重新打开测试程序",L"更新完成",MB_OK);
                UpdateApplicationVersion();
                JGW_DeleteFile(JGW_A2W(msUpdateHelpConfig.mstrUpdatePackFilePath));
            }
            else
            {
                ::MessageBox(m_hWnd,L"工具包更新完成，在更新过程中出现了错误,要解决此问题，请将测试工具进行关闭，重新进行升级",L"更新完成",MB_ICONERROR);
            }
            
            Close(IDOK);
        }
    }

    void CCJaGuarwaveUpdateHelpDlg::OnAppendRichEditTest(WPARAM wParam,LPARAM lParam)
    {
        static int line = 0;
        mpUpdateLogRichEdt->AppendText((const wchar_t*)wParam);
        mpUpdateLogRichEdt->AppendText(L"\r\n");
        if (line > 3000) {mpUpdateLogRichEdt->Clear();line = 0;}
        line ++;
        mpUpdateLogRichEdt->EndDown();
    }

    void CCJaGuarwaveUpdateHelpDlg::OnRspSelectFolderBtnMsg()
    {
        std::wstring strUpdatePackFile;
        if(!JGW_ChooseFile(strUpdatePackFile,L"Update Pack(update*.zip)\0update*.zip\0Pack File(*.zip)\0*.zip\0all file(*.*)\0*.*\0\0",m_hWnd))  return ;
        mpFolderPathEdt->SetText(strUpdatePackFile.c_str());
        msUpdateHelpConfig.mstrUpdateExecFolder = "C:\\JW";
        msUpdateHelpConfig.mstrUpdatePackFilePath = JGW_W2A(strUpdatePackFile);

        std::ifstream inp(JGW_W2A(strUpdatePackFile), std::ios::binary);
        _errCnt = 0;
        Poco::Zip::Decompress dec(inp,Poco::Path(msUpdateHelpConfig.mstrUpdateExecFolder));
        dec.EError += Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressError);
        dec.EOk += Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressOkCallback);
        dec.decompressAllFiles();
        dec.EError -= Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressError);
        dec.EOk -= Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressOkCallback);
    }

    void CCJaGuarwaveUpdateHelpDlg::OnDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
    {
        ++_errCnt;
        OnAppendRichEditTest(WPARAM(JGW_A2W(info.second).c_str()),NULL);
        MessageBox(m_hWnd,JGW_A2W(info.second).c_str(),L"当前文件升级失败",MB_ICONERROR);
    }

    void CCJaGuarwaveUpdateHelpDlg::OnDecompressOkCallback(const void* pSender,std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info)
    {
        std::string path = info.second.toString();
        OnAppendRichEditTest(WPARAM(JGW_A2W(msUpdateHelpConfig.mstrUpdateExecFolder + path).c_str()),NULL);
    }

    void CCJaGuarwaveUpdateHelpDlg::OnAutoUpdateTimer()
    {
        std::ifstream inp(msUpdateHelpConfig.mstrUpdatePackFilePath, std::ios::binary);
        _errCnt = 0;
        Poco::Zip::Decompress dec(inp,Poco::Path(msUpdateHelpConfig.mstrUpdateExecFolder));
        dec.EError += Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressError);
        dec.EOk += Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressOkCallback);
        dec.decompressAllFiles();
        dec.EError -= Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressError);
        dec.EOk -= Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressOkCallback);
    }

    void CCJaGuarwaveUpdateHelpDlg::UpdateApplicationVersion()
    {
        CCJGW_ConfigIni versionConfigIni;
        std::wstring strVersionConfigFilePath(JGW_A2W(msUpdateHelpConfig.mstrUpdateExecFolder));
        strVersionConfigFilePath += L"\\exec\\version.ini";

        versionConfigIni.InitIniFilePath(strVersionConfigFilePath.c_str());
        if (msUpdateHelpConfig.mbIsConfigVersion)
        {
            versionConfigIni.SetIniKeyValue(L"config",L"version",msUpdateHelpConfig.mstrUpdateVersion);
        }
        else
        {
            versionConfigIni.SetIniKeyValue(L"update",L"version",msUpdateHelpConfig.mstrUpdateVersion);
        }   
    }
}