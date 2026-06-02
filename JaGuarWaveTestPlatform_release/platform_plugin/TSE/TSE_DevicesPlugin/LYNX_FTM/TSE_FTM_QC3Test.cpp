#include "StdAfx.h"
#include "TSE_FTM_QC3Test.h"
#include "CJGW_FTM_QC3InfoResponceVoJson.h"

namespace JGW
{
    CTSE_FTM_QC3Test::CTSE_FTM_QC3Test(void) : mstrMode("QC3.0")
    {
    }


    CTSE_FTM_QC3Test::~CTSE_FTM_QC3Test(void)
    {
    }

    const wchar_t* CTSE_FTM_QC3Test::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_QC3Test\",\
                \"TestName\": \"充电测试(LYNX FTM)\",\
                \"mode\": \"USB_HVDCP_3 : QC3.0 USB_HVDCP : QC2.0 USB_DCP : Normal USB : PC USB  other\"\
                }";
    }

    bool CTSE_FTM_QC3Test::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"mode"))
        {
            mstrMode = JGW_W2A_W(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_QC3Test::TSE_Run()
    {
        CCJGW_FTM_Json ftmJson;
        std::string strJsonCommand = CTSE_FTM_JSONCommand::GetQC3InfoJson();


        if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_QC_3_CODE,ftmJson,mTimeOutSec)) return false;

        CCJGW_FTM_QC3InfoResponceVoJson qc3InfoResponceInfo;
        if (!qc3InfoResponceInfo.FromJosn(ftmJson)) return false;

        if (NULL != mstrMode.compare(qc3InfoResponceInfo.mode))
        {
            LogE_F("设备当前的充电模式：%s,与期望的充电模式(%s)不一致",qc3InfoResponceInfo.mode.c_str(),mstrMode.c_str());
            Sleep(mnIntervalTimeMS);
            return false;
        }
        return true;
    }
}