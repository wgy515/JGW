#pragma once
#include <map>

namespace JGW
{
    //! IPQ40XX 金机测试数据
    class CJGW_IPQ40xxGuTestData
    {
    public:
        static CJGW_IPQ40xxGuTestData* GetInstance();
    private:
        CJGW_IPQ40xxGuTestData(void);
        ~CJGW_IPQ40xxGuTestData(void);
    public:
        void ClearIPQ40xxGuTestData();
        //! 设置TxStationCalPathName 
        void SetTxStationCalPathName(const std::wstring& strTxStationCalPathName);
        //! 更新2.4G WIFI测试信道信息
        void UpdateIPQ40xxGuTest24ChannelInfo(const std::wstring& str24GChannelInfo);
        //! 更新5G WIFI测试信道信息
        void UpdateIPQ40xxGuTest5ChannelInfo(const std::wstring& str5GChannelInfo);
        //! 更新IPQ40xx金机测试数据
        void UpdateIPQ40xxGuTestData(int nTxChainMask,int nChannel,double dPower);
        //! 保存IPQ金机测试数据
        void SaveIPQ40xxGuTestData(const std::wstring& strSN,const std::wstring& strRelativePath);
    private:
        //double GetPowerToTxChainMaskChannel(int nTxChainMask,int nChannel);
    private:
        std::wstring mstr24GChannelInfo;
        std::wstring mstr5GChannelInfo;
        std::wstring mstrTxStationCalPathName;
        //std::map<int,std::map<int,double>> mmap24gPowerInfo;
        std::map<int,std::map<int,double>> mmapChainMaskChannelPowerInfo;
    };
}

