#pragma once
#include <UiLib/UIlib.h>
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include "CTSECalConfigDefine.h"
#include "CTSEGuCalBase.h"
#include <sstream>

#define FindUIPtr(ControlName,CControl) mp##ControlName = static_cast<CControl*>(mpPluginPaintManagerUI->FindControl(_T(#ControlName)))
#define DefUIPtr(ControlName,CControl) CControl* mp##ControlName
#define GetUIPtr(ControlName) mp##ControlName


namespace JGW
{
    class CCTSECalConfigDlg : public CCJGWUiLibViewResolverImpl
    {
    public:
        CCTSECalConfigDlg(void);
        ~CCTSECalConfigDlg(void);
    private:
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();

        bool InitGuCalConfig();
    private:
        bool GetDuiContronlClass();

        void OnStartCalClickMsg();

        void OnTestPassMsg(const WPARAM& wParam,const LPARAM& lParam);

        void OnTestFailMsg(const WPARAM& wParam,const LPARAM& lParam);

        void OnSaveCalClickMsg();

        void OnImportClickMsg();

        std::wstring GetSingleTestTime(double dTimer);

        void OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam);

        void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);

        void OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam);

        void OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam);

        void OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam);

        void OnRspTimeMsg(const WPARAM& wParam,const LPARAM& lParam);

        void OnRspSaveGuCalDataResult(const WPARAM& wParam,const LPARAM& lParam);

        void OnTSEAdapterLog(const WPARAM& wParam,const LPARAM& lParam);
    private:
        bool mbIsRunTest;
        //! 
        size_t mdwTestTime;
        //! 参数试图信息
        LPS_VIEW_RESOLVER_PARAM mpViewResolverParam;
        //! 
        CPaintManagerUI* mpPluginPaintManagerUI;
        //! UI PTR
        DefUIPtr(CalConfigTestTimeBtn,CButtonUI);
        DefUIPtr(CalConfigStartBtn,CButtonUI);
        DefUIPtr(CalConfigSaveBtn,CButtonUI);
        DefUIPtr(CalConfigConfigBtn,CButtonUI);
        DefUIPtr(CalConfigDomainlist,CListTestCtrlUI);
        DefUIPtr(CalConfigTestLogEdt,CRichEditUI);
        DefUIPtr(CalConfigHorizontalLayout,CHorizontalLayoutUI);
        DefUIPtr(CalTestStatusBtn,CButtonUI);
        DefUIPtr(ExportGuConfigConfigBtn,CButtonUI);
        

        CCMessageThread* mpTestThread;
        CCTSEGuCalBase* mpTSEGuCalBase;
        std::wostringstream mstrTestLogStream;
        CCJGW_CHiPerfTimer	m_cHiperTime;
        S_TSE_CALCONFIG_TEST_CONFIG_PARAM msTSECalConfigTestConfig;
    };

}

