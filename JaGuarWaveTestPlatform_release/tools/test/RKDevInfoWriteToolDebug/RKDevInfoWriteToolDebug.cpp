// RKDevInfoWriteToolDebug.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <tchar.h>
#include <Windows.h>
#include "RKUpgradeDll.h"
#include "DevicesAPI.h"
#include <string>
#include <algorithm>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#if 0
typedef enum
{
    //! 没有发现设备
    RKUSB_NONE=0x0,
    //! MASKROM设备
    RKUSB_MASKROM=0x01,
    //! LOADER设备
    RKUSB_LOADER=0x02,
    //! MSC设备
    RKUSB_MSC=0x04,
    //! ADB设备
    RKUSB_ADB=0x08,
    //! MTP设备
    RKUSB_MTP=0x10,
    //! UVC设备
    RKUSB_UVC=0x20
} RKUSB_TYPPE;

#define VENDOR_MAX_LEN 4096

RKDContextHandle m_RKDContext = NULL;

STRUCT_DEVICE_DESC_W* GetFirstDevicesLocked(unsigned int type);
bool PreDeviceAccess(DWORD &dwLayer,const std::wstring& strLoaderPath,int nSleepMs = 0,bool bSupportAdb = true,bool bForceMskrom = false);

void EnterLoader(DWORD dwLayer)
{
    BYTE Buf[256];

    printf("EnterLoader[%x]", dwLayer);
    if(RK_ReadChipInfo(Buf, dwLayer)) 
        printf("EnterLoader[%x] success", dwLayer);
    else
        printf("EnterLoader[%x] fail", dwLayer);
}

//void ScanDeviceProc()
//{
//    DWORD dwEvent;
//    HANDLE lpHandles[2] = {m_ScanEvent.m_hObject, m_ScanExitEvent.m_hObject};
//    bool bDeviceChanged = FALSE;
//    DWORD dwType;
//
//    PSTRUCT_DEVICE_DESC pDevs = NULL;
//    UINT nDeviceCount = 0;
//
//    while (1) {
//        dwEvent = ::WaitForMultipleObjects(2, lpHandles, FALSE, 200);
//        switch (dwEvent) {
//        case WAIT_OBJECT_0 + 1:
//            goto exit;
//        case WAIT_OBJECT_0 + 0:
//        case WAIT_TIMEOUT:
//            break;
//        default:
//            break;
//        }
//        /* do scan device and update device list */
//        nDeviceCount = 0;
//        bDeviceChanged = FALSE;
//        m_csScanLock.Lock();
//        nDeviceCount = RK_ScanDevice(&pDevs);
//        if (m_nDeviceCount != nDeviceCount) {
//            m_nDeviceCount = nDeviceCount;
//            bDeviceChanged = TRUE;
//        } else {
//            for (int i = 0; i < m_nDeviceCount; i++) {
//                if (m_deviceSet_W[i].emUsbType != pDevs[i].emUsbType)
//                    bDeviceChanged = TRUE;
//            }
//        }
//        if (bDeviceChanged) {
//            memset(m_deviceSet_W, 0, sizeof(m_deviceSet_W));
//            memcpy(m_deviceSet_W, pDevs, m_nDeviceCount * sizeof(pDevs[0]));
//        }
//        m_csScanLock.Unlock();
//
///*
//    RKUSB_NONE=0x0,
//    RKUSB_MASKROM=0x01,
//    RKUSB_LOADER=0x02,
//    RKUSB_MSC=0x04,
//    RKUSB_ADB=0x08,
//    RKUSB_MTP=0x10,
//    RKUSB_UVC=0x20
// */
//        if (m_nDeviceCount == 0) {
//            m_bRedLedLight = TRUE;
//            m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_ON")).c_str();
//        } else if (m_nDeviceCount == 1) {
//            m_bRedLedLight = FALSE;
//            dwType = pDevs[0].emUsbType;
//            if (pDevs[0].emUsbType == 0x04) {
//                m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_MSC")).c_str();
//            } else if (pDevs[0].emUsbType == 0x8) {
//                m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_ADB")).c_str();
//            } else if (pDevs[0].emUsbType == 0x10) {
//                m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_MTP")).c_str();
//            } else if (pDevs[0].emUsbType == 0x20) {
//                m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_UVC")).c_str();
//            } else if (pDevs[0].emUsbType == 0x1) {
//                m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_MASKROM")).c_str();
//                m_MaksDeviceEvent.SetEvent();
//            } else {
//                m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_LOADER")).c_str();
//                if (m_Configs.m_fastEnterLoader && bDeviceChanged)
//                    EnterLoader(pDevs[0].dwLayer);
//                m_LoaderDeviceEvent.SetEvent();
//            }
//        } else if (m_nDeviceCount > 1) {
//            m_bRedLedLight = FALSE;
//            m_strDevice = GetLocalString(_T("IDS_INFO_DEVICE_MANY")).c_str();
//        }
//        
//        if (bDeviceChanged) {
//            PostMessage(WM_UPDATE_MSG, UPDATE_WINDOW, 0);
//            if (m_nDeviceCount > 0) {
//                PostMessage(WM_UPDATE_MSG, DEVICE_CHG, TRUE);
//            } else {
//                PostMessage(WM_UPDATE_MSG, DEVICE_CHG, FALSE);
//            }
//        }
//    }
//exit:
//    m_ScanExitNotify.SetEvent();
//}

