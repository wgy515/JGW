#include "StdAfx.h"
#include "TSE_CSVConfigDlg.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <TSE_FoundationLib/TSE_BaseConfigXML.h>

namespace JGW
{
    CTSE_CSVConfigDlg::CTSE_CSVConfigDlg(HWND hParentHwnd,std::vector<S_TSE_CSV_CONFIG_ITEM>& vTSEBaseItems,const std::wstring& strCSVFilePathCommand,int nPrimaryColumnNameRowIndex,int nPrimaryColumnValueIndex,const std::wstring& strPrimaryValue) : m_hParentHwnd(hParentHwnd),mvsTSECSVConfigItems(vTSEBaseItems),mstrCSVFilePathCommand(strCSVFilePathCommand),mnPrimaryColumnNameRowIndex(nPrimaryColumnNameRowIndex),mnPrimaryColumnValueIndex(nPrimaryColumnValueIndex),mstrPrimaryValue(strPrimaryValue)
    {
    }


    CTSE_CSVConfigDlg::~CTSE_CSVConfigDlg(void)
    {
    }

    void CTSE_CSVConfigDlg::OnCloseWindow()
    {
        if (mbSave)
        {
            SaveUIInfo();
			if (!mvTSEBaseItems.empty())
			{
				CTSE_BaseConfigXML baseConfigXml(mvTSEBaseItems);
				baseConfigXml.SaveLanSuiteConfig(JGW_W2A(GetGlobalEnvironment()->GetString(TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH)));
			}
            mscvDoc.Save();
            mbSave = false;
        }
        else
        {
            bool bSaveCSV = false;
            CHorizontalLayoutUI* lplsitContainerItem = NULL;
            for (int i = 0;i < mpBaseVerticalLayout->GetCount();i ++)
            {
                lplsitContainerItem = (CHorizontalLayoutUI*)mpBaseVerticalLayout->GetItemAt(i);
                if( !lplsitContainerItem ) continue;
                CEditUI* pEdit = (CEditUI*)(lplsitContainerItem->GetItemAt(2));

                if (mvsTSECSVConfigItems[i].mbUpdateCsv)
                {
                    SaveColumnValueToColumnNames((LPCTSTR)pEdit->GetDate(),pEdit->GetText().GetData());
                    bSaveCSV = true;
                }  
            }
            if (bSaveCSV) mscvDoc.Save();
        }
    }

