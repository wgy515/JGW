#pragma once
#include "UIlib.h"

namespace UiLib
{
// 	enum UILIB_RESOURCETYPE	{UILIB_FILE=1,				// 来自磁盘文件UILIB_ZIP,						// 来自磁盘zip压缩包	UILIB_RESOURCE,			// 来自资源	UILIB_ZIPRESOURCE,	// 来自资源的zip压缩包 	};

	class CDuiDialog;
	typedef void (CDuiDialog::*threadWndfunc)(WPARAM wParam,LPARAM lParam);

	struct AFX_DUI_MSGMAP;
	typedef const AFX_DUI_MSGMAP* (*pfnGetBaseMapFunc)();

	typedef struct
	{
		DWORD mssageID;
		threadWndfunc myFunc;
	}S_WND_MESSAGE,*LPS_WND_MESSAGE;

	struct AFX_DUI_MSGMAP
	{
		pfnGetBaseMapFunc pfnGetBaseMap;
		const S_WND_MESSAGE* lpEntries;
	};
	//typedef AFX_DUI_MSGMAP*   AFX_DUI_MSGMAP*;

#define BEGIN_MYWND_MESSAGE_MAP(theClass, baseClass) \
	const  AFX_DUI_MSGMAP* theClass::GetMessageMap() const\
	{ return const_cast<theClass *>(this)->GetThisMessageMap(); }\
	LRESULT theClass::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)\
	{ return baseClass::HandleMessage(uMsg,wParam,lParam);}\
	const AFX_DUI_MSGMAP* theClass::GetThisMessageMap() \
	{ \
	typedef theClass ThisClass;						   \
	typedef baseClass TheBaseClass;					   \
	static  const S_WND_MESSAGE _messageEntries[] =  \
	{

#define ON_MYWND_MESSGAE(Message_Id,memberFxn)\
	{(Message_Id),(static_cast< void (CDuiDialog::*)(WPARAM wParam,LPARAM lParam)>(&ThisClass::memberFxn))},

#define END_MYWND_MESSAGE_MAP() \
	{0, (threadWndfunc)0 } \
	}; \
	static const AFX_DUI_MSGMAP messageMap=\
	{&TheBaseClass::GetThisMessageMap,&_messageEntries[0]};\
	return &messageMap; \
	}

#define DECLARE_MYWND_MESSAGE_MAP() 	\
	public:\
	static const AFX_DUI_MSGMAP* GetThisMessageMap();\
	virtual const  AFX_DUI_MSGMAP* GetMessageMap() const;\
	virtual LRESULT	HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


	class UILIB_API CDuiDialog:public CWindowWnd
		, public INotifyUI
		, public IMessageFilterUI
		, public IDialogBuilderCallback
	{
	public:
		CDuiDialog(void);
		virtual ~CDuiDialog(void);

	//	void				LoadSkinFile(LPCTSTR strSkin);
		void				SetSkinFolder(LPCTSTR strSkinFolder);//设置当前皮肤路径 不设置默认为当前应用程序路径
		void				SetResourceType(UILIB_RESOURCETYPE eResourceType,DWORD dwResourceID = 0,LPCTSTR strResourceName = _T("ZIPRES"));//设置资源类型
		HWND				CreateDuiDialogWindow(LPCTSTR strSkinFolder
			,LPCTSTR strSkinFile,DWORD dwStyle = UI_WNDSTYLE_DIALOG,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW,LPCTSTR pstrWindowName = _T("CHDuilibWnd"));
		HWND				CreateDuiDialogWindow(LPCTSTR strSkinFolder
			,LPCTSTR strSkinFile,LPCTSTR pstrWindowName ,DWORD dwStyle = UI_WNDSTYLE_DIALOG,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW);
		HWND				CreateDuiDialogWindow(HWND hwndParent,LPCTSTR strSkinFolder
			,LPCTSTR strSkinFile,DWORD dwStyle = UI_WNDSTYLE_DIALOG,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW,LPCTSTR pstrWindowName = _T("CHDuilibWnd"));


		HWND				CreateDuiWindow(HWND hwndParent,DWORD dwStyle =0, DWORD dwExStyle =0, LPCTSTR pstrWindowName = _T("CHDuilibWnd"));//创建DUI窗口 无客户区
		virtual void		SetSkinFileName(LPCTSTR strSkinFilePath);//设置当前皮肤
		virtual UINT		GetClassStyle() const;
		virtual LPCTSTR		GetSuperClassName();

		/*         X3插件G管理接口           */
		/*  mfc SDI Interface*/
		virtual HWND GetWindow() {return GetHWND();}

		//! Create window with a specified control id (eg: AFX_IDW_PANE_FIRST).
		virtual bool CreateWnd(HWND hwndParent, int id) { return true;}

		HWND GetSafeHwnd() const{return GetHWND();}
		//! Destroy window and object.
		virtual void DestroyWindow(){::DestroyWindow(GetHWND());/*//Close(IDCANCEL);//SendMessage(WM_DESTROY,0,0);*/}

		//! Dispatch a command message.
		virtual bool OnCommand(int id, bool test = false) {return true;}

		//! Dispatch the update notify of a command UI object.
		virtual bool OnUpdateUI(int id, bool& enabled, bool& checked, std::wstring& text) { return true;}

		//! Refresh display.
		virtual void Refresh() {return ;/*this->Refresh();*/}

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, void* pHandlerInfo){return TRUE;}

		/*		子类必须实现 	*/
		virtual LPCTSTR		GetWindowClassName() const;

		/*    非必需默认实现 */
		virtual bool		InitWindow(){	return true;}
		virtual void		OnFinalMessage(HWND hWnd);
		//virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual CControlUI* CreateControl(LPCTSTR pstrClass) {return NULL;}
		virtual LRESULT		HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT		MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
		virtual LRESULT		ResponseDefaultKeyEvent(WPARAM wParam);
		virtual LRESULT		OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT		OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
#if defined(WIN32) && !defined(UNDER_CE)
		virtual LRESULT		OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		virtual LRESULT		OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		virtual LRESULT		OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT		OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT		OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
		virtual LRESULT		OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT		OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT		OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT		OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT		OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

	protected:
		LPCTSTR				GetSkinFolder(){return m_strSkinFolder.GetData();}
		LPCTSTR				GetSkinFileName();
		UILIB_RESOURCETYPE	GetResourceType(){return m_eResourceType;}

		DECLARE_MYWND_MESSAGE_MAP()
		//int					m_nMessageCounts;
		//int					GetThisMessageSize(){return m_nMessageCounts;}

	protected:
		virtual bool		LoadLayoutFile();
	protected:
		CPaintManagerUI		m_PaintManager;
	protected:
		//DWORD		m_dwStyle;
		DWORD				m_dwResourceID;//资源ID
		CDuiString			m_strSkinFolder;//皮肤文件夹
		CDuiString			m_strSkinFilePath;//皮肤文件路径
		UILIB_RESOURCETYPE	m_eResourceType;//资源类型
		CDuiString			m_strResourceName;//资源名
		bool				m_bIsCreateDuiWnd;//是否创建DUI窗体
	};

}