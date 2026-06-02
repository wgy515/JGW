#include "StdAfx.h"
#include "TSE_FTM_ADScanAP.h"

namespace JGW
{
    CTSE_FTM_ADScanAP::CTSE_FTM_ADScanAP(void) : mminAPCounts(3),mnMinRSSI(0)
    {
    }


    CTSE_FTM_ADScanAP::~CTSE_FTM_ADScanAP(void)
    {
    }

    bool CTSE_FTM_ADScanAP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (NULL == JGW_WStrComparenoCaseWStr(strParamName,L"ADCounts"))
        {
            mminAPCounts = _ttoi(strParamValue);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strParamName,L"MinRssi"))
        {
            mnMinRSSI = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_ADScanAP::TSE_Run()
    {
        std::string strJsonCommand = CTSE_FTM_JSONCommand::GetWiFiScanAPJson();
        CCJGW_FTM_Json ftmJson;
        if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_SCAN_AP_CODE,ftmJson,mTimeOutSec))
        {
            return false;
        }

        CCJGW_FTM_WiFiAPInfoResponceVo wifiAPInfos;
        if (!wifiAPInfos.FromJosn(ftmJson)) return false;

        if (wifiAPInfos.mvsWiFiAPInfo.size() < mminADCounts)
        {
            LogE_F("扫描的WiFi热点数量(%d) 必须大于等于%d\n",wifiAPInfos.mvsWiFiAPInfo.size(),mminAPCounts);
            return false;
        }

        if (0 == mnMinRSSI) return true;

        for (size_t nIndex = 0;nIndex < wifiAPInfos.mvsWiFiAPInfo.size();nIndex ++)
        {
            if (mnMinRSSI <= wifiAPInfos.mvsWiFiAPInfo.at(nIndex).level)
                return true;
        }
        LogE_F("扫描的WiFi热点最小RSSI必须大于等于%d\n",mnMinRSSI);
        return false;
    }
}

