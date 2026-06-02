#include "StdAfx.h"
#include <tchar.h>
#include "CJGW_UsbFilterPortConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#define MAX_USB_SORT_INFO_COUNTS 200
namespace JGW
{
    CCJGW_UsbFilterPortConfig::CCJGW_UsbFilterPortConfig(void)
    {
    }


    CCJGW_UsbFilterPortConfig::~CCJGW_UsbFilterPortConfig(void)
    {
    }

    bool CCJGW_UsbFilterPortConfig::LoadUsbFilterPortConfig(const wchar_t* strUsbFilterPortConfigPath)
    {
        mMaxDevCounts = 0;
        mvsUsbSortInfos.clear();
        mvFilterUsbPidVid.clear();

        if (!mConfigIni.InitIniFilePath(strUsbFilterPortConfigPath)) return false;
        mMaxDevCounts = mConfigIni.GetIniKeyIntValue(L"MAIN",L"DevCounts");
        if (mMaxDevCounts > MAX_USB_SORT_INFO_COUNTS) mMaxDevCounts = MAX_USB_SORT_INFO_COUNTS;

        S_USB_SORT_INFO sUsbSortInfo;
        wchar_t szBuf[10] = {0};
        for (size_t i = 1;i <= mMaxDevCounts;i ++)
        {
            _stprintf_s(szBuf,10,L"DEV_%d",i);
            sUsbSortInfo.mnPort = mConfigIni.GetIniKeyIntValue(szBuf,L"Port");
            sUsbSortInfo.mstrPhysicalAddress = mConfigIni.GetIniKeyValue(szBuf,L"PhysicalAddress");
            mvsUsbSortInfos.push_back(sUsbSortInfo);
        }
        LoadUsbFilterPidVidConfig();
        return true;
    }

    size_t CCJGW_UsbFilterPortConfig::GetUsbSortIndexToComPort(size_t nComport)
    {
        for (size_t i = 0;i < mMaxDevCounts;i ++)
        {
            if (nComport == mvsUsbSortInfos[i].mnPort) return i + 1;
        }

        S_USB_SORT_INFO sUsbSortInfo;
        sUsbSortInfo.mnPort = nComport;
        mvsUsbSortInfos.push_back(sUsbSortInfo);

        wchar_t szBuf[10] = {0};
        _stprintf_s(szBuf,10,L"DEV_%d",mvsUsbSortInfos.size());
        mConfigIni.SetIniKeyValue(szBuf,L"Port",nComport);
        
        return mvsUsbSortInfos.size();
    }

    size_t CCJGW_UsbFilterPortConfig::GetUsbSortIndexToPhysicalAddress(const wchar_t* strPhysicalAddress)
    {
        for (size_t i = 0;i < mMaxDevCounts;i ++)
        {
            if (NULL == mvsUsbSortInfos[i].mstrPhysicalAddress.compare(strPhysicalAddress)) return i + 1;
        }

        S_USB_SORT_INFO sUsbSortInfo;
        sUsbSortInfo.mstrPhysicalAddress = strPhysicalAddress;
        mvsUsbSortInfos.push_back(sUsbSortInfo);

        wchar_t szBuf[10] = {0};
        _stprintf_s(szBuf,10,L"DEV_%d",mvsUsbSortInfos.size());
        mConfigIni.SetIniKeyValue(szBuf,L"PhysicalAddress",strPhysicalAddress);
        mMaxDevCounts ++;
        mConfigIni.SetIniKeyValue(L"MAIN",L"DevCounts",mMaxDevCounts);
        //! mMaxDevCounts = mConfigIni.GetIniKeyIntValue(L"MAIN",L"DevCounts");

        return mvsUsbSortInfos.size();
    }

    void CCJGW_UsbFilterPortConfig::ClearUsbSortIndex()
    {
        mMaxDevCounts = 0;
        mvsUsbSortInfos.clear();
        mConfigIni.SetIniKeyValue(L"MAIN",L"DevCounts",0);
    }

    void CCJGW_UsbFilterPortConfig::LoadUsbFilterPidVidConfig()
    {
        std::wstring strFilteUsbInfo = mConfigIni.GetIniKeyValue(L"MAIN",L"FilterPidVid");
        if (strFilteUsbInfo.empty()) return;

        std::vector <std::wstring> vStrTemp;
        std::wstring strTemp;
        JGW_ParserStrW(strFilteUsbInfo.c_str(),L",",vStrTemp);
        long pid = 0,vid = 0;

        for (std::vector <std::wstring>::iterator it = vStrTemp.begin();
            it != vStrTemp.end();
            ++ it)
        {
            //!VID_05C6&PID_901D
            _stscanf_s(it[0].c_str(),L"%X-%X",&vid,&pid);
            JGW_UpdateRegUsbFlags(pid,vid);
            JGW_FormatWString(strTemp,L"VID_%04X&PID_%04X",vid,pid);
            mvFilterUsbPidVid.push_back(strTemp);
        }
    }

    bool CCJGW_UsbFilterPortConfig::IsFilterUsbPidVid(const wchar_t* strUsbInstance)
    {
        if (mvFilterUsbPidVid.empty()) return false;

        for (std::vector <std::wstring>::iterator it = mvFilterUsbPidVid.begin();
            it != mvFilterUsbPidVid.end();
            ++ it)
        {
            if (NULL != JGW_WStrCaseWStr(strUsbInstance,it[0].c_str())) return false;
        }

        return true;
    }
}

