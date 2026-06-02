#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>
namespace JGW
{
    /*
    0 成功
    1 操作失败
    10 当前测试全部PASS
    11 当前测试有FAIL
    12 当前测试ASSERT
    13 当前测试被手动停止了
    15 当前测试完成
    20 读取WIFI MAC成功
    23 写入WIFI MAC成功
    201-219 为读写MAC的失败原因
    201 means 要写入的WIFI MAC 长度错误
    202 means 要写入的WIFI MAC 有非法字符
    203 means 要写入的WIFI MAC 为全0
    204 means WIFI MAC EFUSE 无法读取
    205 means WIFI MAC EFUSE 已经存在不可写入
    206 means WIFI MAC 写入后读为0 错误
    207 means WIFI MAC 写入后读取数值和写入的不一致
    208 means WIFI MAC EFUSE 没有保存过MAC地址
    215 means WIFI MAC EFUSE 已经存在不可写入
    216 means WIFI MAC 写入后读为0 错误
    219 means CFT测试未通过,无法写入MAC地址
    220 means没有使能EFUSE写入,无法写入MAC地址
    221 means DUT未进行成功校准
    222 means WIFI MAC地址读取长度错误
    */
    typedef enum
    {
        //! Successful 0 成功
        E_XWGAUGE_SUCCESSFUL = 0,
        //! Operation failure 1 操作失败
        E_XWGAUGE_OPERATION_FAILURE = 1,
        //! All current tests PASS 10 当前测试全部PASS
        E_XWGAUGE_ALL_TEST_PASS = 10,
        //! The current test fails 11 当前测试有FAIL
        E_XWGAUGE_TEST_FAIL = 11,
        //! 当前测试ASSERT 12 当前测试ASSERT
        E_XWGAUGE_TEST_ASSERT = 12,
        //! The current test was manually stopped 13 当前测试被手动停止了
        E_XWGAUGE_TEST_MANUALLY_STOPPED = 13,
        //! Current test complete 当前测试完成
        E_XWGAUGE_TEST_COMPLETE = 15,
        //! Read WIFI MAC successfully 读取WIFI MAC成功
        E_XWGAUGE_READ_WIFI_MAC_SUCCESSFULLY = 20,
        //! 写入WIFI MAC成功 写入WIFI MAC成功
        E_XWGAUGE_WRITE_WIFI_MAC_SUCCESSFULLY = 23,
        //! 201 means 要写入的WIFI MAC 长度错误
        E_XWGAUGE_WRITE_WIFI_MAC_LEMNGTH_ERROR = 201,
        //! Illegal character 202 means 要写入的WIFI MAC 有非法字符
        E_XWGAUGE_WRITE_WIFI_MAC_ILLEGAL_CHAR_ERROR = 202,
        //! 203 means 要写入的WIFI MAC 为全0
        E_XWGAUGE_WRITE_WIFI_MAC_ALL_ZERO_ERROR = 203,
        //! unreadable  204 means WIFI MAC EFUSE 无法读取
        E_XWGAUGE_WIFI_MAC_EFUSE_UNREADABLE_ERROR = 204,
        //205 means WIFI MAC EFUSE 已经存在不可写入
        E_XWGAUGE_WIFI_MAC_EFUSE_NOT_WRITE_ERROR = 205,
        //206 means WIFI MAC 写入后读为0 错误 Error reading 0 after write
        E_XWGAUGE_WIFI_MAC_ERROR_READING = 206,
        //207 means WIFI MAC 写入后读取数值和写入的不一致 The read value after the write is inconsistent with that of the write
        E_XWGAUGE_WIFI_MAC_READ_WRITE_NOT_EQUAL = 207,
        //208 means WIFI MAC EFUSE 没有保存过MAC地址
        E_XWGAUGE_WIFI_MAC_EFUSE_NOT_SAVE_MAC_ADDR = 208,
        //215 means WIFI MAC EFUSE 已经存在不可写入
        E_XWGAUGE_WIFI_MAC_EFUSE_NOT_WRITE_ERROR2 = 215,
        //216 means WIFI MAC 写入后读为0 错误
        E_XWGAUGE_WIFI_MAC_ERROR_READING2 = 216,
        //219 means CFT测试未通过,无法写入MAC地址
        E_XWGAUGE_WIFI_MAC_CFT_TEST_FAIL_NOT_WRITE_MAC = 219,
        //220 means没有使能EFUSE写入,无法写入MAC地址
        E_XWGAUGE_NOT_ENABLE_EFUSE_NOT_WRITE_MAC = 220,
        //221 means DUT未进行成功校准
        E_XWGAUGE_WIFI_NOT_CALIBRATE = 221,
        //222 means WIFI MAC地址读取长度错误
        E_XWGAUGE_WIFI_MAC_READ_LENGTH_ERROR = 222
    } E_XWGAUGE_ERROR_CODE;


    /*
    CONF INIT
    -1|0|OK
    CONF READ 0
    -1|0|OK
    START READ
    -1|0|OK
    0|13|连接测试仪表失败!
    -1|15|STOP
    */
    class CTSE_XWGaugeAsyncSocket : public CTSE_TestBase
    {
    public:
        CTSE_XWGaugeAsyncSocket(void);
        ~CTSE_XWGaugeAsyncSocket(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        int mnCheckResultTimeOutSec;
        std::wstring mstrConnectAsyncSocketInstanceEnvironment;
    };
}
