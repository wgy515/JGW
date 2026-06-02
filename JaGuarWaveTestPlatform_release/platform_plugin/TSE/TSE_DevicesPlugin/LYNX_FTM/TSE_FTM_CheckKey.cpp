#include "StdAfx.h"
#include "TSE_FTM_CheckKey.h"
#include "TSE_FTM_JSONCommand.h"
#include "TSE_FTM_Define.h"
#include "CJGW_FTM_JsonSocket.h"
#include "CJGW_FTM_KeyEventResponceVoJson.h"

#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_FTM_CheckKey::CTSE_FTM_CheckKey(void):mKeyTimeOutSec(10)
    {
    }


    CTSE_FTM_CheckKey::~CTSE_FTM_CheckKey(void)
    {
    }

    const wchar_t* CTSE_FTM_CheckKey::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_CheckKey\",\
                \"TestName\": \"按键测试(LYNX FTM)\",\
                \"keyType\": \"按键类型(PowerKey,ResetKey)\",\
                \"event\": \"事件类型(up,down)\",\
                \"TimeOutSec\": \"发送命令超时时间,以S为单位\",\
                \"KeyTimeOutSec\": \"按键超时时间设置,以S为单位\"\
                }";
    }

    bool CTSE_FTM_CheckKey::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"keyType"))
        {
            mstrKeyType = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"event"))
        {
            mstrEvent = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mTimeOutSec = _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"KeyTimeOutSec"))
        {
            mKeyTimeOutSec = _wtoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_CheckKey::TSE_Run()
    {
        CCJGW_FTM_Json ftmJson;
        std::string json = CTSE_FTM_JSONCommand::GetKesyJson(E_TSE_FTM_KEY_START_STATUS);
        if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(json,E_REQ_KEYS_CODE,ftmJson,mTimeOutSec))
        {
            return false;
        }

        CCJGW_FTM_KeyEventResponceVoJson keyEventResponceVoJson;
        jgw_timer tm;
        do
        {
            keyEventResponceVoJson.FromJosn(ftmJson);
            if (NULL == JGW_StrComparenoCaseStr(mstrKeyType.c_str(),keyEventResponceVoJson.keyType.c_str())
                && NULL == JGW_StrComparenoCaseStr(mstrEvent.c_str(),keyEventResponceVoJson.event.c_str()))
            {
                return true;
            }
            CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket("",E_REQ_KEYS_CODE,ftmJson,mTimeOutSec);
        }while (tm.elapsed() < mKeyTimeOutSec);

        return false;
    }

    bool CTSE_FTM_CheckKey::TSE_Exit()
    {
        std::string json = CTSE_FTM_JSONCommand::GetKesyJson(E_TSE_FTM_KEY_END_STATUS);
        CCJGW_FTM_JsonSocket::WriteJsonSocket(json,mTimeOutSec);
        return true;
    }
}