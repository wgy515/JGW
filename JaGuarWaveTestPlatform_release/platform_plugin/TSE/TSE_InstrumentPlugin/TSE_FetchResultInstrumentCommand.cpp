#include "StdAfx.h"
#include "TSE_FetchResultInstrumentCommand.h"
#include "CJGW_TSEIntrument.h"
#include <regex>
#include <vector>

namespace JGW
{
    CTSE_FetchResultInstrumentCommand::CTSE_FetchResultInstrumentCommand(void) : mstrSeparator(L","),mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT"),mbIsRecordMesLog(false)
    {
    }


    CTSE_FetchResultInstrumentCommand::~CTSE_FetchResultInstrumentCommand(void)
    {
    }

    const wchar_t* CTSE_FetchResultInstrumentCommand::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_FetchResultInstrumentCommand::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("Separator"),strParamName))
        {
            mstrSeparator = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("TestRangeParamNames"),strParamName))
        {
            mstrTestRangeParamNames = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("ShowTestResultForamts"),strParamName))
        {
            mstrShowTestResultForamts = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("TestResultIndexs"),strParamName))
        {
            mstrTestResultIndexs = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveTestResultEnvironments"),strParamName))
        {
            mstrSaveTestResultEnvironments = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("FetchResultOkRegExp"),strParamName))
        {
            mstrFetchResultOkRegExp = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("FetchInstrumentCommand"),strParamName))
        {
            mstrFetchInstrumentCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("InstrumentEnvironment"),strParamName))
        {
            mstrInstrumentEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("MESLogPathEnvionment"),strParamName))
        {
            mstrMESLogPathEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("MESLogAppName"),strParamName))
        {
            mstrMESLogAppName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("MESLogKeyNames"),strParamName))
        {
            mstrMESLogKeyNames = strParamValue;
        }
        else
        {
            mmapParams[strParamName] = strParamValue;
        }
        return true;
    }

    std::wstring CTSE_FetchResultInstrumentCommand::FormatFetchCommand(const std::wstring& strShowTestResultCommand)
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

    bool CTSE_FetchResultInstrumentCommand::CheckTestResult(float fTestValue,const std::wstring& strTestRange,const std::wstring& strShowTestResult,const std::wstring& strSaveTestResultEnvironment,const std::wstring& strMESLogAppName,const std::wstring& strMESLogKeyName)
    {
        std::vector<float> vTestRange;
        JGW_ParserFloatW(strTestRange.c_str(),L"~",vTestRange);
        if (2 != vTestRange.size())
        {
            LogE_F(L"%s Test Range Config Error",strShowTestResult.c_str());
            return false;
        }

        std::wstring strSaveTestResult = FormatFetchCommand(strSaveTestResultEnvironment);
        GetGlobalEnvironment()->PutFloat(strSaveTestResult,fTestValue);

        std::wstring strTemp;
        bool result = false;
        JGW_FormatWString(strTemp,L"%s\t%.2f\t%.2f\t%.2f",strShowTestResult.c_str(),vTestRange[0],fTestValue,vTestRange[1]);
        if (fTestValue >= vTestRange[0] && fTestValue <= vTestRange[1])
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
            mConfigIni.SetIniKeyValue(strMESLogAppName.c_str(),strMESLogKeyName.c_str(),JGW_GetFormatWString(L"%s,%.2f,%.2f,%.2f",result ? L"PASS":L"FAIL",fTestValue,vTestRange[0],vTestRange[1]).c_str());
        }
        LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%.2f <= %.2f <= %.2f",vTestRange[0],fTestValue,vTestRange[1]);
        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strTemp.c_str());
        return result;
    }

    bool CTSE_FetchResultInstrumentCommand::TSE_Run()
    {
        CCJGW_TSEIntrument* pTSEIntrument = (CCJGW_TSEIntrument*)(GetGlobalEnvironment()->GetInt(mstrInstrumentEnvironment));
        IEquipCommTL* pEquipInter = pTSEIntrument->GetIEquipCommTL();
        if (!pEquipInter)
        {
            LogE(L"仪器未连接，请先连接仪器");
            return false;
        }
        //! 查询命令返回结果
        std::string strRecv;
        std::wstring strFetchInstrumentCommand (mstrFetchInstrumentCommand),strRecvResult;
        GetEnvironmentCommandString(strFetchInstrumentCommand);
        pEquipInter->query(JGW_W2A(strFetchInstrumentCommand).c_str(),strRecv);
#ifdef _DEBUG
        strRecv = "0,2.265106E+001";
#endif
        strRecvResult = JGW_A2W(strRecv);
		JGW_EraseLastAndFristTwoCharsW(strRecvResult);
		JGW_EraseLastAndFristTwoCharsW(strRecvResult,' ');
#if 0
		std::wregex partten(mstrFetchResultOkRegExp,std::wregex::icase);
		if (!regex_match (strRecvResult,partten))
		{
			LogE_F(L"Query Instrument Command Error(%s)",strRecvResult.c_str());
			return false;
		}
#else
		if (NULL == JGW_WStrCaseWStr(strRecvResult.c_str(),mstrFetchResultOkRegExp.c_str()))
		{
			LogE_F(L"Query Instrument Command Error(%s)",strRecvResult.c_str());
			//return false;
		}
#endif
        //! 处理相关测试数据
        std::vector<float> vResult;
        JGW_ParserFloatW(strRecvResult.c_str(),mstrSeparator.c_str(),vResult);
        //! 结果索引
        std::vector<int> vdResultIndexs;
        JGW_ParserIntW(mstrTestResultIndexs.c_str(),L",",vdResultIndexs);
        std::vector<std::wstring> vstrTestRangeParamNames;
        JGW_ParserStrW(mstrTestRangeParamNames.c_str(),L",",vstrTestRangeParamNames);
        std::vector<std::wstring> vstrShowTestResultForamts;
        JGW_ParserStrW(mstrShowTestResultForamts.c_str(),L",",vstrShowTestResultForamts);
        std::vector<std::wstring> vstrSaveTestResultEnvironments;
        JGW_ParserStrW(mstrSaveTestResultEnvironments.c_str(),L",",vstrSaveTestResultEnvironments);
        std::vector<std::wstring> vstrMESLogKeyNames;
        JGW_ParserStrW(mstrMESLogKeyNames.c_str(),L",",vstrMESLogKeyNames);

        if (vdResultIndexs.size() != vstrTestRangeParamNames.size() 
            && vstrShowTestResultForamts.size() != vstrTestRangeParamNames.size() 
            && vstrShowTestResultForamts.size() != vstrMESLogKeyNames.size() 
            && vstrShowTestResultForamts.size() < 1)
        {
            LogE_F(L"请检查参数配置");
            return false;
        }

        //! vstrTestRangeParamNames
        for (size_t index = 0;index < vstrTestRangeParamNames.size();index++)
        {
            std::map<std::wstring,std::wstring>::iterator it = mmapParams.find(vstrTestRangeParamNames[index]);
            if (it != mmapParams.end())
            {
                vstrTestRangeParamNames[index] = it->second;
            }
            else
            {
                LogE_F(L"请检查参数配置");
                return false;
            }
        }
        std::wstring strMESLogAppName = FormatFetchCommand(mstrMESLogAppName);
        mbIsRecordMesLog = false;
        if (!mstrMESLogPathEnvionment.empty())
        {
            std::wstring strMESLogPath = GetGlobalEnvironment()->GetString(mstrMESLogPathEnvionment);
            if (!strMESLogPath.empty())
            {
                mbIsRecordMesLog = mConfigIni.InitIniFilePath(strMESLogPath.c_str());
            } 
        }

        //! 处理各项结果参数
        for (size_t index = 0;index < vdResultIndexs.size();index++)
        {
            std::wstring strShowTestResult = FormatFetchCommand(vstrShowTestResultForamts[index]);
            if (vResult.size() < static_cast<size_t>(vdResultIndexs[index]))
            {
                LogE_F(L"Read %s",strShowTestResult.c_str());
                return false;
            }
            if (!CheckTestResult(vResult[vdResultIndexs[index]],vstrTestRangeParamNames[index],strShowTestResult,vstrSaveTestResultEnvironments[index],strMESLogAppName,vstrMESLogKeyNames[index]))
            {
                return false;
            }
        }
        return true;
    }
}
