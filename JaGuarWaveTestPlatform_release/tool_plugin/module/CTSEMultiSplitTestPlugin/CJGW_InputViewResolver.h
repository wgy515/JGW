#pragma once
#include <UiLib/CDuiDelegateDlg.h>
#include <JaGuarWaveTestPlatform/CJGW_InputViewResolver_Define.h>
//! 获取当前控件字符串
//typedef struct
//{
//	std::wstring	name; //! 控件名
//	std::wstring	pstrName; //! 属性名称
//	LPVOID			pstrValue; //! 属性值
//}S_CONTROL_ATTR_GETTING,*LPS_CONTROL_ATTR_GETTING;
namespace JGW
{
    class CCJGWInputViewResolver:public CDuiDelegateDlg
    {
    public:
        CCJGWInputViewResolver(LPS_ALL_INPUT_UI_ATTR_SETTING psAllControlAttrSetting);
        ~CCJGWInputViewResolver(void);
    public:
		//! 模态窗口
        int			DoModal();
		//! 响应默认按键
        LRESULT		ResponseDefaultKeyEvent(WPARAM wParam);
        //! 获取控件事件消息 TDO:
        void		GetControlEventAttr();
        //! 获取对应控件名称属性
        std::wstring GetControlStringAttr( LPCTSTR name,LPCTSTR attr_name );
        //! 获取控件属性
        unUserData 	GetControlAttr( LPCTSTR name,LPCTSTR attr_name );
		//! 响应客户事件消息
        bool        OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam );
		//! 正在关闭窗口
		void OnCloseWindow();
    private:
		//! 关闭输入窗口
        void        CloseInputWindow();
		//! 获取窗口类名称
        LPCTSTR		GetWindowClassName() const;
		//! 初始化窗口
        bool		InitWindow();
		//! 通知事件
        void		Notify( TNotifyUI& msg );
    private:
		//! 设置对应控件名称属性
        void		SettingControlAttrToWindow();
    private:
        E_STRING_UPPERLOWCASE_TYPE meStringUpperLowCaseType;
        int mnMaxChar;
        int mnMinChar; 
        size_t mnNumberType;
        CButtonUI* m_pCloseBtn;
        LPS_ALL_INPUT_UI_ATTR_SETTING mpsAllControlAttrSetting;
        std::wstring  mstrInputPrefix;
    };
}