//
//BOOL Crk_provision_toolDlg::OnStartReadLocked(BOOL bDoSingle)
//{
//    CString strPromt;
//    CWinThread *pThread = NULL;
//    int idx;
//
//    for (idx = 0; idx < ITEM_COUNT; idx++) {
//        if (m_Configs.GetItemEnabled(idx)) {
//            break;
//        }
//    }
//
//    if (ITEM_COUNT == idx) {
//        MessageBox(GetLocalString(_T("IDS_ERROR_NO_READ")).c_str(),_T("Error"),MB_OK|MB_ICONERROR);
//        goto OnStartReadExit;
//    }
//    //! K_USERDEBUG_RK3576-EBOOK-V10__20241212.2258\IMAGES\MiniLoaderAll.bin
//    if (m_Configs.bSetLoader) {
//        GetDlgItemText(IDC_EDIT_LOADER, m_strLoader);
//        if (m_strLoader.IsEmpty()) {
//            strPromt = GetLocalString(_T("IDS_LOADER_NOSEL_FAIL")).c_str();
//            goto OnStartReadExit;
//        }
//    }
//
//    if (!m_Configs.bDebug) { /* if debug, no test devices exist or not */
//        m_csScanLock.Lock();
//        if (m_nDeviceCount < 1) {
//            if (bDoSingle) {
//                strPromt = GetLocalString(_T("IDS_ERROR_NO_USB_WRITE")).c_str();
//            }
//            m_csScanLock.Unlock();
//            goto OnStartReadExit;
//        }
//        m_csScanLock.Unlock();
//    }
//
//    m_pReadThread = pThread = AfxBeginThread(ThreadRead, (LPVOID)this);
//    if (!pThread)
//        goto OnStartReadExit;
//    return TRUE;
//OnStartReadExit:
//    if (!strPromt.IsEmpty() && bDoSingle ) {
//        MessageBox(strPromt, GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(), MB_OK|MB_ICONERROR);
//    }
//    if (bDoSingle) {
//        PostMessage(WM_UPDATE_MSG, UPDATE_PROMPT, PROMPT_FAIL);
//    }
//    return FALSE;
//}
//
//
//
//
//BOOL Crk_provision_toolDlg::PreDeviceAccess(DWORD &dwLayer)
//{
//    STRUCT_DEVICE_DESC_W *pDeviceDesc = NULL, DeviceDesc = {0};
//    BOOL bRet = FALSE;
//
//    app_log_info("PreDeviceAccess: enter\r\n");
//    /* step 1 get a preferred device */
//    m_csScanLock.Lock();
//    if (m_Configs.bSupportAdb) {
//        pDeviceDesc = GetFirstDevicesLocked(RKUSB_ADB);
//    }
//    if (!pDeviceDesc)
//        pDeviceDesc = GetFirstDevicesLocked(RKUSB_LOADER|RKUSB_MASKROM);
//    if (pDeviceDesc)
//        memcpy(&DeviceDesc, pDeviceDesc, sizeof(DeviceDesc));
//    else if (m_Configs.bDebug) {
//        swprintf_s(DeviceDesc.szLinkName, nof(DeviceDesc.szLinkName), _T("DUMMY DEVICE"));
//        DeviceDesc.emUsbType = RKUSB_LOADER;
//        DeviceDesc.dwLayer = 0x100;
//        pDeviceDesc = &DeviceDesc;
//        app_log_info("PreDeviceAccess: No device find, and debug enabled we use dummy device\r\n");
//    }
//    m_csScanLock.Unlock();
//    if (!pDeviceDesc) {
//        app_log_info("PreDeviceAccess: no device find, exit\r\n");
//        goto exit;
//    }
//    dwLayer = DeviceDesc.dwLayer;
//    if (m_bUserStop)
//        goto exit;
//    /* get device okay */
//
//    /* step 2 adb devices and switch to loader or maskrom */
//    if (m_Configs.bDebug) {
//        app_log_info("PreDeviceAccess[%x]: debug is open, key will write to file instead of device", dwLayer);
//    } else if (DeviceDesc.emUsbType == RKUSB_ADB) {
//        bRet = AdbToRockusbWait(DeviceDesc.szLinkName, dwLayer, 3);
//        if (!bRet) {
//            app_log_info("PreDeviceAccess[%x]: switch to loader fail\r\n", dwLayer);
//            goto exit;
//        }
//        DeviceDesc.emUsbType = RKUSB_LOADER;
//    }
//    if (m_bUserStop)
//        goto exit;
//    /* for adb devices end */
//
//    /* force maskrom */
//    if (m_Configs.bForceMskrom) {
//        if (DeviceDesc.emUsbType == RKUSB_LOADER) {
//            AddPrompt(GetLocalString(_T("IDS_REBOOT_TO_MASKROM")).c_str(), LIST_INFO);
//            app_log_info("PreDeviceAccess[%x]: Current device is in loader mode, reset to maskrom...\r\n", dwLayer);
//            m_MaksDeviceEvent.ResetEvent();
//            RK_ResetRockusb(3, dwLayer);
//            if (RK_WaitDevice(dwLayer, RKUSB_MASKROM)) {
//                AddPrompt(GetLocalString(_T("IDS_REBOOT_SUCCESS")).c_str(), LIST_INFO);
//            } else {
//                AddPrompt(GetLocalString(_T("IDS_ERROR_REBOOT_MASKROM")).c_str(), LIST_ERR);
//                goto exit;
//            }
//            DeviceDesc.emUsbType = RKUSB_MASKROM;
//            if(m_Configs.nSleepMsAfterLoader) {
//                app_log_info("PreDeviceAccess[%x]: sleep %d ms after down boot\r\n", dwLayer, m_Configs.nSleepMsAfterLoader);
//                Sleep(m_Configs.nSleepMsAfterLoader);
//            }
//        }
//    }
//    if (m_bUserStop)
//        goto exit;
//    /* force maskrom end */
//
//    /* down boot */
//    if (DeviceDesc.emUsbType == RKUSB_MASKROM) {
//        app_log_info("PreDeviceAccess: download loader...\r\n");
//        AddPrompt(LIST_INFO, GetLocalString(_T("IDS_SET_LOADER")).c_str());
//        if (m_Configs.strLoaderPath.empty() || !IsFileExisted(m_Configs.strLoaderPath.c_str())) {
//            app_log_error("PreDeviceAccess[%x]: loader no exitst\r\n", dwLayer);
//            AddPrompt(LIST_ERR, GetLocalString(_T("IDS_GET_LOADER_FAIL")).c_str());
//            goto exit;
//        }
//        bRet = RK_SetFirmware(m_Configs.strLoaderPath.c_str());
//        if (!bRet) {
//            AddPrompt(LIST_ERR, GetLocalString(_T("IDS_SET_LOADER_FAIL")).c_str());
//            app_log_error(_T("PreDeviceAccess[%x]: Error:RK_SetFirmware failed. Loader path:%s"), dwLayer, m_Configs.strLoaderPath.c_str());
//            goto exit;
//        }
//        AddPrompt(LIST_INFO, GetLocalString(_T("IDS_SET_LOADER_SUCCESS")).c_str());
//
//        AddPrompt(LIST_INFO, GetLocalString(_T("IDS_DOWN_BOOT")).c_str());
//        bRet = RK_DownloadBoot(dwLayer);
//        if (!bRet) {
//            AddPrompt(LIST_ERR, GetLocalString(_T("IDS_DOWN_BOOT_FAIL")).c_str());
//            app_log_error("PreDeviceAccess[%x] Error: RK_DownloadBoot failed.", dwLayer);
//            goto exit;
//        }
//        AddPrompt(LIST_INFO, GetLocalString(_T("IDS_DOWN_BOOT_SUCCESS")).c_str());
//    }
//    /* down boot okay */
//    return TRUE;
//exit:
//    return FALSE;
//}
//
//BOOL Crk_provision_toolDlg::ReadProc()
//{
//    DWORD dwLayer;
//    BOOL bRet;
//    DWORD dwTotalTick;
//    CString strPrompt;
//    CString strItem;
//    int readlen;
//    BYTE bItemBufer[VENDOR_MAX_LEN];
//    BOOL bSuccess = FALSE;
//    DWORD mask = 0;
//    BOOL items[ITEM_END - ITEM_START + 1] = {0};
//
//    dwTotalTick = GetTickCount();
//    if (m_listInfo.GetCount()>0) {
//        PostMessage(WM_UPDATE_MSG, UPDATE_LIST, LIST_EMPTY);
//    }
//    AddPrompt(GetLocalString(_T("IDS_INFO_START_READ")).c_str(), LIST_INFO);
//    if(!PreDeviceAccess(dwLayer)) {
//        goto exit;
//    }
//
//    {
//        readlen = sizeof(bItemBufer);
//        memset(bItemBufer, 0, readlen);
//        if (RK_ReadOtpData(bItemBufer, readlen, 0, dwLayer)) {
//            printhex(_T("CPUID"), &bItemBufer[1], bItemBufer[0]);
//        }
//    }
//
//    for (int i = 0; i < ITEM_COUNT; i ++ ) {
//        if (m_bUserStop)
//            goto exit;
//        ItemBaseInfo *pItemInfo = m_Configs.GetItemInfo(i);
//
//        if (pItemInfo && pItemInfo->bEnable) {
//            strPrompt.Format(GetLocalString(_T("IDS_READ_SS")).c_str(), pItemInfo->name.c_str());
//            AddPrompt(strPrompt, LIST_INFO);
//            if (pItemInfo->wo) {
//                AddPrompt(LIST_WARN, _T("%s %s"), pItemInfo->name.c_str(), GetLocalString(_T("IDS_TEXT_WRITEONLY")).c_str());
//                continue;
//            }
//            memset(bItemBufer, 0, sizeof(bItemBufer));
//            readlen = sizeof(bItemBufer);
//            if (i == ITEM_AVB_STATE) {
//                if (m_Configs.bRPMB) {
//                    readlen = ReadVendorRpmbItem(pItemInfo->dest, pItemInfo->VendorId, bItemBufer, readlen, dwLayer);
//                } else {
//                    readlen = ReadItem(pItemInfo->VendorId, bItemBufer, readlen, dwLayer);
//                }
//                if (readlen < 0)
//                    bRet = FALSE;
//                else
//                    bRet = TRUE;
//            } else {
//                bRet = ReadFromDevice(pItemInfo->VendorId, bItemBufer, &readlen, APITYPE_NONE);
//            }
//            if (bRet && readlen > 0) {
//                strItem = SetItemString(i, bItemBufer, readlen);
//                SetItem(i, bItemBufer, readlen);
//                items[i] = TRUE;
//                strPrompt.Format(GetLocalString(_T("IDS_READ_SS_SS_PASS")).c_str(),
//                                 pItemInfo->name.c_str(), strItem.GetString());
//                AddPrompt(strPrompt, LIST_INFO);
//                app_log_error("ReadProc[%x]:Read %s:%s", dwLayer, pItemInfo->name.c_str(), strItem.GetString());
//            } else {
//                strPrompt.Format(GetLocalString(_T("IDS_READ_SS_FAIL")).c_str(), pItemInfo->name.c_str());
//                AddPrompt(strPrompt,LIST_ERR);
//                app_log_error("ReadProc[%x]:Read %d failed.", dwLayer, pItemInfo->VendorId);
//                goto exit;
//            }
//        }
//    }
//    /* wrting end */
//    if (m_bUserStop)
//        goto exit;
//    if (m_Configs.bRebootDevice) {
//        AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT")).c_str(),LIST_INFO);
//        if (!RK_ResetRockusb(0, dwLayer)) {
//            AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT_FAIL")).c_str(),LIST_ERR);
//            goto exit;
//        }
//        Sleep(1000);//sleep for device offline
//        AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT_PASS")).c_str(),LIST_INFO);
//    }
//    bSuccess = TRUE;
//
//exit:
//    /* every is ok, saveing result and do contact with user */
//    if (!m_bUserStop) {
//        ShowReadResultOnPass(bSuccess, items, nof(items));
//    }
//    if (m_bUserStop) {
//        strPrompt.Format(GetLocalString(_T("IDS_INFO_USER_ABORT")).c_str());
//        AddPrompt(strPrompt, LIST_ERR);
//        PostMessage(WM_UPDATE_MSG, UPDATE_PROMPT, PROMPT_EMPTY);
//    }
//    if (bSuccess) {
//        PostMessage(WM_UPDATE_MSG, UPDATE_PROMPT, PROMPT_PASS);
//    } else {
//        PostMessage(WM_UPDATE_MSG, UPDATE_PROMPT, PROMPT_FAIL);
//    }
//    strPrompt.Format(GetLocalString(_T("IDS_INFO_TIME_ELAPSE")).c_str(), (GetTickCount() - dwTotalTick )/1000,(GetTickCount() - dwTotalTick )%1000);
//    AddPrompt(strPrompt, LIST_TIME);
//
//    /* clear state */
//    m_criSection.Lock();
//    if ( m_bUserStop || m_Configs.bSingle) { /* update button status */
//        /* here means we need exit read/write status
//         */
//        PostMessage(WM_UPDATE_MSG, UPDATE_TEST_EXIT, ID_OP_RD);
//    }
//    if (m_bUserStop && (!m_Configs.bSingle)) {
//        m_bStarted = FALSE;
//    }
//    m_pWriteThread = NULL;
//    m_bRunning = FALSE;
//    m_bUserStop = FALSE;
//    m_criSection.Unlock();
//    return bSuccess;
//}

