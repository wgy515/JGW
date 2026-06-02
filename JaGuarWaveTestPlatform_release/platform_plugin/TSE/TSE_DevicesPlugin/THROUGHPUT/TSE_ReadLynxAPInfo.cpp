#include "StdAfx.h"
#include "TSE_ReadLynxAPInfo.h"
#include <fstream>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_ReadLynxAPInfo::CTSE_ReadLynxAPInfo(void)
    {
    }


    CTSE_ReadLynxAPInfo::~CTSE_ReadLynxAPInfo(void)
    {
    }

    const wchar_t* CTSE_ReadLynxAPInfo::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadLynxAPInfo\",\
                \"TestName\": \"读取LYNX SSID PASSWORD(lynx)\",\
                \"SoftAPPath\": \"softap文件路径\",\
                \"SSIDEnvironment\": \"SSID环境变量\",\
                \"PasswordEnvironment\": \"PASSWORD环境变量\"\
                }";
    }

    bool CTSE_ReadLynxAPInfo::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SoftAPPath"))
        {
            mstrSoftApConfPath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SSIDEnvironment"))
        {
            mstrSSIDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PasswordEnvironment"))
        {
            mstrPasswordEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }
    typedef unsigned char  uint8;
    typedef unsigned short uint16;
    typedef   signed short  int16;
    typedef unsigned int   uint32;
    typedef   signed int    int32;
    typedef unsigned int   uint;

    bool CTSE_ReadLynxAPInfo::TSE_Run()
    {
        struct s_wigig_softap_conf 
        {
            uint32 resverd;
            uint8 space;
            uint8 ssid_len;
            char ssid[1];
        };

        std::ifstream fssid_conf;
        char szBuf[256] = {0};

        fssid_conf.open(JGW_W2A(mstrSoftApConfPath));
        if (!fssid_conf.is_open())
        {
            LogE(L"打开wigig_softap.conf失败\n");
            return false;
        }

        fssid_conf.read(szBuf,256);
        s_wigig_softap_conf* psWigigSsid = (s_wigig_softap_conf*)szBuf;
        s_wigig_softap_conf* psWigigPassword = (s_wigig_softap_conf*)(psWigigSsid->ssid + psWigigSsid->ssid_len + 8);

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_GetFormatWString(L"%s %s",JGW_A2W_A(psWigigSsid->ssid).c_str(),JGW_A2W_A(psWigigPassword->ssid).c_str()).c_str());
        GetGlobalEnvironment()->PutString(mstrSSIDEnvironment,JGW_A2W_A(psWigigSsid->ssid));
        GetGlobalEnvironment()->PutString(mstrPasswordEnvironment,JGW_A2W_A(psWigigPassword->ssid));

        return true;
    }
}