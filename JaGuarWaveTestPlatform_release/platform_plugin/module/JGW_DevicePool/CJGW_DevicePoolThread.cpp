#include "StdAfx.h"

#include "usb_host/AutoUsbDefine.h"
#include "usb_config/CJGW_UsbConfig.h"


#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "CJGW_DevicePoolThread.h"

#include "../../../platform_include/JGW_MSG_ID_Define.h"

extern int g_nUserPortSort;

namespace JGW
{
    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_DevicePoolThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(REG_DEVICEPOOL_MSG_DEFINE,&CCJGW_DevicePoolThread::OnRspRegDevicePoolDevInfo)
        ON_MYTHREAD_MESSGAE(ADD_DEVICEPOOL_MSG_DEFINE,&CCJGW_DevicePoolThread::OnRspDeviceStatusChangeMsg)
        ON_MYTHREAD_MESSGAE(REMOVE_DEVICEPOOL_MSG_DEFINE,&CCJGW_DevicePoolThread::OnRemoveRegDevicePoolDevInfo)
        ON_MYTHREAD_MESSGAE(REMOVE_PLUGIN_REG_DEVICEPOLL_DEFINE,&CCJGW_DevicePoolThread::OnRemovePluginRegDevicePoolDevInfo)
        ON_MYTHREAD_MESSGAE(CLEAR_SORT_DEVICEPOOL_MSG_DEFINE,&CCJGW_DevicePoolThread::OnClearSortIndex)
    END_MYTHREAD_MESSAGE_MAP()

    CCJGW_DevicePoolThread::CCJGW_DevicePoolThread(void):mhMainHwnd(NULL)
    {
        //! RUN THRED时间间隔
        SetThreadSleepTimeInterval(DEVICE_MSG_TIME_INRERVAL);
        g_nUserPortSort = GetPrivateProfileInt(_T("MAIN" ),_T( "DETECT_INST_PORT_NO"),0,GetFilterIniConfigPath() );
    }

    CCJGW_DevicePoolThread::~CCJGW_DevicePoolThread(void)
    {
        //! mmapGuidDevPoolInfo.clear();
        CloseDevicePoolThread();
    }

    /************************************************************************/
    /*              JGW_DevicePoolImpl                                      */
    /************************************************************************/
    bool CCJGW_DevicePoolThread::RegDevicePoolMsg(S_REG_DEVICE_POOL_DEVINFO* psRegDevicePoolDevInfo)
    {
        return PostThreadMessage(REG_DEVICEPOOL_MSG_DEFINE,WPARAM(psRegDevicePoolDevInfo),NULL);
    }

    bool CCJGW_DevicePoolThread::RegDevicePoolMsg(UINT nMsgID,HWND hwnd,GUID guid,E_DEVICE_MODE eDeviceModeEnum)
    {
        PS_REG_DEVICE_POOL_DEVINFO psRegDevicePoolDevInfo = new S_REG_DEVICE_POOL_DEVINFO;
        psRegDevicePoolDevInfo->m_nmsgid = nMsgID;
        psRegDevicePoolDevInfo->mhWnd = hwnd;
        psRegDevicePoolDevInfo->mGuid = guid;
        psRegDevicePoolDevInfo->meDeviceMode = eDeviceModeEnum;
        return RegDevicePoolMsg(psRegDevicePoolDevInfo);
    }

    bool CCJGW_DevicePoolThread::SendDeviceMsgToDevicePool(SDeviceMsg_Info* lpDeviceMsg)
    {
        return PostThreadMessage(ADD_DEVICEPOOL_MSG_DEFINE,WPARAM(lpDeviceMsg),NULL);
    }

    unsigned int CCJGW_DevicePoolThread::GetDevicePoolThreadID()
    {
        return m_dThreadID;
    }

    bool CCJGW_DevicePoolThread::IsStartDevicePoolThread()
    {
        return IsThreadRun();
    }

