#include "StdAfx.h"
#include "CJGW_PSRabbitMQ.h"

namespace JGW
{
	std::string CCJGW_PSRabbitMQ::mstrRoomID;
	E_TAG_RMQ CCJGW_PSRabbitMQ::meTagRMQ = TAG_RMQ_UNKNOWN;

	CCJGW_PSRabbitMQ::CCJGW_PSRabbitMQ(void) : mbConnectedRabbitMQ(false),mbDeclaredRabbitMQ(false),mbIsConsume(false),mpRecvMessageFunc(NULL)
	{

	}


	CCJGW_PSRabbitMQ::~CCJGW_PSRabbitMQ(void)
	{
	}

	void CCJGW_PSRabbitMQ::AddRabbitMQRoomInfo(E_TAG_RMQ eTagRMQ,const std::string& strRoomID)
	{
		mstrRoomID = strRoomID;
		meTagRMQ = eTagRMQ;
	}

	bool CCJGW_PSRabbitMQ::StartRabbitMQThread(LPRecvMessageFunc pRecvMessageFunc)
	{
		mpRecvMessageFunc = pRecvMessageFunc;
		if (_thread.isRunning()) return true;
		try
		{
			_thread.start(*this);
			return true;
		}
		catch (Poco::Exception& )
		{
		}
		return false;
	}

	CCJGW_PSRabbitMQ& CCJGW_PSRabbitMQ::GetPSRabbitMQInstance()
	{
		static CCJGW_PSRabbitMQ gPSRabbitMQ;
		return gPSRabbitMQ;
	}

	E_TAG_RMQ CCJGW_PSRabbitMQ::GetTagRmq()
	{
		return meTagRMQ;
	}

	void CCJGW_PSRabbitMQ::SendRabbitMQMessage(const std::string& strMessage)
	{
		CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
		mqueueMsg.push(strMessage);
	}

	void CCJGW_PSRabbitMQ::run()
	{
		while (true)
		{
			// 观察者模式不进行连接操作
			if (TAG_RMQ_VIEW == meTagRMQ || mstrRoomID.empty()) 
			{
				Sleep(100);
				continue;
			}
			//! 开始连接RABBIT MQ
			if (!mbConnectedRabbitMQ || !mcRabbitMQCHelp.IsConnected())
			{
				mbConnectedRabbitMQ = false;
				mbDeclaredRabbitMQ = false;
				mbConnectedRabbitMQ = mcRabbitMQCHelp.Connect(HOST_RABBITMQ,USER_NAME_RABBITMQ,PASSWORD_RABBITMQ);
				if (!mbConnectedRabbitMQ)
				{
					Sleep(100);
					continue;
				}
			}

			if (!mbDeclaredRabbitMQ)
			{
				mbDeclaredRabbitMQ = mcRabbitMQCHelp.ExchangeQueueBind(LOCATION_EXCHANGE,mstrRoomID);
				if (!mbDeclaredRabbitMQ)
				{
					Sleep(100);
					mcRabbitMQCHelp.Disconnect();
					mbConnectedRabbitMQ = false;
					continue;
				}
			}

			if (!mbIsConsume)
			{
				mbIsConsume = mcRabbitMQCHelp.Consume();
				if (!mbIsConsume)
				{
					Sleep(100);
					mcRabbitMQCHelp.Disconnect();
					mbConnectedRabbitMQ = false;
					mbDeclaredRabbitMQ = false;
					continue;
				}
			}

			while (!mqueueMsg.empty())
			{
				std::string strMsg;
				{
					CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
					strMsg = mqueueMsg.front();
					
				}
				try
				{
					if (!mcRabbitMQCHelp.Publish(strMsg,LOCATION_EXCHANGE,mstrRoomID))
					{
						Sleep(100);
						mcRabbitMQCHelp.Disconnect();
						mbConnectedRabbitMQ = false;
						mbDeclaredRabbitMQ = false;
						continue;
					}
					{
						CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
						mqueueMsg.pop();
					}
				}
				catch (...)
				{
					
				}
			}
			mcRabbitMQCHelp.ConsumeMessageAsync(mpRecvMessageFunc);
		}
	}
}
