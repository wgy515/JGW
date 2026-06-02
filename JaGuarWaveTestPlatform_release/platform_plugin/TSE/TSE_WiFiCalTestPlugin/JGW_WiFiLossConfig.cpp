#include "StdAfx.h"
#include "JGW_WiFiLossConfig.h"
#include <tinyxml/tinyxml.h>

namespace JGW
{
    CJGW_WiFiLossConfig::CJGW_WiFiLossConfig(void)
    {
        InitWlanChainEquipData();
    }


    CJGW_WiFiLossConfig::~CJGW_WiFiLossConfig(void)
    {
    }

    CJGW_WiFiLossConfig& CJGW_WiFiLossConfig::GetWiFiLossConfig()
    {
        static CJGW_WiFiLossConfig wifiLossConfig;
        return wifiLossConfig;
    }

    void CJGW_WiFiLossConfig::InitWlanChainEquipData()
    {
        for (int i = 0;i < MAX_WLAN_CHAIN;i ++)
        {
            for (int y = 0;y <= E_6G_PATHID;y ++)
            {
                mvsWlanChainEquipData[i].mDutPortVsas[y] = 1;
                mvsWlanChainEquipData[i].mDutPortVsgs[y] = 1;
            }
            mvsWlanChainEquipData[i].mvsWlanFreqConfig.clear();
        }
    }

    bool CJGW_WiFiLossConfig::LoadWiFiLossConfig(const std::string& strWiFiLossConfig)
    {
        InitWlanChainEquipData();

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(strWiFiLossConfig.c_str()))
        {
            LogE_F(L"Load WiFi Loss Config Fail(%s)",JGW_A2W(strWiFiLossConfig).c_str());
            return false;
        }

        if(!(xmlRoot = ptrXmlDoc->RootElement()))
        {
            return false;
        }

        for (int i = 1;i <= MAX_WLAN_CHAIN;i ++)
        {
            std::string strNode,strTemp;
            std::vector<int> vPort,vFreq;
            std::vector<float> vLoss;
            JGW_FormatString(strNode,"WLAN_CHAIN_%d",i);

            const TiXmlElement* pTempXmlNode = xmlRoot->FirstChildElement(strNode);
            if (pTempXmlNode)
            {
                GetElementTextValueA(pTempXmlNode,"DUT_PORT_VSA",strTemp,"1,1,1");
                vPort.clear();
                JGW_ParserIntA(strTemp.c_str(),",",vPort);
                for (size_t x = 0;x < vPort.size() && x <= E_6G_PATHID;x ++)
                {
                    mvsWlanChainEquipData[i].mDutPortVsas[i] = vPort[i];
                }

                GetElementTextValueA(pTempXmlNode,"DUT_PORT_VSG",strTemp,"1,1,1");
                vPort.clear();
                JGW_ParserIntA(strTemp.c_str(),",",vPort);
                for (size_t x = 0;x < vPort.size() && x <= E_6G_PATHID;x ++)
                {
                    mvsWlanChainEquipData[i].mDutPortVsgs[i] = vPort[x];
                }

                GetElementTextValueA(pTempXmlNode,"Frequency",strTemp,"2412,2442,2472,5180");
                vFreq.clear();
                JGW_ParserIntA(strTemp.c_str(),",",vFreq);

                GetElementTextValueA(pTempXmlNode,"Loss",strTemp,"1,1,1,1");
                vLoss.clear();
                JGW_ParserFloatA(strTemp.c_str(),",",vLoss);

                for (size_t x = 0;x < vFreq.size();x ++)
                {
                    S_WLAN_FREQCONFIG sWlanFreqConfig;
                    sWlanFreqConfig.mnFrequency = vFreq[x];
                    sWlanFreqConfig.mfLoss = vLoss.size() > x ? vLoss[x] : 1.0f;
                    mvsWlanChainEquipData[i].mvsWlanFreqConfig.push_back(sWlanFreqConfig);
                }
            }
        }
        return false;
    }


    void CJGW_WiFiLossConfig::GetLossVsaVsgPortToFreq(WLAN_CHAIN_MASK2 chainMask,int chFreqMHz,float& fLoss,int& vsaPort,int& vsgPort)
    {
        int wlanChainID = 0;
        switch (chainMask)
        {
        case WLAN_CHAIN_1:
            wlanChainID = 0;
            break;
        case WLAN_CHAIN_2:
            wlanChainID = 1;
            break;
        case WLAN_CHAIN_3:
            wlanChainID = 2;
            break;
        case WLAN_CHAIN_4:
            wlanChainID = 3;
            break;
        }

        if (chFreqMHz <= 2484.0)
        {
            vsaPort = mvsWlanChainEquipData[wlanChainID].mDutPortVsas[E_2_4G_PATHID];
            vsgPort = mvsWlanChainEquipData[wlanChainID].mDutPortVsgs[E_2_4G_PATHID];
        }
        else if (chFreqMHz > 4900.0 && chFreqMHz < 6000.0)
        {
            vsaPort = mvsWlanChainEquipData[wlanChainID].mDutPortVsas[E_5G_PATHID];
            vsgPort = mvsWlanChainEquipData[wlanChainID].mDutPortVsgs[E_5G_PATHID];
        }
        else if (chFreqMHz >= 6000.0)
        {
            vsaPort = mvsWlanChainEquipData[wlanChainID].mDutPortVsas[E_6G_PATHID];
            vsgPort = mvsWlanChainEquipData[wlanChainID].mDutPortVsgs[E_6G_PATHID];
        }
        if (0 == mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig.size())
        {
            fLoss = 1;
            return;
        }

        if (1 == mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig.size())
        {
            fLoss = mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[0].mfLoss;
            return ;
        }

        if (chFreqMHz <= mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[0].mnFrequency)
        {
            fLoss = mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[0].mfLoss;
            return ;
        }

        if (chFreqMHz >= mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig.size() - 1].mnFrequency)
        {
            fLoss = mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig.size() - 1].mfLoss;
            return ;
        }
        /// 
        for (size_t i = 1;i < mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig.size();i++)
        {
            if (chFreqMHz == mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i - 1].mnFrequency)
            {
                fLoss = mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i - 1].mfLoss;
                break;
            }

            if (chFreqMHz == mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i].mnFrequency)
            {
                fLoss = mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i].mfLoss;
                break;
            }

            if (chFreqMHz > mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i - 1].mnFrequency 
                && chFreqMHz < mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i].mnFrequency)
            {
                int delta = mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i].mnFrequency - mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i - 1].mnFrequency;
                float deltaLoss = mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i].mfLoss - mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i - 1].mfLoss;
                fLoss = (chFreqMHz - mvsWlanChainEquipData[wlanChainID].mvsWlanFreqConfig[i - 1].mnFrequency) * (deltaLoss / delta);
                break;
            }
        }
    }
}

