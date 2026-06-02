#include "StdAfx.h"
#include "TSE_ReadSerialComPort.h"

namespace JGW
{
    CTSE_ReadSerialComPort::CTSE_ReadSerialComPort(void) : mnTimeOut(50)
    {
    }


    CTSE_ReadSerialComPort::~CTSE_ReadSerialComPort(void)
    {
    }

    const wchar_t* CTSE_ReadSerialComPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadSerialComPort\",\
                \"TestName\": \"读取串行USB设备数据\",\
                \"PortEnvironment\": \"端口环境变量\",\
                \"BaudRate\":\"COM波特率\",\
                \"TimeOut\": \"查找或者连接超时时间，默认50S\"\
                }";
    }

    bool CTSE_ReadSerialComPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;

        if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOut"))
        {
            mnTimeOut = _wtoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReadSerialComPort::TSE_Run()
    {
        return false;
    }
}

