#pragma once
#include <string>
namespace JGW
{
    class CCJGWPrsStatusBrowserInterface
    {
    public:
        //! 
        virtual void LoadFile(const std::wstring& strFile) = 0;
        //! 
        virtual void LoadUrl(const std::wstring& strUrl) = 0;
        //! 修改纵轴图例名称
        virtual void ChangeLegendData(int nIndex,const std::wstring& strStaMac) = 0;
        //! 添加纵轴图例
        virtual void AddYSeries(const std::wstring& strStaMac) = 0;
        //! E_PRS_ROAMING_CONNECT_STATUS
        //void AddYSeriesData(int nIndex,__int64 nTimeStampMsec,int nPrsRoamingConnectStatus,const std::wstring& strDes);
        //! E_PRS_ROAMING_CONNECT_STATUS
        virtual void AddYSeriesData(const std::wstring& strMac,__int64 nTimeStampMsec,int nPrsRoamingConnectStatus,const std::wstring& strDes) = 0;
        //! 重新加载当前页面 
        virtual void ReloadLocation() = 0;
        //! 设置最大显示LIST
        virtual void SettingMaxListCount(int nMaxListCount) = 0;
        //! 开启自动滚动
        virtual void StartAutoScroll() = 0;
        //! 停止自动滚动
        virtual void StopAutoScroll() = 0;
    };
}

