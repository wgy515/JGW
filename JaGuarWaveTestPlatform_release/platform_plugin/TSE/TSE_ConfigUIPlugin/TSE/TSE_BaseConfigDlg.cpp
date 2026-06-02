#include "StdAfx.h"
#include "TSE_BaseConfigDlg.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <algorithm>

#include "CJGW_FTM_Json.h"

namespace JGW
{
    CTSE_BaseConfigDlg::CTSE_BaseConfigDlg(HWND hParentHwnd,std::vector<S_TSE_BASE_ITEM>& vTSEBaseItems) : m_hParentHwnd(hParentHwnd),mvTSEBaseItems(vTSEBaseItems)
    {
    }


    CTSE_BaseConfigDlg::~CTSE_BaseConfigDlg(void)
    {
    }

    void CTSE_BaseConfigDlg::OnCloseWindow()
    {
        mvBrowseButton.clear();
    }

    UINT CTSE_BaseConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }
    //! 默认100
    int CTSE_BaseConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("tse_base_config.xml"),_T("JaGuarWave TSE Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("tse_base_config.xml"),_T("JaGuarWave TSE Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("tse_base_config.xml"),_T("JaGuarWave TSE Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        ResizeClient(600,100 + mvTSEBaseItems.size() * 30);

        return ShowModal();
    }

    void CTSE_BaseConfigDlg::OnInitWindow()
    {
        bool bSucceseful = true;
        mpBaseVerticalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"BaseVerticalLayout"));
        mpSaveBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"OkBtn"));
        mpCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"CancelBtn"));
        bSucceseful &= (NULL != mpBaseVerticalLayout);
        bSucceseful &= (NULL != mpSaveBtn);
        bSucceseful &= (NULL != mpCancelBtn);
        if (!bSucceseful)
        {
            Close(IDCANCEL);
            return ;
        }

        InitBaseLayout();
    }

    void CTSE_BaseConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpSaveBtn == msg.pSender)
            {
                SaveUIInfo();
                CTSE_BaseConfigXML baseConfigXml(mvTSEBaseItems);
                baseConfigXml.SaveLanSuiteConfig(JGW_W2A(GetGlobalEnvironment()->GetString(TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH)));
                Close(IDOK);
            }
            else if (mpCancelBtn == msg.pSender)
            {
                Close(IDCANCEL);
            }
            if (mvBrowseButton.end() != std::find_if(mvBrowseButton.begin(),mvBrowseButton.end(),[&](CButtonUI *value){ return msg.pSender == value;}))
                //if (mvBrowseButton.end() != std::find_if(mvBrowseButton.begin(),mvBrowseButton.end(),[&](CButtonUI *value) => msg.pSender == value;))
            {
                OnRspBrowseButtonClickMsg((CButtonUI *)msg.pSender);
            }
        }
    }

    void CTSE_BaseConfigDlg::InitBaseLayout()
    {
        CHorizontalLayoutUI* lplsitContainerItem = NULL;
        mvBrowseButton.clear();
        for (size_t i = 0;i < mvTSEBaseItems.size();i ++)
        {
            //! FILE_PATH 文件路径标识 末尾标识位FILE_PATH
            if (mvTSEBaseItems[i].mstrValueEnvironment.length() - strlen("FILE_PATH") == mvTSEBaseItems[i].mstrValueEnvironment.rfind(L"FILE_PATH"))
            {
                CDialogBuilder builder1;
                lplsitContainerItem = (CHorizontalLayoutUI*)(builder1.Create(_T("tse_base_ui_file_path.xml"),(UINT)0,NULL,&m_PaintManager));
                if( !lplsitContainerItem ) continue;
                if( lplsitContainerItem != NULL ) mpBaseVerticalLayout->Add(lplsitContainerItem);

                lplsitContainerItem->GetItemAt(1)->SetText(mvTSEBaseItems[i].mstrLabelName.c_str());
                CEditUI* pEdit = (CEditUI*)(lplsitContainerItem->GetItemAt(2));
                pEdit->SetText(GetGlobalEnvironment()->GetString(mvTSEBaseItems[i].mstrValueEnvironment).c_str());
                pEdit->SetDate((LPVOID)&mvTSEBaseItems[i]);

                std::wstring strTemp;
                JGW_FormatWString(strTemp,L"TSE_BASE_CONFIG_ITEM_%d",i + 1);
                CButtonUI *pButtonUI = (CButtonUI *)(lplsitContainerItem->GetItemAt(4));
                pButtonUI->SetDate(pEdit);
                pButtonUI->SetName(strTemp.c_str());
                mvBrowseButton.push_back(pButtonUI);
            }
            else
            {
                CDialogBuilder builder1;
                lplsitContainerItem = (CHorizontalLayoutUI*)(builder1.Create(_T("tse_base_item.xml"),(UINT)0,NULL,&m_PaintManager));
                if( !lplsitContainerItem ) continue;
                if( lplsitContainerItem != NULL ) mpBaseVerticalLayout->Add(lplsitContainerItem);

                lplsitContainerItem->GetItemAt(1)->SetText(mvTSEBaseItems[i].mstrLabelName.c_str());
                CEditUI* pEdit = (CEditUI*)(lplsitContainerItem->GetItemAt(2));
				if (mvTSEBaseItems[i].mstrRateValue.empty())
				{
					pEdit->SetText(GetGlobalEnvironment()->GetString(mvTSEBaseItems[i].mstrValueEnvironment).c_str());
				}
				else
				{
					std::wstring strValue = GetGlobalEnvironment()->GetString(mvTSEBaseItems[i].mstrValueEnvironment);
					__int64 dw64Rate = _ttoi64(mvTSEBaseItems[i].mstrRateValue.c_str());
					if (std::wstring::npos == strValue.find(L"~"))
					{
						__int64 dw64Value = _ttoi64(strValue.c_str());
						pEdit->SetText(JGW_GetFormatWString(L"%f",(double)dw64Value / dw64Rate).c_str());
					}
					else
					{
						std::vector<std::wstring> vstrTemp;
						JGW_ParserStrW(strValue.c_str(),L"~",vstrTemp);
						strValue = L"";
						for (std::vector<std::wstring>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
						{
							__int64 dw64Temp = _ttoi64(it->c_str());
							if (strValue.empty())
							{
								strValue += JGW_GetFormatWString(L"%f",(double)dw64Temp / dw64Rate);
							}
							else
							{
								strValue += JGW_GetFormatWString(L"~%f",(double)dw64Temp / dw64Rate);
							}
						}
						pEdit->SetText(strValue.c_str());
					}
				}
                pEdit->SetDate((LPVOID)&mvTSEBaseItems[i]);
            }
        }
    }

    void CTSE_BaseConfigDlg::SaveUIInfo()
    {
        CHorizontalLayoutUI* lplsitContainerItem = NULL;
        for (int i = 0;i < mpBaseVerticalLayout->GetCount();i ++)
        {
            lplsitContainerItem = (CHorizontalLayoutUI*)mpBaseVerticalLayout->GetItemAt(i);
            if( !lplsitContainerItem ) continue;
            CEditUI* pEdit = (CEditUI*)(lplsitContainerItem->GetItemAt(2));
			LPS_TSE_BASE_ITEM psTSEBaseItem = (LPS_TSE_BASE_ITEM)pEdit->GetDate();
			if (NULL != psTSEBaseItem)
			{
				if (psTSEBaseItem->mstrRateValue.empty())
				{
					GetGlobalEnvironment()->PutString(psTSEBaseItem->mstrValueEnvironment,pEdit->GetText().GetData());
				}
				else
				{
					__int64 dw64Rate = _ttoi64(psTSEBaseItem->mstrRateValue.c_str());
					std::wstring strValue = pEdit->GetText().GetData();
					if (std::wstring::npos == strValue.find(L"~"))
					{
						double dw64Value = _ttof(strValue.c_str());
						GetGlobalEnvironment()->PutInt64(psTSEBaseItem->mstrValueEnvironment,(__int64)(dw64Value * dw64Rate));
					}
					else
					{
						std::vector<std::wstring> vstrTemp;
						JGW_ParserStrW(strValue.c_str(),L"~",vstrTemp);
						strValue = L"";
						for (std::vector<std::wstring>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
						{
							double dw64Temp = _ttof(it->c_str());
							dw64Temp *= dw64Rate;
							if (strValue.empty())
							{
								strValue += JGW_GetFormatWString(L"%I64d",(__int64)dw64Temp);
							}
							else
							{
								strValue += JGW_GetFormatWString(L"~%I64d",(__int64)dw64Temp);
							}
						}
						GetGlobalEnvironment()->PutString(psTSEBaseItem->mstrValueEnvironment,strValue);
					}
				}	
			}    
        }
    }

    void CTSE_BaseConfigDlg::OnRspBrowseButtonClickMsg(CButtonUI *pBrowseButton)
    {
        CEditUI *pEdit = (CEditUI *)pBrowseButton->GetDate();
        std::wstring strFormat = GetGlobalEnvironment()->GetString(pBrowseButton->GetName().GetData());
        bool bIsFolder = false;
        std::wstring strFilter;
        //! 解析配置参数:{&quot;IsFolder&quot;:&quot;false&quot;,&quot;Filter&quot;:&quot;excel File(*.xls,*.xlsx)\0*.xlsx;*.xls\0all file(*.*)\0*.*\0\0&quot;}
        if (!GetBrowseConfigToJson(strFormat,bIsFolder,strFilter))
        {
            MessageBox(GetHWND(),L"Browse Config Json Unable to resolved",L"Error",MB_ICONERROR);
            return ;
        }

        pEdit->SetText(L"");
        if (bIsFolder)
        {
            std::wstring strFolder;
            if (JGW_ChooseFolder(strFolder,GetHWND()))
            {
                pEdit->SetText(strFolder.c_str());
            }
            return ;
        }
        // 将过滤信息解析成WIN API识别的过滤信息
        wchar_t strFilterW[260] =  L"all file(*.*)\0*.*\0\0";
        if (!strFilter.empty())
        {
            size_t len = 0;
            for (size_t index = 0;index < strFilter.length() - 1;)
            {
                if (L'\\' == strFilter[index] && L'0' == strFilter[index + 1])
                {
                    strFilterW[len++] = L'\0';
                    index += 2;
                }
                else
                {
                    strFilterW[len++] = strFilter[index];
                    index++;
                }
            }
        }
        //! 选择文件
        std::wstring strFile;
        if (JGW_ChooseFile(strFile,strFilterW,GetHWND()))
        {
            pEdit->SetText(strFile.c_str());
        }
    }

    bool CTSE_BaseConfigDlg::GetBrowseConfigToJson(const std::wstring& strJson,bool &bIsFolder,std::wstring& strFilter)
    {
        CCJGW_FTM_Json ftmJson;
        std::string strTemp;
        std::wstring strTempW(strJson);
        JGW_ReplaceStringW(strTempW,L"\\",L"\\\\");

        if (!ftmJson.FromJsonToString(JGW_W2A(strTempW,CP_UTF8)))
        {
            return false;
        }

        ftmJson.GetJsonValueToKey<bool>("IsFolder",bIsFolder);
        //ftmJson.GetJsonValueToKey<bool>("IsMultiSelectFile",bIsMultiSelectFile);
        ftmJson.GetJsonValueToKey<std::string>("Filter",strTemp);
        strFilter = JGW_A2W(strTemp,CP_UTF8);

        return true;
    }
}