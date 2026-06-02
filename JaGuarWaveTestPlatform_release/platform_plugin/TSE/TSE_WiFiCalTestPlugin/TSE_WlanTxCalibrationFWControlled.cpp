#include "StdAfx.h"
#include "TSE_WlanTxCalibrationFWControlled.h"

namespace JGW
{
    CTSE_WlanTxCalibrationFWControlled::CTSE_WlanTxCalibrationFWControlled(void) : mbAllChainOn(true),mbDebugInfo(true),mnNumAverages(3),mbVsaOnePort(true),mnVsaAverages(1)
    {
        mviMaxIteration.push_back(10);
        mviMaxIteration.push_back(10);
    }


    CTSE_WlanTxCalibrationFWControlled::~CTSE_WlanTxCalibrationFWControlled(void)
    {
    }

    const wchar_t* CTSE_WlanTxCalibrationFWControlled::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanTxCalibrationFWControlled\",\
                \"TestName\": \"Wlan Tx Calibration FWControlled\",\
                \"AllChainOn\": \"Enable all chains if AllChainOn = true, Disable all chains if AllChainOn = false.def:1\",\
                \"DebugInfo\": \"Collect debug info if DebugInfo = true, donot collect debug info if DebugInfo = false,def:1\",\
                \"MaxIteration\": \"maximum calibration iteration\",\
                \"NumAverages\": \"Number of power averages\",\
                \"VsaAverages\": \"VSA of test power count averages\",\
                \"VsaOnePort\": \"Single VSA port if true, Multiple VSA ports if false\"\
                }";
    }

    bool CTSE_WlanTxCalibrationFWControlled::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"AllChainOn",strParamName))
        {
            mbAllChainOn = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"DebugInfo",strParamName))
        {
            mbDebugInfo = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"MaxIteration",strParamName))
        {
            mviMaxIteration.clear();
            JGW_ParserIntW(strParamValue,L",",mviMaxIteration);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"NumAverages",strParamName))
        {
            mnNumAverages = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"VsaAverages",strParamName))
        {
            mnVsaAverages = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"VsaOnePort",strParamName))
        { 
            mbVsaOnePort = (1 == _ttoi(strParamValue));
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WlanTxCalibrationFWControlled::TSE_Run()
    {
        PHONE->mbVsaSinglePort = mbVsaOnePort;
        return PHONE->TxCalibrationFWControlled(mbAllChainOn,mbDebugInfo,mviMaxIteration,mnNumAverages,mnVsaAverages);
    }
}

