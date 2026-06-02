#include "StdAfx.h"
#include "CJGW_LANSuiteConfigXML.h"
#include <memory>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
	CCJGW_LANSuiteConfigXML::CCJGW_LANSuiteConfigXML(void)
	{
	}


	CCJGW_LANSuiteConfigXML::~CCJGW_LANSuiteConfigXML(void)
	{
	}

	void CCJGW_LANSuiteConfigXML::SaveLanSuiteConfig(const std::string& strSuiteConfigPath,int nOperID,const std::wstring& strLANIPAddress ,const std::wstring strBroadcastIPAddress,const std::wstring& strEnvironmentName ,const std::wstring& strEnvironmentValue)
	{
		std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
		TiXmlElement* pRoot,*pCommonNode,*pConfigNode = NULL;
		if (!ptrXmlDoc->LoadFile(strSuiteConfigPath.c_str())) return ;

		pRoot = ptrXmlDoc->RootElement();
		if (!pRoot) return ;

		pCommonNode = pRoot->FirstChildElement("common");
		while (pCommonNode)
		{
			SetElementTextValue(pCommonNode,JGW_W2A(strEnvironmentName).c_str(),strEnvironmentValue);
			pCommonNode = pCommonNode->NextSiblingElement("common");
		}

		pConfigNode = pRoot->FirstChildElement("config");
		while (pConfigNode)
		{
			//! LANIPAddress BroadcastAddress OperID
			TiXmlElement* pLANIPAddress,*pBroadcastAddress,*pOperID = NULL;
			pOperID = pConfigNode->FirstChildElement("OperID");
			if (pOperID) SetElementAttributeValue(pOperID,"paramVal",1 == nOperID?L"1":L"0");

			if (1 == nOperID)
			{
				pLANIPAddress = pConfigNode->FirstChildElement("LANIPAddress");
				if (pLANIPAddress) SetElementAttributeValue(pLANIPAddress,"paramVal",strLANIPAddress);

				pBroadcastAddress = pConfigNode->FirstChildElement("BroadcastAddress");
				if (pBroadcastAddress) SetElementAttributeValue(pBroadcastAddress,"paramVal",strBroadcastIPAddress);
			}
			pConfigNode = pConfigNode->NextSiblingElement("config");
		}

		ptrXmlDoc->SaveFile(strSuiteConfigPath);
	}
}