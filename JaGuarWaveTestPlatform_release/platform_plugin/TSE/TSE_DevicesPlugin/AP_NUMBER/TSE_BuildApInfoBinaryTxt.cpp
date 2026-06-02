#include "StdAfx.h"
#include "TSE_BuildApInfoBinaryTxt.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_BuildApInfoBinaryTxt::CTSE_BuildApInfoBinaryTxt(void)
    {
    }


    CTSE_BuildApInfoBinaryTxt::~CTSE_BuildApInfoBinaryTxt(void)
    {
    }

    const wchar_t* CTSE_BuildApInfoBinaryTxt::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_BuildApInfoBinaryTxt\",\
                \"TestName\": \"构建小AP 号段配置文件(unix格式)\",\
                \"SNEnvironment\": \"SN对应的环境变量值\",\
                \"MACEnvironment\": \"MAC对应的环境变量值\",\
                \"SSIDEnvironment\": \"SSID对应的环境变量值\",\
                \"PasswordEnvironment\": \"password对应的环境变量值\",\
                \"APTxtFilePathEnvironment\": \"小AP 号段配置文件保存路径环境变量\",\
                \"ModeEnvironment\": \"MODE对应的环境变量值(mode=1 用户模式 mode=0 工程模式)\"\
                }";
    }

    bool CTSE_BuildApInfoBinaryTxt::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SNEnvironment",strParamName))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"MACEnvironment",strParamName))
        {
            mstrMACEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SSIDEnvironment",strParamName))
        {
            mstrSSIDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PasswordEnvironment",strParamName))
        {
            mstrPasswordEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"APTxtFilePathEnvironment",strParamName))
        {
            mstrAPInfoFilePathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ModeEnvironment",strParamName))
        {
            mstrModeEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_BuildApInfoBinaryTxt::TSE_Run()
    {
        char szBuf[1024] = {0};
        size_t len = 0;
        std::string strTemp,strAPTxtFilePath = JGW_W2A(JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrAPInfoFilePathEnvironment).c_str()));
        std::wstring temp,numberInfo;
        
        len = strlen("[ap_info]\n");
        memcpy_s(szBuf,1024,"[ap_info]\n",strlen("[ap_info]\n"));

        if (!mstrSNEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrSNEnvironment).c_str();
            numberInfo += temp;
            strTemp = JGW_W2A(temp);
            memcpy_s(szBuf + len,1024,"sn=",strlen("sn="));
            len += strlen("sn=");
            memcpy_s(szBuf + len,1024,strTemp.c_str(),strTemp.length());
            len += strTemp.length();
            szBuf[len++] = '\n';
        }

        if (!mstrMACEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrMACEnvironment).c_str();
            numberInfo += L",";
            numberInfo += temp;
            strTemp = JGW_W2A(temp);
            memcpy_s(szBuf + len,1024,"mac=",strlen("mac="));
            len += strlen("mac=");
            memcpy_s(szBuf + len,1024,strTemp.c_str(),strTemp.length());
            len += strTemp.length();
            szBuf[len++] = '\n';
        }
        else
        {
            memcpy_s(szBuf + len,1024,"mac=\n",strlen("mac=\n"));
            len += strlen("mac=\n");
        }
        if (!mstrSSIDEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrSSIDEnvironment).c_str();
            numberInfo += L",";
            numberInfo += temp;
            strTemp = JGW_W2A(temp);
            memcpy_s(szBuf + len,1024,"ssid=",strlen("ssid="));
            len += strlen("ssid=");
            memcpy_s(szBuf + len,1024,strTemp.c_str(),strTemp.length());
            len += strTemp.length();
            szBuf[len++] = '\n';
        }
        else
        {
            memcpy_s(szBuf + len,1024,"ssid=\n",strlen("ssid=\n"));
            len += strlen("ssid=\n");
        }
        if (!mstrPasswordEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrPasswordEnvironment).c_str();
            numberInfo += L",";
            numberInfo += temp;
            strTemp = JGW_W2A(temp);
            memcpy_s(szBuf + len,1024,"password=",strlen("password="));
            len += strlen("password=");
            memcpy_s(szBuf + len,1024,strTemp.c_str(),strTemp.length());
            len += strTemp.length();
            szBuf[len++] = '\n';
        }
        else
        {
            memcpy_s(szBuf + len,1024,"password=\n",strlen("password=\n"));
            len += strlen("password=\n");
        }

        if (!mstrModeEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrModeEnvironment).c_str();
            numberInfo += L",";
            numberInfo += temp;
            strTemp = JGW_W2A(temp);
            memcpy_s(szBuf + len,1024,"mode=",strlen("mode="));
            len += strlen("mode=");
            memcpy_s(szBuf + len,1024,strTemp.c_str(),strTemp.length());
            len += strTemp.length();
            //szBuf[len++] = '\n';
        }
        else
        {
            memcpy_s(szBuf + len,1024,"mode=0\n",strlen("mode=0\n"));
            len += strlen("mode=0\n");
        }

        if (!JGW_WriteFileToBuf(strAPTxtFilePath.c_str(),szBuf,len))
        {
            LogE_F("Write File(%s) Fail\n",strAPTxtFilePath.c_str());
            return false;
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,numberInfo.c_str());
        return true;
    }
}