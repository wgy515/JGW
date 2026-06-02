#include "StdAfx.h"
#include "JGWWiFiTestConfig.h"

#include <map>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

#include <Shlwapi.h>

#define IsNull(x,result)\
    if( !(x) )\
    return result;
namespace JGW
{
    JGWWiFiTestConfig::JGWWiFiTestConfig(void)
    {
    }


    JGWWiFiTestConfig::~JGWWiFiTestConfig(void)
    {
    }
#if 0
    E_WLAN_RX_DATA_RATE JGWWiFiTestConfig::GetWlanRxDataRateToTxDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate)
    {
        switch (eWlanTxDataRate)
        {
            //! 802.11 b
        case E_RATE_1MBPS_L_CCK:
            return E_RX_RATE_11B_LONG_1_MBPS_CCK;
        case E_RATE_2MBPS_L_CCK:
            return E_RX_RATE_11B_LONG_2_MBPS_CCK;
        case E_RATE_2MBPS_S_CCK:
            return E_RX_RATE_11B_SHORT_2_MBPS_CCK;
        case E_RATE_5_5MBPS_L_CCK:
            return E_RX_RATE_11B_LONG_5_5_MBPS_CCK;
        case E_RATE_5_5MBPS_S_CCK:
            return E_RX_RATE_11B_SHORT_5_5_MBPS_CCK;
        case E_RATE_11MBPS_L_CCK:
            return E_RX_RATE_11B_LONG_11_MBPS_CCK;
        case E_RATE_11MBPS_S_CCK:
            return E_RX_RATE_11B_SHORT_11_MBPS_CCK;
            //! 802.11 a/g
        case E_RATE_6MBPS_NO_HT:
            return E_RX_RATE_11A_6_MBPS_NO_HT;
        case E_RATE_9MBPS_NO_HT:
            return E_RX_RATE_11A_9_MBPS_NO_HT;
        case E_RATE_12MBPS_NO_HT:
            return E_RX_RATE_11A_12_MBPS_NO_HT;
        case E_RATE_18MBPS_NO_HT:
            return E_RX_RATE_11A_18_MBPS_NO_HT;
        case E_RATE_24MBPS_NO_HT:
            return E_RX_RATE_11A_24_MBPS_NO_HT;
        case E_RATE_36MBPS_NO_HT:
            return E_RX_RATE_11A_36_MBPS_NO_HT;
        case E_RATE_48MBPS_NO_HT:
            return E_RX_RATE_11A_48_MBPS_NO_HT;
        case E_RATE_54MBPS_NO_HT:
            return E_RX_RATE_11A_54_MBPS_NO_HT;

        }
    }
#else
    E_WLAN_RX_DATA_RATE JGWWiFiTestConfig::GetWlanRxDataRateToTxDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate)
    {
        for (int i = 0;i < _countof(gvWlanTxRxDataRateInfo);i ++)
        {
            if (eWlanTxDataRate == gvWlanTxRxDataRateInfo[i].meWlanTxDataRate)
                return gvWlanTxRxDataRateInfo[i].meWlanRxDataRate;
        }
        return gvWlanTxRxDataRateInfo[0].meWlanRxDataRate;
    }
