#pragma once
#include <UiLib/UIlib.h>
#include <wke/wke.h>
#include "CTSEMultiPluginAppPluginDefine.h"

#define DUI_CTR_PLUGINAPPWINDOWUI _T("PluginAppWindow")

namespace JGW
{
    class CPluginAppWindowUI : public CControlUI
    {
    public:
        CPluginAppWindowUI(void);
        ~CPluginAppWindowUI(void);
    public:
        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        virtual void SetPos(RECT rc);
        virtual void DoEvent(TEventUI& event);
        virtual void PaintBkImage(HDC hDC);
    public:
        void SetPluginAppWindow(HWND hPluginAppWindow);
        bool StartPluginAppExecute(int nIndex,HWND hMianWindow,int nPluginAppID,int nMesMode,const LPS_MultiPluginAppConfig psMultiPluginAppConfig);
        void StopPluginAppExecute();
        void SendPluginAppMsg(int nMsg,WPARAM wParam,LPARAM lParam);
    private:
       HWND mhPluginAppWindow;
    };


}
