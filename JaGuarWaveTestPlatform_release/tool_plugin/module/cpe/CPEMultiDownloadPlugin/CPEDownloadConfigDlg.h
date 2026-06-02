#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>

#include "MultiDownloadDefine.h"

#define UILIB_CONTROL_CLASS_MEMBER(class,name)\
    class* mp##name

#define UILIB_GET(name) mp##name

#define UILIB_GETCONTROLPTR(class,name) \
    (NULL != (mp##name = static_cast<class*>(m_PaintManager.FindControl(_T(#name)))));

namespace JGW
{
    class CCPEDownloadConfigDlg : public CCUiLibWnd
    {
    public:
        CCPEDownloadConfigDlg(HWND hParentHwnd,S_MULTI_DOWNLOAD_CONFIG& sMultiDownloadConfig);
        ~CCPEDownloadConfigDlg(void);
    public:    
        int DoModal(int uiDevicePoolThreadID);
    private:
        UILIB_GetWindowClassName(CCJGW_APSUpgradeConfigDlg);
        void				OnInitWindow();
        virtual UINT		GetClassStyle() const;
        void				OnNotify(TNotifyUI& msg);
        void				OnCloseWindow();
    private:
        bool GetDuiContronlClass();

        void InitDownloadConfigDlgToConfig();

        void UpdateConfigToDownloadConfigDlg();
		//! 点击排序按钮
		bool OnMsgSortBtnClick( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
		//! 取消排序
		void CancelSort();
		//! 更新排序LIST index: -1 更新所有
		void UpdateSortListView(int index = -1);
		//! 接收响应得消息
		bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //!
        void SaveSortListView(int index);
    private:
        HWND mhParentHwnd;
		int mnCurrentSortIndex;
		int muiDevicePoolThreadID;
        //! ui control ptr
        //UILIB_CONTROL_CLASS_MEMBER(CEditUI,CPEHostIPAddrEdt);
        UILIB_CONTROL_CLASS_MEMBER(CEditUI,TftpdIpAddrEdt);
        //UILIB_CONTROL_CLASS_MEMBER(CEditUI,SblEdt);
        //UILIB_CONTROL_CLASS_MEMBER(CEditUI,CdtEdt);
        //UILIB_CONTROL_CLASS_MEMBER(CEditUI,TzEdt);
        //UILIB_CONTROL_CLASS_MEMBER(CEditUI,DevEdt);
        //UILIB_CONTROL_CLASS_MEMBER(CEditUI,RpmEdt);
        //UILIB_CONTROL_CLASS_MEMBER(CEditUI,BootEdt);
        UILIB_CONTROL_CLASS_MEMBER(CEditUI,DownloadFileParamEdt);
        UILIB_CONTROL_CLASS_MEMBER(CListTestCtrlUI,DomainList);
        UILIB_CONTROL_CLASS_MEMBER(CButtonUI,SaveBtn);
        UILIB_CONTROL_CLASS_MEMBER(CButtonUI,CancelBtn);
        UILIB_CONTROL_CLASS_MEMBER(CButtonUI,ClearAllSortBtn);
        UILIB_CONTROL_CLASS_MEMBER(CButtonUI,CancelSortBtn);
        UILIB_CONTROL_CLASS_MEMBER(CEditUI,DownloadTimeEdt);
        UILIB_CONTROL_CLASS_MEMBER(CEditUI,DownloadTimeOutEdt);

        S_MULTI_DOWNLOAD_CONFIG& msMultiDownloadConfig;
        /*CEditUI* mpCPEHostIPAddrEdt;
        CEditUI* mpTftpdIpAddrEdt;
        CEditUI* mpSblEdt;
        CEditUI* mpCdtEdt;
        CEditUI* mpTzEdt;
        CEditUI* mpDevEdt;
        CEditUI* mpRpmEdt;
        CEditUI* mpBootEdt;
        CListTestCtrlUI* mpDomainList;
        CButtonUI* mpSaveBtn;
        CButtonUI* mpCancelBtn;
        CButtonUI* mpSortBtn;
        CButtonUI* mpCancelSortBtn;
        CEditUI* mpDownloadTimeEdt;
        CEditUI* mpDownloadTimeOutEdt;*/

    };
}


