#include "StdAfx.h"
#include "CJGW_PrsVendorRoamingBroadcast.h"
#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>
#include <poco/Net/SocketImpl.h>

namespace JGW
{
	void RecvFromCallback(void* lpParam)
	{

	}


	CCJGW_PrsVendorRoamingBroadcast::CCJGW_PrsVendorRoamingBroadcast(void)
	{
	}


	CCJGW_PrsVendorRoamingBroadcast::~CCJGW_PrsVendorRoamingBroadcast(void)
	{
	}
	
	bool CCJGW_PrsVendorRoamingBroadcast::StartPrsVendorRoamingServer(Poco::UInt16 nSocketPort)
	{
		/*

		#include <winsock2.h>
		#include <ws2tcpip.h>
		#pragma comment(lib, "Ws2_32.lib")

		int main() {
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);

		// 创建原始套接字（IPPROTO_RAW 表示自行构造IP头）
		SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
		if (sock == INVALID_SOCKET) {
		printf("Socket error: %d\n", WSAGetLastError());
		return 1;
		}

		// 启用IP_HDRINCL选项以自定义IP头
		int enable = 1;
		if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&enable, sizeof(enable)) == SOCKET_ERROR) {
		printf("Setsockopt error: %d\n", WSAGetLastError());
		closesocket(sock);
		return 1;
		}

		// 构造IP数据包（此处省略IP头和负载的填充步骤）
		char packet[1024];
		// ...（填充IP头及负载）

		// 发送到目标地址（示例：192.168.1.1）
		sockaddr_in destAddr;
		destAddr.sin_family = AF_INET;
		destAddr.sin_addr.s_addr = inet_addr("192.168.1.1");

		if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&destAddr, sizeof(destAddr)) == SOCKET_ERROR) {
		printf("Sendto error: %d\n", WSAGetLastError());
		}

		closesocket(sock);
		WSACleanup();
		return 0;
		}


		*/
		//PF_INET 
		//_sockfd = ::socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		//AF_INET
		/*
		int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  
		if(sock < 0){  
		perror("sock");  
		return -1;  
		}  

		while(1){  
		ret = recvfrom(sock, buf, 1024, 0, NULL, NULL);  

		printf("recv: ");  
		printhex(buf, ret, " ");  
		}  
		close(sock );
		*/

		return false;
	}

	bool CCJGW_PrsVendorRoamingBroadcast::StopPrsVendorRoamingServer()
	{
		return false;
	}
}

