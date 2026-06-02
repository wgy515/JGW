#include "StdAfx.h"
#include "TSE_FTM_WiFiScanAP.h"
#include "CJGW_FTM_WiFiAPInfoResponceVo.h"

namespace JGW
{
    CTSE_FTM_WiFiScanAP::CTSE_FTM_WiFiScanAP(void) : mminAPCounts(3),mnMinRSSI(0),mb11AD(false)
    {
    }


    CTSE_FTM_WiFiScanAP::~CTSE_FTM_WiFiScanAP(void)
    {
    }

    const wchar_t* CTSE_FTM_WiFiScanAP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_WiFiScanAP\",\
                \"TestName\": \"扫描WIFI测试(LYNX FTM)\",\
                \"APCounts\": \"扫描到的AP数量\",\
                \"MinRssi\": \"扫描到的WIFI信号中最小的RSSI必须大于等于指定的RSSI\",\
                \"IsAD\": \"0 : 11bgn 1: 11AD 默认0\"\
                }";
    }

    bool CTSE_FTM_WiFiScanAP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"APCounts"))
        {
            mminAPCounts = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"MinRssi"))
        {
            mnMinRSSI = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsAD"))
        {
            mb11AD = (0 != _ttoi(strParamValue));           
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_WiFiScanAP::TSE_Run()
    {
        std::string strJsonCommand; 
        CCJGW_FTM_Json ftmJson;

        if (!mb11AD)
        {
            strJsonCommand = CTSE_FTM_JSONCommand::GetWiFiScanAPJson();
            if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_SCAN_AP_CODE,ftmJson,mTimeOutSec))
            {
                return false;
            }
        }
        else
        {
            strJsonCommand = CTSE_FTM_JSONCommand::GetADScanAPJson();
            if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_WIGIG_SCAN_AP_CODE,ftmJson,mTimeOutSec))
            {
                return false;
            }
        }


        CCJGW_FTM_WiFiAPInfoResponceVo wifiAPInfos;
        if (!wifiAPInfos.FromJosn(ftmJson)) {Sleep(mnIntervalTimeMS); return false;}

        if (wifiAPInfos.mvsWiFiAPInfo.size() < mminAPCounts)
        {
            LogE_F("扫描的AP热点数量(%d) 必须大于等于%d\n",wifiAPInfos.mvsWiFiAPInfo.size(),mminAPCounts);
            Sleep(mnIntervalTimeMS);
            return false;
        }

        if (0 == mnMinRSSI) return true;

        for (size_t nIndex = 0;nIndex < wifiAPInfos.mvsWiFiAPInfo.size();nIndex ++)
        {
            if (mnMinRSSI <= wifiAPInfos.mvsWiFiAPInfo.at(nIndex).level)
                return true;
        }
        LogE_F("扫描的AP热点最小RSSI必须大于等于%d\n",mnMinRSSI);
        Sleep(mnIntervalTimeMS);
        return false;
    }
}
