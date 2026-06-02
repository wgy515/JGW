#pragma once
#include "CJGW_DeviceSortConfigImpl.h"
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "CJGW_DeviceQueueSortConfig.h"


#define UILIB_CONTROL_CLASS_MEMBER(class,name)\
class* mp##name

#define UILIB_GET(name) mp##name

#define UILIB_GETCONTROLPTR(class,name) \
    (NULL != (mp##name = static_cast<class*>(m_PaintManager.FindControl(_T(#name)))));

namespace JGW
{
    class CCJGW_DeviceQueueSortConfigDlg : public CCJGW_DeviceSortConfigImpl,public CCUiLibWnd
    {
    public:
        CCJGW_DeviceQueueSortConfigDlg(HWND hParentHwnd,const S_DEVICEQUEUE_PROGRAM_INFO& sDevoceQueueProgramInfo,std::wstring& strTFTPD32IPAddr,std::wstring& strDownloadFileConfig,CCJGW_DeviceQueueSortConfig& cDeviceQueueSortConfig);
        ~CCJGW_DeviceQueueSortConfigDlg(void);
    public:
        int DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_DeviceQueueSortConfigDlg);
        void OnInitWindow();
        virtual UINT GetClassStyle() const;
        void OnNotify(TNotifyUI& msg);
        virtual void OnHandleDeviceMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,const LPARAM& lParam);
    private:
        bool GetDuiContronlClass();
        void InitDownloadConfigDlgToConfig();
        std::wstring GetColumnNameToDeviceMode(E_DEVICE_MODE eDeviceMode);
        //! 点击排序按钮
        bool OnMsgSortBtnClick( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
        //! 取消排序
        void CancelSort();
        void UpdateConfigToDownloadConfigDlg();
        //! 更新排序LIST index: -1 更新所有
        void UpdateSortListView(int index = -1);
    private:
        void UpdateListContainerElementUI(int index,CListContainerElementUIEx* plsitContainerItem,S_DEVICESORT_CONFIG& sDeviceSortConfig);
        //!
        void SaveSortListView(int index);
    private:
        int mnCurrentSortIndex;
        int mnMaxSortIndex;
        S_DEVICEQUEUE_PROGRAM_INFO msDevoceQueueProgramInfo;
        HWND mhParentHwnd;
        std::wstring mstrSkinFile;

        std::wstring& mstrTFTPD32IPAddr;
        std::wstring& mstrDownloadFileConfig;
        CCJGW_DeviceQueueSortConfig& mcDeviceQueueSortConfig;
    private:
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
        UILIB_CONTROL_CLASS_MEMBER(CLabelUI,DownloadTimeLabel);
        UILIB_CONTROL_CLASS_MEMBER(CLabelUI,DownloadTimeOutLabel);
    };
}
