#include "StdAfx.h"
#include "TSE_FTM_CheckVersion.h"
#include "..\TSE_DevicesGlobalResource.h"
#include "TSE_FTM_JSONCommand.h"
#include "TSE_FTM_Define.h"
#include "CJGW_FTM_JsonSocket.h"

namespace JGW
{
    CTSE_FTM_CheckVersion::CTSE_FTM_CheckVersion(void)
    {
    }

    const wchar_t* CTSE_FTM_CheckVersion::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_CheckVersion\",\
                \"TestName\": \"检测版本信息(LYNX FTM)\",\
                \"ClientVersion\": \"客户版本号\",\
                \"BuildNumber\": \"内部版本号,编译版本号\",\
                \"ModemVersion\": \"Modem版本号\",\
                \"GSMRFCalFlag\": \"GSM校准标记位\",\
                \"WCDMARFCalFlag\": \"WCDMA校准标记位\",\
                \"LTERFCalFlag\": \"LTE校准标记位\",\
                \"GSMVFSFlag\": \"GSM综测标记位\",\
                \"WCDMAVFSFlag\": \"WCDMA综测标记位\",\
                \"LTEVFSFlag\": \"LTE综测标记位\",\
                \"TimeOutSec\": \"通信超时时间设置 以S为单位，默认10S\"\
                }";
    }


    bool CTSE_FTM_CheckVersion::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ClientVersion"))
        {
            mstrClientVersion = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"BuildNumber"))
        {
            mstrBuildNumber = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ModemVersion"))
        {
            mstrModemVersion = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"GSMRFCalFlag"))
        {
            mstrGSMRFCalFlag = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WCDMARFCalFlag"))
        {
            mstrWCDMARFCalFlag = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"LTERFCalFlag"))
        {
            mstrLTERFCalFlag = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"GSMVFSFlag"))
        {
            mstrGSMVFSFlag = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WCDMAVFSFlag"))
        {
            mstrWCDMAVFSFlag = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"LTEVFSFlag"))
        {
            mstrLTEVFSFlag = JGW_W2A_W(strParamValue);
        }
        else
        {
            mmapUserDefineCompare[JGW_W2A_W(strParamName)] = (JGW_W2A_W(strParamValue));
        }

        return true;
    }

    bool CTSE_FTM_CheckVersion::TSE_Run()
    {
        CCJGW_FTM_Json ftmJson;
        std::string strJsonCommand = CTSE_FTM_JSONCommand::GetVersionJson();
        if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_VERSION_CODE,ftmJson,mTimeOutSec))
        {
            return false;
        }

        if (!CheckAttribute(ftmJson,"customVersion",mstrClientVersion,"客户版本号") 
            || !CheckAttribute(ftmJson,"internalVersion",mstrBuildNumber,"内部版本号")
            || !CheckAttribute(ftmJson,"modeVersion",mstrModemVersion,"Modem版本") 
            || !CheckAttribute(ftmJson,"gsm6857",mstrGSMRFCalFlag,"GSM校准标记位") 
            || !CheckAttribute(ftmJson,"wcdma6858",mstrWCDMARFCalFlag,"WCDMA校准标记位")
            || !CheckAttribute(ftmJson,"lte6860",mstrLTERFCalFlag,"LTE校准标记位")
            || !CheckAttribute(ftmJson,"gsm6853",mstrGSMVFSFlag,"GSM综测标记位")
            || !CheckAttribute(ftmJson,"wcdma6854",mstrWCDMAVFSFlag,"WCDMA综测标记位")
            || !CheckAttribute(ftmJson,"lte6855",mstrLTEVFSFlag,"LTE综测标记位"))
        {
            return false;
        }

        for (std::map<std::string,std::string>::iterator it  = mmapUserDefineCompare.begin();
            it != mmapUserDefineCompare.end();
            ++ it)
        {
            if (!CheckAttribute(ftmJson,it->first.c_str(),it->second))
            {
                return false;
            }
        }

        return true;
    }

    bool CTSE_FTM_CheckVersion::CheckAttribute(CCJGW_FTM_Json& ftmJson,const char* strAttrName,const std::string& strCompareValue,const char* strDefalutName /* = NULL */)
    {
        if (strCompareValue.empty()) return true;

        std::string strAttrValue;
        if (!ftmJson.GetJsonValueToKey<std::string>(strAttrName,strAttrValue))
        {
            LogE_F("%s get value fail\n",strDefalutName?strDefalutName:strAttrName);
            return false;
        }

        if (NULL == strCompareValue.compare(strAttrValue)) return true;
        LogE_F("%s compare fail(config:(%s) device:(%s))"
            ,strDefalutName?strDefalutName:strAttrName
            ,strCompareValue.c_str(),strAttrValue.c_str());

        return false;
    }
}