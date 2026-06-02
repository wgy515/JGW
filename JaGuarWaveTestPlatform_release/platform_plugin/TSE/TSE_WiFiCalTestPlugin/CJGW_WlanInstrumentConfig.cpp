#include "StdAfx.h"
#include "CJGW_WlanInstrumentConfig.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    CCJGW_WlanInstrumentConfig::CCJGW_WlanInstrumentConfig(void)
    {
    }


    CCJGW_WlanInstrumentConfig::~CCJGW_WlanInstrumentConfig(void)
    {
    }

    /*CCJGW_WlanInstrumentConfig* CCJGW_WlanInstrumentConfig::GetWlanInstrumentConfig()
    {
    static CCJGW_WlanInstrumentConfig gWlanInstrumentConfig;
    return &gWlanInstrumentConfig;
    }*/

    bool CCJGW_WlanInstrumentConfig::LoadWlanInstrumentConfig(const std::wstring& strWiFiConfigIniPath,const std::wstring& strTRxChainPort)
    {
        mvPortDutIndex.clear();
        JGW_ParserIntW(strTRxChainPort.c_str(),L",",mvPortDutIndex);
        if(0 == mvPortDutIndex.size()) mvPortDutIndex.push_back(1);
        while (mvPortDutIndex.size() < 4) mvPortDutIndex.push_back(mvPortDutIndex[0]);

        CCJGW_ConfigIni m_pConfigIniFile;
        if (!m_pConfigIniFile.InitIniFilePath(strWiFiConfigIniPath.c_str())) return false;

        std::wstring strAddressType = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"ADDRESS");
        std::wstring strTemp;
        msMonitorInfo.m_strModelName = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"MODEL");

        if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"GPIB"))
        {
            msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;

            strTemp = m_pConfigIniFile.GetIniKeyValue(L"GPIB",L"PartialClassName",L"NIGPIB");
            msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
            if (0 != strTemp.compare(L"NIGPIB")) msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = AGILENT_GPIB_TYPE;

            msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"BoardID",0);
            msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"PrimAddress",20);
            msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"ScndAddress",0);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"VISA"))
        {
            msMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
            msMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"VISA",L"VisaAddress",L"TCPIP0::CMW50050-134371::inst0::INSTR"));
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"TCPIP"))
        {
            msMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;
            msMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"IPAddress",L"10.62.8.197"));
            msMonitorInfo.msTCPIPInstrumentInfo.mstrSourceAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"SourceAddress"));
            msMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort = m_pConfigIniFile.GetIniKeyIntValue(L"TCPIP",L"Port",5026);
            msMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument = m_pConfigIniFile.GetIniKeyIntValue(L"TCPIP",L"SubInstrument",0);
        }
        else
        {
            msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
            msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = 0;
            msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = 20;
            msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 0;
        }

        return true;
    }

    int CCJGW_WlanInstrumentConfig::GetDutIndex(int txChain)
    {
        if (mvPortDutIndex.size() > (size_t)txChain)
        {
            return mvPortDutIndex[txChain];
        }
        if (0 == mvPortDutIndex.size())
        {
            return 1;
        }
        return mvPortDutIndex[0];
    }

    S_MONITOR_INFO* CCJGW_WlanInstrumentConfig::GetWlanMonitorInfo()
    {
        return &msMonitorInfo;
    }

    std::vector<S_STATION_CAL_PATH>& CCJGW_WlanInstrumentConfig::GetTxStationCalPath()
    {
        return mvsTxStationCalPath;
    }

    std::vector<S_STATION_CAL_PATH>& CCJGW_WlanInstrumentConfig::GetRxStationCalPath()
    {
        return mvsRxStationCalPath;
    }

    bool CCJGW_WlanInstrumentConfig::LoadWlanLossConfig(const std::wstring& strStationCalFileName,const std::wstring& strTxStationCalPathName,const std::wstring& strRxStationCalPathName)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        if (!ptrXmlDoc->LoadFile(JGW_W2A(strStationCalFileName).c_str()))
        {
            LogE_F(L"Load Station Cal File(%s) Fail",strStationCalFileName.c_str());
            return false;
        }
        std::vector<std::wstring> vstrTxStationCalPathName,vstrRxStationCalPathName;
        JGW_ParserStrW(strTxStationCalPathName.c_str(),L",",vstrTxStationCalPathName);
        JGW_ParserStrW(strRxStationCalPathName.c_str(),L",",vstrRxStationCalPathName);
        mvsRxStationCalPath.clear();
        mvsTxStationCalPath.clear();

        TiXmlElement* xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            LogE_F(L"Load Station Cal File(%s) Fail",strStationCalFileName.c_str());
            return false;
        }
        if (vstrTxStationCalPathName.size() != vstrRxStationCalPathName.size())
        {
            LogE(L"Config TxStationCalPathName And RxStationCalPathName Fail");
            return false;
        }
        std::wstring strPathName;
        bool bLoadTxStationCalPath = false,bLoadRxStationCalPath = false;
        for (size_t i = 0;i < vstrTxStationCalPathName.size();i ++)
        {
            bLoadTxStationCalPath = false;
            bLoadRxStationCalPath = false;
            TiXmlElement* xmlPathNode = xmlRoot->FirstChildElement("Path");
            while (xmlPathNode)
            {
                GetElementTextValue(xmlPathNode,"PathName",strPathName);
                if (NULL == JGW_WStrComparenoCaseWStr(strPathName.c_str(),vstrTxStationCalPathName[i].c_str()))
                {
                    S_STATION_CAL_PATH sStationCalPath;
                    if (!LoadXmlPathNode(xmlPathNode,sStationCalPath))
                    {
                        LogE_F(L"Load Xml Path Node(%s) Fail",strPathName.c_str());
                        return false;
                    }
                    mvsTxStationCalPath.push_back(sStationCalPath);
					bLoadRxStationCalPath = true;
                }
                if (NULL == JGW_WStrComparenoCaseWStr(strPathName.c_str(),vstrRxStationCalPathName[i].c_str()))
                {
                    S_STATION_CAL_PATH sStationCalPath;
                    if (!LoadXmlPathNode(xmlPathNode,sStationCalPath))
                    {
                        LogE_F(L"Load Xml Path Node(%s) Fail",strPathName.c_str());
                        return false;
                    }
                    mvsRxStationCalPath.push_back(sStationCalPath);
					bLoadTxStationCalPath = true;
                }
                xmlPathNode = xmlPathNode->NextSiblingElement("Path");
            }
            if (!bLoadRxStationCalPath || !bLoadTxStationCalPath)
            {
                LogE_F(L"Not Find PathName %s-%s",vstrTxStationCalPathName[i].c_str(),vstrRxStationCalPathName[i].c_str());
                return false;
            }
        }

        return true;
    }

    bool CCJGW_WlanInstrumentConfig::LoadXmlPathNode(TiXmlElement* xmlPathNode,S_STATION_CAL_PATH& sStationCalPath)
    {
        std::wstring strTemp;
        GetElementTextValue(xmlPathNode,"PathName",strTemp);
        sStationCalPath.mstrPathName = JGW_W2A(strTemp,CP_UTF8);
        TiXmlElement* dataListNode = xmlPathNode->FirstChildElement("DataList");
        if (NULL == dataListNode)
        {
            LogE(L"Not Find DataList XML Node Fail");
            return false;
        }
        TiXmlElement* dataNode = dataListNode->FirstChildElement("Data");
        std::wstring strFreq,strValue,strDelta;
        while (dataNode)
        {
            GetElementTextValue(dataNode,"Frequency",strFreq);
            GetElementTextValue(dataNode,"Value",strValue,L"1");
            GetElementTextValue(dataNode,"Delta",strDelta,L"0");
            if (!strFreq.empty())
            {
                S_LOSS_Data sLossData = { _ttoi(strFreq.c_str()),static_cast<float>(_ttof(strValue.c_str())),static_cast<float>(_ttof(strDelta.c_str()))};
                sStationCalPath.mvsLossData.push_back(sLossData);
            }
            dataNode = dataNode->NextSiblingElement("Data");
        }
        return true;
    }
}