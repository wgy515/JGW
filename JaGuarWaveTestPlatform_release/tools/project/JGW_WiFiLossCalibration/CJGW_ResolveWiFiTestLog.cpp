#include "StdAfx.h"
#include "CJGW_ResolveWiFiTestLog.h"
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include <iostream>
namespace JGW
{
    CCJGW_ResolveWiFiTestLog::CCJGW_ResolveWiFiTestLog(void)
    {
    }


    CCJGW_ResolveWiFiTestLog::~CCJGW_ResolveWiFiTestLog(void)
    {
    }

    /*
    private string GetProtocol(string rateBw, uint uchannel)
    {
    if (rateBw.Equals("RateBW_LegacyOFDM", StringComparison.OrdinalIgnoreCase))
    {
    return uchannel >= 4900 ? "A" : "G";
    }
    else if (rateBw.Equals("RateBW_CCK", StringComparison.OrdinalIgnoreCase))
    {
    return "B";
    }
    else if (rateBw.Equals("RateBW_11N_HT20", StringComparison.OrdinalIgnoreCase))
    {
    return "N_20M";
    }
    else if (rateBw.Equals("RateBW_11N_HT40", StringComparison.OrdinalIgnoreCase))
    {
    return "N_40M";
    }
    else if (rateBw.Equals("RateBW_11AC_VHT20", StringComparison.OrdinalIgnoreCase))
    {
    return "AC_20M";
    }
    else if (rateBw.Equals("RateBW_11AC_VHT40", StringComparison.OrdinalIgnoreCase))
    {
    return "AC_40M";
    }
    else if (rateBw.Equals("RateBW_11AC_VHT80", StringComparison.OrdinalIgnoreCase))
    {
    return "AC_80M";
    }
    else if (rateBw.Equals("RateBW_11AC_VHT80P80", StringComparison.OrdinalIgnoreCase))
    {
    return "AC_80P80";
    }
    else if (rateBw.Equals("RateBW_11AX_HE20", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_20M";
    }
    else if (rateBw.Equals("RateBW_11AX_HE40", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_40M";
    }
    else if (rateBw.Equals("RateBW_11AX_HE80", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_80M";
    }
    else if (rateBw.Equals("RateBW_11AX_HE80P80", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_80P80";
    }
    else if (rateBw.Equals("RateBW_11AX_OFDMA_HE20", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_O20M";
    }
    else if (rateBw.Equals("RateBW_11AX_OFDMA_HE40", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_O40M";
    }
    else if (rateBw.Equals("RateBW_11AX_OFDMA_HE80", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_O80M";
    }
    else if (rateBw.Equals("RateBW_11AX_OFDMA_HE80P80", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_O80P80";
    }
    else if (rateBw.Equals("RateBW_11AX_HE160", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_160M";
    }
    else if (rateBw.Equals("RateBW_11AX_OFDMA_HE160", StringComparison.OrdinalIgnoreCase))
    {
    return "AX_O160M";
    }
    return "";
    }
    */
    // return string.Format("{0}_{1}_{2}_{3}", protocol, channel, rate, chain);

