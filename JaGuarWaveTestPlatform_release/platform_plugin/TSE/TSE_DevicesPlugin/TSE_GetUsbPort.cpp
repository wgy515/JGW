#include "StdAfx.h"
#include "TSE_GetUsbPort.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
namespace JGW
{
    CTSE_GetUsbPort::CTSE_GetUsbPort(void): mstrUsbServiceName(L"usbccgp"),mnCheckTimeOut(10),mstrUsbVid(L"VID_05C6"),mstrUsbPid(L"PID_9025")
    {
    }


    CTSE_GetUsbPort::~CTSE_GetUsbPort(void)
    {
    }

    const wchar_t* CTSE_GetUsbPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetUsbPort\",\
                \"TestName\": \"获取USB端口\",\
                \"UsbVid\": \"USB设备VID\",\
                \"UsbPid\": \"USB设备PID\",\
                \"UsbDeviceDes\": \"usb devvice des\",\
                \"UsbServiceName\": \"USB设备服务名称\",\
                \"SavePortEnvrionment\": \"保存端口到指定全局环境变量中\",\
                \"CheckTimeOut\": \"检测超时时间\"\
                }";
    }

    bool CTSE_GetUsbPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"UsbVid"))
        {
            mstrUsbVid = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UsbPid"))
        {
            mstrUsbPid = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UsbServiceName"))
        {
            mstrUsbServiceName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SavePortEnvrionment"))
        {
            mstrSavePortEnvrionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UsbDeviceDes"))
        {
            mstrUsbDeviceDes = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckTimeOut"))
        {
            mnCheckTimeOut = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetUsbPort::TSE_Run()
    {
        jgw_timer jt;
        std::wstring strComPort;
        int nPort = -1;
        LogD(L"Find Usb ComPort ...\n");
        while (jt.elapsed() < mnCheckTimeOut)
        {
            strComPort = JGW_GetComPortToUsbServiceName(mstrUsbServiceName.c_str(),mstrUsbVid.c_str(),mstrUsbPid.empty()?L"":mstrUsbPid.c_str(),mstrUsbDeviceDes.c_str());
            if (strComPort.empty()) 
            { 
                Sleep(1000); 
                continue;
            }
            JGW_ReplaceStringW(strComPort,L"COM",L"");
            nPort = _ttoi(strComPort.c_str());
            LogI_F(L"Find usb comPort : COM%d",nPort);
            LogU(E_TSE_SHOW_CONTENT_INFO,strComPort.c_str());
            GetGlobalEnvironment()->PutInt(mstrSavePortEnvrionment,nPort);
            return true;
        }
        return false;
    }
}
