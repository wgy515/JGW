#include "StdAfx.h"
#include "TSE_WriteQualcommNumber.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_WriteQualcommNumber::CTSE_WriteQualcommNumber(void) : mNumberLen(0)
    {
    }


    CTSE_WriteQualcommNumber::~CTSE_WriteQualcommNumber(void)
    {
    }

    const wchar_t* CTSE_WriteQualcommNumber::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WriteQualcommNumber\",\
                \"TestName\": \"写号段(Qualcomm)\",\
                \"NumberType\": \"号段类型E_SN:0,E_IMEI:1,E_IMEI2:2,E_MEID:3,E_WIFI_MAC:4,E_ETH0_MAC:5,E_BT_MAC:6,E_2_4_SSID:7,E_2_4_PASSWORD:8,E_5_SSID:9,E_5_PASSWORD:10,E_AD_SSID:11,E_AD_PASSWORD:12,client sn:13,barcode:14,tinnosn:15\",\
                \"Environment\": \"长度限制 默认：0 不限制长度\",\
                \"EnvironmentLen\": \"要写入环境变量对应的值\"\
                }";
    }

    bool CTSE_WriteQualcommNumber::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"NumberType",strParamName))
        {
            meNubmerType = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Environment",strParamName))
        {
            mstrEnvironment = strParamValue;
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

    bool CTSE_WriteQualcommNumber::TSE_Run()
    {
        if (meNubmerType >= E_UNKNOWN_NUMBER) { LogE(L"未知号段类型写入\n");return false;}

        std::wstring strEnvironmentValue = GetGlobalEnvironment()->GetString(mstrEnvironment);
        if (strEnvironmentValue.empty())
        {
            LogE_F(L"写入的%s不能为空\n",gszNumberName[meNubmerType]);
            return false;
        }
        //! 写入号段长度定义
        if (mNumberLen > 0 && mNumberLen != strEnvironmentValue.length())
        {
            LogE_F(L"写入的%s长度不等于%d位\n",gszNumberName[meNubmerType],mNumberLen);
            return false;
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strEnvironmentValue.c_str());
        LogI_F(L"写入的%s:%s\n",gszNumberName[meNubmerType],strEnvironmentValue.c_str());
        switch (meNubmerType)
        {
        case E_SN:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteSN(strEnvironmentValue.c_str(),strEnvironmentValue.length());
        case E_IMEI:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteIMEI1(strEnvironmentValue.c_str());
        case E_IMEI2:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteIMEI2(strEnvironmentValue.c_str());
        case E_WIFI_MAC:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteGNetWifiMac(strEnvironmentValue.c_str());
        case E_ETH0_MAC:
            //! TDO:
            break;
        case E_BT_MAC:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteGNetBtAddress(strEnvironmentValue.c_str());
        case E_2_4_SSID:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Write24GWiFiSSID(strEnvironmentValue.c_str());
        case E_2_4_PASSWORD:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Write24GWiFiPassword(strEnvironmentValue.c_str());
        case E_5_SSID:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Write5GWiFiSSID(strEnvironmentValue.c_str());
        case E_5_PASSWORD:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->Write5GWiFiPassword(strEnvironmentValue.c_str());
        case E_AD_SSID:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteADWiFiSSID(strEnvironmentValue.c_str());
        case E_AD_PASSWORD:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteADWiFiPassword(strEnvironmentValue.c_str());
        case E_CLIENT_SN:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteClientSN(strEnvironmentValue.c_str());
        case E_Barcode:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteBarcode(strEnvironmentValue.c_str());
        case E_TinnoSN:
            return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteTinnoSN(strEnvironmentValue.c_str());
        default:
            LogE(L"未知号段类型\n");
            break;
        }

        return false;
    }
}