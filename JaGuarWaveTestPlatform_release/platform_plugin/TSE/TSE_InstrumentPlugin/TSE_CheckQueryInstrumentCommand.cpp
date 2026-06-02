#include "StdAfx.h"
#include "TSE_CheckQueryInstrumentCommand.h"
#include "CJGW_TSEIntrument.h"
#include <numeric>
namespace JGW
{
	CTSE_CheckQueryInstrumentCommand::CTSE_CheckQueryInstrumentCommand(void) : mnCheckIntervalTimeMs(100)
		,mnQueryMaxCount(3),mnQueryRate(1),mnRecordMaxValue(0)
		,mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT"),mfMinRange(-999.00f),mfMaxRange(999.00f)
		,mstrPassResultFormat(L"Query %s,Min=%.2f,Curr=%.2f,Max=%.2f")
		,mstrFailResultFormat(L"Query %s,Min=%.2f,Curr=%.2f,Max=%.2f")
		,mnQueryAvgResultCount(1)
	{
	}


	CTSE_CheckQueryInstrumentCommand::~CTSE_CheckQueryInstrumentCommand(void)
	{
	}

	const wchar_t* CTSE_CheckQueryInstrumentCommand::TSE_GetParamDescription()
	{
		return L"{\
				\"RealName\": \"CTSE_CheckQueryInstrumentCommand\",\
				\"TestName\": \"Check Query Instrument Command\",\
				\"CheckIntervalTimeMs\": \"Check Interval Time Ms\",\
				\"QueryMaxCount\": \"Query max count\",\
				\"QueryAvgResultCount\": \"Query avg result count\",\
				\"QueryRate\": \"Query Rate\",\
				\"QueryCommand\": \"Query Command def:MEAS:VOLT?\",\
				\"PassResultFormat\": \"Query test Pass,Min=%.2f,Curr=%.2f,Max=%.2f\",\
				\"FailResultFormat\": \"Failed to test query,Min=%.2f,Curr=%.2f,Max=%.2f\",\
				\"RecordMaxValue\": \"record max value def : 0\",\
				\"TestRangeEnvironment\": \"Test Range 1~5\",\
				\"InstrumentEnvironment\":\"Save Instrument instance,default:TSE_INSTRUMENT_ENVIRONMENT\"\
				}";
	}

