#include "StdAfx.h"
#include "CJGW_RKDeviceHelp.h"
#include <algorithm>

namespace JGW
{
    STRUCT_DEVICE_DESC_W CCJGW_RKDeviceHelp::m_deviceSet_W[MAX_DEVICE] = {0};
    int CCJGW_RKDeviceHelp::m_nDeviceCount = 0;
    STRUCT_DEVICE_DESC_W CCJGW_RKDeviceHelp::m_currentDeviceDesc = {0};
    RKDContextHandle CCJGW_RKDeviceHelp::mhRKDContextHandle = NULL;
	int gdwLayerID = -1;

    const ID_MAP2 gid_map2[ITEM_COUNT] = {
        {ITEM_SN,       _T("SN"),       FIXED_ID | TYPE_DYN | VENDOR_SN_ID,         TYPE_TXT, NULL/*check_sn*/},
        {ITEM_WIFI,     _T("WIFI MAC"), FIXED_ID | TYPE_DYN | VENDOR_WIFI_MAC_ID,   TYPE_BIN, NULL/*check_mac*/},
        {ITEM_LAN,      _T("LAN MAC"),  FIXED_ID | TYPE_DYN | VENDOR_LAN_MAC_ID,    TYPE_BIN, NULL/*check_mac*/},
        {ITEM_BT,       _T("BT MAC"),   FIXED_ID | TYPE_DYN | VENDOR_BLUETOOTH_ID,  TYPE_BIN,NULL /*check_mac*/},
        {ITEM_IMEI,     _T("IMEI"),     FIXED_ID | TYPE_DYN | VENDOR_IMEI_ID,       TYPE_TXT,NULL /*check_imei*/},
        {ITEM_CUST1,    _T("VCOM"),      FIXED_ID | TYPE_DYN | EINK_VCOM_ID,      TYPE_NON, NULL/*check_custx*/},
        {ITEM_CUST2,    _T("SECRETKEY"),            FIXED_ID | TYPE_DYN | VENDOR_ZYB_SECRETKEY,     TYPE_NON,NULL /*check_secretkey*/},
        {ITEM_ID_ATTE,  _T("Device ID attestation"),    FIXED_ID | TYPE_DYN | TA_IDATTE | TYPE_WO,      TYPE_BIN, NULL},
        {ITEM_ATTENTION,_T("Attestation Key"),          FIXED_ID | VENDOR_ATTENTION_KEY | TYPE_WO,      TYPE_BIN, NULL},
        {ITEM_WIDEVINE, _T("Widevine Key"),             FIXED_ID | VENDOR_WIDEVINE_ID   | TYPE_WO,      TYPE_BIN, NULL},
        {ITEM_ID_RKP,   _T("RKP"),                      FIXED_ID | TYPE_DYN  | VENDOR_RKP_ID | TYPE_RO, TYPE_BIN, NULL},
        {ITEM_HDCP14_HDMITX,_T("HDCP HDMI"),    FIXED_ID | VENDOR_HDCP14_HDMI_ID,   TYPE_NON, NULL}, /* ITEM_HDCP14_HDMITX 7 */
        {ITEM_HDCP14DP,     _T("HDCP DP"),      FIXED_ID | VENDOR_HDCP14_DP_ID,     TYPE_NON, NULL}, /* ITEM_HDCP14DP 8 */
        {ITEM_HDCP2XWFD,    _T("HDCP WFD"),     FIXED_ID | VENDOR_HDCP2X_WFD_ID,    TYPE_NON, NULL}, /* ITEM_HDCP2XWFD 9 */
        {ITEM_HDCP2X_HDMIRX,_T("HDCP2X HDMIRX"),FIXED_ID | HDCP_2X_HDMIRX_ID,       TYPE_NON, NULL}, /* ITEM_HDCP2X_HDMIRX 10 */
        {ITEM_HDCP14_HDMIRX,_T("HDCP14 HDMIRX"),FIXED_ID | VENDOR_HDCP14_HDMIRX_ID, TYPE_NON, NULL}, /* ITEM_HDCP14_HDMIRX 11 */

        {ITEM_AVB_ATTR      , _T("AVB PERM ATTR")       , FIXED_ID | AT_PERM_ATTR_FUSE| TYPE_WO, TYPE_BIN, NULL},
        {ITEM_AVB_ATTR_CER  , _T("AVB PERM ATTR CER")   , FIXED_ID | AT_RSA_PERM_ATTR_FUSE| TYPE_WO, TYPE_BIN, NULL},
        {ITEM_AVB_LOCK      , _T("AVB LOCK")            , FIXED_ID | AT_LOCK_VBOOT| TYPE_WO, TYPE_BIN, NULL},
        {ITEM_AVB_STATE     , _T("AVB STATE")           , FIXED_ID | AT_LOCK_STATE | TYPE_RO, TYPE_BIN, NULL},
        {ITEM_OEMHUK       , _T("OEMHUK")               , FIXED_ID | VENDOR_TACIPHER | TYPE_WO, TYPE_BIN, NULL},
        {ITEM_TACIPHER     , _T("TACIPHER")             , FIXED_ID | VENDOR_TACIPHER | TYPE_WO, TYPE_BIN, NULL},

		{ITEM_BARCODE_ID,       _T("Barcode"),       FIXED_ID | TYPE_DYN | VENDOR_BARCODE_ID,         TYPE_TXT, NULL/*check_sn*/},
		{ITEM_FT_FLAG,       _T("FT Flag"),       FIXED_ID | TYPE_DYN | VENDOR_FT_FLAG,         TYPE_TXT, NULL/*check_sn*/},
		{ITEM_BATTERY_ID,       _T("Battery"),       FIXED_ID | TYPE_DYN | VENDOR_BATTERY_ID | TYPE_RO,         TYPE_TXT, NULL/*check_sn*/},
		{ITEM_DDR,       _T("DDR"),       FIXED_ID | TYPE_DYN | VENDOR_DDR | TYPE_RO,         TYPE_TXT, NULL/*check_sn*/},
		{ITEM_EMMC,       _T("EMMC"),       FIXED_ID | TYPE_DYN | VENDOR_EMMC | TYPE_RO,         TYPE_TXT, NULL/*check_sn*/},
        {ITEM_FIRMWARE_VERSION,       _T("Firmware Version"),       FIXED_ID | TYPE_DYN | VENDOR_FIRMWARE_VERSION | TYPE_RO,         TYPE_TXT, NULL/*check_sn*/},
        {ITEM_CPU,       _T("CPU"),       FIXED_ID | TYPE_DYN | VENDOR_CPU | TYPE_RO,         TYPE_TXT, NULL/*check_sn*/},
        {ITEM_ZYB_VERSION,       _T("CPU"),       FIXED_ID | TYPE_DYN | VENDOR_ZYB_VERSION | TYPE_RO,         TYPE_TXT, NULL/*check_sn*/},
        
        
    };

