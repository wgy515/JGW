#include "StdAfx.h"
#include "TSE_BaseConfig.h"
#include "TSE_BaseConfigDlg.h"
namespace JGW
{
	CTSE_BaseConfig::CTSE_BaseConfig(void) : mnConfigCount(0)
	{
	}


	CTSE_BaseConfig::~CTSE_BaseConfig(void)
	{
	}

	const wchar_t* CTSE_BaseConfig::TSE_GetParamDescription()
	{
		return L"{\
				\"RealName\":\"CTSE_BaseConfig\",\
				\"TestName\":\"TSE基础配置界面自定义\",\
				\"ConfigCount\":\"配置文本框组(包含标签和文本输入框)的数量,索引默认1开始\",\
				\"LabelName1\":\"标签名称，后面1表示文本框组的索引\",\
				\"EditValueEnvironment1\":\"输入框保存的环境变量名称，后面1表示文本框组的索引\",\
				\"LabelName2\":\"标签名称，后面2表示文本框组的索引\",\
				\"EditValueEnvironment2\":\"输入框保存的环境变量名称，后面2表示文本框组的索引\"\
				}";
	}

	bool CTSE_BaseConfig::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
	{
		if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
		if( TSE_PARAM_NAME_EQUAL(_T("ConfigCount"),strParamName) )
		{
			mnConfigCount = _ttoi(strParamValue);
		}
		else
		{
			m_mParam[strParamName] = strParamValue;
		}  
		return true;
	}

	bool CTSE_BaseConfig::TSE_Init()
	{
		std::wstring strLabelNameKey;
		std::wstring strEditValueEnvironmentKey;
		std::wstring strEditValueRateKey;
		S_TSE_BASE_ITEM sTSEBaseItem;
		mvsTSEBaseItems.clear();

		for (size_t i = 1;i <= mnConfigCount;i ++)
		{
			sTSEBaseItem.mstrLabelName = L"";
			sTSEBaseItem.mstrValueEnvironment = L"";
			sTSEBaseItem.mstrRateValue = L"";
			JGW_FormatWString(strLabelNameKey,L"LabelName%d",i);
			JGW_FormatWString(strEditValueEnvironmentKey,L"EditValueEnvironment%d",i);
			JGW_FormatWString(strEditValueRateKey,L"EditValueRate%d",i);

			if( m_mParam.end() != m_mParam.find(strLabelNameKey) )
			{
				sTSEBaseItem.mstrLabelName = m_mParam[strLabelNameKey];
			}
			if( m_mParam.end() != m_mParam.find(strEditValueEnvironmentKey) )
			{
				sTSEBaseItem.mstrValueEnvironment = m_mParam[strEditValueEnvironmentKey];
			}
			if( m_mParam.end() != m_mParam.find(strEditValueRateKey) )
			{
				sTSEBaseItem.mstrRateValue = m_mParam[strEditValueRateKey];
			}

			mvsTSEBaseItems.push_back(sTSEBaseItem);
		}

		return true;
	}

	bool CTSE_BaseConfig::TSE_Run()
	{
		if (mnConfigCount != mvsTSEBaseItems.size()) TSE_Init();
		CTSE_BaseConfigDlg lanConfigDlg(GetGlobalMainHwnd(),mvsTSEBaseItems);
		return (IDOK == lanConfigDlg.DoModal());
	}
}