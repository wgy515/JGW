#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
class IEquipCommTL;
namespace JGW
{
    
    class CTSE_ReadInstrumentVoltageCurrent : public CTSE_TestBase
    {
    public:
        CTSE_ReadInstrumentVoltageCurrent(void);
        ~CTSE_ReadInstrumentVoltageCurrent(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        float GetResultToCommand(IEquipCommTL* pEquipInter,const std::string& strCommand);
        bool CheckResult(std::vector<float> vfResult,const std::wstring& strTestRangeEnvironment,const std::wstring& strTestContent);
    private:
        //! 是否检测电压
        bool mbCheckVoltage;
        //! 是否检测电流
        bool mbCheckCurrent;
        //! 间隔读取时长
        int mnCheckIntervalTimeMs;
		//! 读取计数
		int mnReadMaxCount;
        //! 检测总时长
        int mnCheckTimeSec;
		//! 读取电压倍率
		int mnReadVoltageRate;
		//! 读取电流倍率
		int mnReadCurrentRate;
        //! 读取电压命令
        std::string mstrReadVoltageCommand;
        //! 读取电流命令
        std::string mstrReadCurrentCommand;
        //! 电压测试范围
        std::wstring mstrVoltageTestRangeEnvironment;
        //! 电流测试范围
        std::wstring mstrCurrentTestRangeEnvironment;
        //!
        std::wstring mstrInstrumentEnvironment;
    };
}

