#include "StdAfx.h"
#include <JaGuarWaveHelp/CJGW_InstallXmlConfig.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_InstallXmlConfig::CCJGW_InstallXmlConfig(void)
    {
    }


    CCJGW_InstallXmlConfig::~CCJGW_InstallXmlConfig(void)
    {
    }

    bool CCJGW_InstallXmlConfig::LoadInstallXmlConfig(const std::wstring& strInstallXmlFilePath,S_JGW_HELP_INSTALL_XML_CONFIG& sInstallXmlConfig)
    {
        sInstallXmlConfig.mvstrInstallInfo.clear();
        sInstallXmlConfig.msNewInstallConfig.mstrVersion = L"";
        std::unique_ptr<TiXmlDocument> ptrXmlDoc(new TiXmlDocument);
        TiXmlElement* pRoot = NULL,*pNewInstallNode = NULL,*pInstallNodeTemp = NULL;

        if ( !ptrXmlDoc->LoadFile(JGW_W2A(strInstallXmlFilePath).c_str()) )
        {
            return false;
        }

        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;

        pNewInstallNode = pRoot->FirstChildElement("new_install");
        if (pNewInstallNode)
        {
            pInstallNodeTemp = pNewInstallNode->FirstChildElement("install");
            LoadInstallNode(pInstallNodeTemp,sInstallXmlConfig.msNewInstallConfig);
            //! 安装版本包括最新的安装版本
            sInstallXmlConfig.mvstrInstallInfo.push_back(sInstallXmlConfig.msNewInstallConfig);
        }
       
        S_JGW_HELP_INSTALL sInstallInfo;
        pInstallNodeTemp = pRoot->FirstChildElement("install");
        while (pInstallNodeTemp)
        {
            LoadInstallNode(pInstallNodeTemp,sInstallInfo);
            sInstallXmlConfig.mvstrInstallInfo.push_back(sInstallInfo);
            pInstallNodeTemp = pInstallNodeTemp->NextSiblingElement("install");
        }
        return true;
    }

    void CCJGW_InstallXmlConfig::LoadInstallNode(TiXmlElement* pInstallNode,S_JGW_HELP_INSTALL& sInstallInfo)
    {
        if (!pInstallNode) return ;

        std::wstring strTemp;
        GetElementAttributeValue(pInstallNode,"version",sInstallInfo.mstrVersion);
        GetElementAttributeValue(pInstallNode,"filename",sInstallInfo.mstrFileName);
        GetElementAttributeValue(pInstallNode,"md5",sInstallInfo.mstrMD5);
        GetElementAttributeValue(pInstallNode,"timestamp",strTemp);
        sInstallInfo.mnTimeStamp = _ttoi(strTemp.c_str());
    }
}