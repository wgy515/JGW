#include "StdAfx.h"
#include "TSE_OpenModemPort.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
namespace JGW
{
    CCJGW_CSerialModemPort* gpCCJGW_CSerialModemPort = NULL;
    CCJGW_CSerialModemPort* GetSerialModemPort()
    {
        return gpCCJGW_CSerialModemPort;
    }

    CTSE_OpenModemPort::CTSE_OpenModemPort(void)
    {
        gpCCJGW_CSerialModemPort = &mcSerialModemPort;
    }


    CTSE_OpenModemPort::~CTSE_OpenModemPort(void)
    {
        gpCCJGW_CSerialModemPort = NULL;
    }

    const wchar_t* CTSE_OpenModemPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_OpenModemPort\",\
                \"TestName\": \"连接MODEM设备\",\
                \"PortEnvironment\": \"端口环境变量,为空默认自动搜索端口\",\
                \"UsbServiceName\": \"USB设备服务名\",\
                \"UsbVid\": \"USB设备Vid\",\
                \"UsbPid\": \"USB设备Pid\",\
                \"TimeOut\": \"查找或者连接超时时间\"\
                }";
    }

    bool CTSE_OpenModemPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"PortEnvironment"))
        {
            mstrPortEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UsbServiceName"))
        {
            mstrUsbServiceName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UsbVid"))
        {
            mstrUsbVid = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UsbPid"))
        {
            mstrUsbPid = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOut"))
        {
            mnTimeOut = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_OpenModemPort::TSE_Run()
    {
        jgw_timer jt;
        std::wstring strComPort;
        int nPort = -1;

        if (!mstrPortEnvironment.empty()) nPort = GetGlobalEnvironment()->GetInt(mstrPortEnvironment);
        //! 自动查询端口
        if (nPort <= 0)
        {
            LogD(L"Find Modem ComPort ...\n");
            while (jt.elapsed() < mnTimeOut)
            {
                strComPort = JGW_GetComPortToUsbServiceName(mstrUsbServiceName.c_str(),mstrUsbVid.c_str(),mstrUsbPid.empty()?L"":mstrUsbPid.c_str());
                if (strComPort.empty()) { Sleep(1000); continue; }
                LogD(L"Start Open Modem ComPort ...\n");
                if (mcSerialModemPort.OpenModemPort(strComPort)) 
                {
                    return true;
                }
                Sleep(200);
            }
        }
        else
        {
            JGW_FormatWString(strComPort,L"COM%d",nPort);
            while (jt.elapsed() < mnTimeOut)
            {
                LogD(L"Start Open Modem ComPort ...\n");
                if (mcSerialModemPort.OpenModemPort(strComPort)) 
                {
                    return true;
                }
                Sleep(200);
            }
        }

        return false;
    }
}

