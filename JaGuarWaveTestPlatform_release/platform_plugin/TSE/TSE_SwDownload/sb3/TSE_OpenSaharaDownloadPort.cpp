#include "StdAfx.h"
#include "TSE_OpenSaharaDownloadPort.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
namespace JGW
{
    CTSE_OpenSaharaDownloadPort::CTSE_OpenSaharaDownloadPort(void) : mnTimeOutSec(10),mstrSaharaSerialClassEnv(L"TSE_SAHARA_SERIAL_CLASS")
    {
    }


    CTSE_OpenSaharaDownloadPort::~CTSE_OpenSaharaDownloadPort(void)
    {
    }

    const wchar_t* CTSE_OpenSaharaDownloadPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_OpenSaharaDownloadPort\",\
                \"TestName\": \"Open Sahara Download Port\",\
                \"SaharaSerialClassEnv\": \"save sahara serial class environment\",\
                \"PortEnv\": \"is empty : auto select port\",\
                \"TimeOutSec\": \"connect sahara serial time out sec\"\
                }";
    }

    bool CTSE_OpenSaharaDownloadPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"PortEnv"))
        {
            mstrPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaharaSerialClassEnv"))
        {
            mstrSaharaSerialClassEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mnTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }
    ///////////////////////////////////////////////////////////////////////////
    /// \"UsbServiceName\": \"USB Service Name\",\\"UsbVid\": \"USB Vid\",\\"UsbPid\": \"USB Pid\",\*/
    bool CTSE_OpenSaharaDownloadPort::TSE_Run()
    {
        int nPort = -1;
        if (!mstrPortEnv.empty()) nPort = GetGlobalEnvironment()->GetInt(mstrPortEnv);
        std::wstring strComPort;
        if (nPort <= 0)
        {
            LogD(L"Find Sahara Download ComPort ...\n");
            jgw_timer jt;
            while (jt.elapsed() < mnTimeOutSec)
            {
                strComPort = JGW_GetComPortToUsbServiceName(L"qcusbser",L"VID_05C6",L"PID_9008");
                if (!strComPort.empty()) 
                {
                    break;
                }
                Sleep(1000);
            }
            if (strComPort.empty())
            {
                LogE("Find Sahara Download ComPort Fail");
                return false;
            }
        }
        else
        {
            JGW_FormatWString(strComPort,L"COM%d",nPort);
        }

        if (!mcSerialPort.OpenSaharaSerialComPort(strComPort))
        {
            return false;
        }
        GetGlobalEnvironment()->PutInt(mstrSaharaSerialClassEnv,(int)(&mcSerialPort));

        return true;
    }

    //bool CTSE_OpenSaharaDownloadPort::OpenSaharaDownloadPort(const std::wstring& strComPort)
    //{
    //    std::wstring strComName(_T("\\\\.\\"));
    //    strComName += strComPort;
    //    LogI_F(L"Open Sahara Download Port:%s",strComName.c_str());
    //    if (!mcSerialPort.OpenPort(strComName,NULL))
    //    {
    //        LogE(L"Could not open to the COMPORT");
    //        return false;
    //    }

    //    COMMTIMEOUTS timeouts = {0};

    //    //timeouts.ReadIntervalTimeout =500;//500 注释的时间比较稳定
    //    //timeouts.ReadTotalTimeoutMultiplier =10;//100
    //    //timeouts.ReadTotalTimeoutConstant =50;//500
    //    ////设定写超时
    //    //timeouts.WriteTotalTimeoutMultiplier =100;
    //    //timeouts.WriteTotalTimeoutConstant =500;

    //    timeouts.ReadIntervalTimeout        = 10000;
    //    timeouts.ReadTotalTimeoutMultiplier = 10000;
    //    timeouts.ReadTotalTimeoutConstant   = 10000;
    //    timeouts.WriteTotalTimeoutMultiplier= 10000;
    //    timeouts.WriteTotalTimeoutConstant  = 10000;
    //    mcSerialPort.SetCommPortTimeout(&timeouts);

    //    return true;
    //}
}
