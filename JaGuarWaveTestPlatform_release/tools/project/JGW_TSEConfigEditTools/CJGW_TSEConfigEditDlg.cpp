#include "StdAfx.h"
#include "CJGW_TSEConfigEditDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <sstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <JGW_WindowsFuncPlugin/CJGW_Clipboard.h>

// multi folderattr
#define TREEVIEW_MULTI_FOLEDER_ATTR _T("padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16' \" hotimage=\"file='treeview_b.png' source='36,0,72,16' \" selectedimage=\"file='treeview_a.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_a.png' source='36,0,72,16' \"")
// single folderattr
#define TREEVIEW_SINGLE_FOLEDER_ATTR _T("padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_d.png' source='0,0,36,16' \" hotimage=\"file='treeview_d.png' source='36,0,72,16' \" selectedimage=\"file='treeview_d.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_d.png' source='36,0,72,16' \"")
// multi  file attr 
#define TREEVIEW_MULTI_FILE_ATTR _T("padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_e.png' source='0,0,36,16' \" hotimage=\"file='treeview_e.png' source='36,0,72,16' \" selectedimage=\"file='treeview_f.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_f.png' source='36,0,72,16' \"")
// file attr 
#define TREEVIEW_FILE_ATTR _T("padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_c.png' source='0,0,36,16' \" hotimage=\"file='treeview_c.png' source='36,0,72,16' \" selectedimage=\"file='treeview_c.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_c.png' source='36,0,72,16' \"")
// checkboxattr
#define TREEVIEW_CHECKBOX_ATTR _T("width=\"16\" height=\"16\" normalimage=\"file='unchecked.png' source='0,0,36,16' \" selectedimage=\"file='checked.png'\"")
// part_checked.png
#define TREEVIEW_PART_CHECKBOX_ATTR _T("width=\"16\" height=\"16\" normalimage=\"file='unchecked.png' source='0,0,36,16' \" selectedimage=\"file='part_checked.png'\"")

#define LIST_HEIGHT_NORMAL "32"
#define LIST_HEIGHT_FOCUS "96"

namespace JGW
{
    std::wstring gstrTSEPlugin[] = {
        L"CTSEAPSMultiUpgradePlugin",
        L"CPEMultiDownloadPlugin",
        L"CRFCalVerTestPlugin",
        L"CTSEMultiunitDownload",
        L"CTSEAutomationPlugin",
        L"CTSECalConfigPlugin",
        L"CTSEMultipleTestPlugin",
        L"CTSETestPlugin",
        L"CTSEThroughTestPlugin",
        L"CTSEUiPlugin"
    };

    typedef struct  
    {
        E_CHECKBOX_SELECTED_STATUS meCurrentCheckBoxStatus;
        E_CHECKBOX_SELECTED_STATUS meUpdateCheckBoxStatus;
        CTreeNodeUI* mpCurrentTreeNodeUI;
    }S_CHECKBOX_DATA,*PS_CHECKBOX_DATA;

    CCJGW_TSEConfigEditDlg::CCJGW_TSEConfigEditDlg(void) : mbIsRspCheckBoxChanged(true),mpHomeTreeNode(NULL),mbIsInitTSEListCompleted(false),mbIsSaveTSEConfig(false),mpPluinSuiteTree(NULL),mpSelectTreeNodeUI(NULL)
    {
        msTreeNodeData.mbIsTSETest = false;
        msTreeNodeData.mmapGlobalVariables = NULL;
        msTreeNodeData.mpsItemTestConfig = NULL;
        msTreeNodeData.mpPrevTreeNodeUI = NULL;
    }


    CCJGW_TSEConfigEditDlg::~CCJGW_TSEConfigEditDlg(void)
    {
    }

    void CCJGW_TSEConfigEditDlg::OnInitWindow()
    {
        m_PaintManager.AddMessageFilter(this);
        m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_TSEConfigEditDlg::OnEventCustomMessage,0);
        // 加载控制UI视图
        LoadControlUiClassPtr();
        // 加载主框架皮肤文件配置
        InitMainFrameSkinConfig();
        //mpPluinSuiteTree->OnEvent += MakeDelegate(this,&CCJGW_TSEConfigEditDlg::OnSuiteTreeMessage,0);
    }

    bool CCJGW_TSEConfigEditDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_MENUCLICK: //! 响应菜单栏点击消息
            OnMenuElementClick(pTEventUI);    
            break;
        case WM_RBUTTONUP:
            POINT pt = { GET_X_LPARAM(pTEventUI->lParam), GET_Y_LPARAM(pTEventUI->lParam) };
            CControlUI* pControl = m_PaintManager.FindSubControlByPoint(mpPluinSuiteTree,pt);//HorizontalLayoutUI
            if (pControl != NULL && NULL != pControl->GetParent() && CDuiString(pControl->GetParent()->GetClass()) == L"TreeNodeUI")
            {
                CTreeNodeUI* pParentControl = (CTreeNodeUI*)pControl->GetParent();
                mpSelectTreeNodeUI = pParentControl;
                // 菜单栏
                CMenuWnd* pMenu = new CMenuWnd();
                CPoint point(0,0);
                GetCursorPos(&point);
                pMenu->Init(NULL, _T("treeview_menu.xml"), point, &m_PaintManager, NULL,eMenuAlignment_Left | eMenuAlignment_Top );
            }
            break;;
        }
        return true;
    }

    //bool CCJGW_TSEConfigEditDlg::OnSuiteTreeMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    //{
    //    if (pTEventUI->Type == UIEVENT_RBUTTONUP)
    //    {
    //        if (pTEventUI->pSender == mpPluinSuiteTree)
    //        {
    //            return false;
    //        }
    //    }
    //    return true;
    //}

    //!
    template <typename T>
    bool GetJsonValueToKey(boost::property_tree::ptree& jsonTree,const std::string& strKey,T& strValue)
    {
        try
        {
            strValue = jsonTree.get<T>(strKey);
            return true;
        }
        catch (boost::property_tree::ptree_bad_path&)
        {

        }
        catch (boost::property_tree::ptree_bad_data&)
        {

        }
        return false;
    }

    std::wstring GetJsonWStringValueToKey(boost::property_tree::ptree& jsonTree,const std::string& strKey)
    {
        std::string strValue;
        GetJsonValueToKey(jsonTree,strKey,strValue);
        return JGW_A2W(strValue,CP_UTF8);
    }

    void ParseSingleItemTestConfigToJson(boost::property_tree::ptree& itemJson,std::shared_ptr<s_suite_test_config>& ptrsSuiteTestConfig)
    {
        GetJsonValueToKey(itemJson,"IsTest",ptrsSuiteTestConfig->msItemTestConfig.mbIsTest);
        GetJsonValueToKey(itemJson,"FinalizeTest",ptrsSuiteTestConfig->msItemTestConfig.mbFinalizeTest);
        GetJsonValueToKey(itemJson,"IfFalse",ptrsSuiteTestConfig->msItemTestConfig.mbIfFalse);
        GetJsonValueToKey(itemJson,"IsFolderTest",ptrsSuiteTestConfig->msItemTestConfig.mbIsFolderTest);
        GetJsonValueToKey(itemJson,"ReversedResult",ptrsSuiteTestConfig->msItemTestConfig.mbReversedResult);
        ptrsSuiteTestConfig->msItemTestConfig.mstrTestName = GetJsonWStringValueToKey(itemJson,"TestName");

        if(itemJson.count("Params"))
        {
            boost::property_tree::ptree ptChildRead = itemJson.get_child("Params");
            for(BOOST_AUTO(pos, ptChildRead.begin()); pos != ptChildRead.end(); ++pos)
            {
                s_tse_test_param sTSETestParam = {E_PARAM_STRING,GetJsonWStringValueToKey(pos->second,"name"),GetJsonWStringValueToKey(pos->second,"value"),GetJsonWStringValueToKey(pos->second,"des")};
                ptrsSuiteTestConfig->msItemTestConfig.mvTestConfigs.push_back(sTSETestParam);
            }
        }

        if (itemJson.count("Childs"))
        {
            boost::property_tree::ptree ptChildRead = itemJson.get_child("Childs");
            for(BOOST_AUTO(pos, ptChildRead.begin()); pos != ptChildRead.end(); ++pos)
            {
                std::shared_ptr<s_suite_test_config> ptrSuiteTestConfig(new s_suite_test_config);
                ParseSingleItemTestConfigToJson(pos->second,ptrSuiteTestConfig);
                ptrsSuiteTestConfig->mvChildrenSuiteTest.push_back(ptrSuiteTestConfig);
            }
        }
    }

