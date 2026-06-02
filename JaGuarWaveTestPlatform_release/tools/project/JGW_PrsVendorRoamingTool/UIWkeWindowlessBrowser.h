#pragma once
#ifndef WKE_WINDOWLESS_BROWSER_H
#define WKE_WINDOWLESS_BROWSER_H
#include <UiLib/UIlib.h>
#include <wke/wke.h>
#include "CJGWPrsStatusBrowserInterface.h"
#define DUI_CTR_WKEWINDOWLESSBROWSER _T("WkeWindowsBrowser")
namespace JGW
{
	class CWkeWindowlessUI : public CControlUI ,public _wkeBufHandler ,public CCJGWPrsStatusBrowserInterface
	{
	public:
		CWkeWindowlessUI(void);
		~CWkeWindowlessUI(void);

		LPCTSTR GetClass() const;

		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);

		void onBufUpdated (const HDC hdc,int x, int y, int cx, int cy);

        /*void UpdateBpsInfo(size_t nIndex,double speed);

        void UpdateBpsInfo(size_t nIndex,int angle,double speed);

        void UpdateBpsInfo(size_t nIndex,const std::wstring& angle,double speed);*/
#if 0
        /// 更新角度传输速率
        void UpdateAngleMbps(int angle,double speed);
        /// 更新角度传输速率
        void UpdateAngleMbps(const std::wstring& angle,double speed);

        void UpdateThrouphputRateSpeed(size_t index,const std::wstring& strRate,const std::wstring& strSpeed);

        void UpdateThrouphputRateSpeed(size_t index,double fRate,double fSpeed);

        void UpdateThrouphputRateSpeed(size_t index,float fRate,float fSpeed);

        

        void ClearThrouphputRateSpeed();

        void ClearBpsInfo();
#endif
		//! 修改纵轴图例名称
		void ChangeLegendData(int nIndex,const std::wstring& strStaMac);
		//! 添加纵轴图例
		void AddYSeries(const std::wstring& strStaMac);
		//! E_PRS_ROAMING_CONNECT_STATUS
		//void AddYSeriesData(int nIndex,__int64 nTimeStampMsec,int nPrsRoamingConnectStatus,const std::wstring& strDes);
		//! E_PRS_ROAMING_CONNECT_STATUS
		void AddYSeriesData(const std::wstring& strMac,__int64 nTimeStampMsec,int nPrsRoamingConnectStatus,const std::wstring& strDes);
		//! 重新加载当前页面 
		void ReloadLocation();
		//! 设置最大显示LIST
		void SettingMaxListCount(int nMaxListCount);
        //! 开启自动滚动
        void StartAutoScroll();
        //! 停止自动滚动
        void StopAutoScroll();

		void SetPos(RECT rc);

		void PaintBkImage(HDC hDC);

		void InitBrowser(UINT nTimerID);

        //! 
        void LoadFile(const std::wstring& strFile);
        //! 
        void LoadUrl(const std::wstring& strUrl);

		void LoadUrl(LPCTSTR szUrl); 

        void LoadFile(LPCTSTR szFile);

        void LoadFile(LPCSTR szFile);
	protected:
		wkeWebView m_webView;
        UINT mnTimerID;
	};
}

#endif

