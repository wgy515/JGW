#include "StdAfx.h"
#include "CJGW_DeviceQueueManage.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_ReportDeviceInfo/CJGW_ReportDeviceInfoExport.h>
#include "CJGW_DeviceQueueSortConfigDlg.h"

#define NETCARD_SORT_INDEX_OFFSET 0x400
namespace JGW
{
    CCJGW_DeviceQueueManage::CCJGW_DeviceQueueManage(void) : mhMianWnd(NULL),mbInitNetCardDevice(false),mpDeviceSortConfigImpl(NULL)
    {
    }


    CCJGW_DeviceQueueManage::~CCJGW_DeviceQueueManage(void)
    {
    }

    bool CCJGW_DeviceQueueManage::Initialization(HWND hMainWnd,const std::wstring& strModuleFolder,const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo)
    {
        std::wstring strDeviceQueueSortConfig = JGW_GetConfigConfigFilePath(strModuleFolder,L"DeviceQueueSortConfig.Data");
        mmapDeviceQueueMsgEvent.clear();
        if (!mcDeviceQueueSortConfig.LoadDeviceQueueSortConfig(strDeviceQueueSortConfig,msDeviceQueueProgramInfo))
        {
            return false;
        }
        ChangeDeviceQueueProgramInfo(sDeviceQueueProgramInfo);

        return true;
    }

    void CCJGW_DeviceQueueManage::ChangeDeviceQueueProgramInfo(const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo)
    {
        if (sDeviceQueueProgramInfo.mstrNetCardHostIPSeg != msDeviceQueueProgramInfo.mstrNetCardHostIPSeg && (
            sDeviceQueueProgramInfo.meFristDeviceMode == E_LAN_MODE || sDeviceQueueProgramInfo.meSecondDeviceMode == E_LAN_MODE || sDeviceQueueProgramInfo.meThreeDeviceMode == E_LAN_MODE))
        {
            StopNetCardDevice();
            StartNetCardDevice();
        }
        if (sDeviceQueueProgramInfo.meDeviceQueueProgram == msDeviceQueueProgramInfo.meDeviceQueueProgram
            && sDeviceQueueProgramInfo.meFristDeviceMode == msDeviceQueueProgramInfo.meFristDeviceMode
            && sDeviceQueueProgramInfo.meSecondDeviceMode == msDeviceQueueProgramInfo.meSecondDeviceMode
            && sDeviceQueueProgramInfo.meThreeDeviceMode == msDeviceQueueProgramInfo.meThreeDeviceMode)
        {
            return;
        }
        msDeviceQueueProgramInfo.meDeviceQueueProgram = sDeviceQueueProgramInfo.meDeviceQueueProgram;
        msDeviceQueueProgramInfo.meFristDeviceMode = sDeviceQueueProgramInfo.meFristDeviceMode;
        msDeviceQueueProgramInfo.meSecondDeviceMode = sDeviceQueueProgramInfo.meSecondDeviceMode;
        msDeviceQueueProgramInfo.meThreeDeviceMode = sDeviceQueueProgramInfo.meThreeDeviceMode;
        msDeviceQueueProgramInfo.mstrNetCardHostIPSeg = sDeviceQueueProgramInfo.mstrNetCardHostIPSeg;
        mcDeviceQueueSortConfig.ClearDeviceQueueSortConfig();
        ClearNetCardSortConfig();
        mcDeviceQueueSortConfig.SaveDeviceQueueSortConfig(msDeviceQueueProgramInfo);
    }

    void CCJGW_DeviceQueueManage::ClearDeviceQueueSortConfig()
    {
        mcDeviceQueueSortConfig.ClearDeviceQueueSortConfig();
        ClearNetCardSortConfig();
    }

