#pragma once
#include <string>
#include <HPSocket/HPTypeDef.h>
#define DonglesThroughputTestConfigName "DonglesThroughputTest.xml"
//! 20000 = 2度  每一个角度 = 10000转  //! 3600000 每周
#define PER_ANGLE_TURN 10000
#define TURNTABLE_NODE_ID 32

namespace UiLib
{
    class CCJGW_AsyncWndMessage;
}
namespace JGW
{
    enum E_SOCKET_TERIMALTYPE
    {
        E_CLIENT_TYPE = 0,
        E_SERVICE_TYPE
    };

    enum E_Transceivers_TYPE
    {
        E_RECV_TYPE = 0,
        E_SEND_TYPE
    };

    enum E_TEST_STATUS_TYPE
    {
        E_TEST_IDEL,
        E_TEST_BUSY,
        E_TEST_PASS,
        E_TEST_FAIL
    };
    
    //! 默认服务端接收，客户端发送
    struct S_ThroughputTest_Confige
    {
        E_SOCKET_TERIMALTYPE meSocketTerimalType; //! 客户端 or 服务端
        std::wstring mstrIPAddress; //! 客户IP地址
        unsigned short mnSocketPort; //! socket端口
        E_Transceivers_TYPE meTranservicesType; //! 接收端 or 发射端
        /* 客户端配置 */
        size_t mnTestTimes; //! 测试时间
        size_t mnSocketCount; //! socket数量
        size_t mnDataLength; //! 发送数据长度
        size_t mnIntervTime;
        /* 服务端配置 */
        EnSendPolicy meSendPolicy; //! 默认SP_PACK  打包模式 当服务端为发射端才有效
        size_t mnServerThreadCount; //! 服务端线程数量
        size_t mnServerMaxConnSocketCount; //! 服务端最大SOCKET连接数量
        //! TurnTable
        bool mbEnableTurnTable; //! 是否开启转台
        int mTurnTableStep; //! 转台步进
        double mfVelocity; //! 周转速度
        double mfAcceleration; //! 加速度
        double mfDeceleration; //! 减速度

        int mTurnTableAngleStart;
        int mTurnTableAngleEnd;

        bool mbReadDonglesSN;

        std::wstring mstrHtmlFile;
        UiLib::CCJGW_AsyncWndMessage* mpAsyncWndMessage;
    };
}