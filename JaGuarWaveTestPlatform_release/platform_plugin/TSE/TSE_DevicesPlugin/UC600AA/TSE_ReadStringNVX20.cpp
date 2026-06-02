#include "StdAfx.h"
#include "TSE_ReadStringNVX20.h"

namespace JGW
{
    CTSE_ReadStringNVX20::CTSE_ReadStringNVX20(void) : mstrSeparator(L",")
    {
    }


    CTSE_ReadStringNVX20::~CTSE_ReadStringNVX20(void)
    {
    }

    const wchar_t* CTSE_ReadStringNVX20::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadStringNVX20\",\
                \"TestName\": \"Read String NV X20\",\
                \"Separator\": \"def:,\",\
                \"DataLen\": \"nv data length\",\
                \"NVRawDataEnvrionment\": \"nv raw data Environment\",\
                \"SaveDataEnvrionment\": \"Save String Environmen Example:TSE_NUMBER,TSE_MAC\"\
                }";
    }

    bool CTSE_ReadStringNVX20::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"NVRawDataEnvrionment",strParamName))
        {
            mstrNVRawDataEnvrionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveDataEnvrionment",strParamName))
        {
            mstrSaveDataEnvrionment = strParamValue;
        }
        if (TSE_PARAM_NAME_EQUAL(L"Separator",strParamName))
        {
            mstrSeparator = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"DataLen",strParamName))
        {
            mnDataLen = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReadStringNVX20::TSE_Run()
    {
        std::wstring strRawData = GetGlobalEnvironment()->GetString(mstrNVRawDataEnvrionment);
        std::vector<int> vNvData;

        JGW_ParserIntW(strRawData.c_str(),mstrSeparator.c_str(),vNvData);
        if (vNvData.size() < mnDataLen)
        {
            LogE_F(L"NV数据长度(%d-raw data len:%d)异常",mnDataLen,strRawData.size());
            return false;
        }

        std::wstring strValue;
        for (size_t i = 0;i < mnDataLen;i ++)
        {
            if (0 == vNvData[i])
            {
                if (i + 1 < mnDataLen && 0 == vNvData[i + 1])
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
            strValue += vNvData[i];
        }

        GetGlobalEnvironment()->PutString(mstrSaveDataEnvrionment,strValue);
        LogU(E_TSE_SHOW_CONTENT_INFO,strValue.c_str());

        return true;
    }
}

