#include "StdAfx.h"
#include "TSE_ConfigEditToolDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

//! 1 按下 0 UP
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 

#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1) 

namespace JGW
{
    CTSE_ConfigEditToolDlg::CTSE_ConfigEditToolDlg(void) : mUIController(this),mbReSaveTestConfig(false),mLoadTSESuiteConfig(msTSESequenceConfigParam)
    {
    }


    CTSE_ConfigEditToolDlg::~CTSE_ConfigEditToolDlg(void)
    {

    }

    void CTSE_ConfigEditToolDlg::OnCloseWindow()
    {
         m_PaintManager.GetEventSource() -= MakeDelegate(&mUIController,&CTSE_ConfigEditToolController::OnEventCustomMessage,0);
        if (mpUpdateLabel && mpUpdateLabel->IsVisible() && IDOK == MessageBox(m_hWnd,L"是否保存当前已修改的配置文件",L"是否保存",MB_OKCANCEL))
        {
            mLoadTSESuiteConfig.SaveTSESuiteConfig();
        }
        m_PaintManager.RemoveMessageFilter(this);
    }

    void CTSE_ConfigEditToolDlg::OnInitWindow()
    {
        {
            if (!LoadControlUiClassPtr()) return ;
            //! 添加按键监视
            m_PaintManager.AddMessageFilter(this);
            //! 加载项目UI配置
            if (!LoadUIConfig()) return;
            //! 
            GetDefaultTSEProjectSuiteFilePath();
            //! 
            LoadTSEProjectUI();
            //! 
            UpdateTSETreeView();
        }

        //! 响应事件消息
        {
            m_PaintManager.GetEventSource() += MakeDelegate(&mUIController,&CTSE_ConfigEditToolController::OnEventCustomMessage,0);
            mUIController.OnResponceViewLogInfoMsg(NULL,NULL);
        }     
    }
    LRESULT CTSE_ConfigEditToolDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
    {
        return mUIController.MessageHandler(uMsg,wParam,lParam,bHandled);
    }

    void CTSE_ConfigEditToolDlg::OnNotify(TNotifyUI& msg)
    {
        mUIController.OnViewNotify(msg);
    }

    bool CTSE_ConfigEditToolDlg::LoadControlUiClassPtr()
    {
        bool bFindControlOk = true;
        //! tree
        mpTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(L"tree"));
        mpDragHorizontalLayoutUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"drag"));
        mpLogHorizontalLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"logHorizontalLayout"));
        mpSuiteList = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(L"suiteList"));
        mpTipLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"tipslab"));
        mpUpdateLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"updateLab"));
        mpSuiteListHorizontalEdit = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"SuiteListHorizontalEdit"));
        bFindControlOk &= (NULL != mpLogHorizontalLayout);
        bFindControlOk &= (NULL != mpTreeView);
        bFindControlOk &= (NULL != mpDragHorizontalLayoutUI);
        bFindControlOk &= (NULL != mpSuiteList);
        bFindControlOk &= (NULL != mpTipLabel);
        bFindControlOk &= (NULL != mpUpdateLabel);
        bFindControlOk &= (NULL != mpSuiteListHorizontalEdit);
#if 0
        //! MENU
        mpEditMenuBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"EditMenuBtn"));
        mpProjectMenuBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"ProjectMenuBtn"));
        mpTestMenuBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"TestMenuBtn"));
        mpLogMenuBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"LogMenuBtn"));
        mpAddSuiteBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"AddSuiteBtn"));
        mpDelSuiteBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"DelSuiteBtn"));
        bFindControlOk &= (NULL != mpEditMenuBtn);
        bFindControlOk &= (NULL != mpProjectMenuBtn);
        bFindControlOk &= (NULL != mpTestMenuBtn);
        bFindControlOk &= (NULL != mpLogMenuBtn);
        bFindControlOk &= (NULL != mpAddSuiteBtn);
        bFindControlOk &= (NULL != mpDelSuiteBtn);
