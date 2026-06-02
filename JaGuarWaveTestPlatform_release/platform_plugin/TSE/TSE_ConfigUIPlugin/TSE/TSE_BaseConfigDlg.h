#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <TSE_FoundationLib/TSE_BaseConfigXML.h>
namespace JGW
{
    class CTSE_BaseConfigDlg : public CCUiLibWnd
    {
    public:
        CTSE_BaseConfigDlg(HWND hParentHwnd,std::vector<S_TSE_BASE_ITEM>& vTSEBaseItems);
        ~CTSE_BaseConfigDlg(void);
    public:
        int					DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_MESConfigDlg);
        void				OnInitWindow();
        virtual UINT		GetClassStyle() const;
        void				OnNotify(TNotifyUI& msg);
        void				OnCloseWindow();
        void                InitBaseLayout();
        void                SaveUIInfo();
    private:
        void OnRspBrowseButtonClickMsg(CButtonUI *pBrowseButton);
        bool GetBrowseConfigToJson(const std::wstring& strJson,bool &bIsFolder,std::wstring& strFilter);
    private:
        HWND m_hParentHwnd;
        CVerticalLayoutUI* mpBaseVerticalLayout;
        CButtonUI* mpSaveBtn;
        CButtonUI* mpCancelBtn;
        std::vector<S_TSE_BASE_ITEM>& mvTSEBaseItems;
        std::vector<CButtonUI *> mvBrowseButton;
    };
}