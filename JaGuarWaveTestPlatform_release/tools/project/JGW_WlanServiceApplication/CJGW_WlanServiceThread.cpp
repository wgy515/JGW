#include "StdAfx.h"
#include "CJGW_WlanServiceThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_RequestBaseJson.h"
#include "CJGW_ResponseScanWlanJSON.h"
#include "CJGW_RequestConnectWlanJson.h"
#include "CJGW_ReqPumasCommadJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>

#define TSE_FTM_MAX_READ_BUF 1024 * 4

namespace JGW
{
	//! connectionMode : manual
	const std::wstring gstrWlanProFile = L"<?xml version=\"1.0\"?>\
										  <WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
										  <name>%s</name>\
										  <SSIDConfig>\
										  <SSID>\
										  <name>%s</name>\
										  </SSID>\
										  </SSIDConfig>\
										  <connectionType>ESS</connectionType>\
										  <connectionMode>manual</connectionMode>\
										  <MSM>\
										  <security>\
										  <authEncryption>\
										  <authentication>WPA2PSK</authentication>\
										  <encryption>AES</encryption>\
										  <useOneX>false</useOneX>\
										  </authEncryption>\
										  <sharedKey>\
										  <keyType>passPhrase</keyType>\
										  <protected>false</protected>\
										  <keyMaterial>%s</keyMaterial>\
										  </sharedKey>\
										  </security>\
										  </MSM>\
										  <MacRandomization xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v3\">\
										  <enableRandomization>false</enableRandomization>\
										  </MacRandomization>\
										  </WLANProfile>";
#if 0
	<?xml version="1.0" encoding="US-ASCII"?>
		<WLANProfile xmlns="https://www.microsoft.com/networking/WLAN/profile/v1">
		<name>SampleWPAPSK</name>
		<SSIDConfig>
		<SSID>
		<name>SampleWPAPSK</name>
		</SSID>
		</SSIDConfig>
		<connectionType>ESS</connectionType>
		<connectionMode>auto</connectionMode>
		<autoSwitch>false</autoSwitch>
		<MSM>
		<security>
		<authEncryption>
		<authentication>WPA2PSK</authentication>
		<encryption>AES</encryption>
		<useOneX>false</useOneX>
		</authEncryption>
		<sharedKey>
		<keyType>passPhrase</keyType>
		<protected>false</protected>
		<keyMaterial> <!-- insert key here --> </keyMaterial>
		</sharedKey>
		</security>
		</MSM>
		</WLANProfile>