STRUCT_DEVICE_DESC_W m_deviceSet_W[MAX_DEVICE];
int m_nDeviceCount = 0;

bool ScanRKDevice()
{
    PSTRUCT_DEVICE_DESC pDevs = NULL;
    m_nDeviceCount = RK_ScanDevice(&pDevs);

    for (int i = 0;i < m_nDeviceCount;i++)
    {
        m_deviceSet_W[i].emUsbType = pDevs[i].emUsbType;
    }
    memcpy(m_deviceSet_W, pDevs, m_nDeviceCount * sizeof(pDevs[0]));

    if (1 == m_nDeviceCount)
    {
        switch (pDevs[0].emUsbType)
        {
        case RKUSB_MASKROM:
        case RKUSB_MSC:
        case RKUSB_ADB:
        case RKUSB_MTP:
        case RKUSB_UVC:
            break;
        case RKUSB_LOADER:
            EnterLoader(pDevs[0].dwLayer);
            break;
        }
    }

    return 0 != m_nDeviceCount;
}

#define RKADBBUFSIZE 4096

bool AdbToRockusb(const std::wstring& strDevPath)
{
    RKDContextHandle pDeviceHanle = NULL;/* Testee */
    wchar_t *pWorkBuf = NULL;
    int workSize = RKADBBUFSIZE;
    bool bRet = false;
    STRUCT_DEVICE_DESC_W *pDeviceDesc = NULL;

    if (!m_RKDContext) {
        printf("m_RKDContext is null, exit");
        return bRet;
    }
    pDeviceDesc = GetFirstDevicesLocked(RKUSB_ADB);
    if (!pDeviceDesc) {
        wsprintf(_TEXT("%s: there is no exist adb device\r\n"), _T(__FUNCTION__));
        return FALSE;
    }
    wsprintf(_TEXT("%s: Got device %s\r\n"), _T(__FUNCTION__), strDevPath.c_str());
    std::wstring wstrDevPath (strDevPath);
    std::transform(wstrDevPath.begin(), wstrDevPath.end(), wstrDevPath.begin(), toupper);
    pDeviceHanle = RKDOpenW(m_RKDContext, wstrDevPath.c_str());
    if (pDeviceHanle) {
        wprintf(_TEXT("AdbToRockusb: open rkdevices successfully\r\n"));
    } else {
        wprintf(_T("AdbToRockusb: open rk devices falied!!!"));
        goto exit;
    }
    pWorkBuf = new wchar_t[workSize];
    if (!pWorkBuf) {
        wprintf(_TEXT("AdbToRockusb: malloc work buffer fail\r\n"));
        goto exit;
    }
    workSize = RKADBBUFSIZE;
    memset(pWorkBuf, 0, sizeof(pWorkBuf[0])*workSize);
    RKDCmdW(pDeviceHanle, &pWorkBuf, &workSize, _T("reboot loader"));
    wprintf(_TEXT("AdbToRockusb: RKDCmd reboot loader finished\r\n"));
    bRet = true;
exit:
    if (pDeviceHanle)
        RKDClose(pDeviceHanle);
    if (pWorkBuf)
        delete[] pWorkBuf;
    return bRet;
}

