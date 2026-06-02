#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_RecordTestTimeLog : public CTSE_TestBase
    {
    public:
        CTSE_RecordTestTimeLog(void);
        ~CTSE_RecordTestTimeLog(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIsNewLine;
        //! 是否记录时间
        bool mbIsRecordTime;
        //! 是否计算时间间隔
        bool mbIsETime;
        std::wstring mstrTestLog;
        //! 保存当前测试时间
        std::wstring mstrStartTestTimeEnvironment;
        std::wstring mstrTestTimeLogEnvironment;
    };
}

