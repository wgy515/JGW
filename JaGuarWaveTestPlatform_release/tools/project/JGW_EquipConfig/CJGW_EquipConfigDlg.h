#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "JGW_EquipConfig_Define.h"
#include <vector>

namespace JGW
{
    class CCJGW_EquipConfigDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_EquipConfigDlg(void);
        ~CCJGW_EquipConfigDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_EquipConfigDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 加载控制UI视图指针
        bool LoadControlUiClassPtr();
        //! 响应目标项目更改UI操作
        void OnChangeTargetProject();
        //! 加载目标项目UI视图
        void LoadTargetProjectView();
        //! 更改控制仪器视图
        void ChangeRemoteInstrumentView();
        //! 更新仪器配置到视图
        void UpdateMonitorInfoView();
        //! 响应保存仪器配置按钮
        void OnSaveEquipConfigBtn();
        //! 获取仪器相关控件信息到仪器配置 
        void GetEquipControlUiInfo();
    private:
        LPS_EQUIP_PROJECT_CONFIG mpsEquipPrjectConfig;

        CComboUI* mpTargetCombo;
        CButtonUI* mpSaveBtn;

        CComboUI*			m_pInstrumentCombo;
        CComboUI*           m_pRemoteInstrumentCombo;
        CLabelUI*           m_pboardidLab;
        CEditUI*			m_pboardidEdt;
        CLabelUI*           m_pPadLab;
        CEditUI*			m_pPadEdt;//! 仪器地址
        CComboUI*           m_pGpibTypeCombo;

        CLabelUI*           m_pVisaLab;
        CEditUI*            m_pVisaEdt;

        CLabelUI*           m_pIPLab;
        CEditUI*            m_pIPAddressEdt;
        CLabelUI*           m_pSourceLab;
        CEditUI*            m_pSourceAddressEdt;
        CLabelUI*           m_pSocketPortLab;
        CEditUI*            m_pSocketPortEdt;
        CLabelUI*           m_pSubInstrumentLab;
        CEditUI*            m_pSubInstrumentEdt;
        S_MONITOR_INFO      msMonitorInfo;
        
        std::vector<S_EQUIP_PROJECT_CONFIG> mvsEquipProjectConfig;
    };
}