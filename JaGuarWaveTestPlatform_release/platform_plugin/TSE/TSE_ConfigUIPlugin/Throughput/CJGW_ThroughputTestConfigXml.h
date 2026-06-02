#pragma once
#include <tinyxml/tinyxml.h>
#include "TSE_ThroughputConfig.h"
namespace JGW
{
    class CCJGW_ThroughputTestConfigXml
    {
    public:
        CCJGW_ThroughputTestConfigXml(void);
        ~CCJGW_ThroughputTestConfigXml(void);
        //! ±£¥Ê ”Õº≈‰÷√
        void SaveThroughputTestConfig(const std::string& strSuiteConfigPath,const S_THROUGHPUT_CONFIG& sThroughputConfig);

    };
}