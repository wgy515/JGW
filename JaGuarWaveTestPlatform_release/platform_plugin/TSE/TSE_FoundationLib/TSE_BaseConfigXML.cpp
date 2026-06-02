#include "StdAfx.h"
#include <TSE_FoundationLib/TSE_BaseConfigXML.h>
#include <memory>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <TSE_FoundationLib/TSE_GlobalResources.h>
namespace JGW
{
    CTSE_BaseConfigXML::CTSE_BaseConfigXML(std::vector<S_TSE_BASE_ITEM>& vTSEBaseItems) : mvTSEBaseItems(vTSEBaseItems)
    {
    }

    CTSE_BaseConfigXML::~CTSE_BaseConfigXML(void)
    {
    }

    void CTSE_BaseConfigXML::SaveLanSuiteConfig(const std::string& strSuiteConfigPath)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot,*pCommonNode,*pConfigNode = NULL;
        if (!ptrXmlDoc->LoadFile(strSuiteConfigPath.c_str())) return ;

        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return ;

        std::string strNodeName ("common");
        pCommonNode = pRoot->FirstChildElement(strNodeName);
        if (NULL == pCommonNode)
        {
            strNodeName = "GlobalVariables";
            pCommonNode = pRoot->FirstChildElement(strNodeName);
        }

        while (pCommonNode)
        {
            for (size_t i = 0;i < mvTSEBaseItems.size();i ++)
            {
                SetElementTextValue(pCommonNode,JGW_W2A(mvTSEBaseItems[i].mstrValueEnvironment).c_str(),GetGlobalEnvironment()->GetString(mvTSEBaseItems[i].mstrValueEnvironment));
            }
            pCommonNode = pCommonNode->NextSiblingElement(strNodeName);
        }
        ptrXmlDoc->SaveFile(strSuiteConfigPath);
    }
}