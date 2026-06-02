#pragma once
#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>
#include "PrsVendorRoamingDataDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>

#include <JGW_SSHPlugin/CJGW_SSHShell.h>
#include <JGW_SSHPlugin/CJGW_SSHSCP.h>
#include "CJGW_PrsVendorRoamingToolDlgController.h"

#include <poco/Event.h>

namespace JGW
{
    class CCJGW_IPQSshThread
    {
    public:
        CCJGW_IPQSshThread(bool& bRunTestThread,S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage);
        ~CCJGW_IPQSshThread(void);
    public:
        bool StartIPQSshThread();
        void StopIPQSshThread();
        bool Check11ADConnected();
        void TerminateIPQSshThread();
    private:
        //! 读取GPS以及速率回调请求
        static void OnReadWriteSSHThreadCallback(void *lpParam);
        //! 读写客户端SOCKET线程回调
        void ReadWriteSSHThreadCallback();
        //! 导出IPQ日志
        bool ExportIPQLog();
        //! 读取RSSI MCS TX SECTOR RX SECTOR
        bool GetRssiMcsTRxSectorUbusPrsLinkeInfoCommand(int& nRssi,int& nMcs,int& nTxSector,int& nRxSector);
        //! 
        bool GetRssiMcs(int& nRssi,int& nMcs);
        //!
        bool GetTRxSector(int& nTxSector,int& nRxSector);
        //!
        int GetPerasoSector(const std::string& strCommand,int nSectorRow);
    private:
        //! 发送测试消息到窗口
        void SendWindowMsg(const std::wstring& strMessage);
        void SendWindowMsg(const wchar_t* szFormat,...);
        //! 检查网络连接
        bool CheckNetworkConnection(const std::wstring& strIPAddr,const std::wstring& strSourceAddr = L"",int nTestTimeSec = 30,int nTestContinuousCount = 2);
        //! 执行SSHCOAMMND
        bool ExecuteSshCommand(S_SSH_COMMAND & sSshCommand);
    private:
        //! 检查IPQ SSH是否开启LOG日志
        bool CheckStartSSHIPQLog(const std::string& strCommand,const std::string& strCheckCommand,const std::string& strSuccessResult);
        //!
        bool StopSSHIPQLog(const std::string& strCommand,const std::string& strSuccessResult);
        //! cat /proc/kmsg > file_1.log
        bool StartProcKmsgLog();
        //! logread -f > log_1.log
        bool StartLogReadLog();
        //!
        bool StopIPQLog();
    private:
        bool mbIsExitIPQSshThread;
        bool mbIsUseUbusPrsLinkeInfoCommand;
        bool mb11ADIsConnected;
        bool& mbRunTestThread;
        std::string mstrProcKmsgLogPath;
        std::string mstrLogReadLogPath;
        S_TestParams* mpsTestParams;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        CCJGW_ICMPSocket mIcmpSocket;
        CCJGW_SimpleThread mcReadWriteSSHThread;
        CCJGW_SSHShell mcSSHShell;
        CCJGW_SSHSCP mcSSHScp;
        //Poco::Event& m11ADConnectEventt;
    };
}
