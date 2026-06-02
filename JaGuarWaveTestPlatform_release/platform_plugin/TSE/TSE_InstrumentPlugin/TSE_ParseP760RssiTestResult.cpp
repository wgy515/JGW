#include "StdAfx.h"
#include "TSE_ParseP760RssiTestResult.h"

namespace JGW
{
    CTSE_ParseP760RssiTestResult::CTSE_ParseP760RssiTestResult(void)
    {
    }


    CTSE_ParseP760RssiTestResult::~CTSE_ParseP760RssiTestResult(void)
    {
    }

    const wchar_t* CTSE_ParseP760RssiTestResult::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_ParseP760RssiTestResult::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("RssiRangeParamName"),strParamName))
        {
            mstrRssiRangeParamName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveTestResultEnvironment"),strParamName))
        {
            mstrSaveTestResultEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("RssiTestResultEnvironment"),strParamName))
        {
            mstrRssiTestResultEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("ShowTestResultForamt"),strParamName))
        {
            mstrShowTestResultForamt = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("MESLogPathEnvionment"),strParamName))
        {
            mstrMESLogPathEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("MESLogAppName"),strParamName))
        {
            mstrMESLogAppName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("MESLogKeyName"),strParamName))
        {
            mstrMESLogKeyName = strParamValue;
        }
        else
        {
            mmapParams[strParamName] = strParamValue;
        }
        return true;
    }

    std::wstring CTSE_ParseP760RssiTestResult::FormatTestResultCommand(const std::wstring& strShowTestResultCommand)
    {
        std::wstring strShowTestResult(strShowTestResultCommand);
        for (std::map<std::wstring,std::wstring>::iterator it = mmapParams.begin();
            it != mmapParams.end();++ it)
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"$$_%s_$$",it->first.c_str());
            JGW_ReplaceStringW(strShowTestResult,strTemp,it->second);
        }
        JGW_ReplaceStringW(strShowTestResult,L"\\t",L"\t");
        return strShowTestResult;
    }

    bool CTSE_ParseP760RssiTestResult::PareseMTKRxRssiResult(const std::wstring& strRxTestResult,short& rssi)
    {
        unsigned char sz16littleEndian[2] = {0};
        if (9 != strRxTestResult.size())
        {
            return false;
        }
        for (int i = 0;i < 2;i ++)
        {
            sz16littleEndian[i] = static_cast<unsigned char>(_tcstol(strRxTestResult.substr(5 + i * 2,2).c_str(),NULL,16));
        }
        memcpy(&rssi,sz16littleEndian,2);
        rssi /= 256;
        return true;
    }

    bool CTSE_ParseP760RssiTestResult::TSE_Run()
    {
        std::wstring strRawRssiTestResult = GetGlobalEnvironment()->GetString(mstrRssiTestResultEnvironment);
        std::wstring strRawTestResult =  JGW_GetSubStrToStartEndStrW(strRawRssiTestResult,L"*MCAL:");
        std::vector<std::wstring> vRssiTest;
        //! 解析*MCAL: "NRF",3,23,4,"LB1003B9D"
        JGW_ParserStrW(strRawTestResult.c_str(),L",",vRssiTest);
        if (vRssiTest.size() != 5)
        {
            LogE_F(L"Parse Rssi Test Result Error(%s)",strRawRssiTestResult.c_str());
            return false;
        }
        //! 解析"LB1003B9D"，并获取RSSI
        std::wstring strTestResult = vRssiTest[4];
        short rssi = -128;
        JGW_EraseLastAndFristTwoCharsW(strTestResult);
        JGW_EraseLastAndFristTwoCharsW(strTestResult,'"');
        if (!PareseMTKRxRssiResult(strTestResult,rssi))
        {
            LogE_F(L"Parse Rssi Test Result Error(%s)",strRawRssiTestResult.c_str());
            return false;
        }
        //! 获取RSSI RANGE配置
        std::wstring strRssiRange;
        std::map<std::wstring,std::wstring>::iterator it = mmapParams.find(mstrRssiRangeParamName);
        if (it == mmapParams.end())
        {
            LogE_F(L"请检查参数配置");
            return false;
        }
        strRssiRange = it->second;
        //! 解析RSSI RANGE
        std::vector<int> vTestRange;
        std::wstring strShowTestResult = FormatTestResultCommand(mstrShowTestResultForamt);
        JGW_ParserIntW(strRssiRange.c_str(),L"~",vTestRange);
        if (2 != vTestRange.size())
        {
            LogE_F(L"%s Test Range Config Error",strShowTestResult.c_str());
            return false;
        }

        std::wstring strMESLogAppName = FormatTestResultCommand(mstrMESLogAppName);
        mbIsRecordMesLog = false;
        if (!mstrMESLogPathEnvionment.empty())
        {
            std::wstring strMESLogPath = GetGlobalEnvironment()->GetString(mstrMESLogPathEnvionment);
            if (!strMESLogPath.empty())
            {
                mbIsRecordMesLog = mConfigIni.InitIniFilePath(strMESLogPath.c_str());
            } 
        }
        
        std::wstring strSaveTestResult = FormatTestResultCommand(mstrSaveTestResultEnvironment);
        GetGlobalEnvironment()->PutFloat(strSaveTestResult,rssi);

        std::wstring strTemp;
        bool result = false;
        JGW_FormatWString(strTemp,L"%s\t%d\t%d\t%d",strShowTestResult.c_str(),vTestRange[0],rssi,vTestRange[1]);
        if (rssi >= vTestRange[0] && rssi <= vTestRange[1])
        {
            strTemp += L"\tPASS";
            result = true;
        }
        else
        {
            strTemp += L"\tFAIL";
        }
        if (mbIsRecordMesLog)
        {
            mConfigIni.SetIniKeyValue(strMESLogAppName.c_str(),mstrMESLogKeyName.c_str(),JGW_GetFormatWString(L"%s,%d,%d,%d",result ? L"PASS":L"FAIL",rssi,vTestRange[0],vTestRange[1]).c_str());
        }
        LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%d <= %d <= %d",vTestRange[0],rssi,vTestRange[1]);
        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strTemp.c_str());
        return result;
    }
}
