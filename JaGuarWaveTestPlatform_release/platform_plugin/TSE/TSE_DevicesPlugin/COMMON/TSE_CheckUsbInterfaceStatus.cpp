#include "StdAfx.h"
#include "TSE_CheckUsbInterfaceStatus.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_DevicePool/Auto_ID_Usb.h>
#pragma comment(lib,"JGW_DevicePool.lib")
namespace JGW
{
    CTSE_CheckUsbInterfaceStatus::CTSE_CheckUsbInterfaceStatus(void) : mstrUsbServiceName(L"usbccgp"),mnTestTimeOut(10),mstrUsbVid(L"VID_05C6"),mstrUsbPid(L"PID_9025")
    {
        mnIntervalTimeMS = 1000;
    }


    CTSE_CheckUsbInterfaceStatus::~CTSE_CheckUsbInterfaceStatus(void)
    {
    }

    const wchar_t* CTSE_CheckUsbInterfaceStatus::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CheckUsbInterfaceStatus\",\
                \"TestName\": \"根据PID VID检查当前USB接口状态 2.0/3.0/3.1\",\
                \"UsbVid\": \"USB设备VID\",\
                \"UsbPid\": \"USB设备PID\",\
                \"UsbServiceName\": \"USB设备服务名称\",\
                \"BCDUSB\": \"usb协议版本 0x0200,0x0300\",\
                \"TimeOut\": \"检测超时时间\"\
                }";
    }

    bool CTSE_CheckUsbInterfaceStatus::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"BCDUSB"))
        {
            _stscanf_s(strParamValue,L"%X",&mBcdUsb);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOut"))
        {
            mnTestTimeOut = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CheckUsbInterfaceStatus::TSE_Run()
    {
        std::vector<std::wstring> vEnumPidVid;
        std::wstring strRegUsbPidVid;

        LogD(L"Start Check USB Protocol Version");
        for (size_t i = 0;i < mnTestTimeOut;i ++,Sleep(1000))
        {
            vEnumPidVid.clear();
            JGW_GetUsbEnumPidVidVectotToServiceName(mstrUsbServiceName.c_str(),vEnumPidVid);
            if (vEnumPidVid.empty())
            {
                LogE_F(L"Usb Service Name(%s) Not Find Usb Device Inserted",mstrUsbServiceName.c_str());
                continue;
            }

            for (size_t i = 0;i < vEnumPidVid.size();i ++)
            {
                LogD_F(L"Find Usb Device Instance %s",vEnumPidVid[i].c_str());
                if ((mstrUsbVid.empty() || NULL != JGW_WStrCaseWStr(vEnumPidVid[i].c_str(),mstrUsbVid.c_str())) 
                    && (mstrUsbPid.empty() || NULL != JGW_WStrCaseWStr(vEnumPidVid[i].c_str(),mstrUsbPid.c_str())))
                {
                    strRegUsbPidVid = vEnumPidVid[i];
                    break;
                }
            }

            if (strRegUsbPidVid.empty())
            {
                LogE_F(L"USB PID(%s) VID(%s) Not Find",mstrUsbVid.c_str(),mstrUsbPid.c_str());
                continue;
            }
            unsigned short usb_version = GetUsbProtocolVersionToInstance(strRegUsbPidVid.c_str());
            if (mBcdUsb == usb_version)
            {
                LogI_F(L"USB Protocol Version : 0x%04X",mBcdUsb);
                return true;
            }
            else
            {
                LogE_F(L"Read USB Protocol Version : 0x%04X (Expect : 0x%04X)",usb_version,mBcdUsb);
                //return false;
            } 
        }

        return false;
    }   
}