#include "StdAfx.h"
#include "PTP4100_QMSLConfigXml.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CPTP4100_QMSLConfigXml::CPTP4100_QMSLConfigXml(void)
    {
    }


    CPTP4100_QMSLConfigXml::~CPTP4100_QMSLConfigXml(void)
    {
    }

    std::vector<S_TLV2_CONFIG>& CPTP4100_QMSLConfigXml::GetTxTlv2Configs()
    {
        return mvTxTlv2Configs;
    }

    std::vector<S_TLV2_CONFIG>& CPTP4100_QMSLConfigXml::GetRxTlv2Configs()
    {
        return mvRxTlv2Configs;
    }

    bool CPTP4100_QMSLConfigXml::LoadQMSLConfigXml()
    {
        std::string strQmslConfigXmlPath;
        JGW_FormatString(strQmslConfigXmlPath,"%s\\qmsl.config.xml",JGW_W2A_W(JGW_GetApplicationFolder()).c_str());
        if (!JGW_FileExistsToFilePathA(strQmslConfigXmlPath.c_str())) return false;

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot = NULL,*pTxNode = NULL,*pRxNode = NULL;

        mvTxTlv2Configs.clear();
        mvRxTlv2Configs.clear();
        if ( !ptrXmlDoc->LoadFile(strQmslConfigXmlPath.c_str()) )
        { 
            return false;
        }

        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;

        return PareseTRxNode(pRoot->FirstChildElement("tx"),mvTxTlv2Configs) && PareseTRxNode(pRoot->FirstChildElement("rx"),mvRxTlv2Configs) ;
    }

    bool CPTP4100_QMSLConfigXml::PareseTRxNode(TiXmlElement* pTRxNode,std::vector<S_TLV2_CONFIG>& vsTlv2Configs)
    {
        std::wstring strTemp;
        TiXmlElement* pTlv2Node = pTRxNode->FirstChildElement("tlv2");

        while (pTlv2Node)
        {
            S_TLV2_CONFIG stlv2Config;

            GetElementAttributeValue(pTlv2Node,"opCode",strTemp,L"-1");
            stlv2Config.mnopCode = _ttoi(strTemp.c_str());
            GetElementAttributeValue(pTlv2Node,"isComplete",strTemp,L"1");
            stlv2Config.mbIsCompleted = (1 == _ttoi(strTemp.c_str()));

            TiXmlElement* pParamElement = pTlv2Node->FirstChildElement();
            const char* pszTemp = NULL;
            while (pParamElement)
            {
                pszTemp = pParamElement->Value();
                std::wstring strParamName = pszTemp ? JGW_A2W_A(pszTemp,CP_UTF8) : L"";

                pszTemp = pParamElement->GetText();
                std::wstring strParamValue = pszTemp ? JGW_A2W_A(pszTemp,CP_UTF8) : L"0";

                S_PARAM_NAME_VALUE sParamNameValue;
                sParamNameValue.mstrParamName = JGW_W2A(strParamName);
                sParamNameValue.mstrParamValue = JGW_W2A(strParamValue);
                stlv2Config.mvParamNameValues.push_back(sParamNameValue);
                //stlv2Config.mmapTlv2Param[JGW_W2A(strParamName)] = JGW_W2A(strParamValue);
                pParamElement = pParamElement->NextSiblingElement();
            }
            vsTlv2Configs.push_back(stlv2Config);

            pTlv2Node = pTlv2Node->NextSiblingElement("tlv2");
        }
        return true;
    }
}

