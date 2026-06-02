#include "StdAfx.h"
#include "RabbitMQCHelp.h"
#include <stdarg.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <time.h>
#include <WinSock.h>
namespace JGW
{
	//inline std::string JGW_FormatString(std::string& strFormat,const char *szFormat,...)
	//{
	//    int nListCount = 0;

	//    {
	//        va_list pArgList;
	//        va_start(pArgList,szFormat);
	//        int nLength = _vscprintf(szFormat,pArgList) + 1;
	//        strFormat.resize(nLength);
	//        nListCount += _vsnprintf_s(const_cast<char *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

	//        if( -1 == nListCount ) return "buffer too small";

	//        va_end(pArgList);
	//    }

	//    strFormat.resize(nListCount);

	//    return strFormat;
	//}


	CRabbitMQCHelp::CRabbitMQCHelp(void) : mConnectionState(NULL),mSocket(NULL),mnChannelt(1),mnHeartbeat(60),mbIsConnect(true)
	{
	}


	CRabbitMQCHelp::~CRabbitMQCHelp(void)
	{
		Disconnect();
	}

	const std::string& CRabbitMQCHelp::GetErrorMessage()
	{
		return mstrErrorMessage;
	}

	void CRabbitMQCHelp::Disconnect()
	{
		if (NULL != mConnectionState)
		{
			mbIsConnect = false;
			amqp_connection_close(mConnectionState, AMQP_REPLY_SUCCESS);
			amqp_destroy_connection(mConnectionState);
			mConnectionState = NULL;
		}
	}

	bool CRabbitMQCHelp::IsConnected()
	{
		return mbIsConnect;
	}

	bool CRabbitMQCHelp::Connect(const std::string& strHostName,const std::string& strUserName,const std::string& strPassword,int nRMQSocketPort /* = AMQP_DEFAULT_SOCKET_PORT */)
	{
		mbIsConnect = false;
		mConnectionState = amqp_new_connection();
		if (NULL == mConnectionState)
		{
			mstrErrorMessage = "amqp new connection failed";
			return false;
		}

		mSocket = amqp_tcp_socket_new(mConnectionState);
		if (NULL == mSocket) 
		{
			mstrErrorMessage = "amqp tcp new socket failed.";
			return false;
		}
		//! amqp_socket_open_noblock(socket, hostname, port, tv),
		int status = amqp_socket_open(mSocket, strHostName.c_str(), nRMQSocketPort);
		if (status < 0) 
		{
			mstrErrorMessage = "amqp socket open failed.";
			return false;
		}
		//! amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,"guest", "guest"),
		if (!GetAmpRpcErrorMsg(amqp_login(mConnectionState, AMQP_DEFAULT_VHOST, AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE, 0,AMQP_SASL_METHOD_PLAIN, strUserName.c_str(), strPassword.c_str()), "Logging in")) 
		{
			return false;
		}

		amqp_channel_open(mConnectionState, mnChannelt);
		if (!GetAmpRpcErrorMsg(amqp_get_rpc_reply(mConnectionState), "open channel for consumer.")) 
		{
			return false;
		}
		mbIsConnect = true;
		return true;
	}

	bool CRabbitMQCHelp::ExchangeQueueBind(const std::string& strExchange,const std::string& strRoutingKey,const std::string& strType /* = "direct" */)
	{
		amqp_exchange_declare(
			mConnectionState,                // AMQP 连接对象
			mnChannelt,                   // 通道号
			amqp_cstring_bytes(strExchange.c_str()),  // 交换机名称
			amqp_cstring_bytes(strType.c_str()),       // 交换机类型
			0,                   // passive 模式（仅检查存在性）
			1,                   // durable 持久化
			0,                   // auto_delete 自动删除
			0,                   // internal 内部交换机
			amqp_empty_table     // arguments 扩展参数
			);
		// 检查声明结果
		if (!GetAmpRpcErrorMsg(amqp_get_rpc_reply(mConnectionState), "Declaring exchange")) 
		{
			return false;
		}

		amqp_queue_declare_ok_t *reply = amqp_queue_declare(
			mConnectionState,        // AMQP 连接对象
			mnChannelt,                   // 通道号
			amqp_empty_bytes,  // 队列名称
			0,                   // passive=0（主动创建）
			1,                   // durable=1（持久化）
			0,                   // exclusive=0（非独占）
			0,                   // auto_delete=0（不自动删除）
			amqp_empty_table     // arguments（无扩展参数）
			);
		if (!GetAmpRpcErrorMsg(amqp_get_rpc_reply(mConnectionState), "Queue declare")) 
		{
			return false;
		}

		// 获取服务器生成的队列名称（如传递空名称时）
		//amqp_bytes_t queue_name = reply->queue;
		mstrQueueName = std::string((char*)reply->queue.bytes,(char*)reply->queue.bytes + reply->queue.len);

		amqp_queue_bind(
			mConnectionState,                  // AMQP 连接对象
			mnChannelt,                     // 通道号
			amqp_cstring_bytes(mstrQueueName.c_str()),  // 队列名称
			amqp_cstring_bytes(strExchange.c_str()),  // 交换机名称
			amqp_cstring_bytes(strRoutingKey.c_str()),     // 路由键（通配符）
			amqp_empty_table       // 扩展参数（可选）
			);
		if (!GetAmpRpcErrorMsg(amqp_get_rpc_reply(mConnectionState), "Queue binding")) 
		{
			return false;
		}

		return true;
	}