bool AdbToRockusbWait(const std::wstring& strDevPath, DWORD dwLayer, int retry)
{
    BOOL bRet;
    int trys = 0;

    printf("AdbToRockusbWait[%x]: switch to loader\r\n", dwLayer);
    do {
        bRet = AdbToRockusb(strDevPath);
        if (!bRet) {
            wprintf(L"AdbToRockusbWait[%x]: switch to rockusb fail Path=%s\r\n",dwLayer, strDevPath.c_str());
            return FALSE;
        }
        bRet = RK_WaitDevice(dwLayer, RKUSB_LOADER);
        if (bRet) {
            printf("AdbToRockusbWait[%x]: RK_WaitDevice RKUSB_LOADER okay\r\n", dwLayer);
            return TRUE;
        }
        wprintf(L"AdbToRockusbWait[%x]: wait loader fail, retry %d Path=%s\r\n",dwLayer, trys, strDevPath.c_str());
        trys++;
    } while(trys < retry);
    return FALSE;
}


STRUCT_DEVICE_DESC_W* GetFirstDevicesLocked(unsigned int type)
{
    int idx;

    if (type & RKUSB_ADB) {
        for (idx = 0; idx < m_nDeviceCount; idx ++) {
            if (m_deviceSet_W[idx].emUsbType == 0x08 /* RKUSB_ADB */) {
                return &m_deviceSet_W[idx];
            }
        }
    }

    if (type & RKUSB_LOADER) {
        for (idx = 0; idx < m_nDeviceCount; idx ++) {
            if (m_deviceSet_W[idx].emUsbType == 0x02 /* RKUSB_LOADER */) {
                return &m_deviceSet_W[idx];
            }
        }
    }

    if (type & RKUSB_MASKROM) {
        for (idx = 0; idx < m_nDeviceCount; idx ++) {
            if (m_deviceSet_W[idx].emUsbType == 0x01 /* RKUSB_MASKROM */) {
                return &m_deviceSet_W[idx];
            }
        }
    }
    return NULL;
}


