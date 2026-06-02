#pragma once
//////////////BEGIN消息映射宏定义////////////////////////////////////////////////////
///

namespace UiLib
{
	enum DuiSig
	{
		DuiSig_end = 0, // [marks end of message map]
		DuiSig_lwl,     // LRESULT (WPARAM, LPARAM)
		DuiSig_vn,      // void (TNotifyUI)
	};

	class CControlUI;

	// Structure for notifications to the outside world
	typedef struct tagTNotifyUI 
	{
		CDuiString sType;
		CDuiString sVirtualWnd;
		CControlUI* pSender;
		DWORD dwTimestamp;
		POINT ptMouse;
		WPARAM wParam;
		LPARAM lParam;
	} TNotifyUI;

	class CNotifyPump;
	typedef void (CNotifyPump::*DUI_PMSG)(TNotifyUI& msg);  //指针类型

    class CWindowWnd;
    typedef void (CWindowWnd::*DUI_PMSG_B)(TNotifyUI& msg);  //指针类型

	union DuiMessageMapFunctions
	{
		DUI_PMSG pfn;   // generic member function pointer
		LRESULT (CNotifyPump::*pfn_Notify_lwl)(WPARAM, LPARAM);
		void (CNotifyPump::*pfn_Notify_vn)(TNotifyUI&);
	};

	//定义所有消息类型
	//////////////////////////////////////////////////////////////////////////

#define DUI_MSGTYPE_MENU					(_T("menu"))
#define DUI_MSGTYPE_LINK					(_T("link"))

#define DUI_MSGTYPE_TIMER					(_T("timer"))
#define DUI_MSGTYPE_CLICK					(_T("click"))

#define DUI_MSGTYPE_RETURN					(_T("return"))
#define DUI_MSGTYPE_SCROLL					(_T("scroll"))

#define DUI_MSGTYPE_DROPDOWN				(_T("dropdown"))
#define DUI_MSGTYPE_SETFOCUS				(_T("setfocus"))

#define DUI_MSGTYPE_KILLFOCUS				(_T("killfocus"))
#define DUI_MSGTYPE_ITEMCLICK				(_T("itemclick"))
#define DUI_MSGTYPE_TABSELECT				(_T("tabselect"))

#define DUI_MSGTYPE_ITEMSELECT				(_T("itemselect"))
#define DUI_MSGTYPE_ITEMEXPAND				(_T("itemexpand"))
#define DUI_MSGTYPE_WINDOWINIT				(_T("windowinit"))
#define DUI_MSGTYPE_BUTTONDOWN				(_T("buttondown"))
#define DUI_MSGTYPE_MOUSEENTER				(_T("mouseenter"))
#define DUI_MSGTYPE_MOUSELEAVE				(_T("mouseleave"))

#define DUI_MSGTYPE_EDITIMTER				(_T("editTimer"))
#define DUI_MSGTYPE_EDITREGEX				(_T("editRegex"))

#define DUI_MSGTYPE_TEXTCHANGED				(_T("textchanged"))
#define DUI_MSGTYPE_HEADERCLICK				(_T("headerclick"))
#define DUI_MSGTYPE_ITEMDBCLICK				(_T("itemdbclick"))
#define DUI_MSGTYPE_SHOWACTIVEX				(_T("showactivex"))

#define DUI_MSGTYPE_ITEMCOLLAPSE			(_T("itemcollapse"))
#define DUI_MSGTYPE_ITEMACTIVATE			(_T("itemactivate"))
#define DUI_MSGTYPE_VALUECHANGED			(_T("valuechanged"))
#define DUI_MSGTYPE_VALUECHANGED_MOVE       (_T("movevaluechanged"))

#define DUI_MSGTYPE_SELECTCHANGED			(_T("selectchanged"))
#define DUI_MSGTYPE_HEADWIDTHCHANGED		(_T("headerwidthchanged"))	


	//////////////////////////////////////////////////////////////////////////



	struct DUI_MSGMAP_ENTRY;
	struct DUI_MSGMAP
	{
#ifndef UILIB_STATIC
		const DUI_MSGMAP* (PASCAL* pfnGetBaseMap)();
#else
		const DUI_MSGMAP* pBaseMap;
#endif
		const DUI_MSGMAP_ENTRY* lpEntries;
	};

