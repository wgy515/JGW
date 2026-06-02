#pragma once
#include "JGW_DonglesThroughputTest_Define.h"
#include <tinyxml/tinyxml.h>
namespace JGW
{
    class CCJGW_ThroughputTestConfig
    {
    public:
        CCJGW_ThroughputTestConfig(S_ThroughputTest_Confige& sThroughputConfig);
        ~CCJGW_ThroughputTestConfig(void);

        bool LoadThroughputTestXMLConfig(const std::string& strXMLConfig);

        bool SaveThroughputTestXMLConfig(const std::string& strXMLConfig);
    private:
        void LoadThroughputNode(TiXmlElement* pThroughputNode);
    private:
        S_ThroughputTest_Confige& msThroughputConfig;
    };
}