//! 
bool PreDeviceAccess(DWORD &dwLayer,const std::wstring& strLoaderPath,int nSleepMs,bool bSupportAdb,bool bForceMskrom)
{
    STRUCT_DEVICE_DESC_W *pDeviceDesc = NULL, DeviceDesc = {0};
    bool bRet = false;

    /* step 1 get a preferred device 获得首选设备*/
    if (bSupportAdb) {
        pDeviceDesc = GetFirstDevicesLocked(RKUSB_ADB);
    }
    if (!pDeviceDesc)
        pDeviceDesc = GetFirstDevicesLocked(RKUSB_LOADER|RKUSB_MASKROM);

    if (pDeviceDesc)
        memcpy(&DeviceDesc, pDeviceDesc, sizeof(DeviceDesc));

    if (!pDeviceDesc) {
        printf("PreDeviceAccess: no device find, exit\r\n");
        return false;
    }
    /* get device okay */

    /* step 2 adb devices and switch to loader or maskrom */
    //! adb 模式切换为loader 或者maskrom模式
    dwLayer = DeviceDesc.dwLayer;
    if (DeviceDesc.emUsbType == RKUSB_ADB) {
        bRet = AdbToRockusbWait(DeviceDesc.szLinkName, dwLayer, 3);
        if (!bRet) {
            printf("PreDeviceAccess[%x]: switch to loader fail\r\n", dwLayer);
            return false;
        }
        DeviceDesc.emUsbType = RKUSB_LOADER;
    }
    /* get device okay */

    /* force maskrom */
    if (bForceMskrom)
    {
        if (DeviceDesc.emUsbType == RKUSB_LOADER) {
            //AddPrompt(GetLocalString(_T("IDS_REBOOT_TO_MASKROM")).c_str(), LIST_INFO);
            printf("PreDeviceAccess[%x]: Current device is in loader mode, reset to maskrom...\r\n", dwLayer);
            RK_ResetRockusb(3, dwLayer);
            if (RK_WaitDevice(dwLayer, RKUSB_MASKROM)) {
                printf("maskrom reboot success\r\n");
                //AddPrompt(GetLocalString(_T("IDS_REBOOT_SUCCESS")).c_str(), LIST_INFO);
            } else {
                //AddPrompt(GetLocalString(_T("IDS_ERROR_REBOOT_MASKROM")).c_str(), LIST_ERR);
                printf("maskrom reboot error\r\n");
                return false;
            }
            DeviceDesc.emUsbType = RKUSB_MASKROM;
            //! 是否需要等待
            if(nSleepMs > 0) {
                printf("PreDeviceAccess[%x]: sleep %d ms after down boot\r\n", dwLayer, nSleepMs);
                Sleep(nSleepMs);
            }
        }
    }
    /* force maskrom end */

    /* down boot */
    //! maskrom 模式下需要下载MiniLoaderAll.bin
    if (DeviceDesc.emUsbType == RKUSB_MASKROM) {
        printf("PreDeviceAccess: download loader...\r\n");
        //AddPrompt(LIST_INFO, GetLocalString(_T("IDS_SET_LOADER")).c_str());
        if (strLoaderPath.empty() || INVALID_FILE_ATTRIBUTES == GetFileAttributes(strLoaderPath.c_str())) {
            printf("PreDeviceAccess[%x]: loader no exitst\r\n", dwLayer);
            return false;
        }
        //! strLoaderPath = MiniLoaderAll.bin
        bRet = (TRUE == RK_SetFirmware(strLoaderPath.c_str()));
        if (!bRet) {
            //AddPrompt(LIST_ERR, GetLocalString(_T("IDS_SET_LOADER_FAIL")).c_str());
            wprintf(_T("PreDeviceAccess[%x]: Error:RK_SetFirmware failed. Loader path:%s"), dwLayer, strLoaderPath.c_str());
           return false;
        }
        printf("set loader success\r\n");
        //AddPrompt(LIST_INFO, GetLocalString(_T("IDS_SET_LOADER_SUCCESS")).c_str());

        //AddPrompt(LIST_INFO, GetLocalString(_T("IDS_DOWN_BOOT")).c_str());
        bRet = (TRUE == RK_DownloadBoot(dwLayer));
        if (!bRet) {
            //AddPrompt(LIST_ERR, GetLocalString(_T("IDS_DOWN_BOOT_FAIL")).c_str());
            printf("PreDeviceAccess[%x] Error: RK_DownloadBoot failed.", dwLayer);
            return false;
        }
        printf("down boot success");
    }
    /* down boot okay */

    return true;
}
#define nof(x) (sizeof(x) / sizeof(x[0]))
BOOL printhex(const wchar_t *tag, BYTE *data, int size)
{
#ifndef _DEBUG
    return TRUE;
#else
    wchar_t buf[128];
    int linelength = 16;
    int i, length = 0;

    if (tag)
        wprintf(tag);
    length = _sntprintf(buf, nof(buf), _T("00: %02X"), data[0]);
    for (i = 1; i < size; i ++) {
        if (!(i%linelength)) {
            length += _sntprintf(&buf[length], nof(buf) - length, _T("\r\n"));
            wprintf(buf);
            length = _sntprintf(buf, nof(buf), _T("%02d: "), i/linelength);
        }
        length += _sntprintf(&buf[length], nof(buf) - length, _T(" %02X"), data[i]);
    }
    if (length) {
        length += _sntprintf(&buf[length], nof(buf) - length, _T("\r\n"));
        wprintf(buf);
    }
    return TRUE;
#endif
}

#define VENDOR_SN_ID 1 /* serialno */
#define VENDOR_WIFI_MAC_ID 2 /* wifi mac */
#define VENDOR_LAN_MAC_ID 3 /* lan mac */
#define VENDOR_BLUETOOTH_ID 4 /* bluetooth mac */
#define VENDOR_HDCP14_HDMI_ID 5 /* HDCP 1.4 HDMI */
#define VENDOR_HDCP14_DP_ID 6 /* HDCP 1.4 DP */
#define VENDOR_HDCP2X_WFD_ID 7 /* HDCP 2.x WFD */
#define VENDOR_WIDEVINE_ID 8
#define VENDOR_PLAYREADY_CERT_ID 9
#define VENDOR_ATTENTION_KEY 10
#define VENDOR_PLAYREADY_ROOT_SL3000_ID 11
#define VENDOR_PLAYREADY_ROOT_SL2000_ID 12
#define VENDOR_HDCP14_HDMIRX_ID 13 /* HDCP 1.4 HDMIRX */
#define VENDOR_IMEI_ID 15 /* imei */
#define VENDOR_CUST1_ID 16
#define VENDOR_CUST2_ID 17
#define EINK_VCOM_ID 17
#define VENDOR_ZYB_SECRETKEY 21
#define VENDOR_RKP_ID 67
#define HDCP_2X_RX_ID 19
#define HDCP_2X_HDMIRX_ID 20
#define VENDOR_TACIPHER 120
#define VENDOR_MAX_LEN 4096

#define APITYPE_MASK 0xF
#define APITYPE_NONE 0
#define APITYPE_VENDORRPMB 1
#define APITYPE_COMPAT2 2
#define APITYPE_PROVISIONING 2
#define APITYPE_COMPAT 3
#define APITYPE_DEBUG 4
#define APITYPE_END 5
/*  flag = 3: CompatRw
 *  flag = 1: force rpmb
 *  flag = 2: provision
 *  flag = 4: debug
 *  flag = 0: by device
 */
int JudgeApiType(int flag, DWORD dwLayer)
{
    int nApiType = 0;
    int nApiTypeFlag = 0;
    BOOL bRPMB = FALSE;
    BOOL bRet;
    bool bDebug = false;
    bool bForceApi = false;
    int ForceApiType = APITYPE_NONE;
    bool bCompatRw = false;

    nApiTypeFlag = flag&APITYPE_MASK;
    if (bDebug) {
        nApiType = APITYPE_DEBUG;
    } else if (nApiTypeFlag > APITYPE_NONE && nApiTypeFlag < APITYPE_END) {
        nApiType = nApiTypeFlag;
    } else {
        if (bForceApi &&
            ForceApiType > APITYPE_NONE &&
            ForceApiType < APITYPE_END ) {
            nApiType = ForceApiType;
        } else if ( bCompatRw) {
            nApiType = APITYPE_COMPAT;
        } else {
            bRet = RK_IsSupportVendorRpmbData(bRPMB, dwLayer);
            if (!bRet) {
                printf("ReadFromDevice:RK_IsSupportVendorRpmbData FAIL");
                bRPMB = FALSE;
                nApiType = APITYPE_PROVISIONING;
            } else {
                bRPMB = TRUE;
                nApiType = APITYPE_VENDORRPMB;
            }
            printf("ReadFromDevice:RK_IsSupportVendorRpmbData (%s)", bRPMB ? "TRUE" : "FALSE");
        }
    }
    return nApiType;
}

