#include "StdAfx.h"
#include "TSE_QMSLLog.h"
#include <JGW_QMSLFuncPlugin/jgw_device_api.h>
namespace JGW
{
    CTSE_QMSLLog::CTSE_QMSLLog(void) : mbLogOn(false),mstrFileName(JGW_RealativePathToAbsPathA("qmsl.log")),mbDutLog(false)
    {
    }


    CTSE_QMSLLog::~CTSE_QMSLLog(void)
    {
    }

    const wchar_t* CTSE_QMSLLog::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_QMSLLog\",\
                \"TestName\": \"QMSL log\",\
                \"ResourceContextEnv\": \"get open port handle environment\",\
                \"LogOn\": \"log on true:1:false:0,def:0\",\
                \"FileName\": \"Filename for logging def:dutlog.txt\"\
                }";
    }

    bool CTSE_QMSLLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"LogOn",strParamName))
        {
            mbLogOn = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResourceContextEnv"))
        {
            mstrResourceContextEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"FileName",strParamName))
        {
            mstrFileName = JGW_RealativePathToAbsPathA(JGW_W2A(strParamValue));
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_QMSLLog::TSE_Run()
    {
        HANDLE hResourceContext = (HANDLE)GetGlobalEnvironment()->GetInt(mstrResourceContextEnv);
        if (NULL == hResourceContext)
        {
            LogE_F(L"Could not connect to the Resource Context");
            return false;
        }

        if (mbLogOn)
        {
            if (!mbDutLog)
            {
                QLIB_SetLogFlags(hResourceContext,LOG_ALL);
                QLIB_StartLogging(hResourceContext,const_cast<char*>(mstrFileName.data()));
            }
        }
        else if (mbDutLog)
        {
            QLIB_StopLogging(hResourceContext);
        }
        mbDutLog = mbLogOn;
        return true;
    }
}