    const ID_MAP2 gid_map2_null = {-1, _T("NULL"), 0, 0, NULL};

    CCJGW_RKDeviceHelp::CCJGW_RKDeviceHelp(void)
    {
    }


    CCJGW_RKDeviceHelp::~CCJGW_RKDeviceHelp(void)
    {
        
    }

    /* the uiid is the id in app, it's a zero base index, always used to index a array
    * the vendorid is the id in vendorstorage
    */
    const ID_MAP2 *get_id_map(int item_id)
    {
        int i;

        for (i = 0; i < _countof(gid_map2); i++) {
            if(gid_map2[i].item_id == item_id)
                return &gid_map2[i];
        }
        return &gid_map2_null;
    }

    int _get_dyn(int uiid, const ID_MAP2 *m)
    {
        if (!m)
            m = get_id_map(uiid);
        return !!(MASK_DYN & m->id);
    }

    int _get_vendorid(int uiid, const ID_MAP2 *m)
    {
        if (!m)
            m = get_id_map(uiid);
        return MASK_VENDOR_ID & m->id;
    }

    BOOL _is_fixedid(int uiid, const ID_MAP2 *m)
    {
        if (!m)
            m = get_id_map(uiid);
        return !!(MASK_FIXED_ID & m->id);
    }

    BOOL _is_readonly(int uiid, const ID_MAP2 *m)
    {
        if (!m)
            m = get_id_map(uiid);
        return !!(MASK_RO & m->id);
    }

