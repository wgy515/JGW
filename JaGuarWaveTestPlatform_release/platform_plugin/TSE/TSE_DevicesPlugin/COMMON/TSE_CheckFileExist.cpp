#include "StdAfx.h"
#include "TSE_CheckFileExist.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
namespace JGW
{
    CTSE_CheckFileExist::CTSE_CheckFileExist(void) : mnCheckTimeOutSec(1)
    {
    }


    CTSE_CheckFileExist::~CTSE_CheckFileExist(void)
    {
    }

    const wchar_t* CTSE_CheckFileExist::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CheckFileExist\",\
                \"TestName\": \"check file is exist\",\
                \"CheckTimeOutSec\": \"check file exist time out\",\
                \"FilePathEnvironment\": \"file path env\"\
                }";
    }

    bool CTSE_CheckFileExist::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"FilePathEnvironment"))
        {
            mstrFilePathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckTimeOutSec"))
        {
            mnCheckTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }
    
    bool CTSE_CheckFileExist::TSE_Run()
    {
        std::wstring strFilePath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrFilePathEnvironment).c_str());
        jgw_timer jt;
        while (jt.elapsed() <= mnCheckTimeOutSec)
        {
            if (JGW_FileExistsToFilePath(strFilePath.c_str()))
            {
                return true;
            }
            Sleep(500);
        }
        return false;
    }
}

