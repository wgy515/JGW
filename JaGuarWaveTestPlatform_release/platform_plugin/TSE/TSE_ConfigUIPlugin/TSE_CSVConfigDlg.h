#pragma once
#include <string>
#include <vector>
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <TSE_FoundationLib/TSE_BaseConfigXML.h>
#include <JGW_FoundationFunc/rapidcsv.hpp>
#include "TSE_CSVConfigUI.h"

namespace JGW
{
    class CTSE_CSVConfigDlg : public CCUiLibWnd
    {
    public:
        CTSE_CSVConfigDlg(HWND hParentHwnd,std::vector<S_TSE_CSV_CONFIG_ITEM>& vTSEBaseItems,const std::wstring& strCSVFilePathCommand,int nPrimaryColumnNameRowIndex,int nPrimaryColumnValueIndex,const std::wstring& strPrimaryValue);
        ~CTSE_CSVConfigDlg(void);
    public:
        int					DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_MESConfigDlg);
        void				OnInitWindow();
        virtual UINT		GetClassStyle() const;
        void				OnNotify(TNotifyUI& msg);
        void				OnCloseWindow();
        bool                InitBaseLayout();
        void                SaveUIInfo();
    private:
        bool LoadCSVInfo();
        std::wstring GetEnvironmentValueToEnvironmentNames(const std::wstring& strEnvironmentNames);
        void SaveColumnValueToColumnNames(const std::wstring& strEnvironmentNames,const std::wstring& strEnvironmentValues);
    private:
        int mnPrimaryColumnNameRowIndex;
        //! 主键列索引
        int mnPrimaryColumnValueIndex;
        //! CSV 文件路径
        std::wstring mstrCSVFilePathCommand;
        //! 主键值
        std::wstring mstrPrimaryValue;
    private:
        bool mbSave;
        HWND m_hParentHwnd;
        CVerticalLayoutUI* mpBaseVerticalLayout;
        CButtonUI* mpSaveBtn;
        CButtonUI* mpCancelBtn;
		std::vector<S_TSE_BASE_ITEM> mvTSEBaseItems;
        std::vector<S_TSE_CSV_CONFIG_ITEM>& mvsTSECSVConfigItems;
        // csv doc instance
        rapidcsv::Document mscvDoc;
    };

}