    WLAN_UniversalDataRate CCJGW_ResolveWiFiTestLog::GetWlanUniversalDataRate(const std::string& strDataRate)
    {
        if ('M' == strDataRate[0] && 'C' == strDataRate[1] && 'S' == strDataRate[2])
        {
            return (WLAN_UniversalDataRate)(atoi(&strDataRate[3]) + RATE_MCS_0);
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"1Mbps") || 0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"Long1Mbps"))
        {
            return WLAN_Universal_RATE_Long_1Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"Long2Mbps"))
        {
            return WLAN_Universal_RATE_Long_2Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"Long5_5Mbps"))
        {
            return WLAN_Universal_RATE_Long_5_5Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"Long11Mbps"))
        {
            return WLAN_Universal_RATE_Long_11Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"2Mbps") || 0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"Short2Mbps"))
        {
            return WLAN_Universal_RATE_Short_2Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"2Mbps") || 0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"Short55Mbps"))
        {
            return WLAN_Universal_RATE_Short_5_5Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"11Mbps") || 0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"Short11Mbps"))
        {
            return WLAN_Universal_RATE_Short_11Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"6Mbps"))
        {
            return WLAN_Universal_RATE_6Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"9Mbps"))
        {
            return WLAN_Universal_RATE_9Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"12Mbps"))
        {
            return WLAN_Universal_RATE_12Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"12Mbps"))
        {
            return WLAN_Universal_RATE_18Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"24Mbps"))
        {
            return WLAN_Universal_RATE_24Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"36Mbps"))
        {
            return WLAN_Universal_RATE_36Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"48Mbps"))
        {
            return WLAN_Universal_RATE_48Mbps;
        }
        else if (0 == JGW_StrComparenoCaseStr(strDataRate.c_str(),"54Mbps"))
        {
            return WLAN_Universal_RATE_54Mbps;
        }

        return WLAN_Universal_CW;
    }

    WLAN_CHAIN_MASK2 CCJGW_ResolveWiFiTestLog::GetWlanChainMask(const std::string& strChainMask)
    {
        if (NULL != JGW_StrCaseStr(strChainMask.c_str(),"Chain"))
        {
            if ('1' == strChainMask[5])
            {
                return WLAN_CHAIN_1;
            }
            else if ('2' == strChainMask[5])
            {
                return WLAN_CHAIN_2;
            }
            else if ('3' == strChainMask[5])
            {
                return WLAN_CHAIN_3;
            }
            else if ('4' == strChainMask[5])
            {
                return WLAN_CHAIN_4;
            }
        }
        return WLAN_CHAIN_NONE;
    }

    bool CCJGW_ResolveWiFiTestLog::ResolveWiFiTestHeader(const std::string& strWiFiTestHeader,S_WIFI_TEST_INFO& sWiFiTestInfo)
    {
        const char *str = strWiFiTestHeader.c_str();
        int pos = 2;
        if ('B' == str[0] && str[1] == '_')
        {
            sWiFiTestInfo.meUniversalRateBW = RateBW_CCK;
        }
        else if ('G' == str[0] && str[1] == '_')
        {
            // 802.11G
            sWiFiTestInfo.meUniversalRateBW = RateBW_LegacyOFDM;
        }
        else if ('A' == str[0] && str[1] == '_')
        {
            // 802.11A
            sWiFiTestInfo.meUniversalRateBW = RateBW_LegacyOFDM;
        }
        else if (0 == _strnicmp(str, "N_20M_", pos = strlen("N_20M_")))
        {
            // 802.11N - N_20M N_40M
            sWiFiTestInfo.meUniversalRateBW = RateBW_11N_HT20;
        }
        else if (0 == _strnicmp(str, "N_40M_", pos = strlen("N_40M_")))
        {
            // 802.11N - N_20M N_40M
            sWiFiTestInfo.meUniversalRateBW = RateBW_11N_HT40;
        }
        else if (0 == _strnicmp(str, "AC_20M_", pos = strlen("AC_20M_")))
        {
            // 802.11AC
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AC_VHT20;
        }
        else if (0 == _strnicmp(str, "AC_40M_", pos = strlen("AC_40M_")))
        {
            // 802.11AC
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AC_VHT40;
        }
        else if (0 == _strnicmp(str, "AC_80M_", pos = strlen("AC_80M_")))
        {
            // 802.11AC
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AC_VHT80;
        }
        else if (0 == _strnicmp(str, "AC_80P80_", pos = strlen("AC_80P80_")))
        {
            // 802.11AC
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AC_VHT80P80;
        }
        else if (0 == _strnicmp(str, "AX_20M_", pos = strlen("AX_20M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_HE20;
        }
        else if (0 == _strnicmp(str, "AX_40M_", pos = strlen("AX_40M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_HE40;
        }
        else if (0 == _strnicmp(str, "AX_80M_", pos = strlen("AX_80M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_HE80;
        }
        else if (0 == _strnicmp(str, "AX_80P80_", pos = strlen("AX_80P80_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_HE80P80;
        }

        else if (0 == _strnicmp(str, "AX_O20M_", pos = strlen("AX_O20M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_OFDMA_HE20;
        }
        else if (0 == _strnicmp(str, "AX_O40M_", pos = strlen("AX_O40M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_OFDMA_HE40;
        }
        else if (0 == _strnicmp(str, "AX_O80M_", pos = strlen("AX_O80M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_OFDMA_HE80;
        }
        else if (0 == _strnicmp(str, "AX_O80P80_", pos = strlen("AX_O80P80_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_OFDMA_HE80P80;
        }
        else if (0 == _strnicmp(str, "AX_160M_", pos = strlen("AX_160M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_HE160;
        }
        else if (0 == _strnicmp(str, "AX_O160M_", pos = strlen("AX_O160M_")))
        {
            // 802.11AX
            sWiFiTestInfo.meUniversalRateBW = RateBW_11AX_OFDMA_HE160;
        }
        else
        {
            return false;
        }
        str += pos;
        // return string.Format("{0}_{1}_{2}_{3}", protocol, channel, rate, chain);
        std::vector<std::string> vstrChanRateChain;
        JGW_ParserStrA(str,"_",vstrChanRateChain);
        if (vstrChanRateChain.size() != 3)
        {
            return false;
        }
        sWiFiTestInfo.mnChannel = atoi(vstrChanRateChain[0].c_str());
        sWiFiTestInfo.meUniversalDataRate = GetWlanUniversalDataRate(vstrChanRateChain[1]);
        sWiFiTestInfo.meWlanChain = GetWlanChainMask(vstrChanRateChain[2]);

        return sWiFiTestInfo.mnChannel >= 2412 && sWiFiTestInfo.meUniversalRateBW != WLAN_Universal_CW && sWiFiTestInfo.meWlanChain != WLAN_CHAIN_NONE;
    }


    bool CCJGW_ResolveWiFiTestLog::ResolveWiFiTestLog(std::wstring& strTestLogPath,std::vector<S_WIFI_TEST_INFO>& vsWiFiTestInfo)
    {
        std::ifstream inputFile;

        inputFile.imbue( std::locale("") );
        inputFile.open(JGW_W2A(strTestLogPath));
        // 读取内容缓冲区
        const int LINE_LENGTH = 1024 * 4; 
        char str[LINE_LENGTH] = {0}; 

        std::string strHead;
        bool isParseHeader = true;
        S_WIFI_TEST_INFO sTempWiFiTestInfo;
        char delim = '\r';
        bool bFirstLine = false;

        while (inputFile.getline(str,LINE_LENGTH,delim))
        {
            std::string strHead = str;
            if (!bFirstLine)
            {
                if (NULL != JGW_StrCaseStr(strHead.c_str(),"Is Save Test Log :") && NULL != JGW_StrCaseStr(strHead.c_str(),"LF"))
                {
                    delim = '\n';
                }
                bFirstLine = true;
            }


            if (strHead.size() < 10) continue;

            // 检查是否是WIFI测试头
            if (!ResolveWiFiTestHeader(strHead,sTempWiFiTestInfo))
            {
                continue;
            }
            // 检查是否是测试内容项 string.Format("{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t{6} 或者 {0}\t{1}_{2}\t{3}\t{4}\t{5}\t{6}\t{7}
            std::vector<std::string> vTestInfos;
            JGW_ParserStrA(strHead.c_str(),"\t",vTestInfos);
            if (vTestInfos.size() < 6)
            {
                continue;
            }

            sTempWiFiTestInfo.mstrTestProject = vTestInfos[0];
            if(0 == vTestInfos[1].compare("chAvgTxPowerDbm"))
            {
                sTempWiFiTestInfo.mfPower = atof(vTestInfos[3].c_str());
                //std::cout << sTempWiFiTestInfo.mstrTestProject << "-" << sTempWiFiTestInfo.mnChannel << "-" << sTempWiFiTestInfo.meWlanChain << ":" << sTempWiFiTestInfo.mfPower << std::endl;
                bool bFind = false;
                for (std::vector<S_WIFI_TEST_INFO>::iterator it = vsWiFiTestInfo.begin();it != vsWiFiTestInfo.end();++it)
                {
                    if (it->mnChannel == sTempWiFiTestInfo.mnChannel && it->meWlanChain == sTempWiFiTestInfo.meWlanChain && sTempWiFiTestInfo.meUniversalRateBW == it->meUniversalRateBW && sTempWiFiTestInfo.meUniversalDataRate == it->meUniversalDataRate)
                    {
                        it->mfPower = sTempWiFiTestInfo.mfPower;
                        bFind = true;
                        break;
                    }
                }
                if (!bFind) vsWiFiTestInfo.push_back(sTempWiFiTestInfo);
            }  
#if 0
            S_WIFI_TEST_INFO* psWiFiTestInfo = GetSWiFiTestInfoToTestProject(vProjectWiFiTestInfo,vTestInfos[0]);
            //std::map<std::string,S_WIFI_TEST_INFO>::iterator it = vProjectWiFiTestInfo.find(vTestInfos[0]);
            //while (it == vProjectWiFiTestInfo.end())
            //{
            //    S_WIFI_TEST_INFO sWifiTestInfo = {INVAILD_VULUE,INVAILD_VULUE,INVAILD_VULUE,INVAILD_VULUE,INVAILD_VULUE};
            //    vProjectWiFiTestInfo.insert(std::make_pair<std::string,S_WIFI_TEST_INFO>(vTestInfos[0],sWifiTestInfo));
            //    it = vProjectWiFiTestInfo.find(vTestInfos[0]);
            //}
            //! TX chEvmDb chCarrierFreqErrorPpm chAvgTxPowerDbm
            if (0 == vTestInfos[1].compare("chEvmDb"))
            {
                psWiFiTestInfo->mfEvm = atof(vTestInfos[3].c_str());
            }
            else if(0 == vTestInfos[1].compare("chCarrierFreqErrorPpm"))
            {
                psWiFiTestInfo->mfFreError = atof(vTestInfos[3].c_str());
            }
            else if(0 == vTestInfos[1].compare("chAvgTxPowerDbm"))
            {
                psWiFiTestInfo->mfPower = atof(vTestInfos[3].c_str());
            }
            //! RX PER PerPower
            else if(0 == vTestInfos[1].compare("PER"))
            {
                psWiFiTestInfo->muPer = atoi(vTestInfos[3].c_str()) < 0 ? 0 : atoi(vTestInfos[3].c_str());
            }
            else if(0 == vTestInfos[1].compare("PerPower"))
            {
                psWiFiTestInfo->mnPerPower = atoi(vTestInfos[3].c_str());
            }
            else
            {
                continue;
            }
#endif
        }
        return true;
    }

    
}
