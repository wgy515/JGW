#include "StdAfx.h"
#include "TSE_ParseWiFiTestLog.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <fstream>
namespace JGW
{
    CTSE_ParseWiFiTestLog::CTSE_ParseWiFiTestLog(void) : mszNewLine('\n'),mstrWiFiTestResultRecordEnvironment(L"TSE_WIFI_TEST_RESULT_RECORD"),mpWiFiTestResultRecord(NULL)
    {
    }


    CTSE_ParseWiFiTestLog::~CTSE_ParseWiFiTestLog(void)
    {
    }

    const wchar_t* CTSE_ParseWiFiTestLog::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_ParseWiFiTestLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"RichEditTestLogFileEnvironment",strParamName))
        {
            mstrRichEditTestLogFileEnvironment =  strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"NewLineChar",strParamName))
        {
            mstrNewLineChar =  strParamValue;
            JGW_ReplaceStringW(mstrNewLineChar,L"\\r",L"\r");
            JGW_ReplaceStringW(mstrNewLineChar,L"\\n",L"\n");
            if (!mstrNewLineChar.empty()) mszNewLine = (char)mstrNewLineChar[0];
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WiFiTestResultRecordEnvironment",strParamName))
        {
            mstrWiFiTestResultRecordEnvironment =  strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ParseWiFiTestLog::TSE_Run()
    {
        std::string strWiFiTestLogFilePath = GetGlobalEnvironment()->GetAnsiString(mstrRichEditTestLogFileEnvironment);
        mpWiFiTestResultRecord = (CCJGW_WiFiTestResultRecord*)GetGlobalEnvironment()->GetInt(mstrWiFiTestResultRecordEnvironment);
        if (NULL == mpWiFiTestResultRecord)
        {
            LogE("Get WiFi Test Result Record Class Fail");
            return false;
        }

        if (!JGW_FileExistsToFilePathA(strWiFiTestLogFilePath.c_str()))
        {
            LogE_F(L"WiFi Test Log File(%s) does not exist",GetGlobalEnvironment()->GetString(mstrRichEditTestLogFileEnvironment));
            return false;
        }

        if (!ParseWiFiTestLogFile(strWiFiTestLogFilePath))
        {
            return false;
        }

        return true;
    }

    LPS_TRX_TEST_DATA CTSE_ParseWiFiTestLog::ParseLogHeader(const std::string& strHeader)
    {
#if 0
        std::string strPhyIDText = JGW_GetSubStrToStartEndStr(strHeader,"_phyId__","_ ","PHY_A0");
        WLAN_UniversalPhyID ePhyID = GetPhyIDToPhyIDText(strPhyIDText);

        uint uChannel =  atoi(JGW_GetSubStrToStartEndStr(strHeader,"_channel__","_ ","5180").c_str());

        std::string strDataBwText =  JGW_GetSubStrToStartEndStr(strHeader,"_rateBw__","_ ","RateBW_LegacyOFDM");
        WLAN_UniversalRateBW eRateBw = GetRateBWToRateBWText(strDataBwText);

        std::string strDataRate =  JGW_GetSubStrToStartEndStr(strHeader,"_rate__","_ ","RATE_54Mbps");
        WLAN_UniversalDataRate eDataRate = GetDataRateToDataRateText(strDataRate);

        std::string strChainMask =  JGW_GetSubStrToStartEndStr(strHeader,"_chainMask__","_ ","WLAN_CHAIN_NONE");
        std::string strChainMaskDes;
        WLAN_CHAIN_MASK2 eChainMask = GetWlanChainMask2TpChainMaskText(strChainMask,strChainMaskDes);
        if (WLAN_CHAIN_NONE == eChainMask)
        {
            strChainMask =  JGW_GetSubStrToStartEndStr(strHeader,"_chainMask__",".","WLAN_CHAIN_1");
            eChainMask = GetWlanChainMask2TpChainMaskText(strChainMask,strChainMaskDes);
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
        return mpWiFiTestResultRecord->GetTRXTestDataRecordToWiFiInfo(ePhyID,eChainMask,uChannel,eDataRate,eRateBw,WLAN_Gen6_ChannelBondingState_none);;
    }

    bool CTSE_ParseWiFiTestLog::ParseWiFiTestLogFile(const std::string& strWiFiTestLogFilePath)
    {
        std::ifstream inputFile;
        inputFile.imbue( std::locale("") );
        inputFile.open(strWiFiTestLogFilePath);
        if (!inputFile.is_open())
        {
            LogE_F(L"open WiFi Test Log File fail");
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
        }
        return true;
    }

    void CTSE_ParseWiFiTestLog::ParseLog(const std::string& strLog,LPS_TRX_TEST_DATA psTRxTestData)
    {
        std::vector<std::string> vTemp;
        JGW_ParserStrA(strLog.c_str(),"\t",vTemp);
        if (vTemp.size() < 5)
        {
            return ;
        }

        std::string strTestName = vTemp[0];
        JGW_ReplaceStringA(strTestName," ","");
        JGW_EraseLastAndFristTwoChars(strTestName);

        if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chEvmDb"))
        {
            psTRxTestData->mfEvm = atof(vTemp[2].c_str());
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chAmpImbDb"))
        {
            psTRxTestData->mfAmplErr = atof(vTemp[2].c_str());
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chCarrierFreqErrorPpm"))
        {
            psTRxTestData->mfFreqErr = atof(vTemp[2].c_str());
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chPhaseImbDeg"))
        {
            psTRxTestData->mfPhaseErr = atof(vTemp[2].c_str());
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chLoLeakageDbc"))
        {
            psTRxTestData->mfLoLeakager = atof(vTemp[2].c_str());
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chAvgTxPowerDbm"))
        {
            psTRxTestData->mfAvgTxPower = atof(vTemp[2].c_str());
            psTRxTestData->mbSemResult = true;
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"PER"))
        {
            psTRxTestData->mfPacketErrorRatio = (float)atoi(vTemp[2].c_str());
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"PerPower"))
        {
            psTRxTestData->mnPERPower = atoi(vTemp[2].c_str());
        }
        else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chSpectralMaskMarginsPass"))
        {
            if (JGW_StrComparenoCaseStr(vTemp[2].c_str(),"True"))
            {
                psTRxTestData->mbSemResult = true;
            }
            else
            {
                psTRxTestData->mbSemResult = false;
            }
        }
    }

    WLAN_UniversalPhyID CTSE_ParseWiFiTestLog::GetPhyIDToPhyIDText(const std::string& strPhyIDText)
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

    WLAN_UniversalRateBW CTSE_ParseWiFiTestLog::GetRateBWToRateBWText(const std::string& strRateBWText)
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

    WLAN_CHAIN_MASK2 CTSE_ParseWiFiTestLog::GetWlanChainMask2TpChainMaskText(const std::string& strChainMaskText,std::string& strChainMaskDes)
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

    WLAN_UniversalDataRate CTSE_ParseWiFiTestLog::GetDataRateToDataRateText(const std::string& strDataRate)
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
