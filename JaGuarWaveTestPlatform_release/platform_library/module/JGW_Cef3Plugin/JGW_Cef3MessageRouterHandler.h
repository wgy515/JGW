#pragma once
#include <include/cef_client.h>
#include <include/cef_app.h>
#include <include/wrapper/cef_message_router.h>
#include <map>
#include <UiLibCef/UiLibCefExportManager.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>

namespace JGW
{
	//class CallJSBingFuncInterface
	//{
	//public:
	//	/**
	//	* C++调用JS回调接口
	//	*/
	//	virtual ();
	//};
	class JGW_Cef3MessageRouterHandler : public CefMessageRouterBrowserSide::Handler
	{
	public:
		virtual bool OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id, const CefString& request, bool persistent, CefRefPtr<Callback> callback)  OVERRIDE;
    public:
        static void RegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK);
        //! 
        static void UnRegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK);
    private:
        static CCJGW_CriticalSectionLock mcCriticalSectionLock;
        static std::vector<JsCallCppFuncBK> gvJsCallCppFuncBK;
	};
}