BOOL ReadFromDevice(int id, BYTE *buffer, int *len, int flag, DWORD dwLayer = 0);
BOOL _ReadFromDevice(int id, BYTE *buffer, int *len, int flag, DWORD dwLayer=0);
int ReadVendorRpmbItem(BYTE dest, int nItemID, BYTE *buf, int len, DWORD dwLayer=0);
int ReadItem(int id, BYTE *buf, int len, DWORD dwLayer=0);
int CompatReadItem(int id, BYTE *buf, int len, DWORD dwLayer=0);
int DebugReadItem(int id, BYTE *buf, int len, DWORD dwLayer=0);


/* flag == 0: force DebugReadItem
 * flag == 1: CompatReadItem
 * flag == -1: API-TYPE1 or API-TYPE2
 */
/* flag == 0: force DebugReadItem
 * flag == 1: CompatReadItem
 * flag == -1: API-TYPE1 or API-TYPE2
 */
BOOL ReadFromDevice(int id, BYTE *buffer, int *len, int flag, DWORD dwLayer)
{
    return _ReadFromDevice(id, buffer, len, flag, dwLayer);
}

int ReadVendorRpmbItem(BYTE dest, int id, BYTE *buf, int len, DWORD dwLayer)
{
    BOOL bRet;
    USHORT nBufferSize;
    int id_in_device = id;

    nBufferSize = len;
    bRet = RK_ReadVendorRpmbData(id_in_device, dest, buf, nBufferSize, dwLayer);
    if (!bRet) {
        printf("RK_ReadVendorRpmbData(dst:%d; id:%d) failed.", dest, id_in_device);
        return -1;
    }
    printf("ReadVendorRpmbItem(dst:%d; id:%d), len=%d", dest, id_in_device, nBufferSize);
    return nBufferSize;
}

int ReadItem(int id, BYTE *buf, int len, DWORD dwLayer)
{
    BOOL bRet;
    USHORT nBufferSize;
    int id_in_device = id;

    nBufferSize = len;
    bRet = RK_ReadProvisioningData(id_in_device, buf, nBufferSize, dwLayer);
    if (!bRet) {
        printf("%s[%08x]: RK_ReadProvisioningData fail Item=%d",
            __FUNCTION__, dwLayer, id_in_device);
        return -1;
    }
    printf("%s[%08x]: Item=%d;len=%d", __FUNCTION__,
        dwLayer, id_in_device, nBufferSize);
    return nBufferSize;
}

int CompatRead(int id, PBYTE pBuf, INT& nLen, DWORD dwLayer)
{
    if (1 == id) {
        return RK_ReadSN(pBuf, nLen, dwLayer);
    } else if (2 == id) {
        return RK_ReadWifi(pBuf, nLen, dwLayer);
    } else if (3 == id) {
        return RK_ReadMAC(pBuf, nLen, dwLayer);
    } else if (4 == id) {
        return RK_ReadBT(pBuf, nLen, dwLayer);
    } else if (5 == id) {
        return RK_ReadIMEI(pBuf, nLen, dwLayer);
    }
    return -1;
}

int CompatReadItem(int id, BYTE *buf, int len, DWORD dwLayer)
{
    BOOL bRet;
    INT nBufferSize = len;
    int id_in_device = id;

    bRet = CompatRead(id_in_device, buf, nBufferSize, dwLayer);
    if (!bRet) {
        printf("CompatRead(%d) failed.", id_in_device);
        return -1;
    }
    printf("CompatReadItem(%d), len=%d", id_in_device, nBufferSize);
    return nBufferSize;
}

int DebugReadItem(int id, BYTE *buf, int len, DWORD dwLayer)
{
    int iRet = 0;
#if 0
    char szDevName[MAX_PATH] = {0};
    int id_in_device = id;

    WCharToChar(szDevName, 256, m_strModulePath.GetString());
    strcat(szDevName, "DEVNAME.bin");
    //Sleep(200);
    iRet = flash_vendor_storage_read(szDevName, id_in_device, buf, len);
    app_log_info("DebugReadItem[%08x]: id=%d,len=%d", dwLayer, id_in_device, len);
#endif
    return iRet;
}

BOOL _ReadFromDevice(int id, BYTE *buffer, int *len, int flag, DWORD dwLayer)
{
    int buffer_size = 0;
    int readlen = -1;
    int nApiType = 0;

    if (!buffer)
        return FALSE;
    if (!len)
        return FALSE;
    nApiType = JudgeApiType(flag, dwLayer);
    //! nApiType == APITYPE_VENDORRPMB
    buffer_size = *len;
    *len = 0;
    switch (nApiType) {
    case APITYPE_VENDORRPMB:
        /* vendor(0) or rpmb(1) */
        readlen = ReadVendorRpmbItem(0, id, buffer, buffer_size, dwLayer);
        break;
    case APITYPE_PROVISIONING:
        readlen = ReadItem(id, buffer, buffer_size, dwLayer);
        break;
    case APITYPE_COMPAT:
        readlen = CompatReadItem(id, buffer, buffer_size, dwLayer);
        break;
    case APITYPE_DEBUG:
    default:
        readlen = DebugReadItem(id, buffer, buffer_size, dwLayer);
        break;
    }
    if (readlen < 0 ) {
        return FALSE;
    }
    *len = readlen;
    return TRUE;
}

/* File type in file mode */
#define TYPE_NON -1
#define TYPE_TXT 0
#define TYPE_BIN 1

TCHAR byte_to_tchar(BYTE hex)
{
    if (hex < 10) {
        return hex + _T('0');
    } else if (hex < 16 ) {
        return hex - 10 + _T('A');
    }
    return _T(' ');
}

TCHAR *HEX_TO_WSTRING_GET(const BYTE *buf, int len)
{
    TCHAR *pOStr;
    USHORT usStrLen;
    int i;

    usStrLen = len*2 + 1;
    pOStr = new TCHAR[usStrLen];
    if (!pOStr)
        return NULL;
    for (i = 0; i < len; i ++ ) {
        pOStr[2*i] = byte_to_tchar((buf[i]>>4)&0xf);
        pOStr[2*i + 1] = byte_to_tchar((buf[i])&0xf);
    }
    pOStr[2*i] = 0;
    return pOStr;
}