#if 0
    // s_single_item_test_config
    boost::property_tree::ptree GetSingleItemTestConfigJson(s_single_item_test_config* psSingleItemTestConfig)
    {
        boost::property_tree::ptree itemJson;

        itemJson.put("IsTest",psSingleItemTestConfig->mbIsTest);
        itemJson.put("FinalizeTest",psSingleItemTestConfig->mbFinalizeTest);
        itemJson.put("IfFalse",psSingleItemTestConfig->mbIfFalse);
        itemJson.put("IsFolderTest",psSingleItemTestConfig->mbIsFolderTest);
        itemJson.put("ReversedResult",psSingleItemTestConfig->mbReversedResult);
        itemJson.put("TestName",JGW_W2A(psSingleItemTestConfig->mstrTestName,CP_UTF8));

        boost::property_tree::ptree pParamsJson;
        for (std::vector<s_tse_test_param>::iterator it = psSingleItemTestConfig->mvTestConfigs.begin();it != psSingleItemTestConfig->mvTestConfigs.end();++it)
        {
            boost::property_tree::ptree item;
            item.put("name",JGW_W2A(it->mParamName,CP_UTF8));
            item.put("value",JGW_W2A(it->mParamValue,CP_UTF8));
            item.put("des",JGW_W2A(it->mParamDescription,CP_UTF8));
            pParamsJson.push_back(std::make_pair("", item));
        }
        itemJson.put_child("Params",pParamsJson);
        return itemJson;
    }
#else
    boost::property_tree::ptree GetSingleItemTestConfigJson(std::shared_ptr<s_suite_test_config>& ptrsSuiteTestConfig)
    {
        boost::property_tree::ptree itemJson;

        itemJson.put("IsTest",ptrsSuiteTestConfig->msItemTestConfig.mbIsTest);
        itemJson.put("FinalizeTest",ptrsSuiteTestConfig->msItemTestConfig.mbFinalizeTest);
        itemJson.put("IfFalse",ptrsSuiteTestConfig->msItemTestConfig.mbIfFalse);
        itemJson.put("IsFolderTest",ptrsSuiteTestConfig->msItemTestConfig.mbIsFolderTest);
        itemJson.put("ReversedResult",ptrsSuiteTestConfig->msItemTestConfig.mbReversedResult);
        itemJson.put("TestName",JGW_W2A(ptrsSuiteTestConfig->msItemTestConfig.mstrTestName,CP_UTF8));

        boost::property_tree::ptree pParamsJson;
        for (std::vector<s_tse_test_param>::iterator it = ptrsSuiteTestConfig->msItemTestConfig.mvTestConfigs.begin();it != ptrsSuiteTestConfig->msItemTestConfig.mvTestConfigs.end();++it)
        {
            boost::property_tree::ptree item;
            item.put("name",JGW_W2A(it->mParamName,CP_UTF8));
            item.put("value",JGW_W2A(it->mParamValue,CP_UTF8));
            item.put("des",JGW_W2A(it->mParamDescription,CP_UTF8));
            pParamsJson.push_back(std::make_pair("", item));
        }
        itemJson.put_child("Params",pParamsJson);

        if (!ptrsSuiteTestConfig->mvChildrenSuiteTest.empty())
        {
            boost::property_tree::ptree pChildsJson;
            for (std::vector<std::shared_ptr<s_suite_test_config>>::iterator it = ptrsSuiteTestConfig->mvChildrenSuiteTest.begin();it != ptrsSuiteTestConfig->mvChildrenSuiteTest.end();++it)
            {
                pChildsJson.push_back(std::make_pair("", GetSingleItemTestConfigJson(it[0])));
            }
            itemJson.put_child("Childs",pChildsJson);
        }

        return itemJson;
    }
