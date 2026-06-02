#include "StdAfx.h"
#include "CJGW_ReflectCef3V8Handler.h"
#include <include/cef_parser.h>
namespace JGW
{
    CCJGW_ReflectCef3V8Handler::CCJGW_ReflectCef3V8Handler(void)
    {
    }


    CCJGW_ReflectCef3V8Handler::~CCJGW_ReflectCef3V8Handler(void)
    {
    }

    void CCJGW_ReflectCef3V8Handler::clear()
    {
        if (!callback_map_.empty()) {
            CallbackMap::iterator it = callback_map_.begin();
            for (; it != callback_map_.end();) {
                if (it->second.first->IsSame(it->second.first))
                    callback_map_.erase(it++);
                else
                    ++it;
            }
        }
    }

    CallbackMap& CCJGW_ReflectCef3V8Handler::GetCallbackMap()
    {
        return callback_map_;
    }

    bool CCJGW_ReflectCef3V8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
    {
        CEF_REQUIRE_RENDERER_THREAD();
        LOG(INFO) << "CCJGW_ReflectCef3V8Handler::Execute.";
        LOG(INFO) << name;
        if (name == "reflect" || name == "async_reflect")
        {
            LOG(INFO) << name << "-" << arguments.size();
            if (arguments.size() == 2 && arguments[1]->IsFunction())
            {
                LOG(INFO) << name << "arguments[1]->IsFunction()" << arguments.size();
                bool bObj = arguments[0]->IsObject();
                bool bArr = arguments[0]->IsArray();
                bool bBuf = arguments[0]->IsArrayBuffer();

                //CefRefPtr<CefDictionaryValue> jsonObject(arguments[0]);
                CefRefPtr<CefV8Value> json = arguments[0];
                CefString v8Value = json->GetStringValue();

                //int type;
                CefString funcNm;

                CefRefPtr<CefValue> jsonObject = CefParseJSON(v8Value, JSON_PARSER_ALLOW_TRAILING_COMMAS);
                if (jsonObject->IsValid())
                {
                    CefRefPtr<CefDictionaryValue> dict = jsonObject->GetDictionary();
                    //type = dict->GetInt("type");
                    funcNm = dict->GetString("func");
                }

                CefRefPtr<CefV8Value> callback_func_ = arguments[1];
                CefRefPtr<CefV8Context> callback_context_ = CefV8Context::GetCurrentContext();

                // »º´æ
                int browser_id = callback_context_->GetBrowser()->GetIdentifier();
                callback_map_.insert(
                    std::make_pair(std::make_pair(funcNm, browser_id),
                    std::make_pair(callback_context_, arguments[1])));

                // notify browser to deal msg
                //TODO: doSomething() in native way
                std::wstring strMessageName(L"Renderer.");
                strMessageName += name.ToWString();
                CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(strMessageName);

                // Retrieve the argument list object.
                CefRefPtr<CefListValue> args = msg->GetArgumentList();

                // Populate the argument values.
                args->SetSize(1);
                args->SetString(0, v8Value);
                //args->SetString(2, funcNm);
                CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_BROWSER, msg);

                return true;
            }
        }

        // Function does not exist.
        return false;
    }
}
