// JGW_ParseWiFiTestLog.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>

#define INVAILD_VULUE 99999
std::string strFilePathTemp;
bool mbMultThread = true;
std::vector<std::string> gvQSEQHtmlFile; 
using namespace JGW;



typedef struct  
{
    WLAN_UniversalPhyID mePyhID;
    //WLAN_UniversalDataRate meDataRate;
    WLAN_UniversalRateBW meRateBw;
    WLAN_CHAIN_MASK2 meChainMask;
    /// rx per
    int mnPerPower;
    uint muPer;
    
    uint muChannel;
    // tx power
    double mfPower;
    double mfEvm;
    double mfFreError;
    
    std::string mstrChainMask;
    std::string mstrChainMaskDes;
    std::string mstrDataBwText;
    std::string mstrDataRateText;
    std::string mstrPyhIDText;
}S_WIFI_TEST_INFO;

std::map<std::string,std::vector<S_WIFI_TEST_INFO*>> mmapTestResultMap;

void ScanWiFiTestResultFile(const char* filepath,const char* filename,void* lpData)
{
    //! NULL != strstr(filename,"result") &&
    if (NULL != strstr(filename,"P_"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvQSEQHtmlFile.push_back(strFilePathTemp);
    }
}

WLAN_UniversalPhyID GetPhyIDToPhyIDText(const std::string& strPhyIDText)
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

WLAN_UniversalRateBW GetRateBWToRateBWText(const std::string& strRateBWText)
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
    return RateBW_CW;
}

WLAN_CHAIN_MASK2 GetWlanChainMask2TpChainMaskText(const std::string& strChainMaskText,std::string& strChainMaskDes)
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

