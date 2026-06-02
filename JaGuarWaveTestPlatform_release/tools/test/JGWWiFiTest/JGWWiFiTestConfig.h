#pragma once
#include <tinyxml/tinyxml.h>
#include "JGWWiFiTest_Define.h"

namespace JGW
{
    class JGWWiFiTestConfig
    {
    public:
        JGWWiFiTestConfig(void);
        ~JGWWiFiTestConfig(void);

        static E_WLAN_RX_DATA_RATE GetWlanRxDataRateToTxDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate);
        static const S_WLAN_TX_RX_DATA_RATE_INFO* GetWlanTxRxDataRateInfoToDataRateName(const wchar_t* strDataRateName);
        static E_WLAN_MODE GetWlanModeToTxDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate);
        static E_WLAN_HT_MODE GetWlanHTModeToTxDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate);
        static const wchar_t* GetWlanDateRateNameToDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate);
    public:
        //! 
        bool LoadWiFiTestCacheXML(S_WLAN_CONFIG& sWlanConfig,std::string strWiFiCacheXML);
        //! Loss “«∆˜≈‰÷√
        bool LoadWiFiTestConfigXML(S_WLAN_CONFIG& sWlanConfig,std::string strWiFiConfigXml);
    private:
        void DefalueWiFiTestConfig();
        bool ParesWiFiTestConfigXml(const TiXmlElement* xmlRoot);
    private:
        bool PareseAllWiFiParamNode(const TiXmlElement* xmlRoot);
        bool PareseTestNodeToCacheXML(const TiXmlElement* xmlTestNode);
        bool PareseConfigNodeToCacheXML(const TiXmlElement* xmlRoot);
    private:
        void PareseInfoItemValue(size_t txChainLen,const wchar_t* strValue,std::vector<std::wstring>& vstrValue);
        void BuildLoss(size_t nTxChain,size_t channelSize,std::vector<std::vector<float>>& vvLoss,std::vector<std::string>& vstrLoss);
        void BuldInfoItemParam(size_t txChainLen,size_t nDataRateSize,size_t channelSize,std::vector<std::vector<float>>& vvvParam,std::wstring strParam);
        void BuldInfoItemParam(size_t txChainLen,size_t nDataRateSize,size_t channelSize,std::vector<std::vector<int>>& vvvParam,std::wstring strParam);
    private:
        S_WLAN_CONFIG* mpSWlanConfig;
        std::vector<std::string> mvstrRxLoss;
        std::vector<std::string> mvstrTxLoss;
    };
}


