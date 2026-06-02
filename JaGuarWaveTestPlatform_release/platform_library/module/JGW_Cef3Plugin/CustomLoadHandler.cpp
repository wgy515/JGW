#include "StdAfx.h"
#include "CustomLoadHandler.h"
#include <include/cef_parser.h>
#include <include/wrapper/cef_helpers.h>
namespace JGW
{
    CustomLoadHandler::CustomLoadHandler(void)
    {
    }


    CustomLoadHandler::~CustomLoadHandler(void)
    {
    }

    void CustomLoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
    {

    }

    void CustomLoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)
    {
        HWND hWnd = browser->GetHost()->GetWindowHandle();
        if (hWnd) 
        {
            HICON hIcon = (HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(gnMainIconResId), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
            ::SendMessage(hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
            hIcon = (HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(gnMainIconResId), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
            ::SendMessage(hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
            //::SendMessage(hWnd, WM_SETICON, TRUE, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(107)));  // 设置大图标
            //::SendMessage(hWnd, WM_SETICON, FALSE, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(107))); // 设置小图标
        }
#if 0
        // 示例：在加载开始时禁用提交按钮
        DisableSubmitButton();
#endif
    }

    class TitleVisitor : public CefStringVisitor {
    public:
        void Visit(const CefString& strContent) override {
            // 解析HTML获取<title>标签内容
            std::wstring strTemp = strContent;
        }
        IMPLEMENT_REFCOUNTING(TitleVisitor);
    };

               


    void CustomLoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
    {
#if 0
        // 示例：加载完成后自动填充表单
        if (frame->IsMain() && http_status_code == 200) {
            AutoFillForm();
        }
#endif
        //browser->GetMainFrame()->GetSource(new TitleVisitor());
    }


    std::string GetErrorString(cef_errorcode_t code) {
        // Case condition that returns |code| as a string.
#define CASE(code) \
  case code:       \
  return #code

        switch (code) {
            CASE(ERR_NONE);
            CASE(ERR_FAILED);
            CASE(ERR_ABORTED);
            CASE(ERR_INVALID_ARGUMENT);
            CASE(ERR_INVALID_HANDLE);
            CASE(ERR_FILE_NOT_FOUND);
            CASE(ERR_TIMED_OUT);
            CASE(ERR_FILE_TOO_BIG);
            CASE(ERR_UNEXPECTED);
            CASE(ERR_ACCESS_DENIED);
            CASE(ERR_NOT_IMPLEMENTED);
            CASE(ERR_CONNECTION_CLOSED);
            CASE(ERR_CONNECTION_RESET);
            CASE(ERR_CONNECTION_REFUSED);
            CASE(ERR_CONNECTION_ABORTED);
            CASE(ERR_CONNECTION_FAILED);
            CASE(ERR_NAME_NOT_RESOLVED);
            CASE(ERR_INTERNET_DISCONNECTED);
            CASE(ERR_SSL_PROTOCOL_ERROR);
            CASE(ERR_ADDRESS_INVALID);
            CASE(ERR_ADDRESS_UNREACHABLE);
            CASE(ERR_SSL_CLIENT_AUTH_CERT_NEEDED);
            CASE(ERR_TUNNEL_CONNECTION_FAILED);
            CASE(ERR_NO_SSL_VERSIONS_ENABLED);
            CASE(ERR_SSL_VERSION_OR_CIPHER_MISMATCH);
            CASE(ERR_SSL_RENEGOTIATION_REQUESTED);
            CASE(ERR_CERT_COMMON_NAME_INVALID);
            CASE(ERR_CERT_DATE_INVALID);
            CASE(ERR_CERT_AUTHORITY_INVALID);
            CASE(ERR_CERT_CONTAINS_ERRORS);
            CASE(ERR_CERT_NO_REVOCATION_MECHANISM);
            CASE(ERR_CERT_UNABLE_TO_CHECK_REVOCATION);
            CASE(ERR_CERT_REVOKED);
            CASE(ERR_CERT_INVALID);
            CASE(ERR_CERT_END);
            CASE(ERR_INVALID_URL);
            CASE(ERR_DISALLOWED_URL_SCHEME);
            CASE(ERR_UNKNOWN_URL_SCHEME);
            CASE(ERR_TOO_MANY_REDIRECTS);
            CASE(ERR_UNSAFE_REDIRECT);
            CASE(ERR_UNSAFE_PORT);
            CASE(ERR_INVALID_RESPONSE);
            CASE(ERR_INVALID_CHUNKED_ENCODING);
            CASE(ERR_METHOD_NOT_SUPPORTED);
            CASE(ERR_UNEXPECTED_PROXY_AUTH);
            CASE(ERR_EMPTY_RESPONSE);
            CASE(ERR_RESPONSE_HEADERS_TOO_BIG);
            CASE(ERR_CACHE_MISS);
            CASE(ERR_INSECURE_RESPONSE);
        default:
            return "UNKNOWN";
        }
    }

    // Returns a data: URI with the specified contents.
    std::string GetDataURI(const std::string& data, const std::string& mime_type) {
        return "data:" + mime_type + ";base64," +
            CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
            .ToString();
    }

    // Load a data: URI containing the error message.
    void CustomLoadHandler::LoadErrorPage(CefRefPtr<CefFrame> frame,
        const std::string& failed_url,
        cef_errorcode_t error_code,
        const std::string& other_info) {
            std::stringstream ss;
            ss << "<html><head><title>Page failed to load</title></head>"
                "<body bgcolor=\"white\">"
                "<h3>Page failed to load.</h3>"
                "URL: <a href=\""
                << failed_url << "\">" << failed_url
                << "</a><br/>Error: " << GetErrorString(error_code) << " ("
                << error_code << ")";

            if (!other_info.empty())
                ss << "<br/>" << other_info;

            ss << "</body></html>";
            frame->LoadURL(GetDataURI(ss.str(), "text/html"));
    }

    void CustomLoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
    {
        CEF_REQUIRE_UI_THREAD();

        // Don't display an error for downloaded files.
        if (errorCode == ERR_ABORTED)
            return;

        // Don't display an error for external protocols that we allow the OS to
        // handle. See OnProtocolExecution().
        if (errorCode == ERR_UNKNOWN_URL_SCHEME) {
            std::string urlStr = frame->GetURL();
            if (urlStr.find("spotify:") == 0)
                return;
        }

        // Load the error page.
        LoadErrorPage(frame, failedUrl, errorCode, errorText);
    }
}