//! _phyId__PHY_B_ _channel__2437_ _rate__RATE_MCS_7_ _rateBw__RateBW_11AC_VHT40_ _cbState__primaryHigh_._chainMask__WLAN_CHAIN_4.
//! _phyId__PHY_A0_ _channel__5180_ _rateBw__RateBW_LegacyOFDM_ _rate__RATE_54Mbps_ _cbState__primary20_ _payLoadSize__1000_._chainMask__WLAN_CHAIN_1_ _chainMaskH__WLAN_CHAIN_NONE.
//rate__RATE_6Mbps_ _testSelect__EVM_Power_MaskMargins_ _chEvmDbUL__-8_._channel__5580_ _payLoadSize__1000_ _rateBw__RateBW_LegacyOFDM_ _cbState__primary20_ _phyId__PHY_A0_._powerLevel__20_._chainMask__WLAN_CHAIN_3_ _chainMaskH__WLAN_CHAIN_NONE_._RetryCount__1
void ParseWiFiTestHeader(const std::string& strHeader,S_WIFI_TEST_INFO* psWiFiTestInfo)
{
#if 0
    psWiFiTestInfo->mstrPyhIDText =  JGW_GetSubStrToStartEndStr(strHeader,"_phyId__","_ ","PHY_A0");
    psWiFiTestInfo->mePyhID = GetPhyIDToPhyIDText(psWiFiTestInfo->mstrPyhIDText);

    psWiFiTestInfo->muChannel =  atoi(JGW_GetSubStrToStartEndStr(strHeader,"_channel__","_ ","5180").c_str());

    psWiFiTestInfo->mstrDataBwText =  JGW_GetSubStrToStartEndStr(strHeader,"_rateBw__","_ ","RateBW_LegacyOFDM");
    psWiFiTestInfo->meRateBw = GetRateBWToRateBWText(psWiFiTestInfo->mstrDataBwText);

    psWiFiTestInfo->mstrDataRateText =  JGW_GetSubStrToStartEndStr(strHeader,"_rate__","_ ","RATE_54Mbps");

    psWiFiTestInfo->mstrChainMask =  JGW_GetSubStrToStartEndStr(strHeader,"_chainMask__","_ ","WLAN_CHAIN_NONE");
    psWiFiTestInfo->meChainMask = GetWlanChainMask2TpChainMaskText(psWiFiTestInfo->mstrChainMask,psWiFiTestInfo->mstrChainMaskDes);
    if (WLAN_CHAIN_NONE == psWiFiTestInfo->meChainMask)
    {
        psWiFiTestInfo->mstrChainMask =  JGW_GetSubStrToStartEndStr(strHeader,"_chainMask__",".","WLAN_CHAIN_1");
        psWiFiTestInfo->meChainMask = GetWlanChainMask2TpChainMaskText(psWiFiTestInfo->mstrChainMask,psWiFiTestInfo->mstrChainMaskDes);
    }
#else
    std::string strTemp(strHeader);
    std::vector<std::string> vTemp;
    //WLAN_UniversalPhyID ePhyID = PHY_A0;
    //WLAN_UniversalRateBW eRateBw = RateBW_CCK;
    //WLAN_CHAIN_MASK2 eChainMask = WLAN_CHAIN_NONE;
    //WLAN_UniversalDataRate eDataRate = WLAN_Universal_CW;
    //uint uChannel = 5180;
    //std::string strChainMaskDes;

    JGW_ReplaceStringA(strTemp,"."," ");
    JGW_ParserStrA(strTemp.c_str()," ",vTemp);
    int pos = -1;
    for (size_t index = 0;index < vTemp.size();index++)
    {
        std::string value = vTemp[index];
        JGW_EraseLastAndFristTwoChars(value,'_','_');
        if ((pos = value.find("phyId__")) != std::string::npos)
        {
            psWiFiTestInfo->mstrPyhIDText = value.substr(pos + strlen("phyId__"));
            if (std::string::npos != psWiFiTestInfo->mstrPyhIDText.find("PHY_"))
            {
                psWiFiTestInfo->mePyhID = GetPhyIDToPhyIDText(psWiFiTestInfo->mstrPyhIDText);
            }
            else
            {
                psWiFiTestInfo->mePyhID = (WLAN_UniversalPhyID)atoi(psWiFiTestInfo->mstrPyhIDText.c_str());
            }
        }
        else if ((pos = value.find("channel__")) != std::string::npos)
        {
            std::string strChannel = value.substr(pos + strlen("channel__"));
            psWiFiTestInfo->muChannel =  atoi(strChannel.c_str());
        }
        else if ((pos = value.find("rateBw__")) != std::string::npos)
        {
            std::string strRateBw = value.substr(pos + strlen("rateBw__"));
            psWiFiTestInfo->mstrDataBwText = strRateBw;
            psWiFiTestInfo->meRateBw = GetRateBWToRateBWText(strRateBw);
        }
        else if ((pos = value.find("rate__")) != std::string::npos)
        {
            psWiFiTestInfo->mstrDataRateText = value.substr(pos + strlen("rate__"));
            //eDataRate = GetDataRateToDataRateText(strDataRate);
        }
        else if ((pos = value.find("chainMask__")) != std::string::npos)
        {
            std::string strChainMask = value.substr(pos + strlen("chainMask__"));
            psWiFiTestInfo->mstrChainMask = strChainMask;
            psWiFiTestInfo->meChainMask = GetWlanChainMask2TpChainMaskText(strChainMask,psWiFiTestInfo->mstrChainMaskDes);
        }
    }
#endif
}


std::string PareseSerialNumber(const std::string& fileName)
{
    int start = 0,end = 0;
    start = fileName.find('_');
    end = fileName.find('_',start + 1);
    if (start >= 0 && end > 0 && start != end)
    {
        return fileName.substr(start + 1,end - start - 1);
    }
    return "0000000000000";
}
//! chEvmDb chCarrierFreqErrorPpm chAvgTxPowerDbm
//! PER PerPower
void ParseLog(const std::string& strLog,S_WIFI_TEST_INFO* psWiFiTestInfo)
{
    std::vector<std::string> vTemp;
    JGW_ParserStrA(strLog.c_str(),"\t",vTemp);
    if (vTemp.size() < 5)
    {
        //std::cout << strLog << std::endl;
        return ;
    }

    std::string strTestName = vTemp[0];
    JGW_ReplaceStringA(strTestName," ","");
    JGW_EraseLastAndFristTwoChars(strTestName);

    if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chEvmDb"))
    {
        psWiFiTestInfo->mfEvm = atof(vTemp[2].c_str());
    }
    else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chCarrierFreqErrorPpm"))
    {
        psWiFiTestInfo->mfFreError = atof(vTemp[2].c_str());
    }
    else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"chAvgTxPowerDbm"))
    {
        psWiFiTestInfo->mfPower = atof(vTemp[2].c_str());
    }
    else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"PER"))
    {
        psWiFiTestInfo->muPer = atoi(vTemp[2].c_str());
    }
    else if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),"PerPower"))
    {
        psWiFiTestInfo->mnPerPower = atoi(vTemp[2].c_str());
    }
}

