#include "StdAfx.h"
#include "CJGW_QCOMMWlanXml.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define QCOMM_RESOURCE_ID "WLANTester"
namespace JGW
{
    CCJGW_QCOMMWlanXml::CCJGW_QCOMMWlanXml(void)
    {
    }


    CCJGW_QCOMMWlanXml::~CCJGW_QCOMMWlanXml(void)
    {
    }

    std::wstring CCJGW_QCOMMWlanXml::GetResourceIDListItem(const TiXmlElement *pInstrumentNode)
    {
        const TiXmlElement* pTmpNode = NULL;
        pTmpNode = pInstrumentNode->FirstChildElement("ResourceIDList");

        std::wstring strValue;
        GetElementTextValue(pTmpNode,"Item",strValue);

        return strValue;
    }


    //! WlanTesterResourceID
    bool CCJGW_QCOMMWlanXml::PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        const TiXmlElement* pTmpNode = NULL;
        std::wstring strID,strConfigString,strTemp;
        bool result = false;

        pTmpNode = xmlRoot->FirstChildElement("InstrumentConfList");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("Instruments");
        if (!pTmpNode) return false;

        pTmpNode = pTmpNode->FirstChildElement("Instrument");
        if (!pTmpNode) return false;

        while (pTmpNode != NULL)
        {
            std::wstring strResourceID = GetResourceIDListItem(pTmpNode);
            if (0 == JGW_WStrComparenoCaseWStr(strResourceID.c_str(),_T(QCOMM_RESOURCE_ID)))
            {
                const TiXmlElement *pHardwareInterfaceNode = pTmpNode->FirstChildElement("HardwareInterface");
                if (!pHardwareInterfaceNode) 
                {
                    pTmpNode = pTmpNode->NextSiblingElement("Instrument");
                    continue;
                }

                std::wstring strPartialClassName;
                GetElementTextValue(pHardwareInterfaceNode,"PartialClassName",strPartialClassName);
                if (0 == JGW_WStrComparenoCaseWStr(strPartialClassName.c_str(),L"TcpClient"))
                {
                    const TiXmlElement *pITcpClientNode = pHardwareInterfaceNode->FirstChildElement("ITcpClient");

                    sMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;
                    GetElementTextValue(pITcpClientNode,"Host",strTemp);
                    sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = JGW_W2A(strTemp,CP_UTF8);
                    GetElementTextValue(pITcpClientNode,"Port",strTemp);
                    sMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort = _wtoi(strTemp.c_str());
                    result = true;
                }
                else if (0 == JGW_WStrComparenoCaseWStr(strPartialClassName.c_str(),L"Visa32"))
                {
                    const TiXmlElement *pIVisa32Node = pHardwareInterfaceNode->FirstChildElement("IVisa32");
                    sMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
                    GetElementTextValue(pIVisa32Node,"Resource",strTemp);
                    sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A(strTemp,CP_UTF8);
                    result = true;
                }
                else if (0 == JGW_WStrComparenoCaseWStr(strPartialClassName.c_str(),L"GPIB"))
                {
                    const TiXmlElement *pIGPIBNode = pHardwareInterfaceNode->FirstChildElement("IGPIB");
                    sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
                    sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
                    GetElementTextValue(pIGPIBNode,"BoardID",strTemp);
                    sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = _wtoi(strTemp.c_str());
                    GetElementTextValue(pIGPIBNode,"PrimAddress",strTemp);
                    sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = _wtoi(strTemp.c_str());
                    GetElementTextValue(pIGPIBNode,"ScndAddress",strTemp);
                    sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = _wtoi(strTemp.c_str());
                    result = true;
                }
            }
            pTmpNode = pTmpNode->NextSiblingElement("Instrument");
        }
        return result;
    }

    bool CCJGW_QCOMMWlanXml::ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        TiXmlElement* pTmpNode = NULL;
        bool result = false;
        std::wstring strID,strConfigString,strTemp;

        pTmpNode = xmlRoot->FirstChildElement("InstrumentConfList");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("Instruments");
        if (!pTmpNode) return false;

        pTmpNode = pTmpNode->FirstChildElement("Instrument");
        if (!pTmpNode) return false;

        while (pTmpNode != NULL)
        {
            std::wstring strResourceID = GetResourceIDListItem(pTmpNode);
            if (0 == JGW_WStrComparenoCaseWStr(strResourceID.c_str(),_T(QCOMM_RESOURCE_ID)))
            {
                TiXmlElement *pHardwareInterfaceNode = pTmpNode->FirstChildElement("HardwareInterface");
                if (!pHardwareInterfaceNode) 
                {
                    pTmpNode = pTmpNode->NextSiblingElement("Instrument");
                    continue;
                }

                if (sMonitorInfo.m_cInterfaceType == E_TCP_IP_HIVE)
                {
                    SetElementTextValue(pHardwareInterfaceNode,"PartialClassName",L"TcpClient");
                    TiXmlElement *pITcpClientNode = pHardwareInterfaceNode->FirstChildElement("ITcpClient");
                    if (NULL == pITcpClientNode)
                    {
                        pITcpClientNode = new TiXmlElement("ITcpClient");
                        pHardwareInterfaceNode->LinkEndChild(pITcpClientNode);
                    }
                    strTemp = JGW_A2W(sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress,CP_UTF8);
                    SetElementTextValue(pITcpClientNode,"Host",strTemp);
                    JGW_FormatWString(strTemp,L"%u",sMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort);
                    SetElementTextValue(pITcpClientNode,"Port",strTemp);
                    result = true;
                }
                else if (sMonitorInfo.m_cInterfaceType == E_VISA_HIVE)
                {
                    SetElementTextValue(pHardwareInterfaceNode,"PartialClassName",L"Visa32");
                    TiXmlElement *pIVisa32Node = pHardwareInterfaceNode->FirstChildElement("IVisa32");
                    if (NULL == pIVisa32Node)
                    {
                        pIVisa32Node = new TiXmlElement("IVisa32");
                        pHardwareInterfaceNode->LinkEndChild(pIVisa32Node);
                    }
                    strTemp = JGW_A2W(sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress,CP_UTF8);
                    SetElementTextValue(pIVisa32Node,"Resource",strTemp);
                    result = true;
                }
                else if (sMonitorInfo.m_cInterfaceType == E_GPIB_HIVE)
                {
                    SetElementTextValue(pHardwareInterfaceNode,"PartialClassName",L"GPIB");
                    TiXmlElement *pIGPIBNode = pHardwareInterfaceNode->FirstChildElement("IGPIB");
                    if (NULL == pIGPIBNode)
                    {
                        pIGPIBNode = new TiXmlElement("IGPIB");
                        pHardwareInterfaceNode->LinkEndChild(pIGPIBNode);
                    }
                    JGW_FormatWString(strTemp,L"%d",sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface);
                    SetElementTextValue(pIGPIBNode,"BoardID",strTemp);
                    JGW_FormatWString(strTemp,L"%d",sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress);
                    SetElementTextValue(pIGPIBNode,"PrimAddress",strTemp);
                    JGW_FormatWString(strTemp,L"%d",sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress);
                    SetElementTextValue(pIGPIBNode,"ScndAddress",strTemp);
                    result = true;
                }
            }
            pTmpNode = pTmpNode->NextSiblingElement("Instrument");
        }
        return result;
    }
}
