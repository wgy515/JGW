#include "StdAfx.h"
#include "TSE_ConnectDMGPeraso.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>

namespace JGW
{
    CTSE_ConnectDMGPeraso::CTSE_ConnectDMGPeraso(void)
    {
    }


    CTSE_ConnectDMGPeraso::~CTSE_ConnectDMGPeraso(void)
    {
    }

    const wchar_t* CTSE_ConnectDMGPeraso::TSE_GetParamDescription()
    {    
        return L"{\
                \"RealName\": \"CTSE_ConnectDMGPeraso\",\
                \"TestName\": \"Connect DMG Peraso\",\
                \"UsbServiceName\": \"USB设备服务名\",\
                \"UsbVid\": \"USB设备Vid\",\
                \"UsbPid\": \"USB设备Pid\",\
                \"GoldenSNEnv\":\"金机SN环境变量\",\
                \"GuSerialComPortEnv\":\"金机SN环境变量\",\
                \"DutSerialComPortEnv\":\"Gu Serial ComPort保存到某个环境变量\",\
                \"DutSerialNumberEnv\":\"DUT Serial ComPort保存到某个环境变量\",\
                \"TimeOut\": \"查找或者连接超时时间\"\
                }";
    }

    bool CTSE_ConnectDMGPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"GoldenSNEnv"))
        {
            mstrGoldenSNEnv = strParamValue;
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"GuSerialComPortEnv"))
        {
            mstrGuSerialComPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DutSerialComPortEnv"))
        {
            mstrDutSerialComPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DutSerialNumberEnv"))
        {
            mstrDutSerialNumberEnv = strParamValue;
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

    bool CTSE_ConnectDMGPeraso::TSE_Run()
    {
        jgw_timer jt;
        std::wstring strGoldenSerialNumber = GetGlobalEnvironment()->GetString(mstrGoldenSNEnv),strComPort,strSerialNumber,strDutSerialNumber;
        std::vector<std::wstring> vstrComPort;
        CCJGW_DonglesComPort guDonglesComPort(mGuSerialComPort),dutDonglesComPort(mDutSerialComPort);

        while (jt.elapsed() < mnTimeOut)
        {
            mGuSerialComPort.CloseSerialComPort();
            mDutSerialComPort.CloseSerialComPort();
            JGW_GetAllComPortToUsbServiceName(vstrComPort,mstrUsbServiceName.c_str(),mstrUsbVid.c_str(),mstrUsbPid.empty()?L"":mstrUsbPid.c_str());
            if (2 != vstrComPort.size())
            {
                Sleep(1000); 
                LogI_F(L"Find Peraso Device Count %d",vstrComPort.size());
                continue;
            }

            LogD(L"Start Open Persaso ComPort ...\n");
            if (!guDonglesComPort.OpenDonglesComPort(vstrComPort[0]) || !guDonglesComPort.ReadSerialNumber(strSerialNumber))
            { 
                Sleep(200);
                continue;
            }
            LogI_F(L"读取设备SN为: %s",strSerialNumber.c_str());
            //! 
            if (!dutDonglesComPort.OpenDonglesComPort(vstrComPort[1]) || !dutDonglesComPort.ReadSerialNumber(strDutSerialNumber))
            {
                Sleep(200);
                continue;
            }
            LogI_F(L"读取设备SN为: %s",strDutSerialNumber.c_str());
            if (!TSE_PARAM_NAME_EQUAL(strSerialNumber.c_str(),strGoldenSerialNumber.c_str()) && !TSE_PARAM_NAME_EQUAL(strDutSerialNumber.c_str(),strGoldenSerialNumber.c_str()))
            {
                LogI_F(L"未查找到金机SN设备,默认金机设备SN为: %s",strGoldenSerialNumber.c_str());
                Sleep(1000); 
                continue;
            }
            //! true 是金机
            if (TSE_PARAM_NAME_EQUAL(strSerialNumber.c_str(),strGoldenSerialNumber.c_str()))
            {
                GetGlobalEnvironment()->PutInt(mstrGuSerialComPortEnv,int(&mGuSerialComPort));
                GetGlobalEnvironment()->PutInt(mstrDutSerialComPortEnv,int(&mDutSerialComPort));
                GetGlobalEnvironment()->PutString(mstrDutSerialNumberEnv,strDutSerialNumber);
            }
            else
            {
                GetGlobalEnvironment()->PutInt(mstrGuSerialComPortEnv,int(&mDutSerialComPort));
                GetGlobalEnvironment()->PutInt(mstrDutSerialComPortEnv,int(&mGuSerialComPort));
                GetGlobalEnvironment()->PutString(mstrDutSerialNumberEnv,strSerialNumber);
            }

            return true;
        }

        //! true 是金机
        if (TSE_PARAM_NAME_EQUAL(strSerialNumber.c_str(),strGoldenSerialNumber.c_str()))
        {
            GetGlobalEnvironment()->PutInt(mstrGuSerialComPortEnv,int(&mGuSerialComPort));
            GetGlobalEnvironment()->PutInt(mstrDutSerialComPortEnv,int(&mDutSerialComPort));
            GetGlobalEnvironment()->PutString(mstrDutSerialNumberEnv,strDutSerialNumber);
        }
        else
        {
            GetGlobalEnvironment()->PutInt(mstrGuSerialComPortEnv,int(&mDutSerialComPort));
            GetGlobalEnvironment()->PutInt(mstrDutSerialComPortEnv,int(&mGuSerialComPort));
            GetGlobalEnvironment()->PutString(mstrDutSerialNumberEnv,strSerialNumber);
        }

        return false;
    }
}