S_WIFI_TEST_INFO* ParseLogHeader(const std::string& strHeader,std::vector<S_WIFI_TEST_INFO*>& vsWiFiTestInfo)
{
    S_WIFI_TEST_INFO sWiFiTestInfo = {PHY_A1,RateBW_CW,WLAN_CHAIN_NONE,0,0,0,0,0,0};
    sWiFiTestInfo.mfPower = INVAILD_VULUE;
    sWiFiTestInfo.mnPerPower = INVAILD_VULUE;
    ParseWiFiTestHeader(strHeader,&sWiFiTestInfo);
    for (std::vector<S_WIFI_TEST_INFO*>::iterator it = vsWiFiTestInfo.begin();
        it != vsWiFiTestInfo.end();
        ++ it)
    {
        if (it[0]->meChainMask == sWiFiTestInfo.meChainMask 
            && it[0]->mePyhID == sWiFiTestInfo.mePyhID
            && it[0]->muChannel == sWiFiTestInfo.muChannel
            && it[0]->meRateBw == sWiFiTestInfo.meRateBw
            && NULL == JGW_StrComparenoCaseStr(it[0]->mstrDataRateText.c_str(),sWiFiTestInfo.mstrDataRateText.c_str()))
        {
            return it[0];
        }
    }

    S_WIFI_TEST_INFO* psWiFiTestInfo = new S_WIFI_TEST_INFO();

    psWiFiTestInfo->mePyhID = sWiFiTestInfo.mePyhID;
    psWiFiTestInfo->meRateBw = sWiFiTestInfo.meRateBw;
    psWiFiTestInfo->meChainMask = sWiFiTestInfo.meChainMask;
    psWiFiTestInfo->mnPerPower = sWiFiTestInfo.mnPerPower;
    psWiFiTestInfo->muPer = sWiFiTestInfo.muPer;
    psWiFiTestInfo->muChannel = sWiFiTestInfo.muChannel;
    psWiFiTestInfo->mfPower = sWiFiTestInfo.mfPower;
    psWiFiTestInfo->mfFreError = sWiFiTestInfo.mfFreError;
    psWiFiTestInfo->mfEvm = sWiFiTestInfo.mfEvm;
    psWiFiTestInfo->mstrChainMask = sWiFiTestInfo.mstrChainMask;
    psWiFiTestInfo->mstrChainMaskDes = sWiFiTestInfo.mstrChainMaskDes;
    psWiFiTestInfo->mstrDataBwText = sWiFiTestInfo.mstrDataBwText;
    psWiFiTestInfo->mstrDataRateText = sWiFiTestInfo.mstrDataRateText;
    psWiFiTestInfo->mstrPyhIDText = sWiFiTestInfo.mstrPyhIDText;
    vsWiFiTestInfo.push_back(psWiFiTestInfo);

    return psWiFiTestInfo;
}


