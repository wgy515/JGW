#include "StdAfx.h"
#include "CJGW_MobilePhoneClientSocketThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <sstream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>

namespace JGW
{
	CCJGW_MobilePhoneClientSocketThread::CCJGW_MobilePhoneClientSocketThread(bool& bRunTestThread,S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage) : mbRunTestThread(bRunTestThread),mpsTestParams(psTestParams),mpAsyncWndMessage(pAsyncWndMessage)
	{
		mszReadBuffer = new char[PHONE_SOCKET_RX_MAX_LENGTH + 1];
	}


	CCJGW_MobilePhoneClientSocketThread::~CCJGW_MobilePhoneClientSocketThread(void)
	{
		delete[] mszReadBuffer;
	}

	bool CCJGW_MobilePhoneClientSocketThread::StartMobilePhoneClientSocketThread()
	{
		if (!mpsTestParams->mbIsUseCellphoneIP) return true;

		mstrReadContectMsg = "";
		//! 1、检查手机热点IP是否能PING通 （如果不能PING通，则终止开始测试请求，并进行提示）
		Log4WI_F(L"Check mobile hotspot(%s) Internet connection ..",mpsTestParams->mstrPhoneIPAddr.c_str());
		if (!CheckNetworkConnection(mpsTestParams->mstrPhoneIPAddr))
		{
			SendWindowMsg(L"Failed to check the phone network(%s) connection",mpsTestParams->mstrPhoneIPAddr.c_str());
			Log4WE_F(L"Check mobile hotspot(%s) Internet connection Failed",mpsTestParams->mstrPhoneIPAddr.c_str());
			return false;
		}
		//! 2、连接手机热点服务端SERVER （如果不能连接，则终止开始测试请求，并进行提示）
		Log4WI_F(L"Connect to the mobile hotspot(%s) server..",mpsTestParams->mstrPhoneIPAddr.c_str());
		if (!mcAsyncSocket.ConnectSocketTimeOut(JGW_W2A(mpsTestParams->mstrPhoneIPAddr).c_str(),PHONE_SOCKET_PORT))
		{
			SendWindowMsg(L"Failed to connect to the mobile(%s) server",mpsTestParams->mstrPhoneIPAddr.c_str());
			Log4WE_F(L"Connect to the mobile hotspot(%s) server Failed",mpsTestParams->mstrPhoneIPAddr.c_str());
			return false;
		}
		//! 3、读取手机热点服务端的速率相关信息，开启线程读取手机热点
		if (!mcClientSocketThread.CreateMessageThread(OnReadWriteClientSocketCallback,this,true))
		{
			SendWindowMsg(L"Description Failed to start the server thread to read GPS speed");
			return false;
		}

		return true;
	}

	bool CCJGW_MobilePhoneClientSocketThread::CheckNetworkConnection(const std::wstring& strIPAddr,const std::wstring& strSourceAddr /* = L"" */,int nTestTimeSec /* = 30 */,int nTestContinuousCount /* = 2 */)
	{
		jgw_timer jt;
		bool bResult = false,bSingleResult = true;
		std::string strIPAddrA = JGW_W2A(strIPAddr);
		std::string strSourceAddrA = JGW_W2A(strSourceAddr);
		std::wstring strTemp;

		while (jt.elapsed() <= nTestTimeSec && !bResult && mbRunTestThread)
		{
			bResult = true;
			for (int index = 0;index < nTestContinuousCount && mbRunTestThread;index++)
			{
				bSingleResult = mIcmpSocket.Ping(strIPAddrA,strSourceAddrA.empty()?NULL:strSourceAddrA.c_str(),1000 * 10);
				SendWindowMsg(L"ping %s %s",strIPAddr.c_str(),JGW_A2W(mIcmpSocket.GetPingResult()).c_str());
				if (bSingleResult)
				{

					JGW_FormatWString(strTemp,L"%.3lf",mIcmpSocket.GetPingTimeMs());
					mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,strTemp.c_str());
				}
				else
				{
					mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
				}
				bResult &= bSingleResult;
				Sleep(900);
			}
		}