	//结构定义
	struct DUI_MSGMAP_ENTRY //定义一个结构体，来存放消息信息
	{
		CDuiString sMsgType;          // DUI消息类型
		CDuiString sCtrlName;         // 控件名称
		UINT       nSig;              // 标记函数指针类型
		DUI_PMSG   pfn;               // 指向函数的指针
	};

    struct DUI_MSGMAP_ENTRY_B;
    struct DUI_MSGMAP_B
    {
#ifndef UILIB_STATIC
        const DUI_MSGMAP_B* (PASCAL* pfnGetBaseMap)();
#else
        const DUI_MSGMAP_B* pBaseMap;
#endif
        const DUI_MSGMAP_ENTRY_B* lpEntries;
    };

    //结构定义
    struct DUI_MSGMAP_ENTRY_B //定义一个结构体，来存放消息信息
    {
        CDuiString sMsgType;          // DUI消息类型
        CDuiString sCtrlName;         // 控件名称
        UINT       nSig;              // 标记函数指针类型    
        DUI_PMSG_B   pfn;               // 指向函数的指针
        UINT       nMsgID;            // 消息ID，是发送消息还是回调函数指针
        WPARAM      wParam;
        LPARAM      lParam;
    };

	//定义
#ifndef UILIB_STATIC
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static const DUI_MSGMAP messageMap;                                   \
	static const DUI_MSGMAP* PASCAL _GetBaseMessageMap();                 \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#else
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static  const DUI_MSGMAP messageMap;				                  \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#endif

    //定义
#ifndef UILIB_STATIC
#define DUI_DECLARE_MESSAGE_MAP_B()                                         \
private:                                                                  \
    static const DUI_MSGMAP_ENTRY_B _messageEntries[];                      \
protected:                                                                \
    static const DUI_MSGMAP_B messageMap;                                   \
    static const DUI_MSGMAP_B* PASCAL _GetBaseMessageMap();                 \
    virtual const DUI_MSGMAP_B* GetMessageMap() const;                      \

#else
#define DUI_DECLARE_MESSAGE_MAP_B()                                         \
private:                                                                  \
    static const DUI_MSGMAP_ENTRY_B _messageEntries[];                      \
protected:                                                                \
    static  const DUI_MSGMAP_B messageMap;				                  \
    virtual const DUI_MSGMAP_B* GetMessageMap() const;                      \

#endif


	//基类声明开始
#ifndef UILIB_STATIC
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
	{ return NULL; }                                                  \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{  &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] };\
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{  NULL, &theClass::_messageEntries[0] };                         \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif

    //基类声明开始
