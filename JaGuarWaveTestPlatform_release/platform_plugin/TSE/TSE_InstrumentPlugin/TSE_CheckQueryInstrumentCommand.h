#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

class IEquipCommTL;

namespace JGW
{
	class CTSE_CheckQueryInstrumentCommand : public CTSE_TestBase
	{
	public:
		CTSE_CheckQueryInstrumentCommand(void);
		~CTSE_CheckQueryInstrumentCommand(void);
	private:
		const wchar_t* TSE_GetParamDescription();
		bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
		bool TSE_Run();
	private:
		float GetResultToCommand(IEquipCommTL* pEquipInter,const std::string& strCommand);
		float GetResultToCount(IEquipCommTL* pEquipInter,const std::string& strCommand,int count);
	private:
		float mfMinRange;
		float mfMaxRange;
		//! 间隔读取时长
		int mnCheckIntervalTimeMs;
		//! 读取计数
		int mnQueryMaxCount;
		//! 每次读取几次平均
		int mnQueryAvgResultCount;
		//! 读取倍率
		int mnQueryRate;
		//! 是否记录最大值
		int mnRecordMaxValue;
		//!
		std::wstring mstrPassResultFormat;
		//! 
		std::wstring mstrFailResultFormat;
		//! 读取电压命令
		std::wstring mstrQueryCommand;
		//! 电流测试范围
		std::wstring mstrTestRangeEnvironment;
		//!
		std::wstring mstrInstrumentEnvironment;
	};

}

