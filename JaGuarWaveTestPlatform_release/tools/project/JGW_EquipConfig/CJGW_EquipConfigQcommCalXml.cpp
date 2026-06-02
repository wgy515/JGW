#include "StdAfx.h"
#include "CJGW_EquipConfigQcommCalXml.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_EquipConfigQcommCalXml::CCJGW_EquipConfigQcommCalXml(void)
    {
    }


    CCJGW_EquipConfigQcommCalXml::~CCJGW_EquipConfigQcommCalXml(void)
    {
    }

    bool CCJGW_EquipConfigQcommCalXml::PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        const TiXmlElement* pEquipmentNode = NULL;
        pEquipmentNode = xmlRoot->FirstChildElement("Equipment");
        if (!pEquipmentNode) return false;

        const char* pszTemp = NULL;
        pszTemp = pEquipmentNode->Attribute("pad");
        if (pszTemp)
        {
            sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
            sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = atoi(pszTemp);
            pszTemp = pEquipmentNode->Attribute("board_id");
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = pszTemp?atoi(pszTemp):0; 
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 0;
        }

        pszTemp = pEquipmentNode->Attribute("ip");
        if (pszTemp)
        {
            sMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;
            sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = pszTemp;
            sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument = 0;
        }

        pszTemp = pEquipmentNode->Attribute("visa");
        if (pszTemp)
        {
            sMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
            sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = pszTemp;
        }
        return true;
    }

    bool CCJGW_EquipConfigQcommCalXml::ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        TiXmlElement* pEquipmentNode = NULL;
        pEquipmentNode = xmlRoot->FirstChildElement("Equipment");
        if (!pEquipmentNode) return false;

        while (pEquipmentNode)
        {
            pEquipmentNode->RemoveAttribute("pad");
            pEquipmentNode->RemoveAttribute("board_id");
            pEquipmentNode->RemoveAttribute("visa");
            pEquipmentNode->RemoveAttribute("ip");

            if (E_GPIB_HIVE == sMonitorInfo.m_cInterfaceType)
            {
                pEquipmentNode->SetAttribute("pad",sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress);
                pEquipmentNode->SetAttribute("board_id",sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface);
            }
            else if (E_VISA_HIVE == sMonitorInfo.m_cInterfaceType)
            {
                pEquipmentNode->SetAttribute("visa",sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress.c_str());
            }
            else if (E_TCP_IP_HIVE == sMonitorInfo.m_cInterfaceType)
            {
                pEquipmentNode->SetAttribute("ip",sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress.c_str());
            }
            pEquipmentNode = pEquipmentNode->NextSiblingElement();
        }
       
        return true;
    }
}