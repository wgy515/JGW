#include "StdAfx.h"
#include "TSE_Connect1hotlinkPeraso.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_Connect1hotlinkPeraso::CTSE_Connect1hotlinkPeraso(void)
    {
    }


    CTSE_Connect1hotlinkPeraso::~CTSE_Connect1hotlinkPeraso(void)
    {
    }

    const wchar_t* CTSE_Connect1hotlinkPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_Connect1hotlinkPeraso\",\
                \"TestName\": \"连接1hotlinkPERASO设备\",\
                \"PortEnvironment\": \"端口环境变量,为空默认自动搜索端口\",\
                \"UsbServiceName\": \"USB设备服务名\",\
                \"UsbVid\": \"USB设备Vid\",\
                \"UsbPid\": \"USB设备Pid\",\
                \"GoldenSerialNumberEnvironment\":\"金机SN环境变量\",\
                \"TimeOut\": \"查找或者连接超时时间\"\
                }";
    }

    bool CTSE_Connect1hotlinkPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"GoldenSerialNumberEnvironment"))
        {
            mstrGoldenSerialNumberEnvironment = strParamValue;
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

    std::wstring CTSE_Connect1hotlinkPeraso::FindGoldenDongles(std::vector<std::wstring>& vstrComPort)
    {
        std::wstring strComPort,strSerialNumber,strGoldenComPort;
        std::wstring strGoldenSerialNumber = GetGlobalEnvironment()->GetString(mstrGoldenSerialNumberEnvironment);

        for (size_t i = 0;i < 2;i ++)
        {
            strComPort = vstrComPort[i];
            LogD(L"Start Open Persaso ComPort ...\n");

            if (!CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->OpenDonglesComPort(strComPort)) 
            {
                Sleep(200);
                break;
            }
            //! 判断golden dongle
            if (!CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->ReadSerialNumber(strSerialNumber))
            {
                Sleep(200);
                CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->CloseDonglesComPort();
                break;
            }
            //! 如果相等则找到了golden dongle
            if (TSE_PARAM_NAME_EQUAL(strSerialNumber.c_str(),strGoldenSerialNumber.c_str()))
            {
                strGoldenComPort = strComPort;
            }
            CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->CloseDonglesComPort();
        }
        return strGoldenComPort;
    }

    bool CTSE_Connect1hotlinkPeraso::TSE_Run()
    {
        jgw_timer jt;
        std::wstring strComPort,strGoldenComPort;
        std::vector<std::wstring> vstrComPort;
        int nPort = -1;

        if (!mstrPortEnvironment.empty()) nPort = GetGlobalEnvironment()->GetInt(mstrPortEnvironment);
        //! 自动查询端口
        if (nPort <= 0)
        {
            LogI_F(L"Find Persaso ComPort ...\n");
            while (jt.elapsed() < mnTimeOut)
            {
                JGW_GetAllComPortToUsbServiceName(vstrComPort,mstrUsbServiceName.c_str(),mstrUsbVid.c_str(),mstrUsbPid.empty()?L"":mstrUsbPid.c_str());
                if (2 != vstrComPort.size())
                {
                    Sleep(1000); 
                    LogI_F(L"Find Peraso Device Count %d",vstrComPort.size());
                    continue;
                }
                //! 查找gold
                strGoldenComPort = FindGoldenDongles(vstrComPort);
                if (strGoldenComPort.empty())
                {
                    LogI_F(L"未查找到金机SN设备,默认金机设备SN为: %s",GetGlobalEnvironment()->GetString(mstrGoldenSerialNumberEnvironment).c_str());
                    Sleep(1000); 
                    continue;
                }

                for (size_t i = 0;i < 2;i ++)
                {
                    if (NULL == strGoldenComPort.compare(vstrComPort[i])) continue;
                    if (CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->OpenDonglesComPort(vstrComPort[i])) return true;

                    Sleep(200);
                    break;

                }

            }      
        }
        else
        {
            JGW_FormatWString(strComPort,L"COM%d",nPort);
            while (jt.elapsed() < mnTimeOut)
            {
                LogI_F(L"Start Open Persaso ComPort ...\n");
                if (CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->OpenDonglesComPort(strComPort)) return true;
                Sleep(200);
            }
        }

        return false;
    }
}