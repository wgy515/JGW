#pragma once
#include "HelpPackageUIInterface.h"
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    typedef enum
    {
        E_TASK_SELECT_ACTION_ID = 0,
        E_TASL_SELECT_PROJECT_ACTION_ID = 1,
        E_TASK_UPDATE_TASK_ACTION_ID = 2,
        E_ADD_TASL_SELECT_PROJECT_ACTION_ID = 3,
        E_TASK_ADD_TASK_ACTION_ID = 4,
    } E_TASK_ASYNC_ACTION_ID;

    typedef struct  
    {
        int mnTaskID;
        int mnPorjectID;
        int mnConfigID;
        int mnInstallVersionID;
        std::wstring mstrTaskNumber;
        std::wstring mstrProjectName;
        std::wstring mstrConfigName;
        std::wstring mstrTaskDes;
        std::vector<S_CONFIG> mvsConfigProject;
    }S_TASK_UPDATE_INFO;


    typedef struct  
    {
        int mnTaskID;
        int mnPorjectID;
        int mnConfigID;
        int mnInstallVersionID;
        std::wstring mstrTaskNumber;
        std::wstring mstrProjectName;
        std::wstring mstrConfigName;
        std::wstring mstrTaskDes;
        std::vector<S_CONFIG> mvsConfigProject;
    }S_TASK_ADD_INFO;

    class CCJGW_HelpTaskDlg  : public CHelpPackageUIInterface
    {
    public:
        CCJGW_HelpTaskDlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig);
        ~CCJGW_HelpTaskDlg(void);
    private:
        //! 初始化UI
        bool InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI);
        //! 响应UI点击事件通知
        void OnNotifyHelpPackageUI(TNotifyUI& msg);
        //! 关闭UI事件回调
        void CloseHelpPackageUI();
        //! 处理客户事件消息
        void OnHandleEventCustomMessage(TEventUI* pTEventUI);
        //! 处理异步函数调用消息
        bool OnHandleAsyncFuncMsg(int nActionIndex);
    private:
        void OnRspOperationCompleteMsg(E_TASK_ASYNC_ACTION_ID eActionIndex,bool result);
        void SelectItemCComboBoxUI(CComboBoxUI *pComboBoxUI,int id);
        bool OnRspTaskTaskComboItemSelectMsg();
        bool OnRspTaskProjectComboItemSelectMsg();
        bool OnRspTaskUpdateTaskBtnClickMsg();
        bool OnRspAddTaskProjectComboItemSelectMsg();
        bool OnRspTaskAddTaskBtnClickMsg();
    private:
        MEMBER_VARIABLE_UI(CComboBoxUI,TaskTaskCombo);
        MEMBER_VARIABLE_UI(CComboBoxUI,TaskInstallVersionCombo);
        MEMBER_VARIABLE_UI(CComboBoxUI,TaskProjectCombo);
        MEMBER_VARIABLE_UI(CComboBoxUI,TaskConfigCombo);
        MEMBER_VARIABLE_UI(CEditUI,TaskDesEdit);
        MEMBER_VARIABLE_UI(CButtonUI,UpdateTaskBtn);

        MEMBER_VARIABLE_UI(CEditUI,AddTaskNumberEdit);
        MEMBER_VARIABLE_UI(CEditUI,AddTaskDesEdit);
        MEMBER_VARIABLE_UI(CComboBoxUI,AddTaskInstallVersionCombo);
        MEMBER_VARIABLE_UI(CComboBoxUI,AddTaskProjectCombo);
        MEMBER_VARIABLE_UI(CComboBoxUI,AddTaskConfigCombo);
        MEMBER_VARIABLE_UI(CButtonUI,UploadAddTaskBtn);
    private:
        CPaintManagerUI* mpPaintManagerUI;
        S_TASK_UPDATE_INFO msTaskUpdateInfo;
        S_TASK_ADD_INFO msTaskAddInfo;
    };
}

