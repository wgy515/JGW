#include "StdAfx.h"
#include "CJGW_ThroughputTestThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
//#include "CJGW_FTM_Json.h"
#include <sstream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>


//root
//20171015

namespace JGW
{
	

	CCJGW_ThroughputTestThread::CCJGW_ThroughputTestThread(S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage) : mpsTestParams(psTestParams),mpAsyncWndMessage(pAsyncWndMessage),mbRunTestThread(false),mbStopIperf3Server(true),mcMobilePhoneClientSocketThread(mbRunTestThread,psTestParams,pAsyncWndMessage),mcIPQSshThread(mbRunTestThread,psTestParams,pAsyncWndMessage,m11ADConnectEventt)
	{
		mcIperf3Command.RegisterCallbackIperf3RawTestLog(&CCJGW_ThroughputTestThread::Iperf3RawTestLogCallbackFunc,this);
	}


	CCJGW_ThroughputTestThread::~CCJGW_ThroughputTestThread(void)
	{
	}

	BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_ThroughputTestThread,CCMessageThread)
		ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_ThroughputTestThread::OnRunTestThread)
	END_MYTHREAD_MESSAGE_MAP()


	void CCJGW_ThroughputTestThread::OnRunTestThread(WPARAM wParam,LPARAM lParam)
	{
		//! 检查SSH连接
		mpsTestParams->mstrIperfIPAddrA = JGW_W2A(mpsTestParams->mstrIperfIPAddr);
		mbRunTestThread = true;
		JGW_KillProcessToName(L"iperf3.exe");
		JGW_KillProcessToName(L"iperf.exe");

		//! 如果开启手机热点服务  && !mpsTestParams->mstrPhoneIPAddr.empty() && 0 != JGW_WStrComparenoCaseWStr(mpsTestParams->mstrPhoneIPAddr.c_str(),L"0.0.0.0")
		if (!mcMobilePhoneClientSocketThread.StartMobilePhoneClientSocketThread())
		{
			goto T_END;
		}
        //! 开启IPQ SSH交互线程
		if (!mcIPQSshThread.StartIPQSshThread())
        {
            goto T_END;
        }

		mpAsyncWndMessage->PutAsyncMessage(WM_START_RECORD_TEST_INFO);
		//! 4、监测11AD连接状态，当11AD状态变化的时候

		//! 5、当11AD状态时候，11AD处于连接状态
		while (mbRunTestThread)
		{
            if (E_PING_TYPE == mpsTestParams->meProtocolType)
            {
                bool bSingleResult = mIcmpSocket.Ping(mpsTestParams->mstrIperfIPAddrA,NULL,1000 * 1);
                mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,bSingleResult ? JGW_GetFormatWString(L"%.3lf",mIcmpSocket.GetPingTimeMs()).c_str() : L"");
                SendWindowMsg(L"ping %s %s",mpsTestParams->mstrIperfIPAddr.c_str(),JGW_A2W(mIcmpSocket.GetPingResult()).c_str());
                if (bSingleResult) m11ADConnectEventt.tryWait(800); 
            }
            else
            {
                //! 服务端，则开启IPERF服务程序,如果上次退出的时候有iperf3: the client has terminated提示以及Server listening on 5201 提示则表示IPERF3 SERVER不需要重新启动]
                if (E_SERVICE_TYPE == mpsTestParams->meSocketTerimalType)
                {
                    IperfServerFlowProcessing();
                }
                else
                {
                    //! 客户端 开始检查IPERF IP是否能连通，如果可以则开启IPERF进程(每次都重启IPERF进程)
                    IperfClientFlowProcessing();
                }
            }
            m11ADConnectEventt.tryWait(100);
		}
