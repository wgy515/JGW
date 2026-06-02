#pragma once
#include <vector>
#include <string>
// #include <ifmib.h>
// #include <IPTypes.h>
#include <iphlpapi.h>
namespace JGW
{
    //保存一个网络连接信息
    struct NetWorkConection
    {
        int index;			//该连接在MIB_IFTABLE中的索引
        std::string description;		//网络描述
        DWORD in_bytes;	//初始时已接收字节数
        DWORD out_bytes;	//初始时已发送字节数
        std::wstring ip_address;		//IP地址
        std::wstring subnet_mask;	//子网掩码
        std::wstring default_gateway;	//默认网关
    };

    class CAdapterCommon
    {
    public:
        CAdapterCommon();
        ~CAdapterCommon();

        //获取网络连接列表，填充网络描述、IP地址、子网掩码、默认网关信息
        static void GetAdapterInfo(std::vector<NetWorkConection>& adapters);

        //获取网络列表中每个网络连接的MIB_IFTABLE中的索引、初始时已接收/发送字节数的信息
        static void GetIfTableInfo(std::vector<NetWorkConection>& adapters, MIB_IFTABLE* pIfTable);
    private:
        //根据一个网络连接描述判断是否在网络连接列表里，返回索引，找不到则返回-1
        static int FindConnectionInAdapterList(std::string connection, const std::vector<NetWorkConection>& adapters);
    };
}