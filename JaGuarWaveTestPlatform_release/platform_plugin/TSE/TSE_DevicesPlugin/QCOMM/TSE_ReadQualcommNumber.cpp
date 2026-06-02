#include "StdAfx.h"
#include "TSE_ReadQualcommNumber.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_ReadQualcommNumber::CTSE_ReadQualcommNumber(void) : mNumberLen(0),meNubmerType(E_SN)
    {
    }

    CTSE_ReadQualcommNumber::~CTSE_ReadQualcommNumber(void)
    {
    }

    const wchar_t* CTSE_ReadQualcommNumber::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadQualcommNumber\",\
                \"TestName\": \"读取号段(Qualcomm)\",\
                \"NumberType\": \"号段类型E_SN:0,E_IMEI:1,E_IMEI2:2,E_MEID:3,E_WIFI_MAC:4,E_ETH0_MAC:5,E_BT_MAC:6,E_2_4_SSID:7,E_2_4_PASSWORD:8,E_5_SSID:9,E_5_PASSWORD:10,E_AD_SSID:11,E_AD_PASSWORD:12,client sn:13,barcode:14,tinnosn:15\",\
                \"NumberPrefix\": \"号段前缀校验\",\
                \"Environment\": \"读取号段保存指定环境变量中\",\
                \"EnvironmentLen\": \"长度限制 默认：0 不限制长度\"\
                }";
    }

    bool CTSE_ReadQualcommNumber::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Environment",strParamName))
        {
            mstrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"NumberType",strParamName))
        {
            meNubmerType = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"NumberPrefix",strParamName))
        {
            mstrNumberPrefix = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"EnvironmentLen",strParamName))
        {
            mNumberLen = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReadQualcommNumber::TSE_Run()
    {
        if (meNubmerType >= E_UNKNOWN_NUMBER) { LogE(L"未知号段类型读取\n");return false;}
        bool bReadOk = false;
        std::wstring strReadNumber;

        switch (meNubmerType)
        {
        case E_SN:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadSN(strReadNumber,mNumberLen > 0?mNumberLen:14);
            break;
        case E_IMEI:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadIMEI1(strReadNumber);
            break;
        case E_IMEI2:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadIMEI1(strReadNumber);
            break;
        case E_WIFI_MAC:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadWifiMac(strReadNumber);
            break;
        case E_ETH0_MAC:
            //! TDO:
            break;
        case E_BT_MAC:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadBtAddress(strReadNumber);
            break;
        case E_2_4_SSID:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Read24GWiFiSSID(strReadNumber);
            break;
        case E_2_4_PASSWORD:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Read24GWiFiPassword(strReadNumber);
            break;
        case E_5_SSID:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Read5GWiFiSSID(strReadNumber);
            break;
        case E_5_PASSWORD:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Read5GWiFiPassword(strReadNumber);
            break;
        case E_AD_SSID:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadADGWiFiSSID(strReadNumber);
            break;
        case E_AD_PASSWORD:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadADGWiFiPassword(strReadNumber);
            break;
        case E_CLIENT_SN:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadClientSN(strReadNumber); 
            break;
        case E_Barcode:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadBarcode(strReadNumber); 
            break;
        case E_TinnoSN:
            bReadOk = CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadTinnoSN(strReadNumber); 
            break;
        default:
            LogE(L"不支持号段类型\n");
            return false;
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strReadNumber.c_str());
        if (NULL != strReadNumber.compare(0,mstrNumberPrefix.length(),mstrNumberPrefix))
        {
            LogI_F(L"读取的%s:%s,前缀与配置的前缀(%s)不相符\n",gszNumberName[meNubmerType],strReadNumber.c_str(),mstrNumberPrefix.c_str());
            return false;
        }

        if (mNumberLen > 0 && strReadNumber.length() != mNumberLen)
        {
            LogE_F(L"读取的%s:%s,长度不等于%d\n",gszNumberName[meNubmerType],strReadNumber.c_str(),mNumberLen);
            return false;
        }

        if (bReadOk)
        {
            GetGlobalEnvironment()->PutString(mstrEnvironment,strReadNumber);
            LogI_F(L"读取的%s:%s\n",gszNumberName[meNubmerType],strReadNumber.c_str());
            return bReadOk;
        }

        LogI_F(L"读取%s失败\n",gszNumberName[meNubmerType]);
        return bReadOk;
    }
}