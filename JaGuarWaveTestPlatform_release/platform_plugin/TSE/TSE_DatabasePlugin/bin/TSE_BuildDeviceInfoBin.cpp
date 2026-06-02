#include "StdAfx.h"
#include "TSE_BuildDeviceInfoBin.h"

namespace JGW
{
    CTSE_BuildDeviceInfoBin::CTSE_BuildDeviceInfoBin(void) : mbIsEncryptFile(false)
    {
    }


    CTSE_BuildDeviceInfoBin::~CTSE_BuildDeviceInfoBin(void)
    {
    }

    const wchar_t* CTSE_BuildDeviceInfoBin::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_BuildDeviceInfoBin\",\
                \"TestName\": \"Build DeviceInfo.bin\",\
                \"IsEncryptFile\": \"is encrypt file\",\
                \"DeviceInfoFilePathEnvironment\": \"device info file path env\",\
                \"DeviceInfoEnvironment\": \"device info content class env\",\
                \"DeviceInfoBaseXmlFilePath\": \"device info base xml file path\"\
                }";
    }

    bool CTSE_BuildDeviceInfoBin::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"DeviceInfoFilePathEnvironment"))
        {
            mstrDeviceInfoFilePathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DeviceInfoEnvironment"))
        {
            mstrDeviceInfoEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DeviceInfoBaseXmlFilePath"))
        {
            mstrDeviceInfoBaseXmlFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsEncryptFile"))
        {
            mbIsEncryptFile = 1 == _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_BuildDeviceInfoBin::TSE_Run()
    {
        GetGlobalEnvironment()->PutInt(mstrDeviceInfoEnvironment,int(&mDeviceInfoBuf));
        std::wstring strDeviceInfoFilePathVal = GetGlobalEnvironment()->GetString(mstrDeviceInfoFilePathEnvironment);
        std::wstring strDeviceInfoFilePath (strDeviceInfoFilePathVal);
        if (!strDeviceInfoFilePathVal.empty())
        {
            strDeviceInfoFilePath = JGW_RealativePathToAbsPath(strDeviceInfoFilePathVal.c_str());
        }
        return mDeviceInfoBuf.LoadDeviceInfoBuf(strDeviceInfoFilePath,mstrDeviceInfoBaseXmlFilePath,mbIsEncryptFile);
    }

}

