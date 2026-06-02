#include "StdAfx.h"
#include "TSE_FTMVersionTest.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_FTMVersionTest::CTSE_FTMVersionTest(void) : mnAdbTimeOut(10)
    {
        msVersion.mstrCommand = L"adb shell \"getprop |grep ro.build.display.version\"";
        msVersion.mstrResultPos = L"[ro.build.display.version]:";
        msModel.mstrCommand = L"adb shell getprop |grep ro.build.display.model";
        msModel.mstrResultPos = L"[ro.build.display.model]:";
    }

    CTSE_FTMVersionTest::~CTSE_FTMVersionTest(void)
    {
    }

    const wchar_t* CTSE_FTMVersionTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTMVersionTest\",\
                \"TestName\": \"检查Android设备软件版本型号\",\
                \"Model\": \"型号\",\
                \"Version\": \"版本\",\
                \"ModelCommand\": \"型号检测命令\",\
                \"ModelPos\": \"匹配型号字符串位置\",\
                \"VersionCommand\": \"软件版本检测命令\",\
                \"VersionPos\": \"匹配软件版本字符串位置\",\
                \"TimeOut\": \"命令发送超时时间\"\
                }";
    }

    bool CTSE_FTMVersionTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Model"))
        {
            mstrModel = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Version"))
        {
            mstrVersion = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ModelCommand"))
        {
            msModel.mstrCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ModelPos"))
        {
            msModel.mstrResultPos = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"VersionCommand"))
        {
            msVersion.mstrCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"VersionPos"))
        {
            msVersion.mstrResultPos = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOut"))
        {
            mnAdbTimeOut = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTMVersionTest::GetAdbCommandResult(S_ADB_COMMAND_RESULT& sAdbCommandResult)
    {
        CCJGW_ProcessPipe* pCMDPipe = CTSE_DevicesGlobalResource::GetProcessCMDPipe();
        if (!pCMDPipe) return false;

        std::wstring strRead,strTmp;
        if (!pCMDPipe->timed_send(sAdbCommandResult.mstrCommand,mnAdbTimeOut)) return false;
        CTSE_DevicesGlobalResource::ReadCMDProcessPipeEOF(pCMDPipe,strRead);
        const wchar_t* pTemp = JGW_WStrCaseWStr(strRead.c_str(),sAdbCommandResult.mstrResultPos.c_str());
        if (!pTemp) { Sleep(mnIntervalTimeMS); return false;}
        wchar_t strResult[260] = {0};
        JGW_FormatWString(strTmp,L"%s%%s ",sAdbCommandResult.mstrResultPos.c_str());
        _stscanf(pTemp,strTmp.c_str(),strResult);
        LogI_F(L"Get Adb Result : %s\n",strResult);
        sAdbCommandResult.mstrResult = strResult;

        return true;
    }

    bool CTSE_FTMVersionTest::TSE_Run()
    {
        if (!mstrModel.empty() && !GetAdbCommandResult(msModel))
        {
            LogE(L"获取软件型号失败!!!");
            return false;
        }

        if (!mstrModel.empty() && NULL != JGW_WStrComparenoCaseWStr(mstrModel.c_str(),msModel.mstrResult.c_str()))
        {
            LogE_F(L"匹配失败,获取软件型号:%s,需要匹配的型号:%s",msModel.mstrResult.c_str(),mstrModel.c_str());
            return false;
        }

        if (!mstrVersion.empty() && !GetAdbCommandResult(msVersion))
        {
            LogE(L"获取软件版本失败!!!");
            return false;
        }

        if (!mstrVersion.empty() && NULL != JGW_WStrComparenoCaseWStr(mstrVersion.c_str(),msVersion.mstrResult.c_str()))
        {
            LogE_F(L"匹配失败,获取软件版本:%s,需要匹配的版本:%s",msVersion.mstrResult.c_str(),mstrVersion.c_str());
            return false;
        }

        return true;
    }
}