T_END:
		mbRunTestThread = false;
        mpAsyncWndMessage->PutAsyncMessage(WM_STOP_RECORD_TEST_INFO);
		mcMobilePhoneClientSocketThread.StopMobilePhoneClientSocketThread();
		mcIPQSshThread.StopIPQSshThread();
		mcIperf3Command.StopIperf3();
        Sleep(2000);
        mcMobilePhoneClientSocketThread.TerminateMobilePhoneClientSocketThread();
        mcIPQSshThread.TerminateIPQSshThread();
		mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
	}

	void CCJGW_ThroughputTestThread::IperfServerFlowProcessing()
	{
		//! 服务端，则开启IPERF服务程序,如果上次退出的时候有iperf3: the client has terminated提示以及Server listening on 5201 提示则表示IPERF3 SERVER不需要重新启动
		if (mcIPQSshThread.Check11ADConnected())
		{
			mbStopIperf3Server = true;
			//! 检查IPERF进程是否正在运行以及测试线程是否运行
			if (!mcIperf3Command.CheckIper3IsRunning() && mbRunTestThread)
			{
				//! 清空吞吐率数据
				mpAsyncWndMessage->PutAsyncMessage(WM_THROUGHPUT_RATE_MSG,NULL);
				mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
				SendWindowMsg(L"Start Iperf3 Service");
				//! 重启IPERF3进程
				if (!mcIperf3Command.StartIper3ServerToUserParameters(mpsTestParams->mstrIperfCommand,mpsTestParams->mstrIperfRelativePath))
				{
					SendWindowMsg(L"Start Iperf server Fail");
				}
			}
		}
		else
		{
			if (mbStopIperf3Server)
			{
				SendWindowMsg(L"Stop Iperf3 Service");
				mcIperf3Command.StopIperf3();
				JGW_KillProcessToName(L"iperf3.exe");
				JGW_KillProcessToName(L"iperf.exe");
				mbStopIperf3Server = false;
			}
		}
	}

	void CCJGW_ThroughputTestThread::IperfClientFlowProcessing()
	{
		//! 客户端 开始检查IPERF IP是否能连通，如果可以则开启IPERF进程(每次都重启IPERF进程)
		if (mcIPQSshThread.Check11ADConnected())
		{
			if (!mcIperf3Command.CheckIper3IsRunning() && mbRunTestThread)
			{
				//! 清空吞吐率数据
				mpAsyncWndMessage->PutAsyncMessage(WM_THROUGHPUT_RATE_MSG,NULL);
				SendWindowMsg(L"Start Iperf3 Client");
				//! 2、检查对面IPERF IP是否能PING通
				//if (CheckNetworkConnection(mpsTestParams->mstrIperfIPAddr,L"",1))
				{
					mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
					if (!mcIperf3Command.StartIperf3ClientToUserParameters(mpsTestParams->mstrIperfCommand,mpsTestParams->mstrIperfRelativePath))
					{
						SendWindowMsg(L"Start Iperf Client Fail");
					}
				}
			}   
		}
		else
		{
			SendWindowMsg(L"Stop Iperf3 Client");
			mcIperf3Command.StopIperf3();
			JGW_KillProcessToName(L"iperf3.exe");
			JGW_KillProcessToName(L"iperf.exe");
		}
	}


	bool HandIperfTestLogSpeed(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,float& speed)
	{
		//! [SUM]   0.00-1.00   sec  1.40 GBytes  12062 Mbits/sec
		if (std::wstring::npos != strIperfLog.find(L"sec") && std::wstring::npos != strIperfLog.find(L"s/sec") && strIperfLog[0] == '[')
		{
			int id = 0;
			id = strIperfLog.find(L']',2) - 1;
			strID = strIperfLog.substr(1,id);
			start = _ttoi(strIperfLog.substr(id + 2).c_str());
			id = strIperfLog.find(L'-',id);
			end = _ttoi(strIperfLog.substr(id + 1).c_str());
			id = strIperfLog.find(L"s/sec",id + 1);
			id = strIperfLog.rfind(L's',id - 1);
			speed = static_cast<float>(_ttof (strIperfLog.substr(id + 1).c_str()));
			return true;
		}
		return false;
	}
	//! 原生LOG
	//! iperf3: interrupt - the client has terminated
	//! iperf3: error - the server has terminated
	//! iperf3: interrupt - the server has terminated
	void CCJGW_ThroughputTestThread::Iperf3RawTestLogCallbackFunc(const std::wstring& strRawLog,void* pContext)
	{
		CCJGW_ThroughputTestThread* pThroughputTestThread = (CCJGW_ThroughputTestThread*)pContext;
		if (NULL == pThroughputTestThread || !pThroughputTestThread->mbRunTestThread) return ;

		static int mnConnectedCount;
		static bool mbMultiThread = false;
		std::wstring strID;
		size_t start = 0,end = 0;
		float fSpeedMbitssec = 0.00f;
#ifdef _DEBUG
		OutputDebugStringW(strRawLog.c_str());
		OutputDebugStringW(L"\n");
#endif  
		Log4WI(strRawLog.c_str());
		pThroughputTestThread->SendWindowMsg(strRawLog);
		//if (NULL != JGW_WStrCaseWStr(strRawLog.c_str(),L"Server listening on"))
		//{
		//    pThroughputTestThread->mbStopIperf3Server = false;
		//}

		if (NULL != JGW_WStrCaseWStr(strRawLog.c_str(),L"connected to")) mnConnectedCount++;
		if (mnConnectedCount > 1) mbMultiThread = true;
		if (1 == mnConnectedCount) mbMultiThread = false;
		if (mbMultiThread && NULL == JGW_WStrCaseWStr(strRawLog.c_str(),L"[SUM]")) return;
		if (!HandIperfTestLogSpeed(strRawLog,strID,start,end,fSpeedMbitssec)) return;
		mnConnectedCount = 0;

		if (end == start + 1)
		{
			std::wstring strTemp;
			JGW_FormatWString(strTemp,L"%.2f",fSpeedMbitssec);
			pThroughputTestThread->mpAsyncWndMessage->PutAsyncStringMessage(WM_THROUGHPUT_RATE_MSG,strTemp.c_str());
			pThroughputTestThread->mbStopIperf3Server = true;
		}
		else
		{
			pThroughputTestThread->mbStopIperf3Server = false;
		}

	}

	void CCJGW_ThroughputTestThread::StopThroughputTestThread()
	{
		mbRunTestThread = false;
		mcIperf3Command.StopIperf3();
		JGW_KillProcessToName(L"iperf3.exe");
		JGW_KillProcessToName(L"iperf.exe");
		Log4WI(L"Stop Throughput Test Thread");
	}

	bool CCJGW_ThroughputTestThread::CheckNetworkConnection(const std::wstring& strIPAddr,const std::wstring& strSourceAddr /* = L"" */,int nTestTimeSec /* = 30 */,int nTestContinuousCount /* = 2 */)
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
					//SendWindowMsg(L"ping %s: seq=%d ttl=%u time=%.3lf ms",strIPAddr.c_str(),mIcmpSocket.GetPingSeq(),mIcmpSocket.GetPingTTL(),mIcmpSocket.GetPingTimeMs());
					JGW_FormatWString(strTemp,L"%.3lf",mIcmpSocket.GetPingTimeMs());
					mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,strTemp.c_str());
				}
				else
				{
					//SendWindowMsg(L"ping %s time out...",strIPAddr.c_str());
					mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
				}
				bResult &= bSingleResult;
				Sleep(900);
			}
		}

		return bResult;
	}

	//    void CCJGW_ThroughputTestThread::Iperf3TestSpeedCallBack(float fSpeedMbitssec,void* pContext)
	//    {
	//#if 0
	//        CCJGW_ThroughputTestTool2Controller* pThis = (CCJGW_ThroughputTestTool2Controller*)pContext;
	//        CCJGW_AsyncWndMessage* pAsyncWndMessage = pThis->mpAsyncWndMessage;
	//        std::wstring strTemp;
	//        JGW_FormatWString(strTemp,L"%.2f Mbits/sec",fSpeedMbitssec);
	//        gfTestSpeed = fSpeedMbitssec;
	//        pAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());
	//#else
	//        CCJGW_AsyncWndMessage* pAsyncWndMessage = (CCJGW_AsyncWndMessage*)pContext;
	//        std::wstring strTemp;
	//        JGW_FormatWString(strTemp,L"%.2f",fSpeedMbitssec);
	//        //gfTestSpeed = fSpeedMbitssec;
	//        pAsyncWndMessage->PutAsyncStringMessage(WM_THROUGHPUT_RATE_MSG,strTemp.c_str());
	//#endif
	//    }
	void CCJGW_ThroughputTestThread::SendWindowMsg(const std::wstring& strMessage)
	{
		Log4WI(strMessage.c_str());
		mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMessage.c_str());
	}

	void CCJGW_ThroughputTestThread::SendWindowMsg(const wchar_t* szFormat,...)
	{
		std::wstring strLog;
		FORMAT_WSTRING(strLog,szFormat);
		Log4WI(strLog.c_str());
		mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
	}
}

