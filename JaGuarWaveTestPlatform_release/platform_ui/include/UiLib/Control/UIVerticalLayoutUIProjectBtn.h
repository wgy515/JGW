#ifndef __UIVERTICALLAYOUTPROJECTBTN_H__
#define __UIVERTICALLAYOUTPROJECTBTN_H__

#pragma once
#include "Layout/UIVerticalLayout.h"
namespace UiLib
{
    //! 项目按键布局
    typedef struct 
    {
        std::wstring m_strImageHeight;
        std::wstring m_strImageWeight;
        std::wstring m_strLabelHeight;
        std::wstring m_strLabelTextColor;
        std::wstring m_strHeight;
        std::wstring m_strFontID;
        std::wstring m_strWidth;
    }S_PROJECT_BUTTON_LAYOUT,*LPS_PROJECT_BUTTON_LAYOUT;
    //!　按键图片
    typedef struct 
    {
        std::wstring mstrNormalImage; //! 
        std::wstring mstrHotImage;//! 热点图片
        std::wstring mstrPushedImage;//! 按下图片
        std::wstring mstrFocusedImage;//! 焦点图片
        std::wstring mstrDisabledImage;//! 禁用图片
    }S_BUTTON_IMAGE_,*LPS_BUTTON_IMAGE_;

    typedef std::vector<S_BUTTON_IMAGE_> _vecButtonImage;
    //! 分组按键布局
    typedef struct 
    {
        std::wstring		m_strHeight;
        std::wstring		m_strFontID;
        std::wstring		m_strTextColor;
        S_BUTTON_IMAGE_		mSButtonImage;
    }S_GROUP_BUTTON_LAYOUT,*LPS_GROUP_BUTTON_LAYOUT;

    typedef struct  
    {
        std::wstring mstrClsid;
        std::wstring mstrModuleName;
    }S_POCO_CLASS_LOADER_INFO,*LPS_POCO_CLASS_LOADER_INFO;
    //S_DEVICE_POOL_MODULE_INFO,*LPS_DEVICE_POOL_MODULE_INFO;

    typedef S_POCO_CLASS_LOADER_INFO S_DEVICE_POOL_MODULE_INFO;
    typedef LPS_POCO_CLASS_LOADER_INFO LPS_DEVICE_POOL_MODULE_INFO;

    typedef struct  
    {
        std::wstring mstrViewLoaderPluginName;
        std::map<int,std::wstring> mID_ClsidMap;
        //! std::wstring mstrViewLoaderPluginClsid;
    }S_VIEW_LOAD_KEY_VALUE,LPS_VIEW_LOAD_KEY_VALUE;
    //! 试图加载器信息
    typedef struct 
    {
        std::vector<S_VIEW_LOAD_KEY_VALUE> mvViewLoadKey_Value;
    }S_VIEWLOADER_INFO,*LPS_VIEWRESOLVER_INFO;

    //! 插件信息
    typedef struct  
    {
        //! 保存视图加载器信息
        LPS_VIEWRESOLVER_INFO mpViewLaoderInfo;
        size_t				mImageGroupID;
        size_t				mnViewLoadID; //! 解析器插件视图加载ID
        std::wstring		mStationID; //! 站点ID
        std::wstring		mstrClsid; //! 注册类名
        std::wstring		mstrModuleFolderName;//! 模块目录名称
        std::wstring        mstrSuiteXmlName;
        std::wstring		mstrStationName; //! 站点按钮名称
        std::wstring		mstrPluginName; //! 插件名称
        std::wstring        mstrInitMenuExec;//! init_menu_exec插件打开前初始化动作
    }S_PLUGIN_INFO,*LPS_PLUGIN_INFO;

    //typedef std::vector<S_SUB_WINDOW_INFO> S_PLUGIN_GROUP_VECTOR;
    //! 插件组信息
    struct S_PLUGIN_GROUP_INFO
    {
        std::wstring mstrGroupID; //! 项目组ID
        std::wstring mstrGroupName;//! 项目组名称
        std::vector<S_PLUGIN_INFO> mvSubWinInfo; //! 项目组下插件信息
    };
    typedef std::vector<S_PLUGIN_GROUP_INFO> _vplugininfo,*_vlpPlugininfo;

    struct S_PROJECT_GROUP_PLUGIN_INFO
    {
        std::wstring mstrProjectName;
        std::wstring mstrProjectMenuName;
        _vplugininfo mvAllPluginInfo;
    };

