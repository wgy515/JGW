#include "StdAfx.h"
#include "TSE_WlanRxCalibrationFWControlled.h"

namespace JGW
{
    CTSE_WlanRxCalibrationFWControlled::CTSE_WlanRxCalibrationFWControlled(void) : mnRadioId(77)
    {
        mvChainToCalibrate.push_back(0);
        mvChainToCalibrate.push_back(1);
    }


    CTSE_WlanRxCalibrationFWControlled::~CTSE_WlanRxCalibrationFWControlled(void)
    {
    }

    const wchar_t* CTSE_WlanRxCalibrationFWControlled::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanRxCalibrationFWControlled\",\
                \"TestName\": \"Wlan Rx Calibration FW Controlled\",\
                \"RadioId\": \"Radio ID\",\
                \"ChainToCalibrate\": \"Chain to calibrate,split ,\"\
                }";
    }

    bool CTSE_WlanRxCalibrationFWControlled::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"RadioId",strParamName))
        {
            mnRadioId = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ChainToCalibrate",strParamName))
        {
            mvChainToCalibrate.clear();
            JGW_ParserIntW(strParamValue,L",",mvChainToCalibrate);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WlanRxCalibrationFWControlled::TSE_Run()
    {
        for (size_t i = 0;i < mvChainToCalibrate.size();i ++)
        {
			size_t chainMask = 1 << mvChainToCalibrate[i];
			GETINSTRUMENT->SwitchLossPath(false,chainMask);
            if (!PHONE->RxCalibrationFWControlled(mnRadioId,mvChainToCalibrate[i],0))
            {
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Wlan Rx Calibration Fail");
                return false;
            }
        }
        return true;
    }
}
