#pragma once
#include <string>
#include <amqp.h>
#include <amqp_tcp_socket.h>

#define AMQP_DEFAULT_SOCKET_PORT 5672

namespace JGW
{
    class CRabbitMQCHelp
    {
    public:
        CRabbitMQCHelp(void);
        ~CRabbitMQCHelp(void);
    public:
        //! Consume => Connect => ExchangeQueueBind => Consume => ConsumeMessage
        //! Publish => Connect => Publish
        //! RabbitMQ连接
        bool Connect(const std::string& strHostName,const std::string& strUserName,const std::string& strPassword,int nRMQSocketPort = AMQP_DEFAULT_SOCKET_PORT);
        //! 声明一个 direct 类型的持久化交换机 exchange amqp_exchange_declare （direct/topic/fanout）
        bool ExchangeQueueBind(const std::string& strExchange,const std::string& strRoutingKey,const std::string& strType = "direct");
        //! consume no_ack 1 自动确认模式，消息投递后立即删除；0：需手动调用  exclusive=0（非独占）1：独占消费，其他消费者无法连接该队列；0：允许多消费者共享队列
        bool Consume(bool bAck = true,bool bExclusive = false,const std::string& strConsumerTag = "");
        //1 consume_message
        void ConsumeMessage(void (*RecvMessageFunc)(const std::string& strMessage));
        //! 
        bool Publish(const std::string& strMessage, const std::string& strExchange, const std::string& strRouteKey); 
        //! 断开RabbitMQ连接
        void Disconnect();
		//!
		bool IsConnected();
    private:
        //! 
        bool GetAmpRpcErrorMsg(amqp_rpc_reply_t x, char const *context);
    public:
        const std::string& GetErrorMessage();
    private:
		bool mbIsConnect;
        //! 0 : 禁用心跳 > 0 : 心跳秒数
        //! 心跳间隔设置优化连接稳定性 60
        int mnHeartbeat;
        //! 默认用1号通道，通道无所谓
        amqp_channel_t mnChannelt;
        amqp_connection_state_t mConnectionState;
        amqp_socket_t* mSocket;
        std::string mstrQueueName;
        std::string mstrErrorMessage;
    };
}


