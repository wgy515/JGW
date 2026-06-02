#pragma once
#include <JGW_Device_WCNTester_CommonSCPIWLAN/JGW_IWlanInstrumentInterface.h>
#include <tinyxml/tinyxml.h>
namespace JGW
{
    //!
    class CCJGW_WlanInstrumentConfig
    {
    public:
        CCJGW_WlanInstrumentConfig(void);
        ~CCJGW_WlanInstrumentConfig(void);
    public:
        //! static CCJGW_WlanInstrumentConfig* GetWlanInstrumentConfig();
        std::vector<S_STATION_CAL_PATH>& GetTxStationCalPath();
        std::vector<S_STATION_CAL_PATH>& GetRxStationCalPath();
    public:
        //£°º”‘ÿ“«∆˜≈‰÷√
        bool LoadWlanInstrumentConfig(const std::wstring& strWiFiConfigIniPath,const std::wstring& strTRxChainPort);
        //! º”‘ÿœﬂÀ≈‰÷√
        bool LoadWlanLossConfig(const std::wstring& strStationCalFileName,const std::wstring& strTxStationCalPathName,const std::wstring& strRxStationCalPathName);
        //!
        S_MONITOR_INFO* GetWlanMonitorInfo();
    private:
        bool LoadXmlPathNode(TiXmlElement* xmlPathNode,S_STATION_CAL_PATH& sStationCalPath);
    public:
        //! 
        double GetPathLoss(int txChain,double chFreqMHz);
        //! 
        int GetDutIndex(int txChain);
    private:
        S_MONITOR_INFO msMonitorInfo;
        std::vector<int> mvPortDutIndex;
        std::vector<S_STATION_CAL_PATH> mvsTxStationCalPath;
        std::vector<S_STATION_CAL_PATH> mvsRxStationCalPath;
    };
}