#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_LossConfig_Define.h"
#include "CJGW_LoadLossConfigImpl.h"
namespace JGW
{
    class CCJGW_LossConfigToolDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_LossConfigToolDlg(void);
        ~CCJGW_LossConfigToolDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_LossConfigToolDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 加载控制UI视图指针
        bool LoadControlUiClassPtr();
        //! 更新树视图
        bool UpdateTreeView();
        //!
        void OnUpdateLossView();
        //! 更新当前项的线损list
        void UpdateLossList(PS_CAL_PATH_NODE psCalPathNode);
        //! 保存当前list修改
        void SaveLossListChange();
        //! 响应设置线损
        void OnSetLossClick();
        //! 响应添加线损
        void OnAddLossClick();
        //! 保存线损
        void OnSaveLossClick();
    private:
        void LoadTargetProjectView();
        const wchar_t* CalPathNumberToCalDBLossString(int number);
    private:
        //! 是否修改当前配置
        bool mbIsChange;

        CComboUI* mpTargetCombo;
        CButtonUI* mpSetLossBtn;
        CButtonUI* mpAddLossBtn;
        CButtonUI* mpSaveLossBtn;
        CRichEditUI* mpLossFilePathEdt;
        CTreeViewUI* mpTreeView;
        CListTestCtrlUI* mpLossList;
        CHorizontalLayoutUI* mpDragHorizontalLayoutUI;

        CCJGW_LoadLossConfigImpl* mpLoadLossConfigImpl;
        S_EQUIP_PROJECT_CONFIG* mpsEquipProjectConfig;
        S_CAL_DB_LOSS_CONFIG msCalDBLossConfig;
        std::vector<S_EQUIP_PROJECT_CONFIG> mvsEquipProjectConfig;
    };
}