#include "StdAfx.h"
#include "TSE_GetIPQVersion.h"
#include <Poco/LocalDateTime.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include <poco/Exception.h>
namespace JGW
{
    CTSE_GetIPQVersion::CTSE_GetIPQVersion(void)
    {
    }


    CTSE_GetIPQVersion::~CTSE_GetIPQVersion(void)
    {
    }

    const wchar_t* CTSE_GetIPQVersion::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_GetIPQVersion::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SrcVersionEnvironment",strParamName))
        {
            mstrSrcVersionEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SrcVersionStartStr",strParamName))
        {
            mstrSrcVersionStartStr = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SrcVersionEndStr",strParamName))
        {
            mstrSrcVersionEndStr = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"VersionFormat",strParamName))
        {
            mstrVersionFormat = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"DestVersionEnvironment",strParamName))
        {
            mstrDestVersionEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetIPQVersion::TSE_Run()
    {
        std::wstring strTemp;
        std::wstring strSrcVersion = GetGlobalEnvironment()->GetString(mstrSrcVersionEnvironment);
        size_t nStartPos = 0;

        if (!mstrSrcVersionStartStr.empty())
        {
            std::wstring strSrcVersionStartStr (mstrSrcVersionStartStr);
            JGW_ReplaceStringW(strSrcVersionStartStr,L"\\\\n",L"\n");
            JGW_ReplaceStringW(strSrcVersionStartStr,L"\\\\t",L"\t");
            JGW_ReplaceStringW(strSrcVersionStartStr,L"\\\\r",L"\r");
            nStartPos = strSrcVersion.find(strSrcVersionStartStr);
            if (std::wstring::npos == nStartPos)
            {
                LogE_F(L"Prefix version information not found : %s",mstrSrcVersionStartStr.c_str());
                return false;
            }
            nStartPos += mstrSrcVersionStartStr.length();
        }

        if (mstrSrcVersionEndStr.empty())
        {
            strTemp = strSrcVersion.substr(nStartPos);
        }
        else
        {
            std::wstring strSrcVersionEndStr (mstrSrcVersionEndStr);
            JGW_ReplaceStringW(strSrcVersionEndStr,L"\\\\n",L"\n");
            JGW_ReplaceStringW(strSrcVersionEndStr,L"\\\\t",L"\t");
            JGW_ReplaceStringW(strSrcVersionEndStr,L"\\\\r",L"\r");
            size_t endIndex = strSrcVersion.find(strSrcVersionEndStr,nStartPos);
            if (std::wstring::npos == endIndex)
            {
                LogE_F(L"Postfix version information not found : %s",mstrSrcVersionEndStr.c_str());
                return false;
            }
            strTemp = strSrcVersion.substr(nStartPos,endIndex - nStartPos);
            JGW_EraseLastAndFristTwoCharsW(strTemp,L' ');
        }

        Poco::DateTime dt;
        std::string strTime = JGW_W2A(strTemp,CP_UTF8);
        try
        {
            int tzd;
            Poco::DateTimeParser::parse(Poco::DateTimeFormat::ASCTIME_FORMAT, strTime, dt, tzd);
            dt.makeUTC(tzd);
        }
        catch (Poco::Exception& e)
        {
            LogE_F(L"Parse DataTime Fail %s",JGW_A2W(e.message(),CP_UTF8).c_str());
            return false;
        }
        //! R100_1.0.20220722172119
        std::wstring strVersion;
        JGW_FormatWString(strVersion,L"%04d%02d%02d%02d%02d%02d",dt.year(),dt.month(),dt.day(),dt.hour(),dt.minute(),dt.second());
        GetGlobalEnvironment()->PutString(L"VERSION",strVersion);

        std::wstring strDestVersion(mstrVersionFormat);
        GetEnvironmentCommandString(strDestVersion);
        GetGlobalEnvironment()->PutString(mstrDestVersionEnvironment,strDestVersion);
        LogU(E_TSE_SHOW_CONTENT_INFO,strDestVersion.c_str());
        LogI_F(L"%s : %s",mstrDestVersionEnvironment.c_str(),strDestVersion.c_str());

        return true;
    }
}

