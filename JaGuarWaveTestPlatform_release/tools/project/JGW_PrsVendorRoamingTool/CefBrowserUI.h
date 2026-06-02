#pragma once
#include <UiLib/UIlib.h>
#include "CJGWPrsStatusBrowserInterface.h"
#include <UiLibCef/UiLibCefExportManager.h>
//类名称
const TCHAR CCefBrowserUIClassName[] = L"CefBrowserUI";		
//控件名称
const TCHAR CCefBrowserUIItemInterface[] = L"CefBrowser";	

namespace JGW
{
    class SimpleUiLibCefMsgHandler : public UiLibCefMsgHandler
    {
    public:
        int mnBrowserID;
        bool mbIsClosing;
        SimpleUiLibCefMsgHandler() : mbIsClosing(false),mnBrowserID(-1){}

        //! 在创建浏览器时调用。
        virtual void OnBrowserCreated(int nBrowserID) override
        {
            OutputDebugStringA("OnBrowserCreated\n");
            mnBrowserID = nBrowserID;
        }

        //! Called when the browser is closing.
        //! 浏览器正在关闭时调用。
        virtual void OnBrowserClosing(int nBrowserID)  override
        {
            OutputDebugStringA("OnBrowserClosing\n");
            mbIsClosing = true;
            mnBrowserID = -1;
        }

        // Called when the browser has been closed.
        //! 当浏览器已经关闭时调用
        virtual void OnBrowserClosed(int nBrowserID) override
        {
            OutputDebugStringA("OnBrowserClosed\n");
        }
        // Set the window URL address.
        //! 设置窗口URL地址。
        virtual void OnSetAddress(const std::string& url) override
        {
            OutputDebugStringA("OnSetAddress\n");
        }
        // Set the window title.
        //! 设置窗口标题。
        virtual void OnSetTitle(const std::string& title) override
        {
            OutputDebugStringA("OnSetTitle\n");
            //SetWindowTextA(ghMainHwnd,title.c_str());
        }
        // Set fullscreen mode.
        //! 设置全屏模式。
        virtual void OnSetFullscreen(bool fullscreen)  override
        {
            OutputDebugStringA("OnSetFullscreen\n");
        }

        // Auto-resize contents.
        //! 自动调整大小的内容。
        virtual void OnAutoResize(int width_val, int height_val)  override
        {
            OutputDebugStringA("OnAutoResize\n");
        }

        // Set the loading state.
        //! 设置加载状态。
        virtual void OnSetLoadingState(bool isLoading,
            bool canGoBack,
            bool canGoForward)  override
        {
            OutputDebugStringA("OnSetLoadingState\n");
        }
        // Set focus to the next/previous control.
        //! 将焦点设置为下一个/上一个控件。
        virtual void OnTakeFocus(bool next)  override
        {
            OutputDebugStringA("OnTakeFocus\n");
        }
    };

    class CCefBrowserUI : public CControlUI,public CCJGWPrsStatusBrowserInterface
    {
    public:
        CCefBrowserUI(HWND hParentHwnd);
        ~CCefBrowserUI(void);
    public:
        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        void DoInit();
        //设置位置
        virtual void SetPos(RECT rc);
        //父容器调用设置显示与否
        virtual void SetInternVisible(bool bVisible = true);
        //设置是否显示
        virtual void SetVisible(bool bVisible = true);
    protected:
        //! 修改纵轴图例名称
        void ChangeLegendData(int nIndex,const std::wstring& strStaMac) override;
        //! 添加纵轴图例
        void AddYSeries(const std::wstring& strStaMac) override;
        //! E_PRS_ROAMING_CONNECT_STATUS
        //void AddYSeriesData(int nIndex,__int64 nTimeStampMsec,int nPrsRoamingConnectStatus,const std::wstring& strDes);
        //! E_PRS_ROAMING_CONNECT_STATUS
        void AddYSeriesData(const std::wstring& strMac,__int64 nTimeStampMsec,int nPrsRoamingConnectStatus,const std::wstring& strDes) override;
        //! 重新加载当前页面 
        void ReloadLocation() override;
        //! 设置最大显示LIST
        void SettingMaxListCount(int nMaxListCount) override;
        //! 开启自动滚动
        void StartAutoScroll() override;
        //! 停止自动滚动
        void StopAutoScroll() override;
        //! 
        void LoadFile(const std::wstring& strFile);
        //! 
        void LoadUrl(const std::wstring& strUrl);
    private:
        //!
        bool mbIsCreated;
        HWND mhParentHwnd;
        //! 浏览器名称
        std::wstring browser_name_;
        //! 默认地址
        std::wstring url_;
        //! 
        SimpleUiLibCefMsgHandler mcSimpleUiLibCefMsgHandler;
    };
}


