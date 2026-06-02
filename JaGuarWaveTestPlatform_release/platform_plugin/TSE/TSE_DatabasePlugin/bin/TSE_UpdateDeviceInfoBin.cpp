#include "StdAfx.h"
#include "TSE_UpdateDeviceInfoBin.h"
#include "JGW_DeviceInfoBuf.h"
namespace JGW
{
    CTSE_UpdateDeviceInfoBin::CTSE_UpdateDeviceInfoBin(void)
    {
    }


    CTSE_UpdateDeviceInfoBin::~CTSE_UpdateDeviceInfoBin(void)
    {
    }

    const wchar_t* CTSE_UpdateDeviceInfoBin::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_UpdateDeviceInfoBin\",\
                \"TestName\": \"Update DeviceInfo.bin entry\",\
                \"DeviceInfoEnvironment\": \"device info content class env\",\
                \"EntryNames\": \"device info entry name list split ,\",\
                \"EntryValueEnv\": \"device info entry value env list split \"\
                }";
    }

    bool CTSE_UpdateDeviceInfoBin::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"DeviceInfoEnvironment"))
        {
            mstrDeviceInfoEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EntryNames"))
        {
            mvEntryNames.clear();
            JGW_ParserStrA(JGW_W2A(strParamValue).c_str(),",",mvEntryNames);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EntryValueEnv"))
        {
            mvEntryValueEnv.clear();
            JGW_ParserStrW(strParamValue,L",",mvEntryValueEnv);
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_UpdateDeviceInfoBin::TSE_Run()
    {
        if (mvEntryNames.size() > mvEntryValueEnv.size())
        {
            LogE(L"Too many parameters");
            return false;
        }

        CJGW_DeviceInfoBuf* mpDeviceInfoBuf = (CJGW_DeviceInfoBuf*)(GetGlobalEnvironment()->GetInt(mstrDeviceInfoEnvironment));
        if (!mpDeviceInfoBuf)
        {
            LogE(L"Not Find DeviceInfo Bin Info");
            return false;
        }
        std::wstring strTemp;
        for (size_t i = 0;i < mvEntryNames.size();i ++)
        {
            mpDeviceInfoBuf->UpdateDeviceInfo(mvEntryNames[i],GetGlobalEnvironment()->GetAnsiString(mvEntryValueEnv[i]));
            strTemp += L",";
            strTemp += GetGlobalEnvironment()->GetString(mvEntryValueEnv[i]); 
        }
        if (strTemp.length() > 0)
        {
            strTemp.erase(0,1);
        }

        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
        return true;
    }
}