	bool CTSE_CheckQueryInstrumentCommand::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
	{
		if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
		if (TSE_PARAM_NAME_EQUAL(L"CheckIntervalTimeMs",strParamName))
		{
			mnCheckIntervalTimeMs = _ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"QueryMaxCount",strParamName))
		{
			mnQueryMaxCount = _ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"QueryAvgResultCount",strParamName))
		{
			mnQueryAvgResultCount = _ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"QueryRate",strParamName))
		{
			mnQueryRate = _ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"RecordMaxValue",strParamName))
		{
			mnRecordMaxValue = _ttoi(strParamValue);
		}	
		else if (TSE_PARAM_NAME_EQUAL(L"PassResultFormat",strParamName))
		{
			mstrPassResultFormat = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"FailResultFormat",strParamName))
		{
			mstrFailResultFormat = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"QueryCommand",strParamName))
		{
			mstrQueryCommand = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"TestRangeEnvironment",strParamName))
		{
			mstrTestRangeEnvironment = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(_T("InstrumentEnvironment"),strParamName))
		{
			mstrInstrumentEnvironment = strParamValue;
		}
		else
		{
			return false;
		}
		return true;
	}

	float CTSE_CheckQueryInstrumentCommand::GetResultToCommand(IEquipCommTL* pEquipInter,const std::string& strCommand)
	{
		std::string strRead;
		if (pEquipInter)
		{
			pEquipInter->write(strCommand);
			pEquipInter->read(strRead);
			return static_cast<float>(atof(strRead.c_str()));
		}
		return 0;
	}

	float CTSE_CheckQueryInstrumentCommand::GetResultToCount(IEquipCommTL* pEquipInter,const std::string& strCommand,int count)
	{
		std::vector<float> vfResult;
		for (int index = 0;index < count;index++)
		{
			vfResult.push_back(GetResultToCommand(pEquipInter,strCommand) * mnQueryRate);
		}
		return std::accumulate(vfResult.begin(), vfResult.end(), 0.0f) / vfResult.size();
	}
	//! std::accumulate(vfResult.begin(), vfResult.end(), 0.0f) / vfResult.size();



	//bool CTSE_CheckQueryInstrumentCommand::CheckResult(float fResult,const std::wstring& strTestRangeEnvironment)
	//{
	//	float fMin = 0, fMax = 0;
	//	std::wstring strTestRange = GetGlobalEnvironment()->GetString(strTestRangeEnvironment);
	//	if (std::wstring::npos != strTestRange.find(L"~"))
	//	{
	//		_stscanf_s(strTestRange.c_str(),L"%f~%f",&fMin,&fMax);
	//		//LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s Value:%.3f(%.3f-%.3f)",strTestContent.c_str(),fResult,fMin,fMax);
	//		if (fResult < fMin || fResult > fMax)
	//		{
	//			//LogE_F(L"%s Out Of Range:%.3f(%.3f-%.3f)",strTestContent.c_str(),fResult,fMin,fMax);
	//			return false;
	//		}
	//		return true;
	//	}
	//	//! LogE(L"Test Range Config Fail");
	//	return false;
	//}

	bool CTSE_CheckQueryInstrumentCommand::TSE_Run()
	{
		CCJGW_TSEIntrument* pTSEIntrument = (CCJGW_TSEIntrument*)(GetGlobalEnvironment()->GetInt(mstrInstrumentEnvironment));
		if (NULL == pTSEIntrument)
		{
			LogE(L"Not Find TSEIntrument instance");
			return false;
		}

		//! check equip connect
		IEquipCommTL* pEquipInter = pTSEIntrument->GetIEquipCommTL();
		if (!pEquipInter)
		{
			LogE(L"Instrument is not connected, please connect instrument first");
			return false;
		}

		std::wstring strQueryCommand(mstrQueryCommand);
		GetEnvironmentCommandString(strQueryCommand);
		if (strQueryCommand.empty())
		{
			LogE(L"Error : query command is empty");
			return false;
		}

		float fResult = 0.00f;
		float fMaxResult = 0.00f;
		bool bMaxFristResult = true;
		std::string strQueryCommandA (JGW_W2A(strQueryCommand));
		std::wstring strTestRange = GetGlobalEnvironment()->GetString(mstrTestRangeEnvironment);
		_stscanf_s(strTestRange.c_str(),L"%f~%f",&mfMinRange,&mfMaxRange);

		for (int index = 0;index < mnQueryMaxCount;index++)
		{
			fResult = GetResultToCount(pEquipInter,strQueryCommandA,mnQueryAvgResultCount);
			if (bMaxFristResult)
			{
				fMaxResult = fResult;
				bMaxFristResult = false;
			}
			if (fResult > fMaxResult) fMaxResult = fResult;
			if (fResult >= mfMinRange && fResult <= mfMaxRange)
			{
				LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%.3f",fResult);
				//! Charge test Pass,Min={}mA,Curr={}mA,Max={}mA
				LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,mstrPassResultFormat.c_str(),mfMinRange,fResult,mfMaxRange);
				return true;
			}
			Sleep(mnCheckIntervalTimeMs);
		}
		if (1 == mnRecordMaxValue) fResult = fMaxResult;
		LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%.3f",fResult);
		LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,mstrFailResultFormat.c_str(),mfMinRange,fResult,mfMaxRange);
		LogE_F(mstrFailResultFormat.c_str(),mfMinRange,fResult,mfMaxRange);
		return false;
	}
}

