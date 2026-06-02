#include "StdAfx.h"
#include "TSE_OpenSerialComPort.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_OpenSerialComPort::CTSE_OpenSerialComPort(void) : mnTimeOut(50),mnBaudRate(CBR_9600),mnDatabits(8),mnStopbits(ONESTOPBIT),mnParity(NOPARITY),mnFlowControl(0),mbIsSetControlCommState(false)
    {
    }


    CTSE_OpenSerialComPort::~CTSE_OpenSerialComPort(void)
    {
    }

    const wchar_t* CTSE_OpenSerialComPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_OpenSerialComPort\",\
                \"TestName\": \"连接串行USB设备\",\
                \"PortEnvironment\": \"端口环境变量\",\
                \"BaudRate\":\"COM波特率\",\
                \"IsSetControlCommState\":\"Is Set ControlCommState def：0\",\
                \"Databits\":\"COM数据位默认8\",\
                \"Stopbits\":\"COM停止位(ONESTOPBIT:0,ONE5STOPBITS,1,TWOSTOPBITS 2)默认2\",\
                \"Parity\":\"COM校验位0,1,2 = 1, 1.5, 2 ,def 0\",\
                \"FlowControl\":\"COM流控制0:NoOne,1:X0F/X0FF,2:RTS/CTS,3:DSR/DTR\",\
                \"UsbServiceName\": \"USB设备服务名\",\
                \"UsbVid\": \"USB设备Vid\",\
                \"UsbPid\": \"USB设备Pid\",\
                \"EndString\": \"串口读取结束符\",\
                \"TimeOut\": \"查找或者连接超时时间，默认50S\"\
                }";
    }

    bool CTSE_OpenSerialComPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"BaudRate"))
        {
            mnBaudRate = _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsSetControlCommState"))
        {
            mbIsSetControlCommState = 1 == _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Databits"))
        {
            mnDatabits = _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Stopbits"))
        {
            mnStopbits = _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Parity"))
        {
            mnParity = _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"FlowControl"))
        {
            mnFlowControl = _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOut"))
        {
            mnTimeOut = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EndString"))
        {
            mstrEndString = JGW_W2A(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_OpenSerialComPort::TSE_Run()
    {
        jgw_timer jt;
        std::wstring strComPort;

        CTSE_DevicesGlobalResource::GetSerialComPort().CloseSerialComPort();
        //! 自动查询端口
        if (mstrPortEnvironment.empty())
        {
            LogD(L"Find Serail ComPort ...\n");
            while (jt.elapsed() < mnTimeOut)
            {
                strComPort = JGW_GetComPortToUsbServiceName(mstrUsbServiceName.c_str(),mstrUsbVid.c_str(),mstrUsbPid.empty()?L"":mstrUsbPid.c_str());
                if (strComPort.empty()) { Sleep(1000); continue; }
                LogD(L"Start Open Serail ComPort ...\n");
                CTSE_DevicesGlobalResource::GetSerialComPort().SetEndString(mstrEndString.c_str());
                if (CTSE_DevicesGlobalResource::GetSerialComPort().OpenSerialComPort(strComPort,false,mnBaudRate)) return  SetControlCommState();
                Sleep(200);
            }
        }
        else
        {
            int nPort = GetGlobalEnvironment()->GetInt(mstrPortEnvironment);
            if (nPort <= 0)
            {
                LogD(L"Find Serail ComPort ...\n");
                while (jt.elapsed() < mnTimeOut)
                {
                    strComPort = JGW_GetComPortToUsbServiceName(mstrUsbServiceName.c_str(),mstrUsbVid.c_str(),mstrUsbPid.empty()?L"":mstrUsbPid.c_str());
                    if (strComPort.empty()) { Sleep(1000); continue; }
                    LogD(L"Start Open Serail ComPort ...\n");
                    CTSE_DevicesGlobalResource::GetSerialComPort().SetEndString(mstrEndString.c_str());
                    if (CTSE_DevicesGlobalResource::GetSerialComPort().OpenSerialComPort(strComPort,false,mnBaudRate)) return  SetControlCommState();
                    Sleep(200);
                }
            }

            JGW_FormatWString(strComPort,L"COM%d",nPort);
            while (jt.elapsed() < mnTimeOut)
            {
                LogD(L"Start Open Serial ComPort ...\n");
                CTSE_DevicesGlobalResource::GetSerialComPort().SetEndString(mstrEndString.c_str());
                if (CTSE_DevicesGlobalResource::GetSerialComPort().OpenSerialComPort(strComPort,false,mnBaudRate)) return SetControlCommState();
                Sleep(200);
            }
        }

        return false;
    }

    bool CTSE_OpenSerialComPort::SetControlCommState()
    {
        if (!mbIsSetControlCommState) return true;

        DCB dcb;
        dcb.DCBlength =sizeof(DCB);
        CTSE_DevicesGlobalResource::GetSerialComPort().GetControlCommState(dcb);

        dcb.fBinary = TRUE;
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
        dcb.fDsrSensitivity = FALSE;
        dcb.fTXContinueOnXoff = FALSE;
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        dcb.fErrorChar = FALSE;
        dcb.fNull = FALSE;
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        dcb.fAbortOnError = FALSE;
        dcb.fOutxCtsFlow = FALSE;
        dcb.fOutxDsrFlow = FALSE;
        dcb.BaudRate = mnBaudRate;
        dcb.ByteSize = mnDatabits;
        dcb.StopBits = mnStopbits;

        if (0 == mnParity)
        {
            dcb.fParity = FALSE;
            dcb.Parity = NOPARITY;
        }
        else
        {
            dcb.fParity = TRUE;
            dcb.Parity = mnParity;
        }
#define SER_FLOW_NONE 0
#define SER_FLOW_XONXOFF 1
#define SER_FLOW_RTSCTS 2
#define SER_FLOW_DSRDTR 3
        switch (mnFlowControl) 
        {
        case SER_FLOW_NONE:
            break;
        case SER_FLOW_XONXOFF:
            dcb.fOutX = dcb.fInX = TRUE;
            break;
        case SER_FLOW_RTSCTS:
            dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
            dcb.fOutxCtsFlow = TRUE;
            break;
        case SER_FLOW_DSRDTR:
            dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
            dcb.fOutxDsrFlow = TRUE;
            break;
        }
        //if (0 == mnFlowControl)
        //{
        //    dcb.fOutX = FALSE;
        //    dcb.fInX = FALSE;
        //    dcb.fRtsControl = RTS_CONTROL_DISABLE;
        //    dcb.fOutxCtsFlow = 0;
        //    dcb.fDtrControl =0 ;//MODEL通信必须
        //    dcb.fDsrSensitivity = 0;
        //    dcb.fOutxDsrFlow = 0;
        //}// XON/XFF
        //else if (1 == mnFlowControl)
        //{
        //    dcb.fOutX = TRUE;
        //    dcb.fInX = TRUE;
        //    dcb.fRtsControl = RTS_CONTROL_DISABLE;
        //    dcb.fOutxCtsFlow = 0;
        //    dcb.fDtrControl =0 ;//MODEL通信必须
        //    dcb.fDsrSensitivity = 0;
        //    dcb.fOutxDsrFlow = 0;
        //}//! RTS/CTS
        //else if (2 == mnFlowControl)
        //{
        //    dcb.fOutX = FALSE;
        //    dcb.fInX = FALSE;
        //    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
        //    dcb.fOutxCtsFlow = 1;
        //    dcb.fDtrControl =0 ;//MODEL通信必须
        //    dcb.fDsrSensitivity = 0;
        //    dcb.fOutxDsrFlow = 0;
        //}//! DSR/DTR
        //else if (3 == mnFlowControl)
        //{
        //    dcb.fOutX = FALSE;
        //    dcb.fInX = FALSE;
        //    dcb.fRtsControl = RTS_CONTROL_DISABLE;
        //    dcb.fOutxCtsFlow = 0;
        //    dcb.fDtrControl =DTR_CONTROL_HANDSHAKE ;//MODEL通信必须
        //    dcb.fDsrSensitivity = TRUE;
        //    dcb.fOutxDsrFlow = DTR_CONTROL_HANDSHAKE;
        //}
        return CTSE_DevicesGlobalResource::GetSerialComPort().SetControlCommState(dcb);
    }
}

