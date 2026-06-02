#include "StdAfx.h"
#include "CJGW_PrsRoamingLogDebugSocket.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "PrsVendorRoamingDataDefine.h"
#include <fstream>
#include <regex>

namespace JGW
{
	CCJGW_PrsRoamingLogDebugSocket::CCJGW_PrsRoamingLogDebugSocket(void) : mbIsStop(true)
	{
		mszLogBuffer = new char[TCP_RECEIVE_BUFFER_MAX_LENGTH];
	}


	CCJGW_PrsRoamingLogDebugSocket::~CCJGW_PrsRoamingLogDebugSocket(void)
	{
		delete[] mszLogBuffer;
	}

	void OnRunThreadCallback(void *lpParam)
	{
		CCJGW_PrsRoamingLogDebugSocket* pPrsRoamingLogDebugSocket = (CCJGW_PrsRoamingLogDebugSocket*)lpParam;
		if (NULL != pPrsRoamingLogDebugSocket)
		{
			pPrsRoamingLogDebugSocket->OnHandlePrsRoamingLogDebugSocket();
		}
	}

    __int64 GetNotifNumeric64Field(const std::string& strNotif,const std::string& strField)
    {
        std::regex regex_patterns(JGW_GetFormatString("\"%s\": (-?\\d+)",strField.c_str()));

        std::smatch cm;
        std::regex_search(strNotif,cm, regex_patterns);
        if (0 == cm.size()) return 0;
        return _atoi64(((std::string)cm[1]).c_str());
    }

    __int64 GetTimeStampToLog(const std::string& strLog)
    {
       __int64 dw64TimeStamp = GetNotifNumeric64Field(strLog,"timeReceived");
        if (0 != dw64TimeStamp)
        {
            return dw64TimeStamp;
        }
        return GetNotifNumeric64Field(strLog,"timeSent");
    }


	void CCJGW_PrsRoamingLogDebugSocket::OnHandlePrsRoamingLogDebugSocket()
	{
		//! 打开日志文件
		std::ifstream ifFile;
		ifFile.imbue(std::locale(""));
		ifFile.open(mstrRoamingLogPath,std::ios_base::in);
		if (!ifFile.is_open()) return;

        __int64 dwPrevTimeStamp = 0;
		//! 
		Poco::Timespan timeout(120,600);
		//! 
		Poco::Net::StreamSocket debugVendorAppTcpSocket;
		try
		{
			//! 连接socket
			debugVendorAppTcpSocket.connect(Poco::Net::SocketAddress("127.0.0.1", PRS_VENDOR_APP_TCP_SOCKET_PORT),timeout);
			while (!mbIsStop)
			{
				if (debugVendorAppTcpSocket.poll(timeout,Poco::Net::Socket::SELECT_WRITE))
				{

#if 0
					if (ifFile.getline(mszLogBuffer,TCP_RECEIVE_BUFFER_MAX_LENGTH))
					{
                        if (ifFile.gcount() > 0)
                        {
                            //int nCount = ifFile.gcount();
						    debugVendorAppTcpSocket.sendBytes(mszLogBuffer,(int)ifFile.gcount());
                        }
					}
					else
					{
						break;
					}
					Sleep(10);
#else
                    if (ifFile.getline(mszLogBuffer,TCP_RECEIVE_BUFFER_MAX_LENGTH))
                    {
                        //timeReceived
                        //timeSent
                        if (ifFile.gcount() > 0)
                        {
                            __int64 dwTimeStamp = GetTimeStampToLog(std::string(mszLogBuffer,mszLogBuffer + ifFile.gcount()));
                            int milliseconds_timestamep = roaming_msec_base_time;
                            if (dwTimeStamp > raw_max_time_stamp)
                            {
                                milliseconds_timestamep = raw_msec_base_time;
                            }
                            if (dwPrevTimeStamp > 0 && dwTimeStamp > 0)
                            {
                                int dwDelayTimeMsec = (dwTimeStamp - dwPrevTimeStamp) / milliseconds_timestamep;
                                Sleep(dwDelayTimeMsec > 300 ? 300 : dwDelayTimeMsec);
                            }
                            else
                            {
                                Sleep(1);
                            }
                            debugVendorAppTcpSocket.sendBytes(mszLogBuffer,(int)ifFile.gcount());
                            debugVendorAppTcpSocket.sendBytes("\n",1);
                            dwPrevTimeStamp = dwTimeStamp;
                        }
                    }
                    else
                    {
                        break;
                    }





#endif
				}
			}
		}
        catch (Poco::Net::ConnectionResetException& ec)
        {
            mstrErrorMessage = JGW_A2W(ec.message(),CP_UTF8);
            OutputDebugStringA(ec.message().c_str());
            OutputDebugStringA("\n");
        }
		catch (Poco::Net::NetException& ec)
		{
			mstrErrorMessage = JGW_A2W(ec.message(),CP_UTF8);
            OutputDebugStringA(ec.message().c_str());
            OutputDebugStringA("\n");
		}
		mbIsStop = true;
	}

	std::wstring CCJGW_PrsRoamingLogDebugSocket::GetErrorMessage()
	{
		return mstrErrorMessage;
	}

	bool CCJGW_PrsRoamingLogDebugSocket::StartPrsRoamingLogDebugSocket(const std::wstring& strRoamingLogPath)
	{
		//! 检查日志文件是否存在
		if (!JGW_FileExistsToFilePath(strRoamingLogPath.c_str()))
		{
			JGW_FormatWString(mstrErrorMessage,L"Error: roaming log file(%s) does not exist",strRoamingLogPath.c_str());
			return false;
		}
		mstrRoamingLogPath = strRoamingLogPath;
		mbIsStop = false;
		return mcPrsRoamingLogDebugSocketThread.CreateMessageThread(OnRunThreadCallback,this,true);
	}

	void CCJGW_PrsRoamingLogDebugSocket::StopPrsRoamingLogDebugSocket()
	{
		mbIsStop = true;
		while (mcPrsRoamingLogDebugSocketThread.IsThreadRun()) Sleep(10);
		mcPrsRoamingLogDebugSocketThread.TerminateMessageThread();
	}
}
