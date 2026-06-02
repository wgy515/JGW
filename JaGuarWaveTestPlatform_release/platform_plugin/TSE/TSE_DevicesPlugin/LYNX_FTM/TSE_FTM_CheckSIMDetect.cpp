#include "StdAfx.h"
#include "TSE_FTM_CheckSIMDetect.h"
#include "CJGW_FTM_SocketResponceVo.h"
namespace JGW
{
    CTSE_FTM_CheckSIMDetect::CTSE_FTM_CheckSIMDetect(void)
    {
    }


    CTSE_FTM_CheckSIMDetect::~CTSE_FTM_CheckSIMDetect(void)
    {
    }

    const wchar_t* CTSE_FTM_CheckSIMDetect::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_CheckSIMDetect\",\
                \"TestName\": \"检查SIM卡(LYNX FTM)\",\
                \"TimeOutSec\": \"通信超时时间设置 以S为单位，默认10S\"\
                }";
    }

    bool CTSE_FTM_CheckSIMDetect::TSE_Run()
    {
        CCJGW_FTM_Json ftmJson;
        std::string strJsonCommand = CTSE_FTM_JSONCommand::GetSimJson();
        if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_SIM_CARD_INFO_CODE,ftmJson,mTimeOutSec)) return false;

        CCJGW_FTM_SocketResponceVo socketResponceVo;
        if (!socketResponceVo.FromJosn(ftmJson)) return false;

        if (TSE_FTM_DEVICE_FAIL == socketResponceVo.mResultCode)
        {
            LogE_F("%s\n",socketResponceVo.mResponseErrorMsg.c_str());
            return false;
        }

        return true;
    }
}