#ifndef UILIB_STATIC
#define DUI_BASE_BEGIN_MESSAGE_MAP_B(theClass)                              \
    const DUI_MSGMAP_B* PASCAL theClass::_GetBaseMessageMap()               \
    { return NULL; }                                                  \
    const DUI_MSGMAP_B* theClass::GetMessageMap() const                     \
    { return &theClass::messageMap; }                                 \
    UILIB_COMDAT const DUI_MSGMAP_B theClass::messageMap =                  \
    {  &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] };\
    UILIB_COMDAT const DUI_MSGMAP_ENTRY_B theClass::_messageEntries[] =     \
    {                                                                     \

#else
#define DUI_BASE_BEGIN_MESSAGE_MAP_B(theClass)                              \
    const DUI_MSGMAP_B* theClass::GetMessageMap() const                     \
    { return &theClass::messageMap; }                                 \
    UILIB_COMDAT const DUI_MSGMAP_B theClass::messageMap =                  \
    {  NULL, &theClass::_messageEntries[0] };                         \
    UILIB_COMDAT const DUI_MSGMAP_ENTRY_B theClass::_messageEntries[] =     \
    {                                                                     \

#endif


	//子类声明开始
#ifndef UILIB_STATIC
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
	{ return &baseClass::messageMap; }                                \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{ &baseClass::messageMap, &theClass::_messageEntries[0] };        \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif

//子类声明开始
#ifndef UILIB_STATIC
#define DUI_BEGIN_MESSAGE_MAP_B(theClass, baseClass)                        \
    const DUI_MSGMAP_B* PASCAL theClass::_GetBaseMessageMap()               \
    { return &baseClass::messageMap; }                                \
    const DUI_MSGMAP_B* theClass::GetMessageMap() const                     \
    { return &theClass::messageMap; }                                 \
    UILIB_COMDAT const DUI_MSGMAP_B theClass::messageMap =                  \
    { &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
    UILIB_COMDAT const DUI_MSGMAP_ENTRY_B theClass::_messageEntries[] =     \
    {                                                                     \

#else
#define DUI_BEGIN_MESSAGE_MAP_B(theClass, baseClass)                        \
    const DUI_MSGMAP_B* theClass::GetMessageMap() const                     \
    { return &theClass::messageMap; }                                 \
    UILIB_COMDAT const DUI_MSGMAP_B theClass::messageMap =                  \
    { &baseClass::messageMap, &theClass::_messageEntries[0] };        \
    UILIB_COMDAT const DUI_MSGMAP_ENTRY_B theClass::_messageEntries[] =     \
    {                                                                     \

#endif


	//声明结束
#define DUI_END_MESSAGE_MAP()                                             \
	{ _T(""), _T(""), DuiSig_end, (DUI_PMSG)0 }                           \
	};                                                                        \

//声明结束
#define DUI_END_MESSAGE_MAP_B()                                             \
    { _T(""), _T(""), DuiSig_end, (DUI_PMSG_B)0 ,NULL,NULL,NULL}                           \
    };                                                                        \


	//定义消息类型--执行函数宏
#define DUI_ON_MSGTYPE(msgtype, memberFxn)                                \
	{ msgtype, _T(""), DuiSig_vn, (DUI_PMSG)&memberFxn},                  \

#define DUI_ON_MSGTYPE_CTRLNAME_B(msgtype,msgctrlname,memberFxn)\
    { msgtype, msgctrlname, DuiSig_vn, (DUI_PMSG_B)&memberFxn,NULL},   \

#define DUI_ON_MSGTYPE_CTRLNAME_MSG_ID(msgtype,msgctrlname,msm_id)\
    { msgtype, msgctrlname, DuiSig_vn, (DUI_PMSG_B)0,msm_id,NULL,NULL},   \

#define DUI_ON_MSGTYPE_CTRLNAME_MSG_ID_PARAM(msgtype,msgctrlname,msm_id,wparam,lparam)\
    { msgtype, msgctrlname, DuiSig_vn, (DUI_PMSG_B)0,msm_id,wparam,lparam},   \

	//定义消息类型--控件名称--执行函数宏
#define DUI_ON_MSGTYPE_CTRNAME(msgtype,ctrname,memberFxn)                 \
	{ msgtype, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },                \


	//定义click消息的控件名称--执行函数宏
#define DUI_ON_CLICK_CTRNAME(ctrname,memberFxn)                           \
	{ DUI_MSGTYPE_CLICK, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },      \


	//定义selectchanged消息的控件名称--执行函数宏
#define DUI_ON_SELECTCHANGED_CTRNAME(ctrname,memberFxn)                   \
	{ DUI_MSGTYPE_SELECTCHANGED,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn }, \


	//定义killfocus消息的控件名称--执行函数宏
#define DUI_ON_KILLFOCUS_CTRNAME(ctrname,memberFxn)                       \
	{ DUI_MSGTYPE_KILLFOCUS,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },     \


	//定义menu消息的控件名称--执行函数宏
#define DUI_ON_MENU_CTRNAME(ctrname,memberFxn)                            \
	{ DUI_MSGTYPE_MENU,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },          \


	//定义与控件名称无关的消息宏

	//定义timer消息--执行函数宏
#define DUI_ON_TIMER()                                                    \
	{ DUI_MSGTYPE_TIMER, _T(""), DuiSig_vn,(DUI_PMSG)&OnTimer },          \


	///
	//////////////END消息映射宏定义////////////////////////////////////////////////////


	//////////////BEGIN控件名称宏定义//////////////////////////////////////////////////
	///

#define  DUI_CTR_EDIT                            (_T("Edit"))
#define  DUI_CTR_LIST                            (_T("List"))
#define  DUI_CTR_TEXT                            (_T("Text"))

#define  DUI_CTR_COMBO                           (_T("Combo"))
#define  DUI_CTR_LABEL                           (_T("Label"))
#define  DUI_CTR_FLASH							 (_T("Flash"))


#define  DUI_CTR_BUTTON                          (_T("Button"))
#define  DUI_CTR_OPTION                          (_T("Option"))
#define  DUI_CTR_SLIDER                          (_T("Slider"))

#define  DUI_CTR_BARCODE                        (_T("Barcode"))
#define  DUI_CTR_CONTROL                         (_T("Control"))
#define  DUI_CTR_ACTIVEX                         (_T("ActiveX"))
#define  DUI_CTR_GIFANIM                         (_T("GifAnim"))

#define  DUI_CTR_LISTITEM                        (_T("ListItem"))
#define  DUI_CTR_PROGRESS                        (_T("Progress"))
#define  DUI_CTR_RICHEDIT                        (_T("RichEdit"))
#define  DUI_CTR_CHECKBOX                        (_T("CheckBox"))
#define  DUI_CTR_COMBOBOX                        (_T("ComboBox"))
#define  DUI_CTR_DATETIME                        (_T("DateTime"))
#define  DUI_CTR_TREEVIEW                        (_T("TreeView"))
#define  DUI_CTR_TREENODE                        (_T("TreeNode"))

#define  DUI_CTR_CHARTVIEW                       (_T("ChartView"))
#define  DUI_CTR_CONTAINER                       (_T("Container"))
#define  DUI_CTR_TABLAYOUT                       (_T("TabLayout"))
#define  DUI_CTR_SCROLLBAR                       (_T("ScrollBar"))

#define  DUI_CTR_FADEBUTTON                      (_T("FadeButton"))
#define  DUI_CTR_LISTHEADER                      (_T("ListHeader"))
#define  DUI_CTR_TILELAYOUT                      (_T("TileLayout"))
#define  DUI_CTR_WEBBROWSER                      (_T("WebBrowser"))

#define  DUI_CTR_CHILDLAYOUT                     (_T("ChildLayout"))
#define  DUI_CTR_LISTELEMENT                     (_T("ListElement"))

#define  DUI_CTR_ZQ_CHARTVIEW                    (_T("ZQChartView"))
#define DUI_CTR_LIST_TEST_CTRL					 (_T("ListTestCtrl"))

#define  DUI_CTR_VERTICALLAYOUT                  (_T("VerticalLayout"))
#define  DUI_CTR_LISTHEADERITEM                  (_T("ListHeaderItem"))

#define  DUI_CTR_LISTTEXTELEMENT                 (_T("ListTextElement"))

#define  DUI_CTR_HORIZONTALLAYOUT                (_T("HorizontalLayout"))
#define  DUI_CTR_LISTLABELELEMENT                (_T("ListLabelElement"))
#define  DUI_CTR_ANIMATIONTABLAYOUT				 (_T("AnimationTabLayout"))
#define  DUI_CTR_LISTCONTAINERELEMENT            (_T("ListContainerElement"))
#define  DUI_CTR_LISTCONTAINERELEMENT_EX         (_T("ListContainerElementEx"))
#define  DUI_CTR_LISTIMAGETEXTELEMENT            (_T("ListImageTextElement"))
#define  DUI_CTR_LISTHEADERITEM_CONTAINER        (_T("ListHeaderItemContainer"))
#define  DUI_CTR_VERTICALLAYOUT_PROJECT_BUTTON	 (_T("VerticalLayoutProjectBtn"))
#define  DUI_CTR_PROJECTLIST						(_T("ProjectList"))
#define  DUI_CTR_PROJECTBTN						(_T("ProjectBtn"))
	///
	//////////////END控件名称宏定义//////////////////////////////////////////////////


	}// namespace DuiLib
