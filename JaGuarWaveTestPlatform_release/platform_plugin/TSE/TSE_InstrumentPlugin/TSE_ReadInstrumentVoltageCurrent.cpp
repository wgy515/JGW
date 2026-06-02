#include "StdAfx.h"
#include "TSE_ReadInstrumentVoltageCurrent.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "CJGW_TSEIntrument.h"
#include <numeric>
namespace JGW
{
    CTSE_ReadInstrumentVoltageCurrent::CTSE_ReadInstrumentVoltageCurrent(void) : mbCheckVoltage(true),mbCheckCurrent(true),mnCheckIntervalTimeMs(500),mnCheckTimeSec(5),mstrReadVoltageCommand("MEAS:VOLT?"),mstrReadCurrentCommand("MEAS:CURR?"),mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT"),mnReadVoltageRate(1),mnReadCurrentRate(1),mnReadMaxCount(10000)
    {
    }


    CTSE_ReadInstrumentVoltageCurrent::~CTSE_ReadInstrumentVoltageCurrent(void)
    {
    }
    /// 3A Range: 0 through MAX (see table A-2) 
    /// 1A Range: 0 through 1 A
    /// 0.02A Range: 0 through 0.02 A
    /// SENS:CURR:RANG 4.0
    const wchar_t* CTSE_ReadInstrumentVoltageCurrent::TSE_GetParamDescription()
    {
		return L"{\
				\"RealName\": \"CTSE_ReadInstrumentVoltageCurrent\",\
				\"TestName\": \"Write Instrument Command\",\
				\"CheckVoltage\": \"Check Voltage\",\
				\"CheckCurrent\": \"Check Current\",\
				\"CheckIntervalTimeMs\": \"Check Interval Time Ms\",\
				\"CheckTimeSec\": \"Check Time Sec\",\
				\"ReadMaxCount\": \"Read max count\",\
				\"VoltageRate\": \"Voltage Rate\",\
				\"CurrentRate\": \"Current Rate\",\
				\"ReadVoltageCommand\": \"Read Voltage Command def:MEAS:VOLT?\",\
				\"ReadCurrentCommand\": \"Read Current Command def:MEAS:CURR?\",\
				\"CurrentTestRangeEnvironment\": \"Current Range 1~5\",\
				\"VoltageTestRangeEnvironment\": \"Voltage Range 2~8\",\
                \"InstrumentEnvironment\":\"Save Instrument instance,default:TSE_INSTRUMENT_ENVIRONMENT\"\
				}";
    }

    bool CTSE_ReadInstrumentVoltageCurrent::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"CheckVoltage",strParamName))
        {
            mbCheckVoltage = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CheckCurrent",strParamName))
        {
            mbCheckCurrent = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CheckIntervalTimeMs",strParamName))
        {
            mnCheckIntervalTimeMs = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CheckTimeSec",strParamName))
        {
            mnCheckTimeSec = _ttoi(strParamValue);
        }
		else if (TSE_PARAM_NAME_EQUAL(L"ReadMaxCount",strParamName))
		{
			mnReadMaxCount = _ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"CurrentRate",strParamName))
		{
			mnReadCurrentRate = _ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"VoltageRate",strParamName))
		{
			mnReadVoltageRate = _ttoi(strParamValue);
		}
        else if (TSE_PARAM_NAME_EQUAL(L"ReadVoltageCommand",strParamName))
        {
            mstrReadVoltageCommand = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ReadCurrentCommand",strParamName))
        {
            mstrReadCurrentCommand = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"VoltageTestRangeEnvironment",strParamName))
        {
            mstrVoltageTestRangeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CurrentTestRangeEnvironment",strParamName))
        {
            mstrCurrentTestRangeEnvironment = strParamValue;
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

    float CTSE_ReadInstrumentVoltageCurrent::GetResultToCommand(IEquipCommTL* pEquipInter,const std::string& strCommand)
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

    bool CTSE_ReadInstrumentVoltageCurrent::CheckResult(std::vector<float> vfResult,const std::wstring& strTestRangeEnvironment,const std::wstring& strTestContent)
    {
        if (vfResult.empty())
        {
            LogE_F(L"Get %s Fail",strTestContent);
            return false;
        }
        float fMin = 0, fMax = 0;
        float fAvg = std::accumulate(vfResult.begin(), vfResult.end(), 0.0f) / vfResult.size();
        std::wstring strTestRange = GetGlobalEnvironment()->GetString(strTestRangeEnvironment);
        if (std::wstring::npos != strTestRange.find(L"~"))
        {
            _stscanf_s(strTestRange.c_str(),L"%f~%f",&fMin,&fMax);
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s Value:%.3f(%.3f-%.3f)",strTestContent.c_str(),fAvg,fMin,fMax);
            if (fAvg < fMin || fAvg > fMax)
            {
                LogE_F(L"%s Out Of Range:%.3f(%.3f-%.3f)",strTestContent.c_str(),fAvg,fMin,fMax);
                return false;
            }
            return true;
        }
        LogE(L"Test Range Config Fail");
        return false;
    }

    bool CTSE_ReadInstrumentVoltageCurrent::TSE_Run()
    {
        jgw_timer jt;
        float fCurrent = 0,fVoltage = 0,fMin = 0,fMax = 0;
        std::vector<float> vfCurrent,vfVoltage;
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
		int count = 0;
        //! get current voltage
        while (mnCheckTimeSec >= jt.elapsed() && count++ < mnReadMaxCount)
        {
            if (mbCheckCurrent)
            {
                fCurrent = GetResultToCommand(pEquipInter,mstrReadCurrentCommand) * mnReadCurrentRate;
                vfCurrent.push_back(fCurrent);
            }
            if (mbCheckVoltage)
            {
                fVoltage = GetResultToCommand(pEquipInter,mstrReadVoltageCommand) * mnReadVoltageRate;
                vfVoltage.push_back(fVoltage);
            }
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Voltage:%.3f,Current:%.3f",fVoltage,fCurrent);
            Sleep(mnCheckIntervalTimeMs);
        }

        if (mbCheckCurrent && !CheckResult(vfCurrent,mstrCurrentTestRangeEnvironment,L"Current"))
        {
            return false;
        }

        if (mbCheckVoltage && !CheckResult(vfVoltage,mstrVoltageTestRangeEnvironment,L"Voltage"))
        {
            return false;
        }

        LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%.3f,%.3f",fVoltage,fCurrent);
        return true;
    }
}
