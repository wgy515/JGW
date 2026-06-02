#pragma once
#include <Poco/Net/TCPServer.h>
#include <memory>

namespace JGW
{
	class CCJGW_PrsVendorRoamingBroadcast
	{
	public:
		CCJGW_PrsVendorRoamingBroadcast(void);
		~CCJGW_PrsVendorRoamingBroadcast(void);
	public:
		bool StartPrsVendorRoamingServer(Poco::UInt16 nSocketPort);
		bool StopPrsVendorRoamingServer();
	private:
		SOCKET _sockfd;
	};
}