#endif

    void GetSuiteTestConfigToTreeNodeUI(CTreeNodeUI* pTreeNodeUI,std::shared_ptr<s_suite_test_config>& ptrsSuiteTestConfig)
    {
        s_single_item_test_config* psSingleItemTestConfig = (s_single_item_test_config*)pTreeNodeUI->GetDate();
        if (NULL == psSingleItemTestConfig) return;
        ptrsSuiteTestConfig->msItemTestConfig.mbFinalizeTest = psSingleItemTestConfig->mbFinalizeTest;
        ptrsSuiteTestConfig->msItemTestConfig.mbIfFalse = psSingleItemTestConfig->mbIfFalse;
        ptrsSuiteTestConfig->msItemTestConfig.mbIsFolderTest = psSingleItemTestConfig->mbIsFolderTest;
        ptrsSuiteTestConfig->msItemTestConfig.mbIsTest = psSingleItemTestConfig->mbIsTest;
        ptrsSuiteTestConfig->msItemTestConfig.mbReversedResult = psSingleItemTestConfig->mbReversedResult;
        ptrsSuiteTestConfig->msItemTestConfig.mstrTestName = psSingleItemTestConfig->mstrTestName;
        for (std::vector<s_tse_test_param>::iterator it = psSingleItemTestConfig->mvTestConfigs.begin();it != psSingleItemTestConfig->mvTestConfigs.end();++it)
        {
            ptrsSuiteTestConfig->msItemTestConfig.mvTestConfigs.push_back(it[0]);
        }

        for (int index = 0;index < pTreeNodeUI->GetCountChild();pTreeNodeUI++)
        {
            CTreeNodeUI* pChildTreeNodeUI = pTreeNodeUI->GetChildNode(index);
            std::shared_ptr<s_suite_test_config> ptrsChildSuiteTestConfig(new s_suite_test_config);
            GetSuiteTestConfigToTreeNodeUI(pChildTreeNodeUI,ptrsChildSuiteTestConfig);
            ptrsSuiteTestConfig->mvChildrenSuiteTest.push_back(ptrsChildSuiteTestConfig);
        }
    }

    void CCJGW_TSEConfigEditDlg::OnMenuElementClick(TEventUI* pTEventUI)
    {
        std::wstring strName ((const wchar_t*)pTEventUI->wParam);
        delete[] (const wchar_t*)pTEventUI->wParam;

        if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("saveMenuElement")))
        { 
            SaveTSEConfig();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("CopyTreeNode")))
        {
            if (mpSelectTreeNodeUI && NULL != mpSelectTreeNodeUI->GetDate())
            {
#if 0
                boost::property_tree::ptree json = GetSingleItemTestConfigJson((s_single_item_test_config*)mpSelectTreeNodeUI->GetDate());
                std::stringstream is;
                boost::property_tree::write_json(is,json);
                std::string s = is.str();
                CCJGW_Clipboard::SetClipBoardText(JGW_A2W(s,CP_UTF8).c_str(),m_hWnd);
                OutputDebugStringA(s.c_str());
                OutputDebugStringA("\r\n");
#else
                std::shared_ptr<s_suite_test_config> ptrsSuiteTestConfig(new s_suite_test_config);
                GetSuiteTestConfigToTreeNodeUI(mpSelectTreeNodeUI,ptrsSuiteTestConfig);
                boost::property_tree::ptree json = GetSingleItemTestConfigJson(ptrsSuiteTestConfig);

                std::stringstream is;
                boost::property_tree::write_json(is,json);
                std::string s = is.str();
                CCJGW_Clipboard::SetClipBoardText(JGW_A2W(s,CP_UTF8).c_str(),m_hWnd);
#endif
            }
            //MessageBox(m_hWnd,L"CopyTreeNode",NULL,MB_OK);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("PasteTreeNode")))
        {
#if 0
            //MessageBox(m_hWnd,L"PasteTreeNode",NULL,MB_OK);
            std::wstring strValue;
            boost::property_tree::ptree json;
            CCJGW_Clipboard::GetClipBoardText(m_hWnd,strValue);
            std::stringstream msg_ss(JGW_W2A(strValue,CP_UTF8));
            read_json(msg_ss, json);
            std::shared_ptr<s_suite_test_config> ptrSuiteTestConfig(new s_suite_test_config);

            ParseSingleItemTestConfigToJson(json,ptrSuiteTestConfig);

            std::wstring strFolderAttr = TREEVIEW_FILE_ATTR;
            if (!ptrSuiteTestConfig->mvChildrenSuiteTest.empty()) strFolderAttr = TREEVIEW_MULTI_FILE_ATTR;

            CTreeNodeUI* pChildTreeNodeUI = BuildTreeNodeUI(strFolderAttr,TREEVIEW_CHECKBOX_ATTR,ptrSuiteTestConfig->msItemTestConfig.mbIsTest);
            pChildTreeNodeUI->SetItemText(ptrSuiteTestConfig->msItemTestConfig.mstrTestName.c_str());
            pChildTreeNodeUI->SetDate(CopySingleItemTestConfig(ptrSuiteTestConfig->msItemTestConfig));
            mpSelectTreeNodeUI->GetParentNode()->AddChildNode(mpSelectTreeNodeUI,pChildTreeNodeUI);
            // AddSuiteTestConfigTreeNodes(mpSelectTreeNodeUI->GetParentNode(),ptrSuiteTestConfig);
            OnUpdateTreeView();
            //msTSEMultiUI.mvsSingleItemTestConfig.push_back(psItemTestConfig);
#else
            std::wstring strValue;
            boost::property_tree::ptree json;
            std::shared_ptr<s_suite_test_config> ptrSuiteTestConfig(new s_suite_test_config);

            CCJGW_Clipboard::GetClipBoardText(m_hWnd,strValue);
            std::stringstream msg_ss(JGW_W2A(strValue,CP_UTF8));
            try 
            {
                read_json(msg_ss, json);
            }
            catch (boost::property_tree::json_parser::json_parser_error& )
            {
                return ;
            }
            ParseSingleItemTestConfigToJson(json,ptrSuiteTestConfig);

            AddSuiteTestConfigTreeNodes(mpSelectTreeNodeUI->GetParentNode(),ptrSuiteTestConfig,mpSelectTreeNodeUI);
            OnUpdateTreeView();
#endif
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName.c_str(),_T("DeleteTreeNode")))
        {
            MessageBox(m_hWnd,L"DeleteTreeNode",NULL,MB_OK);
        }
    }

    LRESULT CCJGW_TSEConfigEditDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) 
    {
        if(!(::GetAsyncKeyState(VK_CONTROL) & 0x8000)) return FALSE;
        switch (wParam)
        {
        case 's':
        case 'S':
            // Ctrl+Alt+S
            if (::GetAsyncKeyState(VK_MENU)& 0x8000)
            {
                //SaveTSEConfig();
            }
            else
            {
                SaveTSEConfig();
            }
            break;
        }
        return FALSE;
    }

    void CCJGW_TSEConfigEditDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (msg.pSender == mpProjectCombo)
            {
                CControlUI* pCControlUI = mpProjectCombo->GetItemAt(mpProjectCombo->GetCurSel());
                if (NULL != pCControlUI->GetDate())
                {
                    OnRspProjectGroupPluginInfoChange((S_PROJECT_GROUP_PLUGIN_INFO*)pCControlUI->GetDate());
                }  
            }
            else if (msg.pSender == mpProjectGroupCombo)
            {
                CControlUI* pCControlUI = mpProjectGroupCombo->GetItemAt(mpProjectGroupCombo->GetCurSel());
                if (NULL != pCControlUI->GetDate())
                {
                    OnRspPluginGroupInfoChange((S_PLUGIN_GROUP_INFO*)pCControlUI->GetDate());
                }  
            }
            else if (msg.pSender == mpPluginNameCombo)
            {
                CControlUI* pCControlUI = mpPluginNameCombo->GetItemAt(mpPluginNameCombo->GetCurSel());
                if (NULL != pCControlUI->GetDate())
                {
                    OnRspPluginInfoChange((S_PLUGIN_INFO*)pCControlUI->GetDate());
                }  
            }
            else if (msg.pSender == mpPluinSuiteTree)
            {
                SaveTestSuiteItem();

                CTreeNodeUI* pTreeNodeUi =  (CTreeNodeUI*)mpPluinSuiteTree->GetItemAt(msg.wParam);
                msTreeNodeData.mpPrevTreeNodeUI = pTreeNodeUi;

                if (NULL != pTreeNodeUi->GetDate())
                {
                    if (pTreeNodeUi->GetItemText() == L"GlobalVariables")
                    {
                        msTreeNodeData.mbIsTSETest = false;
                        msTreeNodeData.mmapGlobalVariables = (_mMapGlobalVariables*)pTreeNodeUi->GetDate();
                        CreateGlobalVariablesList(msTreeNodeData.mmapGlobalVariables);
                    }
                    else
                    {
                        msTreeNodeData.mbIsTSETest = true;
                        msTreeNodeData.mpsItemTestConfig = (s_single_item_test_config*)pTreeNodeUi->GetDate();
                        CreateTestSuiteItemLList(msTreeNodeData.mpsItemTestConfig);
                    }
                }
            }
        }
        else if (msg.sType == DUI_MSGTYPE_KILLFOCUS)
        {
            CRichEditUI* pControl = (CRichEditUI*)msg.pSender;
            std::vector<S_TSE_LIST_UI_INFO>::iterator it = FindRichEditUIIsTseList(pControl);
            if (mvTSEListUIInfo.end() != it)
            {
                CListContainerElementUIEx* pitListContainerElementUIEx = it->mpListContainerElementUIEx;
                pitListContainerElementUIEx->SetAttribute(_T("height"),_T(LIST_HEIGHT_NORMAL));
            }
        }
        else if (msg.sType == DUI_MSGTYPE_SETFOCUS)
        {
            CRichEditUI* pControl = (CRichEditUI*)msg.pSender;
            std::vector<S_TSE_LIST_UI_INFO>::iterator it = FindRichEditUIIsTseList(pControl);
            if (mvTSEListUIInfo.end() != it)
            {
                CListContainerElementUIEx* pitListContainerElementUIEx = it->mpListContainerElementUIEx;
                pitListContainerElementUIEx->SetAttribute(_T("height"),_T(LIST_HEIGHT_FOCUS));
            }
        }
        else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
        {
            if (!mvTSEListUIInfo.empty() && mbIsInitTSEListCompleted)
            {
                std::vector<S_TSE_LIST_UI_INFO>::iterator it = mvTSEListUIInfo.end();
                -- it; 
                mbIsSaveTSEConfig = true;
                mpSaveTipsLab->SetText(L"*");
#if 0
                if (it->first == msg.pSender || it->second.mpFristEditUI == msg.pSender)
#else
                if (it->mpFristEditUI == msg.pSender)
#endif
                {
                    LPS_TSE_LIST_UI_INFO psTSEListUIInfo = AddEmptyTSETestParam();
                    psTSEListUIInfo->mpFristEditUI->SetReadOnly(false);
                }
            }
        }
        else if (msg.sType == L"treeviewenddrag")
        {
            if (msg.pSender == mpPluinSuiteTree)
            {
                OnUpdateTreeView();
                mbIsSaveTSEConfig = true;
                mpSaveTipsLab->SetText(L"*");
            }  
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"EditMenuBtn")
            {
                OnEditMenuBtnClick(msg);
            }
        }
    }

    std::vector<S_TSE_LIST_UI_INFO>::iterator CCJGW_TSEConfigEditDlg::FindRichEditUIIsTseList(CRichEditUI* pRichEditUI)
    {
        for (std::vector<S_TSE_LIST_UI_INFO>::iterator it = mvTSEListUIInfo.begin();it != mvTSEListUIInfo.end();++it)
        {
            if (it->mpSecondRichEditUI == pRichEditUI) return it;
        }
        return mvTSEListUIInfo.end();
    }

    void CCJGW_TSEConfigEditDlg::OnCloseWindow()
    {
        //if (NULL != mpPluinSuiteTree) mpPluinSuiteTree->OnEvent += MakeDelegate(this,&CCJGW_TSEConfigEditDlg::OnSuiteTreeMessage,0);
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_TSEConfigEditDlg::OnEventCustomMessage,0);
        if (mbIsSaveTSEConfig && MB_OK == MessageBox(m_hWnd,L"配置文件有修改是否保存配置文件",L"保存配置文件",MB_OKCANCEL))
        {
            SaveTSEConfig();
        } 
        m_PaintManager.RemoveMessageFilter(this);
    }

    bool CCJGW_TSEConfigEditDlg::LoadControlUiClassPtr()
    {
        bool bFindControlOk = true;

        mpTipslab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"Tipslab"));
        mpSaveTipsLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"SaveTipsLab"));
        mpProjectCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"ProjectCombo"));
        mpProjectGroupCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"ProjectGroupCombo"));
        mpPluginNameCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"PluginNameCombo"));
        //mpAddSuiteBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"AddSuiteBtn"));
        //mpDelSuiteBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"DelSuiteBtn"));
        mpPluinSuiteTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(L"PluinSuiteTree"));
        mpSuiteList = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(L"SuiteList"));
        mpEditTips = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"EditTips"));
        mpDragHorizontalLayoutUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"DragHorizontalLayoutUI"));

        bFindControlOk &= (NULL != mpTipslab);
        bFindControlOk &= (NULL != mpSaveTipsLab);
        bFindControlOk &= (NULL != mpProjectCombo);
        bFindControlOk &= (NULL != mpProjectGroupCombo);
        bFindControlOk &= (NULL != mpPluginNameCombo);
        /*bFindControlOk &= (NULL != mpAddSuiteBtn);
        bFindControlOk &= (NULL != mpDelSuiteBtn);*/
        bFindControlOk &= (NULL != mpPluinSuiteTree);
        bFindControlOk &= (NULL != mpSuiteList);
        bFindControlOk &= (NULL != mpEditTips);
        bFindControlOk &= (NULL != mpDragHorizontalLayoutUI);

        if(!bFindControlOk) { Close(IDCANCEL); return bFindControlOk;}
        mpPluinSuiteTree->m_pDragingCtrl = mpDragHorizontalLayoutUI;
        mpPluinSuiteTree->SetAllowDiffParentNodeMove(true);

        return bFindControlOk;
    }

    void CCJGW_TSEConfigEditDlg::AddComboControlUI(CComboUI* mpCComboUI,const std::wstring& strValue,LPVOID lpVoid)
    {
        if (NULL != mpCComboUI)
        {
            CListLabelElementUI* pListLabelEUi = new CListLabelElementUI();
            pListLabelEUi->SetText(strValue.c_str());
            pListLabelEUi->SetDate(lpVoid);
            mpCComboUI->Add(pListLabelEUi);
        }
    }

    bool CCJGW_TSEConfigEditDlg::InitMainFrameSkinConfig()
    {
        if (!mcTSEConfigEditDlg.LoadViewLayoutConfig(JGW::JGW_W2A_W(JGW_GetUIConfigFilePath()))) 
        {
            ::MessageBox(GetHWND(),GetString(E_VIEW_LAYOUT_FILE_LOADING_ERROR),GetString(E_VIEW_INIT_ERROR),MB_ICONERROR);
            Close(IDCANCEL);
            return false;
        }
        // 获取UI文件项目信息
        PS_UI_PROJECT_INFO psUiProjectInfo = mcTSEConfigEditDlg.GetUIProjectInfo();
        if (psUiProjectInfo->mvProjectGroupPluginInfo.empty()) return true;
        // 选择项目
        size_t nSelectProjectIndex = psUiProjectInfo->mnProjectIndex;
        if (nSelectProjectIndex >= psUiProjectInfo->mvProjectGroupPluginInfo.size()) nSelectProjectIndex = 0;
        mpProjectCombo->RemoveAll();
        // 添加项目信息到UI
        for (std::vector<S_PROJECT_GROUP_PLUGIN_INFO>::iterator it = psUiProjectInfo->mvProjectGroupPluginInfo.begin();it != psUiProjectInfo->mvProjectGroupPluginInfo.end();++ it)
        {
            AddComboControlUI(mpProjectCombo,it->mstrProjectName,&it[0]);
        }
        mpProjectCombo->SelectItem(nSelectProjectIndex);

        return true;
    }

    void CCJGW_TSEConfigEditDlg::OnRspProjectGroupPluginInfoChange(S_PROJECT_GROUP_PLUGIN_INFO* psProjectGroupPluginInfo)
    {
        if (NULL == psProjectGroupPluginInfo || psProjectGroupPluginInfo->mvAllPluginInfo.size() == 0)
        {
            return ;
        }
        // 获取UI选择的测试段信息
        size_t nSelectGroupIndex = mcTSEConfigEditDlg.GetUIProjectInfo()->mnProjectIndex;
        if (nSelectGroupIndex >= psProjectGroupPluginInfo->mvAllPluginInfo.size()) nSelectGroupIndex = 0;
        mpProjectGroupCombo->RemoveAll();
        // 添加测试分段信息到UI
        for (_vplugininfo::iterator it = psProjectGroupPluginInfo->mvAllPluginInfo.begin();
            it != psProjectGroupPluginInfo->mvAllPluginInfo.end();
            ++ it)
        {
            AddComboControlUI(mpProjectGroupCombo,it->mstrGroupName,&it[0]);
        }
        mpProjectGroupCombo->SelectItem(nSelectGroupIndex);
    }

    void CCJGW_TSEConfigEditDlg::OnRspPluginGroupInfoChange(S_PLUGIN_GROUP_INFO* psPluginGroupInfo)
    {
        if (NULL == psPluginGroupInfo || psPluginGroupInfo->mvSubWinInfo.size() == 0) return ;
        mpPluginNameCombo->RemoveAll();
        // 添加测试信息到UI
        for (std::vector<S_PLUGIN_INFO>::iterator it = psPluginGroupInfo->mvSubWinInfo.begin();
            it != psPluginGroupInfo->mvSubWinInfo.end();
            ++ it)
        {
            AddComboControlUI(mpPluginNameCombo,it->mstrStationName,&it[0]);
            if (!CheckPluginTestIsTSEPlugin(it->mstrPluginName))
            {
                mpPluginNameCombo->GetItemAt(mpPluginNameCombo->GetCount() - 1)->SetEnabled(false);
            }
        }
        mpPluginNameCombo->SelectItem(0);
    }

    bool CCJGW_TSEConfigEditDlg::CheckPluginTestIsTSEPlugin(const std::wstring& strPluginName)
    {
        for (size_t index = 0;index < _countof(gstrTSEPlugin);index++)
        {
            if (NULL != JGW_WStrCaseWStr(strPluginName.c_str(),gstrTSEPlugin[index].c_str()))
            {
                return true;
            }
        }
        return false;
    }

    void CCJGW_TSEConfigEditDlg::ClearTreeAndListView()
    {
        mpSelectTreeNodeUI = NULL;
        mpSaveTipsLab->SetText(L""); 
        mbIsSaveTSEConfig = false;
        mpSuiteList->GetHeader()->RemoveAll();
        mpSuiteList->GetList()->RemoveAll();
        if (NULL != mpHomeTreeNode) DeleteTreeViewCheckBoxData(mpHomeTreeNode);
        mpPluinSuiteTree->RemoveAll();
        msTreeNodeData.mbIsTSETest = false;
        msTreeNodeData.mpsItemTestConfig = NULL;
        msTreeNodeData.mpPrevTreeNodeUI = NULL;
        msTSEMultiUI.mmapGlobalVariables.clear();
        //msTSEMultiUI.mvsSingleItemTestConfig.clear();
        for (std::vector<s_single_item_test_config*>::iterator it = msTSEMultiUI.mvsSingleItemTestConfig.begin();it != msTSEMultiUI.mvsSingleItemTestConfig.end();++it)
        {
            delete it[0];
        }
        msTSEMultiUI.mvsSingleItemTestConfig.clear();
        msTSEMultiUI.mmapGlobalVariables.clear();
    }

    void CCJGW_TSEConfigEditDlg::DeleteTreeViewCheckBoxData(CTreeNodeUI* pTreeNode)
    {
        PS_CHECKBOX_DATA psCheckBoxData =  (PS_CHECKBOX_DATA)pTreeNode->GetCheckBox()->GetDate();
        if (NULL != psCheckBoxData) delete psCheckBoxData;
        pTreeNode->GetCheckBox()->SetDate(NULL);

        for (int index = 0;index < pTreeNode->GetCountChild();index++)
        {
            CTreeNodeUI* pControl = (CTreeNodeUI*)pTreeNode->GetChildNode(index);
            DeleteTreeViewCheckBoxData(pControl);
        }
    }

    CTreeNodeUI* CCJGW_TSEConfigEditDlg::BuildTreeNodeUI(const std::wstring& strFolderAttr,const std::wstring& strCheckBoxAttr,bool isTest/* = true*/)
    {
#if 0
        CTreeNodeUI* pSubTreeNodeUi = new CTreeNodeUI;
        //pSubTreeNodeUi->SetAttribute(L"height",L"36");
        //pSubTreeNodeUi->SetAttribute(L"selected",L"false");
        //pSubTreeNodeUi->SetAttribute(_T("itemattr"), _T("valign=&quot;center&quot;"));
        // folderattr=&quot;treenode_folder_style&quot;
        //pSubTreeNodeUi->SetAttribute(L"style",L"folderattr=&quot;treenode_folder_style&quot;");
        // pSubTreeNodeUi->SetAttribute(L"style",L"treeview_style");
        pSubTreeNodeUi->SetAttribute(L"folderattr",L"padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16' \" hotimage=\"file='treeview_b.png' source='36,0,72,16' \" selectedimage=\"file='treeview_a.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_a.png' source='36,0,72,16' \"");
        pSubTreeNodeUi->SetAttribute(L"checkboxattr",L"width=\"16\" height=\"16\" normalimage=\"file='unchecked.png' source='0,0,36,16' \" selectedimage=\"file='checked.png'\"");
#else
        CTreeNodeUI* pSubTreeNodeUi = new CTreeNodeUI;
        if (!strFolderAttr.empty())
        {
            pSubTreeNodeUi->SetAttribute(L"folderattr",strFolderAttr.c_str());
        }

        if (!strCheckBoxAttr.empty())
        {
            pSubTreeNodeUi->SetAttribute(L"checkboxattr",strCheckBoxAttr.c_str());
        }
        else
        {
            pSubTreeNodeUi->GetCheckBox()->SetEnabled(false);
            pSubTreeNodeUi->GetCheckBox()->SetMaxWidth(0);
        }
        S_CHECKBOX_DATA* psCheckBoxData = new S_CHECKBOX_DATA;
        psCheckBoxData->meCurrentCheckBoxStatus = E_UNKNOW_CHECKBOX;
        psCheckBoxData->meUpdateCheckBoxStatus = isTest ? E_SELECTED:E_UNSELECTED;
        psCheckBoxData->mpCurrentTreeNodeUI = pSubTreeNodeUi;
        pSubTreeNodeUi->GetCheckBox()->SetDate(psCheckBoxData);
        pSubTreeNodeUi->GetCheckBox()->OnNotify += MakeDelegate(this,&CCJGW_TSEConfigEditDlg::OnTreeViewCheckBoxChanged,DUI_MSGTYPE_SELECTCHANGED);
#endif
        return pSubTreeNodeUi;
    }

    void CCJGW_TSEConfigEditDlg::ExpandCTreeNodeUI(CTreeNodeUI* pTreeNodeUI,bool isExpand)
    {
        if (isExpand)
        {
            pTreeNodeUI->GetFolderButton()->Selected(false);
            mpPluinSuiteTree->SetItemExpand(true,pTreeNodeUI);
        }
        else
        {
            pTreeNodeUI->GetFolderButton()->Selected(true);
            mpPluinSuiteTree->SetItemExpand(false,pTreeNodeUI);
        }
    }

    void CCJGW_TSEConfigEditDlg::CopyMapGlobalVariables(_mMapGlobalVariables& mmapDest,_mMapGlobalVariables& mmapSrc)
    {
        for (_mMapGlobalVariables::iterator it = mmapSrc.begin();it != mmapSrc.end();++it)
        {
            mmapDest[it->first] = it->second;
        }
    }

    void CCJGW_TSEConfigEditDlg::OnRspPluginInfoChange(S_PLUGIN_INFO* psPluginInfo)
    {
        if (mbIsSaveTSEConfig && MB_OK == MessageBox(m_hWnd,L"配置文件有修改是否保存配置文件",L"保存配置文件",MB_OKCANCEL))
        {
            SaveTSEConfig();
        } 
        ClearTreeAndListView();

        std::wstring strTSEConfigFilePath = JGW_GetTSEConfigFilePath(psPluginInfo->mstrModuleFolderName,psPluginInfo->mstrSuiteXmlName);
        mpEditTips->SetText(strTSEConfigFilePath.c_str());
        if (!mcTSEFactoryConfig.LoadTSEFactoryConfig(JGW_W2A(strTSEConfigFilePath,CP_UTF8)))
        {
            MessageBox(m_PaintManager.GetPaintWindow(),L"加载配置文件出错",L"错误",MB_ICONERROR);
            return ;
        }
        // 创建treeview
        LPS_TSE_CONFIG_INFOS psTSEConfigInfos = mcTSEFactoryConfig.GetTSEConfigInfos();
        CTreeNodeUI* pHomeTreeNodeUi = BuildTreeNodeUI(TREEVIEW_MULTI_FOLEDER_ATTR,TREEVIEW_CHECKBOX_ATTR);
        pHomeTreeNodeUi->SetItemText(L"TSE");
        mpHomeTreeNode = pHomeTreeNodeUi;

        // 创建 GlobalVariables
        {
            CTreeNodeUI* pGlobalVariablesFolderTreeNodeUI = BuildTreeNodeUI(TREEVIEW_MULTI_FOLEDER_ATTR,TREEVIEW_CHECKBOX_ATTR);
            pGlobalVariablesFolderTreeNodeUI->SetItemText(L"Environment");

            CTreeNodeUI* pGlobalVariablesTreeNodeUI = BuildTreeNodeUI(TREEVIEW_FILE_ATTR,TREEVIEW_CHECKBOX_ATTR);
            pGlobalVariablesTreeNodeUI->SetItemText(L"GlobalVariables");
            CopyMapGlobalVariables(msTSEMultiUI.mmapGlobalVariables,psTSEConfigInfos->mmapGlobalVariables);
            pGlobalVariablesTreeNodeUI->SetDate(&msTSEMultiUI.mmapGlobalVariables);

            pGlobalVariablesFolderTreeNodeUI->AddChildNode(pGlobalVariablesTreeNodeUI);
            //ExpandCTreeNodeUI(pGlobalVariablesFolderTreeNodeUI,false);
            pHomeTreeNodeUi->AddChildNode(pGlobalVariablesFolderTreeNodeUI);
        }
        // Config
        {
            if (!psTSEConfigInfos->m_vConfigViewTestConfig.empty())
            {
                CTreeNodeUI* pConfigFolderTreeNodeUI = BuildTreeNodeUI(TREEVIEW_MULTI_FOLEDER_ATTR,TREEVIEW_CHECKBOX_ATTR);
                pConfigFolderTreeNodeUI->SetItemText(L"Config");
                pHomeTreeNodeUi->AddChildNode(pConfigFolderTreeNodeUI);

                for (std::vector<std::shared_ptr<s_suite_test_config>>::iterator it = psTSEConfigInfos->m_vConfigViewTestConfig.begin();it != psTSEConfigInfos->m_vConfigViewTestConfig.end();++ it)
                {
                    AddSuiteTestConfigTreeNodes(pConfigFolderTreeNodeUI,it[0]);
                }
            }
        }
        // Test Automation
        {
            if (!psTSEConfigInfos->m_vAutoTestConfig.empty())
            {
                CTreeNodeUI* pTestAutomationTreeNodeUI = BuildTreeNodeUI(TREEVIEW_MULTI_FOLEDER_ATTR,TREEVIEW_CHECKBOX_ATTR);
                pTestAutomationTreeNodeUI->SetItemText(L"Test Automation");
                pHomeTreeNodeUi->AddChildNode(pTestAutomationTreeNodeUI);

                for (std::vector<std::shared_ptr<s_suite_test_config>>::iterator it = psTSEConfigInfos->m_vAutoTestConfig.begin();it != psTSEConfigInfos->m_vAutoTestConfig.end();++ it)
                {
                    AddSuiteTestConfigTreeNodes(pTestAutomationTreeNodeUI,it[0]);
                } 
            }
        }
        // Test Main
        {
            if (!psTSEConfigInfos->m_vSuiteTestConfig.empty())
            {
                CTreeNodeUI* pTestMainTreeNodeUI = BuildTreeNodeUI(TREEVIEW_MULTI_FOLEDER_ATTR,TREEVIEW_CHECKBOX_ATTR);
                pTestMainTreeNodeUI->SetItemText(L"Test Main");
                pHomeTreeNodeUi->AddChildNode(pTestMainTreeNodeUI);

                for (std::vector<std::shared_ptr<s_suite_test_config>>::iterator it = psTSEConfigInfos->m_vSuiteTestConfig.begin();it != psTSEConfigInfos->m_vSuiteTestConfig.end();++ it)
                {
                    AddSuiteTestConfigTreeNodes(pTestMainTreeNodeUI,it[0]);
                }

            }
        }
        mpPluinSuiteTree->Add(pHomeTreeNodeUi);
        OnUpdateTreeView();
        return ;
    }

    void CCJGW_TSEConfigEditDlg::OnUpdateTreeView()
    {
        mbIsRspCheckBoxChanged = false;
        UpdateTreeViewCheckBoxView(mpHomeTreeNode);
        mbIsRspCheckBoxChanged = true;
    }

    E_CHECKBOX_SELECTED_STATUS CCJGW_TSEConfigEditDlg::UpdateTreeNodeUI(CTreeNodeUI* pControl)
    {
        PS_CHECKBOX_DATA psCheckBoxData =  (PS_CHECKBOX_DATA)pControl->GetCheckBox()->GetDate();
        if (psCheckBoxData->meCurrentCheckBoxStatus != psCheckBoxData->meUpdateCheckBoxStatus)
        {
            if (psCheckBoxData->meUpdateCheckBoxStatus ==  E_UNSELECTED)
            {
                pControl->GetCheckBox()->Selected(false);
            }
            else if (psCheckBoxData->meUpdateCheckBoxStatus ==  E_SELECTED)
            {
                pControl->GetCheckBox()->Selected(true);
                pControl->SetAttribute(L"checkboxattr",TREEVIEW_CHECKBOX_ATTR);
            }
            else
            {
                pControl->GetCheckBox()->Selected(true);
                pControl->SetAttribute(L"checkboxattr",TREEVIEW_PART_CHECKBOX_ATTR);
            }
            psCheckBoxData->meCurrentCheckBoxStatus = psCheckBoxData->meUpdateCheckBoxStatus;
        }
        return psCheckBoxData->meCurrentCheckBoxStatus;
    }

    E_CHECKBOX_SELECTED_STATUS CCJGW_TSEConfigEditDlg::UpdateTreeViewCheckBoxView(CTreeNodeUI* pTreeNodeUI)
    {
        E_CHECKBOX_SELECTED_STATUS eCheckBoxStatus;
        int currentCheckBoxStatus = 0x00;

        for (int index = 0;index < pTreeNodeUI->GetCountChild();index++)
        {
            CTreeNodeUI* pControl = (CTreeNodeUI*)pTreeNodeUI->GetChildNode(index);
            if (0 != pControl->GetCountChild())
            {
                eCheckBoxStatus = UpdateTreeViewCheckBoxView(pControl);
            }
            else
            {
                eCheckBoxStatus = UpdateTreeNodeUI(pControl);
            }
            // 检查当前是否混合选中以及未选中的节点
            switch (eCheckBoxStatus)
            {
            case E_SELECTED:
                currentCheckBoxStatus |= 0x01;
                break;
            case E_UNSELECTED:
                currentCheckBoxStatus |= 0x02;
                break;
            case E_PART_SELECTED:
                currentCheckBoxStatus |= 0x04;
                break;
            }
        }
        if (0 != pTreeNodeUI->GetCountChild())
        {
            if (currentCheckBoxStatus == 0x01)
            {
                eCheckBoxStatus =  E_SELECTED;
            }
            else if (currentCheckBoxStatus == 0x02)
            {
                eCheckBoxStatus =  E_UNSELECTED;
            }
            else
            {
                eCheckBoxStatus = E_PART_SELECTED;
            }
            PS_CHECKBOX_DATA psCheckBoxData =  (PS_CHECKBOX_DATA)pTreeNodeUI->GetCheckBox()->GetDate();
            psCheckBoxData->meUpdateCheckBoxStatus = eCheckBoxStatus;
        }
        eCheckBoxStatus = UpdateTreeNodeUI(pTreeNodeUI);

        return eCheckBoxStatus;
    }

    s_single_item_test_config* CCJGW_TSEConfigEditDlg::CopySingleItemTestConfig(s_single_item_test_config& sSingleItemTestConfig)
    {
        s_single_item_test_config* psItemTestConfig = new s_single_item_test_config;
        psItemTestConfig->mbFinalizeTest = sSingleItemTestConfig.mbFinalizeTest;
        psItemTestConfig->mbIfFalse = sSingleItemTestConfig.mbIfFalse;
        psItemTestConfig->mbIsFolderTest = sSingleItemTestConfig.mbIsFolderTest;
        psItemTestConfig->mbIsTest = sSingleItemTestConfig.mbIsTest;
        psItemTestConfig->mbReversedResult = sSingleItemTestConfig.mbReversedResult;
        psItemTestConfig->mstrTestName = sSingleItemTestConfig.mstrTestName;
        for (std::vector<s_tse_test_param>::iterator it = sSingleItemTestConfig.mvTestConfigs.begin();it != sSingleItemTestConfig.mvTestConfigs.end();++it)
        {
            psItemTestConfig->mvTestConfigs.push_back(it[0]);
        }
        msTSEMultiUI.mvsSingleItemTestConfig.push_back(psItemTestConfig);
        return psItemTestConfig;
    }

    void CCJGW_TSEConfigEditDlg::AddSuiteTestConfigTreeNodes(CTreeNodeUI* pParentTreeNodeUI,std::shared_ptr<s_suite_test_config>& ptrSuiteTestConfig,CTreeNodeUI* pSiblingTreeNodeUI /* = NULL */)
    {
        // TREEVIEW_MULTI_FILE_ATTR TREEVIEW_FILE_ATTR
        std::wstring strFolderAttr = TREEVIEW_FILE_ATTR;
        if (!ptrSuiteTestConfig->mvChildrenSuiteTest.empty()) strFolderAttr = TREEVIEW_MULTI_FILE_ATTR;

        CTreeNodeUI* pChildTreeNodeUI = BuildTreeNodeUI(strFolderAttr,TREEVIEW_CHECKBOX_ATTR,ptrSuiteTestConfig->msItemTestConfig.mbIsTest);
        pChildTreeNodeUI->SetItemText(ptrSuiteTestConfig->msItemTestConfig.mstrTestName.c_str());
        pChildTreeNodeUI->SetDate(CopySingleItemTestConfig(ptrSuiteTestConfig->msItemTestConfig));
        if (NULL == pSiblingTreeNodeUI) pParentTreeNodeUI->AddChildNode(pChildTreeNodeUI);
        else pParentTreeNodeUI->AddChildNode(pSiblingTreeNodeUI,pChildTreeNodeUI);

        for (std::vector<std::shared_ptr<s_suite_test_config>>::iterator it = ptrSuiteTestConfig->mvChildrenSuiteTest.begin();it != ptrSuiteTestConfig->mvChildrenSuiteTest.end();++ it)
        {
            AddSuiteTestConfigTreeNodes(pChildTreeNodeUI,it[0],pSiblingTreeNodeUI);
        }   
    }

    void CCJGW_TSEConfigEditDlg::SetTreeItemCheckBox(bool isSelect,CTreeNodeUI* pTreeNode)
    {
        if(pTreeNode->GetCountChild() > 0)
        {
            for(int nIndex = 0;nIndex < pTreeNode->GetCountChild();nIndex++)
            {
                CTreeNodeUI* pItem = pTreeNode->GetChildNode(nIndex);
                PS_CHECKBOX_DATA psCheckBoxData =  (PS_CHECKBOX_DATA)pItem->GetCheckBox()->GetDate();
                if (NULL != psCheckBoxData) psCheckBoxData->meUpdateCheckBoxStatus = isSelect ? E_SELECTED:E_UNSELECTED;
                if(pItem->GetCountChild())
                    SetTreeItemCheckBox(isSelect,pItem);
            }
        }
    }

    bool CCJGW_TSEConfigEditDlg::OnTreeViewCheckBoxChanged(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
    {
        if (!mbIsRspCheckBoxChanged) return false;
#if 1
        CCheckBoxUI* pControl = (CCheckBoxUI*)pTNotifyUI->pSender;
        PS_CHECKBOX_DATA psCheckBoxData =  (PS_CHECKBOX_DATA)pControl->GetDate();
        if (NULL != psCheckBoxData)
        {
            mbIsSaveTSEConfig = true;
            mpSaveTipsLab->SetText(L"*");
            if (0 == psCheckBoxData->mpCurrentTreeNodeUI->GetCountChild())
            {
                psCheckBoxData->meUpdateCheckBoxStatus = pControl->GetCheck() ? E_SELECTED:E_UNSELECTED;
            }
            else
            {
                psCheckBoxData->meUpdateCheckBoxStatus = pControl->GetCheck() ? E_SELECTED:E_UNSELECTED;
                SetTreeItemCheckBox(pControl->GetCheck(),psCheckBoxData->mpCurrentTreeNodeUI);
            }
            OnUpdateTreeView();
        }
#endif
        return false;
    }

    LPS_TSE_LIST_UI_INFO CCJGW_TSEConfigEditDlg::AddEmptyTSETestParam()
    {
        CDialogBuilder builder1;
        S_TSE_LIST_UI_INFO sTSEListUiInfo = {0};
        CListContainerElementUIEx* lplsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(_T("edit_suite_list.xml"),(UINT)0,NULL,&m_PaintManager));
        if(lplsitContainerItem != NULL) mpSuiteList->InsertItem(mpSuiteList->GetCount(), atoi(LIST_HEIGHT_NORMAL), lplsitContainerItem);

        CHorizontalLayoutUI* pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(0));
        sTSEListUiInfo.mpFristEditUI = ((CEditUI*)(pHorizontalLayoutUi->GetItemAt(0)));
        pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
        sTSEListUiInfo.mpSecondRichEditUI = (CRichEditUI*)pHorizontalLayoutUi->GetItemAt(0);
        sTSEListUiInfo.mpSecondRichEditUI->SetEventMask(ENM_CHANGE);
        pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(2));
        sTSEListUiInfo.mpThreeEditUI = ((CEditUI*)(pHorizontalLayoutUi->GetItemAt(0)));
        sTSEListUiInfo.mpListContainerElementUIEx = lplsitContainerItem;

        mvTSEListUIInfo.push_back(sTSEListUiInfo);
        //mmapRichEditUIList.insert(std::make_pair<CRichEditUI*,S_TSE_LIST_UI_INFO>(sTSEListUiInfo.mpSecondRichEditUI,sTSEListUiInfo));
        return &mvTSEListUIInfo[mvTSEListUIInfo.size() - 1];
    }

    void CCJGW_TSEConfigEditDlg::CreateTestSuiteItemLList(s_single_item_test_config* psSuiteItemTestConfig)
    {
        mbIsInitTSEListCompleted = false;
        mvTSEListUIInfo.clear();
        mpSuiteList->GetHeader()->RemoveAll();
        mpSuiteList->GetList()->RemoveAll();
        mpSuiteList->SetDate(psSuiteItemTestConfig);

        mpSuiteList->InsertColumn(0,_T("Param Name"),0);
        mpSuiteList->InsertColumn(1,_T("Param Value"),0);
        mpSuiteList->InsertColumn(2,_T("Description"),0);

        std::wstring strTemp = L"",strChannel,strLoss;
        for (std::vector<s_tse_test_param>::iterator it = psSuiteItemTestConfig->mvTestConfigs.begin();
            it != psSuiteItemTestConfig->mvTestConfigs.end();
            ++ it)
        {
            LPS_TSE_LIST_UI_INFO psTSEListUIInfo = AddEmptyTSETestParam();
            psTSEListUIInfo->mpFristEditUI->SetText(it->mParamName.c_str());
            psTSEListUIInfo->mpSecondRichEditUI->SetText(it->mParamValue.c_str());
            //psTSEListUIInfo->mpSecondRichEditUI->SetEventMask(ENM_CHANGE);
            psTSEListUIInfo->mpThreeEditUI->SetText(it->mParamDescription.c_str());
#if 0
            CDialogBuilder builder1;
            lplsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(_T("edit_suite_list.xml"),(UINT)0,NULL,&m_PaintManager));
            if( !lplsitContainerItem )continue;
            if(lplsitContainerItem != NULL) mpSuiteList->InsertItem(mpSuiteList->GetCount(), atoi(LIST_HEIGHT_NORMAL), lplsitContainerItem);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(0));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(0)->SetText(it->mParamName.c_str());

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
            if( !pHorizontalLayoutUi )continue;
            CRichEditUI* pTempEdt = (CRichEditUI*)pHorizontalLayoutUi->GetItemAt(0);
            //pTempEdt->
            pTempEdt->SetText(it->mParamValue.c_str());
            mmapRichEditUIList.insert(std::make_pair<CRichEditUI*,CListContainerElementUIEx*>(pTempEdt,lplsitContainerItem));
            //if (E_PARAM_INT == it->meParamType)
            //{
            //    //! ^[0-9]*$
            //    pTempEdt->SetRegularCheck(L"^[0-9]*$");
            //    pTempEdt->SetToolTip(L"Input Type: int");
            //}
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(2));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(0)->SetText(it->mParamDescription.c_str());
#endif
        }
        LPS_TSE_LIST_UI_INFO psTSEListUIInfo = AddEmptyTSETestParam();
        psTSEListUIInfo->mpFristEditUI->SetReadOnly(false);
        mbIsInitTSEListCompleted = true;
    }

    void CCJGW_TSEConfigEditDlg::CreateGlobalVariablesList(_mMapGlobalVariables* mmapGlobalVariables)
    {
        mbIsInitTSEListCompleted = false;
        mvTSEListUIInfo.clear();
        mpSuiteList->GetHeader()->RemoveAll();
        mpSuiteList->GetList()->RemoveAll();
        mpSuiteList->SetDate(mmapGlobalVariables);

        mpSuiteList->InsertColumn(0,_T("Param Name"),0);
        mpSuiteList->InsertColumn(1,_T("Param Value"),0);
        mpSuiteList->InsertColumn(2,_T("Description"),0);

        std::wstring strTemp = L"",strChannel,strLoss;
        for (std::unordered_map<std::wstring,std::wstring>::iterator it = mmapGlobalVariables->begin();
            it != mmapGlobalVariables->end();
            ++ it)
        {
            LPS_TSE_LIST_UI_INFO psTSEListUIInfo = AddEmptyTSETestParam();
            psTSEListUIInfo->mpFristEditUI->SetText(it->first.c_str());
            psTSEListUIInfo->mpSecondRichEditUI->SetText(it->second.c_str());
            //psTSEListUIInfo->mpThreeEditUI->SetText(it->mParamDescription.c_str());
        }
        LPS_TSE_LIST_UI_INFO psTSEListUIInfo = AddEmptyTSETestParam();
        psTSEListUIInfo->mpFristEditUI->SetReadOnly(false);
        mbIsInitTSEListCompleted = true;
    }

    void CCJGW_TSEConfigEditDlg::SaveTestSuiteItem()
    {
        if (NULL == msTreeNodeData.mpsItemTestConfig) return ;
        if (msTreeNodeData.mbIsTSETest)
        {
            msTreeNodeData.mpsItemTestConfig->mvTestConfigs.clear();
            //msTreeNodeData.mpsItemTestConfig->mbIsTest = msTreeNodeData.mpPrevTreeNodeUI->GetCheckBox()->GetCheck();
            s_tse_test_param sTSETestParam;
            for (std::vector<S_TSE_LIST_UI_INFO>::iterator it = mvTSEListUIInfo.begin();it != mvTSEListUIInfo.end();++it)
            {
                sTSETestParam.meParamType = E_PARAM_STRING;
                sTSETestParam.mParamName = it->mpFristEditUI->GetText().GetData();
                sTSETestParam.mParamValue = it->mpSecondRichEditUI->GetText().GetData();
                sTSETestParam.mParamDescription = it->mpThreeEditUI->GetText().GetData();
                if (!sTSETestParam.mParamName.empty()) msTreeNodeData.mpsItemTestConfig->mvTestConfigs.push_back(sTSETestParam);
            }
        }
        else
        {
            msTreeNodeData.mmapGlobalVariables->clear();
            for (std::vector<S_TSE_LIST_UI_INFO>::iterator it = mvTSEListUIInfo.begin();it != mvTSEListUIInfo.end();++it)
            {
                if (0 != it->mpFristEditUI->GetText().GetLength()) 
                    msTreeNodeData.mmapGlobalVariables->insert(std::make_pair<std::wstring,std::wstring>(it->mpFristEditUI->GetText().GetData(),it->mpSecondRichEditUI->GetText().GetData()));
            }
        }
    }

    void CCJGW_TSEConfigEditDlg::SaveTSEConfig()
    {
        SaveTestSuiteItem();
        std::wstring strTSEConfigFilePath = mpEditTips->GetText();
        if (strTSEConfigFilePath.empty())
        {
            MessageBox(m_hWnd,L"配置文件路径不能为空",L"Error",MB_ICONERROR);
            return ;
        }
        //strTSEConfigFilePath += L".1";
        CopyFile(strTSEConfigFilePath.c_str(),JGW_GetFormatWString(L"%s_%u",strTSEConfigFilePath.c_str(),JGW_GetTimeOfDay()).c_str(),FALSE);
        if (mcTSEFactoryConfig.SaveTSEFactoryConfig(JGW_W2A(strTSEConfigFilePath,CP_UTF8),mpHomeTreeNode))
        {
            MessageBox(m_hWnd,L"保存测试文件成功",L"Success",MB_OK);
            mbIsSaveTSEConfig = false;
            mpSaveTipsLab->SetText(L"");
        }
        else
        {
            MessageBox(m_hWnd,L"保存测试文件失败",L"Error",MB_ICONERROR);
        }
    }

    void CCJGW_TSEConfigEditDlg::OnEditMenuBtnClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(m_hWnd, &point);
        pMenu->Init(NULL,_T("edit_menu.xml"),point,&m_PaintManager,NULL);
    }
}
