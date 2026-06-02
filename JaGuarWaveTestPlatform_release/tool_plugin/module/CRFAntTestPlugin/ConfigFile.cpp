#include "StdAfx.h"

#include "ConfigFile.h"
#include "CRFAntTestThread.h"

#include <JGW_QMSLFuncPlugin/CalFreq.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CConfigFile::CConfigFile(void):m_strCacheIniPath(L"")
        ,m_strConfigIniPath(L"")
    {
    }


    CConfigFile::~CConfigFile(void)
    {
    }

    bool CConfigFile::LoadMonitorConfig()
    {
        m_sRfConfigInfo.m_sMonitorInfo.m_strModelName = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"MODEL");
        std::wstring strAddressType = m_pConfigIniFile.GetIniKeyValue(L"MONITOR",L"ADDRESS");
        std::wstring strTemp;

        if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"GPIB"))
        {
            m_sRfConfigInfo.m_sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;

            strTemp = m_pConfigIniFile.GetIniKeyValue(L"GPIB",L"PartialClassName",L"NIGPIB");
            m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
            if (0 != strTemp.compare(L"NIGPIB")) m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = 1;

            m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"BoardID",0);
            m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"PrimAddress",20);
            m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = m_pConfigIniFile.GetIniKeyIntValue(L"GPIB",L"ScndAddress",0);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"VISA"))
        {
            m_sRfConfigInfo.m_sMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
            m_sRfConfigInfo.m_sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"VISA",L"VisaAddress",L"TCPIP0::CMW50050-134371::inst0::INSTR"));
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strAddressType.c_str(),L"TCPIP"))
        {
            m_sRfConfigInfo.m_sMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;

            m_sRfConfigInfo.m_sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = JGW_W2A(m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"IPAddress",L"10.62.8.197"));

            strTemp = m_pConfigIniFile.GetIniKeyValue(L"TCPIP",L"SubInstrument",L"0");
            m_sRfConfigInfo.m_sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument = _ttoi(strTemp.c_str());
        }
        else
        {
            m_sRfConfigInfo.m_sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
            m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = 0;
            m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = 20;
            m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 0;
        }

        return true;
    }

    bool CConfigFile::ChangeMonitorConfig()
    {
        m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"MODEL",m_sRfConfigInfo.m_sMonitorInfo.m_strModelName.c_str());

        if (E_GPIB_HIVE == m_sRfConfigInfo.m_sMonitorInfo.m_cInterfaceType)
        {
            m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"ADDRESS",L"GPIB");

            if (AGILENT_GPIB_TYPE == m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType)
            {
                m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PartialClassName",L"AGILENTGPIB");
            }
            else
            {
                m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PartialClassName",L"NIGPIB");
            }
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"BoardID",m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface);
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"PrimAddress",m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress);
            m_pConfigIniFile.SetIniKeyValue(L"GPIB",L"ScndAddress",m_sRfConfigInfo.m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress);
        }
        else if (E_VISA_HIVE == m_sRfConfigInfo.m_sMonitorInfo.m_cInterfaceType)
        {
            m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"ADDRESS",L"VISA");
            m_pConfigIniFile.SetIniKeyValue(L"VISA",L"VisaAddress",JGW_A2W(m_sRfConfigInfo.m_sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress).c_str());
        }
        else if (E_TCP_IP_HIVE == m_sRfConfigInfo.m_sMonitorInfo.m_cInterfaceType)
        {
            m_pConfigIniFile.SetIniKeyValue(L"MONITOR",L"ADDRESS",L"TCPIP");
            m_pConfigIniFile.SetIniKeyValue(L"TCPIP",L"IPAddress",JGW_A2W(m_sRfConfigInfo.m_sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress).c_str());
            m_pConfigIniFile.SetIniKeyValue(L"TCPIP",L"SubInstrument",m_sRfConfigInfo.m_sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument);
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

    bool CConfigFile::LoadConfigFile()
    {
        m_sRfConfigInfo.m_sWcdmaInfo.m_sTestBandInfo.Clear();
        m_sRfConfigInfo.m_sLteInfo.m_sTestBandInfo.Clear();
        m_sRfConfigInfo.m_sCdmaInfo.m_sTestBandInfo.Clear();
        m_sRfConfigInfo.m_sTdscdmaInfo.m_sTestBandInfo.Clear();
        m_sRfConfigInfo.m_sGsmInfo.m_sTestBandInfo.Clear();
        m_sRfConfigInfo.m_sGsmInfo.m_mGsmBandNormInfo.clear();

        ISNOTNULL( m_pConfigIniFile.InitIniFilePath(m_strConfigIniPath.c_str()) );
        ISNOTNULL( m_pCacheIniFile.InitIniFilePath(m_strCacheIniPath.c_str()) );
        m_sRfConfigInfo.m_nReTestTimeOut = m_pConfigIniFile.GetIniKeyIntValue(L"Setting",L"ReTestTime");
        m_sRfConfigInfo.m_bIsExportTestInfo = m_pConfigIniFile.GetIniKeyBoolValue(L"Setting",L"IsExportTestInfo");

        LoadMonitorConfig();

        m_sRfConfigInfo.m_nErrorReTestCount = m_pCacheIniFile.GetIniKeyIntValue(L"Setting",L"ErrorReTestCount");
        m_sRfConfigInfo.m_bIsTestRx = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"RX_NS");
        m_sRfConfigInfo.m_sModeSelecter.lte = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"LTE_NS");
        m_sRfConfigInfo.m_sModeSelecter.tds = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"TDSCDMA_NS");
        m_sRfConfigInfo.m_sModeSelecter.wcdma = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"WCDMA_NS");
        m_sRfConfigInfo.m_sModeSelecter.cdma = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"CDMA2000_NS");
        m_sRfConfigInfo.m_sModeSelecter.gsm = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"GSM_NS");
        m_sRfConfigInfo.m_sModeSelecter.gps = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"GPS_NS");
        m_sRfConfigInfo.m_sModeSelecter.wlan = m_pCacheIniFile.GetIniKeyBoolValue(L"SELECTER",L"WIFI_NS");
#ifdef LTE_NS_TEST_
        if(m_sRfConfigInfo.m_sModeSelecter.lte)
        {
            ISNOTNULL(LoadLteConfig());
        }
#endif
#ifdef GSP_NS_SNR
        if(m_sRfConfigInfo.m_sModeSelecter.gps)
        {
            ISNOTNULL(LoadGpsConfig());
        }
#endif
#ifdef TDSCDMA_NS_TEST_
        if(m_sRfConfigInfo.m_sModeSelecter.tds)
        {
            ISNOTNULL(LoadTdscdmaConfig());
        }
#endif
#ifdef WCDMA_NS_TEST_
        if(m_sRfConfigInfo.m_sModeSelecter.wcdma)
        {
            ISNOTNULL(LoadWcdmaConfig());
        }
#endif
#ifdef CDMA_NS_TEST_
        if(m_sRfConfigInfo.m_sModeSelecter.cdma)
        {
            ISNOTNULL(LoadCdmaConfig());
        }
