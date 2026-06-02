#pragma once
#include <map>
#include <vector>
#include <string>
#include <tinyxml/tinyxml.h>
namespace JGW
{
    typedef struct  
    {
        std::string mstrParamName;
        std::string mstrParamValue;
    }S_PARAM_NAME_VALUE;
    // qmsl.config.xml
    typedef struct  
    {
        bool mbIsCompleted;
        int mnopCode;
        std::vector<S_PARAM_NAME_VALUE> mvParamNameValues;
        //std::map<std::string,std::string> mmapTlv2Param;
    }S_TLV2_CONFIG;

    class CPTP4100_QMSLConfigXml
    {
    public:
        CPTP4100_QMSLConfigXml(void);
        ~CPTP4100_QMSLConfigXml(void);
    public:
        bool LoadQMSLConfigXml();
        std::vector<S_TLV2_CONFIG>& GetTxTlv2Configs();
        std::vector<S_TLV2_CONFIG>& GetRxTlv2Configs();
    private:
        bool PareseTRxNode(TiXmlElement* pTRxNode,std::vector<S_TLV2_CONFIG>& vsTlv2Configs);
    private:
        std::vector<S_TLV2_CONFIG> mvTxTlv2Configs;
        std::vector<S_TLV2_CONFIG> mvRxTlv2Configs;
    };
}