    typedef struct  
    {
        //! size_t mnPrevClickGroupIndex; //! 上次程序关闭后，最后点击的项目组索引ID
        size_t mnProjectIndex;
        size_t mnGroupIndex;
        S_PROJECT_BUTTON_LAYOUT	msProjectButtonLayout; //! 测试项目按钮视图配置信息
        S_GROUP_BUTTON_LAYOUT msGroupButtonLayout; //! 项目组按钮视图配置信息
        S_VIEWLOADER_INFO msViewLoaderInfo;//! 试图加载器信息
        _vecButtonImage mvButtonImage; //! 项目按钮图像显示信息
        _vplugininfo mvAllPluginInfo;//! 所有插件配置相关信息
        std::vector<S_PROJECT_GROUP_PLUGIN_INFO> mvProjectGroupPluginInfo;
    }S_UI_PROJECT_INFO,*PS_UI_PROJECT_INFO;

	class CProjectListUI;
    class CProjectBtnUI;
	class UILIB_API CUIVerticalLayoutUIProjectBtn: public CVerticalLayoutUI
	{
	public:
		CUIVerticalLayoutUIProjectBtn(void);
		virtual ~CUIVerticalLayoutUIProjectBtn(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		
		void DoEvent(TEventUI& event);

		void DoPaint(HDC hDC, const RECT& rcPaint);

        void InitPluginGroupView(PS_UI_PROJECT_INFO psUIProjectInfo);

        COptionUI* GenOptionUi(S_PLUGIN_GROUP_INFO* pPluginGroupInfo);

        CProjectListUI*	GenProjectListUI(S_PLUGIN_GROUP_INFO* pPluginGroupInfo);

        void GenProjectList(CProjectListUI* pProjectListUi,std::vector<S_PLUGIN_INFO>& psSubWindowInfos);

        CProjectBtnUI* GenProjectBtnUI(S_PLUGIN_INFO* pSubWindowInfo);

		bool OnDBClickProject(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);

		bool Add(CControlUI* pControl);

		void RemoveAll();

		void InitControlPos();

        void SetSelProjectIndex(int nIndex);

        int  GetSelProjectIndex();

		bool IsPrjectButtonClick(CControlUI* pControl);

		CProjectListUI* GetProjectListUI(LPCTSTR strGroupName);
	private:
        int	m_nProjectHeight;
        int	m_nLastWeight;
        int	m_nLastHeight;
        PS_UI_PROJECT_INFO mpsUIProjectInfo;
        LPVOID m_pCurretProjectItems;
		CStdPtrArray m_aProjectItems;
        CStdPtrArray m_aSrcProjectItems;
		CStdPtrArray m_aProjectListUI;
	};

	struct S_PROJECT_LIST
	{
		CButtonUI* mpBtn;
		CLabelUI* mpLabelText;
	};

	class UILIB_API CProjectListUI: public CVerticalLayoutUI
	{
	public:
		CProjectListUI();
		~CProjectListUI();

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);

		LPCTSTR	GetGroup() const;
		void	SetGroup(LPCTSTR pStrGroupName = NULL);

		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void	DoPaint(HDC hDC, const RECT& rcPaint);
		//void	Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
		void	RemoveAll();

		int		GetSelIndex();
		void	SetSelIndex(int nSelIndex);

		bool	IsProjectButton(CControlUI* pControl);

		bool	OnDBClickItem(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
		bool	Add(CControlUI* pControl);
	private:
		CDuiString		m_sGroupName;
		CStdPtrArray	m_aProjectList;
		CDuiString		m_strNormalImage;
		int				m_nSelIndex;
		int				m_nProjectHeight;
		int				m_nItemHeight;
	};

	class UILIB_API CProjectBtnUI:public CVerticalLayoutUI
	{
	public:
		CProjectBtnUI();
		~CProjectBtnUI();

		LPCTSTR GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);

		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	DoPaint(HDC hDC, const RECT& rcPaint);
		void	InitControlPos(int width);

		void	SetPos(RECT rc);
		CButtonUI*	GetImageButton();
		CLabelUI*	GetLabelUI();
	private:
		CButtonUI*		m_pImageBtn;
		CLabelUI*		m_pLabelUI;
		bool			m_bInit;
		int				m_nImageWeight;
		int				m_nImageHeight;
		int				m_nLabelHeight;
		int				m_nLabelAndImageInterval;
		int				m_nLastWeight;
		int				m_nLastHeight;
	};

}
#endif