#endif

    const S_WLAN_TX_RX_DATA_RATE_INFO* JGWWiFiTestConfig::GetWlanTxRxDataRateInfoToDataRateName(const wchar_t* strDataRateName)
    {
        for (int i = 0;i < _countof(gvWlanTxRxDataRateInfo);i ++)
        {
            if (NULL == JGW_WStrComparenoCaseWStr(strDataRateName,gvWlanTxRxDataRateInfo[i].szDataRateName))
                return &(gvWlanTxRxDataRateInfo[i]);
        }
        return NULL;
    }

    const wchar_t* JGWWiFiTestConfig::GetWlanDateRateNameToDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate)
    {
        for (int i = 0;i < _countof(gvWlanTxRxDataRateInfo);i ++)
        {
            if (eWlanTxDataRate == gvWlanTxRxDataRateInfo[i].meWlanTxDataRate)
                return gvWlanTxRxDataRateInfo[i].szDataRateName;
        }
        return L"Unknow Data Rate";
    }

    E_WLAN_HT_MODE JGWWiFiTestConfig::GetWlanHTModeToTxDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate)
    {
        switch (eWlanTxDataRate)
        {
        case E_RATE_1MBPS_L_CCK:
        case E_RATE_2MBPS_L_CCK:
        case E_RATE_2MBPS_S_CCK:
        case E_RATE_5_5MBPS_L_CCK:
        case E_RATE_5_5MBPS_S_CCK:
        case E_RATE_11MBPS_L_CCK:
        case E_RATE_11MBPS_S_CCK:
            return E_WLAN_CCK;
        case E_RATE_6MBPS_NO_HT:
        case E_RATE_9MBPS_NO_HT:
        case E_RATE_12MBPS_NO_HT:
        case E_RATE_18MBPS_NO_HT:
        case E_RATE_24MBPS_NO_HT:
        case E_RATE_36MBPS_NO_HT:
        case E_RATE_48MBPS_NO_HT:
        case E_RATE_54MBPS_NO_HT:
            return E_WLAN_NO_HT_DSRC10;
        case E_RATE_MCS_0_20_HT20:
        case E_RATE_MCS_1_20_HT20:
        case E_RATE_MCS_2_20_HT20:
        case E_RATE_MCS_3_20_HT20:
        case E_RATE_MCS_4_20_HT20:
        case E_RATE_MCS_5_20_HT20:
        case E_RATE_MCS_6_20_HT20:
        case E_RATE_MCS_7_20_HT20:
            return E_WLAN_HT20;
        case E_RATE_MCS_0_40_HT40_ADD_DEC:
        case E_RATE_MCS_1_40_HT40_ADD_DEC:
        case E_RATE_MCS_2_40_HT40_ADD_DEC:
        case E_RATE_MCS_3_40_HT40_ADD_DEC:
        case E_RATE_MCS_4_40_HT40_ADD_DEC:
        case E_RATE_MCS_5_40_HT40_ADD_DEC:
        case E_RATE_MCS_6_40_HT40_ADD_DEC:
        case E_RATE_MCS_7_40_HT40_ADD_DEC:
            return E_WLAN_HT40_ADD;
        case E_RATE_AC_MCS_0_20_VHT20:
        case E_RATE_AC_MCS_1_20_VHT20:
        case E_RATE_AC_MCS_2_20_VHT20:
        case E_RATE_AC_MCS_3_20_VHT20:
        case E_RATE_AC_MCS_4_20_VHT20:
        case E_RATE_AC_MCS_5_20_VHT20:
        case E_RATE_AC_MCS_6_20_VHT20:
        case E_RATE_AC_MCS_7_20_VHT20:
        case E_RATE_AC_MCS_8_20_VHT20:
        case E_RATE_AC_MCS_9_20_VHT20:
            return E_WLAN_VHT20;
        case E_RATE_AC_MCS_0_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_1_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_2_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_3_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_4_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_5_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_6_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_7_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_8_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_9_40_VHT40_ADD_DEC:
            return E_WLAN_VHT40_ADD;
        case E_RATE_AC_MCS_0_80_VHT80X:
        case E_RATE_AC_MCS_1_80_VHT80X:
        case E_RATE_AC_MCS_2_80_VHT80X:
        case E_RATE_AC_MCS_3_80_VHT80X:
        case E_RATE_AC_MCS_4_80_VHT80X:
        case E_RATE_AC_MCS_5_80_VHT80X:
        case E_RATE_AC_MCS_6_80_VHT80X:
        case E_RATE_AC_MCS_7_80_VHT80X:
        case E_RATE_AC_MCS_8_80_VHT80X:
        case E_RATE_AC_MCS_9_80_VHT80X:
            return E_WLAN_VHT80_0;
        }
        return E_WLAN_NO_HT_DSRC10;
    }

    E_WLAN_MODE JGWWiFiTestConfig::GetWlanModeToTxDataRate(E_WLAN_TX_DATA_RATE eWlanTxDataRate)
    {
        switch (eWlanTxDataRate)
        {
        case E_RATE_1MBPS_L_CCK:
        case E_RATE_2MBPS_L_CCK:
        case E_RATE_2MBPS_S_CCK:
        case E_RATE_5_5MBPS_L_CCK:
        case E_RATE_5_5MBPS_S_CCK:
        case E_RATE_11MBPS_L_CCK:
        case E_RATE_11MBPS_S_CCK:
            return E_B_WLAN_MODE;
        case E_RATE_6MBPS_NO_HT:
        case E_RATE_9MBPS_NO_HT:
        case E_RATE_12MBPS_NO_HT:
        case E_RATE_18MBPS_NO_HT:
        case E_RATE_24MBPS_NO_HT:
        case E_RATE_36MBPS_NO_HT:
        case E_RATE_48MBPS_NO_HT:
        case E_RATE_54MBPS_NO_HT:
            return E_G_WLAN_MODE;
        case E_RATE_MCS_0_20_HT20:
        case E_RATE_MCS_1_20_HT20:
        case E_RATE_MCS_2_20_HT20:
        case E_RATE_MCS_3_20_HT20:
        case E_RATE_MCS_4_20_HT20:
        case E_RATE_MCS_5_20_HT20:
        case E_RATE_MCS_6_20_HT20:
        case E_RATE_MCS_7_20_HT20:
        case E_RATE_MCS_0_40_HT40_ADD_DEC:
        case E_RATE_MCS_1_40_HT40_ADD_DEC:
        case E_RATE_MCS_2_40_HT40_ADD_DEC:
        case E_RATE_MCS_3_40_HT40_ADD_DEC:
        case E_RATE_MCS_4_40_HT40_ADD_DEC:
        case E_RATE_MCS_5_40_HT40_ADD_DEC:
        case E_RATE_MCS_6_40_HT40_ADD_DEC:
        case E_RATE_MCS_7_40_HT40_ADD_DEC:
            return E_N_WLAN_MODE;
        case E_RATE_AC_MCS_0_20_VHT20:
        case E_RATE_AC_MCS_1_20_VHT20:
        case E_RATE_AC_MCS_2_20_VHT20:
        case E_RATE_AC_MCS_3_20_VHT20:
        case E_RATE_AC_MCS_4_20_VHT20:
        case E_RATE_AC_MCS_5_20_VHT20:
        case E_RATE_AC_MCS_6_20_VHT20:
        case E_RATE_AC_MCS_7_20_VHT20:
        case E_RATE_AC_MCS_8_20_VHT20:
        case E_RATE_AC_MCS_9_20_VHT20:
        case E_RATE_AC_MCS_0_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_1_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_2_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_3_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_4_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_5_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_6_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_7_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_8_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_9_40_VHT40_ADD_DEC:
        case E_RATE_AC_MCS_0_80_VHT80X:
        case E_RATE_AC_MCS_1_80_VHT80X:
        case E_RATE_AC_MCS_2_80_VHT80X:
        case E_RATE_AC_MCS_3_80_VHT80X:
        case E_RATE_AC_MCS_4_80_VHT80X:
        case E_RATE_AC_MCS_5_80_VHT80X:
        case E_RATE_AC_MCS_6_80_VHT80X:
        case E_RATE_AC_MCS_7_80_VHT80X:
        case E_RATE_AC_MCS_8_80_VHT80X:
        case E_RATE_AC_MCS_9_80_VHT80X:
            return E_AC_WLAN_MODE;
        }
        return E_UNKNOW_MODE;
    }

    void JGWWiFiTestConfig::DefalueWiFiTestConfig()
    {
        mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
        mpSWlanConfig->msMonitorInfo.m_cInterfaceType = 0;
        mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = 0;
        mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = 20;
        mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 96;
        mpSWlanConfig->msMonitorInfo.m_strModelName = L"CMW500";
        //! Tx 
        mvstrTxLoss.push_back(L"1.0");
        mvstrTxLoss.push_back(L"1.0");
        mvstrTxLoss.push_back(L"1.0");
        mvstrTxLoss.push_back(L"1.0");
        //! rx
        mvstrRxLoss.push_back(L"1.0");
        mvstrRxLoss.push_back(L"1.0");
        mvstrRxLoss.push_back(L"1.0");
        mvstrRxLoss.push_back(L"1.0");
    }

    bool JGWWiFiTestConfig::LoadMonitorConfig(const std::wstring strWiFiConfigIniPath,S_WLAN_CONFIG& sWlanConfig)
    {
        CCJGW_ConfigIni m_pConfigIniFile;
        if (!m_pConfigIniFile.InitIniFilePath(strWiFiConfigIniPath.c_str())) return false;

        std::wstring strAddressType = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"ADDRESS");
        std::wstring strTemp;
        mpSWlanConfig->msMonitorInfo.m_strModelName = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"MODEL");

        if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"GPIB"))
        {
            mpSWlanConfig->msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;

            strTemp = m_pConfigIniFile.GetIniKeyValue(L"GPIB",L"PartialClassName",L"NIGPIB");
            mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
            if (0 != strTemp.compare(L"NIGPIB")) mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = 1;

            mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"BoardID",0);
            mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"PrimAddress",20);
            mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"ScndAddress",0);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"VISA"))
        {
            mpSWlanConfig->msMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
            mpSWlanConfig->msMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"VISA",L"VisaAddress",L"TCPIP0::CMW50050-134371::inst0::INSTR"));
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"TCPIP"))
        {
            mpSWlanConfig->msMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;

            mpSWlanConfig->msMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"IPAddress",L"10.62.8.197"));

            strTemp = m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"SubInstrument",L"0");
            mpSWlanConfig->msMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument = _ttoi(strTemp.c_str());
        }
        else
        {
            mpSWlanConfig->msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
            mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = 0;
            mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = 20;
            mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 0;
        }

        return true;
    }

    bool JGWWiFiTestConfig::LoadWiFiTestConfigXML(S_WLAN_CONFIG& sWlanConfig,std::string strWiFiConfigXml)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;
        mpSWlanConfig = &sWlanConfig;
        mvstrTxLoss.clear();
        mvstrRxLoss.clear();

        if ( !ptrXmlDoc->LoadFile( strWiFiConfigXml.c_str() ) )
        {
            DefalueWiFiTestConfig();
            return true;
        }

        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !ParesWiFiTestConfigXml(xmlRoot) )
        {
            //! DefalueWiFiTestConfig();
            return false;
        }

        return true;
    }

    bool JGWWiFiTestConfig::ParesWiFiTestConfigXml(const TiXmlElement* xmlRoot)
    {
        //! Instrument
//         {
//             const TiXmlElement*	pInstrumentNode,*pIGPIBNode = NULL,*pVISANode = NULL,*pTCPIPNode = NULL;
//             std::string strTemp;
//             std::wstring strModelName;
//             pInstrumentNode = xmlRoot->FirstChildElement("Instrument");
//             IsNull(pInstrumentNode,false);
// 
//             
//             GetElementTextValue(pInstrumentNode,"Model",strModelName,L"CMW500");
//             mpSWlanConfig->msMonitorInfo.m_strModelName = strModelName;
// 
// 
//             pIGPIBNode = pInstrumentNode->FirstChildElement("IGPIB");
//             if (pIGPIBNode)
//             {
//                 mpSWlanConfig->msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
//                 GetElementTextValueA(pIGPIBNode,"PartialClassName",strTemp,"NIGPIB");
//                 mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = 0;
//                 if (0 != strTemp.compare("NIGPIB")) mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = 1;
// 
//                 GetElementTextValueA(pIGPIBNode,"BoardID",strTemp,"0");
//                 mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = atoi(strTemp.c_str());
// 
//                 GetElementTextValueA(pIGPIBNode,"PrimAddress",strTemp,"20");
//                 mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = atoi(strTemp.c_str());
// 
//                 GetElementTextValueA(pIGPIBNode,"ScndAddress",strTemp,"0");
//                 mpSWlanConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = atoi(strTemp.c_str());
//             }
//             pVISANode = pInstrumentNode->FirstChildElement("VISA");
//             if (pVISANode)
//             {
//                 mpSWlanConfig->msMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
//                 GetElementTextValueA(pVISANode,"VisaAddress",mpSWlanConfig->msMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress,"TCPIP0::CMW50050-134371::inst0::INSTR");
//             }
// 
//             pTCPIPNode = pInstrumentNode->FirstChildElement("TCPIP");
//             if (pTCPIPNode)
//             {
//                 mpSWlanConfig->msMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;
//                 GetElementTextValueA(pTCPIPNode,"IPAddress",mpSWlanConfig->msMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress,"10.62.8.197");
//                 GetElementTextValueA(pTCPIPNode,"SubInstrument",strTemp,"0");
//                 mpSWlanConfig->msMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument = atoi(strTemp.c_str());
//             }
//         }
        //! Loss
        {
            const TiXmlElement*	pLossNode = NULL,*pTxNode = NULL,*pRxNode = NULL;
            std::map<std::wstring,std::wstring> mtxMap,mrxMap;
            std::wstring strTemp,strName;
            pLossNode = xmlRoot->FirstChildElement("Loss");
            IsNull(pLossNode,false);

            pTxNode = pLossNode->FirstChildElement("TX");
            IsNull(pTxNode,false);
            while ( pTxNode )
            {
                GetElementAttributeValue(pTxNode,"Name",strName,L"B");
                GetElementAttributeValue(pTxNode,"Value",strTemp,L"B");
                mtxMap[strName] = strTemp;
                pTxNode = pTxNode->NextSiblingElement();
            }

            pRxNode = pLossNode->FirstChildElement("RX");
            IsNull(pRxNode,false);
            while ( pRxNode )
            {
                GetElementAttributeValue(pRxNode,"Name",strName,L"B");
                GetElementAttributeValue(pRxNode,"Value",strTemp,L"1");
                mrxMap[strName] = strTemp;
                pRxNode = pRxNode->NextSiblingElement();
            }

            mvstrTxLoss.push_back(mtxMap[L"B"]);
            mvstrTxLoss.push_back(mtxMap[L"G"]);
            mvstrTxLoss.push_back(mtxMap[L"N"]);
            mvstrTxLoss.push_back(mtxMap[L"AC"]);

            mvstrRxLoss.push_back(mrxMap[L"B"]);
            mvstrRxLoss.push_back(mrxMap[L"G"]);
            mvstrRxLoss.push_back(mrxMap[L"N"]);
            mvstrRxLoss.push_back(mrxMap[L"AC"]);
        }
        return true;
    }

    bool JGWWiFiTestConfig::LoadWiFiTestCacheXML(S_WLAN_CONFIG& sWlanConfig,std::string strWiFiConfigXML)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if ( !ptrXmlDoc->LoadFile( strWiFiConfigXML.c_str() ) )
        {
            return false;
        }

        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllWiFiParamNode(xmlRoot) )
        {
            return false;
        }	

        return true;
    }

    bool JGWWiFiTestConfig::PareseAllWiFiParamNode(const TiXmlElement* xmlRoot)
    {
        const TiXmlElement*	pConfigXmlNode = NULL;
        pConfigXmlNode = xmlRoot->FirstChildElement("config");
        IsNull(pConfigXmlNode,false);
        //! ½âÎöConfigs
        IsNull(PareseConfigNodeToCacheXML(pConfigXmlNode),false);

        const TiXmlElement*	pTempXmlNode = NULL;
        pTempXmlNode = xmlRoot->FirstChildElement("test");
        IsNull(pTempXmlNode,false);

        while ( pTempXmlNode )
        {
            IsNull(PareseTestNodeToCacheXML(pTempXmlNode),false);
            pTempXmlNode = pTempXmlNode->NextSiblingElement();
        }
        return true;

        return true;
    }

    bool JGWWiFiTestConfig::PareseConfigNodeToCacheXML(const TiXmlElement* xmlRoot)
    {
        std::wstring strTemp,strBoardDataFilePath;

        GetElementTextValue(xmlRoot,"BoardDataFilePath",strTemp,L"wlanConfig.bin");
        if(PathIsRelative(strTemp.c_str()))
        {
            JGW_GetModuleWorkPath(strBoardDataFilePath);
            strBoardDataFilePath += strTemp;
            strTemp = strBoardDataFilePath;
        }
        mpSWlanConfig->mstrBoardDataFilePath = w2a(strTemp);

        GetElementTextValue(xmlRoot,"WlanBatFilePath",strTemp,L"wlan.bat");
        if(PathIsRelative(strTemp.c_str()))
        {
            JGW_GetModuleWorkPath(mpSWlanConfig->mstrWlanBatFilePath);
            mpSWlanConfig->mstrWlanBatFilePath += strTemp;
            strTemp = mpSWlanConfig->mstrWlanBatFilePath;
        }

        mpSWlanConfig->mstrWlanBatDir = JGW_GetPathOfFile(mpSWlanConfig->mstrWlanBatFilePath.c_str());

        GetElementTextValue(xmlRoot,"ErrorReTest",strTemp,L"3");
        mpSWlanConfig->mnErrorReTest = _ttoi(strTemp.c_str());

        return true;
    }

    struct  S_WLAN_TEST_NODE_INFO_ITEM
    {
        std::wstring mstrValue;
        std::wstring mstrRange;
    };

    void JGWWiFiTestConfig::PareseInfoItemValue(size_t txChainLen,const wchar_t* strValue,std::vector<std::wstring>& vstrValue)
    {
        std::vector<std::wstring> vstrTemp;
        JGW_ParserStrW(strValue,L";",vstrTemp);
        while (txChainLen > vstrTemp.size()) vstrTemp.push_back(vstrTemp.at(0));
        for (std::vector<std::wstring>::iterator it = vstrTemp.begin();
            it != vstrTemp.end();
            ++ it)
        {
            JGW_ParserStrW(it->c_str(),L",",vstrValue);
        }
    }

    void JGWWiFiTestConfig::BuildLoss(size_t nTxChain,size_t channelSize,std::vector<std::vector<float>>& vvLoss,std::vector<std::wstring>& vstrLoss)
    {
        for (size_t i = 0;i < vstrLoss.size();i ++)
        {
            std::vector<float> vfLoss;
            std::wstring strLoss = vstrLoss.at(i);
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(strLoss.c_str(),L";",vstrTemp);
            while (nTxChain > vstrTemp.size())  vstrTemp.push_back(vstrTemp.at(0));
            JGW_ParserFloatW(vstrTemp.at(nTxChain - 1).c_str(),L",",vfLoss);
            while (channelSize > vfLoss.size()) vfLoss.push_back(vfLoss.at(0));
            vvLoss.push_back(vfLoss);
        }

#if 0
        for (size_t i = 0;i < vstrLoss.size();i ++)
        {
            std::string strLoss = vstrLoss.at(i);
            std::vector<std::string> vstrTemp;

            JGW_ParserStrA(strLoss.c_str(),";",vstrTemp);
            std::vector<float> vfLoss;
            while (txChainLen > vstrTemp.size()) vstrTemp.push_back(vstrTemp.at(0));
            for (size_t x = 0;x < vstrTemp.size();x ++)
            {
                strLoss = vstrTemp.at(x);
                std::vector<float> vfTemp;
                JGW_ParserFloatA(strLoss.c_str(),",",vfTemp);
                while (channelSize > vfTemp.size()) vfTemp.push_back(vfTemp.at(0));
                vfLoss.swap(vfTemp);
            }
            vvLoss.push_back(vfLoss);
        }
#endif
    }

    void JGWWiFiTestConfig::BuldInfoItemParam(size_t txChainLen,size_t nDataRateSize,size_t channelSize,std::vector<std::vector<int>>& vvvParam,std::wstring strParam)
    {
        std::vector<std::wstring> vstrTemp;
        JGW_ParserStrW(strParam.c_str(),L";",vstrTemp);
        while (nDataRateSize > vstrTemp.size())  vstrTemp.push_back(vstrTemp.at(0));

        for (std::vector<std::wstring>::iterator it = vstrTemp.begin();
            it != vstrTemp.end();
            ++ it)
        {
            std::vector<int> vIntTemp;
            JGW_ParserIntW(it->c_str(),L",",vIntTemp);
            while (channelSize > vIntTemp.size()) vIntTemp.push_back(vIntTemp.at(0));
            vvvParam.push_back(vIntTemp);
        }
    }

    void JGWWiFiTestConfig::BuldInfoItemParam(size_t txChainLen,size_t nDataRateSize,size_t channelSize,std::vector<std::vector<float>>& vvvParam,std::wstring strParam)
    {
        std::vector<std::wstring> vstrTemp;
        JGW_ParserStrW(strParam.c_str(),L";",vstrTemp);
        while (nDataRateSize > vstrTemp.size())  vstrTemp.push_back(vstrTemp.at(0));

        for (std::vector<std::wstring>::iterator it = vstrTemp.begin();
            it != vstrTemp.end();
            ++ it)
        {
            std::vector<float> vIntTemp;
            JGW_ParserFloatW(it->c_str(),L",",vIntTemp);
            while (channelSize > vIntTemp.size()) vIntTemp.push_back(vIntTemp.at(0));
            vvvParam.push_back(vIntTemp);
        }
    }

    bool JGWWiFiTestConfig::PareseTestNodeToCacheXML(const TiXmlElement* xmlTestNode)
    {
        const TiXmlElement*	pTempXmlNode = NULL;
        pTempXmlNode = xmlTestNode->FirstChildElement("InfoItem");
        std::map<std::wstring,S_WLAN_TEST_NODE_INFO_ITEM> vInfoItem;
        S_WLAN_TEST_NODE_INFO_ITEM sWlanTestNodeInfoItem;
        std::wstring strName;
        IsNull(pTempXmlNode,false);

        while ( pTempXmlNode )
        {
            GetElementAttributeValue(pTempXmlNode,"Name",strName);
            GetElementAttributeValue(pTempXmlNode,"Value",sWlanTestNodeInfoItem.mstrValue);
            GetElementAttributeValue(pTempXmlNode,"Rang",sWlanTestNodeInfoItem.mstrRange);
            vInfoItem.insert(std::map<std::wstring,S_WLAN_TEST_NODE_INFO_ITEM>::value_type(strName,sWlanTestNodeInfoItem));
            pTempXmlNode = pTempXmlNode->NextSiblingElement("InfoItem");
        }

        //! add S_WLAN_MODE_CONFIG;
        S_WLAN_MODE_CONFIG sWlanModeConfig;
        std::wstring strTemp;
        std::vector<std::wstring> vstrValue;
        const S_WLAN_TX_RX_DATA_RATE_INFO* psWlanTxRxDataRateInfo = NULL; 
        //! TxChain
        std::wstring strValue = vInfoItem[L"TxChain"].mstrValue;
        if (strValue.empty()) strValue = L"1";
        sWlanModeConfig.mnTxChain = _ttoi(strValue.c_str());

        //! DataRate
        strValue = vInfoItem[L"DataRate"].mstrValue;
        vstrValue.clear();
        JGW_ParserStrW(strValue.c_str(),L",",vstrValue);

        for (std::vector<std::wstring>::iterator it = vstrValue.begin();
            it != vstrValue.end();
            ++ it)
        {
            psWlanTxRxDataRateInfo = GetWlanTxRxDataRateInfoToDataRateName(it->c_str());
            if (!psWlanTxRxDataRateInfo) return false;

            sWlanModeConfig.mveWlanMode.push_back(GetWlanModeToTxDataRate(psWlanTxRxDataRateInfo->meWlanTxDataRate));
            sWlanModeConfig.mvWlanTxDataRate.push_back(psWlanTxRxDataRateInfo->meWlanTxDataRate);
            sWlanModeConfig.mvWlanRxDataRate.push_back(psWlanTxRxDataRateInfo->meWlanRxDataRate);
        }

        //! Channel
        strValue = vInfoItem[L"Channel"].mstrValue;
        vstrValue.clear();
        JGW_ParserIntW(strValue.c_str(),L",",sWlanModeConfig.mvFrequency);

        //! RF Port
        strValue = vInfoItem[L"RFPort"].mstrValue;
        if (strValue.empty()) strValue = L"1";
        sWlanModeConfig.mnRfPort = _ttoi(strValue.c_str());


        std::vector<std::vector<float>> vvTxLoss,vvRxLoss,vvfPower,vvfPowerRange,vvfRssi,vvfRssiRange,vvfPER,vvfExpectPower;
        std::vector<std::vector<int>> vvRxTest,vvTxTest;


        BuildLoss(sWlanModeConfig.mnTxChain,sWlanModeConfig.mvFrequency.size(),vvRxLoss,mvstrRxLoss);
        BuildLoss(sWlanModeConfig.mnTxChain,sWlanModeConfig.mvFrequency.size(),vvTxLoss,mvstrTxLoss);

        strValue = vInfoItem[L"Rx"].mstrValue;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvRxTest,strValue);

        strValue = vInfoItem[L"Tx"].mstrValue;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvTxTest,strValue);

        strValue = vInfoItem[L"ExpectedPower"].mstrValue;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvfExpectPower,strValue);

        strValue = vInfoItem[L"Power"].mstrValue;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvfPower,strValue);

        strValue = vInfoItem[L"PER"].mstrValue;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvfPER,strValue);

        strValue = vInfoItem[L"RSSI"].mstrValue;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvfRssi,strValue);

        strValue = vInfoItem[L"Power"].mstrRange;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvfPowerRange,strValue);

        strValue = vInfoItem[L"RSSI"].mstrRange;
        BuldInfoItemParam(1,sWlanModeConfig.mvWlanRxDataRate.size(),sWlanModeConfig.mvFrequency.size(),vvfRssiRange,strValue);


        for (size_t y = 0;y < sWlanModeConfig.mvWlanRxDataRate.size();y ++)
        {
            for (size_t z = 0;z < sWlanModeConfig.mvFrequency.size();z ++)
            {
                S_WLAN_TEST_ITEM_KPI sWlanTestItemKPI = {0};

                sWlanTestItemKPI.mfTxLoss = vvTxLoss.at(sWlanModeConfig.mveWlanMode.at(y)).at(z);
                sWlanTestItemKPI.mfRxLoss = vvRxLoss.at(sWlanModeConfig.mveWlanMode.at(y)).at(z);

                sWlanTestItemKPI.mbRxTest = vvRxTest.at(y).at(z) == 1;
                sWlanTestItemKPI.mbTxTest = vvTxTest.at(y).at(z) == 1;

                sWlanTestItemKPI.msWlanRxTestItemKpi.mfPER = vvfPER.at(y).at(z);
                sWlanTestItemKPI.msWlanRxTestItemKpi.mfRssi = vvfRssi.at(y).at(z);
                sWlanTestItemKPI.msWlanRxTestItemKpi.mfRssiRange = vvfRssiRange.at(y).at(z);
                sWlanTestItemKPI.msWlanTxTestItemKpi.mfExpectPower = vvfExpectPower.at(y).at(z);
                sWlanTestItemKPI.msWlanTxTestItemKpi.mfPower = vvfPower.at(y).at(z);
                sWlanTestItemKPI.msWlanTxTestItemKpi.mfPowerRange = vvfPowerRange.at(y).at(z);

                sWlanModeConfig.mvWlanTestItemKPI.push_back(sWlanTestItemKPI);
            }
        }

        mpSWlanConfig->mvsWlanModeConfig.push_back(sWlanModeConfig);
        return true;
    }











