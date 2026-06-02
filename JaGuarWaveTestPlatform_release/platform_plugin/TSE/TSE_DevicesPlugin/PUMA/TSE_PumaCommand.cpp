#include "StdAfx.h"
#include "TSE_PumaCommand.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_PumaCommand::CTSE_PumaCommand(void) : mnTimeOut(10)
    {
    }


    CTSE_PumaCommand::~CTSE_PumaCommand(void)
    {
    }

    const wchar_t* CTSE_PumaCommand::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PumaCommand\",\
                \"TestName\": \"发送Peraso命令并比对返回结果(Peraso)\",\
                \"Command\": \"Peraso命令\",\
                \"ReadResultEnvironment\": \"保存命令返回值到环境变量中\",\
                \"Result\": \"读取结果用于比对\",\
                \"TimeOut\": \"命令发送超时时间秒为单位\"\
                }";
    }

    bool CTSE_PumaCommand::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Command"))
        {
            mstrCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Result"))
        {
            mstrResult = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOut"))
        {
            mnTimeOut = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultEnvironment"))
        {
            mstrReadResultEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_PumaCommand::TSE_Run()
    {
        std::wstring strResultEnv(mstrResult);
        GetEnvironmentCommandString(strResultEnv);

        std::string strCommand = JGW_W2A(mstrCommand) + "\r",strCompare = JGW_W2A(strResultEnv);
        std::string strResult;

        if (!CTSE_DevicesGlobalResource::GetSerialComPort().ReadRespCommandToWriteCommand(strCommand.c_str(),strResult,mnTimeOut * 1000))
        {
            LogE(L"命令写入失败 ！！！");
            return false;
        }
        //! 保存读取结果
        if (!mstrReadResultEnvironment.empty())
        {
            GetGlobalEnvironment()->PutAnsiString(mstrReadResultEnvironment,strResult);
        }

        if (!strResultEnv.empty() && NULL == JGW_StrCaseStr(strResult.c_str(),strCompare.c_str()))
        {
            LogE_F("返回值: %s,期望值: %s,比对错误",strResult.c_str(),strCompare.c_str());
            return false;
        }

        return true;
    }
}
