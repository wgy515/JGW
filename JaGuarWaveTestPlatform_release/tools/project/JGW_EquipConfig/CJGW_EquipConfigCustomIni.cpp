#include "StdAfx.h"
#include "CJGW_EquipConfigCustomIni.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_EquipConfigCustomIni::CCJGW_EquipConfigCustomIni(void)
    {
    }


    CCJGW_EquipConfigCustomIni::~CCJGW_EquipConfigCustomIni(void)
    {
    }

    bool CCJGW_EquipConfigCustomIni::LoadCustomIni(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo)
    {
        mpsEquipProjectConfig = psEquipProjectConfig;
        GetEquipConfigXmlPath();
        CCJGW_ConfigIni m_pConfigIniFile;
        if (!m_pConfigIniFile.InitIniFilePath(JGW_A2W(mstrConfigFilePath).c_str())) return false;
        return LoadMonitorConfig(m_pConfigIniFile,sMonitorInfo);
    }

    bool CCJGW_EquipConfigCustomIni::SaveCustomIni(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo)
    {
        mpsEquipProjectConfig = psEquipProjectConfig;
        GetEquipConfigXmlPath();
        CCJGW_ConfigIni m_pConfigIniFile;
        if (!m_pConfigIniFile.InitIniFilePath(JGW_A2W(mstrConfigFilePath).c_str())) return false;
        return ChangeMonitorConfig(m_pConfigIniFile,sMonitorInfo);
    }

    bool CCJGW_EquipConfigCustomIni::LoadMonitorConfig(CCJGW_ConfigIni& m_pConfigIniFile,S_MONITOR_INFO& sMonitorInfo)
    {
        std::wstring strAddressType = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"ADDRESS");
        std::wstring strTemp;
        sMonitorInfo.m_strModelName = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"MODEL");
        
        if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"GPIB"))
        {
            sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;

            strTemp = m_pConfigIniFile.GetIniKeyValue(L"GPIB",L"PartialClassName",L"NIGPIB");
            sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
            if (0 != strTemp.compare(L"NIGPIB")) sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = 1;

            sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"BoardID",0);
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"PrimAddress",20);
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"ScndAddress",0);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"VISA"))
        {
            sMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
            sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"VISA",L"VisaAddress",L"TCPIP0::CMW50050-134371::inst0::INSTR"));
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"TCPIP"))
        {
            sMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;

            sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"IPAddress",L"10.62.8.197"));
            sMonitorInfo.msTCPIPInstrumentInfo.mstrSourceAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"SourceAddress"));
            sMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort = m_pConfigIniFile.GetIniKeyIntValue(L"TCPIP",L"Port",5026);
            strTemp = m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"SubInstrument",L"0");
            sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument = _ttoi(strTemp.c_str());
        }
        else
        {
            sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = 0;
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = 20;
            sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 0;
        }

        return true;
    }

    bool CCJGW_EquipConfigCustomIni::ChangeMonitorConfig(CCJGW_ConfigIni& m_pConfigIniFile,S_MONITOR_INFO& sMonitorInfo)
    {
        m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"MODEL",sMonitorInfo.m_strModelName.c_str());
        if (E_GPIB_HIVE == sMonitorInfo.m_cInterfaceType)
        {
            m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"ADDRESS",L"GPIB");

            if (AGILENT_GPIB_TYPE == sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType)
            {
                m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PartialClassName",L"AGILENTGPIB");
            }
            else
            {
                m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PartialClassName",L"NIGPIB");
            }
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"BoardID",sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface);
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PrimAddress",sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress);
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"ScndAddress",sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress);
        }
        else if (E_VISA_HIVE == sMonitorInfo.m_cInterfaceType)
        {
            m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"ADDRESS",L"VISA");
            m_pConfigIniFile.SetIniKeyValue(L"VISA",L"VisaAddress",JGW_A2W(sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress).c_str());
        }
        else if (E_TCP_IP_HIVE == sMonitorInfo.m_cInterfaceType)
        {
            m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"ADDRESS",L"TCPIP");
            m_pConfigIniFile.SetIniKeyValue(L"TCPIP",L"IPAddress",JGW_A2W(sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress).c_str());

            m_pConfigIniFile.SetIniKeyValue(L"TCPIP",L"SourceAddress",JGW_A2W(sMonitorInfo.msTCPIPInstrumentInfo.mstrSourceAddress).c_str());
            m_pConfigIniFile.SetIniKeyValue(L"TCPIP",L"Port",sMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort);

            m_pConfigIniFile.SetIniKeyValue(L"TCPIP",L"SubInstrument",sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument);
        }
        else
        {
            m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"ADDRESS",L"GPIB");
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PartialClassName",L"NIGPIB");
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"BoardID",0);
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PrimAddress",20);
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"ScndAddress",0);
        }
        return true;
    }
}