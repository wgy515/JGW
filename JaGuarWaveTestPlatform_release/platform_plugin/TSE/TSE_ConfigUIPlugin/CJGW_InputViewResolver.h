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
        int			DoModal();

        LRESULT		ResponseDefaultKeyEvent(WPARAM wParam);
        //! 获取控件事件消息 TDO:
        void		GetControlEventAttr();
        //! 
        std::wstring GetControlStringAttr( LPCTSTR name,LPCTSTR attr_name );
        //! 
        unUserData 	GetControlAttr( LPCTSTR name,LPCTSTR attr_name );

        bool        OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam );

		void OnCloseWindow();
    private:
        void        CloseInputWindow();

        LPCTSTR		GetWindowClassName() const;
        bool		InitWindow();
        void		Notify( TNotifyUI& msg );
    private:
        void		SettingControlAttrToWindow();
    private:
        E_STRING_UPPERLOWCASE_TYPE meStringUpperLowCaseType;
        int mnMaxChar;
        int mnMinChar; 
        size_t mnNumberType;
        CButtonUI* m_pCloseBtn;
        LPS_ALL_INPUT_UI_ATTR_SETTING mpsAllControlAttrSetting;
        ///LPS_WINDOW_ATTR	 mpsWindowAttr;
        std::wstring  mstrInputPrefix;
        ///std::vector<LPS_CONTROL_ATTR_SETTING> m_vControlAttrSetting;
    };
}