//! chEvmDb chCarrierFreqErrorPpm chAvgTxPowerDbm
//! PER PerPower
void ParseWiFiTestLogFile(const std::string strWiFiTestLogFilePath,std::vector<S_WIFI_TEST_INFO*>& vsWiFiTestInfo)
{
    std::ifstream inputFile;
    inputFile.imbue( std::locale("") );
    inputFile.open(strWiFiTestLogFilePath);

    const int LINE_LENGTH = 1024 * 4; 
    char str[LINE_LENGTH] = {0}; 
    // 0 : 默认不解析 1 : 查找到表头 2 : 查找到TX测试数据 3 : 查找到RX测试数据
    int nStartParseFlag = 0;
    std::string strHead;
    S_WIFI_TEST_INFO* psWiFiTestInfo = NULL;
    bool isParseHeader = true;

    while (inputFile.getline(str,LINE_LENGTH,'\r'))
    {
        //! 1、TX定位到chEvmDb开始位置，取消chEvmDb - chAvgTxPowerDeltaDb 中间关于文件头的解析
        //! 2、RX定位到PER开始位置，取消PER - PerPower 中间关于文件头的解析
        if (NULL != JGW_StrCaseStr(str,"_phyId__") && isParseHeader)
        {
            nStartParseFlag = 1;
            strHead = str;
        }
        //! chEvmDb
        if (NULL != JGW_StrCaseStr(str,"TxWlanMeasurementQueryChannels\t") 
            || 0 == _strnicmp(str, "PER\t", strlen("PER\t")))
        {
            isParseHeader = false;
            psWiFiTestInfo = ParseLogHeader(strHead,vsWiFiTestInfo);
        }

        if (isParseHeader && 1 == nStartParseFlag)
        {
            //psWiFiTestInfo = ParseLogHeader(strHead,vsWiFiTestInfo);
        }
        else
        {
            if (NULL == psWiFiTestInfo)
            {
                continue;
            }
            nStartParseFlag = 2;
            ParseLog(str,psWiFiTestInfo);
            if (NULL != JGW_StrCaseStr(str,"chAvgTxPowerDeltaDb") || 0 == _strnicmp(str, "PerPower", 8))
            {
                isParseHeader = true;
                psWiFiTestInfo = NULL;
            }
        }
        





        //if (1 == nStartParseFlag && (NULL != JGW_StrCaseStr(str,"chEvmDb\t") || 0 == _strnicmp(str, "PER\t", strlen("PER\t"))))
        //{
        //    psWiFiTestInfo = ParseLogHeader(strHead,vsWiFiTestInfo);
        //    nStartParseFlag = 2;
        //} 

        //if (2 == nStartParseFlag)
        //{
        //    ParseLog(str,psWiFiTestInfo);
        //    if (NULL != JGW_StrCaseStr(str,"chAvgTxPowerDeltaDb") || 0 == _strnicmp(str, "PerPower\t", strlen("PerPower\t")))
        //    {
        //        nStartParseFlag = 0;
        //    }
        //}

#if 0
        if (NULL != JGW_StrCaseStr(str,"_phyId__") && nStartParseFlag == 0)
        {
            nStartParseFlag = 1;
            strHead = str;
        }
        //if (NULL != JGW_StrCaseStr(str,"_phyId__PHY_B_ _channel__2437_ _rate__RATE_MCS_7_ _rateBw__RateBW_11AC_VHT40_ _cbState__primaryHigh_._chainMask__WLAN_CHAIN_1."))
        //{
        //    std::cout << strHead << std::endl;
        //}
#if 0
        if (1 == nStartParseFlag && (NULL != JGW_StrCaseStr(str,"chEvmDb ") || 0 == _strnicmp(str, "PER", 3)))
        {
            psWiFiTestInfo = ParseLogHeader(strHead,vsWiFiTestInfo);
            nStartParseFlag = 2;
        } 
        if (2 == nStartParseFlag)
        {
            ParseLog(str,psWiFiTestInfo);
            if (NULL != JGW_StrCaseStr(str,"chAvgTxPowerDbm") || 0 == _strnicmp(str, "PerPower", 8))
            {
                nStartParseFlag = 0;
            }
        }
#else
        
        if (1 == nStartParseFlag && (NULL != JGW_StrCaseStr(str,"chEvmDb\t") || 0 == _strnicmp(str, "PER\t", strlen("PER\t"))))
        {
            psWiFiTestInfo = ParseLogHeader(strHead,vsWiFiTestInfo);
            nStartParseFlag = 2;
        } 
        if (2 == nStartParseFlag)
        {
            int index = 0;
            if (NULL != JGW_StrCaseStr(str,"chAvgTxPowerDbm"))
            {
                index = 1;
                std::cout << str << std::endl;
            }
            ParseLog(str,psWiFiTestInfo);
            if (1 == index)
            {
                std::cout << "Power ; " << psWiFiTestInfo->mfPower << std::endl;

            }

            if (NULL != JGW_StrCaseStr(str,"chAvgTxPowerDbm") || 0 == _strnicmp(str, "PerPower", 8))
            {
                nStartParseFlag = 0;
            }
        }
#endif
#endif
    }
}

