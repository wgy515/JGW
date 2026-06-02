#include "StdAfx.h"
#include "TSE_ConfigEditToolDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_LogWindowsView.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_Clipboard.h>
namespace JGW
{
    CTSE_ConfigEditToolController::CTSE_ConfigEditToolController(CTSE_ConfigEditToolDlg* pView) : mpView(pView),mbClickProjectMenu(false)
    {
    }


    CTSE_ConfigEditToolController::~CTSE_ConfigEditToolController(void)
    {
    }

    void CTSE_ConfigEditToolController::OnViewNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT && msg.pSender == mpView->mpTreeView)
        {
            CTreeNodeUI* pTreeNodeUi =  (CTreeNodeUI*)mpView->mpTreeView->GetItemAt(msg.wParam);
            LPS_TSE_TREE_NODE_DATA psTSETreeNodeData = (LPS_TSE_TREE_NODE_DATA)pTreeNodeUi->GetDate();
            mpView->SaveTSEListMemroyConfig();
            mpView->UpdateTSEListView(psTSETreeNodeData);
            mpView->mbReSaveTestConfig = false;
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"EditMenuBtn")
            {
                OnEditMenuBtnClick(msg);
            }
            else if (msg.pSender->GetName() == L"ProjectMenuBtn")
            {
                OnProjectMenuBtnClick(msg);
            }
            else if (msg.pSender->GetName() == L"TestMenuBtn")
            {
                OnTestMenuBtnClick(msg);
            }
            else if (msg.pSender->GetName() == L"LogMenuBtn")
            {
                OnLogMenuBtnClick(msg);
            }
            else if (msg.pSender->GetName() == L"AddSuiteBtn")
            {
                OnAddSuiteBtnClick(msg);
            }
            else if (msg.pSender->GetName() == L"DelSuiteBtn")
            {
                OnDelSuiteBtnClick(msg);
            }
            else if (msg.pSender->GetName() == L"maximizeLogLayoutBtn")
            {
                OnMaximizeLogLayoutBtnClick(msg);
            }
        }
        else if (msg.sType == DUI_MSGTYPE_EDITREGEX)
        {
            if (msg.wParam == IDNO)
            {
                msg.pSender->SetBorderColor(0xFF0000);
            }
            else
            {
                msg.pSender->SetBorderColor(0xFFBAC0C5);
            }
        }
        else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
        {
            mpView->mpUpdateLabel->SetVisible(true);
            mpView->mbReSaveTestConfig = true;
        }
    }

    bool CTSE_ConfigEditToolController::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_MENUCLICK: //! 响应菜单栏点击消息
            OnMenuElementClick(pTEventUI);    
            mbClickProjectMenu = false;
            break;
        }
        return true;
    }

    LRESULT CTSE_ConfigEditToolController::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) 
    {
        if(!(::GetAsyncKeyState(VK_CONTROL) & 0x8000)) return FALSE;
        switch (wParam)
        {
        case 's':
        case 'S':
            if (::GetAsyncKeyState(VK_MENU)& 0x8000)
            {
                OnSaveAsMenuElementClick();
            }
            else
            {
                OnSaveMenuElementClick();
            }
            break;
        case 'o':
        case 'O':
            OnOpenMenuElementClick();
            break;
        case 'n':
        case 'N':
            OnNewMenuElementClick();
            break;
        case 'c':
        case 'C':
            OnCopyShortcutClick();
            break;
        case 'v':
        case 'V':
            OnPasteShortcutClick();
            break;
        }
        return FALSE;
#if 0
        static UINT mod = 0;
        if (uMsg == WM_KEYDOWN)
        {
            if (wParam == VK_CONTROL)
            {
                mod |= MOD_CONTROL;
            }
            else if (wParam == VK_SHIFT)
            {
                mod |= MOD_SHIFT;
            }
            else if (wParam == VK_MENU)
            {
                mod |= MOD_ALT;
            }
            else if (mod & MOD_CONTROL)
            {
                switch (wParam)
                {
                case 's':
                case 'S':
                    if (mod & MOD_ALT)
                    {
                        MessageBox(NULL,L"另存为",NULL,NULL);
                    }
                    else
                    {
                        MessageBox(NULL,L"保存",NULL,NULL);
                    }
                    break;
                case 'o':
                case 'O':
                    MessageBox(NULL,L"打开",NULL,NULL);
                    break;
                case 'n':
                case 'N':
                    MessageBox(NULL,L"新建",NULL,NULL);
                    break;
                }
            }
        }
        else if (uMsg == WM_KEYUP)
        {
            if (wParam == VK_CONTROL)
            {
                mod &= (~MOD_CONTROL);
            }
            else if (wParam == VK_SHIFT)
            {
                mod &= (~MOD_SHIFT);
            }
            else if (wParam == VK_MENU)
            {
                mod &= (~MOD_ALT);
            }
            return 0;
        }
#endif
    }

    void CTSE_ConfigEditToolController::OnMenuElementClick(TEventUI* pTEventUI)
    {
        std::wstring strName = (const wchar_t*)pTEventUI->wParam;
        delete[] (const wchar_t*)pTEventUI->wParam;

        if (mbClickProjectMenu)
        {
            mpView->mstrSelectModuleFolder = strName;
            mpView->mstrTSEProjectSuiteXmlFilePath = JGW_GetTSEConfigFilePath(strName,L"");
            mpView->LoadTSEProjectUI();
            mpView->UpdateTSETreeView();
            return ;
        }

        if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("showLogLayoutMenuElement")))
        {
            OnResponceViewLogInfoMsg(mpView->mpLogHorizontalLayout->IsVisible()?0:1,NULL);
        } 
        else if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("newMenuElement")))
        {
            OnNewMenuElementClick();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("openMenuElement")))
        {
            OnOpenMenuElementClick();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("saveMenuElement")))
        {
            OnSaveMenuElementClick();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("saveAsMenuElement")))
        {
            OnSaveAsMenuElementClick();
        }
    }

    void CTSE_ConfigEditToolController::OnEditMenuBtnClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(mpView->m_hWnd, &point);
        pMenu->Init(NULL,_T("edit_menu.xml"),point,&mpView->m_PaintManager,&mpView->mpMenuCheckInfo);
    }

    void CTSE_ConfigEditToolController::OnProjectMenuBtnClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(mpView->m_hWnd, &point);
        pMenu->Init(NULL,_T("project_menu.xml"),point,&mpView->m_PaintManager,&mpView->mpMenuCheckInfo);
        OnUpdateProjectMenuUIInfo(pMenu);
        mbClickProjectMenu = true;
    }

    void CTSE_ConfigEditToolController::OnTestMenuBtnClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(mpView->m_hWnd, &point);
        pMenu->Init(NULL,_T("test_menu.xml"),point,&mpView->m_PaintManager,&mpView->mpMenuCheckInfo);
    }

    void CTSE_ConfigEditToolController::OnLogMenuBtnClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(mpView->m_hWnd, &point);
        pMenu->Init(NULL,_T("log_menu.xml"),point,&mpView->m_PaintManager,&mpView->mpMenuCheckInfo);
    }

    void CTSE_ConfigEditToolController::OnAddSuiteBtnClick(TNotifyUI& msg)
    {
        OnCopyShortcutClick();
        OnPasteShortcutClick();
    }

    void CTSE_ConfigEditToolController::OnDelSuiteBtnClick(TNotifyUI& msg)
    {
        int nCurSel = mpView->mpTreeView->GetCurSel();
        CTreeNodeUI* pTreeNodeUi =  (CTreeNodeUI*)mpView->mpTreeView->GetItemAt(nCurSel);
        LPS_TSE_TREE_NODE_DATA psTSETreeNodeData = (LPS_TSE_TREE_NODE_DATA)pTreeNodeUi->GetDate();
        if (NULL == psTSETreeNodeData) return ;

        if (E_CONFIG_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_AUTO_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_SUITE_TEST_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            mpView->mLoadTSESuiteConfig.mptrXmlDoc->RootElement()->RemoveChild(psTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode);
            mpView->mpTreeView->Remove(pTreeNodeUi);
            mpView->mbReSaveTestConfig = true;
        }
    }

    void CTSE_ConfigEditToolController::OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam)
    {
        //! 隐藏log显示信息
        if (0 == (int)wParam)
        {
            //! 已经隐藏
            if (!mpView->mpLogHorizontalLayout->IsVisible()) return;
            mpView->mpLogHorizontalLayout->SetVisible(false);
            mpView->mpMenuCheckInfo[L"showLogLayoutMenuElement"] = false;
        }
        else
        {
            COptionUI* pError =  (COptionUI*)mpView->m_PaintManager.FindControl(L"errorTab");
            COptionUI* debugTab =  (COptionUI*)mpView->m_PaintManager.FindControl(L"debugTab");
            debugTab->Selected(true);
            pError->Selected(true);
            mpView->mpLogHorizontalLayout->SetVisible(true);
            mpView->mpMenuCheckInfo[L"showLogLayoutMenuElement"] = true;
        }
    }

    void CTSE_ConfigEditToolController::OnMaximizeLogLayoutBtnClick(TNotifyUI& msg)
    {
        OnResponceViewLogInfoMsg(NULL,NULL);
        CCJGW_LogWindowsView logWindowsView;
        logWindowsView.DoModal();
    }


    void CTSE_ConfigEditToolController::OnSaveAsMenuElementClick()
    {
        std::wstring strSaveFilePath = L"suite.xml";
        if (!JGW_SaveFileName(strSaveFilePath,L"xml File(*.xml)\0*.xml\0all file(*.*)\0*.*\0\0",mpView->m_hWnd,L"xml"))
        {
            return ;
        }
        mpView->mLoadTSESuiteConfig.SaveAsTSESuiteConfig(JGW_W2A(strSaveFilePath));
        mpView->mpUpdateLabel->SetVisible(false);
    }

    void CTSE_ConfigEditToolController::OnSaveMenuElementClick()
    {
        mpView->SaveTSEListMemroyConfig();
        mpView->mLoadTSESuiteConfig.SaveTSESuiteConfig();
        mpView->mpUpdateLabel->SetVisible(false);
    }

    void CTSE_ConfigEditToolController::OnOpenMenuElementClick()
    {
        std::wstring strOpenFilePath;
        if (!JGW_ChooseFile(strOpenFilePath,L"xml File(*.xml)\0*.xml\0all file(*.*)\0*.*\0\0",mpView->m_hWnd))
        {
            return ;
        }
        mpView->mstrTSEProjectSuiteXmlFilePath = strOpenFilePath;
        mpView->LoadTSEProjectUI();
        mpView->UpdateTSETreeView();
    }

    void CTSE_ConfigEditToolController::OnNewMenuElementClick()
    {
        std::wstring strNewFilePath = L"suite.xml";
        if (!JGW_SaveFileName(strNewFilePath,L"xml File(*.xml)\0*.xml\0all file(*.*)\0*.*\0\0",mpView->m_hWnd,L"xml"))  return ;
        TiXmlDocument tiXmlDoc;
        tiXmlDoc.SaveFile(JGW_W2A(strNewFilePath));
        mpView->mstrTSEProjectSuiteXmlFilePath = strNewFilePath;
        mpView->LoadTSEProjectUI();
        mpView->UpdateTSETreeView();
    }

    void CTSE_ConfigEditToolController::OnUpdateProjectMenuUIInfo(CMenuWnd* pMenu)
    {
        //! project -> group -> plugin
        CMenuUI* rootMenu = pMenu->GetMenuUI();
        for (auto itProject = mpView->mpvsProjectGroupPlnInfo->begin();
            itProject != mpView->mpvsProjectGroupPlnInfo->end();
            ++ itProject)
        {
            CMenuElementUI* pProjectMenu = new CMenuElementUI;
            pProjectMenu->SetText(itProject->mstrProjectName.c_str());
            pProjectMenu->SetName(itProject->mstrProjectMenuName.c_str());
            mpView->mpMenuCheckInfo[pProjectMenu->GetName()] = false;
            pProjectMenu->SetShowExplandIcon(true);
            pProjectMenu->SetIcon(_T("right.png"));
            pProjectMenu->SetIconSize(9,9);
            pProjectMenu->SetCheckItem(true);

            for (auto itGroup = itProject->mvAllPluginInfo.begin();
                itGroup != itProject->mvAllPluginInfo.end();
                ++ itGroup)
            {
                CMenuElementUI* pGroupMenu = new CMenuElementUI;
                pGroupMenu->SetText(itGroup->mstrGroupName.c_str());
                pGroupMenu->SetName(itGroup->mstrGroupName.c_str());
                mpView->mpMenuCheckInfo[pGroupMenu->GetName()] = false;
                pGroupMenu->SetShowExplandIcon(true);
                pGroupMenu->SetIcon(_T("right.png"));
                pGroupMenu->SetIconSize(9,9);
                pGroupMenu->SetCheckItem(true);
                //! && 
                for (auto itPln = itGroup->mvSubWinInfo.begin();
                    itPln != itGroup->mvSubWinInfo.end();
                    ++ itPln)
                {
                    CMenuElementUI* pPlnMenu = new CMenuElementUI;
                    pPlnMenu->SetText(itPln->mstrStationName.c_str());
                    pPlnMenu->SetName(itPln->mstrModuleFolderName.c_str());
                    pPlnMenu->SetIcon(_T("right.png"));
                    pPlnMenu->SetIconSize(9,9);
                    pPlnMenu->SetCheckItem(true);
                    mpView->mpMenuCheckInfo[pPlnMenu->GetName()] = false;

                    if (NULL == JGW_WStrComparenoCaseWStr(mpView->mstrSelectModuleFolder.c_str(),itPln->mstrModuleFolderName.c_str()))
                    {
                        mpView->mpMenuCheckInfo[pProjectMenu->GetName()] = true;
                        mpView->mpMenuCheckInfo[pGroupMenu->GetName()] = true;
                        mpView->mpMenuCheckInfo[pPlnMenu->GetName()] = true;
                    }

                    if (!JGW_FileExistsToFilePath(JGW_GetTSEConfigFilePath(itPln->mstrModuleFolderName,itPln->mstrSuiteXmlName).c_str()))
                        pPlnMenu->SetEnabled(false);
                    pGroupMenu->Add(pPlnMenu);
                }
                pProjectMenu->Add(pGroupMenu);
            }
            rootMenu->Add(pProjectMenu);
        }
        pMenu->ResizeMenu();
    }
    //! 分隔符  {{{  }}}{{{  }}}{{{  }}}{{{  }}}{{{  }}}{{{  }}}{{{  }}}{{{  }}}{{{  }}}
    void CTSE_ConfigEditToolController::OnCopyShortcutClick()
    {
        int nCurSel = mpView->mpTreeView->GetCurSel();
        CTreeNodeUI* pTreeNodeUi =  (CTreeNodeUI*)mpView->mpTreeView->GetItemAt(nCurSel);
        LPS_TSE_TREE_NODE_DATA psTSETreeNodeData = (LPS_TSE_TREE_NODE_DATA)pTreeNodeUi->GetDate();
        if (NULL == psTSETreeNodeData) return ;
        //! JSON
        std::wstring strCopy;
        std::wstring strTemp;
        JGW_FormatWString(strCopy,L"{{{096019DD-6BC9-4633-B765-A02B31E11382{{{%d",psTSETreeNodeData->meTreeNodeUIType);
        if (E_CONFIG_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_AUTO_SUITE_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType
            || E_SUITE_TEST_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            for (auto it = psTSETreeNodeData->mpsSuiteItemConfig->m_mTestParam.begin();
                it != psTSETreeNodeData->mpsSuiteItemConfig->m_mTestParam.end();
                ++ it
                )
            {
                JGW_FormatWString(strTemp,L"{{{%s\t%s\t%s",it->mParamName.empty()?L" ":it->mParamName.c_str(),it->mParamValue.empty()?L" ":it->mParamValue.c_str(),it->mParamDescription.empty()?L" ":it->mParamDescription.c_str());
                strCopy += strTemp;
            }
        }
        else if (E_ENVIRONMENT_TREE_TYPE == psTSETreeNodeData->meTreeNodeUIType)
        {
            for (auto it = psTSETreeNodeData->mpEnvironmentConfig->mEnvironment.begin();
                it != psTSETreeNodeData->mpEnvironmentConfig->mEnvironment.end();
                ++ it
                )
            {
                JGW_FormatWString(strTemp,L"{{{%s\t%s",it->first.c_str(),it->second.c_str());
                strCopy += strTemp;
            }
        }
        CCJGW_Clipboard::SetClipBoardText(strCopy.c_str(),mpView->m_hWnd);
    }

    void CTSE_ConfigEditToolController::OnPasteShortcutClick()
    {
        int nCurSel = mpView->mpTreeView->GetCurSel();
        CTreeNodeUI* pTreeNodeUi =  (CTreeNodeUI*)mpView->mpTreeView->GetItemAt(nCurSel);
        LPS_TSE_TREE_NODE_DATA psTSETreeNodeData = (LPS_TSE_TREE_NODE_DATA)pTreeNodeUi->GetDate();
        if (NULL == psTSETreeNodeData) return ;

        std::vector<std::wstring> strVec;
        std::wstring strClipBoardText;
        CCJGW_Clipboard::GetClipBoardText(mpView->m_hWnd,strClipBoardText);
        JGW_ParserStrW(strClipBoardText.c_str(),L"{{{",strVec);
        if (strVec.size() < 2 || NULL != strVec[0].compare(L"096019DD-6BC9-4633-B765-A02B31E11382")) return;
        std::shared_ptr<S_TSE_TREE_NODE_DATA> ptrsTSETreeNodeData(new S_TSE_TREE_NODE_DATA);
        ptrsTSETreeNodeData->meTreeNodeUIType = (E_TREENODEUI_TYPE)_ttoi(strVec[1].c_str());

        if (E_CONFIG_SUITE_TREE_TYPE == ptrsTSETreeNodeData->meTreeNodeUIType
            || E_AUTO_SUITE_TREE_TYPE == ptrsTSETreeNodeData->meTreeNodeUIType
            || E_SUITE_TEST_TREE_TYPE == ptrsTSETreeNodeData->meTreeNodeUIType)
        {
            mpView->mvsTSETreeNodeData.push_back(ptrsTSETreeNodeData);
            std::shared_ptr<s_suite_item_test_config> ptrSuiteItem(new s_suite_item_test_config);
            s_suite_single_item_test_config sSuiteSingeItemTestConfig;
            ptrSuiteItem->mbIsTest = true;
            mpView->msTSESequenceConfigParam.m_vSuiteTestConfig.push_back(ptrSuiteItem);
            ptrsTSETreeNodeData->mpsSuiteItemConfig = ptrSuiteItem.get();
            //! TiXmlElement* xmlElement = new TiXmlElement("SuiteProject");
            if (E_SUITE_TEST_TREE_TYPE == ptrsTSETreeNodeData->meTreeNodeUIType)
                ptrsTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode = new TiXmlElement("SuiteProject");
            else if (E_AUTO_SUITE_TREE_TYPE == ptrsTSETreeNodeData->meTreeNodeUIType)
                ptrsTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode = new TiXmlElement("auto");
            else
                ptrsTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode = new TiXmlElement("config");

            for (size_t i = 2;i < strVec.size();i ++)
            {
                std::vector<std::wstring> vstrTemp;
                JGW_ParserStrW(strVec[i].c_str(),L"\t",vstrTemp);
                sSuiteSingeItemTestConfig.mParamName = vstrTemp[0];
                TiXmlElement* pXmlTemp = new TiXmlElement(JGW_W2A(sSuiteSingeItemTestConfig.mParamName));
                SetElementAttributeValue(pXmlTemp,"paramVal",vstrTemp[1]);
                sSuiteSingeItemTestConfig.mParamValue = vstrTemp[1];
                sSuiteSingeItemTestConfig.mParamDescription = vstrTemp[2];
                SetElementAttributeValue(pXmlTemp,"description",vstrTemp[2]);
                ptrSuiteItem->m_mTestParam.push_back(sSuiteSingeItemTestConfig);
                ptrsTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode->LinkEndChild(pXmlTemp);
            }
            mpView->mbReSaveTestConfig = true;
            mpView->AddSubSigleTreeNode(ptrsTSETreeNodeData,pTreeNodeUi);

            mpView->mLoadTSESuiteConfig.mptrXmlDoc->RootElement()->LinkEndChild(ptrsTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode);
            //! psTSETreeNodeData->mpsSuiteItemConfig->mpSuiteXmlNode->InsertAfterChild();
        }
        else if (E_ENVIRONMENT_TREE_TYPE == ptrsTSETreeNodeData->meTreeNodeUIType)
        {
            for (size_t i = 2;i < strVec.size();i ++)
            {
                std::vector<std::wstring> vstrTemp;
                JGW_ParserStrW(strVec[i].c_str(),L"\t",vstrTemp);
                mpView->msTSESequenceConfigParam.m_mCommonTestParam.mEnvironment[vstrTemp[0]] = vstrTemp[1];
                //! psTSETreeNodeData->mpEnvironmentConfig->mpCommonXmlNode;
                SetElementTextValue(psTSETreeNodeData->mpEnvironmentConfig->mpCommonXmlNode,JGW_W2A(vstrTemp[0]).c_str(),vstrTemp[1]);
            }
            mpView->mbReSaveTestConfig = true;
            mpView->UpdateTSEListView(psTSETreeNodeData);
        }
    }
}