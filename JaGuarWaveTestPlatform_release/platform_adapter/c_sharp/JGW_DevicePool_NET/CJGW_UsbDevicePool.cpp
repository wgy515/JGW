#include "StdAfx.h"
#include "CJGW_UsbDevicePool.h"

namespace JGW
{
    CCJGW_UsbDevicePool::CCJGW_UsbDevicePool(void) : m_lpDevicePoolImpl(NULL),mdwThreadID(0)
    {
    }


    CCJGW_UsbDevicePool::~CCJGW_UsbDevicePool(void)
    {
        for (size_t i = 0;i < m_vSDeviceMsg.size();i ++)
        {
            delete m_vSDeviceMsg[i];
        }
        m_vSDeviceMsg.clear();
    }

    bool CCJGW_UsbDevicePool::StartUsbDevicePool(HWND hMainWnd)
    {
        if (m_lpDevicePoolImpl) return false;

        S_POCO_CLASS_LOADER_INFO sPocoClassLoaderInfo;
        sPocoClassLoaderInfo.mstrClsid = L"JGW::CCJGW_DevicePoolThread";
        sPocoClassLoaderInfo.mstrModuleName = JGW_GetApplicationFolder();
        sPocoClassLoaderInfo.mstrModuleName += L"JGW_DevicePool.dll";
        
        m_lpDevicePoolImpl = m_cLoadDevicePoolSubPlugin.LoadPocoClassLoader(sPocoClassLoaderInfo);
        Log4WD(L"CCJGWMainDlg_Net::StartDevicePool");

        if ( !m_lpDevicePoolImpl ) return false;
        //! 注册USB标记 防止重复注册
        //! UPDATEREGUSBFLAGS();
        //! 初始化设备消息起始大小

        std::wstring strTemp (JGW_GetApplicationFolder());
        strTemp += _T(USB_CONFIG_INI_FILE_NAME);
        //! 开启设备池
        if ( !m_lpDevicePoolImpl->StartDevicePoolThread(hMainWnd,strTemp.c_str()) ) 
        {
            ::MessageBox(hMainWnd,_T("设备池创建失败"),_T("错误"),MB_ICONERROR);
            return false;
        }
        //! 注册线程设备PIDVID消息
        GUID guid = GUID_COM_INTERFACE_USB_DEVICE;
        m_lpDevicePoolImpl->RegDevicePoolMsg(WM_RESPONCE_DEVICEPOOL_MSG_DEFINE,hMainWnd,guid,E_ALL_MODE);

        //! 保存线程池线程ID
        mdwThreadID = m_lpDevicePoolImpl->GetDevicePoolThreadID();
        return true;
    }

//     void CCJGW_UsbDevicePool::RegUsbDevicePoolMsg(UINT nMsgID,HWND hwnd,GUID guid,short eDeviceModeEnum)
//     {
// 
//     }

    size_t CCJGW_UsbDevicePool::GetUsbThreadPoolID()
    {
        return mdwThreadID;
    }

    void CCJGW_UsbDevicePool::StopUsbDevicePool()
    {
        if (m_lpDevicePoolImpl) 
        {
            m_lpDevicePoolImpl->CloseDevicePoolThread();
            delete m_lpDevicePoolImpl;
            m_lpDevicePoolImpl = NULL;
        }
    }

    SDeviceMsg_Info* CCJGW_UsbDevicePool::GetDeviceMsgInfoPtr()
    {
        for ( size_t i = 0;i < m_vSDeviceMsg.size();i ++)
        {
            if (m_vSDeviceMsg[i]->m_nRefCount <= 0 && !m_vSDeviceMsg[i]->m_bIsCache)
            {
                Log4WD_F(L"[GetDeviceMsgInfoPtr] %d",i);
                return m_vSDeviceMsg[i];
            }
        }

        SDeviceMsg_Info* psDeviceMsg = new SDeviceMsg_Info;
        m_vSDeviceMsg.push_back(psDeviceMsg);
        return psDeviceMsg;
    }

    void CCJGW_UsbDevicePool::OnDeviceChange(WPARAM wParam,LPARAM lParam)
    {
        if( !m_lpDevicePoolImpl || !m_lpDevicePoolImpl->IsStartDevicePoolThread() || !lParam ) return ;

        SDeviceMsg_Info* lpdevicemsg = NULL;
        PDEV_BROADCAST_DEVICEINTERFACE lpdb = (DEV_BROADCAST_DEVICEINTERFACE*)lParam;
        if( DBT_DEVTYP_DEVICEINTERFACE != lpdb->dbcc_devicetype ) return ;
        switch (wParam)
        {
        case DBT_DEVNODES_CHANGED:
            break;
        case DBT_DEVICEARRIVAL:
            lpdevicemsg = GetDeviceMsgInfoPtr();
            if(!lpdevicemsg) return ;
            lpdevicemsg->InitDeviceMsg();
            lpdevicemsg->Add();
            lpdevicemsg->m_strDbcc_name = lpdb->dbcc_name;
            lpdevicemsg->m_nDeviceMsgType = DBT_DEVICEARRIVAL;
            memcpy(&lpdevicemsg->m_guid,&lpdb->dbcc_classguid,sizeof(GUID));
            Log4WD_F(L"CCJGWMainDlg_Net::OnDeviceChange DBT_DEVICEARRIVAL %s",lpdb->dbcc_name);
            m_lpDevicePoolImpl->SendDeviceMsgToDevicePool(lpdevicemsg);
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            lpdevicemsg = GetDeviceMsgInfoPtr();
            if(!lpdevicemsg) return ;
            lpdevicemsg->InitDeviceMsg();
            lpdevicemsg->Add();
            lpdevicemsg->m_strDbcc_name = lpdb->dbcc_name;
            lpdevicemsg->m_nDeviceMsgType = DBT_DEVICEREMOVECOMPLETE;
            Log4WD_F(L"CCJGWMainDlg_Net::OnDeviceChange DBT_DEVICEREMOVECOMPLETE %s",lpdb->dbcc_name);
            memcpy(&lpdevicemsg->m_guid,&lpdb->dbcc_classguid,sizeof(GUID));
            m_lpDevicePoolImpl->SendDeviceMsgToDevicePool(lpdevicemsg);
            break;
        default:
            break;
        }
    }
}