    void CCJGW_DevicePoolThread::CloseDevicePoolThread()
    {
        if( IsThreadRun() )
        {
            TerminateThread(m_hThread,-1);
            m_hThread = NULL;
            m_dThreadID = 0;
        }
        std::map<GUID_KEY,S_GUID_DEVICE_POOL_INFO>::iterator it = mmapGuidDevPoolInfo.begin();
        while (it != mmapGuidDevPoolInfo.end())
        {
            UnregisterDevNotification(it->second.mhDevNotify);
            it = mmapGuidDevPoolInfo.erase(it);
        }   
    }

    bool CCJGW_DevicePoolThread::StartDevicePoolThread(HWND hWnd,const wchar_t* strConfigPath)
    {
        if( !IsWindow(hWnd) ) return false;
        mhMainHwnd = hWnd;
        mmapsGuidUsbConfig.clear();
        mmapGuidDevPoolInfo.clear();
        m_vSDeviceMsg.clear();
        Log4WD_F(L"StartDevicePoolThread Config File Path: %s",strConfigPath);
        CCJGW_UsbConfig usbConfig;
        return (usbConfig.LoadUsbConfigToConfigPath(strConfigPath,mmapsGuidUsbConfig) && CreateMessageThread(0,0,true));
    }

    void CCJGW_DevicePoolThread::OnRspDeviceStatusChangeMsg(WPARAM wParam,LPARAM lParam)
    {
        if ( !wParam  )return ;

        SDeviceMsg_Info* lpDeviceMsg = (SDeviceMsg_Info*)wParam;
        DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] OnAddDeviceMsg %s",lpDeviceMsg->m_strDbcc_name.c_str());

        //! 过滤掉未注册的GUID信息
        if (mmapGuidDevPoolInfo.end() == mmapGuidDevPoolInfo.find(lpDeviceMsg->m_guid)) return ;

