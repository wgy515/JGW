#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>

#include <fstream>

namespace JGW
{
    class CTSE_IperfSSHTest : public CTSE_TestBase
    {
    public:
        CTSE_IperfSSHTest(void);
        ~CTSE_IperfSSHTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit();
    private:
        void GetEnvironmentCommand(std::wstring& strCommand);
        void BuildIperfTestLogPath();
        //!bool ReadIperfCMDProcessPipe(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec /* = 10 */);
        bool ReadSSHIperfCMD(std::wstring& strRead,size_t nTimeOutSec = 10);

        bool HandIperfTestLog(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,double& speed);
        void UpdateIperfLogInfo(const std::wstring& strBuf);
    private:
        bool mbTestStatus;
        bool mbSaveTestLogging;
        bool mbMultThread;
        //std::streamoff mnSeekgLog;
        size_t mnTestTotalTimes;
        float mfTestMinSpeed;
        float mfTestSpeed;
        std::wstring mstrIperfTestLogPath;
        std::wstring mstrIperfCommand;
        std::wstring mstrIperfLogEnviroment;
        std::wstring mstrIperfTestRangeEnvironment;
        std::wstring mstrIperfTestTimeEnvironment;
        std::wstring mstrIperfCommandTimeOutEnvironment;
        std::wstring mstrSNEnvironment;
        std::wofstream mofstream;
        CCJGW_MoveTestLogging mMoveTestLogging;
    };
}