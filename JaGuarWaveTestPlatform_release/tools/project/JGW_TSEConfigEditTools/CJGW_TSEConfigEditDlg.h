#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "JGWViewLayoutConfig.h"
#include "CJGW_TSEFactoryConfig.h"
namespace JGW
{
    typedef enum
    {
        E_UNSELECTED = 0, //! 未选中状态
        E_SELECTED = 1, //! 选中状态
        E_PART_SELECTED = 2, //! 部分选中状态
        E_UNKNOW_CHECKBOX
    }E_CHECKBOX_SELECTED_STATUS;

    typedef struct
    {
        CEditUI* mpFristEditUI;
        CRichEditUI* mpSecondRichEditUI;
        CEditUI* mpThreeEditUI;
        CListContainerElementUIEx* mpListContainerElementUIEx;
    }S_TSE_LIST_UI_INFO,*LPS_TSE_LIST_UI_INFO;

    typedef struct  
    {
        _mMapGlobalVariables mmapGlobalVariables;
        std::vector<s_single_item_test_config*> mvsSingleItemTestConfig;
    }S_TSE_MULTI_UI;

    // s_single_item_test_config _mMapGlobalVariables
    typedef struct
    {
        bool mbIsTSETest;
        CTreeNodeUI* mpPrevTreeNodeUI;
        union
        {
            s_single_item_test_config* mpsItemTestConfig;
            _mMapGlobalVariables* mmapGlobalVariables;
        };
    } S_TREENODE_DATA;
    /*
    1、加载UI视图框架
    2、加载uiconfig.xml文件根据<prev_plugin_info_index project_index="1" group_index="0" /,并初始化project-group-pln CComboUI
    3、加载首个插件文件suite_name.xml文件并初始化treeviewui
    4、
    */
    class CCJGW_TSEConfigEditDlg : public CCAsyncMsgDialog,public IMessageFilterUI
    {
    public:
        CCJGW_TSEConfigEditDlg(void);
        ~CCJGW_TSEConfigEditDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_TSEConfigEditDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 加载控制UI视图指针
        bool LoadControlUiClassPtr();
        //! 加载主框架皮肤文件
        bool InitMainFrameSkinConfig();
        //
        void AddTreeViewChildNode(CTreeNodeUI* pTreeNodeUI);
    private:
        // S_PROJECT_GROUP_PLUGIN_INFO 响应测试项目修改
        void OnRspProjectGroupPluginInfoChange(S_PROJECT_GROUP_PLUGIN_INFO* psProjectGroupPluginInfo);
        // S_PLUGIN_GROUP_INFO 响应测试组修改
        void OnRspPluginGroupInfoChange(S_PLUGIN_GROUP_INFO* psPluginGroupInfo);
        // S_PLUGIN_INFO 响应测试节点修改
        void OnRspPluginInfoChange(S_PLUGIN_INFO* psPluginInfo);
        // 添加下拉组合框
        void AddComboControlUI(CComboUI* mpCComboUI,const std::wstring& strValue,LPVOID lpVoid);
        //! 检查当前插件是否是TSE插件
        bool CheckPluginTestIsTSEPlugin(const std::wstring& strPluginName);
        // 清空测试树视图以及编辑List
        void ClearTreeAndListView();
        // 创建TreeView节点
        CTreeNodeUI* BuildTreeNodeUI(const std::wstring& strFolderAttr,const std::wstring& strCheckBoxAttr,bool isTest = true);
        // 添加TSE测试配置节点
        void AddSuiteTestConfigTreeNodes(CTreeNodeUI* pParentTreeNodeUI,std::shared_ptr<s_suite_test_config>& ptrSuiteTestConfig,CTreeNodeUI* pSiblingTreeNodeUI = NULL);
        // 展开当前节点
        void ExpandCTreeNodeUI(CTreeNodeUI* pTreeNodeUI,bool isExpand);
        // 响应TreeView视图检查框事件更新
        bool OnTreeViewCheckBoxChanged(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
    private:
        // 更新TreeView视图
        void OnUpdateTreeView();
        // 设置Tree项目对话框
        void SetTreeItemCheckBox(bool isSelect,CTreeNodeUI* pTreeNode);
        //
        void DeleteTreeViewCheckBoxData(CTreeNodeUI* pTreeNode);
        // 更新CheckBox视图框
        E_CHECKBOX_SELECTED_STATUS UpdateTreeViewCheckBoxView(CTreeNodeUI* pTreeNodeUI);
        // 更新TreeNode UI
        E_CHECKBOX_SELECTED_STATUS UpdateTreeNodeUI(CTreeNodeUI* pTreeNodeU);
    private:
        void CreateTestSuiteItemLList(s_single_item_test_config* psSuiteItemTestConfig);
        //
        void SaveTestSuiteItem();
        //
        LPS_TSE_LIST_UI_INFO AddEmptyTSETestParam();
        //
        std::vector<S_TSE_LIST_UI_INFO>::iterator FindRichEditUIIsTseList(CRichEditUI* pRichEditUI);
    private:
        void CreateGlobalVariablesList(_mMapGlobalVariables* mmapGlobalVariables);
    private:
        void CopyMapGlobalVariables(_mMapGlobalVariables& mmapDest,_mMapGlobalVariables& mmapSrc);
        s_single_item_test_config* CopySingleItemTestConfig(s_single_item_test_config& sSingleItemTestConfig);
        void SaveTSEConfig();
        void OnEditMenuBtnClick(TNotifyUI& msg);
        //! 响应WINDOWS 消息
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        void OnMenuElementClick(TEventUI* pTEventUI);
        virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
        //bool OnSuiteTreeMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
    private:
        bool mbIsSaveTSEConfig;
        bool mbIsInitTSEListCompleted;
        bool mbIsRspCheckBoxChanged;
        //! 标题栏
        CLabelUI* mpTipslab;
        CLabelUI* mpSaveTipsLab;
        CComboUI* mpProjectCombo;
        CComboUI* mpProjectGroupCombo;
        CComboUI* mpPluginNameCombo;
        CButtonUI* mpAddSuiteBtn;
        CButtonUI* mpDelSuiteBtn;
        CTreeViewUI* mpPluinSuiteTree;
        CTreeNodeUI* mpHomeTreeNode;
        CListTestCtrlUI* mpSuiteList;
        CLabelUI* mpEditTips;
        CHorizontalLayoutUI* mpDragHorizontalLayoutUI;
        std::vector<S_TSE_LIST_UI_INFO> mvTSEListUIInfo;
    private:
        CTreeNodeUI* mpSelectTreeNodeUI;
        S_TREENODE_DATA msTreeNodeData;
        S_TSE_MULTI_UI msTSEMultiUI;
        CCJGWViewLayoutConfig mcTSEConfigEditDlg;
        CCJGW_TSEFactoryConfig mcTSEFactoryConfig;
    };
}