    BOOL _is_writeonly(int uiid, const ID_MAP2 *m)
    {
        if (!m)
            m = get_id_map(uiid);
        return !!(MASK_WO & m->id);
    }

    int CCJGW_RKDeviceHelp::GetStringIntHexToVendorID(int nVendorID,const ID_MAP2 *m)
    {
        if (!m) return TYPE_TXT;
        return m->str_int_hex;
    }

    int CCJGW_RKDeviceHelp::GetRKDeviceInfoToLayerID(DWORD dwLayerID)
    {
        PSTRUCT_DEVICE_DESC pDevs = NULL;
        m_nDeviceCount = RK_ScanDevice(&pDevs);
		gdwLayerID = dwLayerID;

        for (int i = 0;i < m_nDeviceCount;i++)
        {
            if (dwLayerID == pDevs[i].dwLayer)
            {
                CCJGW_RKDeviceHelp::m_deviceSet_W[i].emUsbType = pDevs[i].emUsbType;
                if (RKUSB_LOADER == pDevs[i].emUsbType)
                {
                    EnterLoader(pDevs[i].dwLayer);
                }
				memcpy(&m_currentDeviceDesc, &pDevs[i], sizeof(m_currentDeviceDesc));
                memcpy(CCJGW_RKDeviceHelp::m_deviceSet_W, pDevs, m_nDeviceCount * sizeof(pDevs[0]));
                return 1;
            } 
        }

        return 0;
    }

    int CCJGW_RKDeviceHelp::ScanRKDevice()
    {
		gdwLayerID = -1;
        PSTRUCT_DEVICE_DESC pDevs = NULL;
        m_nDeviceCount = 0;

        m_nDeviceCount = RK_ScanDevice(&pDevs);
        if (m_nDeviceCount > 0)
        {
            for (int i = 0;i < m_nDeviceCount;i++)
            {
                CCJGW_RKDeviceHelp::m_deviceSet_W[i].emUsbType = pDevs[i].emUsbType;
                if (RKUSB_LOADER == pDevs[i].emUsbType)
                {
                    EnterLoader(pDevs[i].dwLayer);
                }
            }
            memcpy(CCJGW_RKDeviceHelp::m_deviceSet_W, pDevs, m_nDeviceCount * sizeof(pDevs[0]));
        }

        return m_nDeviceCount;
    }

    bool CCJGW_RKDeviceHelp::EnterLoader(DWORD dwLayer)
    {
        BYTE buf[256];

        if (TRUE == RK_ReadChipInfo(buf, dwLayer))
        {
            LogI_F(L"EnterLoader[%x] success", dwLayer);
            return true;
        }
        LogE_F(L"EnterLoader[%x] fail", dwLayer);

        return false;
    }

    STRUCT_DEVICE_DESC_W* CCJGW_RKDeviceHelp::GetFirstDevicesLocked(unsigned int type)
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

