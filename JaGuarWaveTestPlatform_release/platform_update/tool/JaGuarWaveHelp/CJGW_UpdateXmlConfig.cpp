#include "StdAfx.h"
#include <JaGuarWaveHelp/CJGW_UpdateXmlConfig.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_UpdateXmlConfig::CCJGW_UpdateXmlConfig(void)
    {
    }


    CCJGW_UpdateXmlConfig::~CCJGW_UpdateXmlConfig(void)
    {
    }

    bool CCJGW_UpdateXmlConfig::LoadUpdateXmlConfig(const std::wstring& strUpdateXmlFilePath,S_JGW_HELP_UPDATE_XML_CONFIG& sUpdateXmlConfig)
    {
        sUpdateXmlConfig.mvsHelpUpdatePackage.clear();
        sUpdateXmlConfig.mbVersieLimiet = false;
        std::unique_ptr<TiXmlDocument> ptrXmlDoc(new TiXmlDocument);
        TiXmlElement* pRoot = NULL,*pPackageNodeTemp = NULL;

        if ( !ptrXmlDoc->LoadFile(JGW_W2A(strUpdateXmlFilePath)))
        {
            return false;
        }
        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;
        std::wstring strTemp;
        GetElementAttributeValue(pRoot,"versielimiet",strTemp,L"0");
        sUpdateXmlConfig.mbVersieLimiet = (1 == _ttoi(strTemp.c_str()));

        pPackageNodeTemp = pRoot->FirstChildElement("package");
        S_JGW_HELP_UPDATE_PACKAGE sUpdatePackage;
        while (pPackageNodeTemp)
        {
            LoadPackageNode(pPackageNodeTemp,sUpdatePackage);
            sUpdateXmlConfig.mvsHelpUpdatePackage.push_back(sUpdatePackage);
            pPackageNodeTemp = pPackageNodeTemp->NextSiblingElement("package");
        }
        return true;
    }

    void CCJGW_UpdateXmlConfig::LoadPackageNode(TiXmlElement* pPackageNode,S_JGW_HELP_UPDATE_PACKAGE& sUpdatePackage)
    {
        TiXmlElement* pUpdateNode = NULL;
        GetElementAttributeValue(pPackageNode,"install_version",sUpdatePackage.mstrInstallVersion);
        pUpdateNode = pPackageNode->FirstChildElement("update");
        if (!pUpdateNode) return ;
        GetElementAttributeValue(pUpdateNode,"version",sUpdatePackage.msUpdateInfo.mstrVersion);
        GetElementAttributeValue(pUpdateNode,"updatepack",sUpdatePackage.msUpdateInfo.mstrUpdatePack);
        GetElementAttributeValue(pUpdateNode,"updatepackmd5",sUpdatePackage.msUpdateInfo.mstrUpdatePackMD5);
    }
}
