#pragma once
#include <tinyxml/tinyxml.h>

namespace JGW
{

	class CCJGW_LANSuiteConfigXML
	{
	public:
		CCJGW_LANSuiteConfigXML(void);
		~CCJGW_LANSuiteConfigXML(void);

		//! ±£¥Ê ”Õº≈‰÷√
		void SaveLanSuiteConfig(const std::string& strSuiteConfigPath,int nOperID,const std::wstring& strLANIPAddress
			,const std::wstring strBroadcastIPAddress,const std::wstring& strEnvironmentName
			,const std::wstring& strEnvironmentValue);
	private:

	};
}
