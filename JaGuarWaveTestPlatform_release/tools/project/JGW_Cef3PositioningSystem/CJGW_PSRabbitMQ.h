#pragma once
#include <string>
#include <Poco/Thread.h>
#include "RMQ/RabbitMQCHelp.h"
#include "JGW_Cef3PositioningSystem.h"
#include <queue>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
	typedef void (*LPRecvMessageFunc)(const std::string& strMessage);
	class CCJGW_PSRabbitMQ : public Poco::Runnable
	{
	private:
		CCJGW_PSRabbitMQ(void);
		~CCJGW_PSRabbitMQ(void);
	public:
		static void AddRabbitMQRoomInfo(E_TAG_RMQ eTagRMQ,const std::string& strRoomID);
		static CCJGW_PSRabbitMQ& GetPSRabbitMQInstance();
		static E_TAG_RMQ GetTagRmq();
	public:
		//! ¿ªÆôRMQÏß³Ì
		bool StartRabbitMQThread(LPRecvMessageFunc pRecvMessageFunc);
		//! 
		void SendRabbitMQMessage(const std::string& strMessage);
	private:
		void run();
		/// Does the work.
	private:
		bool mbIsConsume;
		bool mbDeclaredRabbitMQ;
		bool mbConnectedRabbitMQ;
		LPRecvMessageFunc mpRecvMessageFunc;
		static E_TAG_RMQ meTagRMQ;
		static std::string mstrRoomID;
		std::queue<std::string> mqueueMsg;
		Poco::Thread _thread;
		CRabbitMQCHelp mcRabbitMQCHelp;
		CCJGW_CriticalSectionLock mcCriticalSectionLock;
	};
}

