#include "stdafx.h"
#include "JGW_Cef3MessageRouterHandler.h"
#include <include/cef_parser.h>
//#include "JGW_Cef3ApiHelp.h"
#include <JGW_FoundationFunc/vecfunc.h>

namespace JGW
{
     CCJGW_CriticalSectionLock JGW_Cef3MessageRouterHandler::mcCriticalSectionLock;
     std::vector<JsCallCppFuncBK> JGW_Cef3MessageRouterHandler::gvJsCallCppFuncBK;

	
#if 0
	const char kLocalHost[] = "localhost";
	const char kTestUrlPath[] = "/binding";
	const char kTestHost[] = "tests";

	bool IsTestURL(const std::string& url, const std::string& path) {
		CefURLParts parts;
		CefParseURL(url, parts);

		const std::string& url_host = CefString(&parts.host);
		if (url_host != kTestHost && url_host != kLocalHost)
			return false;

		const std::string& url_path = CefString(&parts.path);
		return url_path.find(path) == 0;
	}
#endif

	bool JGW_Cef3MessageRouterHandler::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id, const CefString& request, bool persistent, CefRefPtr<Callback> callback)
	{
#if 1
        std::wstring strResponse;
        const std::wstring& url = frame->GetURL();
        CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
        for (std::vector<JsCallCppFuncBK>::iterator it = gvJsCallCppFuncBK.begin();it != gvJsCallCppFuncBK.end();it++)
        {
            int result = it[0](url,request,strResponse);
            if (1 == result)
            {
                callback->Success(strResponse);
                return true;
            }
            else if (result < 0)
            {
                callback->Failure(result,strResponse);
                return true;
            }
        }
        return false;
		//// Only handle messages from the test URL.
		//const std::string& url = frame->GetURL();
		////if (!IsTestURL(url, kTestUrlPath))
		////	return false;
		//if (request == "DialogTest.FileOpen")
		//{
		//	// Reverse the string and return.
		//	callback->Success("My Open Dialog");
		//	return true;
		//}
  //      callback->Failure(-5,"Error Query");

		//return true;
#else
		// Only handle messages from the test URL.
		const std::wstring& url = frame->GetURL().ToWString();
		const _UrlCallCppBkInterfaceMap& gUrlCallCppBkInterfaceMap = JGW_Cef3ApiHelp::GetInstance()->GetUrlCallCppBkInterfaceMap();
		_UrlCallCppBkInterfaceMap::const_iterator it = gUrlCallCppBkInterfaceMap.find(url);
		std::wstring strResponse;

		if (it != gUrlCallCppBkInterfaceMap.end())
		{
			E_CallCppBk_Result res = it->second->OnExecute(request.ToWString(), strResponse);
			switch (res)
			{
			case E_CallCppBk_Success:
				// Reverse the string and return.
				callback->Success(strResponse);
				return true;
			case E_CallCppBk_Failure:
				callback->Failure(-1, strResponse);
				return true;
			default:
				break;
			}

		}
		return false;
#endif
	}

    void JGW_Cef3MessageRouterHandler::RegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK)
    {
        CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
        if ( -1 == find_if(gvJsCallCppFuncBK,std::bind2nd(std::equal_to<JsCallCppFuncBK>(), pJsCallCppFuncBK)) )
        {
            gvJsCallCppFuncBK.push_back(pJsCallCppFuncBK);
        }
    }

    void JGW_Cef3MessageRouterHandler::UnRegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK)
    {
        CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
        std::vector<JsCallCppFuncBK>::iterator it = std::find_if(gvJsCallCppFuncBK.begin(), gvJsCallCppFuncBK.end(),std::bind2nd(std::equal_to<JsCallCppFuncBK>(), pJsCallCppFuncBK));
        if (it != gvJsCallCppFuncBK.end())
        {
            gvJsCallCppFuncBK.erase(it);
        }
    }
}