bool ReadRKDevInfo()
{
    DWORD dwLayer = 0;
    if(!PreDeviceAccess(dwLayer,L"\\\\192.168.16.25\\zengliwei\\rk3576_14\\IMAGE\\RK3576_EBOOK_USERDEBUG_RK3576-EBOOK-V10__20241212.2258\\IMAGES\\MiniLoaderAll.bin")) {
        return false;
    }

    int readlen = 0;
    BYTE bItemBufer[VENDOR_MAX_LEN];
    {
        readlen = sizeof(bItemBufer);
        memset(bItemBufer, 0, readlen);
        if (RK_ReadOtpData(bItemBufer, readlen, 0, dwLayer)) {
            printhex(_T("CPUID"), &bItemBufer[1], bItemBufer[0]);
        }
    }
    //! read sn /mac
    DWORD VendorId = VENDOR_SN_ID;
    DWORD bBinOrText = TYPE_TXT;
    std::wstring strValue;
    BOOL bRet = ReadFromDevice(VendorId, bItemBufer, &readlen, APITYPE_NONE);
     if (bRet && readlen > 0) {
         if (TYPE_TXT == bBinOrText) {
             char szBuff[128] = {0};
             memcpy_s(szBuff,128,bItemBufer,readlen);
             strValue = JGW::JGW_A2W_A(szBuff);
             //pszTemp = BYTE_TO_WSTRING_GET(pBuf, len);
             //Str = pszTemp;
             //WSTRING_PUT(pszTemp);
         } else if (TYPE_BIN == bBinOrText) {
             wchar_t* pszTemp = HEX_TO_WSTRING_GET(bItemBufer, readlen);
             strValue = pszTemp;
             delete[] pszTemp;
         }
     }

     bool bRebootDevice = false;
     //! 重启设备
     if (bRebootDevice) {
         if (!RK_ResetRockusb(0, dwLayer)) {
             return false;
         }
         Sleep(1000);//sleep for device offline
     }

    return true;
}

BOOL WriteCmItem(DWORD dwLayer, BOOL &bSkip, int item_id);
/*  flag = 2: CompatRw
*  flag = 1: force rpmb
*  flag = 0: provision
*  flag = -1: read from device
*/
BOOL WriteToDevice(int id, BYTE *buf, int len, BOOL *bSkiped, int flag, DWORD dwLayer = 0);
int WriteVendorRpmbItem(BYTE dest, int nItemID, BYTE *buf, int len, DWORD dwLayer=0);
int WriteItem(int id, BYTE *buf, int len, DWORD dwLayer=0);
int CompatWriteItem(int id, BYTE *buf, int len, DWORD dwLayer=0);
int CompatWrite(int id, PBYTE pSN, INT& nSNLen, DWORD dwLayer=0);

int WriteVendorRpmbItem(BYTE dest, int id, BYTE *buf, int len, DWORD dwLayer/* =0 */)
{
    BOOL bRet;
    PBYTE pReadBuf = NULL;
    USHORT nBufferSize = len;
    int id_in_device = id;

    if (!buf || (0 >= len)) {
        printf("WriteVendorRpmbItem(dst:%d; id:%d) buf=0x%08x, len=%d", dest, id_in_device, buf, len);
        return FALSE;
    }
    bRet = RK_WriteVendorRpmbData(id_in_device, dest, buf, len, dwLayer);
    if (!bRet) {
        printf("RK_WriteVendorRpmbData(dst:%d; id:%d) failed.", dest, id_in_device);
        return FALSE;
    }
    pReadBuf = new BYTE[nBufferSize];
    if (!pReadBuf) {
        printf("WriteVendorRpmbItem(dst:%d; id:%d) alloc read buffer failed.", dest, id_in_device);
        return FALSE;
    }
    bRet = RK_ReadVendorRpmbData(id_in_device, dest, pReadBuf, nBufferSize, dwLayer);
    if (!bRet) {
        printf("RK_ReadVendorRpmbData(dst:%d; id:%d) failed.", dest, id_in_device);
        return FALSE;
    }

    if (memcmp(pReadBuf, buf, nBufferSize) != 0) {
        if (pReadBuf) {
            delete[] pReadBuf;
        }
        printf("WriteVendorRpmbItem(dst:%d; id:%d) compare failed.", dest, id_in_device);
        return FALSE;
    }
    printf("WriteVendorRpmbItem(dst:%d; id:%d), len=%d", dest, id_in_device, nBufferSize);
    if (pReadBuf) {
        delete[] pReadBuf;
    }
    return TRUE;
}

int WriteItem(int id, BYTE *buf, int len, DWORD dwLayer/* =0 */)
{
    BOOL bRet;
    PBYTE pReadBuf = NULL;
    USHORT nBufferSize = len;
    int id_in_device = id;

    if (!buf || (0 >= len)) {
        printf("WriteItem(%d) buf=0x%08x, len=%d", id_in_device, buf, len);
        return FALSE;
    }
    bRet = RK_WriteProvisioningData(id_in_device, buf, len, 0, dwLayer);
    if (!bRet) {
        printf("%s[%08x]: RK_ReadProvisioningData fail Item=%d",
            __FUNCTION__, dwLayer, id_in_device);
        return FALSE;
    }
    pReadBuf = new BYTE[nBufferSize];
    if (!pReadBuf) {
        printf("WriteItem(%d) alloc read buffer failed.", id_in_device);
        return FALSE;
    }
    bRet = RK_ReadProvisioningData(id_in_device, pReadBuf, nBufferSize, dwLayer);
    if (!bRet) {
        printf("RK_ReadProvisioningData(%d) failed.", id_in_device);
        return FALSE;
    }
    if (memcmp(pReadBuf, buf, nBufferSize) != 0) {
        if (pReadBuf) {
            delete[] pReadBuf;
        }
        printf("WriteItem(%d) compare failed.", id_in_device);
        return FALSE;
    }
    printf("WriteItem(%d), len=%d", id_in_device, nBufferSize);
    if (pReadBuf) {
        delete[] pReadBuf;
    }
    return TRUE;
}