	bool CRabbitMQCHelp::Consume(bool bAck /* = true */,bool bExclusive /* = false */,const std::string& strConsumerTag /* = "" */)
	{
		//! amqp_connection_state_t state, amqp_channel_t channel, amqp_bytes_t queue,
		//amqp_bytes_t consumer_tag, amqp_boolean_t no_local, amqp_boolean_t no_ack,
		//   amqp_boolean_t exclusive, amqp_table_t arguments

		// 订阅队列 "error_logs"，启用手动确认模式
		amqp_basic_consume(
			mConnectionState,                           // AMQP 连接对象
			mnChannelt,                              // 通道号
			amqp_cstring_bytes(mstrQueueName.c_str()), // 队列名称
			strConsumerTag.empty() ? amqp_empty_bytes : amqp_cstring_bytes(strConsumerTag.c_str()),               // consumer_tag（自动生成）
			0,                              // no_local（不支持该特性）
			bAck ? 1 : 0,                              // no_ack 1 自动确认模式，消息投递后立即删除；0：需手动调用 
			bExclusive ? 1 : 0,                              // exclusive=0（非独占）1：独占消费，其他消费者无法连接该队列；0：允许多消费者共享队列
			amqp_empty_table               // arguments（无扩展参数）
			);
		if (!GetAmpRpcErrorMsg(amqp_get_rpc_reply(mConnectionState), "Consuming")) 
		{
			return false;
		}

		return true;
	}

	bool CRabbitMQCHelp::Publish(const std::string& strMessage, const std::string& strExchange, const std::string& strRouteKey)
	{
		amqp_basic_properties_t props;
		props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
		props.content_type = amqp_cstring_bytes("text/plain");
		props.content_encoding = amqp_cstring_bytes("utf-8");
		props.delivery_mode = 2; /* persistent delivery mode */
		amqp_basic_publish(mConnectionState, mnChannelt, amqp_cstring_bytes(strExchange.c_str()),
			amqp_cstring_bytes(strRouteKey.c_str()), 0, 0,
			&props, amqp_cstring_bytes(strMessage.c_str()));
		if (!GetAmpRpcErrorMsg(amqp_get_rpc_reply(mConnectionState), "Publishing ")) 
		{
			return false;
		}

		return true;
	}

	void CRabbitMQCHelp::ConsumeMessageAsync(void (*RecvMessageFunc)(const std::string& strMessage))
	{
		amqp_rpc_reply_t ret;
		amqp_frame_t frame;
		amqp_envelope_t envelope;
		struct timeval tv = {0, 100}; // 1秒超时

		amqp_maybe_release_buffers(mConnectionState);
		ret = amqp_consume_message(mConnectionState, &envelope, &tv, 0);

		if (AMQP_RESPONSE_NORMAL == ret.reply_type) 
		{
			std::string strResult((char*)envelope.message.body.bytes,(char*)envelope.message.body.bytes + envelope.message.body.len);
			RecvMessageFunc(strResult);
			amqp_destroy_envelope(&envelope);
		}
		else
		{
			if (AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type && AMQP_STATUS_UNEXPECTED_STATE == ret.library_error) 
			{
				if (AMQP_STATUS_OK != amqp_simple_wait_frame(mConnectionState, &frame)) 
				{
					return;
				}

				if (AMQP_FRAME_METHOD == frame.frame_type) 
				{
					switch (frame.payload.method.id) 
					{
					case AMQP_BASIC_ACK_METHOD:
						/* if we've turned publisher confirms on, and we've published a
						* message here is a message being confirmed.
						*/
						break;
					case AMQP_BASIC_RETURN_METHOD:
						/* if a published message couldn't be routed and the mandatory
						* flag was set this is what would be returned. The message then
						* needs to be read.
						*/
						{
							amqp_message_t message;
							ret = amqp_read_message(mConnectionState, frame.channel, &message, 0);
							if (AMQP_RESPONSE_NORMAL != ret.reply_type) 
							{
								return;
							}

							amqp_destroy_message(&message);
						}

						break;

					case AMQP_CHANNEL_CLOSE_METHOD:
						/* a channel.close method happens when a channel exception occurs,
						* this can happen by publishing to an exchange that doesn't exist
						* for example.
						*
						* In this case you would need to open another channel redeclare
						* any queues that were declared auto-delete, and restart any
						* consumers that were attached to the previous channel.
						*/
						break;

					case AMQP_CONNECTION_CLOSE_METHOD:
						/* a connection.close method happens when a connection exception
						* occurs, this can happen by trying to use a channel that isn't
						* open for example.
						*
						* In this case the whole connection must be restarted.
						*/
						return;

					default:
						JGW_FormatString(mstrErrorMessage,"An unexpected method was received %u\n",
							frame.payload.method.id);
						return;
					}
				}
			}
		} 
	}

