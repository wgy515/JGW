#include "StdAfx.h"
#include "TSE_ReadNumberToPeraso.h"
#include "..\TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_ReadNumberToPeraso::CTSE_ReadNumberToPeraso(void) : meNubmerType(E_SN_PERASO),mNumberLen(-1)
    {
    }


    CTSE_ReadNumberToPeraso::~CTSE_ReadNumberToPeraso(void)
    {
    }

    const wchar_t* CTSE_ReadNumberToPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadNumberToPeraso\",\
                \"TestName\": \"读取Peraso号段(Peraso)\",\
                \"NumberType\": \"号段类型E_SN_PERASO:0,E_RSN_MAC_PERASO:1(只读MAC),E_FW_VERSION_PERASO:2\",\
                \"NumberPrefix\": \"读取号段前缀校验\",\
                \"Environment\": \"读取号段保存到某个环境变量\",\
                \"EnvironmentLen\": \"判断读取号段的长度校验，为0不进行长度校验\"\
                }";
    }

    bool CTSE_ReadNumberToPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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

    bool CTSE_ReadNumberToPeraso::TSE_Run()
    {
        if (meNubmerType >= E_UNKNOWN_NUMBER_PERASO) { LogE(L"未知号段类型读取\n");return false;}
        bool bReadOk = false;
        std::wstring strReadNumber;

        switch (meNubmerType)
        {
        case E_SN_PERASO:
            bReadOk = CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->ReadSerialNumber(strReadNumber);
            break;
        case E_RSN_MAC_PERASO:
            bReadOk = CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->ReadMacAddress(strReadNumber);
            break;
        case E_FW_VERSION_PERASO:
            bReadOk = CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->ReadFirmwareVersion(strReadNumber);
            break;
        default:
            LogE(L"不支持读取号段类型\n");
            return false;
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strReadNumber.c_str());
        if (NULL != strReadNumber.compare(0,mstrNumberPrefix.length(),mstrNumberPrefix))
        {
            LogE_F(L"读取的%s:%s,前缀与配置的前缀(%s)不相符\n",gszPerasoNumberName[meNubmerType],strReadNumber.c_str(),mstrNumberPrefix.c_str());
            return false;
        }

        if (mNumberLen > 0 && strReadNumber.length() != mNumberLen)
        {
            LogE_F(L"读取的%s:%s,长度不等于%d\n",gszPerasoNumberName[meNubmerType],strReadNumber.c_str(),mNumberLen);
            return false;
        }

        if (bReadOk)
        {
            GetGlobalEnvironment()->PutString(mstrEnvironment,strReadNumber);
            LogI_F(L"读取的%s:%s\n",gszPerasoNumberName[meNubmerType],strReadNumber.c_str());
            return bReadOk;
        }
        LogE_F(L"读取%s失败\n",gszPerasoNumberName[meNubmerType]);
        return bReadOk;
    }
}