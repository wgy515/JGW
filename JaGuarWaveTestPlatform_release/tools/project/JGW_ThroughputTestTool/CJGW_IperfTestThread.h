#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_IPERF_TOOL_DEFINE.h"
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include <fstream>
#define  READ_BUFFER_MAX_LEN 1024
namespace JGW
{
    class CCJGW_IperfTestThread : public CCMessageThread
    {
    public:
        CCJGW_IperfTestThread(void);
        ~CCJGW_IperfTestThread(void);

        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        void InitIperfTestThread(WPARAM wParam,LPARAM lParam);
        void RunIperfTestThread(WPARAM wParam,LPARAM lParam);
    private:
        void ExecAttenutorTest();
        bool SendTxSector(const std::wstring& strSector); 
        bool SendRxSector(const std::wstring& strSector);

        void RelMoveTrunTable(int nTurnTableAngle);
        void AbsMoveTrunTable(int nTurnTableAngle);
        void SetAuttenuatorValue(float fAttenutorVal);
    private:
        void RunIperfTest();
        bool ConnectDonglesComPort();
    private:
        void UpdateIperfLogInfo();
        CCJGW_ProcessPipe* GetProcessCMDPipe();
        bool ReadIperfCMDProcessPipe(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec  = 10 );
        bool HandIperfTestLog(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,double& speed);
        void BuildIperfLog();
    private:
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        int mnTrunTableRelMoveAngle;
        std::wstring mstrTimeStamp;
        std::wofstream mfIperfLog;
        PS_IperfToolConfig mpsIperfToolConfig;
        
        CCJGW_CSerialComPort mcSerialComPort;
        CCJGW_DonglesComPort mcDonglesComPort;
        wchar_t szReadBuf[1024];
    };
}