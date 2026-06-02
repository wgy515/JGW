#include "StdAfx.h"
#include "CJGW_HelpProjectDlg.h"
#include "CJGW_ProjectSQLite.h"
namespace JGW
{
    CCJGW_HelpProjectDlg::CCJGW_HelpProjectDlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig) : CHelpPackageUIInterface(psPackageUpdateConfig)
    {
    }


    CCJGW_HelpProjectDlg::~CCJGW_HelpProjectDlg(void)
    {
    }

    bool CCJGW_HelpProjectDlg::InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI)
    {
        mpPaintManagerUI = pPaintManagerUI;
        {
            bool result = true;

            FIND_UI(CComboBoxUI,ProjectNameCombo,pPaintManagerUI);
            FIND_UI(CEditUI,ProjectNameEdit,pPaintManagerUI);
            FIND_UI(CEditUI,ProjectDesEdit,pPaintManagerUI);
            FIND_UI(CButtonUI,ProjectUpdateBtn,pPaintManagerUI);
            result &= CHECK_UI(ProjectNameCombo);
            result &= CHECK_UI(ProjectNameEdit);
            result &= CHECK_UI(ProjectDesEdit);
            result &= CHECK_UI(ProjectUpdateBtn);

            FIND_UI(CEditUI,AddProjectNameEdit,pPaintManagerUI);
            FIND_UI(CEditUI,AddProjectDesEdit,pPaintManagerUI);
            FIND_UI(CButtonUI,UploadAddProjectPackageBtn,pPaintManagerUI);
            result &= CHECK_UI(AddProjectNameEdit);
            result &= CHECK_UI(AddProjectDesEdit);
            result &= CHECK_UI(UploadAddProjectPackageBtn);

            if (!result)
            {
                return false;
            }
            mpProjectUpdateBtn->SetEnabled(false);
        }
        {
            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsProject.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = mpsPackageUpdateConfig->mvsProject[index].mstrName;
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate((LPVOID)&mpsPackageUpdateConfig->mvsProject[index]);
                mpProjectNameCombo->Add(pListLabelEle);
            }
        }

        return true;
    }

    void CCJGW_HelpProjectDlg::OnNotifyHelpPackageUI(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpProjectUpdateBtn == msg.pSender)
            {
                if (mpProjectNameCombo->GetCurSel() <= 0 || mpProjectNameEdit->GetText().GetLength() == 0)
                {
                    MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Please check project ui config",L"Error",MB_ICONERROR);
                }
                else
                {
                    OnRspProjectUpdateBtnClickMsg();
                }
            }
            else if (mpUploadAddProjectPackageBtn == msg.pSender)
            {
                if (mpAddProjectNameEdit->GetText().GetLength() == 0)
                {
                    MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Please check project ui config",L"Error",MB_ICONERROR);
                }
                else
                {
                    OnRspUploadProjectAddBtnClickMsg();
                }
            }
        }
        else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (mpProjectNameCombo == msg.pSender)
            {
                mpProjectNameEdit->SetText(L"");
                mpProjectDesEdit->SetText(L"");
                mpProjectUpdateBtn->SetEnabled(false);
                if (mpProjectNameCombo->GetCurSel() > 0)
                {
                    S_PROJECT *psProject = (S_PROJECT *)mpProjectNameCombo->GetItemAt(mpProjectNameCombo->GetCurSel())->GetDate();
                    mpProjectNameEdit->SetText(psProject->mstrName.c_str());
                    mpProjectDesEdit->SetText(psProject->mstrDes.c_str());
                    mpProjectUpdateBtn->SetEnabled(true);
                }
            }
        }
    }

    void CCJGW_HelpProjectDlg::CloseHelpPackageUI()
    {

    }

    void CCJGW_HelpProjectDlg::OnHandleEventCustomMessage(TEventUI* pTEventUI)
    {

    }

    bool CCJGW_HelpProjectDlg::OnHandleAsyncFuncMsg(int nActionIndex)
    {
        return false;
    }

    bool CCJGW_HelpProjectDlg::OnRspProjectUpdateBtnClickMsg()
    {
        S_PROJECT *psProject = (S_PROJECT *)mpProjectNameCombo->GetItemAt(mpProjectNameCombo->GetCurSel())->GetDate();
        S_PROJECT sProject;
        sProject.mnID = psProject->mnID;
        sProject.mstrName = mpProjectNameEdit->GetText().GetData();
        sProject.mstrDes = mpProjectDesEdit->GetText().GetData();

        CCJGW_ProjectSQLite projectSQLite;
        if (!projectSQLite.UpdateProjectInfo(mpsPackageUpdateConfig->mcSQLite,sProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The project information is modified failed.",L"Error",MB_ICONERROR);
            return false;
        }
        MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The project information is modified successfully.",L"Succeed",MB_OK);
        mpProjectNameCombo->SetEnabled(false);
        mpProjectUpdateBtn->SetEnabled(false);
        mpProjectNameEdit->SetEnabled(false);
        mpProjectDesEdit->SetEnabled(false);
        if (!projectSQLite.LoadProjectSQLite(mpsPackageUpdateConfig->mcSQLite,mpsPackageUpdateConfig->mvsProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Project information  failed.",L"Error",MB_ICONERROR);
        }

        return true;
    }

    bool CCJGW_HelpProjectDlg::OnRspUploadProjectAddBtnClickMsg()
    {
        S_PROJECT sProject;
        sProject.mstrName = mpAddProjectNameEdit->GetText().GetData();
        sProject.mstrDes = mpAddProjectDesEdit->GetText().GetData();

        CCJGW_ProjectSQLite projectSQLite;
        if (!projectSQLite.AddProjectInfo(mpsPackageUpdateConfig->mcSQLite,sProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The add project information failed.",L"Error",MB_ICONERROR);
            return false;
        }

        MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The project information is add successfully.",L"Succeed",MB_OK);
        mpProjectNameCombo->SetEnabled(false);
        mpProjectUpdateBtn->SetEnabled(false);
        mpProjectNameEdit->SetEnabled(false);
        mpProjectDesEdit->SetEnabled(false);
        if (!projectSQLite.LoadProjectSQLite(mpsPackageUpdateConfig->mcSQLite,mpsPackageUpdateConfig->mvsProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Project information  failed.",L"Error",MB_ICONERROR);
        }

        return true;
    }
}

