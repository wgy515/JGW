#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_PerasoSSHRSSITest : public CTSE_TestBase
    {
    public:
        CTSE_PerasoSSHRSSITest(void);
        ~CTSE_PerasoSSHRSSITest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit();
    private:
        //! 获取RSSI
        bool GetRSSI(std::vector<int>& vRSSI,int& rssi);
        //! 计算RSSI
        int CalcRSSIAverage(std::vector<int>& vRSSI,const int max_rssi,const int min_rssi);
        //!
        void BuildIperfTestLogPath();
        //!
        void BuildTestStandard();
    private:
        bool mbInitLogFile; //! 是否重新初始化log文件
        bool mbSaveLog;
        int mnTestCount;
        int mnMinRSSI;
        int mnMaxRSSI;
        int mnTestRSSI;
        std::string mstrRssiCommand;
        std::wstring mstrRssiCommandEnvironment;
        std::wstring mstrSectorLog;
        std::wstring mstrTestStandard;
        std::wstring mstrSerialNumberEnvironment;
        static std::wstring mstrPerasoRSSITestLogPath;
        std::wstring mstrSaveLogPathEnvironment;
        std::vector<int> mvRSSI;
    };
}


