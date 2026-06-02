#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "TSE_ConfigEditToolController.h"
#include "JGWViewLayoutConfig.h"
#include "CTSE_ConfigEditTool_Define.h"
#include "TSE_LoadTSESuiteConfig.h"
//! UI视图
namespace JGW
{
    class CTSE_ConfigEditToolDlg: public CCAsyncMsgDialog,public IMessageFilterUI
    {
    public:
        CTSE_ConfigEditToolDlg(void);
        ~CTSE_ConfigEditToolDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CTSE_ConfigEditToolDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
#if 0
        DUI_DECLARE_MESSAGE_MAP_B()
#endif 
    private:
        //! 加载控制UI视图指针
        bool LoadControlUiClassPtr();
        //! 
        bool LoadUIConfig();
        //! 
        bool LoadTSEProjectUI();
        //! 
        void GetDefaultTSEProjectSuiteFilePath();
        //! 
        void UpdateTSETreeView();
        //!
        void UpdateTSEListView(LPS_TSE_TREE_NODE_DATA psTSETreeNodeData);
        //!
        void AddSubTreeNode(_vsuitetestconfig& vSuiteTestConfigParam,CTreeNodeUI* pTreeNodeUi,E_TREENODEUI_TYPE eTreeNodeUi);
        //!
        void AddSubSigleTreeNode(std::shared_ptr<S_TSE_TREE_NODE_DATA> ptrTSETreeNode,CTreeNodeUI* pTreeNodeUi);
    private:
        void UpdateTSTListSuiteItem(s_suite_item_test_config* psSuiteItemTestConfig);
        void UpdateTSTListEnvironment(_mcommontestconfig* psEnvironmentConfig);
        void SaveTSEListMemroyConfig();
        void SaveTSTListSuiteItemMemroyConfig(s_suite_item_test_config* psSuiteItemTestConfig);
        void SaveTSTListEnvironmentMemroyConfig(_mcommontestconfig* psEnvironmentConfig);
        CTreeNodeUI* DefaultBuildTreeNodeUI();
    private:
        LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

        bool OnClickCheckBox(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
    private:
        CTreeViewUI* mpTreeView;
        //!
        CHorizontalLayoutUI* mpDragHorizontalLayoutUI;
        //!
        CHorizontalLayoutUI* mpSuiteListHorizontalEdit;
        //! Log插件试图布局
        CHorizontalLayoutUI* mpLogHorizontalLayout;
#if 0
        CButtonUI* mpAddSuiteBtn;
        CButtonUI* mpDelSuiteBtn;
        CButtonUI* mpEditMenuBtn;
        CButtonUI* mpProjectMenuBtn;
        CButtonUI* mpTestMenuBtn;
        CButtonUI* mpLogMenuBtn;
#endif
        CListTestCtrlUI* mpSuiteList;
        CLabelUI* mpQueryHintsLabel;
        CLabelUI* mpTipLabel;
        CLabelUI* mpUpdateLabel;

        //! 菜单栏点击状态
        map<CDuiString,bool> mpMenuCheckInfo;
        friend class CTSE_ConfigEditToolController;
        //! MVC Controller
        CTSE_ConfigEditToolController mUIController;
    private:
        bool mbReSaveTestConfig;

        std::wstring mstrSelectModuleFolder;
        //! TSE SUITE XML文件路径
        std::wstring mstrTSEProjectSuiteXmlFilePath;
        //! 测试项目组信息
        const std::vector<S_PROJECT_GROUP_PLUGIN_INFO>* mpvsProjectGroupPlnInfo;       
        //! 加载UIConfig.xml
        CCJGWViewLayoutConfig mJGWViewLayoutConfig;
        //! TSE 测试参数组
        S_TSE_SequenceConfig msTSESequenceConfigParam;
        //!
        CTSE_LoadTSESuiteConfig mLoadTSESuiteConfig;
        //!
        std::list<std::shared_ptr<S_TSE_TREE_NODE_DATA>> mvsTSETreeNodeData;
    };
}