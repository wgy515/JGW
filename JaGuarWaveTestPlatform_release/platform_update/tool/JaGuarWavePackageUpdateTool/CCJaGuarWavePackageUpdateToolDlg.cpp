#include "StdAfx.h"
#include "CCJaGuarWavePackageUpdateToolDlg.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJaGuarWavePackageUpdateToolDlg::CCJaGuarWavePackageUpdateToolDlg(void) : mpPackageUpdateThread(NULL)
    {
        msPackageUpdateInfo.mbIsParseProgramFiles = false;
    }


    CCJaGuarWavePackageUpdateToolDlg::~CCJaGuarWavePackageUpdateToolDlg(void)
    {
        if (mpPackageUpdateThread)
        {
            mpPackageUpdateThread->TerminateMessageThread();
            delete mpPackageUpdateThread;
            mpPackageUpdateThread = NULL;
        }
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnInitWindow()
    {
        {
            bool success = true;
            mpProgramFilesEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ProgramFilesEdt"));
            mpSelectProgramFilesEdtBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"SelectProgramFilesEdtBtn"));
            mpPackageFilesTotal = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"PackageFilesTotal"));
            mpTaskNumberEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TaskNumberEdt"));
            mpUpdateVersionEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"UpdateVersionEdt"));
            mpPackageFileList = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(L"PackageFileList"));
            mpAddPackageFileBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"AddPackageFileBtn"));
            mpAddPackageFolderBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"AddPackageFolderBtn"));
            mpPackageBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"PackageBtn"));
            mpHomeVerticalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"HomeVerticalLayout"));

            success &= (NULL != mpProgramFilesEdt);
            success &= (NULL != mpPackageFilesTotal);
            success &= (NULL != mpSelectProgramFilesEdtBtn);
            success &= (NULL != mpTaskNumberEdt);
            success &= (NULL != mpUpdateVersionEdt);
            success &= (NULL != mpPackageFileList);
            success &= (NULL != mpAddPackageFileBtn);
            success &= (NULL != mpAddPackageFolderBtn);
            success &= (NULL != mpPackageBtn);
            success &= (NULL != mpHomeVerticalLayout);
            if (!success)
            {
                Close();
                return;
            }
        }
        {
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJaGuarWavePackageUpdateToolDlg::OnEventCustomMessage,0);
            msPackageUpdateInfo.mpAsyncWndMessage = &mAsyncWndMessage;
            msPackageUpdateInfo.mhMainWnd = m_PaintManager.GetPaintWindow();
            UpdateUiShow();
            mpPackageUpdateThread = new CPackageUpdateThread(msPackageUpdateInfo);
            mpPackageUpdateThread->CreateMessageThread(0,0,true);
        }
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpSelectProgramFilesEdtBtn)
            {
                OnClickSelectProgramFilesBtnMsg();
            }
            else if (msg.pSender == mpPackageBtn)
            {
                OnClickPackageBtnMsg();
            }
            else if (msg.pSender == mpAddPackageFileBtn)
            {
                OnClickAddPackageFileBtnMsg();
            }
            else if (msg.pSender == mpAddPackageFolderBtn)
            {
                OnClickAddPackageFolderBtnMsg();
            }
        }
        else if (msg.sType == DUI_MSGTYPE_MENU)
        {
            if (msg.pSender == mpPackageFileList)
            {
                if (mpPackageFileList->GetSelectItemCount() > 0)
                {
                    OnListMenuRightClick(msg);
                }
            }
        }
    }

    bool CCJaGuarWavePackageUpdateToolDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
        switch (pTEventUI->Type)
        {
        case WM_HWND_OPERATION_COMPLETE_MSG:
            mpHomeVerticalLayout->SetEnabled(true);
            UpdateUiShow();
            break;
        case WM_MENUCLICK: //! 响应菜单栏点击消息
            OnMenuElementClick(pTEventUI);   
            break;
        }
        return true;
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnCloseWindow()
    {
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJaGuarWavePackageUpdateToolDlg::OnEventCustomMessage,0);
    }

    void CCJaGuarWavePackageUpdateToolDlg::UpdateUiShow()
    {
        mpPackageBtn->SetEnabled(msPackageUpdateInfo.mbIsParseProgramFiles);
        mpAddPackageFileBtn->SetEnabled(msPackageUpdateInfo.mbIsParseProgramFiles);
        mpAddPackageFolderBtn->SetEnabled(msPackageUpdateInfo.mbIsParseProgramFiles);

        if (msPackageUpdateInfo.mbIsParseProgramFiles)
        {
            mpProgramFilesEdt->SetText(msPackageUpdateInfo.mstrProgramFiles.c_str());
            mpTaskNumberEdt->SetText(msPackageUpdateInfo.mstrTaskNumber.c_str());
            mpUpdateVersionEdt->SetText(msPackageUpdateInfo.mstrUpdateVersion.c_str());
            mpPackageFileList->RemoveAllShow();

            for (size_t i = 0;i < msPackageUpdateInfo.mvUpdateFilePath.size();i ++)
            {
                CListTextElementUI* pListTextElementUi = mpPackageFileList->InsertItem( mpPackageFileList->GetList()->GetCount(),msPackageUpdateInfo.mvUpdateFilePath[i].c_str(),30);
                 pListTextElementUi->SetText(0,msPackageUpdateInfo.mvUpdateFilePath[i].c_str());
            }
            mpPackageFilesTotal->SetText(JGW_GetFormatWString(L"Package Files Total:%d",mpPackageFileList->GetList()->GetCount() + 1).c_str());
            mpPackageFileList->SetNeedEndDown(true);
        }
        else
        {
            mpPackageFilesTotal->SetText(L"");
            mpProgramFilesEdt->SetText(L"");
            mpTaskNumberEdt->SetText(L"");
            mpUpdateVersionEdt->SetText(L"");
            mpPackageFileList->RemoveAllShow();
            msPackageUpdateInfo.mbIsParseProgramFiles = false;
            msPackageUpdateInfo.mvUpdateFilePath.clear();
        }  
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnClickSelectProgramFilesBtnMsg()
    {
        msPackageUpdateInfo.mbIsParseProgramFiles = false;
        msPackageUpdateInfo.mstrProgramFiles = L"";
        msPackageUpdateInfo.mvUpdateFilePath.clear();
        std::wstring strFolder;
        UpdateUiShow();
        if (JGW_ChooseFolder(strFolder,m_PaintManager.GetPaintWindow()))
        {
            msPackageUpdateInfo.mstrProgramFiles = strFolder;
            //! strFolder = msPackageUpdateInfo.mstrProgramFiles + L"/";
            mpHomeVerticalLayout->SetEnabled(false);
            mpPackageUpdateThread->PostThreadMessage(WM_THREAD_PARSE_PROGRAMFILES_MSG);
            //! UpdateUiShow();
        }
    }

    bool CCJaGuarWavePackageUpdateToolDlg::CheckFileIsAddUpdateList(int& index,const std::wstring& strFilePath)
    {
        for (int i = 0;i < mpPackageFileList->GetList()->GetCount();i ++)
        {
            CListTextElementUI* pListTextElementUi = (CListTextElementUI*)mpPackageFileList->GetList()->GetItemAt(i);
            if (0 == strFilePath.compare(pListTextElementUi->GetText(0)))
            {
                index = i;
                return true;
            }
        }
        return false;
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnClickAddPackageFileBtnMsg()
    {
#if 0
        std::wstring strChosseUpdateFile;
        if(!JGW_ChooseFile(strChosseUpdateFile,L"all file(*.*)\0*.*\0\0",msPackageUpdateInfo.mhMainWnd,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_LONGNAMES|OFN_PATHMUSTEXIST,msPackageUpdateInfo.mstrProgramFiles.c_str()))  return ;
        if(_wcsnicmp(strChosseUpdateFile.c_str(), msPackageUpdateInfo.mstrProgramFiles.c_str(), msPackageUpdateInfo.mstrProgramFiles.length()) == 0)
        {
            JGW_ReplaceStringW(strChosseUpdateFile,msPackageUpdateInfo.mstrProgramFiles + L"\\",L"");
            msPackageUpdateInfo.mvUpdateFilePath.push_back(strChosseUpdateFile);
            CListTextElementUI* pListTextElementUi = mpPackageFileList->InsertItem( mpPackageFileList->GetList()->GetCount(),strChosseUpdateFile.c_str(),30);
            pListTextElementUi->SetText(0,strChosseUpdateFile.c_str());
        }
#else
        std::vector<std::wstring> vstrMultiFiles;
        int index = 0;
        if (!JGW_ChooseMultiSeleJGWile(vstrMultiFiles,L"all file(*.*)\0*.*\0\0",msPackageUpdateInfo.mhMainWnd,OFN_EXPLORER | OFN_ALLOWMULTISELECT,msPackageUpdateInfo.mstrProgramFiles.c_str())) return ;
        if (!vstrMultiFiles.empty() && 0 == _wcsnicmp(vstrMultiFiles[0].c_str(), msPackageUpdateInfo.mstrProgramFiles.c_str(), msPackageUpdateInfo.mstrProgramFiles.length()))
        {
            std::wstring strChosseUpdateFile;
            for (size_t i = 0;i < vstrMultiFiles.size();i ++)
            {
                strChosseUpdateFile = vstrMultiFiles[i];
                JGW_ReplaceStringW(strChosseUpdateFile,msPackageUpdateInfo.mstrProgramFiles + L"\\",L"");
                JGW_ReplaceStringW(strChosseUpdateFile,L"\\",L"/");
                if (!CheckFileIsAddUpdateList(index,strChosseUpdateFile))
                {
                    msPackageUpdateInfo.mvUpdateFilePath.push_back(strChosseUpdateFile);
                    CListTextElementUI* pListTextElementUi = mpPackageFileList->InsertItem( mpPackageFileList->GetList()->GetCount(),strChosseUpdateFile.c_str(),30);
                    pListTextElementUi->SetText(0,strChosseUpdateFile.c_str());
                }
            }
            mpPackageFilesTotal->SetText(JGW_GetFormatWString(L"Package Files Total:%d",mpPackageFileList->GetList()->GetCount() + 1).c_str());
        }
#endif
    }

    void ScanPackageUpdateFiles(const wchar_t* filepath,const wchar_t* filename,void* lpData)
    {
        std::vector<std::wstring>* vpScanPackageUpdateFiles = (std::vector<std::wstring>*)lpData;
        std::wstring strTmep (filepath);
        strTmep += L"/";
        strTmep += filename;
        vpScanPackageUpdateFiles->push_back(strTmep);
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnClickAddPackageFolderBtnMsg()
    {
        std::wstring strFolder;
        if (JGW_ChooseFolderSetFolderPath(strFolder,msPackageUpdateInfo.mstrProgramFiles.c_str(),m_PaintManager.GetPaintWindow()))
        {
            if (0 != _wcsnicmp(strFolder.c_str(), msPackageUpdateInfo.mstrProgramFiles.c_str(), msPackageUpdateInfo.mstrProgramFiles.length()))
            {
                return ;
            }

            int numFiles = 0,index = 0;
            std::vector<std::wstring> vScanPackageUpdateFiles;
            std::wstring strChosseUpdateFile;
            JGW_TraverseFolder_VC(strFolder,numFiles,L"\\*.*",ScanPackageUpdateFiles,&vScanPackageUpdateFiles);
            for (size_t i = 0;i < vScanPackageUpdateFiles.size();i ++)
            {
                strChosseUpdateFile = vScanPackageUpdateFiles[i];
                JGW_ReplaceStringW(strChosseUpdateFile,msPackageUpdateInfo.mstrProgramFiles,L"");
                JGW_EraseLastAndFristTwoCharsW(strChosseUpdateFile,L'\\',L'/');

                if (!CheckFileIsAddUpdateList(index,strChosseUpdateFile))
                {
                    msPackageUpdateInfo.mvUpdateFilePath.push_back(strChosseUpdateFile);
                    CListTextElementUI* pListTextElementUi = mpPackageFileList->InsertItem( mpPackageFileList->GetList()->GetCount(),strChosseUpdateFile.c_str(),30);
                    pListTextElementUi->SetText(0,strChosseUpdateFile.c_str());
                } 
            }
            mpPackageFilesTotal->SetText(JGW_GetFormatWString(L"Package Files Total:%d",mpPackageFileList->GetList()->GetCount() + 1).c_str());
        }
        //! JGW_TraverseFolder_VC
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnClickPackageBtnMsg()
    {
        if (mpPackageFileList->GetList()->GetCount() > 0)
        {
            mpHomeVerticalLayout->SetEnabled(false);
            msPackageUpdateInfo.mvUpdateFilePath.clear();
            for (int i = 0;i < mpPackageFileList->GetList()->GetCount();i ++)
            {
                CListTextElementUI* pListTextElementUi = (CListTextElementUI*)mpPackageFileList->GetList()->GetItemAt(i);
                msPackageUpdateInfo.mvUpdateFilePath.push_back(pListTextElementUi->GetText(0));
            }

            mpPackageUpdateThread->PostThreadMessage(WM_THREAD_UPLOAD_PACKAGEUPDATEFILE_MSG);
        }
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnListMenuRightClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.ptMouse.x;
        point.y = msg.ptMouse.y;
        ClientToScreen(m_hWnd, &point);
        pMenu->Init(NULL,_T("list_menu.xml"),point,&m_PaintManager,&mpMenuCheckInfo);
        pMenu->ResizeMenu();
    }

    void CCJaGuarWavePackageUpdateToolDlg::OnMenuElementClick(TEventUI* pTEventUI)
    {
        const wchar_t* strName = (const wchar_t*)pTEventUI->wParam;
        if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("DeleteUpdateFileMenu")))
        {
            while (mpPackageFileList->GetSelectItemCount() > 0)
            {
                 mpPackageFileList->RemoveAt(mpPackageFileList->GetCurSel());
            }
        } 
        delete[] strName;
    }
}