#endif
        //! 
        mpQueryHintsLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"queryhints"));
        bFindControlOk &= (NULL != mpQueryHintsLabel);

        if(!bFindControlOk) { Close(IDCANCEL); return bFindControlOk;}
        mpTreeView->m_pDragingCtrl = mpDragHorizontalLayoutUI;
        mpTreeView->SetAllowDiffParentNodeMove(true);

        return bFindControlOk;
    }

    bool CTSE_ConfigEditToolDlg::LoadUIConfig()
    {
        mJGWViewLayoutConfig.LoadViewLayoutConfig(JGW_W2A(JGW_GetSkinUIConfigFilePath()));
        mpvsProjectGroupPlnInfo = &mJGWViewLayoutConfig.GetUIProjectInfo()->mvProjectGroupPluginInfo;
        return true;
    }

    void CTSE_ConfigEditToolDlg::GetDefaultTSEProjectSuiteFilePath()
    {
        if (mpvsProjectGroupPlnInfo->empty() || mpvsProjectGroupPlnInfo->at(0).mvAllPluginInfo.empty() || mpvsProjectGroupPlnInfo->at(0).mvAllPluginInfo[0].mvSubWinInfo.empty()) return ;

        mstrSelectModuleFolder = mpvsProjectGroupPlnInfo->at(0).mvAllPluginInfo[0].mvSubWinInfo.at(0).mstrModuleFolderName;
        mstrTSEProjectSuiteXmlFilePath = JGW_GetTSEConfigFilePath(mpvsProjectGroupPlnInfo->at(0).mvAllPluginInfo[0].mvSubWinInfo.at(0).mstrModuleFolderName,mpvsProjectGroupPlnInfo->at(0).mvAllPluginInfo[0].mvSubWinInfo.at(0).mstrSuiteXmlName);
    }

    bool CTSE_ConfigEditToolDlg::LoadTSEProjectUI()
    {
        if (mstrTSEProjectSuiteXmlFilePath.empty() || !JGW_FileExistsToFilePath(mstrTSEProjectSuiteXmlFilePath.c_str())) return false;

        if (!mLoadTSESuiteConfig.PareseTSESuiteConfig(JGW_W2A(mstrTSEProjectSuiteXmlFilePath))) return false;
        mpTipLabel->SetText(mstrTSEProjectSuiteXmlFilePath.c_str());
        mpTipLabel->GetParent()->SetPos(mpTipLabel->GetParent()->GetPos());
        mpUpdateLabel->SetVisible(false);

        return true;
    }

    void CTSE_ConfigEditToolDlg::UpdateTSETreeView()
    {
        mvsTSETreeNodeData.clear();
        mpTreeView->RemoveAll();
        CTreeNodeUI* pTreeNodeUi = DefaultBuildTreeNodeUI();
        pTreeNodeUi->SetItemText(L"TSE");
        //! Environment
        {

            CTreeNodeUI* pSubTreeNodeUi = DefaultBuildTreeNodeUI();
            pSubTreeNodeUi->SetItemText(L"Environment");
            pSubTreeNodeUi->GetCheckBox()->SetCheck(true);
            pTreeNodeUi->AddChildNode(pSubTreeNodeUi);   

            std::shared_ptr<S_TSE_TREE_NODE_DATA> ptrsTSETreeNodeData(new S_TSE_TREE_NODE_DATA);
            ptrsTSETreeNodeData.get()->meTreeNodeUIType = E_ENVIRONMENT_TREE_TYPE;
            ptrsTSETreeNodeData.get()->mpEnvironmentConfig = &msTSESequenceConfigParam.m_mCommonTestParam;
            mvsTSETreeNodeData.push_back(ptrsTSETreeNodeData);
            pSubTreeNodeUi->SetDate(ptrsTSETreeNodeData.get());

            mpTreeView->Add(pTreeNodeUi);
            mpTreeView->SetItemExpand(true,pTreeNodeUi);
            //! pSubTreeNodeUi->GetCheckBox()->SetVisible(false);
        }
        //! SUITE 
        {
            //! ConfigView
            {
                CTreeNodeUI* pSubTreeNodeUi = DefaultBuildTreeNodeUI();
                pSubTreeNodeUi->SetItemText(L"ConfigView");
                pTreeNodeUi->AddChildNode(pSubTreeNodeUi);
                pSubTreeNodeUi->GetCheckBox()->SetCheck(true);

                AddSubTreeNode(msTSESequenceConfigParam.m_vConfigViewTestConfig,pSubTreeNodeUi,E_CONFIG_SUITE_TREE_TYPE);
            }
            //! Auto
            {
                CTreeNodeUI* pSubTreeNodeUi = DefaultBuildTreeNodeUI();
                pSubTreeNodeUi->SetItemText(L"Auto");
                pTreeNodeUi->AddChildNode(pSubTreeNodeUi);  
                pSubTreeNodeUi->GetCheckBox()->SetCheck(true);

                AddSubTreeNode(msTSESequenceConfigParam.m_vAutoTestConfig,pSubTreeNodeUi,E_AUTO_SUITE_TREE_TYPE);
            }
            //! suite
            {
                CTreeNodeUI* pSubTreeNodeUi = DefaultBuildTreeNodeUI();
                pSubTreeNodeUi->SetItemText(L"Project");
                pTreeNodeUi->AddChildNode(pSubTreeNodeUi);  
                pSubTreeNodeUi->GetCheckBox()->SetCheck(true);

                AddSubTreeNode(msTSESequenceConfigParam.m_vSuiteTestConfig,pSubTreeNodeUi,E_SUITE_TEST_TREE_TYPE);
            }
        }
    }

    CTreeNodeUI* CTSE_ConfigEditToolDlg::DefaultBuildTreeNodeUI()
    {
        CTreeNodeUI* pSubTreeNodeUi = new CTreeNodeUI;
        pSubTreeNodeUi->SetAttribute(L"folderattr",L"padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16' \" hotimage=\"file='treeview_b.png' source='36,0,72,16' \" selectedimage=\"file='treeview_a.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_a.png' source='36,0,72,16' \"");
        pSubTreeNodeUi->SetAttribute(L"checkboxattr",L"width=\"16\" height=\"16\" normalimage=\"file='unchecked.png' source='0,0,36,16' \" selectedimage=\"file='checked.png'\"");
        return pSubTreeNodeUi;
    }

    void CTSE_ConfigEditToolDlg::AddSubTreeNode(_vsuitetestconfig& vSuiteTestConfigParam,CTreeNodeUI* pTreeNodeUi,E_TREENODEUI_TYPE eTreeNodeUi)
    {
        bool bChecked = false;
        for (_vsuitetestconfig::iterator it = vSuiteTestConfigParam.begin();
            it != vSuiteTestConfigParam.end();
            ++ it)
        {
            std::vector<s_suite_single_item_test_config>::iterator itTestName = it[0]->find(L"TestName");
            if (itTestName == it[0]->m_mTestParam.end()) continue;

            CTreeNodeUI* pSubTreeNodeUi = DefaultBuildTreeNodeUI();
            pSubTreeNodeUi->SetItemText(itTestName->mParamValue.c_str());
            pSubTreeNodeUi->GetCheckBox()->SetCheck(it[0]->mbIsTest);
            pSubTreeNodeUi->SetDate(it[0].get());

            std::shared_ptr<S_TSE_TREE_NODE_DATA> ptrsTSETreeNodeData(new S_TSE_TREE_NODE_DATA);
            ptrsTSETreeNodeData.get()->meTreeNodeUIType = eTreeNodeUi;
            ptrsTSETreeNodeData.get()->mpsSuiteItemConfig = it[0].get();
            mvsTSETreeNodeData.push_back(ptrsTSETreeNodeData);
            pSubTreeNodeUi->SetDate(ptrsTSETreeNodeData.get());

            pSubTreeNodeUi->GetCheckBox()->OnNotify += MakeDelegate(this,&CTSE_ConfigEditToolDlg::OnClickCheckBox,DUI_MSGTYPE_SELECTCHANGED);
            pSubTreeNodeUi->GetCheckBox()->SetDate(ptrsTSETreeNodeData.get());
            //! pSubTreeNodeUi->SetVisibleFolderBtn(false);
            pSubTreeNodeUi->SetAttribute(L"folderattr",L"padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_c.png' source='0,0,36,16' \" hotimage=\"file='treeview_c.png' source='36,0,72,16' \" selectedimage=\"file='treeview_c.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_c.png' source='36,0,72,16' \"");

            if (it[0]->mbIsTest) bChecked = true;

            pTreeNodeUi->AddChildNode(pSubTreeNodeUi);
        }
        if (!bChecked) pTreeNodeUi->GetCheckBox()->SetCheck(false);
    }

    void CTSE_ConfigEditToolDlg::AddSubSigleTreeNode(std::shared_ptr<S_TSE_TREE_NODE_DATA> ptrTSETreeNode,CTreeNodeUI* pTreeNodeUi)
    {
        CTreeNodeUI* pSubTreeNodeUi = DefaultBuildTreeNodeUI();
        if (E_CONFIG_SUITE_TREE_TYPE == ptrTSETreeNode->meTreeNodeUIType
            || E_AUTO_SUITE_TREE_TYPE == ptrTSETreeNode->meTreeNodeUIType
            || E_SUITE_TEST_TREE_TYPE == ptrTSETreeNode->meTreeNodeUIType)
        {
            std::vector<s_suite_single_item_test_config>::iterator itTestName = ptrTSETreeNode->mpsSuiteItemConfig->find(L"TestName");
            if (itTestName == ptrTSETreeNode->mpsSuiteItemConfig->m_mTestParam.end()) return;
            pSubTreeNodeUi->SetItemText(itTestName->mParamValue.c_str());
            pSubTreeNodeUi->GetCheckBox()->SetCheck(ptrTSETreeNode->mpsSuiteItemConfig->mbIsTest);
        }
        pSubTreeNodeUi->SetDate(ptrTSETreeNode.get());
        pSubTreeNodeUi->GetCheckBox()->OnNotify += MakeDelegate(this,&CTSE_ConfigEditToolDlg::OnClickCheckBox,DUI_MSGTYPE_SELECTCHANGED);
        pSubTreeNodeUi->GetCheckBox()->SetDate(ptrTSETreeNode.get());
        pTreeNodeUi->GetParentNode()->AddChildNode(pSubTreeNodeUi);     
    }

    bool CTSE_ConfigEditToolDlg::OnClickCheckBox(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
    {
        S_TSE_TREE_NODE_DATA* psTSETreeNodeData = (S_TSE_TREE_NODE_DATA*)pTNotifyUI->pSender->GetDate();
        if (NULL == psTSETreeNodeData) return true;
        if (E_CONFIG_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_AUTO_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_SUITE_TEST_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            psTSETreeNodeData->mpsSuiteItemConfig->mbIsTest = ((CCheckBoxUI*)pTNotifyUI->pSender)->GetCheck();
            SetElementAttributeValue(psTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode,"IsTest",psTSETreeNodeData->mpsSuiteItemConfig->mbIsTest?L"1":L"0");
            mpUpdateLabel->SetVisible(true);
        }
        return true;
    }

    void CTSE_ConfigEditToolDlg::UpdateTSEListView(LPS_TSE_TREE_NODE_DATA psTSETreeNodeData)
    {
        mpSuiteList->GetHeader()->RemoveAll();
        mpSuiteList->GetList()->RemoveAll();
        mpSuiteList->SetDate(psTSETreeNodeData);
        if (!psTSETreeNodeData) return ;


        if (E_CONFIG_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_AUTO_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_SUITE_TEST_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            UpdateTSTListSuiteItem(psTSETreeNodeData->mpsSuiteItemConfig);
        }
        else if (E_ENVIRONMENT_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            UpdateTSTListEnvironment(psTSETreeNodeData->mpEnvironmentConfig);
        }
    }

    void CTSE_ConfigEditToolDlg::UpdateTSTListSuiteItem(s_suite_item_test_config* psSuiteItemTestConfig)
    {
        mpSuiteList->InsertColumn(0,_T("Param Name"),0);
        mpSuiteList->InsertColumn(1,_T("Param Value"),0);
        mpSuiteList->InsertColumn(2,_T("Description"),0);

        CListContainerElementUIEx* lplsitContainerItem = NULL;
        CEditUI* lpEditui = NULL;
        std::wstring strTemp = L"",strChannel,strLoss;
        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;

        for (std::vector<s_suite_single_item_test_config>::iterator it = psSuiteItemTestConfig->m_mTestParam.begin();
            it != psSuiteItemTestConfig->m_mTestParam.end();
            ++ it)
        {
            CDialogBuilder builder1;
            lplsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(_T("edit_suite_list.xml"),(UINT)0,NULL,&m_PaintManager));
            if( !lplsitContainerItem )continue;
            if(lplsitContainerItem != NULL) mpSuiteList->InsertItem(mpSuiteList->GetCount(), 24, lplsitContainerItem);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(0));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(0)->SetText(it->mParamName.c_str());

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
            if( !pHorizontalLayoutUi )continue;
            CEditUI* pTempEdt = (CEditUI*)pHorizontalLayoutUi->GetItemAt(0);
            pTempEdt->SetText(it->mParamValue.c_str());
            if (E_PARAM_INT == it->meParamType)
            {
                //! ^[0-9]*$
                pTempEdt->SetRegularCheck(L"^[0-9]*$");
                pTempEdt->SetToolTip(L"Input Type: int");
            }


            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(2));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(0)->SetText(it->mParamDescription.c_str());
        }
    }

    void CTSE_ConfigEditToolDlg::UpdateTSTListEnvironment(_mcommontestconfig* psEnvironmentConfig)
    {
        mpSuiteList->InsertColumn(0,_T("name"),0);
        mpSuiteList->InsertColumn(1,_T("value"),0);

        CListContainerElementUIEx* lplsitContainerItem = NULL;
        CEditUI* lpEditui = NULL;
        std::wstring strTemp = L"",strChannel,strLoss;
        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;

        for (auto it = psEnvironmentConfig->mEnvironment.begin();
            it != psEnvironmentConfig->mEnvironment.end();
            ++ it)
        {
            CDialogBuilder builder1;
            lplsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(_T("edit_env_list.xml"),(UINT)0,NULL,&m_PaintManager));
            if( !lplsitContainerItem )continue; 
            if(lplsitContainerItem != NULL) mpSuiteList->InsertItem(mpSuiteList->GetList()->GetCount(), 24, lplsitContainerItem);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(0));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(0)->SetText(it->first.c_str());

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(0)->SetText(it->second.c_str());
        }
    }

    void CTSE_ConfigEditToolDlg::SaveTSEListMemroyConfig()
    {
        if (0 == mpSuiteList->GetCount()) return ;
        LPS_TSE_TREE_NODE_DATA psTSETreeNodeData = (LPS_TSE_TREE_NODE_DATA)mpSuiteList->GetDate();
        if (E_SUITE_TEST_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_CONFIG_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_AUTO_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            SaveTSTListSuiteItemMemroyConfig(psTSETreeNodeData->mpsSuiteItemConfig);
        }
        else if (E_ENVIRONMENT_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            SaveTSTListEnvironmentMemroyConfig(psTSETreeNodeData->mpEnvironmentConfig);
        }
    }

    void CTSE_ConfigEditToolDlg::SaveTSTListSuiteItemMemroyConfig(s_suite_item_test_config* psSuiteItemTestConfig)
    {
        if (!mbReSaveTestConfig) return ;
        for (int i = 0;i < mpSuiteList->GetCount();i ++)
        {
            CHorizontalLayoutUI* pHorizontalLayoutUi = (CHorizontalLayoutUI*)(mpSuiteList->GetItemAt(i));
            psSuiteItemTestConfig->m_mTestParam[i].mParamName = ((CHorizontalLayoutUI*)pHorizontalLayoutUi->GetItemAt(0))->GetItemAt(0)->GetText().GetData();
            psSuiteItemTestConfig->m_mTestParam[i].mParamValue = ((CHorizontalLayoutUI*)pHorizontalLayoutUi->GetItemAt(1))->GetItemAt(0)->GetText().GetData();
            psSuiteItemTestConfig->m_mTestParam[i].mParamDescription = ((CHorizontalLayoutUI*)pHorizontalLayoutUi->GetItemAt(2))->GetItemAt(0)->GetText().GetData();
        }

        for (int i = 0;i < mpSuiteList->GetCount();i ++)
        {
            TiXmlElement* pTempNode = const_cast< TiXmlElement* >(psSuiteItemTestConfig->mpSuiteXmlNode->FirstChildElement(JGW_W2A(psSuiteItemTestConfig->m_mTestParam[i].mParamName).c_str()));
            if (!pTempNode) continue;
            SetElementAttributeValue(pTempNode,"paramVal",psSuiteItemTestConfig->m_mTestParam[i].mParamValue);
            SetElementAttributeValue(pTempNode,"description",psSuiteItemTestConfig->m_mTestParam[i].mParamDescription);
        }
    }

    void CTSE_ConfigEditToolDlg::SaveTSTListEnvironmentMemroyConfig(_mcommontestconfig* psEnvironmentConfig)
    {
        if (!mbReSaveTestConfig) return ;
        for (int i = 0;i < mpSuiteList->GetCount();i ++)
        {
            CHorizontalLayoutUI* pHorizontalLayoutUi = (CHorizontalLayoutUI*)(mpSuiteList->GetItemAt(i));
            psEnvironmentConfig->mEnvironment[((CHorizontalLayoutUI*)pHorizontalLayoutUi->GetItemAt(0))->GetItemAt(0)->GetText().GetData()] =  ((CHorizontalLayoutUI*)pHorizontalLayoutUi->GetItemAt(1))->GetItemAt(0)->GetText().GetData();
            SetElementTextValue(const_cast<TiXmlElement*>(psEnvironmentConfig->mpCommonXmlNode),JGW_W2A_W(((CHorizontalLayoutUI*)pHorizontalLayoutUi->GetItemAt(0))->GetItemAt(0)->GetText().GetData()).c_str(),((CHorizontalLayoutUI*)pHorizontalLayoutUi->GetItemAt(1))->GetItemAt(0)->GetText().GetData());
        }
        //         for (int i = 0;i < mpSuiteList->GetCount();i ++)
        //         {
        //             
        // //             TiXmlElement* pTempNode = const_cast< TiXmlElement* >(psEnvironmentConfig->mpCommonXmlNode->FirstChildElement(JGW_W2A(psSuiteItemTestConfig->m_mTestParam[i].mParamName).c_str()));
        // //             if (!pTempNode) continue;
        // //             SetElementAttributeValue(pTempNode,"paramVal",psSuiteItemTestConfig->m_mTestParam[i].mParamValue);
        // //             SetElementAttributeValue(pTempNode,"description",psSuiteItemTestConfig->m_mTestParam[i].mParamDescription);
        //         }
    }
}