        //! 过滤相同未转发消息
        for(std::vector<SDeviceMsg_Info*>::iterator it = m_vSDeviceMsg.begin();
            it != m_vSDeviceMsg.end();
            it ++)
        {
            if ( 0 == lpDeviceMsg->m_strDbcc_name.compare(it[0]->m_strDbcc_name)  )
            {
                lpDeviceMsg->Dec();
                DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] OnAddDeviceMsg delete = %s",it[0]->m_strDbcc_name.c_str());
                return ;
            }
        }
        //! 注册表PID VID集合处理
        lpDeviceMsg->m_strRegUsbPidVid = lpDeviceMsg->m_strDbcc_name;
        /*if( std::wstring::npos != lpDeviceMsg->m_strRegUsbPidVid.find(L"USB#") )
        {
            lpDeviceMsg->m_strRegUsbPidVid.erase(0,lpDeviceMsg->m_strRegUsbPidVid.find(L"USB#") );
            if (std::wstring::npos != lpDeviceMsg->m_strRegUsbPidVid.find(L"#{"))
                lpDeviceMsg->m_strRegUsbPidVid.erase(lpDeviceMsg->m_strRegUsbPidVid.find(_T("#{")));
            ReplaceCharW(lpDeviceMsg->m_strRegUsbPidVid,'#','\\');
        }*/
        JGW_ReplaceStringW(lpDeviceMsg->m_strRegUsbPidVid,L"\\\\?\\",L"");
        if (std::wstring::npos != lpDeviceMsg->m_strRegUsbPidVid.find(L"#{"))
            lpDeviceMsg->m_strRegUsbPidVid.erase(lpDeviceMsg->m_strRegUsbPidVid.find(_T("#{")));
        ReplaceCharW(lpDeviceMsg->m_strRegUsbPidVid,'#','\\');
        
       /* lpDeviceMsg->guidtype = E_MAX_GUID_TYPE;
        //! guid type
        for (int i = 0;i < E_MAX_GUID_TYPE;i ++)
        {
            if (NULL == memcmp(&g_svGuidInfo[i].mGuid,&lpDeviceMsg->m_guid,sizeof(GUID)))
            {
                lpDeviceMsg->guidtype = g_svGuidInfo[i].meGuid;
                break;
            }
        }*/

        //! { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } }
        DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] OnAddDeviceMsg GuidType = { 0x%08X, 0x%04X, 0x%04X, {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}}",lpDeviceMsg->m_guid.Data1,lpDeviceMsg->m_guid.Data2,lpDeviceMsg->m_guid.Data3,lpDeviceMsg->m_guid.Data4[0]
            ,lpDeviceMsg->m_guid.Data4[1]
            ,lpDeviceMsg->m_guid.Data4[2]
            ,lpDeviceMsg->m_guid.Data4[3]
            ,lpDeviceMsg->m_guid.Data4[4]
            ,lpDeviceMsg->m_guid.Data4[5]
            ,lpDeviceMsg->m_guid.Data4[6]
            ,lpDeviceMsg->m_guid.Data4[7]);
        m_vSDeviceMsg.push_back(lpDeviceMsg);
    }

    void CCJGW_DevicePoolThread::RunThread()
    {
        std::vector<SDeviceMsg_Info*>::iterator it = m_vSDeviceMsg.begin();
        while (it != m_vSDeviceMsg.end())
        {
            DispatchDeviceMsg(it[0]);
            it[0]->Dec();
            it = m_vSDeviceMsg.erase(it);
        }
    }

    void CCJGW_DevicePoolThread::DispatchDeviceMsg(SDeviceMsg_Info* lpdevice)
    {
        PS_GUID_DEVICE_POOL_INFO psGuidDevInfo = &mmapGuidDevPoolInfo[lpdevice->m_guid];
        PS_GUID_USB_CONFIG psGuidUsbConfig = &mmapsGuidUsbConfig[lpdevice->m_guid];

        //! 如果未找到 cache 
        if (mmapCacheDeviceInfo.end() == mmapCacheDeviceInfo.find(lpdevice->m_strDbcc_name.c_str()))
        {
            lpdevice->m_nSortIndex = -1;
            lpdevice->m_strSeralNumber = L"";
            lpdevice->m_nComPort = -1;
            
            if (!mfilteUsbConfig.FilteUsbConfigInfo(psGuidUsbConfig,lpdevice)) return;
            //! 排序索引
            lpdevice->m_nSortIndex = GetSortIndex(lpdevice->m_strRegUsbPidVid.c_str());
            //! cache dbcc name
            mmapCacheDeviceInfo[lpdevice->m_strDbcc_name.c_str()] = lpdevice;
            lpdevice->m_bIsCache = true;
            lpdevice->Add();
        }
        else
        {
            SDeviceMsg_Info* pdevice = mmapCacheDeviceInfo.find(lpdevice->m_strDbcc_name.c_str())->second;
            pdevice->m_nDeviceMsgType = lpdevice->m_nDeviceMsgType;
            lpdevice = pdevice;
            if (E_DOWNLOADER_MODE & lpdevice->m_nDeviceMode || E_DIAG_MODE & lpdevice->m_nDeviceMode || E_MODEM_MODE & lpdevice->m_nDeviceMode)
                lpdevice->m_nComPort = mfilteUsbConfig.GetUsbComPort(lpdevice->m_strRegUsbPidVid.c_str());
            if (-1 == lpdevice->m_nSortIndex && E_ADB_MODE != lpdevice->m_nDeviceMode && E_FASTBOOT_MODE != lpdevice->m_nDeviceMode)
                lpdevice->m_nSortIndex = GetSortIndex(lpdevice->m_strRegUsbPidVid.c_str());
        }

        if (E_ADB_MODE == lpdevice->m_nDeviceMode || E_FASTBOOT_MODE == lpdevice->m_nDeviceMode)
        {
            lpdevice->m_nSortIndex = GetSortIndexAndSerialNumber(lpdevice->m_strRegUsbPidVid.c_str(),lpdevice->m_strSeralNumber);
        }

        for (std::vector<S_REG_DEVICE_POOL_DEVINFO>::iterator it = psGuidDevInfo->mvsRegDevPoolInfo.begin();
            it != psGuidDevInfo->mvsRegDevPoolInfo.end();
            ++ it)
        {
            //! 检查GUID 是否匹配
            if (NULL != memcmp(&lpdevice->m_guid,&it[0].mGuid,sizeof(GUID))) continue;
            //! 检查当前模式是否一致
            if ( !(it[0].meDeviceMode & lpdevice->m_nDeviceMode)) continue;
            
            //! SeralNumber
            lpdevice->Add();
            DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] DispatchDeviceMsg(%08X) %s (%s port:%d - %s)",
                it[0].mhWnd,lpdevice->m_strDbcc_name.c_str(),g_szDeviceMode[lpdevice->m_nDeviceMode],lpdevice->m_nComPort,lpdevice->m_strSeralNumber.c_str() );
            //! 同步发送消息
            ::SendMessage(it[0].mhWnd,it[0].m_nmsgid,WPARAM(lpdevice),NULL);
        }
        
    }

    int CCJGW_DevicePoolThread::GetSortIndexAndSerialNumber(const wchar_t* strInstanceID,std::wstring& strSerialNumber)
    {
        int nSortIndex = 0;
        wchar_t szSerialNumber[260] = {0};
        size_t nSerialNumberBufLen = 260;

        for ( int i = 0;i < 3;i ++)
        {
            //获取端口索引
            if( GetInstanceIDSortIndexAndSerialNumber( nSortIndex,strInstanceID,szSerialNumber,nSerialNumberBufLen) )
            {
                DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] GetDownModeUsbSortIndex registering[%d] %s",nSortIndex,strInstanceID);
                strSerialNumber = szSerialNumber;
                return nSortIndex;
            }
        }
        return -1;
    }

    int CCJGW_DevicePoolThread::GetSortIndex(const wchar_t* strInstanceID)
    {
        int nSortIndex = 0;
        for ( int i = 0;i < 3;i ++)
        {
            //获取端口索引
            if( GetInstanceIDSortIndex( nSortIndex,strInstanceID) )
            {
                DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] GetDownModeUsbSortIndex registering[%d] %s",nSortIndex,strInstanceID);
                return nSortIndex;
            }
        }
        return -1;
    }

    void CCJGW_DevicePoolThread::OnClearSortIndex(WPARAM wParam,LPARAM lParam)
    {
        std::map<std::wstring,SDeviceMsg_Info*>::iterator it = mmapCacheDeviceInfo.begin();
        while (it != mmapCacheDeviceInfo.end())
        {
            it->second->Clear();
            it->second->m_bIsCache = false;;
            it = mmapCacheDeviceInfo.erase(it);
        }
    }

    void CCJGW_DevicePoolThread::OnRemovePluginRegDevicePoolDevInfo(WPARAM wParam,LPARAM lParam)
    {
        HWND hWnd = (HWND)wParam;
        if (!hWnd) return ;

//         for (std::map<GUID_KEY,S_GUID_DEVICE_POOL_INFO>::iterator itpoolinfo = mmapGuidDevPoolInfo.begin();
//             itpoolinfo != mmapGuidDevPoolInfo.end();
//             ++ itpoolinfo)
        std::map<GUID_KEY,S_GUID_DEVICE_POOL_INFO>::iterator itpoolinfo = mmapGuidDevPoolInfo.begin();
        while (itpoolinfo != mmapGuidDevPoolInfo.end())
        {
            PS_GUID_DEVICE_POOL_INFO psGuidDevInfo = &itpoolinfo->second;
            //! 如果是最后一个注册
            if (1 == psGuidDevInfo->count)
            {
                UnregisterDevNotification(psGuidDevInfo->mhDevNotify);
                psGuidDevInfo->mhDevNotify = NULL;
                psGuidDevInfo->count = 0;
                psGuidDevInfo->mvsRegDevPoolInfo.clear();
                //! std::map<GUID_KEY,S_GUID_DEVICE_POOL_INFO>::iterator it = mmapGuidDevPoolInfo.find(psRegDevicePoolDevInfo->mGuid);
                itpoolinfo = mmapGuidDevPoolInfo.erase(itpoolinfo);
                continue;
            }
            //! 取消掉当前设备消息的注册
            for (std::vector<S_REG_DEVICE_POOL_DEVINFO>::iterator it = psGuidDevInfo->mvsRegDevPoolInfo.begin();
                it != psGuidDevInfo->mvsRegDevPoolInfo.end();
                ++ it)
            {
                if (it[0].mhWnd == hWnd)
                {
                    psGuidDevInfo->mvsRegDevPoolInfo.erase(it);
                    psGuidDevInfo->count --;
                    break;
                }
            }
            ++ itpoolinfo;
        }

    }

    void deleteSRegDevicePoolDevInfo(PS_REG_DEVICE_POOL_DEVINFO psRegDevicePoolDevInfo)
    {
        if (psRegDevicePoolDevInfo)
        {
            delete psRegDevicePoolDevInfo;
        }
    }
    
    void CCJGW_DevicePoolThread::OnRemoveRegDevicePoolDevInfo(WPARAM wParam,LPARAM lParam)
    {
        std::shared_ptr<S_REG_DEVICE_POOL_DEVINFO> ptrRegDevicePoolDevInfo((PS_REG_DEVICE_POOL_DEVINFO)wParam,deleteSRegDevicePoolDevInfo);
        PS_GUID_DEVICE_POOL_INFO psGuidDevInfo = NULL;
        //! 
        if (!ptrRegDevicePoolDevInfo || NULL == ptrRegDevicePoolDevInfo->mhWnd || mmapGuidDevPoolInfo.end() == mmapGuidDevPoolInfo.find(ptrRegDevicePoolDevInfo->mGuid))
        {
            return ;
        }
        psGuidDevInfo = &mmapGuidDevPoolInfo[ptrRegDevicePoolDevInfo->mGuid];

        //! 如果是最后一个注册
        if (1 == psGuidDevInfo->count)
        {
            UnregisterDevNotification(psGuidDevInfo->mhDevNotify);
            psGuidDevInfo->mhDevNotify = NULL;
            psGuidDevInfo->count = 0;
            psGuidDevInfo->mvsRegDevPoolInfo.clear();
            std::map<GUID_KEY,S_GUID_DEVICE_POOL_INFO>::iterator it = mmapGuidDevPoolInfo.find(ptrRegDevicePoolDevInfo->mGuid);
            mmapGuidDevPoolInfo.erase(it);
            return ;
        }
        //! 取消掉当前设备消息的注册
        for (std::vector<S_REG_DEVICE_POOL_DEVINFO>::iterator it = psGuidDevInfo->mvsRegDevPoolInfo.begin();
            it != psGuidDevInfo->mvsRegDevPoolInfo.end();
            ++ it)
        {
            if (it[0].mhWnd == ptrRegDevicePoolDevInfo->mhWnd && ptrRegDevicePoolDevInfo->meDeviceMode == it[0].meDeviceMode)
            {
                psGuidDevInfo->mvsRegDevPoolInfo.erase(it);
                psGuidDevInfo->count --;
                break;
            }
        }
    }
    //! 如果USB CONFIG 未配置GUID,那么就忽略此GUID的注册
    void CCJGW_DevicePoolThread::OnRspRegDevicePoolDevInfo(WPARAM wParam,LPARAM lParam)
    {
        std::shared_ptr<S_REG_DEVICE_POOL_DEVINFO> ptrsRegDevicePoolDevInfo((PS_REG_DEVICE_POOL_DEVINFO)wParam,deleteSRegDevicePoolDevInfo);
        PS_GUID_DEVICE_POOL_INFO psGuidDevInfo = NULL;
        //! 
        if (!ptrsRegDevicePoolDevInfo || NULL == ptrsRegDevicePoolDevInfo->mhWnd || mmapsGuidUsbConfig.end() == mmapsGuidUsbConfig.find(ptrsRegDevicePoolDevInfo->mGuid))
        {
            return ;
        }

        //! 未注册
        if (mmapGuidDevPoolInfo.end() == mmapGuidDevPoolInfo.find(ptrsRegDevicePoolDevInfo->mGuid))
        {
            psGuidDevInfo = &mmapGuidDevPoolInfo[ptrsRegDevicePoolDevInfo->mGuid];
            psGuidDevInfo->count = 0;
            psGuidDevInfo->meGuidType = E_MAX_GUID_TYPE;
            //! psGuidDevInfo->mbIsRegGuidType = false;
            psGuidDevInfo->mhDevNotify = NULL;
            //! guid type
            for (int i = 0;i < E_MAX_GUID_TYPE;i ++)
            {
                if (NULL == memcmp(&g_svGuidInfo[i].mGuid,&ptrsRegDevicePoolDevInfo->mGuid,sizeof(GUID)))
                {
                    psGuidDevInfo->meGuidType = g_svGuidInfo[i].meGuid;
                    break;
                }
            }
        }
        else //! 已经注册的GUID增加其计数以及添加到当前GUID的数组中
        {
            psGuidDevInfo = &mmapGuidDevPoolInfo[ptrsRegDevicePoolDevInfo->mGuid];
            psGuidDevInfo->count ++;
            psGuidDevInfo->mvsRegDevPoolInfo.push_back(*ptrsRegDevicePoolDevInfo);
            return ;
        }

        DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] OnRspRegDevicePoolDevInfo:RegisterNotification: { 0x%08X, 0x%04X, 0x%04X, {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}}",ptrsRegDevicePoolDevInfo->mGuid.Data1,ptrsRegDevicePoolDevInfo->mGuid.Data2,ptrsRegDevicePoolDevInfo->mGuid.Data3,ptrsRegDevicePoolDevInfo->mGuid.Data4[0]
        ,ptrsRegDevicePoolDevInfo->mGuid.Data4[1]
        ,ptrsRegDevicePoolDevInfo->mGuid.Data4[2]
        ,ptrsRegDevicePoolDevInfo->mGuid.Data4[3]
        ,ptrsRegDevicePoolDevInfo->mGuid.Data4[4]
        ,ptrsRegDevicePoolDevInfo->mGuid.Data4[5]
        ,ptrsRegDevicePoolDevInfo->mGuid.Data4[6]
        ,ptrsRegDevicePoolDevInfo->mGuid.Data4[7]);
        //! RegisterDeviceNotification GUID消息
        psGuidDevInfo->mhDevNotify = RegisterNotification(ptrsRegDevicePoolDevInfo->mGuid);
        if (psGuidDevInfo->mhDevNotify)
        {
            //! psGuidDevInfo->mbIsRegGuidType = true;
            psGuidDevInfo->count = 1;
            psGuidDevInfo->mvsRegDevPoolInfo.push_back(*ptrsRegDevicePoolDevInfo);
        }
        else
        {
            std::map<GUID_KEY,S_GUID_DEVICE_POOL_INFO>::iterator it = mmapGuidDevPoolInfo.find(ptrsRegDevicePoolDevInfo->mGuid);
            mmapGuidDevPoolInfo.erase(it);
            DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] RegisterNotification fail(%s)",JGW_GetSystemErrorString(GetLastError()).c_str());   
        }
    }

    HDEVNOTIFY CCJGW_DevicePoolThread::RegisterNotification(GUID guid)
    {
        DEV_BROADCAST_DEVICEINTERFACE  dbr = {0};

        dbr.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE  );
        dbr.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        dbr.dbcc_reserved = 0;
        dbr.dbcc_classguid = guid;

        return RegisterDeviceNotification(mhMainHwnd, &dbr, DEVICE_NOTIFY_WINDOW_HANDLE);
    }

    bool CCJGW_DevicePoolThread::UnregisterDevNotification(HDEVNOTIFY hDevNotify)
    {
        if (hDevNotify)
        {
            return (TRUE == UnregisterDeviceNotification(hDevNotify));
        }
        return true;
    }
}

