#pragma once
#ifndef JGW_UILIB_VIEWRESOLVER_DEFINE_H__
#define JGW_UILIB_VIEWRESOLVER_DEFINE_H__

typedef struct 
{
    CHorizontalLayoutUI*    mpMainParentContainer;//! ¸¸ÈÝÆ÷
    CPaintManagerUI*		mpMainPaintManager;//! JGW¿ò¼ÜPaintManager
    S_PLUGIN_INFO*		    mpPluginInfo;
    LPVOID		            mpViewResolverClassLoader;
    _map_reg_msg*		    mpMapRegMsg;
    LPS_COMMON_WND_MSG_INFO	mpsCommonWndMsgInfo;
}S_VIEW_RESOLVER_PARAM,*LPS_VIEW_RESOLVER_PARAM;

#endif