#endif
#ifdef GSM_NS_TEST_
        if(m_sRfConfigInfo.m_sModeSelecter.gsm)
        {
            ISNOTNULL(LoadGsmConfig());
        }
#endif
#ifdef WLAN_NS_TEST_
        if (m_sRfConfigInfo.m_sModeSelecter.wlan)
        {
            ISNOTNULL(LoadWlanConfig());
        }
#endif
        return true;
    }

    void CConfigFile::SetCacheConfigFilePath(const wchar_t* strIniPath)
    {
        m_strCacheIniPath = strIniPath;
    }

    void CConfigFile::SetConfigFilePath(const wchar_t* strconfigIniPath)
    {
        m_strConfigIniPath = strconfigIniPath;
    }

    bool CConfigFile::LoadLteConfig()
    {
        COMMUNICATIONSSYS esys = TECHNOLOGY_LTE;
        ISNOTNULL(LoadSysRfConfig(esys,m_sRfConfigInfo.m_sLteInfo.m_sTestBandInfo));
        float fTempLim = 0;
        float fTemp = 0;
        int	  nTemp = 0;
        //! Max Pwr
        {
            m_sRfConfigInfo.m_sLteInfo.m_bTestMaxPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMaxPWR");
            fTemp = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRLim");
            fTempLim = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRLimErr");
            m_sRfConfigInfo.m_sLteInfo.m_fMaxPwrLim[MIN_VALUE] = fTemp - fTempLim;
            m_sRfConfigInfo.m_sLteInfo.m_fMaxPwrLim[MAX_VALUE] = fTemp + fTempLim;
        }
        // 	//! Min Pwr
        // 	{
        // 		m_sRfConfigInfo.m_sLteInfo.m_bTestMinPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMinPWR");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fMinPwrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrHigh");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fMinPwrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrLow");
        // 	}
        //! ACLR
        // 	{
        // 		m_sRfConfigInfo.m_sLteInfo.m_bTestAclr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestACLR");
        // 
        // 		m_sRfConfigInfo.m_sLteInfo.m_fEutraAclrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EutraACLRMin");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fEutraAclrLim[MAX_VALUE] = 99999;
        // 		
        // 		m_sRfConfigInfo.m_sLteInfo.m_fUtra1AclrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"UtraOneACLRMin");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fUtra1AclrLim[MAX_VALUE] = 99999;
        // 		
        // 		m_sRfConfigInfo.m_sLteInfo.m_fUtra2AclrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"UtraTwoACLRMin");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fUtra2AclrLim[MAX_VALUE] = 99999;
        // 	}
        //! OBW
        // 	{
        // 		m_sRfConfigInfo.m_sLteInfo.m_bTestOBW = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestOBW");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fObwLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"OBWLimMax");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fObwLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"OBWLimMin");
        // 	}

        //! Freq Err
        // 	{
        // 		m_sRfConfigInfo.m_sLteInfo.m_bTestFrqErr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestFreqErr");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fFreqErrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMax");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fFreqErrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMin");
        // 	}
        //! SEM
        // 	{
        // 		m_sRfConfigInfo.m_sLteInfo.m_bTestSem = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestSEM");
        // 		m_sRfConfigInfo.m_sLteInfo.m_nSemLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMLimMax");
        // 		m_sRfConfigInfo.m_sLteInfo.m_nSemLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMLimMin");
        // 	}
        //! EVM 
        // 	{
        // 		m_sRfConfigInfo.m_sLteInfo.m_bTestEvm = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestEVM");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fEvmLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMin");
        // 		m_sRfConfigInfo.m_sLteInfo.m_fEvmLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMax");
        // 	}
        //! Rx Level 
        {
            m_sRfConfigInfo.m_sLteInfo.m_bTestRxLevel = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestRxLevel");
            fTempLim = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelPowerRange");
            m_sRfConfigInfo.m_sLteInfo.m_fRxLevelCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelCellPower");
            m_sRfConfigInfo.m_sLteInfo.m_fRxLevelLim[MAX_VALUE] = m_sRfConfigInfo.m_sLteInfo.m_fRxLevelCellPwr + fTempLim;
            m_sRfConfigInfo.m_sLteInfo.m_fRxLevelLim[MIN_VALUE] = m_sRfConfigInfo.m_sLteInfo.m_fRxLevelCellPwr - fTempLim;

            // 		m_sRfConfigInfo.m_sLteInfo.m_fPrxLevelLoss = m_pConfigIniFile.GetIniKeyFloatValue(g_szRfConfigSecW[esys],L"PRXLevel_LOSS");
            // 		m_sRfConfigInfo.m_sLteInfo.m_fDrxLevelLoss = m_pConfigIniFile.GetIniKeyFloatValue(g_szRfConfigSecW[esys],L"DRXLevel_LOSS");
        }


        //! bler 
        {
            m_sRfConfigInfo.m_sLteInfo.m_bTestBler = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestBler");
            m_sRfConfigInfo.m_sLteInfo.m_fBerLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BLER_ERR");
            m_sRfConfigInfo.m_sLteInfo.m_fBlerCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BLER_POWER");
            m_sRfConfigInfo.m_sLteInfo.m_fBerLim[MIN_VALUE] = 0;
        }
        return true;
    }

    bool CConfigFile::LoadTdscdmaConfig()
    {
        COMMUNICATIONSSYS esys = TECHNOLOGY_TD_SCDMA;
        ISNOTNULL(LoadSysRfConfig(esys,m_sRfConfigInfo.m_sTdscdmaInfo.m_sTestBandInfo));
        float fTempLim = 0;
        float fTemp = 0;
        int	  nTemp = 0;
        //! Max Pwr
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestMaxPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMaxPWR");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fMaxPwrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRUpLim");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fMaxPwrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRDownLim");
        }
        //! Min Pwr
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestMinPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMinPWR");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fMinPwrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrHigh");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fMinPwrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrLow");
        }
        //! ACLR
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestAclr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestACLR");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrNega16Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrNega16MMin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrNega16Lim[MAX_VALUE] = 99999;

            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrNega32Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrNega32MMin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrNega32Lim[MAX_VALUE] = 99999;

            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrPosi16Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrPosi16MMin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrPosi16Lim[MAX_VALUE] = 99999;

            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrPosi32Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrPosi32MMin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fAclrPosi32Lim[MAX_VALUE] = 99999;
        }
        //! OBW
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestOBW = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestOBW");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fObwLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"OBWLimMax");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fObwLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"OBWLimMin");
        }

        //! Freq Err
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestFrqErr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestFreqErr");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fFreqErrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMax");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fFreqErrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMin");
        }
        //! SEM
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestSem = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestSEM");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemABLim[MIN_VALUE] = -100;
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemCDLim[MIN_VALUE] = -100;
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemDELim[MIN_VALUE] = -100;
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemEDLim[MIN_VALUE] = -100;
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemDCLim[MIN_VALUE] = -100;
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemBALim[MIN_VALUE] = -100;
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemABLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMABMargin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemCDLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMCDMargin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemDELim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMDEMargin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemEDLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMEDMargin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemDCLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMDCMargin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_nSemBALim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMBAMargin");
        }
        //! EVM 
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestEvm = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestEVM");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fEvmLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMin");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fEvmLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMax");
        }
        //! PCDE
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestPCde = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestPCDE");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fPeakCDELim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"PeakCDELim");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fPeakCDELim[MIN_VALUE] = -60;
        }
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestPvt = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestPVT");
        }
        //! Rx Level 
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestRxLevel = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestRxLevel");
            fTempLim = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelPowerRange");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fRxLevelCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelCellPower");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fRxLevelLim[MAX_VALUE] = m_sRfConfigInfo.m_sTdscdmaInfo.m_fRxLevelCellPwr + fTempLim;
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fRxLevelLim[MIN_VALUE] = m_sRfConfigInfo.m_sTdscdmaInfo.m_fRxLevelCellPwr - fTempLim;
        }
        //! bler 
        {
            m_sRfConfigInfo.m_sTdscdmaInfo.m_bTestBler = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestBer");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fBerLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BER_ERR");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fBlerCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BER_POWER");
            m_sRfConfigInfo.m_sTdscdmaInfo.m_fBerLim[MIN_VALUE] = 0;
        }
        return true;
    }

    bool CConfigFile::LoadWcdmaConfig()
    {
        COMMUNICATIONSSYS esys = TECHNOLOGY_WCDMA;
        ISNOTNULL(LoadSysRfConfig(TECHNOLOGY_WCDMA,m_sRfConfigInfo.m_sWcdmaInfo.m_sTestBandInfo));
        float fTempLim = 0;
        float fTemp = 0;
        int	  nTemp = 0;
        //! Max Pwr
        {
            m_sRfConfigInfo.m_sWcdmaInfo.m_bTestMaxPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMaxPWR");
            m_sRfConfigInfo.m_sWcdmaInfo.m_fMaxPwrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRUpLim");
            m_sRfConfigInfo.m_sWcdmaInfo.m_fMaxPwrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRDownLim");
        }
        //! Min Pwr
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestMinPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMinPWR");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fMinPwrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrHigh");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fMinPwrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrLow");
        //}
        //! ACLR
        // 	{
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_bTestAclr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestACLR");
        // 
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrNega16Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrNega16MMin");
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrNega16Lim[MAX_VALUE] = 99999;
        // 
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrNega32Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrNega32MMin");
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrNega32Lim[MAX_VALUE] = 99999;
        // 
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrPosi16Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrPosi16MMin");
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrPosi16Lim[MAX_VALUE] = 99999;
        // 
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrPosi32Lim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"AclrPosi32MMin");
        // 		m_sRfConfigInfo.m_sWcdmaInfo.m_fAclrPosi32Lim[MAX_VALUE] = 99999;
        // 	}
        //! OBW
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestOBW = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestOBW");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fObwLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"OBWLimMax");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fObwLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"OBWLimMin");
        //}

        //! Freq Err
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestFrqErr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestFreqErr");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fFreqErrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMax");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fFreqErrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMin");
        //}
        //! SEM
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestSem = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestSEM");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemABLim[MIN_VALUE] = -100;
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemBCLim[MIN_VALUE] = -100;
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemCDLim[MIN_VALUE] = -100;
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemEFLim[MIN_VALUE] = -100;
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemFELim[MIN_VALUE] = -100;
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemDCLim[MIN_VALUE] = -100;
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemCBLim[MIN_VALUE] = -100;
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemBALim[MIN_VALUE] = -100;

        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemABLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMABMargin");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemBCLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMBCMargin");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemCDLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMCDMargin");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemEFLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMEFMargin");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemFELim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMFEMargin");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemDCLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMDCMargin");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemCBLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMCBMargin");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_nSemBALim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyIntValue(g_szRfNormSecW[esys],L"SEMBAMargin");
        //}
        //! EVM 
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestEvm = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestEVM");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fEvmLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMax");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fEvmLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMin");
        //}
        //! PCDE
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestPCde = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestPCDE");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fPeakCDELim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"PeakCDELim");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fPeakCDELim[MIN_VALUE] = -60;
        //}
        //! PhaseErr
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestPhaseError = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestPhaseError");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fPhaseErrRmsLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"PhaseErrRMSLim");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fPhaseErrRmsLim[MIN_VALUE] = 0;
        //}
        //! IQOffset
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestIQOffset = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestIQoffset");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fIQOffsetLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"IQoffsetLim");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fIQOffsetLim[MIN_VALUE] = -100;
        //}
        //! IQBalance
        //{
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_bTestIQBalance = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestIQBalance");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fIQBalance[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"IQBalanceLim");
        //	m_sRfConfigInfo.m_sWcdmaInfo.m_fIQBalance[MIN_VALUE] = -100;
        //}

        //! Rx Level 
        {
            m_sRfConfigInfo.m_sWcdmaInfo.m_bTestRxLevel = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestRxLevel");
            fTempLim = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelPowerRange");
            m_sRfConfigInfo.m_sWcdmaInfo.m_fRxLevelCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelCellPower");
            m_sRfConfigInfo.m_sWcdmaInfo.m_fRxLevelLim[MAX_VALUE] = m_sRfConfigInfo.m_sWcdmaInfo.m_fRxLevelCellPwr + fTempLim;
            m_sRfConfigInfo.m_sWcdmaInfo.m_fRxLevelLim[MIN_VALUE] = m_sRfConfigInfo.m_sWcdmaInfo.m_fRxLevelCellPwr - fTempLim;

            //m_sRfConfigInfo.m_sWcdmaInfo.m_fPrxLevelLoss = m_pConfigIniFile.GetIniKeyFloatValue(g_szRfConfigSecW[esys],L"PRXLevel_LOSS");
            //m_sRfConfigInfo.m_sWcdmaInfo.m_fDrxLevelLoss = m_pConfigIniFile.GetIniKeyFloatValue(g_szRfConfigSecW[esys],L"DRXLevel_LOSS");
        }
        //! bler 
        {
            m_sRfConfigInfo.m_sWcdmaInfo.m_bTestBler = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestBer");
            m_sRfConfigInfo.m_sWcdmaInfo.m_fBerLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BER_ERR");
            m_sRfConfigInfo.m_sWcdmaInfo.m_fBlerCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BER_POWER");
            m_sRfConfigInfo.m_sWcdmaInfo.m_fBerLim[MIN_VALUE] = 0;
        }
        return true;
    }

    bool CConfigFile::LoadCdmaConfig()
    {
        COMMUNICATIONSSYS esys = TECHNOLOGY_CDMA;
        ISNOTNULL(LoadSysRfConfig(TECHNOLOGY_CDMA,m_sRfConfigInfo.m_sCdmaInfo.m_sTestBandInfo));
        float fTempLim = 0;
        float fTemp = 0;
        int	  nTemp = 0;
        //! Max Pwr
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestMaxPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMaxPWR");
            m_sRfConfigInfo.m_sCdmaInfo.m_fMaxPwrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRUpLim");
            m_sRfConfigInfo.m_sCdmaInfo.m_fMaxPwrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MaxPWRDownLim");
        }
        //! Min Pwr
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestMinPwr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestMinPWR");
            m_sRfConfigInfo.m_sCdmaInfo.m_fMinPwrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrHigh");
            m_sRfConfigInfo.m_sCdmaInfo.m_fMinPwrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinPwrLow");
        }
        //! Freq Err
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestFrqErr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestFreqErr");
            m_sRfConfigInfo.m_sCdmaInfo.m_fFreqErrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMax");
            m_sRfConfigInfo.m_sCdmaInfo.m_fFreqErrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"FreqErrLimMin");
        }
        //! EVM 
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestEvm = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestEVM");
            m_sRfConfigInfo.m_sCdmaInfo.m_fEvmLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMax");
            m_sRfConfigInfo.m_sCdmaInfo.m_fEvmLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"EVMLimMin");
        }
        //! ACPR
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestAcpr = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestACPR");
        }
        //! WFQ
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestWFQ = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestWFQ");
            m_sRfConfigInfo.m_sCdmaInfo.m_fWFQLim[MAX_VALUE] = 1;
            m_sRfConfigInfo.m_sCdmaInfo.m_fWFQLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"MinWFQ");
        }
        //! PhaseErr
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestPhaseError = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestPhaseError");
            m_sRfConfigInfo.m_sCdmaInfo.m_fPhaseErrRmsLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"PhaseErrRMSLim");
            m_sRfConfigInfo.m_sCdmaInfo.m_fPhaseErrRmsLim[MIN_VALUE] = 0;
        }
        //! IQBalance
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestIQBalance = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestIQBalance");
            m_sRfConfigInfo.m_sCdmaInfo.m_fIQBalance[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"IQBalanceLim");
            m_sRfConfigInfo.m_sCdmaInfo.m_fIQBalance[MIN_VALUE] = -100;
        }

        //! Rx Level 
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestRxLevel = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestRxLevel");
            fTempLim = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelPowerRange");
            m_sRfConfigInfo.m_sCdmaInfo.m_fRxLevelCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"RxLevelCellPower");
            m_sRfConfigInfo.m_sCdmaInfo.m_fRxLevelLim[MAX_VALUE] = m_sRfConfigInfo.m_sCdmaInfo.m_fRxLevelCellPwr + fTempLim;
            m_sRfConfigInfo.m_sCdmaInfo.m_fRxLevelLim[MIN_VALUE] = m_sRfConfigInfo.m_sCdmaInfo.m_fRxLevelCellPwr - fTempLim;
        }
        //! bler 
        {
            m_sRfConfigInfo.m_sCdmaInfo.m_bTestBler = m_pCacheIniFile.GetIniKeyBoolValue(g_szRfNormSecW[esys],L"IsTestBer");
            m_sRfConfigInfo.m_sCdmaInfo.m_fBerLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BER_ERR");
            m_sRfConfigInfo.m_sCdmaInfo.m_fBlerCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(g_szRfNormSecW[esys],L"BER_POWER");
            m_sRfConfigInfo.m_sCdmaInfo.m_fBerLim[MIN_VALUE] = 0;
        }

        return true;
    }

    bool CConfigFile::LoadGpsConfig()
    {
        m_sRfConfigInfo.m_sGpsInfo.m_ePort = (CMW500_PORT)m_pCacheIniFile.GetIniKeyIntValue(L"GPS_RF_CONFIG",L"BAND_PORT",ERFAC);
        m_sRfConfigInfo.m_sGpsInfo.m_fFreq = m_pCacheIniFile.GetIniKeyFloatValue(L"GPS_RF_CONFIG",L"GPS_GEN_FREQ",L"1575.42");

        m_sRfConfigInfo.m_sGpsInfo.m_fCellPwr = m_pCacheIniFile.GetIniKeyFloatValue(L"GPS_TESTOPTION",L"CellPower",L"-130");
        m_sRfConfigInfo.m_sGpsInfo.m_fSnrLim[MAX_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(L"GPS_TESTOPTION",L"SNRUP",L"38");
        m_sRfConfigInfo.m_sGpsInfo.m_fSnrLim[MIN_VALUE] = m_pCacheIniFile.GetIniKeyFloatValue(L"GPS_TESTOPTION",L"SNRLow",L"32");

        return true;
    }

    bool CConfigFile::CheckWlanBandVal(int nBand)
    {
        switch(nBand)
        {
        case 1:
        case 2:
        case 3:
        case 4:
            return true;
        default:
            m_strErrorMsg = L"当前模式频段不属于WLAN";
            return false;
        }
    }

    bool CConfigFile::CheckGSMBandVal(int nBand)
    {
        switch(nBand)
        {
        case 8:
        case 900:
        case 5:
        case 850:
        case 3:
        case 1800:
        case 2:
        case 1900:
            return true;
        default:
            m_strErrorMsg = L"当前频段不属于GSM";
            return false;
        }
        //	return true;
    }

    bool CConfigFile::LoadGsmConfig()
    {
        COMMUNICATIONSSYS esys = GSM;
        //! 获取频段信息
        ISNOTNULL(LoadSysRfConfig(esys,m_sRfConfigInfo.m_sGsmInfo.m_sTestBandInfo));
        //! 添加每个频段测试标准
        S_GSM_BAND_NORM_INFO sGsmBandNormInfo;
        std::wstring strSec(L"");
        int nBand = 0;

        for (size_t i = 0;i < m_sRfConfigInfo.m_sGsmInfo.m_sTestBandInfo.m_vBandInfo.size();i ++)
        {
            sGsmBandNormInfo.Init();
            nBand = m_sRfConfigInfo.m_sGsmInfo.m_sTestBandInfo.m_vBandInfo.at(i).m_nBand;
            JGW_FormatWString(strSec,L"GSM_%d_TESTOPTION",nBand);
            ISNOTNULL( CheckGSMBandVal(nBand) );

            sGsmBandNormInfo.m_bTestBler = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestBer");
            sGsmBandNormInfo.m_bTestPwrLevel = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestPowerLevel");
            //sGsmBandNormInfo.m_bTestFrqErr = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestFreqError");
            //sGsmBandNormInfo.m_bTestEvm = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestEVM");
            //sGsmBandNormInfo.m_bTestPhaseError = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestPhaseErrorRMS");
            //sGsmBandNormInfo.m_bTestPhaseErrorPeak = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestPhaseErrorPeak");
            //sGsmBandNormInfo.m_bTestPVT = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestPVTime");
            //sGsmBandNormInfo.m_bTestSpectrumSwitch = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestSpectrumSwitch");
            //sGsmBandNormInfo.m_bTestSpectrumModulation = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestSpectrumModulation");
            sGsmBandNormInfo.m_bTestRxLevel = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestRxLevel");
            ISNOTNULL(ParseGsmNormConfig(strSec.c_str(),sGsmBandNormInfo));
            m_sRfConfigInfo.m_sGsmInfo.m_mGsmBandNormInfo.insert(std::make_pair<int,S_GSM_BAND_NORM_INFO>(nBand,sGsmBandNormInfo));
        }

        return true;
    }

    bool CConfigFile::LoadSysRfConfig(COMMUNICATIONSSYS esys,S_SYS_INFO& sSysInfo)
    {
        std::wstring strTemp(L"");
        sSysInfo.Clear();
        // 	std::vector<int> vPort;
        // 	std::vector<int> vDrxPort;
        std::vector<int> vBand;

        strTemp = m_pCacheIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],L"BAND_NUM");
        JGW_FormatWString(m_strErrorMsg,L"%s节点配置频段不能为空",g_szRfConfigSecW[esys]);
        ISNOTNULL( !strTemp.empty() );
        JGW_ParserIntW(strTemp.c_str(),L",",vBand);

        // 	strTemp = m_pCacheIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],L"BAND_PORT");
        // 	JGW_FormatWString(m_strErrorMsg,L"%s频段请配置主端口信息",g_szSysName[esys]);
        // 	ISNOTNULL( !strTemp.empty() );
        // 	JGW_ParserIntW(strTemp.c_str(),L",",vPort);
        // 	ISNOTNULL(vPort.size() >= 1);
        // 
        // 	strTemp = m_pCacheIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],L"BAND_DRX_PORT");
        // 	if ( !strTemp.empty() )
        // 	{
        // 		JGW_ParserIntW(strTemp.c_str(),L",",vDrxPort);
        // 		JGW_FormatWString(m_strErrorMsg,L"%s主辅端口信息配置不一致",g_szSysName[esys]);
        // 		ISNOTNULL(vPort.size() == vDrxPort.size());
        // 	}
        // 	if(vDrxPort.size()==0)vDrxPort.push_back(ERFAO);

        std::wstring strChannelKey(L"");
        S_BAND_INFO sBandInfo;
        S_BAND_CHANNEL_INFO sChannelInfo;
        SBAND_INFO sBandChannelInfo;
        int nPortId = 0;
        std::vector<int> vChannel;
        std::vector<float> vPTxLoss;
        std::vector<float> vPRxLoss;
        std::vector<float> vDRxLoss;
        try
        {
            for( size_t i = 0;i < vBand.size();i ++ )
            {
                sBandInfo.Init();
                sBandInfo.m_nBand = vBand.at(i);
                vChannel.clear();
                vPTxLoss.clear();
                vPRxLoss.clear();
                vDRxLoss.clear();
                //sBandInfo.m_ePort = (CMW500_PORT)(vPort.size()>i?vPort.at(i):vPort.at(vPort.size()-1));
                //sBandInfo.m_eDrxPort = (CMW500_PORT)(vDrxPort.size()>i?vDrxPort.at(i):vDrxPort.at(vDrxPort.size()-1));

                //解析主端口信息
                JGW_FormatWString(strChannelKey,L"BAND%d_PRX_PORT",sBandInfo.m_nBand);
                strTemp = m_pCacheIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],strChannelKey.c_str());
                sBandInfo.m_ePort = (CMW500_PORT)(strTemp.empty()?1:_ttoi(strTemp.c_str()));

                JGW_FormatWString(strChannelKey,L"BAND%d_DRX_PORT",sBandInfo.m_nBand);
                nPortId = m_pCacheIniFile.GetIniKeyIntValue(g_szRfConfigSecW[esys],strChannelKey.c_str());
                if (0 == nPortId)
                {
                    sBandInfo.m_eDrxPort = ERF_UNKNOW;
                }
                else
                {
                    sBandInfo.m_eDrxPort = (CMW500_PORT)nPortId;
                }
                
                //解析信道信息
                JGW_FormatWString(strChannelKey,L"BAND%d_CHAN",sBandInfo.m_nBand);
                strTemp = m_pCacheIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],strChannelKey.c_str());
                if ( strTemp.empty() )
                {
                    m_strErrorMsg = L"信道配置不能为空,请检查配置文件";
                    return false;
                }
                JGW_ParserIntW(strTemp.c_str(),L",",vChannel);

                JGW_FormatWString(strChannelKey,L"BAND%d_PTx_LOSS",sBandInfo.m_nBand);
                strTemp = m_pConfigIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],strChannelKey.c_str(),L"1.0");
                JGW_ParserFloatW(strTemp.c_str(),L",",vPTxLoss);
                if(vPTxLoss.size()==0) vPTxLoss.push_back(1.00f);
                JGW_FormatWString(strChannelKey,L"BAND%d_PRx_LOSS",sBandInfo.m_nBand);
                strTemp = m_pConfigIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],strChannelKey.c_str(),L"1.0");
                JGW_ParserFloatW(strTemp.c_str(),L",",vPRxLoss);
                if(vPRxLoss.size()==0) vPRxLoss.push_back(1.00f);
                JGW_FormatWString(strChannelKey,L"BAND%d_DRx_LOSS",sBandInfo.m_nBand);
                strTemp = m_pConfigIniFile.GetIniKeyValue(g_szRfConfigSecW[esys],strChannelKey.c_str(),L"1.0");
                JGW_ParserFloatW(strTemp.c_str(),L",",vDRxLoss);
                if(vDRxLoss.size()==0) vDRxLoss.push_back(1.00f);

                for ( size_t y = 0;y < vChannel.size();y ++)
                {
                    sBandChannelInfo.InitParam();
                    sChannelInfo.Clear();
                    sBandChannelInfo.m_nBand = sBandInfo.m_nBand;
                    sBandChannelInfo.m_nUpLinkChannel = vChannel.at(y);

                    ISNOTNULL( CCalFreq::CalFreq(esys,sBandChannelInfo) );
                    sChannelInfo.m_fDownLinkFreq = sBandChannelInfo.dFreqDL;
                    sChannelInfo.m_fUpLinkFreq = sBandChannelInfo.dFreqUL;
                    sChannelInfo.m_nDownLinkChannel = sBandChannelInfo.m_nDownloadLinkChannel;
                    sChannelInfo.m_nUpLinkChannel = sBandChannelInfo.m_nUpLinkChannel;
                    sChannelInfo.m_fPTxLoss = (vPTxLoss.size()>y?vPTxLoss.at(y):vPTxLoss.at(vPTxLoss.size()-1));
                    sChannelInfo.m_fPRxLoss = (vPRxLoss.size()>y?vPRxLoss.at(y):vPRxLoss.at(vPRxLoss.size()-1));
                    sChannelInfo.m_fDRxLoss = (vDRxLoss.size()>y?vDRxLoss.at(y):vDRxLoss.at(vDRxLoss.size()-1));
                    sBandInfo.m_vBandChannelInfo.push_back(sChannelInfo);
                }
                sSysInfo.m_vBandInfo.push_back(sBandInfo);
            }
        }
        catch (wchar_t* e)
        {
            m_strErrorMsg = e;
            return false;
        }

        return true;
    }

    bool CConfigFile::ParseGsmNormConfig(const wchar_t* strSec,S_GSM_BAND_NORM_INFO& sGsmBandNormInfo)
    {
        std::wstring strTemp( L"" );
        std::vector<int> vPwrIndex;

        strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"BAND_PowerLevel");
        if( strTemp.empty() )
        {
            m_strErrorMsg = L"当前GSM频段未配置功率等级测试信息";
            return false;
        }
        JGW_ParserIntW(strTemp.c_str(),L",",vPwrIndex);
        S_GSM_BAND_PWL_NORM_INFO* sGsmBandPwlInfo = new S_GSM_BAND_PWL_NORM_INFO[vPwrIndex.size()];
        std::vector<float>	vUpRange;
        std::vector<float>  vLowRange;
        m_strErrorMsg = L"功率等级数量与测试标准不符";

        if( sGsmBandNormInfo.m_bTestPwrLevel)
        {
            vUpRange.clear();
            vLowRange.clear();
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"MaxPWRUpLim");
            JGW_ParserFloatW(strTemp.c_str(),L",",vUpRange);
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"MaxPWRDownLim");
            JGW_ParserFloatW(strTemp.c_str(),L",",vLowRange);
            ISNOTNULL( (vUpRange.size() == vLowRange.size())&&(vUpRange.size() == vPwrIndex.size()));
            for (size_t i = 0;i < vPwrIndex.size();i ++)
            {
                sGsmBandPwlInfo[i].m_nPwrLevel = vPwrIndex.at(i);
                sGsmBandPwlInfo[i].m_fPwrLim[MAX_VALUE] = vUpRange.at(i);
                sGsmBandPwlInfo[i].m_fPwrLim[MIN_VALUE] = vLowRange.at(i);
            }
        }
        //! m_sRfConfigInfo.m_sGsmInfo.m_fPrxLevelLoss = m_pConfigIniFile.GetIniKeyFloatValue(L"GSM_RF_CONFIG",L"PRXLevel_LOSS");
        //! m_sRfConfigInfo.m_sGsmInfo.m_fDrxLevelLoss = m_pConfigIniFile.GetIniKeyFloatValue(L"GSM_RF_CONFIG",L"DRXLevel_LOSS");

        // 	if( sGsmBandNormInfo.m_bTestFrqErr)
        // 	{
        // 		vUpRange.clear();
        // 		vLowRange.clear();
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"FreqErrorUpLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vUpRange);
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"FreqErrorDownLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vLowRange);
        // 		ISNOTNULL( (vUpRange.size() == vLowRange.size())&&(vUpRange.size() == vPwrIndex.size()));
        // 		for (size_t i = 0;i < vPwrIndex.size();i ++)
        // 		{
        // 			sGsmBandPwlInfo[i].m_fFreqErrLim[MAX_VALUE] = vUpRange.at(i);
        // 			sGsmBandPwlInfo[i].m_fFreqErrLim[MIN_VALUE] = vLowRange.at(i);
        // 		}
        // 	}

        // 	if( sGsmBandNormInfo.m_bTestEvm)
        // 	{
        // 		vUpRange.clear();
        // 		vLowRange.clear();
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"EVMUpLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vUpRange);
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"EVMDownLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vLowRange);
        // 		ISNOTNULL( (vUpRange.size() == vLowRange.size())&&(vUpRange.size() == vPwrIndex.size()));
        // 		for (size_t i = 0;i < vPwrIndex.size();i ++)
        // 		{
        // 			sGsmBandPwlInfo[i].m_fEvmLim[MAX_VALUE] = vUpRange.at(i);
        // 			sGsmBandPwlInfo[i].m_fEvmLim[MIN_VALUE] = vLowRange.at(i);
        // 		}
        // 	}

        // 	if( sGsmBandNormInfo.m_bTestPhaseError)
        // 	{
        // 		vUpRange.clear();
        // 		vLowRange.clear();
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"PhaseErrorRMSUpLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vUpRange);
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"PhaseErrorRMSDownLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vLowRange);
        // 		ISNOTNULL( (vUpRange.size() == vLowRange.size())&&(vUpRange.size() == vPwrIndex.size()));
        // 		for (size_t i = 0;i < vPwrIndex.size();i ++)
        // 		{
        // 			sGsmBandPwlInfo[i].m_fPhaseErrRmsLim[MAX_VALUE] = vUpRange.at(i);
        // 			sGsmBandPwlInfo[i].m_fPhaseErrRmsLim[MIN_VALUE] = vLowRange.at(i);
        // 		}
        // 	}

        // 	if( sGsmBandNormInfo.m_bTestPhaseErrorPeak)
        // 	{
        // 		vUpRange.clear();
        // 		vLowRange.clear();
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"PhaseErrorPeakUpLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vUpRange);
        // 		strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"PhaseErrorPeakDownLim");
        // 		JGW_ParserFloatW(strTemp.c_str(),L",",vLowRange);
        // 		ISNOTNULL( (vUpRange.size() == vLowRange.size())&&(vUpRange.size() == vPwrIndex.size()));
        // 		for (size_t i = 0;i < vPwrIndex.size();i ++)
        // 		{
        // 			sGsmBandPwlInfo[i].m_fPhaseErrPeakLim[MAX_VALUE] = vUpRange.at(i);
        // 			sGsmBandPwlInfo[i].m_fPhaseErrPeakLim[MIN_VALUE] = vLowRange.at(i);
        // 		}
        // 	}

        if( sGsmBandNormInfo.m_bTestRxLevel)
        {
            vUpRange.clear();
            vLowRange.clear();
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"RxLevelCellPower");
            JGW_ParserFloatW(strTemp.c_str(),L",",vUpRange);
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"RxLevelPowerRange");
            JGW_ParserFloatW(strTemp.c_str(),L",",vLowRange);
            ISNOTNULL( (vUpRange.size() == vLowRange.size())&&(vUpRange.size() == vPwrIndex.size()));
            for (size_t i = 0;i < vPwrIndex.size();i ++)
            {
                sGsmBandPwlInfo[i].m_fRxLevelPwr = vUpRange.at(i);
                sGsmBandPwlInfo[i].m_fRxLevelLim[MIN_VALUE] = vUpRange.at(i) - vLowRange.at(i);
                sGsmBandPwlInfo[i].m_fRxLevelLim[MAX_VALUE] = vUpRange.at(i) + vLowRange.at(i);
            }
        }

        if( sGsmBandNormInfo.m_bTestBler)
        {
            vUpRange.clear();
            vLowRange.clear();
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"BER_POWER");
            JGW_ParserFloatW(strTemp.c_str(),L",",vUpRange);
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"BER_ERR");
            JGW_ParserFloatW(strTemp.c_str(),L",",vLowRange);
            ISNOTNULL( (vUpRange.size() == vLowRange.size())&&(vUpRange.size() == vPwrIndex.size()));
            for (size_t i = 0;i < vPwrIndex.size();i ++)
            {
                sGsmBandPwlInfo[i].m_fBlerCellPwr = vUpRange.at(i);
                sGsmBandPwlInfo[i].m_fBerLim[MIN_VALUE] = 0;
                sGsmBandPwlInfo[i].m_fBerLim[MAX_VALUE] = vLowRange.at(i);
            }
        }

        for (size_t i = 0;i < vPwrIndex.size();i ++)
        {
            sGsmBandNormInfo.m_vGsmBandPwl.push_back(sGsmBandPwlInfo[i]);
        }

        // 	for (size_t i = 0;i < vPwrIndex.size();i ++)
        // 	{
        delete sGsmBandPwlInfo;
        // 	}

        return true;
    }

    bool CConfigFile::ChangesModeSelecter(S_CHANGE_BAND_INFO& sChangeBandInfo)
    {
        m_pCacheIniFile.SetIniKeyValue(L"Setting",L"ErrorReTestCount",m_sRfConfigInfo.m_nErrorReTestCount);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"RX_NS",m_sRfConfigInfo.m_bIsTestRx?1:0);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"LTE_NS",m_sRfConfigInfo.m_sModeSelecter.lte?1:0);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"TDSCDMA_NS",0/*m_sRfConfigInfo.m_sModeSelecter.tds?1:0*/);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"WCDMA_NS",m_sRfConfigInfo.m_sModeSelecter.wcdma?1:0);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"CDMA2000_NS",0/*m_sRfConfigInfo.m_sModeSelecter.cdma?1:0*/);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"GSM_NS",m_sRfConfigInfo.m_sModeSelecter.gsm?1:0);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"GPS_NS",m_sRfConfigInfo.m_sModeSelecter.gps?1:0);
        m_pCacheIniFile.SetIniKeyValue(L"SELECTER",L"WIFI_NS",m_sRfConfigInfo.m_sModeSelecter.wlan?1:0);
        ChangeMonitorConfig();
        m_pConfigIniFile.SetIniKeyValue(L"Setting",L"IsExportTestInfo",m_sRfConfigInfo.m_bIsExportTestInfo?1:0);

        if ( m_sRfConfigInfo.m_sModeSelecter.lte )
        {
            m_pCacheIniFile.SetIniKeyValue(L"LTE_RF_CONFIG",L"BAND_NUM",sChangeBandInfo.m_strLteBand.c_str());
        }
        if ( m_sRfConfigInfo.m_sModeSelecter.wcdma )
        {
            m_pCacheIniFile.SetIniKeyValue(L"WCDMA_RF_CONFIG",L"BAND_NUM",sChangeBandInfo.m_strWcdmaBand.c_str());
        }
        if ( m_sRfConfigInfo.m_sModeSelecter.tds )
        {
            //::MessageBox(GetFocus(),L"耦合测试暂不支持TDS,将忽略当前模式配置",L"配置错误",MB_ICONWARNING);
            m_pCacheIniFile.SetIniKeyValue(L"TDSCDMA_RF_CONFIG",L"BAND_NUM",sChangeBandInfo.m_strTdscdmaBand.c_str());
        }
        if ( m_sRfConfigInfo.m_sModeSelecter.cdma )
        {
            ::MessageBox(GetFocus(),L"耦合测试暂不支持CDMA,将忽略当前模式配置",L"配置错误",MB_ICONWARNING);
            //m_pCacheIniFile.SetIniKeyValue(L"CDMA_RF_CONFIG",L"BAND_NUM",sChangeBandInfo.m_strcdmaBand.c_str());
        }
        if ( m_sRfConfigInfo.m_sModeSelecter.gsm)
        {
            m_pCacheIniFile.SetIniKeyValue(L"GSM_RF_CONFIG",L"BAND_NUM",sChangeBandInfo.m_strGsmBand.c_str());
        }

        if (m_sRfConfigInfo.m_sModeSelecter.wlan)
        {
            m_pCacheIniFile.SetIniKeyValue(L"WIFI_CONFIG",L"BAND_NUM",sChangeBandInfo.m_strWlanBand.c_str());
        }

        if ( !LoadConfigFile() )
        {
            ::MessageBox(GetFocus(),L"配置文件加载失败",L"配置错误",MB_ICONERROR);
            return false;
        }

        return true;
    }

    bool CConfigFile::ChangeChannelInfo(std::vector<S_CHANGE_Chanel_INFO>& vChannelInfo)
    {
        std::wstring strTemp;
        std::vector<std::wstring> vstrLoss;
        for ( std::vector<S_CHANGE_Chanel_INFO>::iterator it = vChannelInfo.begin();
            it != vChannelInfo.end();
            it ++)
        {
            JGW_FormatWString(strTemp,L"BAND%d_PRX_PORT",it->m_nBand);
            m_pCacheIniFile.SetIniKeyValue( g_szRfConfigSecW[it->m_esys],strTemp.c_str(),it->m_strPrx.c_str() );

            JGW_FormatWString(strTemp,L"BAND%d_DRX_PORT",it->m_nBand);
            m_pCacheIniFile.SetIniKeyValue( g_szRfConfigSecW[it->m_esys],strTemp.c_str(),it->m_strDrx.c_str() );

            JGW_FormatWString(strTemp,L"BAND%d_CHAN",it->m_nBand);
            m_pCacheIniFile.SetIniKeyValue( g_szRfConfigSecW[it->m_esys],strTemp.c_str(),it->m_strChannel.c_str() );

            vstrLoss.clear();
            JGW_ParserStrW(it->m_strLoss.c_str(),L";",vstrLoss);
            if( vstrLoss.size() == 0 ) vstrLoss.push_back(L"1.00");
            while( vstrLoss.size() < 3 )
                vstrLoss.push_back(vstrLoss.at(0));
            JGW_FormatWString(strTemp,L"BAND%d_PTx_LOSS",it->m_nBand);
            m_pConfigIniFile.SetIniKeyValue(g_szRfConfigSecW[it->m_esys],strTemp.c_str(),vstrLoss.at(0).c_str());
            JGW_FormatWString(strTemp,L"BAND%d_PRx_LOSS",it->m_nBand);
            m_pConfigIniFile.SetIniKeyValue(g_szRfConfigSecW[it->m_esys],strTemp.c_str(),vstrLoss.at(1).c_str());
            JGW_FormatWString(strTemp,L"BAND%d_DRx_LOSS",it->m_nBand);
            m_pConfigIniFile.SetIniKeyValue(g_szRfConfigSecW[it->m_esys],strTemp.c_str(),vstrLoss.at(2).c_str());
        }

        if ( !LoadConfigFile() )
        {
            ::MessageBox(GetFocus(),L"配置文件加载失败",L"配置错误",MB_ICONERROR);
            return false;
        }

        return true;
    }

    bool CConfigFile::LoadWlanConfig()
    {
        COMMUNICATIONSSYS esys = TECHNOLOGY_WLAN;
        //! 获取频段信息
        ISNOTNULL(LoadSysRfConfig(esys,m_sRfConfigInfo.m_sWlanInfo.m_sTestBandInfo));
        //! 加载Board DataFile
        {
            std::wstring strTemp,strBoardDataFilePath;
            strTemp = m_pCacheIniFile.GetIniKeyValue(L"WIFI_CONFIG",L"BoardDataFilePath",L"wlanConfig.bin");
            if(PathIsRelative(strTemp.c_str()))
            {
                JGW_GetModuleWorkPath(strBoardDataFilePath);
                strBoardDataFilePath += strTemp;
                strTemp = strBoardDataFilePath;
            }
            m_sRfConfigInfo.m_sWlanInfo.mstrBoardDataFilePath = w2a(strTemp);

            strTemp = m_pCacheIniFile.GetIniKeyValue(L"WIFI_CONFIG",L"WlanBatFilePath",L"wlan.bat");
            if(PathIsRelative(strTemp.c_str()))
            {
                JGW_GetModuleWorkPath(m_sRfConfigInfo.m_sWlanInfo.mstrWlanBatFilePath);
                m_sRfConfigInfo.m_sWlanInfo.mstrWlanBatFilePath += strTemp;
                strTemp = m_sRfConfigInfo.m_sWlanInfo.mstrWlanBatFilePath;
            }
            m_sRfConfigInfo.m_sWlanInfo.mstrWlanBatDir = JGW_GetPathOfFile(m_sRfConfigInfo.m_sWlanInfo.mstrWlanBatFilePath.c_str());
        }
        //! 添加每个频段测试标准
        S_WLAN_BAND_NORM_INFO sWlanBandNormInfo;
        std::wstring strSec(L"");
        int nBand = 0;

        for (size_t i = 0;i < m_sRfConfigInfo.m_sWlanInfo.m_sTestBandInfo.m_vBandInfo.size();i ++)
        {
            sWlanBandNormInfo.Init();
            nBand = m_sRfConfigInfo.m_sWlanInfo.m_sTestBandInfo.m_vBandInfo.at(i).m_nBand;
            JGW_FormatWString(strSec,L"WIFI_%d_TESTOPTION",nBand);
            ISNOTNULL( CheckWlanBandVal(nBand) );

            sWlanBandNormInfo.m_bTestBler = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestBler");
            sWlanBandNormInfo.m_bTestPwrLevel = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestPower");
            sWlanBandNormInfo.m_bTestRxLevel = m_pCacheIniFile.GetIniKeyBoolValue(strSec.c_str(),L"IsTestRxLevel");
            ISNOTNULL(ParseWlanNormConfig(strSec.c_str(),m_sRfConfigInfo.m_sWlanInfo.m_sTestBandInfo.m_vBandInfo.at(i),sWlanBandNormInfo));
            m_sRfConfigInfo.m_sWlanInfo.m_mWlanBandNormInfo.insert(std::make_pair<int,S_WLAN_BAND_NORM_INFO>(nBand,sWlanBandNormInfo));
        }

        return true;
    }

    bool CConfigFile::ParseWlanNormConfig(const wchar_t* strSec,const S_BAND_INFO &sBandInfo,S_WLAN_BAND_NORM_INFO& sWlanBandNormInfo)
    {
        std::wstring strTemp( L"" );
        S_WLAN_CHANNEL_NORM_INFO* sWlanBandPwlInfo = new S_WLAN_CHANNEL_NORM_INFO[sBandInfo.m_vBandChannelInfo.size()];
        std::vector<float>	vValue;
        std::vector<float>  vRange;
        m_strErrorMsg = L"功率等级数量与测试标准不符";

        if(1/*sWlanBandNormInfo.m_bTestPwrLevel*/)
        {
            vValue.clear();
            vRange.clear();
            std::vector<float> vExpectedPower;//!ExpectedPower
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"Power");
            JGW_ParserFloatW(strTemp.c_str(),L",",vValue);
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"Range");
            JGW_ParserFloatW(strTemp.c_str(),L",",vRange);
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"ExpectedPower");
            JGW_ParserFloatW(strTemp.c_str(),L",",vExpectedPower);
            if (vValue.empty() || vRange.empty() || vExpectedPower.empty()) goto PARSE_ERROR;

            while (sBandInfo.m_vBandChannelInfo.size() > vValue.size())  vValue.push_back(vValue.at(0));
            while (sBandInfo.m_vBandChannelInfo.size() > vRange.size())  vRange.push_back(vRange.at(0));
            while (sBandInfo.m_vBandChannelInfo.size() > vExpectedPower.size())  vExpectedPower.push_back(vExpectedPower.at(0));
            for (size_t i = 0;i < sBandInfo.m_vBandChannelInfo.size();i ++)
            {
                sWlanBandPwlInfo[i].m_nPwrLevel = vValue.at(i);
                sWlanBandPwlInfo[i].m_fPwrLim[MAX_VALUE] = vRange.at(i) + sWlanBandPwlInfo[i].m_nPwrLevel;
                sWlanBandPwlInfo[i].m_fPwrLim[MIN_VALUE] = sWlanBandPwlInfo[i].m_nPwrLevel - vRange.at(i);
                sWlanBandPwlInfo[i].m_fExportPower = vExpectedPower.at(i);
            }
        }

        if(1/* sWlanBandNormInfo.m_bTestRxLevel*/)
        {
            vValue.clear();
            vRange.clear();
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"RxCellPower");
            JGW_ParserFloatW(strTemp.c_str(),L",",vValue);
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"RxLevelPowerRange");
            JGW_ParserFloatW(strTemp.c_str(),L",",vRange);
            if (vValue.empty() || vRange.empty()) goto PARSE_ERROR;

            while (sBandInfo.m_vBandChannelInfo.size() > vValue.size())  vValue.push_back(vValue.at(0));
            while (sBandInfo.m_vBandChannelInfo.size() > vRange.size())  vRange.push_back(vRange.at(0));
            for (size_t i = 0;i < sBandInfo.m_vBandChannelInfo.size();i ++)
            {
                sWlanBandPwlInfo[i].m_fRxCellPwr = vValue.at(i);
                sWlanBandPwlInfo[i].m_fRxLevelLim[MAX_VALUE] = vRange.at(i) + sWlanBandPwlInfo[i].m_fRxCellPwr;
                sWlanBandPwlInfo[i].m_fRxLevelLim[MIN_VALUE] = sWlanBandPwlInfo[i].m_fRxCellPwr - vRange.at(i);
            }
        }

        if(1/* sWlanBandNormInfo.m_bTestBler*/)
        {
            vRange.clear();
            strTemp = m_pCacheIniFile.GetIniKeyValue(strSec,L"BLER_ERR");
            JGW_ParserFloatW(strTemp.c_str(),L",",vRange);
            if (vRange.empty()) goto PARSE_ERROR;

            while (sBandInfo.m_vBandChannelInfo.size() > vRange.size())  vRange.push_back(vRange.at(0));
            for (size_t i = 0;i < sBandInfo.m_vBandChannelInfo.size();i ++)
            {
                sWlanBandPwlInfo[i].m_fBer = vRange.at(i);
            }
        }

        for (size_t i = 0;i < sBandInfo.m_vBandChannelInfo.size();i ++)
        {
            sWlanBandNormInfo.mvWlanChannelInfo.push_back(sWlanBandPwlInfo[i]);
        }

        delete[] sWlanBandPwlInfo;
        return true;
PARSE_ERROR:
        delete[] sWlanBandPwlInfo;
        return false;
    }
}