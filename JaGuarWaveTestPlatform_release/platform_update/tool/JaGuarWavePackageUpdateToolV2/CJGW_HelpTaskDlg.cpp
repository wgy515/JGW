#include "StdAfx.h"
#include "CJGW_HelpTaskDlg.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_ConfigSQLite.h"
#include "CJGW_TaskSQLite.h"

namespace JGW
{
    CCJGW_HelpTaskDlg::CCJGW_HelpTaskDlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig) : CHelpPackageUIInterface(psPackageUpdateConfig)
    {
    }


    CCJGW_HelpTaskDlg::~CCJGW_HelpTaskDlg(void)
    {
    }

    bool CCJGW_HelpTaskDlg::InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI)
    {
        {
            bool result = true;
            mpPaintManagerUI = pPaintManagerUI;
            FIND_UI(CComboBoxUI,TaskTaskCombo,pPaintManagerUI);
            result &= CHECK_UI(TaskTaskCombo);

            FIND_UI(CComboBoxUI,TaskInstallVersionCombo,pPaintManagerUI);
            result &= CHECK_UI(TaskInstallVersionCombo);

            FIND_UI(CComboBoxUI,TaskProjectCombo,pPaintManagerUI);
            result &= CHECK_UI(TaskProjectCombo);

            FIND_UI(CComboBoxUI,TaskConfigCombo,pPaintManagerUI);
            result &= CHECK_UI(TaskConfigCombo);

            FIND_UI(CEditUI,TaskDesEdit,pPaintManagerUI);
            result &= CHECK_UI(TaskDesEdit);

            FIND_UI(CButtonUI,UpdateTaskBtn,pPaintManagerUI);
            result &= CHECK_UI(UpdateTaskBtn);
            //! add task
            FIND_UI(CEditUI,AddTaskNumberEdit,pPaintManagerUI);
            result &= CHECK_UI(AddTaskNumberEdit);

            FIND_UI(CEditUI,AddTaskDesEdit,pPaintManagerUI);
            result &= CHECK_UI(AddTaskDesEdit);

            FIND_UI(CComboBoxUI,AddTaskInstallVersionCombo,pPaintManagerUI);
            result &= CHECK_UI(AddTaskInstallVersionCombo);

            FIND_UI(CComboBoxUI,AddTaskProjectCombo,pPaintManagerUI);
            result &= CHECK_UI(AddTaskProjectCombo);

            FIND_UI(CComboBoxUI,AddTaskConfigCombo,pPaintManagerUI);
            result &= CHECK_UI(AddTaskConfigCombo);

            FIND_UI(CButtonUI,UploadAddTaskBtn,pPaintManagerUI);
            result &= CHECK_UI(UploadAddTaskBtn);

            if (!result)
            {
                return false;
            }

            mpUpdateTaskBtn->SetEnabled(false);
            mpUploadAddTaskBtn->SetEnabled(false);
            mpTaskConfigCombo->SetEnabled(false);
            mpAddTaskConfigCombo->SetEnabled(false);
        }
        {
            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsTask.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = JGW_A2W(mpsPackageUpdateConfig->mvsTask[index].mstrName,CP_UTF8);
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate(&mpsPackageUpdateConfig->mvsTask[index]);
                mpTaskTaskCombo->Add(pListLabelEle);
            }

            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsInstallVersion.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = mpsPackageUpdateConfig->mvsInstallVersion[index].mstrVersion;
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate((LPVOID)mpsPackageUpdateConfig->mvsInstallVersion[index].mnID);
                mpAddTaskInstallVersionCombo->Add(pListLabelEle);
            }

            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsInstallVersion.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = mpsPackageUpdateConfig->mvsInstallVersion[index].mstrVersion;
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate((LPVOID)mpsPackageUpdateConfig->mvsInstallVersion[index].mnID);
                mpTaskInstallVersionCombo->Add(pListLabelEle);
            }

            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsProject.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = mpsPackageUpdateConfig->mvsProject[index].mstrName;
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate((LPVOID)mpsPackageUpdateConfig->mvsProject[index].mnID);
                mpTaskProjectCombo->Add(pListLabelEle);
            }

            for (size_t index = 0;index < mpsPackageUpdateConfig->mvsProject.size();index++)
            {
                CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                std::wstring strTemp = mpsPackageUpdateConfig->mvsProject[index].mstrName;
                pListLabelEle->SetText(strTemp.c_str());
                pListLabelEle->SetDate((LPVOID)mpsPackageUpdateConfig->mvsProject[index].mnID);
                mpAddTaskProjectCombo->Add(pListLabelEle);
            }
        }

        return true;
    }

    void CCJGW_HelpTaskDlg::OnNotifyHelpPackageUI(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (msg.pSender == mpTaskTaskCombo)
            {
                mpTaskInstallVersionCombo->SelectItem(0);
                mpTaskProjectCombo->SelectItem(0);
                mpTaskConfigCombo->SelectItem(0);
                mpTaskDesEdit->SetText(L"");
                mpUpdateTaskBtn->SetEnabled(false);
                //! 选择订单号组合框
                if (mpTaskTaskCombo->GetCurSel() > 0)
                {
                    S_TASK *psTask = (S_TASK *)mpTaskTaskCombo->GetItemAt(mpTaskTaskCombo->GetCurSel())->GetDate();
                    msTaskUpdateInfo.mnTaskID = psTask->mnID;
                    msTaskUpdateInfo.mstrTaskNumber = JGW_A2W(psTask->mstrName,CP_UTF8);
                    msTaskUpdateInfo.mnConfigID = psTask->mnConfigID;
                    msTaskUpdateInfo.mnInstallVersionID = psTask->mnInstallVersionID;
                    msTaskUpdateInfo.mstrTaskDes = JGW_A2W(psTask->mstrDes,CP_UTF8);
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_TASK_SELECT_ACTION_ID,NULL);
                }   
            }
            else if (msg.pSender == mpTaskProjectCombo)
            {
                //! 选择项目组合框
                while (mpTaskConfigCombo->GetCount() > 1) mpTaskConfigCombo->RemoveAt(1);
                mpTaskConfigCombo->SelectItem(0);
                mpTaskConfigCombo->SetEnabled(false);
                int projectID = (int)mpTaskProjectCombo->GetItemAt(mpTaskProjectCombo->GetCurSel())->GetDate();

                if (mpTaskProjectCombo->GetCurSel() > 0 && msTaskUpdateInfo.mnPorjectID != projectID)
                {
                    msTaskUpdateInfo.mnPorjectID = projectID ;
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_TASL_SELECT_PROJECT_ACTION_ID,NULL);
                }
            }
            else if (msg.pSender == mpAddTaskProjectCombo)
            {
                if (mpAddTaskProjectCombo->GetCurSel() > 0)
                {
                    int projectID = (int)mpAddTaskProjectCombo->GetItemAt(mpAddTaskProjectCombo->GetCurSel())->GetDate();
                    msTaskAddInfo.mnPorjectID = projectID;
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_ADD_TASL_SELECT_PROJECT_ACTION_ID,NULL);
                }
                else
                {
                    while (mpAddTaskConfigCombo->GetCount() > 1) mpAddTaskConfigCombo->RemoveAt(1);
                    mpAddTaskConfigCombo->SetEnabled(false);
                    mpUploadAddTaskBtn->SetEnabled(false);
                }
            }
            else if (msg.pSender == mpAddTaskConfigCombo)
            {
                if (mpAddTaskConfigCombo->GetCurSel() > 0)
                {
                    mpUploadAddTaskBtn->SetEnabled(true);
                }
                else
                {
                    mpUploadAddTaskBtn->SetEnabled(false);
                }
            }
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpUpdateTaskBtn)
            {
                if (mpTaskProjectCombo->GetCurSel() > 0 && mpTaskTaskCombo->GetCurSel() > 0 && mpTaskInstallVersionCombo->GetCurSel() > 0 && mpTaskConfigCombo->GetCurSel() > 0)
                {
                    S_TASK *psTask = (S_TASK *)mpTaskTaskCombo->GetItemAt(mpTaskTaskCombo->GetCurSel())->GetDate();
                    msTaskUpdateInfo.mnTaskID = psTask->mnID;
                    msTaskUpdateInfo.mnInstallVersionID = (int)mpTaskInstallVersionCombo->GetItemAt(mpTaskInstallVersionCombo->GetCurSel())->GetDate();
                    msTaskUpdateInfo.mnConfigID = (int)mpTaskConfigCombo->GetItemAt(mpTaskConfigCombo->GetCurSel())->GetDate();
                    msTaskUpdateInfo.mstrTaskDes = mpTaskDesEdit->GetText().GetData();
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_TASK_UPDATE_TASK_ACTION_ID,NULL);
                }
                else
                {
                    MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Please check task project config install version ui config",L"Error",MB_ICONERROR);
                }
            }
            else if (msg.pSender == mpUploadAddTaskBtn)
            {
                mpUploadAddTaskBtn->SetEnabled(false);
                mpAddTaskInstallVersionCombo->SetEnabled(false);
                mpAddTaskProjectCombo->SetEnabled(false);
                mpAddTaskConfigCombo->SetEnabled(false);
                mpTaskTaskCombo->SetEnabled(false);
                msTaskAddInfo.mstrTaskNumber = mpAddTaskNumberEdit->GetText().GetData();

                if (mpAddTaskConfigCombo->GetCurSel() > 0 && mpAddTaskProjectCombo->GetCurSel() > 0 && mpAddTaskInstallVersionCombo->GetCurSel() > 0 && msTaskAddInfo.mstrTaskNumber.size() > 0)
                {
                    msTaskAddInfo.mstrTaskDes = mpAddTaskDesEdit->GetText().GetData();
                    msTaskAddInfo.mnConfigID = (int)mpAddTaskConfigCombo->GetItemAt(mpAddTaskConfigCombo->GetCurSel())->GetDate();
                    msTaskAddInfo.mnInstallVersionID = (int)mpAddTaskInstallVersionCombo->GetItemAt(mpAddTaskInstallVersionCombo->GetCurSel())->GetDate();
                    PostThreadMessage(mpsPackageUpdateConfig->mnThreadID,WM_THREAD_ASYNC_FUNC_MSG,E_TASK_ADD_TASK_ACTION_ID,NULL);
                }
                else
                {
                    MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Please check task project config install version ui config",L"Error",MB_ICONERROR);
                }
            }
        }
    }

    void CCJGW_HelpTaskDlg::CloseHelpPackageUI()
    {

    }

    void CCJGW_HelpTaskDlg::OnHandleEventCustomMessage(TEventUI* pTEventUI)
    {
        if (pTEventUI->Type == WM_HWMD_OPERATION_COMPLETE_MSG)
        {
            OnRspOperationCompleteMsg((E_TASK_ASYNC_ACTION_ID)pTEventUI->wParam,pTEventUI->lParam == 1);
        }
    }

    bool CCJGW_HelpTaskDlg::OnHandleAsyncFuncMsg(int nActionIndex)
    {
        switch ((E_TASK_ASYNC_ACTION_ID)nActionIndex)
        {
        case E_TASK_SELECT_ACTION_ID:
            return OnRspTaskTaskComboItemSelectMsg();
        case E_TASL_SELECT_PROJECT_ACTION_ID:
            return OnRspTaskProjectComboItemSelectMsg();
        case E_TASK_UPDATE_TASK_ACTION_ID:
            return OnRspTaskUpdateTaskBtnClickMsg();
        case E_ADD_TASL_SELECT_PROJECT_ACTION_ID:
            return OnRspAddTaskProjectComboItemSelectMsg();;
        case E_TASK_ADD_TASK_ACTION_ID:
            return OnRspTaskAddTaskBtnClickMsg();
        }
        return false;
    }

    void CCJGW_HelpTaskDlg::OnRspOperationCompleteMsg(E_TASK_ASYNC_ACTION_ID eActionIndex,bool result)
    {
        if (E_TASK_SELECT_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                mpTaskDesEdit->SetText(msTaskUpdateInfo.mstrTaskDes.c_str());
                SelectItemCComboBoxUI(mpTaskInstallVersionCombo,msTaskUpdateInfo.mnInstallVersionID);
                SelectItemCComboBoxUI(mpTaskProjectCombo,msTaskUpdateInfo.mnPorjectID);
                mpTaskConfigCombo->SetEnabled(true);
                while (mpTaskConfigCombo->GetCount() > 1) mpTaskConfigCombo->RemoveAt(1);
                for (size_t index = 0;index < msTaskUpdateInfo.mvsConfigProject.size();index++)
                {
                    CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                    pListLabelEle->SetText(msTaskUpdateInfo.mvsConfigProject[index].mstrName.c_str());

                    std::wstring strTemp = msTaskUpdateInfo.mvsConfigProject[index].mstrName;
                    strTemp += L" ";
                    strTemp += msTaskUpdateInfo.mvsConfigProject[index].mstrVersion;
                    pListLabelEle->SetToolTip(msTaskUpdateInfo.mvsConfigProject[index].mstrVersion.c_str());
                    pListLabelEle->SetDate((LPVOID)msTaskUpdateInfo.mvsConfigProject[index].mnID);
                    mpTaskConfigCombo->Add(pListLabelEle);
                }
                SelectItemCComboBoxUI(mpTaskConfigCombo,msTaskUpdateInfo.mnConfigID);
                mpUpdateTaskBtn->SetEnabled(true);
            }
        }
        else if (E_TASL_SELECT_PROJECT_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                while (mpTaskConfigCombo->GetCount() > 1) mpTaskConfigCombo->RemoveAt(1);
                mpTaskConfigCombo->SetEnabled(true);

                for (size_t index = 0;index < msTaskUpdateInfo.mvsConfigProject.size();index++)
                {
                    CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                    std::wstring strTemp = msTaskUpdateInfo.mvsConfigProject[index].mstrName;
                    pListLabelEle->SetText(strTemp.c_str());
                    pListLabelEle->SetToolTip(msTaskUpdateInfo.mvsConfigProject[index].mstrVersion.c_str());
                    pListLabelEle->SetDate((LPVOID)msTaskUpdateInfo.mvsConfigProject[index].mnID);
                    mpTaskConfigCombo->Add(pListLabelEle);
                }
            }
        }
        else if (E_TASK_UPDATE_TASK_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                mpUpdateTaskBtn->SetEnabled(false);
                mpTaskTaskCombo->SetEnabled(false);
                MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The task information is modified successfully.",L"Succeed",MB_OK);
            }
            else
            {
                mpUpdateTaskBtn->SetEnabled(false);
            }
        }
        else if (E_ADD_TASL_SELECT_PROJECT_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                while (mpAddTaskConfigCombo->GetCount() > 1) mpAddTaskConfigCombo->RemoveAt(1);
                mpAddTaskConfigCombo->SetEnabled(true);
                for (size_t index = 0;index < msTaskAddInfo.mvsConfigProject.size();index++)
                {
                    CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
                    std::wstring strTemp = msTaskAddInfo.mvsConfigProject[index].mstrName;
                    pListLabelEle->SetText(strTemp.c_str());
                    pListLabelEle->SetToolTip(msTaskAddInfo.mvsConfigProject[index].mstrVersion.c_str());
                    pListLabelEle->SetDate((LPVOID)msTaskAddInfo.mvsConfigProject[index].mnID);
                    mpAddTaskConfigCombo->Add(pListLabelEle);
                }
            }
        }
        else if (E_TASK_ADD_TASK_ACTION_ID == eActionIndex)
        {
            if (result)
            {
                mpUpdateTaskBtn->SetEnabled(false);
                MessageBox(mpPaintManagerUI->GetPaintWindow(),L"The task information is add successfully.",L"Succeed",MB_OK);
            }
            else
            {
                mpUpdateTaskBtn->SetEnabled(false);
            }
        }
    }

    void CCJGW_HelpTaskDlg::SelectItemCComboBoxUI(CComboBoxUI *pComboBoxUI,int id)
    {
        for (int index = 0;index < pComboBoxUI->GetCount();index ++)
        {
            if ((int)(pComboBoxUI->GetItemAt(index)->GetDate()) == id)
            {
                pComboBoxUI->SelectItem(index);
            }
        }
    }

    bool CCJGW_HelpTaskDlg::OnRspTaskTaskComboItemSelectMsg()
    {
        S_CONFIG sConfig;

        CCJGW_ConfigSQLite configSQLite;
        if (!configSQLite.GetConfigDBToConfigID(mpsPackageUpdateConfig->mcSQLite,msTaskUpdateInfo.mnConfigID,sConfig))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Config DB Fail",L"Error",MB_ICONERROR);
            return false;
        }

        msTaskUpdateInfo.mnPorjectID = sConfig.mnProjectID;
        if (!configSQLite.GetConfigDBToProjectID(mpsPackageUpdateConfig->mcSQLite,msTaskUpdateInfo.mnPorjectID,msTaskUpdateInfo.mvsConfigProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Config DB To Project Fail",L"Error",MB_ICONERROR);
            return false;
        }

        return true;
    }

    bool CCJGW_HelpTaskDlg::OnRspTaskProjectComboItemSelectMsg()
    {
        CCJGW_ConfigSQLite configSQLite;
        if (!configSQLite.GetConfigDBToProjectID(mpsPackageUpdateConfig->mcSQLite,msTaskUpdateInfo.mnPorjectID,msTaskUpdateInfo.mvsConfigProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Config DB To Project Fail",L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGW_HelpTaskDlg::OnRspTaskUpdateTaskBtnClickMsg()
    {
        S_TASK sTask;
        sTask.mnID = msTaskUpdateInfo.mnTaskID;
        sTask.mnConfigID = msTaskUpdateInfo.mnConfigID;
        sTask.mnInstallVersionID = msTaskUpdateInfo.mnInstallVersionID;
        sTask.mstrDes = JGW_W2A(msTaskUpdateInfo.mstrTaskDes,CP_UTF8);

        CCJGW_TaskSQLite taskSQLite;
        if (!taskSQLite.UpdateTaskSQLite(mpsPackageUpdateConfig->mcSQLite,sTask))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Error updating task information",L"Error",MB_ICONERROR);
            return false;
        }

        if (!taskSQLite.LoadTaskSQLite(mpsPackageUpdateConfig->mcSQLite,mpsPackageUpdateConfig->mvsTask))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Retry load task information fail",L"Error",MB_ICONERROR);
            return false;
        }


        return true;
    }

    bool CCJGW_HelpTaskDlg::OnRspAddTaskProjectComboItemSelectMsg()
    {
        CCJGW_ConfigSQLite configSQLite;
        if (!configSQLite.GetConfigDBToProjectID(mpsPackageUpdateConfig->mcSQLite,msTaskAddInfo.mnPorjectID,msTaskAddInfo.mvsConfigProject))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Get Config DB To Project Fail",L"Error",MB_ICONERROR);
            return false;
        }
        return true;
    }

    bool CCJGW_HelpTaskDlg::OnRspTaskAddTaskBtnClickMsg()
    {
        S_TASK sTask;
        sTask.mstrName = JGW_W2A(msTaskAddInfo.mstrTaskNumber,CP_UTF8);
        sTask.mnConfigID = msTaskAddInfo.mnConfigID;
        sTask.mnInstallVersionID = msTaskAddInfo.mnInstallVersionID;
        sTask.mnOTAID = -1;
        sTask.mnTaskConfigID = 1;
        sTask.mstrDes = JGW_W2A(msTaskAddInfo.mstrTaskDes,CP_UTF8);

        CCJGW_TaskSQLite taskSQLite;
        if (!taskSQLite.AddTaskSQLite(mpsPackageUpdateConfig->mcSQLite,sTask))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Error add task information",L"Error",MB_ICONERROR);
            return false;
        }

        if (!taskSQLite.LoadTaskSQLite(mpsPackageUpdateConfig->mcSQLite,mpsPackageUpdateConfig->mvsTask))
        {
            MessageBox(mpPaintManagerUI->GetPaintWindow(),L"Retry load task information fail",L"Error",MB_ICONERROR);
            return false;
        }

        return true;
    }
}
