#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    
    class CHelpPackageUIInterface
    {
    public:
        typedef void (CHelpPackageUIInterface::*PackageUICallBack)(WPARAM wParam,LPARAM lParam);
        //! 
        CHelpPackageUIInterface(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig) : mpsPackageUpdateConfig(psPackageUpdateConfig){}
        //! 初始化模组UI
        virtual bool InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI) = 0;
        //! 
        virtual void OnNotifyHelpPackageUI(TNotifyUI& msg) = 0;
        //! 
        virtual void OnHandleEventCustomMessage(TEventUI* pTEventUI) = 0;
        //! 
        virtual bool OnHandleAsyncFuncMsg(int nActionIndex) = 0;
        //! 
        virtual void CloseHelpPackageUI() = 0;
    protected:
        LPS_PACKAGE_UPDATE_CONFIG mpsPackageUpdateConfig;
    };
    
}