    void CCJGW_DeviceQueueManage::AddListenerDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg,LPVOID pContext)
    {
        mmapDeviceQueueMsgEvent[pOnDeviceQueueMsg] = pContext;
    }

    void CCJGW_DeviceQueueManage::RemoveListenerDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg)
    {
        std::map<OnDeviceQueueMsg,LPVOID>::iterator it = mmapDeviceQueueMsgEvent.find(pOnDeviceQueueMsg);
        if (it != mmapDeviceQueueMsgEvent.end())
        {
            mmapDeviceQueueMsgEvent.erase(it);
        }
    }

    E_DEVICE_MODE CCJGW_DeviceQueueManage::GetDeviceModeToHandleDeviceMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,std::wstring& strDevice,bool& bIsPlugInDevice,int& nSortIndex,std::wstring& strDes)
    {
        E_DEVICE_MODE eDeviceMode = E_DEVICE_UNKNOWMODE;
        if (eMsgType == E_COM_MSG_TYPE)
        {
            SDeviceMsg_Info* lpdevicemsg = (SDeviceMsg_Info*)(wParam);
            eDeviceMode = (E_DEVICE_MODE)lpdevicemsg->m_nDeviceMode;
            bIsPlugInDevice = (lpdevicemsg->m_nDeviceMsgType == DBT_DEVICEARRIVAL);
            nSortIndex = lpdevicemsg->m_nSortIndex;
            JGW_FormatWString(strDevice,L"%d",lpdevicemsg->m_nComPort);
            JGW_FormatWString(strDes,L"COM%d",lpdevicemsg->m_nComPort);
            if (eDeviceMode == E_ADB_MODE || eDeviceMode == E_FASTBOOT_MODE)
            {
                strDevice = lpdevicemsg->m_strSeralNumber;
                strDes = strDevice;
            }      
            lpdevicemsg->Dec();
            return eDeviceMode;
        }

        PS_NETCARD_INFO psNetCardInfo = reinterpret_cast<PS_NETCARD_INFO>(wParam);
        bIsPlugInDevice = (E_NETCARD_PHYSICAL_CONNECT == psNetCardInfo->meNetCardPhysicalState);
        nSortIndex = psNetCardInfo->mnSortIndex + NETCARD_SORT_INDEX_OFFSET;
        JGW_FormatWString(strDes,L"%s",psNetCardInfo->mstrDescription);
        JGW_FormatWString(strDevice,L"{\"IPAddr\":\"%s\",\"NetGUID\":\"%s\",\"NetGUID\":\"%s\",\"NetDes\":\"%s\"}",psNetCardInfo->mstrIPAddr.c_str(),psNetCardInfo->mstrNetGUID.c_str(),psNetCardInfo->mstrDescription.c_str());
        return E_LAN_MODE;
    }

    void CCJGW_DeviceQueueManage::OnHandleDeviceMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,const LPARAM& lParam)
    {
        if (NULL == wParam) return;
        if (NULL != mpDeviceSortConfigImpl) { mpDeviceSortConfigImpl->OnHandleDeviceMsg(eMsgType,wParam,lParam); return;}
#if 0
        int nViewIndex = 0;
        if (eMsgType == E_COM_MSG_TYPE)
        {
            SDeviceMsg_Info* lpdevicemsg = reinterpret_cast<SDeviceMsg_Info>(wParam);

        }
        else if (eMsgType == E_NETCARD_MSG_TYPE)
        {
            std::wstring strTemp;
            PS_NETCARD_INFO psNetCardInfo = reinterpret_cast<PS_NETCARD_INFO>(wParam);

            JGW_FormatWString(strTemp,L"{\"IPAddr\":\"%s\",\"NetGUID\":\"%s\",\"NetGUID\":\"%s\",\"NetDes\":\"%s\"}",psNetCardInfo->mstrIPAddr.c_str(),psNetCardInfo->mstrNetGUID.c_str(),psNetCardInfo->mstrDescription.c_str());
            msDeviceQueueInfo.mbIsPlugInDevice = (E_NETCARD_PHYSICAL_CONNECT == psNetCardInfo->meNetCardPhysicalState);
            if (msDeviceQueueProgramInfo.meFristDeviceMode == E_LAN_MODE)
            {
                msDeviceQueueInfo.mstrFristDevice = strTemp;
                msDeviceQueueInfo.mnViewIndex = mcDeviceQueueSortConfig.GetFritDeviceSortIndex(psNetCardInfo->mnSortIndex,strTemp);
                OnDeviceQueueMsgEvent(&msDeviceQueueInfo);
            }
            if (msDeviceQueueProgramInfo.meSecondDeviceMode == E_LAN_MODE)
            {
                nViewIndex = mcDeviceQueueSortConfig.GetSencondDeviceSortIndex(psNetCardInfo->mnSortIndex,strTemp);
            }
            if (msDeviceQueueProgramInfo.meThreeDeviceMode == E_LAN_MODE)
            {
                nViewIndex = mcDeviceQueueSortConfig.GetThreeDeviceSortIndex(psNetCardInfo->mnSortIndex,strTemp);
            }
        }
#else
        bool bIsPlugInDevice = false;;
        int nSortIndex = -1;
        std::wstring strDevice,strDes;
        S_DEVICESORT_CONFIG sDeviceSortConfig;
        E_DEVICE_MODE eDeviceMode = GetDeviceModeToHandleDeviceMsg(eMsgType,wParam,strDevice,bIsPlugInDevice,nSortIndex,strDes);
        if (eDeviceMode == msDeviceQueueProgramInfo.meFristDeviceMode)
        {
            msDeviceQueueInfo.mnViewIndex = mcDeviceQueueSortConfig.GetFritDeviceViewIndexToSortIndex(nSortIndex,strDevice);
            if (!mcDeviceQueueSortConfig.GetDeviceSortConfigToFristSortIndex(msDeviceQueueInfo.mnViewIndex,sDeviceSortConfig)) return;
            msDeviceQueueInfo.mbIsPlugInDevice = bIsPlugInDevice;
            msDeviceQueueInfo.mstrFristDevice = strDevice;
            msDeviceQueueInfo.mstrDes = strDes;
            msDeviceQueueInfo.mstrSecondDevice = sDeviceSortConfig.mstrSecondDevice;
            msDeviceQueueInfo.mstrThreeDevice = sDeviceSortConfig.mstrThreeDevice;
            OnDeviceQueueMsgEvent(&msDeviceQueueInfo);
        }
        else if (eDeviceMode == msDeviceQueueProgramInfo.meSecondDeviceMode)
        {
            mcDeviceQueueSortConfig.GetThreeDeviceViewIndexToSortIndex(nSortIndex,strDevice);
        }
        else if (eDeviceMode == msDeviceQueueProgramInfo.meThreeDeviceMode)
        {
            mcDeviceQueueSortConfig.GetThreeDeviceViewIndexToSortIndex(nSortIndex,strDevice);
        }
#endif
    }

    void CCJGW_DeviceQueueManage::UnInitialization()
    {
        StopNetCardDevice();
        mmapDeviceQueueMsgEvent.clear();
    }

    void CCJGW_DeviceQueueManage::StartNetCardDevice()
    {
        if (!mbInitNetCardDevice)
        {
            StartNetCardDeviceThread(500);
            RegNetCardDeviceMsg(mhMianWnd,msDeviceQueueProgramInfo.mstrNetCardHostIPSeg.c_str());
            mbInitNetCardDevice = true;
        }
    }

    void CCJGW_DeviceQueueManage::StopNetCardDevice()
    {
        if (mbInitNetCardDevice)
        {
            RemoveNetCardDeviceMsg(mhMianWnd);
            CloseNetCardDeviceThread();
        }
    }

    void CCJGW_DeviceQueueManage::OnDeviceQueueMsgEvent(LPS_DEVICE_QUEUE_INFO psDeviceQueueInfo)
    {
        for (std::map<OnDeviceQueueMsg,LPVOID>::iterator it = mmapDeviceQueueMsgEvent.begin();
            it != mmapDeviceQueueMsgEvent.end();
            ++ it)
        {
            it->first(psDeviceQueueInfo,it->second);
        }
    }

    bool CCJGW_DeviceQueueManage::OnResponseClickConfigMsg(HWND hWindow,const S_DEVICEQUEUE_PROGRAM_INFO& sDevoceQueueProgramInfo,std::wstring& strTFTPD32IPAddr,std::wstring& strDownloadFileConfig)
    {
        CCJGW_DeviceQueueSortConfigDlg cDeviceQueueSortConfigDlg(hWindow,sDevoceQueueProgramInfo,strTFTPD32IPAddr,strDownloadFileConfig,mcDeviceQueueSortConfig);
        mpDeviceSortConfigImpl = &cDeviceQueueSortConfigDlg;
        int nModal = cDeviceQueueSortConfigDlg.DoModal();
        mpDeviceSortConfigImpl = NULL;
        return nModal == IDOK;
    }
}
