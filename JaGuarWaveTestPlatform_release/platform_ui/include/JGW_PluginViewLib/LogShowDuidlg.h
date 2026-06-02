#pragma once
#include "JGW_PluginView_Define.h"
#include <UiLib/CDuiDialog.h>
#include <sstream>

// CLogShowDuidlg ¶Ô»°¿ò
class CLogShowDuidlg:public CDuiDialog
{
public:
	CLogShowDuidlg(std::wstring& strDubugLog,std::wstring& strErrorLog,HWND hParentHwnd = NULL,std::wstring& m_strHtmlPath = g_strHtmlPath);
	~CLogShowDuidlg();
	//void	SetShowString(){ m_lpstrDubugLog = strDubugLog;m_lpstrErrorLog = strErrorLog;}
	int					DoModal();
public:
	virtual LPCTSTR		GetWindowClassName() const;
	bool				InitWindow();
	virtual UINT		GetClassStyle() const;
	virtual void		SetHtmlFilePath(std::wstring strHtmlPath){m_strHtmlPath = strHtmlPath;}
	//DECLARE_MYWND_MESSAGE_MAP();
private:
	void				Notify(TNotifyUI& msg);
	void				OnBnClickedOK();
	static std::wstring g_strHtmlPath;
	HWND				m_hParentHwnd;
	CButtonUI*			m_pCloseBtn;
	CButtonUI*			m_pMinBtn;
	CButtonUI*			m_pMaxBtn;
	CButtonUI*			m_pRestoreBtn;
	CRichEditUI*		m_plogedt;
	COptionUI*			m_pdebugOption;
	COptionUI*			m_perrorOption;
	std::wstring& m_strDubugLog;
	std::wstring& m_strErrorLog;
	std::wstring& m_strHtmlPath;
};