std::string GetWlan80211PortocolText(const S_WIFI_TEST_INFO* psWiFiTestInfo,const std::string& strTestProject)
{
    std::string strValue ("802.11");
    //! 802.11a_20M_5180_RATE_54Mbps_Chain1_Power
    switch (psWiFiTestInfo->meRateBw)
    {
    case RateBW_CCK:
        strValue += JGW_GetFormatString("b_20M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_LegacyOFDM:
        strValue += JGW_GetFormatString("%s_20M_%u_%s_%s%s",psWiFiTestInfo->muChannel > 4900 ? "a":"g",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11N_HT20:
        strValue += JGW_GetFormatString("n_20M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11N_HT40:
        strValue += JGW_GetFormatString("n_40M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AC_VHT20:
        strValue += JGW_GetFormatString("ac_20M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AC_VHT40:
        strValue += JGW_GetFormatString("ac_40M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AC_VHT80:
        strValue += JGW_GetFormatString("ac_80M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AC_VHT80P80:
        strValue += JGW_GetFormatString("ac_80MP80_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_HE20:
        strValue += JGW_GetFormatString("ax_20M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_HE40:
        strValue += JGW_GetFormatString("ax_40M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_HE80:
        strValue += JGW_GetFormatString("ax_80M_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_HE80P80:
        strValue += JGW_GetFormatString("ax_80P80_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_OFDMA_HE20:
        strValue += JGW_GetFormatString("ax_OFDMA_HE20_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_OFDMA_HE40:
        strValue += JGW_GetFormatString("ax_OFDMA_HE40_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_OFDMA_HE80:
        strValue += JGW_GetFormatString("ax_OFDMA_HE80_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_OFDMA_HE80P80:
        strValue += JGW_GetFormatString("ax_OFDMA_HE80P80_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_HE160:
        strValue += JGW_GetFormatString("ax_HE160_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    case RateBW_11AX_OFDMA_HE160:
        strValue += JGW_GetFormatString("ax_OFDMA_HE160_%u_%s_%s%s",psWiFiTestInfo->muChannel,psWiFiTestInfo->mstrDataRateText.c_str(),psWiFiTestInfo->mstrChainMaskDes.c_str(),strTestProject.c_str());
        break;
    }
    return strValue;
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::string strPath = "D:\\Development\\RFNET\\R100_Log\\tmp\\";
    std::string strOutPutPath = "D://output.csv";
    std::string strTemp;
    int numFiles = 0,start = 0;
    std::string strSerialNumber ;

    if (argc >= 2)
    {
        strPath = JGW_W2A(argv[1]);
    }
    if (argc >= 3)
    {
        strOutPutPath = JGW_W2A(argv[2]);
    }
    std::cout << "Start Scan Wifi Test Result File ..." << std::endl;
    JGW_TraverseFolder_C(strPath,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanWiFiTestResultFile);
    std::cout << "Scan Scan Wifi Test Result File Completed !!!" << std::endl;

    for (std::vector<std::string>::iterator it = gvQSEQHtmlFile.begin();
        it != gvQSEQHtmlFile.end();
        ++ it)
    {
        start = it->rfind('\\');
        strSerialNumber = PareseSerialNumber(it->substr(start + 1));
        std::cout << strSerialNumber << std::endl;
        std::vector<S_WIFI_TEST_INFO*> vsWiFiTestInfo;
        ParseWiFiTestLogFile(it[0],vsWiFiTestInfo);
        mmapTestResultMap[strSerialNumber] = vsWiFiTestInfo;
        /*
        S_WIFI_TEST_INFO* psWiFiTestInfo = NULL;

        std::map<std::string,S_WIFI_TEST_INFO*>::iterator itWifiTestInfo = mmapTestResultMap.find(strSerialNumber);
        if (itWifiTestInfo != mmapTestResultMap.end())
        {
            psWiFiTestInfo = itWifiTestInfo->second;
        }
        else
        {
            psWiFiTestInfo = new S_WIFI_TEST_INFO();
            psWiFiTestInfo->mePyhID = PHY_A1;
            psWiFiTestInfo->meChainMask = WLAN_CHAIN_NONE;
            mmapTestResultMap.insert(std::make_pair<std::string,S_WIFI_TEST_INFO*>(strSerialNumber,psWiFiTestInfo))
        }
        */
    }

    std::ofstream outputFile;
    std::string strSaveFilePath (strOutPutPath);
    outputFile.imbue( std::locale("") );
    outputFile.open(strSaveFilePath,std::ios_base::out | std::ios_base::trunc );
    if (!outputFile.is_open() || 0 == mmapTestResultMap.size()) return -1;

    std::string strTableHeader ("SerialNumber"),strTableValue;
    bool bWriteTableHeader = false;

    for (std::map<std::string,std::vector<S_WIFI_TEST_INFO*>>::iterator itTestResultMap = mmapTestResultMap.begin();
        itTestResultMap != mmapTestResultMap.end();
        ++ itTestResultMap)
    {
        strTableValue = itTestResultMap->first;
        for (std::vector<S_WIFI_TEST_INFO*>::iterator itVecTemp = itTestResultMap->second.begin();
            itVecTemp != itTestResultMap->second.end();
            ++ itVecTemp)
        {
            //! 无效值
            //! 802.11a_5180_RATE_54Mbps_Chain1_Power	802.11a_5180_RATE_54Mbps_Chain1_Evm	802.11a_5180_RATE_54Mbps_Chain1_FreqError

            //if (itVecTemp[0]->mfPower != INVAILD_VULUE)
            {
                strTableValue += JGW_GetFormatString(",%.2lf",itVecTemp[0]->mfPower);
                strTableValue += JGW_GetFormatString(",%.2lf",itVecTemp[0]->mfEvm);
                strTableValue += JGW_GetFormatString(",%.2lf",itVecTemp[0]->mfFreError);
                if (!bWriteTableHeader)
                {
                    strTableHeader += ",";
                    strTableHeader += GetWlan80211PortocolText(itVecTemp[0],"_Power");
                    strTableHeader += ",";
                    strTableHeader += GetWlan80211PortocolText(itVecTemp[0],"_Evm");
                    strTableHeader += ",";
                    strTableHeader += GetWlan80211PortocolText(itVecTemp[0],"_FreqError");
                }
            }
            //! 802.11a_5180_RATE_54Mbps_Chain1_PerPower_Per -78 <-10

            //if (itVecTemp[0]->mnPerPower != INVAILD_VULUE)
            {
                strTableValue += JGW_GetFormatString(",R%d <%d%%",itVecTemp[0]->mnPerPower,itVecTemp[0]->muPer);
                if (!bWriteTableHeader)
                {
                    strTableHeader += ",";
                    strTableHeader += GetWlan80211PortocolText(itVecTemp[0],"_PerPower_Per");
                }
            }
        }

        if (!bWriteTableHeader)
        {
            outputFile << strTableHeader << std::endl;
            bWriteTableHeader = true;
        }
        outputFile << strTableValue << std::endl;
    }
    std::cout << "save file path :" << strSaveFilePath << std::endl;
    //std::cout << "save file path :" << strSaveFilePath << std:endl;
#if 0

    std::map<std::string,std::vector<S_WIFI_TEST_INFO*>>::iterator itTestResultMap = mmapTestResultMap.begin();
    outputFile << "SerialNumber" << std::endl;

    for (std::vector<S_WIFI_TEST_INFO*>::iterator itVecTemp = itTestResultMap->second.begin();
        itVecTemp != itTestResultMap->second.end();
        ++ itVecTemp)
    {
        // 无效值
        if (itVecTemp[0]->mfPower != INVAILD_VULUE)
        {

        }
        if (itVecTemp[0]->mnPerPower != INVAILD_VULUE)
        {

        }
        outputFile << "," << ;
    }
#endif

    //for (std::map<std::string,S_SERIAL_NUMBER_TEST_INFO>::iterator it = gmapThroughputTestResults.begin();
    //    it != gmapThroughputTestResults.end();
    //    ++ it)
    //{
    //    outputFile << it->first << ",";
    //    JGW_FormatString(strTemp,"%.2f,%.2f",it->second.msFristTestSpeedInfo.mfTestSpeed,it->second.msSecTestSpeedInfo.mfTestSpeed);
    //    outputFile << strTemp << "," << it->second.mstrRSSI << std::endl;
    //}

    outputFile.flush();
    outputFile.close();

    return 0;
}

