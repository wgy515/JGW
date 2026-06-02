#pragma once
#include "HelpPackageUIInterface.h"
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    typedef enum
    {
        E_OTA_UPDATE_SELECT_INSTALLVERSION_ACTION_ID = 0,
        E_OTA_UPDATE_DOWNLOAD_OTA_ACTION_ID = 1,
        E_OTA_UPDATE_OTA_ACTION_ID = 2,
        E_OTA_ADD_OTA_ACTION_ID = 3
    }E_OTA_ASYNC_ACTION_ID;

    typedef struct
    {
        int mnInstallVersionID;
        int mnOTAID;
        std::wstring mstrInstallVersion;
        std::wstring mstrVersion;
        std::wstring mstrFtpUrl;
        std::wstring mstrLocalUpdateOTAFolder;
        std::wstring mstrLocalUpdateOTAZipPath;
        std::wstring mstrUpdateOTAVersion;
        std::wstring mstrUpdateOTAName;
        std::wstring mstrUpdateOTADes;
        std::wstring mstrUpdateFTPUrl;
        std::vector<S_OTA> mvsOTA;
    }S_OTA_UPDATE_INFO;

    typedef struct
    {
        int mnInstallVersionID;
        std::wstring mstrInstallVersion;
        std::wstring mstrOTAName;
        std::wstring mstrOTAVersion;
        std::wstring mstrOTADes;
        std::wstring mstrLocalOTAFolder;
        std::wstring mstrLocalOTAPackageZipPath;
        std::wstring mstrUploadFTPUrl;
    }S_OTA_ADD_INFO;

    class CCJGW_HelpOTADlg : public CHelpPackageUIInterface
    {
    public:
        CCJGW_HelpOTADlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig);
        ~CCJGW_HelpOTADlg(void);
    private:
        //! 初始化UI
        bool InitHelpPackageUI(CPaintManagerUI *pPaintManagerUI);
        //! 响应UI点击事件通知
        void OnNotifyHelpPackageUI(TNotifyUI& msg);
        //! 关闭UI事件回调
        void CloseHelpPackageUI();
        //! 处理客户事件消息
        void OnHandleEventCustomMessage(TEventUI* pTEventUI);
        //! 处理异步函数调用消息
        bool OnHandleAsyncFuncMsg(int nActionIndex);
    private:
        void OnRspOperationCompleteMsg(E_OTA_ASYNC_ACTION_ID eActionIndex,bool result);
        bool OnRspOTAUpdateInstallVersionComboItemSelectMsg();
        bool OnRspOTAUpdateDownloadOTABtnClickMsg();
        bool OnRspOTAUpdateOtaBtnClickMsg();
        bool OnRspOTAAddOtaBtnClickMsg();
    private:
        MEMBER_VARIABLE_UI(CComboBoxUI,OTAInstallVersionCombo);
        MEMBER_VARIABLE_UI(CComboBoxUI,OTAOTAVersionCombo);
        MEMBER_VARIABLE_UI(CButtonUI,OTADownloadOTABtn);
        MEMBER_VARIABLE_UI(CEditUI,OTAOTAFolderEdt);
        MEMBER_VARIABLE_UI(CEditUI,OTAVersionEdit);
        MEMBER_VARIABLE_UI(CEditUI,OTANameEdit);
        MEMBER_VARIABLE_UI(CEditUI,OTADesEdit);
        MEMBER_VARIABLE_UI(CButtonUI,UpdateOTABtn);

        MEMBER_VARIABLE_UI(CComboBoxUI,AddOTAInstallVersionCombo);
        MEMBER_VARIABLE_UI(CButtonUI,AddOTAFolderBtn);
        MEMBER_VARIABLE_UI(CEditUI,AddOTAFolderEdt);
        MEMBER_VARIABLE_UI(CEditUI,AddOTAVersionEdit);
        MEMBER_VARIABLE_UI(CEditUI,AddOTANameEdit);
        MEMBER_VARIABLE_UI(CEditUI,AddOTADesEdit);
        MEMBER_VARIABLE_UI(CButtonUI,UploadAddOTABtn);
    private:
        CPaintManagerUI *mpPaintManagerUI;
        S_OTA_UPDATE_INFO msOTAUpdateInfo;
        S_OTA_ADD_INFO msOTAAddInfo;
    };
}


