#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace JGW
{
    //! 上报当前设备信息状态
    class CCJGW_RepStatusJson
    {
    public:
        CCJGW_RepStatusJson(void);
        ~CCJGW_RepStatusJson(void);
    public:
        std::string BuildStatusJson();
        //! 解析JSON字符串
        void FromJsonToString(const std::string& strJson);
    public:
        int requestCode; //! 请求码 默认1001
        int class_num; //！身份级别。Windows为0级、linux根据下级数目实时计算
        int rssi; //! 本机信号强度
        int tx_mcs; //! 发送mcs
        int rx_mcs;//! 接收mcs
        int tx_sector; //!
        int rx_sector;//! 
        float tx_drop_rate;//! 发送丢包率 double
        float rx_drop_rate;//! 接收丢包率 double
        std::string mac; //! Dongle mac地址
        std::string ip; //! Dongle ip地址
        std::string tx_speed; //! TX速率
        std::string rx_speed;//! RX速率
    };
}