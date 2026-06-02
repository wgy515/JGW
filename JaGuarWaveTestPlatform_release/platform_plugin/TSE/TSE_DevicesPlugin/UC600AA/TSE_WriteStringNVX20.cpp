#include "StdAfx.h"
#include "TSE_WriteStringNVX20.h"

namespace JGW
{
    CTSE_WriteStringNVX20::CTSE_WriteStringNVX20(void) : mstrSeparator(L""),mnDataLen(128)
    {
    }


    CTSE_WriteStringNVX20::~CTSE_WriteStringNVX20(void)
    {
    }

    const wchar_t* CTSE_WriteStringNVX20::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WriteStringNVX20\",\
                \"TestName\": \"Write String NV X20\",\
                \"Separator\": \"def:,\",\
                \"WriteStringNVCommand\": \"Write String NV Environment\",\
                \"DataLen\": \"nv data length\",\
                \"SaveWriteStringNVEnvrionment\": \"Save String Environmen Example:TSE_NUMBER,TSE_MAC\"\
                }";
    }

    bool CTSE_WriteStringNVX20::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"WriteStringNVCommand",strParamName))
        {
            mstrWriteStringNVCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveWriteStringNVEnvrionment",strParamName))
        {
            mstrSaveWriteStringNVEnvrionment = strParamValue;
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

    bool CTSE_WriteStringNVX20::TSE_Run()
    {
        std::wstring strWriteStringNV(mstrWriteStringNVCommand),strSaveWriteNV;
        GetEnvironmentCommandString(strWriteStringNV);
        //JGW_ReplaceStringW(strWriteStringNV,L"\\\\r",L"\r");
        //JGW_ReplaceStringW(strWriteStringNV,L"\\\\n",L"\n");
        //JGW_ReplaceStringW(strWriteStringNV,L"\\\\t",L"\t");
        //JGW_ReplaceStringW(strWriteStringNV,L"\\\\,",L"\''");
        //!\0\r\n
        size_t index = 0;
        for (index = 0;index < strWriteStringNV.length() && index < mnDataLen;index ++)
        {
            if (index == 0)
            {
                strSaveWriteNV = JGW_GetFormatWString(L"%d",strWriteStringNV[index]);
            }
            else
            {
                if (L'\\' == strWriteStringNV[index] && index + 1 < strWriteStringNV.length() && index + 1 < mnDataLen)
                {
                    if (L'0' == strWriteStringNV[index + 1])
                    {
                        strSaveWriteNV += L",";
                        strSaveWriteNV += JGW_GetFormatWString(L"%d",0);
                        index ++;
                        continue;
                    }
                    else if (L'r' == strWriteStringNV[index + 1])
                    {
                        strSaveWriteNV += L",";
                        strSaveWriteNV += JGW_GetFormatWString(L"%d",L'\r');
                        index ++;
                        continue;
                    }
                    else if (L'n' == strWriteStringNV[index + 1])
                    {
                        strSaveWriteNV += L",";
                        strSaveWriteNV += JGW_GetFormatWString(L"%d",L'\n');
                        index ++;
                        continue;
                    }
                    else if (L't' == strWriteStringNV[index + 1])
                    {
                        strSaveWriteNV += L",";
                        strSaveWriteNV += JGW_GetFormatWString(L"%d",L'\t');
                        index ++;
                        continue;
                    }
                }
                strSaveWriteNV += L",";
                strSaveWriteNV += JGW_GetFormatWString(L"%d",strWriteStringNV[index]);
            }
        }

        for (;index < mnDataLen;index++)
        {
            if (0 == index)
            {
                strSaveWriteNV = L"0";
            }
            else
            {
                strSaveWriteNV += L",0";
            }
        }
        GetGlobalEnvironment()->PutString(mstrSaveWriteStringNVEnvrionment,strSaveWriteNV);
        LogU(E_TSE_SHOW_CONTENT_INFO,strSaveWriteNV.c_str());

        return true;
    }
}