    bool CCJGW_RKDeviceHelp::AdbToRockusb(const std::wstring& strDevPath)
    {
        RKDContextHandle pDeviceHanle = NULL;/* Testee */
        wchar_t *pWorkBuf = NULL;
        int workSize = RKADBBUFSIZE;
        STRUCT_DEVICE_DESC_W *pDeviceDesc = NULL;
        bool bRet = false;

        if (NULL == mhRKDContextHandle) 
        {
            LogE("RKDContext is null, exit");
            return false;
        }

        pDeviceDesc = GetFirstDevicesLocked(RKUSB_ADB);
        if (!pDeviceDesc) 
        {
            LogE(L"AdbToRockusb there is no exist adb device\r\n");
            return false;
        }
        LogI_F(_TEXT("%s: Got device %s\r\n"), _T(__FUNCTION__), strDevPath.c_str());

        std::wstring wstrDevPath (strDevPath);
        std::transform(wstrDevPath.begin(), wstrDevPath.end(), wstrDevPath.begin(), toupper);

        pDeviceHanle = RKDOpenW(mhRKDContextHandle, wstrDevPath.c_str());
        if (pDeviceHanle) 
        {
            LogI(_TEXT("AdbToRockusb: open rkdevices successfully\r\n"));
        } 
        else 
        {
            LogE(_T("AdbToRockusb: open rk devices falied!!!"));
            goto exit;
        }
        pWorkBuf = new wchar_t[workSize];
        if (!pWorkBuf) 
        {
            wprintf(_TEXT("AdbToRockusb: malloc work buffer fail\r\n"));
            goto exit;
        }
        workSize = RKADBBUFSIZE;
        memset(pWorkBuf, 0, sizeof(pWorkBuf[0])*workSize);
        RKDCmdW(pDeviceHanle, &pWorkBuf, &workSize, _T("reboot loader"));
        LogI(_TEXT("AdbToRockusb: RKDCmd reboot loader finished\r\n"));
        bRet = true;
exit:
        if (pDeviceHanle)
            RKDClose(pDeviceHanle);
        if (pWorkBuf)
            delete[] pWorkBuf;
        return bRet;
    }

    bool CCJGW_RKDeviceHelp::AdbToRockusbWait(const std::wstring& strDevPath, DWORD dwLayer, int retry)
    {
        bool bRet;
        int trys = 0;

        LogI_F(L"AdbToRockusbWait[%x]: switch to loader\r\n", dwLayer);
        do {
            bRet = AdbToRockusb(strDevPath);
            if (!bRet) {
                LogI_F(L"AdbToRockusbWait[%x]: switch to rockusb fail Path=%s\r\n",dwLayer, strDevPath.c_str());
                return FALSE;
            }
            bRet = (TRUE == RK_WaitDevice(dwLayer, RKUSB_LOADER));
            if (bRet) {
                LogI_F(L"AdbToRockusbWait[%x]: RK_WaitDevice RKUSB_LOADER okay\r\n", dwLayer);
                return TRUE;
            }
            LogI_F(L"AdbToRockusbWait[%x]: wait loader fail, retry %d Path=%s\r\n",dwLayer, trys, strDevPath.c_str());
            trys++;
        } while(trys < retry);
        return false;
    }

    bool CCJGW_RKDeviceHelp::SwitchLoader()
    {
		if (gdwLayerID < 0)
		{
			STRUCT_DEVICE_DESC_W *pDeviceDesc = NULL;
			pDeviceDesc = GetFirstDevicesLocked(RKUSB_ADB);
			if (NULL == pDeviceDesc)
			{
				pDeviceDesc = GetFirstDevicesLocked(RKUSB_LOADER);
			}

			if (NULL == pDeviceDesc) 
			{
				LogE(L"PreDeviceAccess: no device find, exit\r\n");
				return false;
			}
			memcpy(&m_currentDeviceDesc, pDeviceDesc, sizeof(m_currentDeviceDesc));
		}
		else
		{
			if (m_currentDeviceDesc.emUsbType != RKUSB_ADB && m_currentDeviceDesc.emUsbType != RKUSB_LOADER)
			{
				LogE(L"PreDeviceAccess: no device find, exit\r\n");
				return false;
			}
		}
        
        /* step 2 adb devices and switch to loader or maskrom */
        //! adb 模式切换为loader 或者maskrom模式
        if (m_currentDeviceDesc.emUsbType == RKUSB_ADB) {
            if (TRUE != AdbToRockusbWait(m_currentDeviceDesc.szLinkName, m_currentDeviceDesc.dwLayer, 3)) 
            {
                LogE_F(L"PreDeviceAccess[%x]: switch to loader fail\r\n", m_currentDeviceDesc.dwLayer);
                return false;
            }
            m_currentDeviceDesc.emUsbType = RKUSB_LOADER;
        }
        /* get device okay */

        return true;
    }
}