int CompatWrite(int id, PBYTE pBuf, INT& nLen, DWORD dwLayer/* =0 */)
{
    if (1 == id) {
        return RK_WriteSN(pBuf, nLen, dwLayer);
    } else if (2 == id) {
        return RK_WriteWifi(pBuf, nLen, dwLayer);
    } else if (3 == id) {
        return RK_WriteMAC(pBuf, nLen, dwLayer);
    } else if (4 == id) {
        return RK_WriteBT(pBuf, nLen, dwLayer);
    } else if (5 == id) {
        return RK_WriteIMEI(pBuf, nLen, dwLayer);
    }
    return -1;
}

int CompatWriteItem(int id, BYTE *buf, int len, DWORD dwLayer/* =0 */)
{
    BOOL bRet;
    PBYTE pReadBuf = NULL;
    INT nBufferSize = len;
    int id_in_device = id;

    if (!buf || (0 >= len)) {
        printf("CompatWriteItem(%d) buf=0x%08x, len=%d", id_in_device, buf, len);
        return FALSE;
    }
    bRet = CompatWrite(id_in_device, buf, len, dwLayer);
    if (!bRet) {
        printf("CompatWrite(%d) failed.", id_in_device);
        return FALSE;
    }
    pReadBuf = new BYTE[nBufferSize];
    if (!pReadBuf) {
        printf("CompatWriteItem(%d) alloc read buffer failed.", id_in_device);
        return FALSE;
    }
    bRet = CompatRead(id_in_device, pReadBuf, nBufferSize, dwLayer);
    if (!bRet) {
        printf("CompatRead(%d) failed.", id_in_device);
        return FALSE;
    }
    if (memcmp(pReadBuf, buf, nBufferSize) != 0) {
        if (pReadBuf) {
            delete[] pReadBuf;
        }
        printf("CompatWriteItem(%d) compare failed.", id_in_device);
        return FALSE;
    }
    printf("CompatWriteItem(%d), len=%d", id_in_device, nBufferSize);
    if (pReadBuf) {
        delete[] pReadBuf;
    }
    return TRUE;
}


BOOL WriteToDevice(int id, BYTE *buffer, int buffer_size, BOOL *bSkiped, int flag, DWORD dwLayer)
{
    int nApiType = 0;
    BYTE readbuffer[VENDOR_MAX_LEN];
    int readbuffer_size = VENDOR_MAX_LEN;
    BOOL bRet = FALSE;

    nApiType = JudgeApiType(flag, dwLayer);
    // nApiType = APITYPE_VENDORRPMB
    //! 是否强制写入，如果不强制写入的话，先读然后判断是否写入一致，否则不判断直接写
    bool bForceWrite = true;
    if (!bForceWrite)
    {
        bRet = _ReadFromDevice(id, readbuffer, &readbuffer_size, nApiType, dwLayer);
        if (bRet && readbuffer_size > 0) {
            *bSkiped = TRUE;
            printf("WriteToDevice:read success, skip");
            return TRUE;
        }
    }

    *bSkiped = FALSE;
    switch (nApiType) {
    case APITYPE_VENDORRPMB:
        /* vendor(0) or rpmb(1) */
        bRet = WriteVendorRpmbItem(0, id, buffer, buffer_size, dwLayer);
        break;
    case APITYPE_PROVISIONING:
        bRet = WriteItem(id, buffer, buffer_size, dwLayer);
        break;
    case APITYPE_COMPAT:
        bRet = CompatWriteItem(id, buffer, buffer_size, dwLayer);
        break;
    case APITYPE_DEBUG:
    default:
        bRet = FALSE;
        break;
    }
    return bRet;
}


BOOL WriteCmItem(DWORD dwLayer, BOOL &bSkip, int item_id)
{
    DWORD VendorId = VENDOR_SN_ID;
    DWORD bBinOrText = TYPE_TXT;
    char* pData = "12345678";
    int size = strlen(pData);
    return (TRUE == WriteToDevice(VendorId, (BYTE*)pData, size, &bSkip, APITYPE_NONE));
}


bool WriteRKDevInfo()
{
    DWORD dwLayer = 0;
    if(!PreDeviceAccess(dwLayer,L"\\\\192.168.16.25\\zengliwei\\rk3576_14\\IMAGE\\RK3576_EBOOK_USERDEBUG_RK3576-EBOOK-V10__20241212.2258\\IMAGES\\MiniLoaderAll.bin")) {
        return false;
    }
    DWORD idx = 0;
    BOOL bSkip = FALSE;
    bool bRet = (TRUE == WriteCmItem(dwLayer, bSkip, idx));

    return bRet;
}

#endif
int _tmain(int argc, _TCHAR* argv[])
{
    INIT_DEV_INFO InitDevInfo;
    INIT_LOG_INFO InitLogInfo;
    INIT_CALLBACK_INFO InitCallbackInfo;
    
    /* UpgradeDll init */
    InitDevInfo.bScan4FsUsb = FALSE;
    InitDevInfo.emSupportDevice = 0;
    InitDevInfo.uiRockMscTimeout = 30;
    InitDevInfo.uiRockusbTimeout = 30;
    InitDevInfo.usRockMscPid = 0;
    InitDevInfo.usRockMscVid = 0;
    InitDevInfo.usRockusbPid = 0;
    InitDevInfo.usRockusbVid = 0;
    InitLogInfo.bLogEnable = TRUE;
    InitLogInfo.lpszLogPathName = L"D:\\Development\\RK3576\\工具源码_63216\\rkdevinfowritetool-v1.2.7-202411271001\\bin\\";
    // 您可以将其设置为 ProgressPromptProc 来显示升级信息；
    InitCallbackInfo.pProgressPromptProc = NULL;//you can set it to ProgressPromptProc for showing upgrade info;
    // 您可以将其设置为 UpgradeStepPromptProc 来显示进度信息；
    InitCallbackInfo.pUpgradeStepPromptProc = NULL;//you can set it to UpgradeStepPromptProc for showing progress info;
    bool m_bUpgradeDllInitOK = (TRUE == RK_Initialize(InitDevInfo, InitLogInfo, InitCallbackInfo));
    if (!m_bUpgradeDllInitOK) {
        MessageBox(NULL,_T("Initialize RKUpgrade dll failed!"),_T("ERROR"),MB_ICONERROR | MB_OK);
    } else {
        //m_pScanThread = AfxBeginThread(ThreadScanDevice,(LPVOID)this);
        //ScanRKDevice();
       // ScanDeviceProc();
    }

    do {
        RKDContextHandle m_RKDContext = RKDInit();
        if (!m_RKDContext) {
            printf("RKDInit failed\r\n");
        } else {
            RKDSetLogEn(m_RKDContext, NULL, 0);
        }
    } while (0);

    //ReadRKDevInfo();
    //WriteRKDevInfo();

	return 0;
}

