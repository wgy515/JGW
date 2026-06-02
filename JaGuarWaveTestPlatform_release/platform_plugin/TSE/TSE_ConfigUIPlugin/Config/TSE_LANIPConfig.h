#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
	class CTSE_LANIPConfig : public CTSE_TestBase
	{
	public:
		CTSE_LANIPConfig(void);
		~CTSE_LANIPConfig(void);
    public:
        virtual const wchar_t* TSE_GetParamDescription();
	private:
		bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
		bool TSE_Run();
	private:
		int mnOperID;
		std::wstring mstrLANIPAddress;
		std::wstring mstrBroadcastIPAddress;
		std::wstring mstrEnvironmentName;
	};
}