    UINT CTSE_CSVConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }
    //! 默认100
    int CTSE_CSVConfigDlg::DoModal()
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

        ResizeClient(600,100 + mvsTSECSVConfigItems.size() * 30);

        return ShowModal();
    }

    bool CTSE_CSVConfigDlg::LoadCSVInfo()
    {
        std::wstring strCSVFilePath (mstrCSVFilePathCommand);

        GetEnvironmentCommandString (strCSVFilePath);
        strCSVFilePath = JGW_RealativePathToAbsPath(strCSVFilePath.c_str());
        //! 检查CSV文件是否存在 
        if (!JGW_FileExistsToFilePath(strCSVFilePath.c_str()))
        {
            LogE_F(L"The file(%s) is not exist ",strCSVFilePath.c_str());
            mscvDoc.Clear();
            return false;
        }
        
        try
        {
            std::string strCSVFilePathA = JGW_W2A(strCSVFilePath,CP_UTF8);
            mscvDoc.Load(strCSVFilePathA,rapidcsv::LabelParams(mnPrimaryColumnNameRowIndex, mnPrimaryColumnValueIndex));
        }
        catch (...)
        {
            LogE_F("The file(%s) load fail ",strCSVFilePath.c_str());
        	return false;
        }
        return true;
    }

    void CTSE_CSVConfigDlg::OnInitWindow()
    {
        bool bSucceseful = true;
        mbSave = false;
		mvTSEBaseItems.clear();
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

        if (!LoadCSVInfo() || !InitBaseLayout())
        {
            MessageBox(GetHWND(),L"Init CSV File Fail",L"Error",MB_ICONERROR);
            Close(IDCANCEL);
            return ;
        }
    }

    void CTSE_CSVConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpSaveBtn == msg.pSender)
            {
#if 0
                SaveUIInfo();
                mscvDoc.Save();
#endif
                mbSave = true;
                Close(IDOK);
            }
            else if (mpCancelBtn == msg.pSender)
            {
                Close(IDCANCEL);
            }
        }
    }

    std::wstring CTSE_CSVConfigDlg::GetEnvironmentValueToEnvironmentNames(const std::wstring& strEnvironmentNames)
    {
        std::vector<std::string> vTemp;

        JGW_ParserStrA(JGW_W2A(strEnvironmentNames).c_str(),",",vTemp);
        if (vTemp.empty())
        {
            return L"";
        }

        try
        {
            std::string strPrimaryValue = JGW_W2A(mstrPrimaryValue);
            std::wstring strEnvironmentValue = JGW_A2W(mscvDoc.GetCell<std::string>(vTemp[0], strPrimaryValue));
            for (size_t index = 1;index < vTemp.size();index++)
            {
                strEnvironmentValue += L",";
                strEnvironmentValue += JGW_A2W(mscvDoc.GetCell<std::string>(vTemp[index], strPrimaryValue));
            }
            return strEnvironmentValue;
        }
        catch (...)
        {
            LogE(L"Get Column Environment Value Fail ..");
            Close(IDCANCEL);
        }

        return L"";
    }

    bool CTSE_CSVConfigDlg::InitBaseLayout()
    {
        CHorizontalLayoutUI* lplsitContainerItem = NULL;
        for (size_t i = 0;i < mvsTSECSVConfigItems.size();i ++)
        {
            CDialogBuilder builder1;
            lplsitContainerItem = (CHorizontalLayoutUI*)(builder1.Create(_T("tse_base_item.xml"),(UINT)0,NULL,&m_PaintManager));
            if( !lplsitContainerItem ) continue;
            if( lplsitContainerItem != NULL ) mpBaseVerticalLayout->Add(lplsitContainerItem);

            lplsitContainerItem->GetItemAt(1)->SetText(mvsTSECSVConfigItems[i].mstrLabelName.c_str());
            CEditUI* pEdit = (CEditUI*)(lplsitContainerItem->GetItemAt(2));
            pEdit->SetReadOnly(mvsTSECSVConfigItems[i].mbReadOnly);

            std::wstring strCSVValue = GetEnvironmentValueToEnvironmentNames(mvsTSECSVConfigItems[i].mstrEditValueEnvironment).c_str();
            if (mvsTSECSVConfigItems[i].mstrGlobalEnvironmentName.empty())
            {
                //pEdit->SetText(GetGlobalEnvironment()->GetString(mvTSEBaseItems[i].mstrValueEnvironment).c_str());
                pEdit->SetText(strCSVValue.c_str());
                //pEdit->SetDate((LPVOID)mvsTSECSVConfigItems[i].mstrEditValueEnvironment.c_str());
				pEdit->SetDate((LPVOID)(&mvsTSECSVConfigItems[i]));
            }
            else
            {
                std::wstring strGlobalEnvValue = GetGlobalEnvironment()->GetString(mvsTSECSVConfigItems[i].mstrGlobalEnvironmentName);
                if (0 != strCSVValue.compare(strGlobalEnvValue)) mvsTSECSVConfigItems[i].mbUpdateCsv = true;
                pEdit->SetText(strGlobalEnvValue.c_str());
                //pEdit->SetDate((LPVOID)mvsTSECSVConfigItems[i].mstrEditValueEnvironment.c_str());
				pEdit->SetDate((LPVOID)(&mvsTSECSVConfigItems[i]));
            }
        }
        return true;
    }

    void CTSE_CSVConfigDlg::SaveColumnValueToColumnNames(const std::wstring& strEnvironmentNames,const std::wstring& strEnvironmentValues)
    {
        try
        {
            std::vector<std::string> vTempNames;
            std::vector<std::string> vTempValues;

            JGW_ParserStrA(JGW_W2A(strEnvironmentNames).c_str(),",",vTempNames);
            JGW_ParserStrA(JGW_W2A(strEnvironmentValues).c_str(),",",vTempValues);
            while (vTempValues.size() < vTempNames.size()) vTempValues.push_back("");

            std::string strPrimaryValue = JGW_W2A(mstrPrimaryValue);
            for (size_t index = 0;index < vTempNames.size();index++)
            {
                mscvDoc.SetCell<std::string>(vTempNames[index], strPrimaryValue,vTempValues[index]);
            }
            
        }
        catch (...)
        {
            LogE(L"Set Column Environment Value Fail ..");
        }
    }

    void CTSE_CSVConfigDlg::SaveUIInfo()
    {
        CHorizontalLayoutUI* lplsitContainerItem = NULL;
        for (int i = 0;i < mpBaseVerticalLayout->GetCount();i ++)
        {
            lplsitContainerItem = (CHorizontalLayoutUI*)mpBaseVerticalLayout->GetItemAt(i);
            if( !lplsitContainerItem ) continue;
            CEditUI* pEdit = (CEditUI*)(lplsitContainerItem->GetItemAt(2));
#if 0
            GetGlobalEnvironment()->PutString((LPCTSTR)pEdit->GetDate(),pEdit->GetText().GetData());
            SaveColumnValueToColumnNames((LPCTSTR)pEdit->GetDate(),pEdit->GetText().GetData());
#else
			S_TSE_CSV_CONFIG_ITEM* psTseCsvConfigItem = (S_TSE_CSV_CONFIG_ITEM*)pEdit->GetDate();
			if (NULL != psTseCsvConfigItem)
			{
				if (!psTseCsvConfigItem->mstrGlobalEnvironmentName.empty())
				{
					GetGlobalEnvironment()->PutString(psTseCsvConfigItem->mstrGlobalEnvironmentName,pEdit->GetText().GetData());
					S_TSE_BASE_ITEM sTseBaseItem;
					//sTseBaseItem.mstrLabelName = psTseCsvConfigItem->mstrGlobalEnvironmentName;
					sTseBaseItem.mstrValueEnvironment = psTseCsvConfigItem->mstrGlobalEnvironmentName;
					mvTSEBaseItems.push_back(sTseBaseItem);
				}
				SaveColumnValueToColumnNames(psTseCsvConfigItem->mstrEditValueEnvironment,pEdit->GetText().GetData());
			}	
#endif
        }
    }
}
