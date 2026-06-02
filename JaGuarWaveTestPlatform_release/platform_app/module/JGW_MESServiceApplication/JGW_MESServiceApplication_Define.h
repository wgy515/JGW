#pragma once
#include <string>

//! 接收缓冲区大小
#define TCP_RECEIVE_BUFFER_MAX_LENGTH (1024*10)

namespace JGW
{
	typedef struct  
	{
		std::string mstrMESServiceSocketPort;
		std::string mstrMESServiceIP;
	}S_MESSericeInfo,*LPS_MESSericeInfo;


}
