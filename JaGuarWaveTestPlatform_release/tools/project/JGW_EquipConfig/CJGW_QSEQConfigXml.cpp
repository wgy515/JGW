#include "StdAfx.h"
#include "CJGW_QSEQConfigXml.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_QSEQConfigXml::CCJGW_QSEQConfigXml(void)
    {
    }


    CCJGW_QSEQConfigXml::~CCJGW_QSEQConfigXml(void)
    {
    }

    bool CCJGW_QSEQConfigXml::PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        const TiXmlElement* pTmpNode = NULL;
        std::wstring strID,strConfigString,strTemp;

        pTmpNode = xmlRoot->FirstChildElement("DeviceConfigSession");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("DeviceGroups");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("DeviceGroup");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("Device");
        if (!pTmpNode) return false;

        while (pTmpNode)
        {
            GetElementAttributeValue(pTmpNode,"ID",strID);
            if (sMonitorInfo.mnDeviceID == _ttoi(strID.c_str()))
            {
                if (!pTmpNode->FirstChildElement("ConfigString")) return false;
                GetElementTextValue(pTmpNode,"ConfigString",strConfigString);
                //pTmpNode = pTmpNode->FirstChildElement("ConfigString");
                //if (!pTmpNode) return false;
                //strConfigString = pTmpNode->GetText();
                break;
            }
            pTmpNode = pTmpNode->NextSiblingElement();
        }
        int i = strConfigString.find(L"|");
        if (std::string::npos != i)
        {
            strTemp = strConfigString.substr(0,i);
            if (L'V' == strTemp[0] || L'v' == strTemp[0])
            {
                sMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
                sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A(strTemp.substr(5));
                return true;
            }
            else
            {
                if (strTemp.length() < strlen("GPIB=GPIB0::0::0::")) return false;
                sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
                const wchar_t* strData = strTemp.c_str();
                //! GPIB0::20::0::
                sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
                std::vector<std::wstring> vstrTemp;
                JGW_ParserStrW(strData + 5,L"::",vstrTemp);
                if (vstrTemp.size() < 3) return false;
                strTemp = vstrTemp.at(0);
                JGW_ReplaceStringW(strTemp,L"GPIB",L"");
                sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = _ttoi(strTemp.c_str());
                sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = _ttoi(vstrTemp.at(1).c_str());
                sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = _ttoi(vstrTemp.at(2).c_str());
                return true;
            }
        }
        return false;
    }

    bool CCJGW_QSEQConfigXml::ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        TiXmlElement* pTmpNode = NULL;
        std::wstring strID,strConfigString,strTemp,strPostfix;

        pTmpNode = xmlRoot->FirstChildElement("DeviceConfigSession");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("DeviceGroups");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("DeviceGroup");
        if (!pTmpNode) return false;
        pTmpNode = pTmpNode->FirstChildElement("Device");
        if (!pTmpNode) return false;

        while (pTmpNode)
        {
            GetElementAttributeValue(pTmpNode,"ID",strID);
            if (sMonitorInfo.mnDeviceID == _ttoi(strID.c_str()))
            {
                if (!pTmpNode->FirstChildElement("ConfigString")) return false;
                GetElementTextValue(pTmpNode,"ConfigString",strConfigString);
                //pTmpNode = pTmpNode->FirstChildElement("ConfigString");
                //if (!pTmpNode) return false;
                //strConfigString = pTmpNode->GetText();
                break;
            }
            pTmpNode = pTmpNode->NextSiblingElement();
        }

        if (!pTmpNode) return false;

        int i = strConfigString.find(L"|");
        if (std::wstring::npos == i) return false;
        strPostfix = strConfigString.substr(i);

        if (E_GPIB_HIVE == sMonitorInfo.m_cInterfaceType)
        {
            JGW_FormatWString(strTemp,L"GPIB=GPIB%d::%d::%d::INSTR%s",sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface,sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress,sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress,strPostfix.c_str());
            pTmpNode->FirstChild()->ToText()->SetValue(JGW_W2A(strTemp));
            SetElementTextValue(pTmpNode,"ConfigString",strTemp);
            return true;
        }
        else if (E_VISA_HIVE == sMonitorInfo.m_cInterfaceType)
        {
            JGW_FormatWString(strTemp,L"VISA=%s%s",JGW_A2W(sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress).c_str(),strPostfix.c_str());
            SetElementTextValue(pTmpNode,"ConfigString",strTemp);
            //pTmpNode->FirstChild()->ToText()->SetValue(JGW_W2A(strTemp).c_str());
            return true;
        }
        return false;
    }
}