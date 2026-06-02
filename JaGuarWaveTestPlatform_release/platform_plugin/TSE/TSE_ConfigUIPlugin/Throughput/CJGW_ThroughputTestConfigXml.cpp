#include "StdAfx.h"
#include "CJGW_ThroughputTestConfigXml.h"

namespace JGW
{
    CCJGW_ThroughputTestConfigXml::CCJGW_ThroughputTestConfigXml(void)
    {
    }


    CCJGW_ThroughputTestConfigXml::~CCJGW_ThroughputTestConfigXml(void)
    {
    }

    void CCJGW_ThroughputTestConfigXml::SaveThroughputTestConfig(const std::string& strSuiteConfigPath,const S_THROUGHPUT_CONFIG& sThroughputConfig)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot,*pCommonNode,*pConfigNode = NULL;
        if (!ptrXmlDoc->LoadFile(strSuiteConfigPath.c_str())) return ;

        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return ;

        pCommonNode = pRoot->FirstChildElement("common");
        while (pCommonNode)
        {
            SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrIperfServerIPAddressEnvironment).c_str(),sThroughputConfig.mstrIperfServerIPAddress);
            SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrTestRangeEnvironment).c_str(),sThroughputConfig.mstrTestRange);
            SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrTestTimeEnvironment).c_str(),sThroughputConfig.mstrTestTime);
            SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrWiFiScanIPAddressEnvironment).c_str(),sThroughputConfig.mstrWiFiScanIPAddress);
            SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrIperfCommandEnvironment).c_str(),sThroughputConfig.mstrIperfCommand);
            SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrIperfCommandTimeOutEnvironment).c_str(),sThroughputConfig.mstrIperfCommandTimeOut);
            pCommonNode = pCommonNode->NextSiblingElement("common");
        }
        ptrXmlDoc->SaveFile(strSuiteConfigPath);
    }
}