#include "StdAfx.h"
#include "TSE_ReadDeviceInfoBin.h"
#include "JGW_DeviceInfoBuf.h"
namespace JGW
{
    CTSE_ReadDeviceInfoBin::CTSE_ReadDeviceInfoBin(void)
    {
    }


    CTSE_ReadDeviceInfoBin::~CTSE_ReadDeviceInfoBin(void)
    {
    }

    const wchar_t* CTSE_ReadDeviceInfoBin::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadDeviceInfoBin\",\
                \"TestName\": \"Read DeviceInfo.bin\",\
                \"DeviceInfoEnvironment\": \"device info content class env\",\
                \"EntryNames\": \"device info entry name list split ,\",\
                \"EntryValueEnv\": \"device info entry value env list split \"\
                }";
    }

    bool CTSE_ReadDeviceInfoBin::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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

    bool CTSE_ReadDeviceInfoBin::TSE_Run()
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

        std::string strTemp;
        std::wstring strContent;
        for (size_t i = 0;i < mvEntryNames.size();i ++)
        {
            mpDeviceInfoBuf->ReadDeviceInfo(mvEntryNames[i],strTemp);
            GetGlobalEnvironment()->PutAnsiString(mvEntryValueEnv[i],strTemp);
            strTemp = "";
            strContent += L",";
            strContent += GetGlobalEnvironment()->GetString(mvEntryValueEnv[i]); 
        }

        if (strContent.length() > 0)
        {
            strContent.erase(0,1);
        }
         LogU(E_TSE_SHOW_CONTENT_INFO,strContent.c_str());
        /*for (size_t i = 0;i < mvEntryNames.size();i ++)
        {
        mpDeviceInfoBuf->UpdateDeviceInfo(mvEntryNames[i],GetGlobalEnvironment()->GetAnsiString(mvEntryValueEnv[i]));
        }*/
        return true;
    }
}

