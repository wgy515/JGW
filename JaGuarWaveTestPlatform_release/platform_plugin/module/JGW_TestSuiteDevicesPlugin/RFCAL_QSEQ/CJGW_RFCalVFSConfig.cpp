#include "StdAfx.h"
#include "CJGW_RFQualcommInterface.h"
#include "CJGW_RFCalVFSConfig.h"

#define IsNull(x,result)\
	if( !(x) )\
	return result;
namespace JGW
{
	CCJGW_RFCalVFSConfig::CCJGW_RFCalVFSConfig(void)
	{
	}


	CCJGW_RFCalVFSConfig::~CCJGW_RFCalVFSConfig(void)
	{
	}

	bool CCJGW_RFCalVFSConfig::PareseXmlToXttConfig(const std::string strXmlFilePath)
	{
		std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
		TiXmlElement* xmlRoot = NULL;

		if ( !ptrXmlDoc->LoadFile( strXmlFilePath.c_str() ) )
		{
			PELOG4WA_F("没有找到配置XML文件(%s)",strXmlFilePath.c_str());
            return false;
		}

		if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllTestProjectNode(xmlRoot) )
		{
			PELOG4WW(L"解析XML文件失败");
            return false;
		}

		return true;
	}

	bool CCJGW_RFCalVFSConfig::PareseAllTestProjectNode(const TiXmlElement* xmlRoot)
	{
		const TiXmlElement*	pTempXmlNode = NULL;
		pTempXmlNode = xmlRoot->FirstChildElement("TestProject");
		IsNull(pTempXmlNode,false);

		while ( pTempXmlNode )
		{
			IsNull(PareseSingleProjectNode(pTempXmlNode),false);
			pTempXmlNode = pTempXmlNode->NextSiblingElement();
		}
		return true;
	}

	bool CCJGW_RFCalVFSConfig::PareseSingleProjectNode(const TiXmlElement* xmlNode)
	{
		const char* pszTemp = NULL;
		const TiXmlElement*	pXmlTemp = NULL;
		const TiXmlElement*	pTempXmlNode = NULL;
		STestProject_STRUCT stestProject;
		//是否测试子节点
		pszTemp = xmlNode->Attribute("IsTest");
		if(pszTemp != NULL)
			stestProject.m_bTest = (atoi(pszTemp) == 0)?false:true;
		IsNull(stestProject.m_bTest,true);

		//! 当前测试结束后动作
		pszTemp = xmlNode->Attribute("IsFinalize");
		if(pszTemp != NULL)
			stestProject.m_bDestory = (atoi(pszTemp) == 0)?false:true;
		
		//测试名，针对中文字符 需要从UTF-8转码到GB2312
		pszTemp = xmlNode->Attribute("TestName");
		IsNull(pszTemp,false);
		stestProject.TestName = pszTemp;
		
		//类名，Cal dll 类名函数
		pszTemp = xmlNode->Attribute("RealName");
		IsNull(pszTemp,false);
		stestProject.RealName = pszTemp;

		//子节点数目，用于校验
		pszTemp = xmlNode->Attribute("NumParams");
		IsNull(pszTemp,false);
		stestProject.m_lNumParams = atol(pszTemp);
		if( stestProject.m_lNumParams == 0 ) 
		{
			m_vTestProject.push_back(stestProject);
				return true;
		}

		IsNull(xmlNode,false);
		pTempXmlNode = xmlNode->FirstChildElement("Parameters");
		IsNull(pTempXmlNode,false);

		while(NULL  != pTempXmlNode)
		{
			SParameters_STRUCT sparameters;
			PareseParametersNode(sparameters,pTempXmlNode);

			pTempXmlNode = pTempXmlNode->NextSiblingElement("Parameters");
			stestProject.m_vParameters.push_back(sparameters);
		}
		IsNull(stestProject.m_vParameters.size() == stestProject.m_lNumParams,false);
		m_vTestProject.push_back(stestProject);
		return true;
	}

	bool CCJGW_RFCalVFSConfig::PareseParametersNode(SParameters_STRUCT& sParameter,const TiXmlElement* pNode)
	{
		const char * pszTemp = NULL;
		const TiXmlElement *pXmlTemp = NULL;
		IsNull(pNode,false);

		pszTemp = pNode->Attribute("paramName");
		sParameter.paramName = pszTemp;

		pszTemp = pNode->Attribute("paramVal");
		sParameter.paramVal = pszTemp;

		pszTemp = pNode->Attribute("upper");
		if(pszTemp)
			sParameter.upper = pszTemp;
		else
			sParameter.upper = "";

		pszTemp = pNode->Attribute("lower");
		if(pszTemp)
			sParameter.lower = pszTemp;
		else
			sParameter.lower = "";

		pszTemp = pNode->Attribute("mode");
		sParameter.mode = pszTemp;

		return true;
	}
}

