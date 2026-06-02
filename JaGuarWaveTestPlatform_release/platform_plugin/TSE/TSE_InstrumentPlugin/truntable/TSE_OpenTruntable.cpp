#include "StdAfx.h"
#include "TSE_OpenTruntable.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#define PER_ANGLE_TURN 10000
#define TURNTABLE_NODE_ID 32
namespace JGW
{
    CTSE_OpenTruntable::CTSE_OpenTruntable(void) : mstrUsbServiceName(L"FTSER2K"),mstrUsbVID(L"VID_0403")
    {
    }


    CTSE_OpenTruntable::~CTSE_OpenTruntable(void)
    {
    }

    const wchar_t* CTSE_OpenTruntable::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_OpenTruntable\",\
                \"TestName\": \"打开转台\",\
                \"UsbServiceName\": \"Usb Service Name\",\
                \"UsbVID\": \"Usb VID\",\
                \"UsbPid\": \"Usb Pid\",\
                \"TruntableEnvironment\": \"Truntable environment\",\
                \"TimeOutSec\": \"超时时间秒\"\
                }";
    }

    bool CTSE_OpenTruntable::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("UsbServiceName"),strParamName))
        {
            mstrUsbServiceName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("UsbVID"),strParamName))
        {
            mstrUsbVID = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("UsbPid"),strParamName))
        {
            mstrUsbPid = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("TruntableEnvironment"),strParamName))
        {
            mstrTruntableEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("TimeOutSec"),strParamName))
        {
            mnTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    //! FTSER2K FTDIBUS\COMPORT&VID_0403&PID_6001
    int CTSE_OpenTruntable::FindTruntableComPort()
    {
        std::wstring strComPort;
        strComPort = JGW_GetComPortToUsbServiceName(mstrUsbServiceName.c_str(),mstrUsbVID.c_str());
        if (strComPort.empty()) return 0;
        JGW_ReplaceStringW(strComPort,L"COM",L"");
        return _ttoi(strComPort.c_str());
    }

    bool CTSE_OpenTruntable::OpenTrunTable(int nTruntaleComPort)
    {
        if (!mSCLLibHelper.IsLoadedSCLLibHelperLibrary()) mSCLLibHelper.LoadSCLLibHelperLibrary();
        if (!mSCLLibHelper.IsLoadedSCLLibHelperLibrary())
        {
            LogE(L"Fail to load SCLLib_x86.dll");
            return false;
        }

        std::wstring strCom;
        JGW_FormatWString(strCom,L"\\\\.\\COM%d",nTruntaleComPort);
        HANDLE hCom = CreateFile(strCom.c_str(),0,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
        if (INVALID_HANDLE_VALUE == hCom)
        {
            LogE(L"Failed to open the turntable device port.");
            LogE(L"Please confirm whether the turntable device has been occupied or has been activated");
            return false;
        }
        CloseHandle(hCom);

        if (!mSCLLibHelper.Open(nTruntaleComPort,9600))
        {
            LogE(L"Failed to open the turntable device port.");
            LogE(L"Please confirm whether the turntable device has been occupied or has been activated");
            return false;
        }
        /* ret = */mSCLLibHelper.SetCommParam(0, FALSE);

        return mSCLLibHelper.WriteMotorEnable(TURNTABLE_NODE_ID)?true:false;
    }

    bool CTSE_OpenTruntable::TSE_Run()
    {
        if (mSCLLibHelper.IsOpen())
        {
            return true;
        }
        GetGlobalEnvironment()->PutInt(mstrTruntableEnvironment,int(&mSCLLibHelper));
        jgw_timer jt;
        while (jt.elapsed() < mnTimeOutSec)
        {
            LogI(L"Find Truntable Com Port ...");
            int nTruntaleComPort = FindTruntableComPort();
            if (nTruntaleComPort > 0)
            {
                if (OpenTrunTable(nTruntaleComPort))
                {
                    return true;
                }
            }
            Sleep(1 * 1000);
        }
        return false;
    }
}
