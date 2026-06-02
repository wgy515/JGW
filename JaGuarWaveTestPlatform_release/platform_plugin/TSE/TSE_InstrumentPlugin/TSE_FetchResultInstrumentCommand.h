#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CTSE_FetchResultInstrumentCommand : public CTSE_TestBase
    {
    public:
        CTSE_FetchResultInstrumentCommand(void);
        ~CTSE_FetchResultInstrumentCommand(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring FormatFetchCommand(const std::wstring& strShowTestResultCommand);
        bool CheckTestResult(float fTestValue,const std::wstring& strTestRange,const std::wstring& strShowTestResult,const std::wstring& strSaveTestResultEnvironment,const std::wstring& strMESLogAppName,const std::wstring& strMESLogKeyName);
    private:
        bool mbIsRecordMesLog;
        //! 默认分隔符
        std::wstring mstrSeparator;
        //! 测试范围格式PowerRange,RssiRange == 20~23 (LL ~ UL) 上限以及下限
        std::wstring mstrTestRangeParamNames;
        //! 显示测试信息格式比如 
        //! B23\tCh25600\tTxPower\t20\t22\t23 PASS
        //! B23\tCh25600\tTxPower\t20\t22\t23 FAIL
        std::wstring mstrShowTestResultForamts;
        //! 测试结果索引，默认0,1,-7.652496E+000,,,,, 以0当作开始索引
        std::wstring mstrTestResultIndexs;
        //! 保存当前测试结果环境变量 TSE_$$_BAND_$$_YX_POWER
        std::wstring mstrSaveTestResultEnvironments;
        //! 返回值OK的正则表达式
        std::wstring mstrFetchResultOkRegExp;
        std::wstring mstrFetchInstrumentCommand;
        std::wstring mstrInstrumentEnvironment;
        std::map<std::wstring,std::wstring> mmapParams;
        std::wstring mstrMESLogPathEnvionment;
        std::wstring mstrMESLogAppName;
        std::wstring mstrMESLogKeyNames;
        CCJGW_ConfigIni mConfigIni;
    };
}

