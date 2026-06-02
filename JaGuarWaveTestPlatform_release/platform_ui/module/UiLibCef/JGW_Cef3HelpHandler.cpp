#include "stdafx.h"
#include "JGW_Cef3HelpHandler.h"
#include "JGW_ZipResResourceProvider.h"
#include "include/cef_app.h"
#include "include/base/cef_bind.h"
#include "include/cef_parser.h"
#include "JGW_Cef3MessageRouterHandler.h"
#include "client_switches.h"
#include <sstream>
#include <include\cef_x509_certificate.h>
#include <iostream>
#include <iomanip>
#define NEWLINE std::endl

#define FLAG(flag)                          \
    if (status & flag) {                      \
    result += std::string(#flag) + "<br/>"; \
    }

#define VALUE(val, def)       \
    if (val == def) {           \
    return std::string(#def); \
    }

//#include "JGW_CEF3ApiHelp.h"
namespace JGW
{
    // Replace all instances of |from| with |to| in |str|.
    std::string StringReplace(const std::string& str,
        const std::string& from,
        const std::string& to) {
            std::string result = str;
            std::string::size_type pos = 0;
            std::string::size_type from_len = from.length();
            std::string::size_type to_len = to.length();
            do {
                pos = result.find(from, pos);
                if (pos != std::string::npos) {
                    result.replace(pos, from_len, to);
                    pos += to_len;
                }
            } while (pos != std::string::npos);
            return result;
    }

    std::string GetTimeString(const CefTime& value) {
        if (value.GetTimeT() == 0)
            return "Unspecified";

        static const char* kMonths[] = {
            "January", "February", "March",     "April",   "May",      "June",
            "July",    "August",   "September", "October", "November", "December"};
            std::string month;
            if (value.month >= 1 && value.month <= 12)
                month = kMonths[value.month - 1];
            else
                month = "Invalid";

            std::stringstream ss;
            ss << month << " " << value.day_of_month << ", " << value.year << " "
                << std::setfill('0') << std::setw(2) << value.hour << ":"
                << std::setfill('0') << std::setw(2) << value.minute << ":"
                << std::setfill('0') << std::setw(2) << value.second;
            return ss.str();
    }

    std::string GetBinaryString(CefRefPtr<CefBinaryValue> value) {
        if (!value.get())
            return "&nbsp;";

        // Retrieve the value.
        const size_t size = value->GetSize();
        std::string src;
        src.resize(size);
        value->GetData(const_cast<char*>(src.data()), size, 0);

        // Encode the value.
        return CefBase64Encode(src.data(), src.size());
    }


    std::string GetCertStatusString(cef_cert_status_t status) {
        std::string result;

        FLAG(CERT_STATUS_COMMON_NAME_INVALID);
        FLAG(CERT_STATUS_DATE_INVALID);
        FLAG(CERT_STATUS_AUTHORITY_INVALID);
        FLAG(CERT_STATUS_NO_REVOCATION_MECHANISM);
        FLAG(CERT_STATUS_UNABLE_TO_CHECK_REVOCATION);
        FLAG(CERT_STATUS_REVOKED);
        FLAG(CERT_STATUS_INVALID);
        FLAG(CERT_STATUS_WEAK_SIGNATURE_ALGORITHM);
        FLAG(CERT_STATUS_NON_UNIQUE_NAME);
        FLAG(CERT_STATUS_WEAK_KEY);
        FLAG(CERT_STATUS_PINNED_KEY_MISSING);
        FLAG(CERT_STATUS_NAME_CONSTRAINT_VIOLATION);
        FLAG(CERT_STATUS_VALIDITY_TOO_LONG);
        FLAG(CERT_STATUS_IS_EV);
        FLAG(CERT_STATUS_REV_CHECKING_ENABLED);
        FLAG(CERT_STATUS_SHA1_SIGNATURE_PRESENT);
        FLAG(CERT_STATUS_CT_COMPLIANCE_FAILED);

        if (result.empty())
            return "&nbsp;";
        return result;
    }

    std::string GetSSLVersionString(cef_ssl_version_t version) {
        VALUE(version, SSL_CONNECTION_VERSION_UNKNOWN);
        VALUE(version, SSL_CONNECTION_VERSION_SSL2);
        VALUE(version, SSL_CONNECTION_VERSION_SSL3);
        VALUE(version, SSL_CONNECTION_VERSION_TLS1);
        VALUE(version, SSL_CONNECTION_VERSION_TLS1_1);
        VALUE(version, SSL_CONNECTION_VERSION_TLS1_2);
        VALUE(version, SSL_CONNECTION_VERSION_TLS1_3);
        VALUE(version, SSL_CONNECTION_VERSION_QUIC);
        return std::string();
    }

    std::string GetContentStatusString(cef_ssl_content_status_t status) {
        std::string result;

        VALUE(status, SSL_CONTENT_NORMAL_CONTENT);
        FLAG(SSL_CONTENT_DISPLAYED_INSECURE_CONTENT);
        FLAG(SSL_CONTENT_RAN_INSECURE_CONTENT);

        if (result.empty())
            return "&nbsp;";
        return result;
    }

    // Return HTML string with information about a certificate.
    std::string GetCertificateInformation(CefRefPtr<CefX509Certificate> cert,
        cef_cert_status_t certstatus) {
            CefRefPtr<CefX509CertPrincipal> subject = cert->GetSubject();
            CefRefPtr<CefX509CertPrincipal> issuer = cert->GetIssuer();

            // Build a table showing certificate information. Various types of invalid
            // certificates can be tested using https://badssl.com/.
            std::stringstream ss;
            ss << "<h3>X.509 Certificate Information:</h3>"
                "<table border=1><tr><th>Field</th><th>Value</th></tr>";

            if (certstatus != CERT_STATUS_NONE) {
                ss << "<tr><td>Status</td><td>" << GetCertStatusString(certstatus)
                    << "</td></tr>";
            }

            ss << "<tr><td>Subject</td><td>"
                << (subject.get() ? subject->GetDisplayName().ToString() : "&nbsp;")
                << "</td></tr>"
                "<tr><td>Issuer</td><td>"
                << (issuer.get() ? issuer->GetDisplayName().ToString() : "&nbsp;")
                << "</td></tr>"
                "<tr><td>Serial #*</td><td>"
                << GetBinaryString(cert->GetSerialNumber()) << "</td></tr>"
                << "<tr><td>Valid Start</td><td>" << GetTimeString(cert->GetValidStart())
                << "</td></tr>"
                "<tr><td>Valid Expiry</td><td>"
                << GetTimeString(cert->GetValidExpiry()) << "</td></tr>";

            CefX509Certificate::IssuerChainBinaryList der_chain_list;
            CefX509Certificate::IssuerChainBinaryList pem_chain_list;
            cert->GetDEREncodedIssuerChain(der_chain_list);
            cert->GetPEMEncodedIssuerChain(pem_chain_list);
            DCHECK_EQ(der_chain_list.size(), pem_chain_list.size());

            der_chain_list.insert(der_chain_list.begin(), cert->GetDEREncoded());
            pem_chain_list.insert(pem_chain_list.begin(), cert->GetPEMEncoded());

            for (size_t i = 0U; i < der_chain_list.size(); ++i) {
                ss << "<tr><td>DER Encoded*</td>"
                    "<td style=\"max-width:800px;overflow:scroll;\">"
                    << GetBinaryString(der_chain_list[i])
                    << "</td></tr>"
                    "<tr><td>PEM Encoded*</td>"
                    "<td style=\"max-width:800px;overflow:scroll;\">"
                    << GetBinaryString(pem_chain_list[i]) << "</td></tr>";
            }

            ss << "</table> * Displayed value is base64 encoded.";
            return ss.str();
    }

    namespace test_runner
    {
        std::string GetDataURI(const std::string& data, const std::string& mime_type) {
            return "data:" + mime_type + ";base64," +
                CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
                .ToString();
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

        // Create a resource response filter for tests.
        CefRefPtr<CefResponseFilter> GetResourceResponseFilter(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefResponse> response)
        {
            return nullptr;
        }

        void CreateMessageHandlers(JGW::MessageHandlerSet& handlers) 
        {
            //! JS请求调用C++函数回调
            handlers.insert(new JGW_Cef3MessageRouterHandler);
#if 0
            handlers.insert(new PromptHandler);

            // Create the binding test handlers.
            binding_test::CreateMessageHandlers(handlers);

            // Create the dialog test handlers.
            dialog_test::CreateMessageHandlers(handlers);

            // Create the drm test handlers.
            drm_test::CreateMessageHandlers(handlers);

            // Create the media router test handlers.
            media_router_test::CreateMessageHandlers(handlers);

            // Create the preferences test handlers.
            preferences_test::CreateMessageHandlers(handlers);

            // Create the server test handlers.
            server_test::CreateMessageHandlers(handlers);

            // Create the urlrequest test handlers.
            urlrequest_test::CreateMessageHandlers(handlers);

            // Create the window test handlers.
            window_test::CreateMessageHandlers(handlers);
#endif
        }
    }
   



    // Custom menu command Ids.
    enum client_menu_ids {
        CLIENT_ID_SHOW_DEVTOOLS = MENU_ID_USER_FIRST,
        CLIENT_ID_CLOSE_DEVTOOLS,
        CLIENT_ID_INSPECT_ELEMENT,
        CLIENT_ID_SHOW_SSL_INFO,
        CLIENT_ID_OFFLINE,
        CLIENT_ID_TESTMENU_SUBMENU,
        CLIENT_ID_TESTMENU_CHECKITEM,
        CLIENT_ID_TESTMENU_RADIOITEM1,
        CLIENT_ID_TESTMENU_RADIOITEM2,
        CLIENT_ID_TESTMENU_RADIOITEM3,
    };

	// Musr match the value in client_renderer.cc.
	const char* kFocusedNodeChangedMessage = "ClientRenderer.FocusedNodeChanged";

	// Returns a data: URI with the specified contents.
	std::string GetDataURI(const std::string& data, const std::string& mime_type) {
		return "data:" + mime_type + ";base64," +
			CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
			.ToString();
	}


	JGW_Cef3HelpHandler::JGW_Cef3HelpHandler(CefHandlerDelegate* pCefHandlerDelegate,bool bIsEnableOffsetReader) : mppCefHandlerDelegate(pCefHandlerDelegate),mbIsEnableOffsetReader(bIsEnableOffsetReader),m_is_focus_oneditable_field(false), m_bUseBrowserMenu(false), m_bOpenDevTools(false), m_is_closing(false),console_log_file_("c:\\cef3.log"),first_console_message_(false),initial_navigation_(true),is_closing_(false)
	{
		resource_manager_ = new CefResourceManager();
		//! 默认添加ZIP资源
		//AddResourceManagerProvider(new JGW_ZipResResourceProvider(),0, std::string());
		//AddCefMessageRouterHandler(new JGW_Cef3MessageRouterHandler());

        // Read command line settings.
        CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
        mouse_cursor_change_disabled_ = command_line->HasSwitch(switches::kMouseCursorChangeDisabled);
        offline_ = command_line->HasSwitch(switches::kOffline);
		mvCefBrowserList.clear();
	}

	JGW_Cef3HelpHandler::~JGW_Cef3HelpHandler()
	{

	}

    void JGW_Cef3HelpHandler::DetachDelegate()
    {
        DCHECK(mppCefHandlerDelegate);
        mppCefHandlerDelegate = nullptr;
    }

    bool JGW_Cef3HelpHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
    {
        CEF_REQUIRE_UI_THREAD();
        //! _ASSERT(source_process == PID_RENDERER);
        //! 猜测大概率是渲染进程发送的消息，被浏览器进程收到
        //! 消息路由当从其他进程接收到新消息时调用
        if (message_router_->OnProcessMessageReceived(browser, frame, source_process,message)) 
        {
                return true;
        }
        // Check for messages from the client renderer.
        //! 检查来自客户端呈现器的消息。
        std::string message_name = message->GetName();
        if (message_name == kFocusedNodeChangedMessage) 
        {
            // A message is sent from ClientRenderDelegate to tell us whether the
            // currently focused DOM node is editable. Use of |focus_on_editable_field_|
            // is redundant with CefKeyEvent.focus_on_editable_field in OnPreKeyEvent
            // but is useful for demonstration purposes.
            m_is_focus_oneditable_field = message->GetArgumentList()->GetBool(0);
            return true;
        }

        return false;
    }

    void JGW_Cef3HelpHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
    {
#if 0
        CEF_REQUIRE_UI_THREAD();

        model->Clear();
        if (m_bUseBrowserMenu)
        {
            model->AddItem(MENU_ID_USER_SHOWDEVTOOLS, "&Show DevTools");    // 添加一个右键菜单项
        }
#else
        CEF_REQUIRE_UI_THREAD();

        if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
            // Add a separator if the menu already has items.
            if (model->GetCount() > 0)
                model->AddSeparator();

            // Add DevTools items to all context menus.
            model->AddItem(CLIENT_ID_SHOW_DEVTOOLS, "&Show DevTools");
            model->AddItem(CLIENT_ID_CLOSE_DEVTOOLS, "Close DevTools");
            model->AddSeparator();
            model->AddItem(CLIENT_ID_INSPECT_ELEMENT, "Inspect Element");

            if (HasSSLInformation(browser)) {
                model->AddSeparator();
                model->AddItem(CLIENT_ID_SHOW_SSL_INFO, "Show SSL information");
            }

            model->AddSeparator();
            model->AddItem(CLIENT_ID_OFFLINE, "Offline mode");
            if (offline_)
                model->SetChecked(CLIENT_ID_OFFLINE, true);

            // Test context menu features.
            BuildTestMenu(model);
        }

        if (mppCefHandlerDelegate)
            mppCefHandlerDelegate->OnBeforeContextMenu(model);
#endif
    }

    bool JGW_Cef3HelpHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefContextMenuParams> params,
        int command_id,
        EventFlags event_flags) 
    {
            CEF_REQUIRE_UI_THREAD();

            switch (command_id) {
            case CLIENT_ID_SHOW_DEVTOOLS:
                ShowDevTools(browser, CefPoint());
                return true;
            case CLIENT_ID_CLOSE_DEVTOOLS:
                CloseDevTools(browser);
                return true;
            case CLIENT_ID_INSPECT_ELEMENT:
                ShowDevTools(browser, CefPoint(params->GetXCoord(), params->GetYCoord()));
                return true;
            case CLIENT_ID_SHOW_SSL_INFO:
                ShowSSLInformation(browser);
                return true;
            case CLIENT_ID_OFFLINE:
                offline_ = !offline_;
                SetOfflineState(browser, offline_);
                return true;
            default:  // Allow default handling, if any.
                return ExecuteTestMenu(command_id);
            }
    }

    void JGW_Cef3HelpHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
    {
        CEF_REQUIRE_UI_THREAD();

        // Only update the address for the main (top-level) frame.
        if (frame->IsMain())
            if (mppCefHandlerDelegate)
                mppCefHandlerDelegate->OnSetAddress(url);
            //NotifyAddress(url);
    }

    void JGW_Cef3HelpHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
    {
        CEF_REQUIRE_UI_THREAD();
        if (mppCefHandlerDelegate)
            mppCefHandlerDelegate->OnSetTitle(title);
    }

    class ClientDownloadImageCallback : public CefDownloadImageCallback 
    {
    public:
        explicit ClientDownloadImageCallback(CefRefPtr<JGW_Cef3HelpHandler> client_handler)
            : client_handler_(client_handler) {}

        void OnDownloadImageFinished(const CefString& image_url,
            int http_status_code,
            CefRefPtr<CefImage> image) OVERRIDE {
                if (image)
                    client_handler_->NotifyFavicon(image);
        }

    private:
        CefRefPtr<JGW_Cef3HelpHandler> client_handler_;

        IMPLEMENT_REFCOUNTING(ClientDownloadImageCallback);
        DISALLOW_COPY_AND_ASSIGN(ClientDownloadImageCallback);
    };

    void JGW_Cef3HelpHandler::OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls)
    {
        CEF_REQUIRE_UI_THREAD();

        if (!icon_urls.empty() && download_favicon_images_) {
            browser->GetHost()->DownloadImage(icon_urls[0], true, 16, false,
                new ClientDownloadImageCallback(this));
        }
    }

    void JGW_Cef3HelpHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
    {
        CEF_REQUIRE_UI_THREAD();

        if (mppCefHandlerDelegate)
            mppCefHandlerDelegate->OnSetFullscreen(fullscreen);
    }

    bool JGW_Cef3HelpHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line)
    {
        CEF_REQUIRE_UI_THREAD();

        FILE* file = fopen(console_log_file_.c_str(), "a");
        if (file) {
            std::stringstream ss;
            ss << "Level: ";
            switch (level) {
            case LOGSEVERITY_DEBUG:
                ss << "Debug" << NEWLINE;
                break;
            case LOGSEVERITY_INFO:
                ss << "Info" << NEWLINE;
                break;
            case LOGSEVERITY_WARNING:
                ss << "Warn" << NEWLINE;
                break;
            case LOGSEVERITY_ERROR:
                ss << "Error" << NEWLINE;
                break;
            default:
                NOTREACHED();
                break;
            }
            ss << "Message: " << message.ToString() << NEWLINE
                << "Source: " << source.ToString() << NEWLINE << "Line: " << line
                << NEWLINE << "-----------------------" << NEWLINE;
            fputs(ss.str().c_str(), file);
            fclose(file);

            if (first_console_message_) {
                Alert(browser, "Console messages written to \"" + console_log_file_ + "\"");
                first_console_message_ = false;
            }
        }
        return false;
    }

    bool JGW_Cef3HelpHandler::OnAutoResize(CefRefPtr<CefBrowser> browser, const CefSize& new_size)
    {
        CEF_REQUIRE_UI_THREAD();

        if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnAutoResize(new_size);

        return true;
    }

    void JGW_Cef3HelpHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
    {
        CEF_REQUIRE_UI_THREAD();
        if (download_item->IsComplete()) {
            Alert(browser, "File \"" +
                download_item->GetFullPath().ToString() +
                "\" downloaded successfully.");

        }
    }

    bool JGW_Cef3HelpHandler::OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask)
    {
        CEF_REQUIRE_UI_THREAD();

        // Forbid dragging of URLs and files.
        //! 禁止拖动url和文件
        if ((mask & DRAG_OPERATION_LINK) && !dragData->IsFragment()) {
            Alert(browser, "cefclient blocks dragging of URLs and files");
            return true;
        }

        return false;
    }

    void JGW_Cef3HelpHandler::OnDraggableRegionsChanged(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const std::vector<CefDraggableRegion>& regions) 
    {
            CEF_REQUIRE_UI_THREAD();
            if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnSetDraggableRegions(regions);
    }

    void JGW_Cef3HelpHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) 
    {
        CEF_REQUIRE_UI_THREAD();

        if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnTakeFocus(next);
    }

    bool JGW_Cef3HelpHandler::OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) {
            CEF_REQUIRE_UI_THREAD();

            active_browser_ = browser;
            if (initial_navigation_) {
                CefRefPtr<CefCommandLine> command_line =
                    CefCommandLine::GetGlobalCommandLine();
                if (command_line->HasSwitch(switches::kNoActivate)) {
                    // Don't give focus to the browser on creation.
                    return true;
                }
            }

            return false;
    }

    bool JGW_Cef3HelpHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
        const CefKeyEvent& event,
        CefEventHandle os_event,
        bool* is_keyboard_shortcut) {
            CEF_REQUIRE_UI_THREAD();

            if (!event.focus_on_editable_field && event.windows_key_code == 0x20) {
                //! 当输入元素没有焦点时对空格字符的特殊处理。在OnPreKeyEvent（）中处理事件可以防止事件在渲染器中被处理。如果我们在OnKeyEvent（）方法中处理事件，除了显示警告框外，空格键还会导致窗口滚动。
                // Special handling for the space character when an input element does not
                // have focus. Handling the event in OnPreKeyEvent() keeps the event from
                // being processed in the renderer. If we instead handled the event in the
                // OnKeyEvent() method the space key would cause the window to scroll in
                // addition to showing the alert box.
                if (event.type == KEYEVENT_RAWKEYDOWN)
                    Alert(browser, "You pressed the space bar!");
                return true;
            }

            return false;
    }

    bool JGW_Cef3HelpHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
    {
        CEF_REQUIRE_UI_THREAD();
        // Return true to cancel the popup window.
        return !CreatePopupWindow(browser, false, popupFeatures, windowInfo, client,
            settings);
    }

    void JGW_Cef3HelpHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();

        browser_count_++;
		mvCefBrowserList.push_back(browser);

		//HWND handle = ::GetParent(browser->GetHost()->GetWindowHandle());
		//if (NULL != handle)
		//{
		//	CCefOperationCallBkManger::GetInstance()->NotifyForBrowserId(handle, browser->GetIdentifier());
		//}

        if (!message_router_) {
            // Create the browser-side router for query handling.
            CefMessageRouterConfig config;
            message_router_ = CefMessageRouterBrowserSide::Create(config);

            // Register handlers with the router.
            test_runner::CreateMessageHandlers(message_handler_set_);
            MessageHandlerSet::const_iterator it = message_handler_set_.begin();
            for (; it != message_handler_set_.end(); ++it)
                message_router_->AddHandler(*(it), false);
        }

        // Disable mouse cursor change if requested via the command-line flag.
        if (mouse_cursor_change_disabled_)
            browser->GetHost()->SetMouseCursorChangeDisabled(true);

        // Set offline mode if requested via the command-line flag.
        if (offline_)
            SetOfflineState(browser, true);

        if (browser->GetHost()->GetExtension()) {
            // Browsers hosting extension apps should auto-resize.
            browser->GetHost()->SetAutoResizeEnabled(true, CefSize(20, 20),
                CefSize(1000, 1000));

            CefRefPtr<CefExtension> extension = browser->GetHost()->GetExtension();
#if 0
            //! 为扩展资源注册内部处理程序。
            if (extension_util::IsInternalExtension(extension->GetPath())) {
                // Register the internal handler for extension resources.
                extension_util::AddInternalExtensionToResourceManager(extension,
                    resource_manager_);
            }
#endif
        }

        if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnBrowserCreated(browser);
    }

    bool JGW_Cef3HelpHandler::DoClose(CefRefPtr<CefBrowser> browser) 
    {
        CEF_REQUIRE_UI_THREAD();

        if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnBrowserClosing(browser);
        //NotifyBrowserClosing(browser);

        // Allow the close. For windowed browsers this will result in the OS close
        // event being sent.
        is_closing_ = true;

        return false;
    }

    void JGW_Cef3HelpHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        CEF_REQUIRE_UI_THREAD();

        if (--browser_count_ == 0) {
            // Remove and delete message router handlers.
            MessageHandlerSet::const_iterator it = message_handler_set_.begin();
            for (; it != message_handler_set_.end(); ++it) {
                message_router_->RemoveHandler(*(it));
                delete *(it);
            }
            message_handler_set_.clear();
            message_router_ = nullptr;
        }


		for (std::vector<CefRefPtr<CefBrowser>>::iterator it = mvCefBrowserList.begin(); it != mvCefBrowserList.end(); ++it)
		{
			if ((*it)->IsSame(browser))
			{
				mvCefBrowserList.erase(it);
				break;
			}
		}

		if (mvCefBrowserList.empty()) {
			// All browser windows have closed. Quit the application message loop.
			//m_is_closing = true;
			//SetClosing(true);
			CefQuitMessageLoop();
            //QuitCefQuitMessageLoop();
		}

        if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnBrowserClosed(browser);
        //NotifyBrowserClosed(browser);
    }

	void JGW_Cef3HelpHandler::CloseAllBrowser(const bool &force_close /* = false */)
	{
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute on the UI thread.
			CefPostTask(TID_UI, base::Bind(&JGW_Cef3HelpHandler::CloseAllBrowser, this,force_close));
			return;
		}

		for (std::vector<CefRefPtr<CefBrowser>>::iterator it = mvCefBrowserList.begin(); it != mvCefBrowserList.end(); ++it)
		{
			CloseBrowser((*it), force_close);
		}
	}

	void JGW_Cef3HelpHandler::CloseBrowser(CefRefPtr<CefBrowser> ptrBrowser,const bool &force_close /* = false */)
	{
		if (ptrBrowser.get())
		{
			ptrBrowser->StopLoad();
			//bool result = ptrBrowser->GetHost()->TryCloseBrowser();
			ptrBrowser->GetHost()->CloseBrowser(force_close);
			//OutputDebugStringA("TryCloseBrowser : ");
			//OutputDebugStringA(result ? "true\n" : "false\n");

		}
	}

	void JGW_Cef3HelpHandler::QuitCefQuitMessageLoop()
	{
		if (!CefCurrentlyOn(TID_UI)) {
			CefPostTask(TID_UI,
				base::Bind(&JGW_Cef3HelpHandler::QuitCefQuitMessageLoop, this));
			return;
		}
		CefQuitMessageLoop();
	}

    void JGW_Cef3HelpHandler::SizeMsgHandler(RECT rect)
    {
        if (!CefCurrentlyOn(TID_UI)) 
        {
            CefPostTask(TID_UI,base::Bind(&JGW_Cef3HelpHandler::SizeMsgHandler, this,rect));
            return;
        }
#if 0
        RECT rect;
        GetClientRect(hWnd, &rect);   
#endif
        CefWindowHandle hWndBrowser = NULL;
        int window_width = rect.right - rect.left;
        int window_beight = rect.bottom - rect.top;;

        if (!mvCefBrowserList.empty())
        {
            for (std::vector<CefRefPtr<CefBrowser>>::iterator ite = mvCefBrowserList.begin();ite != mvCefBrowserList.end();++ite)
            {
                hWndBrowser = (*ite)->GetHost()->GetWindowHandle();
                if (hWndBrowser)
                    MoveWindow(hWndBrowser, rect.left, rect.top/* + window_beight / 20*/,
                    window_width,
                    window_beight,
                    false);
            }
        }
    }

    HWND JGW_Cef3HelpHandler::GetBrowserWindowHandleToId(int id)
    {
        CefRefPtr<CefBrowser> browser = GetBrowserByID(id);
        if (browser.get())
        {
            return browser->GetHost()->GetWindowHandle();
        }
        return NULL;
    }

	CefRefPtr<CefBrowser> JGW_Cef3HelpHandler::GetBrowserByID(const INT32 &id)
	{
		CefRefPtr<CefBrowser> retbrowser = NULL;
		for (std::vector<CefRefPtr<CefBrowser>>::iterator it = mvCefBrowserList.begin(); it != mvCefBrowserList.end(); ++it)
		{
			if (id == (*it)->GetIdentifier())
			{
				retbrowser = *it;
				break;
			}
		}
		return retbrowser;
	}

	void JGW_Cef3HelpHandler::CloseBrowserToBrowserId(int id,const bool &force_close /* = false */)
	{
		if (!CefCurrentlyOn(TID_UI)) {
			CefPostTask(TID_UI,
				base::Bind(&JGW_Cef3HelpHandler::CloseBrowserToBrowserId, this, id, force_close));
			return;
		}

		CefRefPtr<CefBrowser> browser = GetBrowserByID(id);
		if (browser.get())
		{
			CloseBrowser(browser, force_close);
		}
	}

    void JGW_Cef3HelpHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
        bool isLoading,
        bool canGoBack,
        bool canGoForward) 
    {
        CEF_REQUIRE_UI_THREAD();

        if (!isLoading && initial_navigation_) {
            initial_navigation_ = false;
        }

        if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnSetLoadingState(isLoading, canGoBack, canGoForward);
        //NotifyLoadingState(isLoading, canGoBack, canGoForward);
    }

    void JGW_Cef3HelpHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        ErrorCode errorCode,
        const CefString& errorText,
        const CefString& failedUrl) {
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

    bool JGW_Cef3HelpHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect)
    {
        CEF_REQUIRE_UI_THREAD();
        message_router_->OnBeforeBrowse(browser, frame);
        return false;
    }

    bool JGW_Cef3HelpHandler::OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, CefRequestHandler::WindowOpenDisposition target_disposition, bool user_gesture)
    {
        //! 通过在新的浏览器窗口中打开URL来处理中击和ctrl +左键单击。
        if (target_disposition == WOD_NEW_BACKGROUND_TAB ||
            target_disposition == WOD_NEW_FOREGROUND_TAB) {
                // Handle middle-click and ctrl + left-click by opening the URL in a new
                // browser window.
#if 0
                RootWindowConfig config;
                config.with_controls = true;
                config.with_osr = is_osr();
                config.url = target_url;
                MainContext::Get()->GetRootWindowManager()->CreateRootWindow(config);
                return true;
#endif
        }

        // Open the URL in the current browser window.
        return false;
    }

    bool JGW_Cef3HelpHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser, const CefString& origin_url, bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback)
    {
        CEF_REQUIRE_IO_THREAD();

        // Used for testing authentication with a proxy server.
        // For example, CCProxy on Windows.
        if (isProxy) {
            callback->Continue("guest", "guest");
            return true;
        }

        // Used for testing authentication with https://jigsaw.w3.org/HTTP/.
        if (host == "jigsaw.w3.org") {
            callback->Continue("guest", "guest");
            return true;
        }

        return false;
    }

    bool JGW_Cef3HelpHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser, const CefString& origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback)
    {
        CEF_REQUIRE_IO_THREAD();

        static const int64 max_size = 1024 * 1024 * 20;  // 20mb.

        // Grant the quota request if the size is reasonable.
        callback->Continue(new_size <= max_size);
        return true;
    }

    bool JGW_Cef3HelpHandler::OnCertificateError(CefRefPtr<CefBrowser> browser,
        ErrorCode cert_error,
        const CefString& request_url,
        CefRefPtr<CefSSLInfo> ssl_info,
        CefRefPtr<CefRequestCallback> callback) {
            CEF_REQUIRE_UI_THREAD();

            if (cert_error == ERR_CERT_AUTHORITY_INVALID &&
                request_url.ToString().find("https://www.magpcss.org/") == 0U) {
                    // Allow the CEF Forum to load. It has a self-signed certificate.
                    callback->Continue(true);
                    return true;
            }

            CefRefPtr<CefX509Certificate> cert = ssl_info->GetX509Certificate();
            if (cert.get()) {
                // Load the error page.
                LoadErrorPage(browser->GetMainFrame(), request_url, cert_error,
                    GetCertificateInformation(cert, ssl_info->GetCertStatus()));
            }

            return false;  // Cancel the request.
    }

    bool JGW_Cef3HelpHandler::OnSelectClientCertificate(
        CefRefPtr<CefBrowser> browser,
        bool isProxy,
        const CefString& host,
        int port,
        const X509CertificateList& certificates,
        CefRefPtr<CefSelectClientCertificateCallback> callback) {
            CEF_REQUIRE_UI_THREAD();

            CefRefPtr<CefCommandLine> command_line =
                CefCommandLine::GetGlobalCommandLine();
            if (!command_line->HasSwitch(switches::kSslClientCertificate)) {
                return false;
            }

            const std::string& cert_name =
                command_line->GetSwitchValue(switches::kSslClientCertificate);

            if (cert_name.empty()) {
                callback->Select(nullptr);
                return true;
            }

            std::vector<CefRefPtr<CefX509Certificate>>::const_iterator it =
                certificates.begin();
            for (; it != certificates.end(); ++it) {
                CefString subject((*it)->GetSubject()->GetDisplayName());
                if (subject == cert_name) {
                    callback->Select(*it);
                    return true;
                }
            }

            return true;
    }

    void JGW_Cef3HelpHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
        TerminationStatus status) {
            CEF_REQUIRE_UI_THREAD();

            message_router_->OnRenderProcessTerminated(browser);

            // Don't reload if there's no start URL, or if the crash URL was specified.
            if (startup_url_.empty() || startup_url_ == "chrome://crash")
                return;

            CefRefPtr<CefFrame> frame = browser->GetMainFrame();
            std::string url = frame->GetURL();

            // Don't reload if the termination occurred before any URL had successfully
            // loaded.
            if (url.empty())
                return;

            std::string start_url = startup_url_;

            // Convert URLs to lowercase for easier comparison.
            std::transform(url.begin(), url.end(), url.begin(), tolower);
            std::transform(start_url.begin(), start_url.end(), start_url.begin(),
                tolower);

            // Don't reload the URL that just resulted in termination.
            if (url.find(start_url) == 0)
                return;

            frame->LoadURL(startup_url_);
    }

    void JGW_Cef3HelpHandler::OnDocumentAvailableInMainFrame(
        CefRefPtr<CefBrowser> browser) 
    {
            CEF_REQUIRE_UI_THREAD();

            // Restore offline mode after main frame navigation. Otherwise, offline state
            // (e.g. `navigator.onLine`) might be wrong in the renderer process.
            if (offline_)
                SetOfflineState(browser, true);
    }

    cef_return_value_t JGW_Cef3HelpHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
    {
        return resource_manager_->OnBeforeResourceLoad(browser, frame, request, callback);
    }

    CefRefPtr<CefResourceHandler> JGW_Cef3HelpHandler::GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request)
    {
        CEF_REQUIRE_IO_THREAD();
        return resource_manager_->GetResourceHandler(browser, frame, request);
    }

    CefRefPtr<CefResponseFilter> JGW_Cef3HelpHandler::GetResourceResponseFilter(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        CefRefPtr<CefResponse> response) 
    {
        CEF_REQUIRE_IO_THREAD();

        return test_runner::GetResourceResponseFilter(browser, frame, request,
            response);
    }

    void JGW_Cef3HelpHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        bool& allow_os_execution) {
            CEF_REQUIRE_IO_THREAD();

            std::string urlStr = request->GetURL();

            // Allow OS execution of Spotify URIs.
            if (urlStr.find("spotify:") == 0)
                allow_os_execution = true;
    }

    int JGW_Cef3HelpHandler::GetBrowserCount() const 
    {
        CEF_REQUIRE_UI_THREAD();
        return browser_count_;
    }

    // Load a data: URI containing the error message.
    void JGW_Cef3HelpHandler::LoadErrorPage(CefRefPtr<CefFrame> frame,
        const std::string& failed_url,
        cef_errorcode_t error_code,
        const std::string& other_info) {
            std::stringstream ss;
            ss << "<html><head><title>Page failed to load</title></head>"
                "<body bgcolor=\"white\">"
                "<h3>Page failed to load.</h3>"
                "URL: <a href=\""
                << failed_url << "\">" << failed_url
                << "</a><br/>Error: " << test_runner::GetErrorString(error_code) << " ("
                << error_code << ")";

            if (!other_info.empty())
                ss << "<br/>" << other_info;

            ss << "</body></html>";
            frame->LoadURL(test_runner::GetDataURI(ss.str(), "text/html"));
    }

    void JGW_Cef3HelpHandler::Alert(CefRefPtr<CefBrowser> browser, const std::string& message) 
    {
        //! 来自扩展主机的警报应该显示在活动浏览器中。
        if (browser->GetHost()->GetExtension()) 
        {
            // Alerts originating from extension hosts should instead be displayed in
            // the active browser.
            browser = active_browser_;
            if (!browser)
                return;
        }

        // Escape special characters in the message.
        std::string msg = StringReplace(message, "\\", "\\\\");
        msg = StringReplace(msg, "'", "\\'");

        // Execute a JavaScript alert().
        CefRefPtr<CefFrame> frame = browser->GetMainFrame();
        frame->ExecuteJavaScript("alert('" + msg + "');", frame->GetURL(), 0);
    }

    void JGW_Cef3HelpHandler::NotifyFavicon(CefRefPtr<CefImage> image)
    {
        if (mppCefHandlerDelegate) mppCefHandlerDelegate->OnSetFavicon(image);
    }

    void JGW_Cef3HelpHandler::ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at) 
    {
#if 0
            if (!CefCurrentlyOn(TID_UI)) {
                // Execute this method on the UI thread.
                CefPostTask(TID_UI, base::Bind(&ClientHandler::ShowDevTools, this, browser,
                    inspect_element_at));
                return;
            }
#endif

            CefWindowInfo windowInfo;
            CefRefPtr<CefClient> client;
            CefBrowserSettings settings;

            //MainContext::Get()->PopulateBrowserSettings(&settings);

            CefRefPtr<CefBrowserHost> host = browser->GetHost();

            // Test if the DevTools browser already exists.
            bool has_devtools = host->HasDevTools();
            if (!has_devtools) {
                // Create a new RootWindow for the DevTools browser that will be created
                // by ShowDevTools().
                has_devtools = CreatePopupWindow(browser, true, CefPopupFeatures(),
                    windowInfo, client, settings);
            }

            if (has_devtools) {
                // Create the DevTools browser if it doesn't already exist.
                // Otherwise, focus the existing DevTools browser and inspect the element
                // at |inspect_element_at| if non-empty.
                host->ShowDevTools(windowInfo, client, settings, inspect_element_at);
            }
    }

    void JGW_Cef3HelpHandler::CloseDevTools(CefRefPtr<CefBrowser> browser) 
    {
        browser->GetHost()->CloseDevTools();
    }



    bool JGW_Cef3HelpHandler::HasSSLInformation(CefRefPtr<CefBrowser> browser) 
    {
        CefRefPtr<CefNavigationEntry> nav =
            browser->GetHost()->GetVisibleNavigationEntry();

        return (nav && nav->GetSSLStatus() &&
            nav->GetSSLStatus()->IsSecureConnection());
    }

    void JGW_Cef3HelpHandler::ShowSSLInformation(CefRefPtr<CefBrowser> browser) 
    {
        std::stringstream ss;
        CefRefPtr<CefNavigationEntry> nav =
            browser->GetHost()->GetVisibleNavigationEntry();
        if (!nav)
            return;

        CefRefPtr<CefSSLStatus> ssl = nav->GetSSLStatus();
        if (!ssl)
            return;

        ss << "<html><head><title>SSL Information</title></head>"
            "<body bgcolor=\"white\">"
            "<h3>SSL Connection</h3>"
            << "<table border=1><tr><th>Field</th><th>Value</th></tr>";

        CefURLParts urlparts;
        if (CefParseURL(nav->GetURL(), urlparts)) {
            CefString port(&urlparts.port);
            ss << "<tr><td>Server</td><td>" << CefString(&urlparts.host).ToString();
            if (!port.empty())
                ss << ":" << port.ToString();
            ss << "</td></tr>";
        }

        ss << "<tr><td>SSL Version</td><td>"
            << GetSSLVersionString(ssl->GetSSLVersion()) << "</td></tr>";
        ss << "<tr><td>Content Status</td><td>"
            << GetContentStatusString(ssl->GetContentStatus()) << "</td></tr>";

        ss << "</table>";

        CefRefPtr<CefX509Certificate> cert = ssl->GetX509Certificate();
        if (cert.get())
            ss << GetCertificateInformation(cert, ssl->GetCertStatus());

        ss << "</body></html>";
#if 0
        RootWindowConfig config;
        config.with_controls = false;
        config.with_osr = is_osr();
        config.url = test_runner::GetDataURI(ss.str(), "text/html");
        MainContext::Get()->GetRootWindowManager()->CreateRootWindow(config);
#endif
    }
     

     void JGW_Cef3HelpHandler::SetStringResource(const std::string& page,
         const std::string& data) {
#if 0
             if (!CefCurrentlyOn(TID_IO)) {
                 CefPostTask(TID_IO, base::Bind(&JGW_Cef3HelpHandler::SetStringResource, this,
                     page, data));
                 return;
             }
#endif
             string_resource_map_[page] = data;
     }

     bool JGW_Cef3HelpHandler::CreatePopupWindow(CefRefPtr<CefBrowser> browser,
         bool is_devtools,
         const CefPopupFeatures& popupFeatures,
         CefWindowInfo& windowInfo,
         CefRefPtr<CefClient>& client,
         CefBrowserSettings& settings) {
             CEF_REQUIRE_UI_THREAD();

             //! 弹出式浏览器将成为一个新的本机窗口的父窗口。不要在DevTools窗口上显示URL栏和导航按钮。
             // The popup browser will be parented to a new native window.
             // Don't show URL bar and navigation buttons on DevTools windows.
             // MainContext::Get()->GetRootWindowManager()->CreateRootWindowAsPopup(!is_devtools, is_osr(), popupFeatures, windowInfo, client, settings);

             return true;
     }

     void JGW_Cef3HelpHandler::BuildTestMenu(CefRefPtr<CefMenuModel> model) {
         if (model->GetCount() > 0)
             model->AddSeparator();

         // Build the sub menu.
         CefRefPtr<CefMenuModel> submenu =
             model->AddSubMenu(CLIENT_ID_TESTMENU_SUBMENU, "Context Menu Test");
         submenu->AddCheckItem(CLIENT_ID_TESTMENU_CHECKITEM, "Check Item");
         submenu->AddRadioItem(CLIENT_ID_TESTMENU_RADIOITEM1, "Radio Item 1", 0);
         submenu->AddRadioItem(CLIENT_ID_TESTMENU_RADIOITEM2, "Radio Item 2", 0);
         submenu->AddRadioItem(CLIENT_ID_TESTMENU_RADIOITEM3, "Radio Item 3", 0);

         // Check the check item.
         if (test_menu_state_.check_item)
             submenu->SetChecked(CLIENT_ID_TESTMENU_CHECKITEM, true);

         // Check the selected radio item.
         submenu->SetChecked(
             CLIENT_ID_TESTMENU_RADIOITEM1 + test_menu_state_.radio_item, true);
     }

     bool JGW_Cef3HelpHandler::ExecuteTestMenu(int command_id) 
     {
         if (command_id == CLIENT_ID_TESTMENU_CHECKITEM) {
             // Toggle the check item.
             test_menu_state_.check_item ^= 1;
             return true;
         } else if (command_id >= CLIENT_ID_TESTMENU_RADIOITEM1 &&
             command_id <= CLIENT_ID_TESTMENU_RADIOITEM3) {
                 // Store the selected radio item.
                 test_menu_state_.radio_item = (command_id - CLIENT_ID_TESTMENU_RADIOITEM1);
                 return true;
         }

         // Allow default handling to proceed.
         return false;
     }

     void JGW_Cef3HelpHandler::SetOfflineState(CefRefPtr<CefBrowser> browser,
         bool offline) 
     {
         // See DevTools protocol docs for message format specification.
         CefRefPtr<CefDictionaryValue> params = CefDictionaryValue::Create();
         params->SetBool("offline", offline);
         params->SetDouble("latency", 0);
         params->SetDouble("downloadThroughput", 0);
         params->SetDouble("uploadThroughput", 0);
         browser->GetHost()->ExecuteDevToolsMethod(
             /*message_id=*/0, "Network.emulateNetworkConditions", params);
     }

     void JGW_Cef3HelpHandler::ExecuteJavaScriptToId(int id,const std::wstring& strJsCode)
     {
         CefRefPtr<CefBrowser> ptrCefBrowser = GetBrowserByID(id);
         if (ptrCefBrowser.get())
         {
             // Execute a JavaScript alert().
             CefRefPtr<CefFrame> frame = ptrCefBrowser->GetMainFrame();
             frame->ExecuteJavaScript(strJsCode, frame->GetURL(), 0);
         }
     }
























    #if 0
	// Add example Providers to the CefResourceManager.
	void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager) {
		if (!CefCurrentlyOn(TID_IO)) {
			// Execute on the browser IO thread.
			CefPostTask(TID_IO, base::Bind(SetupResourceManager, resource_manager));
			return;
		}
#if 0
		const std::string& test_origin = shared::kTestOrigin;

		// Add the Provider for dumping request contents.
		resource_manager->AddProvider(
			new RequestDumpResourceProvider(test_origin + "request.html"), 0,
			std::string());

		// Add the Provider for bundled resource files.
#if defined(OS_WIN)
  // Read BINARY resources from the executable.
		resource_manager->AddProvider(
			shared::CreateBinaryResourceProvider(test_origin), 100, std::string());
#elif defined(OS_POSIX)
  // Read individual resource files from a directory on disk.
		std::string resource_dir;
		if (shared::GetResourceDir(resource_dir)) {
			resource_manager->AddDirectoryProvider(test_origin, resource_dir, 100,
				std::string());
		}
#endif
#endif
	}

	void JGW_Cef3HelpHandler::AddResourceManagerProvider(CefResourceManager::Provider* provider, int order, const std::string& identifier)
	{
		if (!CefCurrentlyOn(TID_IO)) {
			// Execute on the browser IO thread.
			CefPostTask(TID_IO, base::Bind(&JGW_Cef3HelpHandler::AddResourceManagerProvider,this,provider,order,identifier));
			return;
		}
		// Acquire the lock for the scope of this method.
		base::AutoLock lock_scope(resource_manager_provider_lock_);

		resource_manager_->AddProvider(provider, order, identifier);
	}

	bool JGW_Cef3HelpHandler::AddCefMessageRouterHandler(CefMessageRouterBrowserSide::Handler* handler, bool first /* = false */)
	{
#if 0
		base::AutoLock lock_scope(after_created_lock_);
		if (!message_router_)
		{
			// Create the browser-side router for query handling.
			CefMessageRouterConfig config;
			message_router_ = CefMessageRouterBrowserSide::Create(config);
		}
		message_handler_set_.insert(handler);
		return message_router_->AddHandler(handler, first);
#else
		message_handler_set_.insert(handler);
		return true;
#endif
	}
#endif
}
