#include "StdAfx.h"
#include "CJGW_ParseQsprWiFiTestResultCSVFile.h"
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>


namespace JGW
{
    CCJGW_ParseQsprWiFiTestResultCSVFile::CCJGW_ParseQsprWiFiTestResultCSVFile(void) : mszNewLine('\n')
    {
    }


    CCJGW_ParseQsprWiFiTestResultCSVFile::~CCJGW_ParseQsprWiFiTestResultCSVFile(void)
    {
    }

    std::wstring CCJGW_ParseQsprWiFiTestResultCSVFile::GetErrorMessage()
    {
        return mstrErrorMessage;
    }

    E_WIFI_TEST_METRIC CCJGW_ParseQsprWiFiTestResultCSVFile::IsParseWiFiParam(const std::string& strParameterName)
    {
        if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"chEvmDb"))
        {
           //psTRxTestData->mfEvm = atof(vTemp[2].c_str());
            return E_chEvmDb_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"chAmpImbDb"))
        {
            //psTRxTestData->mfAmplErr = atof(vTemp[2].c_str());
            return E_chAmpImbDb_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"chCarrierFreqErrorPpm"))
        {
            //psTRxTestData->mfFreqErr = atof(vTemp[2].c_str());
            return E_chCarrierFreqErrorPpm_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"chPhaseImbDeg"))
        {
            //psTRxTestData->mfPhaseErr = atof(vTemp[2].c_str());
            return E_chPhaseImbDeg_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"chLoLeakageDbc"))
        {
            //psTRxTestData->mfLoLeakager = atof(vTemp[2].c_str());
            return E_chLoLeakageDbc_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"chAvgTxPowerDbm"))
        {
            //psTRxTestData->mfAvgTxPower = atof(vTemp[2].c_str());
            //psTRxTestData->mbSemResult = true;
            return E_chAvgTxPowerDbm_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"PER"))
        {
            //psTRxTestData->mfPacketErrorRatio = (float)atoi(vTemp[2].c_str());
            return E_PER_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"PerPower"))
        {
            //psTRxTestData->mnPERPower = atoi(vTemp[2].c_str());
            return E_PerPower_WIFI_TEST_METRIC;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strParameterName.c_str(),"chSpectralMaskMarginsPass"))
        {
            /*
            if (JGW_StrComparenoCaseStr(vTemp[2].c_str(),"True"))
            {
                psTRxTestData->mbSemResult = true;
            }
            else
            {
                psTRxTestData->mbSemResult = false;
            }
            */
            return E_chSpectralMaskMarginsPass_WIFI_TEST_METRIC;
        }
        return E_UNKNOW_WIFI_TEST_METRIC;
    }



    bool CCJGW_ParseQsprWiFiTestResultCSVFile::LoadQsprWiFiTestResultCSVFile(const std::string& strWiFiCSVPath,CCJGW_WiFiTestResultRecord& wifiTestResultRecord)
    {
        std::ifstream inputFile;
        inputFile.imbue( std::locale("") );
        inputFile.open(strWiFiCSVPath);
        if (!inputFile.is_open())
        {
            mstrErrorMessage = (L"open WiFi Test Log File fail");
            return false;
        }
        const int LINE_LENGTH = 1024 * 4; 
        char str[LINE_LENGTH] = {0}; 
        // 0 : 默认不解析 1 : 查找到表头 2 : 查找到TX测试数据 3 : 查找到RX测试数据
        int nStartParseFlag = 0;
        std::string strHead;
        LPS_TRX_TEST_DATA psTRxTestData = NULL;

        while (inputFile.getline(str,LINE_LENGTH,mszNewLine))
        {
#if 1
            std::vector<std::string> vTemp;
            //JGW_ParserStrA(str,",",vTemp);
            JGW_PopulateCSVLineA(vTemp,str);
            if (vTemp.size() >= 9)
            {
                E_WIFI_TEST_METRIC eWiFiTestMetric = IsParseWiFiParam(vTemp[2]);
                if (E_UNKNOW_WIFI_TEST_METRIC != eWiFiTestMetric)
                {
                    psTRxTestData = ParseLogHeader(vTemp[3],wifiTestResultRecord);
                    switch (eWiFiTestMetric)
                    {
                    case E_chAvgTxPowerDbm_WIFI_TEST_METRIC:
                        psTRxTestData->mfAvgTxPower = atof(vTemp[5].c_str());
                        break;
                    case E_chEvmDb_WIFI_TEST_METRIC:
                        psTRxTestData->mfEvm = atof(vTemp[5].c_str());
                        break;
                    case E_chAmpImbDb_WIFI_TEST_METRIC:
                        psTRxTestData->mfAmplErr = atof(vTemp[5].c_str());
                        break;
                    case E_chPhaseImbDeg_WIFI_TEST_METRIC:
                        psTRxTestData->mfPhaseErr = atof(vTemp[5].c_str());
                        break;
                    case E_chCarrierFreqErrorPpm_WIFI_TEST_METRIC:
                        psTRxTestData->mfFreqErr = atof(vTemp[5].c_str());
                        break;
                    case E_chLoLeakageDbc_WIFI_TEST_METRIC:
                        psTRxTestData->mfLoLeakager = atof(vTemp[5].c_str());
                        break;
                    case E_PER_WIFI_TEST_METRIC:
                        psTRxTestData->mfPacketErrorRatio = (float)atoi(vTemp[5].c_str());
                        break;
                    case E_PerPower_WIFI_TEST_METRIC:
                        psTRxTestData->mnPERPower = atoi(vTemp[5].c_str());
                        break;
                    case E_chSpectralMaskMarginsPass_WIFI_TEST_METRIC:
                        if (JGW_StrComparenoCaseStr(vTemp[5].c_str(),"True"))
                        {
                            psTRxTestData->mbSemResult = true;
                        }
                        else
                        {
                            psTRxTestData->mbSemResult = false;
                        }
                        break;
                    }
                }
            }
#else
            if (NULL != JGW_StrCaseStr(str,"_phyId__"))
            {
                nStartParseFlag = 1;
                strHead = str;
            }
            if (1 == nStartParseFlag && (NULL != JGW_StrCaseStr(str,"chEvmDb") || 0 == _strnicmp(str, "PER", 3)))
            {
                //! chSpectralMaskMarginsPass
                psTRxTestData = ParseLogHeader(strHead);
                nStartParseFlag = 2;
            } 
            if (2 == nStartParseFlag)
            {
                ParseLog(str,psTRxTestData);
                if (NULL != JGW_StrCaseStr(str,"chAvgTxPowerDbm") || 0 == _strnicmp(str, "PerPower", 8))
                {
                    nStartParseFlag = 0;
                }
            }
#endif
        }
        return true;
    }

    LPS_TRX_TEST_DATA CCJGW_ParseQsprWiFiTestResultCSVFile::ParseLogHeader(const std::string& strHeader,CCJGW_WiFiTestResultRecord& wifiTestResultRecord)
    {
#if 0
        std::string strPhyIDText = JGW_GetSubStrToStartEndStr(strHeader,"phyId__","_ ","PHY_A0");
        JGW_EraseLastAndFristTwoChars(strPhyIDText,'_','_');
        if (strPhyIDText.find('.') != std::string::npos)
        {
            strPhyIDText =  JGW_GetSubStrToStartEndStr(strHeader,"phyId__",".","");
            JGW_EraseLastAndFristTwoChars(strPhyIDText,'_','_');
        }
        WLAN_UniversalPhyID ePhyID = GetPhyIDToPhyIDText(strPhyIDText);

        uint uChannel =  atoi(JGW_GetSubStrToStartEndStr(strHeader,"channel__","_ ","5180").c_str());

        std::string strDataBwText =  JGW_GetSubStrToStartEndStr(strHeader,"rateBw__","_ ","RateBW_LegacyOFDM");
        JGW_EraseLastAndFristTwoChars(strDataBwText,'_','_');
        WLAN_UniversalRateBW eRateBw = GetRateBWToRateBWText(strDataBwText);

        std::string strDataRate =  JGW_GetSubStrToStartEndStr(strHeader,"rate__","_ ","");
        JGW_EraseLastAndFristTwoChars(strDataRate,'_','_');
        if (strDataRate.find('.') != std::string::npos)
        {
            strDataRate =  JGW_GetSubStrToStartEndStr(strHeader,"rate__",".","");
            JGW_EraseLastAndFristTwoChars(strDataRate,'_','_');
        }
        WLAN_UniversalDataRate eDataRate = GetDataRateToDataRateText(strDataRate);

        std::string strChainMask =  JGW_GetSubStrToStartEndStr(strHeader,"chainMask__","_ ","WLAN_CHAIN_NONE");
        JGW_EraseLastAndFristTwoChars(strChainMask,'_','_');
        std::string strChainMaskDes;
        WLAN_CHAIN_MASK2 eChainMask = GetWlanChainMask2TpChainMaskText(strChainMask,strChainMaskDes);
        if (WLAN_CHAIN_NONE == eChainMask)
        {
            strChainMask =  JGW_GetSubStrToStartEndStr(strHeader,"chainMask__",".","WLAN_CHAIN_NONE");
            JGW_EraseLastAndFristTwoChars(strChainMask,'_','_');
            eChainMask = GetWlanChainMask2TpChainMaskText(strChainMask,strChainMaskDes);

            if (WLAN_CHAIN_NONE == eChainMask)
            {
                std::string strHeaderEnd(strHeader);
                strHeaderEnd += "\n";
                strChainMask =  JGW_GetSubStrToStartEndStr(strHeaderEnd,"chainMask__","\n","WLAN_CHAIN_NONE");
                JGW_EraseLastAndFristTwoChars(strChainMask,'_','_');
                eChainMask = GetWlanChainMask2TpChainMaskText(strChainMask,strChainMaskDes);
            }
        }
#else
        std::string strTemp(strHeader);
        std::vector<std::string> vTemp;
        WLAN_UniversalPhyID ePhyID = PHY_A0;
        WLAN_UniversalRateBW eRateBw = RateBW_CCK;
        WLAN_CHAIN_MASK2 eChainMask = WLAN_CHAIN_NONE;
        WLAN_UniversalDataRate eDataRate = WLAN_Universal_CW;
        uint uChannel = 5180;
        std::string strChainMaskDes;

        JGW_ReplaceStringA(strTemp,"."," ");
        JGW_ParserStrA(strTemp.c_str()," ",vTemp);
        int pos = -1;
        for (size_t index = 0;index < vTemp.size();index++)
        {
            std::string value = vTemp[index];
            JGW_EraseLastAndFristTwoChars(value,'_','_');
            if ((pos = value.find("phyId__")) != std::string::npos)
            {
                std::string strPhyIDText = value.substr(pos + strlen("phyId__"));
                if (std::string::npos != strPhyIDText.find("PHY_"))
                {
                    ePhyID = GetPhyIDToPhyIDText(strPhyIDText);
                }
                else
                {
                    ePhyID = (WLAN_UniversalPhyID)atoi(strPhyIDText.c_str());
                }
            }
            else if ((pos = value.find("channel__")) != std::string::npos)
            {
                std::string strChannel = value.substr(pos + strlen("channel__"));
                uChannel =  atoi(strChannel.c_str());
            }
            else if ((pos = value.find("rateBw__")) != std::string::npos)
            {
                std::string strRateBw = value.substr(pos + strlen("rateBw__"));
                eRateBw = GetRateBWToRateBWText(strRateBw);
            }
            else if ((pos = value.find("rate__")) != std::string::npos)
            {
                std::string strDataRate = value.substr(pos + strlen("rate__"));
                eDataRate = GetDataRateToDataRateText(strDataRate);
            }
            else if ((pos = value.find("chainMask__")) != std::string::npos)
            {
                std::string strChainMask = value.substr(pos + strlen("chainMask__"));
                eChainMask = GetWlanChainMask2TpChainMaskText(strChainMask,strChainMaskDes);
            }
        }
#endif

        return wifiTestResultRecord.GetTRXTestDataRecordToWiFiInfo(ePhyID,eChainMask,uChannel,eDataRate,eRateBw,WLAN_Gen6_ChannelBondingState_none);;
    }

    WLAN_UniversalPhyID CCJGW_ParseQsprWiFiTestResultCSVFile::GetPhyIDToPhyIDText(const std::string& strPhyIDText)
    {
        if (0 == JGW_StrComparenoCaseStr(strPhyIDText.c_str(),"PHY_A0"))
        {
            return PHY_A0; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strPhyIDText.c_str(),"PHY_B"))
        {
            return PHY_B; 
        }
        return PHY_A1;
    }

    WLAN_UniversalRateBW CCJGW_ParseQsprWiFiTestResultCSVFile::GetRateBWToRateBWText(const std::string& strRateBWText)
    {
        if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_LegacyOFDM"))
        {
            return RateBW_LegacyOFDM; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_CCK"))
        {
            return RateBW_CCK; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11N_HT20"))
        {
            return RateBW_11N_HT20; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11N_HT40"))
        {
            return RateBW_11N_HT40; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AC_VHT20"))
        {
            return RateBW_11AC_VHT20; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AC_VHT40"))
        {
            return RateBW_11AC_VHT40; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AC_VHT80"))
        {
            return RateBW_11AC_VHT80; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AC_VHT80P80"))
        {
            return RateBW_11AC_VHT80P80; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_HE20"))
        {
            return RateBW_11AX_HE20; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_HE40"))
        {
            return RateBW_11AX_HE40; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_HE80"))
        {
            return RateBW_11AX_HE80; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_HE80P80"))
        {
            return RateBW_11AX_HE80P80; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_OFDMA_HE20"))
        {
            return RateBW_11AX_OFDMA_HE20; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_OFDMA_HE40"))
        {
            return RateBW_11AX_OFDMA_HE40; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_OFDMA_HE80"))
        {
            return RateBW_11AX_OFDMA_HE80; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_OFDMA_HE80P80"))
        {
            return RateBW_11AX_OFDMA_HE80P80; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strRateBWText.c_str(),"RateBW_11AX_OFDMA_HE160"))
        {
            return RateBW_11AX_OFDMA_HE160; 
        }
        return (WLAN_UniversalRateBW)atoi(strRateBWText.c_str());
    }

    WLAN_CHAIN_MASK2 CCJGW_ParseQsprWiFiTestResultCSVFile::GetWlanChainMask2TpChainMaskText(const std::string& strChainMaskText,std::string& strChainMaskDes)
    {
        if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_NONE"))
        {
            strChainMaskDes = "ChainNone";
            return WLAN_CHAIN_NONE; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_1"))
        {
            strChainMaskDes = "Chain1";
            return WLAN_CHAIN_1; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_12"))
        {
            strChainMaskDes = "Chain12";
            return WLAN_CHAIN_12; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_2"))
        {
            strChainMaskDes = "Chain2";
            return WLAN_CHAIN_2; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_3"))
        {
            strChainMaskDes = "Chain3";
            return WLAN_CHAIN_3; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_13"))
        {
            strChainMaskDes = "Chain13";
            return WLAN_CHAIN_13; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_23"))
        {
            strChainMaskDes = "Chain23";
            return WLAN_CHAIN_23; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_123"))
        {
            strChainMaskDes = "Chain123";
            return WLAN_CHAIN_123; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_4"))
        {
            strChainMaskDes = "Chain4";
            return WLAN_CHAIN_4; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_14"))
        {
            strChainMaskDes = "Chain14";
            return WLAN_CHAIN_14; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_24"))
        {
            strChainMaskDes = "Chain24";
            return WLAN_CHAIN_24; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_124"))
        {
            strChainMaskDes = "Chain124";
            return WLAN_CHAIN_124; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_34"))
        {
            strChainMaskDes = "Chain34";
            return WLAN_CHAIN_34; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_134"))
        {
            strChainMaskDes = "Chain134";
            return WLAN_CHAIN_134; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_234"))
        {
            strChainMaskDes = "Chain234";
            return WLAN_CHAIN_234; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strChainMaskText.c_str(),"WLAN_CHAIN_1234"))
        {
            strChainMaskDes = "Chain1234";
            return WLAN_CHAIN_1234; 
        }

        return (WLAN_CHAIN_MASK2)atoi(strChainMaskText.c_str());
    }

    WLAN_UniversalDataRate CCJGW_ParseQsprWiFiTestResultCSVFile::GetDataRateToDataRateText(const std::string& strDataRate)
    {
        if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_Long_1Mbps"))
        {
            return WLAN_Universal_RATE_Long_1Mbps; 
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_Long_2Mbps"))
        {
            return WLAN_Universal_RATE_Long_2Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_Long_5_5Mbps"))
        {
            return WLAN_Universal_RATE_Long_5_5Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_Long_11Mbps"))
        {
            return WLAN_Universal_RATE_Long_11Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_Short_2Mbps"))
        {
            return WLAN_Universal_RATE_Short_2Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_Short_5_5Mbps"))
        {
            return WLAN_Universal_RATE_Short_5_5Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_Short_11Mbps"))
        {
            return WLAN_Universal_RATE_Short_11Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_6Mbps"))
        {
            return WLAN_Universal_RATE_6Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_9Mbps"))
        {
            return WLAN_Universal_RATE_9Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_12Mbps"))
        {
            return WLAN_Universal_RATE_12Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_18Mbps"))
        {
            return WLAN_Universal_RATE_18Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_24Mbps"))
        {
            return WLAN_Universal_RATE_24Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_36Mbps"))
        {
            return WLAN_Universal_RATE_36Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_48Mbps"))
        {
            return WLAN_Universal_RATE_48Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_54Mbps"))
        {
            return WLAN_Universal_RATE_54Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_0"))
        {
            return RATE_MCS_0;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_1"))
        {
            return RATE_MCS_1;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_2"))
        {
            return RATE_MCS_2;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_3"))
        {
            return RATE_MCS_3;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_4"))
        {
            return RATE_MCS_4;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_5"))
        {
            return RATE_MCS_5;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_6"))
        {
            return RATE_MCS_6;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_7"))
        {
            return RATE_MCS_7;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_8"))
        {
            return RATE_MCS_8;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_9"))
        {
            return RATE_MCS_9;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_10"))
        {
            return RATE_MCS_10;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_11"))
        {
            return RATE_MCS_11;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_12"))
        {
            return RATE_MCS_12;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"RATE_MCS_13"))
        {
            return RATE_MCS_13;
        }
        return (WLAN_UniversalDataRate)atoi(strDataRate.c_str());
    }
}
