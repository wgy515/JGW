#pragma once

typedef struct
{
	std::wstring pstrName;
	std::wstring pstrValue;
}S_ATTR_NAME_VALUE,*LPS_ATTR_NAME_VALUE;

//! 设置控件属性
typedef struct 
{
	std::wstring name;
	std::vector<S_ATTR_NAME_VALUE> mvsAttrNameValue;
}S_CONTROL_ATTR_SETTING,*LPS_CONTROL_ATTR_SETTING;


typedef struct
{
	std::wstring name; //! 控件名称
	std::wstring stype; //! 控件消息
}S_CONTROL_EVENT_SETTING;


//! 窗口属性
typedef struct 
{
	HWND			mhParentHwnd;
	std::wstring	mstrSkinFolder;
	std::wstring	mstrSkinFile;
	DWORD dwStyle /* = UI_WNDSTYLE_DIALOG */;
	DWORD dwExStyle /* = WS_EX_STATICEDGE | WS_EX_APPWINDOW */;
	std::wstring	mstrWindowName /* = _T */;
}S_WINDOW_ATTR,*LPS_WINDOW_ATTR;

enum E_STRING_UPPERLOWCASE_TYPE
{
    //! 不忽略大小写，对大小写敏感
    E_STRING_NOT_IGNORE_CASE_TYPE = 0,
    //! 默认大写模式
    E_STRING_UPPERCASE_TYPE,
    //！ 小写模式
    E_STRING_LOWCASE_TYPE
};

typedef struct
{
	S_WINDOW_ATTR							m_sWindowAttr;
	std::vector<LPS_CONTROL_ATTR_SETTING>	m_vControlAttrSetting;
}S_ALL_CONTROL_ATTR_SETTING,*LPS_ALL_CONTROL_ATTR_SETTING;


typedef struct  
{
    S_WINDOW_ATTR m_sWindowAttr;
    S_CONTROL_ATTR_SETTING	m_sGetControlAttrSetting;
    std::vector<S_CONTROL_ATTR_SETTING> m_vControlAttrSetting;
}S_ALL_INPUT_UI_ATTR_SETTING,*LPS_ALL_INPUT_UI_ATTR_SETTING;

//! 获取当前控件字符串
//typedef struct
//{
//	std::wstring	name; //! 控件名
//	std::wstring	pstrName; //! 属性名称
//	LPVOID			pstrValue; //! 属性值
//}S_CONTROL_ATTR_GETTING,*LPS_CONTROL_ATTR_GETTING;

