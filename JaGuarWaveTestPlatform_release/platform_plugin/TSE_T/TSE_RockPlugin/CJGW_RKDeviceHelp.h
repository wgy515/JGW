#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "RKUpgradeDll.h"
#include "DevicesAPI.h"

#define RKADBBUFSIZE 4096
#define VENDOR_MAX_LEN 4096

typedef bool (*PFITEMCHECK)(const TCHAR *str, unsigned char  flag, void *data);
//! (IN)dest			    vendor(0) or rpmb(1)
#define DEST_VENDOR 0
#define DEST_RPMB 1

///////////////////////////////////////////
//! barcode主板号
#define VENDOR_BARCODE_ID 51
//! FT测试记录
#define VENDOR_FT_FLAG 52
//! 电量
#define VENDOR_BATTERY_ID 53
//! DDR大小
#define VENDOR_DDR 54
//! EMMC大小
#define VENDOR_EMMC 55
//! 底包固件版本
#define VENDOR_FIRMWARE_VERSION 56
//! CPU型号
#define VENDOR_CPU 57
//! 作业帮固件版本预留
#define VENDOR_ZYB_VERSION 59



#define ITEM_BARCODE_ID 51
#define ITEM_FT_FLAG 52
#define ITEM_BATTERY_ID 53
#define ITEM_DDR 54
#define ITEM_EMMC 55
#define ITEM_FIRMWARE_VERSION 56
#define ITEM_CPU 57
#define ITEM_ZYB_VERSION 59




//////////////////////////////////////////

#define MASK_VENDOR_ID 0XFFFF
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

#define TA_IDATTE       1

#define MASK_FIXED_ID (1 << 31)
#define FIXED_ID (1 << 31)

#define MASK_RO (1 << 30)
#define TYPE_RO (1 << 30)
#define MASK_WO (1 << 29)
#define TYPE_WO (1 << 29)

#define MASK_DYN (1 << 28)
#define TYPE_DYN (1 << 28)

/* File type in file mode */
#define TYPE_NON -1
#define TYPE_TXT 0
#define TYPE_BIN 1

/* the string type in device */
#define TYPE_STR 0
#define TYPE_DEC 1
#define TYPE_HEX 2

/* avb  command */
#define AT_PERM_ATTR_FUSE           1
#define AT_RSA_PERM_ATTR_FUSE       2
#define AT_LOCK_VBOOT               3
#define AT_LOCK_STATE               4
#define PERM_ATTR_TOTAL_SIZE        1052
#define RSA_PERM_ATTR_TOTAL_SIZE    256
#define TICIPHER_LEN 32


/* all item decsript in thie tool is
 * referenced by this index
 * change to device vendor id using: INDEX_ID_MAP[index]::id
 */
/* all item decsript in thie tool is
 * referenced by this index
 * change to device vendor id using: INDEX_ID_MAP[index]::id
 */

 /*important: common items must be in continuous, and end with CM_ITEM_COUNT
  */
#define ITEM_START 0
#define ITEM_SN 0
#define ITEM_WIFI 1
#define ITEM_LAN 2
#define ITEM_BT 3
#define ITEM_IMEI 4
#define ITEM_FIX 5
#define ITEM_CUST1 5
#define ITEM_CUST2 6
#define CM_ITEM_COUNT 7

/* ITEM_ID_ATTE must define after CM_ITEM_COUNT, because in 
 * Crk_provision_toolDlg::GetItemetItem ->GetIDAtte is depends on SN has got
 */
#define ITEM_ID_ATTE 15
#define ITEM_ATTENTION 16
#define ITEM_WIDEVINE 17
#define ITEM_ID_RKP 18
/*hdcp grp */
#define ITEM_HDCP_START 20
#define ITEM_HDCP14_HDMITX 20
#define ITEM_HDCP14DP 21
#define ITEM_HDCP2XWFD 22
#define ITEM_HDCP2X_HDMIRX 23
#define ITEM_HDCP14_HDMIRX 24
#define ITEM_HDCP_END 25
#define ITEM_AVB_ATTR 25
#define ITEM_AVB_ATTR_CER 26
#define ITEM_AVB_LOCK 27
#define ITEM_AVB_STATE 28
#define ITEM_AVB_END 29
#define ITEM_TACIPHER 29
#define ITEM_OEMHUK 30
#define ITEM_END 30
#define ITEM_COUNT 31

#define IMEI_LEN 15
#define MAC_LEN 6
#define ITEM_TEXT_MAX 128
#define ITEM_STEP_MAX 9

#define ITEM_PLAYREADY_CERT 14
#define ITEM_PLAYREADY_SL2000 15
#define ITEM_PLAYREADY_SL3000 16

#define TYPE_ITEM_CM 7
#define TYPE_ITEM_KEYBOX 8
#define TYPE_ITEM_IDATTE 9
#define TYPE_ITEM_RKP 10
#define TYPE_ITEM_HDCP 11
#define TYPE_ITEM_AVB 12
#define TYPE_ITEM_SECKEYS 13

typedef struct _ID_MAP2_ {
    int item_id;
    const TCHAR *name;
    unsigned int id;
    int str_int_hex; /* type 0:str 1:decimal 2:hex*/
    PFITEMCHECK check;
} ID_MAP2;

namespace JGW
{
    class CCJGW_RKDeviceHelp
    {
    public:
        CCJGW_RKDeviceHelp(void);
        ~CCJGW_RKDeviceHelp(void);
    public:
        //! dwLayerID 不同USB端口LAYER ID不一致
		static int GetRKDeviceInfoToLayerID(DWORD dwLayerID);
        static int ScanRKDevice();
        static bool SwitchLoader();
        static int GetStringIntHexToVendorID(int nVendorID,const ID_MAP2 *m);
    private:
        static bool EnterLoader(DWORD dwLayer);
        static STRUCT_DEVICE_DESC_W* GetFirstDevicesLocked(unsigned int type);
        static bool AdbToRockusbWait(const std::wstring& strDevPath, DWORD dwLayer, int retry);
        static bool AdbToRockusb(const std::wstring& strDevPath);
    public:
        static RKDContextHandle mhRKDContextHandle;
        static int m_nDeviceCount;
        static STRUCT_DEVICE_DESC_W m_currentDeviceDesc;
        static STRUCT_DEVICE_DESC_W m_deviceSet_W[MAX_DEVICE];
    };
    extern int _get_vendorid(int uiid, const ID_MAP2 *m);
    extern const ID_MAP2 *get_id_map(int item_id);
    extern BOOL _is_writeonly(int uiid, const ID_MAP2 *m);
    extern BOOL _is_readonly(int uiid, const ID_MAP2 *m);
}

