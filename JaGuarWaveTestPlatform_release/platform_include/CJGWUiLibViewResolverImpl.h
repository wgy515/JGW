#pragma once
#ifndef JGW_UILIB_VIEWLOADER_IMPL_H__
#define JGW_UILIB_VIEWLOADER_IMPL_H__
#include <string>
#include <map>

class CCJGWUiLibViewResolverImpl;

typedef void (CCJGWUiLibViewResolverImpl::*LPOnResponceMsgFun)(const WPARAM& wParam,const LPARAM& lParam);
/* 定义一个简单的JGW视图加载器 */
struct S_REG_MSG 
{
    //!	UINT			   uMsg;
    //！	std::wstring	mstrMsgType; //! 消息类型
    //!	std::wstring	mstrContainerName;//! 容器名称
    LPOnResponceMsgFun mpFunc; //! 绑定的函数指针
};

typedef std::map<UINT,LPOnResponceMsgFun> _map_reg_msg;
/*
#define ADD_REG_MSG(map_reg_msg,uMsg,lp_msg_func) map_reg_msg->insert(std::map<UINT,LPOnResponceMsgFun>::value_type(uMsg,static_cast<LPOnResponceMsgFun>(lp_msg_func)));
*/

#define ADD_REG_MSG(map_reg_msg,uMsg,lp_msg_func) map_reg_msg->insert(std::map<UINT,LPOnResponceMsgFun>::value_type(uMsg,(LPOnResponceMsgFun)lp_msg_func));

//! 插件信息结构体
typedef struct 
{
    std::wstring	mstrSkinXmlPath;
    std::wstring	mstrPluginVersions;
}S_PLUGIN_SKIN,*LPS_PLUGIN_SKIN;

 typedef struct 
 {
 	LPVOID		mpMainParentContainer;//! 父容器
 	LPVOID		mpMainPaintManager;//! JGW框架PaintManager
 	LPVOID		mpPluginInfo;
 	LPVOID		mpViewResolverClassLoader;
 	LPVOID		mpMapRegMsg;
 	LPVOID		mpWParam;
 }S_VIEW_RESOLVER_IMPL_PARAM,*LPS_VIEW_RESOLVER_IMPL_PARAM;

class CCJGWUiLibViewResolverImpl
{
public:
    CCJGWUiLibViewResolverImpl():mbIsCloseSubWnd(false){}
    virtual ~CCJGWUiLibViewResolverImpl(){ if (!mbIsCloseSubWnd) CloseSubWndPlugin();}
	//! 获取插件皮肤相关信息
	virtual const S_PLUGIN_SKIN* const GetSubWndPluginSkin() { return &msPluginSkin;}
	//! 初始化注册消息(当返回false，卸载当前加载的子窗体)
	virtual bool InitSubWndPlugin( LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam ) = 0;
	//! 响应通知事件消息
	virtual void OnNotifySubWndPlugin( LPVOID pMsg ) = 0;
	//! 子窗口关闭
	virtual void CloseSubWndPlugin() { mbIsCloseSubWnd = true;}
    //! 子窗口是否已经关闭
    virtual bool IsCloseSubWnd(){ return mbIsCloseSubWnd;}
protected:
    S_PLUGIN_SKIN msPluginSkin;
    bool mbIsCloseSubWnd;
};

#endif