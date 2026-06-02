#pragma once
#define TSE_FTM_MAX_READ_BUF 1024 * 4
/**
* 1:开始Keys测试
* 2:结束Keys测试
*/
#define TSE_FTM_START_KEY_TEST 1
#define TSE_FTM_END_KEY_TEST 2

#define TSE_FTM_DEVICE_PASS 1
#define TSE_FTM_DEVICE_FAIL -1

#define WIFI_AP_MODE 1
#define WIFI_STAT_MODE 2

namespace JGW
{
    enum E_TSE_FTM_KEY_STATUS
    {
        E_TSE_FTM_KEY_START_STATUS = 1,
        E_TSE_FTM_KEY_END_STATUS = 2
    };
    enum E_REQUEST_CODE
    {
        //! Version 信息
        E_REQ_VERSION_CODE = 0x01,
        //! Keys
        E_REQ_KEYS_CODE = 0x02,
        //! LCD Color
        E_REQ_LCD_COLOR_CODE = 0x03,
        //! LCD Back Light
        E_REQ_LCD_BACK_LIGHT_CODE = 0x04,
        //! Memory Card
        E_REQ_MEMORY_CARD_CODE = 0x05,
        //! SIM Detect
        E_REQ_SIM_CARD_INFO_CODE = 0x06,
        //! 呼吸灯
        E_REQ_BREATHING_LED_CODE = 0x07,
        //! RGB LED
        E_REQ_RGB_LED_CODE = 0x08,
        //! WIFIF
        E_REQ_WIFI_CODE = 0x09,
        //! WIGIG
        E_REQ_WIGIG_CODE = 0x0A,
        //! Ethernet
        E_REQ_ETHERNET_CODE = 0x0B,
        //! Battery & Char
        E_REQ_BATTERY_CHARGER_CODE = 0x0C,
        //! QC3.0 Test
        E_REQ_QC_3_CODE = 0x0D,
        //! Reboot
        E_REQ_REBOOT_CODE = 0x0E,
        //! Shutdown
        E_REQ_SHUTDOWN_CODE = 0x0F,
        //! AP热点扫描
        E_REQ_SCAN_AP_CODE = 0x10,
        //! WIFI模式切换
        E_REQ_WIFI_MODE = 0x11,
        //! 11AD 热点扫描
        E_REQ_WIGIG_SCAN_AP_CODE = 0x12,
        //! OTG测试请求
        E_REQ_OTG_CODE = 0x13
    };
    //! Socket连接成功
    const char MSG_CLINET_CONNECT_SUCCESS[] = "UNREQ_1";
    //! MEMORY CARD上报
    const char UNREQ_MEMORY_CARD_CODE[] = "UNREQ_100";
    //! SIM Detect
    const char UNREQ_SIM_CARD_INFO_CODE[] = "UNREQ_101";
}