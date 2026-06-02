#include "StdAfx.h"
#include "CJGW_DeviceQueueSortConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_DeviceQueueSortConfig::CCJGW_DeviceQueueSortConfig(void)
    {
    }


    CCJGW_DeviceQueueSortConfig::~CCJGW_DeviceQueueSortConfig(void)
    {
    }

    bool CCJGW_DeviceQueueSortConfig::LoadDeviceQueueSortConfig(const std::wstring& strDeviceQueueSortConfig,S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo)
    {
        int nDevCount = 0;
        std::wstring strAppName;

        mcConfigIni.InitIniFilePath(strDeviceQueueSortConfig.c_str());
        nDevCount = mcConfigIni.GetIniKeyIntValue(L"MAIN",L"DevCounts");
        sDeviceQueueProgramInfo.meDeviceQueueProgram = (E_DEVICE_QUEUE_PROGRAM)mcConfigIni.GetIniKeyIntValue(L"MAIN",L"DeviceQueueProgram");
        sDeviceQueueProgramInfo.meFristDeviceMode = (E_DEVICE_MODE)mcConfigIni.GetIniKeyIntValue(L"MAIN",L"FristDeviceMode");
        sDeviceQueueProgramInfo.meSecondDeviceMode = (E_DEVICE_MODE)mcConfigIni.GetIniKeyIntValue(L"MAIN",L"SecondDeviceMode");
        sDeviceQueueProgramInfo.meThreeDeviceMode = (E_DEVICE_MODE)mcConfigIni.GetIniKeyIntValue(L"MAIN",L"ThreeDeviceMode");
        meDeviceQueueProgram = sDeviceQueueProgramInfo.meDeviceQueueProgram;

        for (int index = 0;index < nDevCount;index++)
        {
            S_DEVICESORT_CONFIG sDeviceSortConfig = {-1,-1,-1,L"",L"",L""};

            JGW_FormatWString(strAppName,L"DEV_%d",index + 1);
            sDeviceSortConfig.mnFristDeviceSortIndex = mcConfigIni.GetIniKeyIntValue(strAppName.c_str(),L"FristDeviceSortIndex");
            sDeviceSortConfig.mnSecondDeviceSortIndex = mcConfigIni.GetIniKeyIntValue(strAppName.c_str(),L"SecondDeviceSortIndex");
            sDeviceSortConfig.mnThreeDeviceSortIndex = mcConfigIni.GetIniKeyIntValue(strAppName.c_str(),L"ThreeDeviceSortIndex");
            sDeviceSortConfig.mstrFristDevice = mcConfigIni.GetIniKeyIntValue(strAppName.c_str(),L"FristDevice");
            sDeviceSortConfig.mstrSecondDevice = mcConfigIni.GetIniKeyIntValue(strAppName.c_str(),L"SecondDevice");
            sDeviceSortConfig.mstrThreeDevice = mcConfigIni.GetIniKeyIntValue(strAppName.c_str(),L"ThreeDevice");
            mvsDeviceSortConfigs.push_back(sDeviceSortConfig);
        }

        return true;
    }

    void CCJGW_DeviceQueueSortConfig::SaveDeviceQueueSortConfig(S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo)
    {
        meDeviceQueueProgram = sDeviceQueueProgramInfo.meDeviceQueueProgram;
        mcConfigIni.SetIniKeyValue(L"MAIN",L"DeviceQueueProgram",(int)sDeviceQueueProgramInfo.meDeviceQueueProgram);
        mcConfigIni.SetIniKeyValue(L"MAIN",L"FristDeviceMode",(int)sDeviceQueueProgramInfo.meFristDeviceMode);
        mcConfigIni.SetIniKeyValue(L"MAIN",L"SecondDeviceMode",(int)sDeviceQueueProgramInfo.meSecondDeviceMode);
        mcConfigIni.SetIniKeyValue(L"MAIN",L"ThreeDeviceMode",(int)sDeviceQueueProgramInfo.meThreeDeviceMode);
    }

    void CCJGW_DeviceQueueSortConfig::ClearDeviceQueueSortConfig()
    {
        mvsDeviceSortConfigs.clear();
        mcConfigIni.SetIniKeyValue(L"MAIN",L"DevCounts",0);
    }

    int CCJGW_DeviceQueueSortConfig::GetFritDeviceViewIndexToSortIndex(int nSortIndex,const std::wstring& strDevice)
    {
        int nUnsorted = -1;
        for (size_t index = 0;index < mvsDeviceSortConfigs.size();index++)
        {
            //！如果为-1则表示未进行过排序
            if (mvsDeviceSortConfigs[index].mnFristDeviceSortIndex == -1)
            {
                if (-1 == nUnsorted) nUnsorted = index;
                continue;
            }
            //! 如果已排序则直接返回
            if (mvsDeviceSortConfigs[index].mnFristDeviceSortIndex == nSortIndex)
            {
                mvsDeviceSortConfigs[index].mstrFristDevice = strDevice;
                mcConfigIni.SetIniKeyValue(JGW_GetFormatWString(L"DEV_%d",index + 1).c_str(),L"FristDevice",strDevice.c_str());
                return index;
            }
        }
        //! 如果是单端口排序则直接进行排序
        if (meDeviceQueueProgram == E_SINGLE_PORT_QUEUE_PROGRAM || E_NETCARD_QUEUE_PROGRAM == meDeviceQueueProgram)
        {  
            if (-1 != nUnsorted)
            {
                mvsDeviceSortConfigs[nUnsorted].mnFristDeviceSortIndex = nSortIndex;
                mvsDeviceSortConfigs[nUnsorted].mstrFristDevice = strDevice;
            }
            else
            { 
                S_DEVICESORT_CONFIG sDeviceSortConfig = {-1,-1,-1,L"",L"",L""};
                sDeviceSortConfig.mnFristDeviceSortIndex = nSortIndex;
                sDeviceSortConfig.mstrFristDevice = strDevice;
                nUnsorted = mvsDeviceSortConfigs.size();
                mvsDeviceSortConfigs.push_back(sDeviceSortConfig); 
            }
            std::wstring strAppName;
            JGW_FormatWString(strAppName,L"DEV_%d",nUnsorted + 1);
            mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"FristDeviceSortIndex",nSortIndex);
            mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"FristDevice",strDevice.c_str());
            return nUnsorted;
        }

        return -1;
    }

    bool CCJGW_DeviceQueueSortConfig::GetDeviceSortConfigToFristSortIndex(int nViewIndex,S_DEVICESORT_CONFIG& sDeviceSortConfig)
    {
        if (-1 == nViewIndex || nViewIndex >= (int)mvsDeviceSortConfigs.size()) return false;

        sDeviceSortConfig.mnFristDeviceSortIndex = mvsDeviceSortConfigs[nViewIndex].mnFristDeviceSortIndex;
        sDeviceSortConfig.mstrFristDevice = mvsDeviceSortConfigs[nViewIndex].mstrFristDevice;
        sDeviceSortConfig.mnSecondDeviceSortIndex = mvsDeviceSortConfigs[nViewIndex].mnSecondDeviceSortIndex;
        sDeviceSortConfig.mstrSecondDevice = mvsDeviceSortConfigs[nViewIndex].mstrSecondDevice;
        sDeviceSortConfig.mnThreeDeviceSortIndex = mvsDeviceSortConfigs[nViewIndex].mnThreeDeviceSortIndex;
        sDeviceSortConfig.mstrThreeDevice = mvsDeviceSortConfigs[nViewIndex].mstrThreeDevice;

        return true;
    }

    int CCJGW_DeviceQueueSortConfig::GetSencondDeviceViewIndexToSortIndex(int nSortIndex,const std::wstring& strDevice)
    {
        // 单端口排序
        if (meDeviceQueueProgram == E_SINGLE_PORT_QUEUE_PROGRAM || E_NETCARD_QUEUE_PROGRAM == meDeviceQueueProgram) return -1;

        for (size_t index = 0;index < mvsDeviceSortConfigs.size();index++)
        {
            if (mvsDeviceSortConfigs[index].mnSecondDeviceSortIndex == nSortIndex)
            {
                mvsDeviceSortConfigs[index].mstrSecondDevice = strDevice;
                mcConfigIni.SetIniKeyValue(JGW_GetFormatWString(L"DEV_%d",index + 1).c_str(),L"SecondDevice",strDevice.c_str());
                return index;
            }
        }
        return -1;
    }

    int CCJGW_DeviceQueueSortConfig::GetThreeDeviceViewIndexToSortIndex(int nSortIndex,const std::wstring& strDevice)
    {
        if (meDeviceQueueProgram == E_SINGLE_PORT_QUEUE_PROGRAM || E_NETCARD_QUEUE_PROGRAM == meDeviceQueueProgram) return -1;

        for (size_t index = 0;index < mvsDeviceSortConfigs.size();index++)
        {
            if (mvsDeviceSortConfigs[index].mnThreeDeviceSortIndex == nSortIndex)
            {
                mvsDeviceSortConfigs[index].mstrThreeDevice = strDevice;
                mcConfigIni.SetIniKeyValue(JGW_GetFormatWString(L"DEV_%d",index + 1).c_str(),L"ThreeDevice",strDevice.c_str());
                return index;
            }
        }
        return -1;
    }

    void CCJGW_DeviceQueueSortConfig::UpdateDeviceQueueSortConfig(int nViewIndex)
    {
        S_DEVICESORT_CONFIG& sDeviceSortConfig = GetDeviceSortConfigToViewIndex(nViewIndex);
        std::wstring strAppName;
        JGW_FormatWString(strAppName,L"DEV_%d",nViewIndex + 1);

        mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"FristDeviceSortIndex",sDeviceSortConfig.mnFristDeviceSortIndex);
        mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"FristDevice",sDeviceSortConfig.mstrFristDevice.c_str());

        if (meDeviceQueueProgram == E_DUAL_PORT_QUEUE_PROGRAM || E_DUAL_PORT_NETCARD_QUEUE_PROGRAM  == meDeviceQueueProgram)
        {
            mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"SecondDeviceSortIndex",sDeviceSortConfig.mnSecondDeviceSortIndex);
            mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"SecondDevice",sDeviceSortConfig.mstrSecondDevice.c_str());
        }
        if (E_DUAL_PORT_NETCARD_QUEUE_PROGRAM  == meDeviceQueueProgram)
        {
            mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"ThreeDeviceSortIndex",sDeviceSortConfig.mnThreeDeviceSortIndex);
            mcConfigIni.SetIniKeyValue(strAppName.c_str(),L"ThreeDevice",sDeviceSortConfig.mstrThreeDevice.c_str());
        }
    }

    void CCJGW_DeviceQueueSortConfig::AddDeviceQueueSortConfig(const S_DEVICESORT_CONFIG& sDeviceSortConfig)
    {
        mvsDeviceSortConfigs.push_back(sDeviceSortConfig);
        mcConfigIni.SetIniKeyValue(L"MAIN",L"DevCounts",mvsDeviceSortConfigs.size() + 1);
        UpdateDeviceQueueSortConfig((int)mvsDeviceSortConfigs.size() - 1);
    }

    std::vector<S_DEVICESORT_CONFIG>& CCJGW_DeviceQueueSortConfig::GetDeviceSortConfigInfo()
    {
        return mvsDeviceSortConfigs;
    }

    S_DEVICESORT_CONFIG& CCJGW_DeviceQueueSortConfig::GetDeviceSortConfigToViewIndex(int nViewIndex)
    {
        S_DEVICESORT_CONFIG sDeviceSortConfig = {-1,-1,-1,L"",L"",L""};
        while (nViewIndex >= (int)mvsDeviceSortConfigs.size()) AddDeviceQueueSortConfig(sDeviceSortConfig);
        return mvsDeviceSortConfigs[nViewIndex];
    }
}
