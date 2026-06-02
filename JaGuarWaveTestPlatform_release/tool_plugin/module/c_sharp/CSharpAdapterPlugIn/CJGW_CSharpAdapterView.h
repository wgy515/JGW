#pragma once
#include "../../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../../platform_include/CJGWUiLibViewResolver_Define.h"

namespace JGW
{
    class CCJGW_CSharpAdapterView : public CCJGWUiLibViewResolverImpl
    {
    public:
        CCJGW_CSharpAdapterView(void);
        ~CCJGW_CSharpAdapterView(void);
    private:
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);
        void OnNotifySubWndPlugin(LPVOID pMsg);
        void CloseSubWndPlugin();
    private:
        void OnPaint(const WPARAM& wParam,const LPARAM& lParam);
        void ReSizeCSharpAdapterHwnd();
        void CloseCSharpAdapterHwnd();
        void OnCsharpAdapterViewReportHwnd(const WPARAM& wParam,const LPARAM& lParam);
    private:
        HWND mhCSharpAdapterExecWindowHandle;
        LPS_VIEW_RESOLVER_PARAM mpViewResolverParam;
    };
}
