#include "StdAfx.h"
#include "TSE_DutLog.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CTSE_DutLog::CTSE_DutLog(void) : mbLogOn(false),mstrFileName(JGW_RealativePathToAbsPathA("dutlog.txt")),mbDutLog(false)
    {
    }


    CTSE_DutLog::~CTSE_DutLog(void)
    {
    }

    const wchar_t* CTSE_DutLog::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DutLog\",\
                \"TestName\": \"Dut log\",\
                \"LogOn\": \"log on true:1:false:0,def:0\",\
                \"FileName\": \"Filename for logging def:dutlog.txt\"\
                }";
    }

    bool CTSE_DutLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"LogOn",strParamName))
        {
            mbLogOn = (1 == _ttoi(strParamValue));
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

    bool CTSE_DutLog::TSE_Run()
    {
        if (mbLogOn)
        {
            if (!mbDutLog)
            {
                PHONE->StartLogging(mstrFileName);
            }
        }
        else if (mbDutLog)
        {
            PHONE->StopLogging();
        }
        mbDutLog = mbLogOn;
        return true;
    }
}

