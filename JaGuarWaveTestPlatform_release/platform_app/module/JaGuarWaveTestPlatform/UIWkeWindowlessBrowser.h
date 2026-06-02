#pragma once
#ifndef WKE_WINDOWLESS_BROWSER_H
#define WKE_WINDOWLESS_BROWSER_H
#include <UiLib/UIlib.h>
#include <wke/wke.h>
#define DUI_CTR_WKEWINDOWLESSBROWSER _T("WkeWindowsBrowser")
namespace JGW
{
	class CWkeWindowlessUI : public CControlUI ,public _wkeBufHandler 
	{
	public:
		CWkeWindowlessUI(void);
		~CWkeWindowlessUI(void);

		LPCTSTR GetClass() const;

		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);

		void onBufUpdated (const HDC hdc,int x, int y, int cx, int cy);

        void UpdateBpsInfo(size_t nIndex,double speed);

        void ClearBpsInfo();

		void SetPos(RECT rc);

		void PaintBkImage(HDC hDC);

		void InitBrowser(UINT nTimerID);

		void LoadUrl(LPCTSTR szUrl); 

        void LoadFile(LPCTSTR szFile);

	protected:
		wkeWebView m_webView;
        UINT mnTimerID;
	};
}

#endif

