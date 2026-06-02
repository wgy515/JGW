#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <iphlpapi.h>
#include "CTSEAPS_UpgradeDefine.h"
#include <map>

#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
namespace JGW
{
    class CCJGW_APSUpgradeConfigDlg : public CCUiLibWnd
    {
    public:
        CCJGW_APSUpgradeConfigDlg(HWND hParentHwnd,LPS_APS_MULTI_UPGRADE_CONFIG psAPSMultiUpgradeConfig,PIP_ADAPTER_INFO pTempIpAdapterInfo);
        ~CCJGW_APSUpgradeConfigDlg(void);
    public:
        int DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_APSUpgradeConfigDlg);
        void				OnInitWindow();
        virtual UINT		GetClassStyle() const;
        void				OnNotify(TNotifyUI& msg);
        void				OnCloseWindow();
        void                AddDefaultList();
    private:
        bool OnSetMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
        bool OnChangeMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
        CCJGW_ProcessPipe* GetProcessPipe();
        bool ReadCMDProcessPipeEOF(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec  = 10 );
    private:
        PIP_ADAPTER_INFO    mpIpAdapterInfo;
        HWND				m_hParentHwnd;
        CButtonUI*          mpOkOptionUI;
        CButtonUI*          mpCancelOptionUI;
        CEditUI*            mpHostIPAddrEdt;
        CEditUI*            mpUpgradeVersionEdt;
        CEditUI*            mpUpgradeTimeEdt;
        CEditUI*            mpDownloadLogPathEdt;
        //CButtonUI*          mpSelectDownloadLogPathButton;
        CCheckBoxUI*        mpSaveDownloadLogComCheckBox;
        CListTestCtrlUI*    mpMainList;
        LPS_APS_MULTI_UPGRADE_CONFIG mpsAPSMultiUpgradeConfig;
        std::map<CControlUI*,bool> mmapCheckEditRegexResult;
        std::map<CControlUI*,std::wstring> mmapApdaters;
        CCJGW_ProcessPipe mProcessPipe;
    };
}