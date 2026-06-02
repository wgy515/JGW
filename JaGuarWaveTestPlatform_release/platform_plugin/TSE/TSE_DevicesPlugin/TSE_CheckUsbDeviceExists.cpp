#include "StdAfx.h"
#include "TSE_CheckUsbDeviceExists.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
namespace JGW
{
    CTSE_CheckUsbDeviceExists::CTSE_CheckUsbDeviceExists(void) : mnCheckTimeOut(120)
    {
    }


    CTSE_CheckUsbDeviceExists::~CTSE_CheckUsbDeviceExists(void)
    {
    }

    const wchar_t* CTSE_CheckUsbDeviceExists::TSE_GetParamDescription()
    {
        return L"";;
    }

    bool CTSE_CheckUsbDeviceExists::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"RegKeys"))
        {
            //mstrSavePortEnvrionment = strParamValue;
            JGW_ParserStrW(strParamValue,L",",mvstrRegKeys);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"FindRegValues"))
        {
            //mstrUsbDeviceDes = strParamValue;
            JGW_ParserStrW(strParamValue,L",",mvstrFindRegValues);
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

    bool CTSE_CheckUsbDeviceExists::TSE_Run()
    {
        LogD(L"Check Usb Device Exists ...\n");
        jgw_timer jt;
        while (jt.elapsed() < mnCheckTimeOut)
        {
            size_t nCount = JGW_GetInsertUsbDeviceCountToServicesName(mstrUsbServiceName.c_str());
            for (size_t i = 0;i < nCount;i ++)
            {
                std::wstring strUsbPidVid = JGW_GetUsbEnumPidVidToServiceName(mstrUsbServiceName.c_str(),i);

                if (!mstrUsbVid.empty())
                {
                    if (NULL == JGW_WStrCaseWStr(strUsbPidVid.c_str(),mstrUsbVid.c_str()))
                    {
                        continue;
                    }
                }
                if (!mstrUsbPid.empty())
                {
                    if (NULL == JGW_WStrCaseWStr(strUsbPidVid.c_str(),mstrUsbPid.c_str()))
                    {
                        continue;
                    }
                }
                if (!mvstrFindRegValues.empty() && !mvstrRegKeys.empty())
                {
                    for (size_t index = 0;index < mvstrRegKeys.size();index++)
                    {
                        std::wstring strDeviceRegValue = JGW_GetRegUsbInfoToTag(strUsbPidVid.c_str(),mvstrRegKeys[index].c_str());
                        if (mvstrFindRegValues.size() > index)
                        {
                            if (NULL == JGW_WStrCaseWStr(strDeviceRegValue.c_str(),mvstrFindRegValues[index].c_str()))
                            {
                                continue;
                            }
                        }
                    }
                }
                return true;
            }
            Sleep(1000);
        }
        return false;
    }
}
