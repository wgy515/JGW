#include "StdAfx.h"
#include "TSE_SaveDeviceInfoBin.h"
#include "JGW_DeviceInfoBuf.h"
namespace JGW
{
    CTSE_SaveDeviceInfoBin::CTSE_SaveDeviceInfoBin(void) : mbIsEncrypt(false)
    {
    }


    CTSE_SaveDeviceInfoBin::~CTSE_SaveDeviceInfoBin(void)
    {
    }

    const wchar_t* CTSE_SaveDeviceInfoBin::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SaveDeviceInfoBin\",\
                \"TestName\": \"Save DeviceInfo.bin file\",\
                \"IsEncrypt\": \"file is encrypt\",\
                \"DeviceInfoEnvironment\": \"device info content class env\",\
                \"SaveDeviceInfoFilePathEnv\": \"save device info bin file path\"\
                }";
    }

    bool CTSE_SaveDeviceInfoBin::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaveDeviceInfoFilePathEnv"))
        {
            mstrSaveDeviceInfoFilePathEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DeviceInfoEnvironment"))
        {
            mstrDeviceInfoEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsEncrypt"))
        {
            mbIsEncrypt = 1 == _ttoi(strParamValue);
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaveDeviceInfoBin::TSE_Run()
    {
        CJGW_DeviceInfoBuf* mpDeviceInfoBuf = (CJGW_DeviceInfoBuf*)(GetGlobalEnvironment()->GetInt(mstrDeviceInfoEnvironment));
        if (!mpDeviceInfoBuf)
        {
            LogE(L"Not Find DeviceInfo Bin Info");
            return false;
        }

        return mpDeviceInfoBuf->SaveDeviceInfoBuf(JGW_RealativePathToAbsPathA(GetGlobalEnvironment()->GetAnsiString(mstrSaveDeviceInfoFilePathEnv)) ,mbIsEncrypt);
    }
}

