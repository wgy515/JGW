#pragma once
#include <string>
namespace JGW
{
    class CJGW_CSharpCommunicationBase
    {
    public:
        //! 创建通讯通路
        virtual bool Create(const std::string& strHost,size_t port) = 0;
        //! 连接通讯通路
        virtual bool Connect() = 0;
        //! 
        virtual bool IsConnect() = 0;
        //! 读
        virtual int Read(char* buf,size_t len) = 0;
        //! 写
        virtual bool Write(char* buf,size_t len) = 0;
        //! 断开连接
        virtual void DisConnect() = 0;
    };
}