		return bResult;
	}

	void CCJGW_MobilePhoneClientSocketThread::SendWindowMsg(const std::wstring& strMessage)
	{
		Log4WI(strMessage.c_str());
		mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMessage.c_str());
	}

	void CCJGW_MobilePhoneClientSocketThread::SendWindowMsg(const wchar_t* szFormat,...)
	{
		std::wstring strLog;
		FORMAT_WSTRING(strLog,szFormat);
		Log4WI(strLog.c_str());
		mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
	}

	void CCJGW_MobilePhoneClientSocketThread::OnReadWriteClientSocketCallback(void *lpParam)
	{
		CCJGW_MobilePhoneClientSocketThread* pMobilePhoneClientSocketThread = (CCJGW_MobilePhoneClientSocketThread*)lpParam;
		if (NULL != pMobilePhoneClientSocketThread)
		{
			pMobilePhoneClientSocketThread->ReadWriteClientSocketThreadCallback();
		}
	}

	void CCJGW_MobilePhoneClientSocketThread::ReadWriteClientSocketThreadCallback()
	{
		std::string strReadContent;
		while (mbRunTestThread)
		{
			//! 检查SOCKET是否已经连接
			if (!mcAsyncSocket.CheckSocketIsConnected())
			{
				if (!mcAsyncSocket.ConnectSocketTimeOut(JGW_W2A(mpsTestParams->mstrPhoneIPAddr).c_str(),PHONE_SOCKET_PORT))
				{
					mStopEvent.tryWait(1000);
					continue;
				}
			}
			//! 是否需要向手机SOCEKT发送测试信息
			if (!mpsTestParams->mstrUploadPhoneSocketMessage.empty())
			{
				mcAsyncSocket.AsyncWriteSocket(JGW_W2A(mpsTestParams->mstrUploadPhoneSocketMessage));
				mpsTestParams->mstrUploadPhoneSocketMessage = L"";
			}

			strReadContent = "";
			if (!ReadClientSocket(strReadContent))
			{
				mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Warning: Error reading GPS and speed information");
                mpAsyncWndMessage->PutAsyncStringMessage(WM_GPS_SPEED_MSG,L" , , , , , ");
				mcAsyncSocket.CleanupSocket();
				mStopEvent.tryWait(100);
				continue;
			}
			HandleClientSocketMessage(strReadContent);
			mStopEvent.tryWait(100);
		}
	}

	void CCJGW_MobilePhoneClientSocketThread::StopMobilePhoneClientSocketThread()
	{
		mbRunTestThread = false;
		mStopEvent.set();	
	}

    void CCJGW_MobilePhoneClientSocketThread::TerminateMobilePhoneClientSocketThread()
    {
        mcAsyncSocket.CleanupSocket();
        mcClientSocketThread.TerminateMessageThread();
    }

	bool CCJGW_MobilePhoneClientSocketThread::ReadClientSocket(std::string& strMessage)
	{
		memset(mszReadBuffer,0x00,PHONE_SOCKET_RX_MAX_LENGTH + 1);
		int len = mcAsyncSocket.RecvSocket(mszReadBuffer,PHONE_SOCKET_RX_MAX_LENGTH);
		if (len == SOCKET_ERROR || len <= 0)
		{
			Log4WE_F(L"recv to the mobile hotspot(%s) server Failed",mpsTestParams->mstrPhoneIPAddr.c_str());
			return false;
		}
		strMessage = mszReadBuffer;
		Log4WI(JGW_A2W(strMessage).c_str());

		return true;
	}

	void CCJGW_MobilePhoneClientSocketThread::HandleClientSocketMessage(const std::string& strMsg)
	{
		//! 起始 {}
		mstrReadContectMsg += strMsg;

		while (true)
		{
			size_t index = mstrReadContectMsg.find("{");
			if (index == std::string::npos) break;
			mstrReadContectMsg = mstrReadContectMsg.erase(0,index);
			size_t endIndex = mstrReadContectMsg.find("}",1);
			if (endIndex == std::string::npos) break;
			std::string strRsp = mstrReadContectMsg.substr(0,endIndex + 1);
			ParseClientSocketMsgJson(strRsp);
			mstrReadContectMsg = mstrReadContectMsg.erase(0,endIndex + 1);
		}
	}

	void CCJGW_MobilePhoneClientSocketThread::ParseClientSocketMsgJson(const std::string& strJsonValue)
	{
		//! GPS JSON
		if (std::string::npos == strJsonValue.find("\"message\""))
		{
			HandleGspInfoJson(strJsonValue);
		}
		else 
		{
			//! Message Json
			HandleMessageInfoJson(strJsonValue);
		}
	}

	void CCJGW_MobilePhoneClientSocketThread::HandleGspInfoJson(const std::string& strJsonValue)
	{
		try 
		{
			boost::property_tree::ptree mJsonTree;
			std::stringstream msg_ss(strJsonValue);

			read_json(msg_ss, mJsonTree);
			std::string strLatitude = mJsonTree.get<std::string>("latitude");
			std::string strLongitude = mJsonTree.get<std::string>("longitude");
			std::string strSpeed = mJsonTree.get<std::string>("speed");
			std::string strDistance = mJsonTree.get<std::string>("distance");
			std::string remoteLatitude = mJsonTree.get<std::string>("remoteLatitude");
			std::string remoteLongtitude = mJsonTree.get<std::string>("remoteLongtitude");

			std::string strGpsSpeedInfo;
			JGW_FormatString(strGpsSpeedInfo,"%s,%s,%s,%s,%s,%s",strLatitude.c_str(),strLongitude.c_str(),strSpeed.c_str(),strDistance.c_str(),remoteLatitude.c_str(),remoteLongtitude.c_str());
			Log4WI(JGW_A2W(strGpsSpeedInfo).c_str());
			mpAsyncWndMessage->PutAsyncStringMessage(WM_GPS_SPEED_MSG,JGW_A2W(strGpsSpeedInfo).c_str());
			return;
		}
		catch (boost::property_tree::json_parser::json_parser_error& e)
		{
			SendWindowMsg(L"parser Json(%s) Fail(%s)",JGW_A2W(strJsonValue).c_str(),JGW_A2W_A(e.what()).c_str());   
		}
		catch (boost::property_tree::ptree_bad_path& e)
		{
			SendWindowMsg(L"%s(%s)",JGW_A2W_A(e.what()).c_str(),JGW_A2W(strJsonValue).c_str());
		}
		catch (boost::property_tree::ptree_bad_data& e)
		{
			SendWindowMsg(L"%s(%s)",JGW_A2W_A(e.what()).c_str(),JGW_A2W(strJsonValue).c_str());
		}
		mpAsyncWndMessage->PutAsyncStringMessage(WM_GPS_SPEED_MSG,L"NaN,NaN,NaN,NaN,NaN,NaN");
	}

	void CCJGW_MobilePhoneClientSocketThread::HandleMessageInfoJson(const std::string& strJsonValue)
	{
		try 
		{
			boost::property_tree::ptree mJsonTree;
			std::stringstream msg_ss(strJsonValue);

			read_json(msg_ss, mJsonTree);
			std::string strRemoteMessage = mJsonTree.get<std::string>("message");
			mpAsyncWndMessage->PutAsyncStringMessage(WM_MPHONE_SOCKET_REMOTE_MSG,JGW_A2W(strRemoteMessage).c_str());
			Log4WI(JGW_A2W(strRemoteMessage).c_str());
			return;
		}
		catch (boost::property_tree::json_parser::json_parser_error& e)
		{
			SendWindowMsg(L"parser Json(%s) Fail(%s)",JGW_A2W(strJsonValue).c_str(),JGW_A2W_A(e.what()).c_str());   
		}
		catch (boost::property_tree::ptree_bad_path& e)
		{
			SendWindowMsg(L"%s(%s)",JGW_A2W_A(e.what()).c_str(),JGW_A2W(strJsonValue).c_str());
		}
		catch (boost::property_tree::ptree_bad_data& e)
		{
			SendWindowMsg(L"%s(%s)",JGW_A2W_A(e.what()).c_str(),JGW_A2W(strJsonValue).c_str());
		}
		mpAsyncWndMessage->PutAsyncStringMessage(WM_MPHONE_SOCKET_REMOTE_MSG,L"");
	}
}