		<?xml version="1.0" encoding="US-ASCII"?>
		<WLANProfile xmlns="https://www.microsoft.com/networking/WLAN/profile/v1">
		<name>SampleWPA2PSK</name>
		<SSIDConfig>
		<SSID>
		<name>SampleWPA2PSK</name>
		</SSID>
		</SSIDConfig>
		<connectionType>ESS</connectionType>
		<connectionMode>auto</connectionMode>
		<autoSwitch>false</autoSwitch>
		<MSM>
		<security>
		<authEncryption>
		<authentication>WPA2PSK</authentication>
		<encryption>AES</encryption>
		<useOneX>false</useOneX>
		</authEncryption>
		<sharedKey>
		<keyType>passPhrase</keyType>
		<protected>false</protected>
		<keyMaterial> <!-- insert key here --> </keyMaterial>
		</sharedKey>
		</security>
		</MSM>
		</WLANProfile>
#endif
		CCJGW_WlanServiceThread::CCJGW_WlanServiceThread(void) 
		: macceptorSocket(mios,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),3001))
		,mdeadline_(mios)
	{
		mTxBuf = (char*)calloc(0x01,TSE_FTM_MAX_READ_BUF);
		mRxBuf = (char*)calloc(0x01,TSE_FTM_MAX_READ_BUF);
		msWlanSocketParam.mpMessageThread = this;
	}


	CCJGW_WlanServiceThread::~CCJGW_WlanServiceThread(void)
	{
		free(mTxBuf);
		free(mRxBuf);
	}

	BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_WlanServiceThread,CCMessageThread)
		ON_MYTHREAD_MESSGAE(WM_THREAD_TEST_START_MSG,&CCJGW_WlanServiceThread::OnStartServiceThread)
	END_MYTHREAD_MESSAGE_MAP()

	void CCJGW_WlanServiceThread::CloseSocketService(const boost::system::error_code e)
	{
		if (e || mdeadline_.expires_at() > boost::asio::deadline_timer::traits_type::now()) return ;
		//! 取消定时函数
		boost::system::error_code ec;
		mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
		mdeadline_.cancel(ec);
		//! 关闭client socket 连接
		if (!!msWlanSocketParam.mClientSock && msWlanSocketParam.mClientSock->is_open())
		{
			msWlanSocketParam.mClientSock->close(ec);
		}
		msWlanSocketParam.mClientSock = NULL;
		//! 关闭service socket 连接
		if (macceptorSocket.is_open()) macceptorSocket.close(ec);
	}

	void CCJGW_WlanServiceThread::WaitSocketReadWriteTimeOut(const boost::system::error_code e)
	{
		if (e || mdeadline_.expires_at() > boost::asio::deadline_timer::traits_type::now()) return ;


		//! 设置永不超时
		boost::system::error_code ec;
		mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
		//! 关闭socket 连接
		if (!!msWlanSocketParam.mClientSock && msWlanSocketParam.mClientSock->is_open())
		{
			msWlanSocketParam.mClientSock->close(ec);
		}
		msWlanSocketParam.mClientSock = NULL;
	}

	void CCJGW_WlanServiceThread::StopClientThread()
	{
		boost::system::error_code ec_error;
		mdeadline_.expires_from_now(boost::posix_time::microsec(100),ec_error);
		mdeadline_.async_wait(boost::bind(&CCJGW_WlanServiceThread::CloseSocketService, this,_1));
		//! 等待socket 关闭
		while (mbConnected) 
		{
			Log4WI(L"waiting wlan service thread close");
			Sleep(200);
		}
	}

	void CCJGW_WlanServiceThread::OnStartServiceThread(WPARAM wParam,LPARAM lParam)
	{
		mAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
		HWND hWnd = (HWND)lParam;
		mbConnected = true;
		try
		{
			StartAccept();
			mios.run();
		}
		catch (std::exception& e)
		{
			Log4WE_F(L"asio run exception(%s)",JGW_A2W_A(e.what()).c_str());
		}
		mbConnected = false;
	}

	void CCJGW_WlanServiceThread::StartAccept()
	{
		if (!macceptorSocket.is_open()) return ;
		boost::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(mios));
		macceptorSocket.async_accept(*sock,boost::bind(&CCJGW_WlanServiceThread::AcceptHandle,this,boost::asio::placeholders::error,sock));
	}

	void CCJGW_WlanServiceThread::AcceptHandle(const boost::system::error_code& ec,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
	{
		//! 请求 accept 出现异常
		if (ec) 
		{
			mAsyncWndMessage->PutAsyncMessage(WM_CLOSE);
			Log4WE_F(L"Wlan Service Thread AcceptHandle Exception (%s)",JGW_A2W(ec.message()).c_str());
		}
		else
		{
			msWlanSocketParam.mClientSock = sock;
			boost::system::error_code ec;       
			mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
			sock->async_read_some(boost::asio::buffer(mRxBuf,TSE_FTM_MAX_READ_BUF),boost::bind(&CCJGW_WlanServiceThread::ReadSocket,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,sock));
		}
	}

	void CCJGW_WlanServiceThread::ReadSocket(const boost::system::error_code& ec,size_t len,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
	{
		if (ec)
		{
			boost::system::error_code ecclose;
			sock->close(ecclose);
			StartAccept();
		}
		else
		{
			mRxBuf[len] = '\0';
			msWlanSocketParam.mstrJson = mRxBuf;
			mAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_A2W(msWlanSocketParam.mstrJson).c_str());
			if (!mWlanHandleThread.IsThreadRun())
			{
				mWlanHandleThread.CreateMessageThread(0,0,true);
				Sleep(250);
			}
			mWlanHandleThread.PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(&msWlanSocketParam));
			//! 如果90s都没有写入操作就超时处理
			boost::system::error_code ec_error;
			mdeadline_.expires_from_now(boost::posix_time::seconds(90),ec_error);
			mdeadline_.async_wait(boost::bind(&CCJGW_WlanServiceThread::WaitSocketReadWriteTimeOut, this,_1));
		}
	}

	void CCJGW_WlanServiceThread::WriteSocket(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
	{
		if (ec)
		{
			boost::system::error_code ecclose;
			sock->close(ecclose);
			StartAccept();
		}
		else
		{
			boost::system::error_code ec;       
			mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
			sock->async_read_some(boost::asio::buffer(mRxBuf,TSE_FTM_MAX_READ_BUF),boost::bind(&CCJGW_WlanServiceThread::ReadSocket,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,sock));
		}
	}

	void CCJGW_WlanServiceThread::SendJsonCommand(const std::string& strJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
	{
		if (!sock || !sock->is_open()) return ;
		strcpy_s(mTxBuf,TSE_FTM_MAX_READ_BUF,strJson.c_str());
		mAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_A2W(mTxBuf).c_str());
		sock->async_write_some(boost::asio::buffer(mTxBuf,TSE_FTM_MAX_READ_BUF),boost::bind(&CCJGW_WlanServiceThread::WriteSocket,this,_1,_2,sock));
	}
#if 0
	std::string CCJGW_WlanServiceThread::HandleRequest(const std::string& strRequest)
	{
		CCJGW_RequestBaseJson requestBaseJson;
		requestBaseJson.FromJosn(strRequest);

		switch (requestBaseJson.requestType)
		{
		case E_SCAN_WLAN:
			return HandleResopnseScanWiFi(strRequest);
		case E_CONNECT_WLAN:
			return HandleResopnseConnectWiFi(strRequest);
		case E_DISCONNECT_WLAN:
			return HandleResponseDisConnectWiFi(strRequest);
		case E_CONNECT_PUMAS:
			return HandleRspConnectPumas(strRequest);
		case E_COMMAND_PUMAS:
			return HandleRspCommandPumas(strRequest);
		case E_DISCONNECT_PUMAS:
			return HandleRspDisConnectPumas(strRequest);
		case E_CHECK_WLAN_CONNECT:
			return HandleRspCheckWlanConnectStatus(strRequest);
		}

		return "{\"responseType\":-1,\"bStatus\":false,\"returnMessage\":\"未知命令类型\"}";
	}

	std::string CCJGW_WlanServiceThread::HandleResopnseScanWiFi(const std::string& strRequest)
	{
		mapBss.clear();
		CCJGW_ResponseScanWlanJSON responseScanJson;
		mstrGUID = wlan.FindWalnGUIDToInterfaceDescriptionKeyword(L"Peraso");
		if (mstrGUID.empty()) 
		{
			responseScanJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg());
		}
		else
		{
			responseScanJson.bStatus = wlan.GetWlanBssList(mstrGUID,mapBss,mScanTime);
			if (!responseScanJson.bStatus)
			{
				responseScanJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg()); 
			}
			if (mScanTime < 5000) mScanTime += 500;
		}
		responseScanJson.responseType = E_SCAN_WLAN;
		return responseScanJson.BuildScanWlanJson(mapBss);
	}

	std::string CCJGW_WlanServiceThread::HandleResopnseConnectWiFi(const std::string& strRequest)
	{
		CCJGW_FTM_Json ftmJson;
		mScanTime = 1000;
		CCJGW_RequestConnectWlanJson requestConnectWlanJson;
		CCJGW_ResponseBaseJson responseJson;
		ftmJson.FromJsonToString(strRequest);
		requestConnectWlanJson.FromJosn(ftmJson);
		mstrSSID = JGW_A2W(requestConnectWlanJson.ssidname);
		responseJson.responseType = E_CONNECT_WLAN;


		if (mstrGUID.empty())
		{
			responseJson.returnMessage = "请先扫描WIFI";
		}
		else
		{
			if (mapBss.end() == mapBss.find(mstrSSID))
			{
				mapBss[mstrSSID] = 1;
			}

			if (requestConnectWlanJson.passwrod.empty())
			{
				responseJson.bStatus = wlan.ConnectOpenWlan(mstrGUID,mstrSSID,mapBss[mstrSSID]);
			}
			else
			{
				std::wstring strWlanProFile;
				JGW_FormatWString(strWlanProFile,gstrWlanProFile.c_str(),mstrSSID.c_str(),mstrSSID.c_str(),JGW_A2W(requestConnectWlanJson.passwrod).c_str());
				responseJson.bStatus = wlan.SetWlanProfile(mstrGUID,strWlanProFile);
				if (responseJson.bStatus )
					responseJson.bStatus = wlan.ConnectWlan(mstrGUID,mstrSSID,mstrSSID,mapBss[JGW_A2W(requestConnectWlanJson.ssidname)]);
			}

			if (!responseJson.bStatus)
			{
				responseJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg());
			}
		}

		return responseJson.BuildResponseBaseJson();
	}

	std::string CCJGW_WlanServiceThread::HandleResponseDisConnectWiFi(const std::string& strRequest)
	{
		CCJGW_ResponseBaseJson responseJson;
		mScanTime = 1000;
		responseJson.responseType = E_DISCONNECT_WLAN;
		if (mstrGUID.empty())
		{
			responseJson.returnMessage = "请先扫描WIFI";
		}
		else
		{
			wlan.DeleteWlanProfile(mstrGUID,mstrSSID);
			responseJson.bStatus = wlan.DisConnectWlan(mstrGUID);
			if (!responseJson.bStatus)
			{
				responseJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg());
			}
		}
		return responseJson.BuildResponseBaseJson();
	}

	std::string CCJGW_WlanServiceThread::HandleRspConnectPumas(const std::string& strRequest)
	{
		CCJGW_ResponseBaseJson responseJson;
		std::wstring strComPort;
		responseJson.responseType = E_CONNECT_PUMAS;

		strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
		if (strComPort.empty()) 
		{
			responseJson.returnMessage = "Not Find Peraso Device";
			return responseJson.BuildResponseBaseJson();
		}
		responseJson.bStatus = mDonglesComPort.OpenDonglesComPort(strComPort);
		if (!responseJson.bStatus) responseJson.returnMessage = "Open Peraso Device Fail";
		return responseJson.BuildResponseBaseJson();
	}

	std::string CCJGW_WlanServiceThread::HandleRspCommandPumas(const std::string& strRequest)
	{
		CCJGW_FTM_Json ftmJson;
		CCJGW_ReqPumasCommadJson puamsCommandJson;
		CCJGW_ResponseBaseJson responseJson;
		responseJson.responseType = E_COMMAND_PUMAS;
		ftmJson.FromJsonToString(strRequest);
		puamsCommandJson.FromJosn(ftmJson);
		//! 未打开Dongles
		if (!mDonglesComPort.IsOpenDonglesComPort())
		{
			puamsCommandJson.command = "Peraso Device Is Not Open";
			return responseJson.BuildResponseBaseJson();
		}

		std::string strRead;
		responseJson.bStatus = mDonglesComPort.ReadWriteCommands(puamsCommandJson.command.c_str(),strRead);
		if (!responseJson.bStatus) responseJson.returnMessage = "Read Write Peraso Device Fail";
		else responseJson.returnMessage = strRead;

		return responseJson.BuildResponseBaseJson();
	}

	std::string CCJGW_WlanServiceThread::HandleRspDisConnectPumas(const std::string& strRequest)
	{
		CCJGW_ResponseBaseJson responseJson;
		responseJson.responseType = E_DISCONNECT_PUMAS;
		responseJson.bStatus = true;
		mDonglesComPort.CloseDonglesComPort();
		return responseJson.BuildResponseBaseJson();
	}
	//! 如果WLAN检查连接失败，则断开WIFI，重新连接wifi
	//! 包含WLAN 网关IP地址(待检测IP地址)，远程主机IP地址(=>以排除本地USB网口IP地址)
	std::string CCJGW_WlanServiceThread::HandleRspCheckWlanConnectStatus(const std::string& strRequest)
	{
		//! 检查网关IP地址
		CCJGW_FTM_Json ftmJson;
		CCJGW_ReqCheckWlanConnectJson checkWlanJson;
		CCJGW_ResponseCheckWlanConnectJson responseJson;

		ftmJson.FromJsonToString(strRequest);
		checkWlanJson.FromJosn(ftmJson);
		responseJson.responseType = E_CHECK_WLAN_CONNECT;

		int i = 2;
		do 
		{
			ConnectWlan(checkWlanJson,responseJson,30);
			//if (!) /*responseJson.returnMessage = "wifi 连接失败"*/;
			Sleep(1000);

			responseJson.wlanAddress = GetWlanIpAddress(checkWlanJson.wlangateway,checkWlanJson.localaddress);
			if (!responseJson.wlanAddress.empty())
			{
				//! 检查内网连接
				for (size_t i = 0;i < 10;i ++)
				{
					//! 如果能PING通，说明网络连接是OK的，否则重新断开WIFI连接
					if (mIcmpSocket.Ping(checkWlanJson.wlangateway,responseJson.wlanAddress.c_str()))
					{
						responseJson.bStatus = true;
						return responseJson.BuildResponseCheckWlanConnectJson();
					}
				}
				JGW_FormatString(responseJson.returnMessage,"ping %s(wlan地址: %s)失败",checkWlanJson.wlangateway.c_str(),responseJson.wlanAddress.c_str());
			}     
		} while (i-- > 0);
		return responseJson.BuildResponseCheckWlanConnectJson();
	}

	std::string CCJGW_WlanServiceThread::GetWlanIpAddress(const std::string& strWlanGateWay,const std::string& strLocalAddress)
	{
		std::vector<std::string> vipAddress;
		msyncSocket.GetHostIP(std::string(""),vipAddress);
		//! 优先取与WLAN网关同一个局域网的IP
		std::string strWlanGateWaySegment = strWlanGateWay.substr(0,strWlanGateWay.rfind('.') + 1);
		for (size_t i = 0;i < vipAddress.size();i ++)
		{
			if (NULL == JGW_StrComparenoCaseStr(strLocalAddress.c_str(),vipAddress[i].c_str())) continue;/*return vipAddress[i];*/
			if (NULL == _strnicmp(strWlanGateWaySegment.c_str(),vipAddress[i].c_str(),strWlanGateWaySegment.length())) return vipAddress[i];
		}
		//! 否则默认取与本地地址不相同的第一个IP
		for (size_t i = 0;i < vipAddress.size();i ++)
		{
			if (NULL == JGW_StrComparenoCaseStr(strLocalAddress.c_str(),vipAddress[i].c_str())) continue;/*return vipAddress[i];*/
			return vipAddress[i];
		}
		return "";
	}

	bool CCJGW_WlanServiceThread::ConnectWlan(CCJGW_ReqCheckWlanConnectJson repJson,CCJGW_ResponseCheckWlanConnectJson rspJson,int nTimeOut)
	{
		jgw_timer jt;
		bool bFindSSID = false;
		while (jt.elapsed() <= nTimeOut)
		{
			mstrGUID = wlan.FindWalnGUIDToInterfaceDescriptionKeyword(JGW_A2W(repJson.InterfaceDescription));
			if (mstrGUID.empty()) 
			{
				Sleep(200);
				rspJson.returnMessage = "未识别到WLAN网卡设备，请检查WLAN网卡是否插入";
				continue;
			}
			wlan.DisConnectWlan(mstrGUID);
			//! scan wifi
			rspJson.bStatus = wlan.GetWlanBssList(mstrGUID,mapBss,mScanTime);
			if (!rspJson.bStatus)
			{
				rspJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg()); 
				if (mScanTime < 5000) mScanTime += 500;
				Sleep(200);
				continue;
			}
			//! 检查SSID是否扫描到
			bFindSSID = false;
			mstrSSID = JGW_A2W(repJson.ssidname);
			for (std::map<std::wstring,int>::iterator it = mapBss.begin();
				it != mapBss.end();
				++ it)
			{
				if (NULL == JGW_WStrComparenoCaseWStr(mstrSSID.c_str(),it->first.c_str()))
				{ 
					bFindSSID = true;
					break ;
				}
			}

			if (!bFindSSID)
			{
				Sleep(200);
				rspJson.returnMessage = "未查到到当前SSID";
				rspJson.returnMessage= repJson.ssidname;
				continue;
			}
			//! 连接11AD
			if (repJson.passwrod.empty())
			{
				rspJson.bStatus = wlan.ConnectOpenWlan(mstrGUID,mstrSSID,mapBss[mstrSSID]);
			}
			else
			{
				std::wstring strWlanProFile;
				JGW_FormatWString(strWlanProFile,gstrWlanProFile.c_str(),mstrSSID.c_str(),mstrSSID.c_str(),JGW_A2W(repJson.passwrod).c_str());
				rspJson.bStatus = wlan.SetWlanProfile(mstrGUID,strWlanProFile);
				if (rspJson.bStatus )
					rspJson.bStatus = wlan.ConnectWlan(mstrGUID,mstrSSID,mstrSSID,mapBss[mstrSSID]);
			}

			if (!rspJson.bStatus)
			{
				rspJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg());
			}
			else
			{
				return true;
			}
		}
		return false;
	}
#endif
}