	void CRabbitMQCHelp::ConsumeMessage(void (*RecvMessageFunc)(const std::string& strMessage))
	{
		amqp_rpc_reply_t ret;
		amqp_envelope_t envelope;
		amqp_frame_t frame;

		for (;;) 
		{
			amqp_maybe_release_buffers(mConnectionState);
			ret = amqp_consume_message(mConnectionState, &envelope, NULL, 0);


			if (AMQP_RESPONSE_NORMAL == ret.reply_type) 
			{
				std::string strResult((char*)envelope.message.body.bytes,(char*)envelope.message.body.bytes + envelope.message.body.len);
				RecvMessageFunc(strResult);
				amqp_destroy_envelope(&envelope);
			}
			else
			{
				if (AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type && AMQP_STATUS_UNEXPECTED_STATE == ret.library_error) 
				{
					if (AMQP_STATUS_OK != amqp_simple_wait_frame(mConnectionState, &frame)) 
					{
						break;
					}

					if (AMQP_FRAME_METHOD == frame.frame_type) 
					{
						switch (frame.payload.method.id) 
						{
						case AMQP_BASIC_ACK_METHOD:
							/* if we've turned publisher confirms on, and we've published a
							* message here is a message being confirmed.
							*/
							break;
						case AMQP_BASIC_RETURN_METHOD:
							/* if a published message couldn't be routed and the mandatory
							* flag was set this is what would be returned. The message then
							* needs to be read.
							*/
							{
								amqp_message_t message;
								ret = amqp_read_message(mConnectionState, frame.channel, &message, 0);
								if (AMQP_RESPONSE_NORMAL != ret.reply_type) 
								{
									break;
								}

								amqp_destroy_message(&message);
							}

							break;

						case AMQP_CHANNEL_CLOSE_METHOD:
							/* a channel.close method happens when a channel exception occurs,
							* this can happen by publishing to an exchange that doesn't exist
							* for example.
							*
							* In this case you would need to open another channel redeclare
							* any queues that were declared auto-delete, and restart any
							* consumers that were attached to the previous channel.
							*/
							break;

						case AMQP_CONNECTION_CLOSE_METHOD:
							/* a connection.close method happens when a connection exception
							* occurs, this can happen by trying to use a channel that isn't
							* open for example.
							*
							* In this case the whole connection must be restarted.
							*/
							return;

						default:
							JGW_FormatString(mstrErrorMessage,"An unexpected method was received %u\n",
								frame.payload.method.id);
							break;
						}
					}
				}
			} 
		}

		Disconnect();
	}


	bool CRabbitMQCHelp::GetAmpRpcErrorMsg(amqp_rpc_reply_t x, char const *context)
	{
		switch (x.reply_type)
		{
		case AMQP_RESPONSE_NORMAL:
			return true;
		case AMQP_RESPONSE_NONE:
			JGW_FormatString(mstrErrorMessage,"%s: missing RPC reply type!", context);
			break;
		case AMQP_RESPONSE_LIBRARY_EXCEPTION:
			JGW_FormatString(mstrErrorMessage,"%s: %s\n", context, amqp_error_string2(x.library_error));
			break;
		case AMQP_RESPONSE_SERVER_EXCEPTION:
			switch (x.reply.id) 
			{
			case AMQP_CONNECTION_CLOSE_METHOD: 
				{
					amqp_connection_close_t *m = (amqp_connection_close_t *)x.reply.decoded;
					JGW_FormatString(mstrErrorMessage,"%s: server connection error %uh, message: %.*s\n",
						context, m->reply_code, (int)m->reply_text.len,
						(char *)m->reply_text.bytes);
					break;                             
				}
			case AMQP_CHANNEL_CLOSE_METHOD: 
				{
					amqp_channel_close_t *m = (amqp_channel_close_t *)x.reply.decoded;
					JGW_FormatString(mstrErrorMessage,"%s: server channel error %uh, message: %.*s\n",
						context, m->reply_code, (int)m->reply_text.len,
						(char *)m->reply_text.bytes);
					break;                             
				}
			default:
				JGW_FormatString(mstrErrorMessage,"%s: unknown server error, method id 0x%08X\n",
					context, x.reply.id);
				break;
			}
			break;
		}
		return false;
	}
}

