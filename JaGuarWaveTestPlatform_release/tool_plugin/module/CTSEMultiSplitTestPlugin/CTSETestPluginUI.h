#pragma once
#include <UiLib/UIlib.h>

namespace JGW
{
    class CCTSETestPluginWnd;
    class CCTSETestPluginUI :
        public CControlUI
    {
    public:
        CCTSETestPluginUI(void);
        ~CCTSETestPluginUI(void);
    public:
		//! 设置模块目录以及TSE序列配置文件名称
        void SetModuleFolderSuitName(const std::wstring& strModuleFolder,const std::wstring& strSulteName); 
    public:
        LPCTSTR	GetClass() const;
        LPVOID	GetInterface(LPCTSTR pstrName);

        void	SetInternVisible(bool bVisible = true);
        void	DoInit();
        void	SetPos(RECT rc);
        void    CloseTSETestPluginWnd();

        friend CCTSETestPluginWnd;
    private:
        std::wstring mstrModuleFolder;
        std::wstring mstrSuiteName;
		HWND mhWindow;
        CCTSETestPluginWnd* m_pWindow;
    };
}


