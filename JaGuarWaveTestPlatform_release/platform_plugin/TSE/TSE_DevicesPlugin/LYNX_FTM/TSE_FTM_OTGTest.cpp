#include "StdAfx.h"
#include "TSE_FTM_OTGTest.h"
#include "CJGW_FTM_OTGInfooResponceVoJson.h"

namespace JGW
{
    CTSE_FTM_OTGTest::CTSE_FTM_OTGTest(void)
    {
    }


    CTSE_FTM_OTGTest::~CTSE_FTM_OTGTest(void)
    {
    }

    const wchar_t* CTSE_FTM_OTGTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_OTGTest\",\
                \"TestName\": \"OTG测试(LYNX FTM)\"\
                }";
    }

    bool CTSE_FTM_OTGTest::TSE_Run()
    {
        CCJGW_FTM_Json ftmJson;
        std::string strJsonCommand = CTSE_FTM_JSONCommand::GetOTGTeseJson();

        if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_OTG_CODE,ftmJson,mTimeOutSec)) return false;

        CCJGW_FTM_OTGInfooResponceVoJson otgInfoResponceInfo;
        if (!otgInfoResponceInfo.FromJosn(ftmJson)) return false;

        if (!otgInfoResponceInfo.otgMounted)
        {
            LogE(L"未发现OTG设备，请插入OTG设备\n");
            Sleep(mnIntervalTimeMS);
            return false;
        }
        return true;
    }
}