#if 0
    bool JGWWiFiTestConfig::LoadWiFiTestConfig(std::wstring strFilePath,S_WiFiTest_Config& sWiFiTestConfig)
    {
        if (strFilePath.empty())
        {
            DefaultWiFiTestConfig(sWiFiTestConfig);
            return false;
        }

        return true;
    }

    void JGWWiFiTestConfig::DefaultWiFiTestConfig(S_WiFiTest_Config& sWiFiTestConfig)
    {
        sWiFiTestConfig.msMonitorInfo.m_cInterfaceType = 0;
        sWiFiTestConfig.msMonitorInfo.m_nGpibInterface = 0;
        sWiFiTestConfig.msMonitorInfo.m_nPrimaryAddress = 20;
        sWiFiTestConfig.msMonitorInfo.m_nSecAddress = 96;
        sWiFiTestConfig.msMonitorInfo.m_strModelName = L"CMW500";
        sWiFiTestConfig.mstrBoardDataFilePath = "c:\\wlanConfig.bin";
        sWiFiTestConfig.mstrWlanBatFilePath = L"c://wlan.bat";
        sWiFiTestConfig.mstrWlanBatDir = L"c://";

        S_WLAN_TX_MODE_CONFIG sWlanTxModeConfig;

        sWlanTxModeConfig.mvChannel.push_back(2412);
        sWlanTxModeConfig.meWlanMode = E_B_WLAN_MODE;
        sWlanTxModeConfig.mvBandWidth.push_back(0);
        sWlanTxModeConfig.mvfPower.push_back(12.00f);
        sWlanTxModeConfig.mvPowerRange.push_back(5);
        sWlanTxModeConfig.mvTxChain.push_back(1);
        sWlanTxModeConfig.mvRfPort.push_back(1);
        sWlanTxModeConfig.mvfRxLoss.push_back(1.00f);

        sWiFiTestConfig.mvWlanModeConfig.push_back(sWlanTxModeConfig);
        sWlanTxModeConfig.meWlanMode = E_G_WLAN_MODE;
        sWiFiTestConfig.mvWlanModeConfig.push_back(sWlanTxModeConfig);
        sWlanTxModeConfig.meWlanMode = E_N_WLAN_MODE;
        sWiFiTestConfig.mvWlanModeConfig.push_back(sWlanTxModeConfig);
        sWlanTxModeConfig.meWlanMode = E_AC_WLAN_MODE;
        sWiFiTestConfig.mvWlanModeConfig.push_back(sWlanTxModeConfig);
    }
#endif
}
