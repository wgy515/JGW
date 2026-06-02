#include "StdAfx.h"
#include "CJGW_LoadCalDBLossConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_LoadCalDBLossConfig::CCJGW_LoadCalDBLossConfig(void) : mpsCalDBLossConfig(NULL)
    {
    }


    CCJGW_LoadCalDBLossConfig::~CCJGW_LoadCalDBLossConfig(void)
    {
    }

    std::wstring CCJGW_LoadCalDBLossConfig::GetLossConfigXmlPath()
    {
        std::wstring strTemp = JGW_GetApplicationFolder();
        strTemp += mpsEquipProjectConfig->mstrModuleFolder;
        strTemp += L"\\";;
        strTemp += mpsEquipProjectConfig->mstrConfigFileName;
        mstrConfigFilePath = JGW_W2A(strTemp);
        return strTemp;
    }

    bool CCJGW_LoadCalDBLossConfig::LoadLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig)
    {
        mpsCalDBLossConfig = &sCalDBLossConfig;
        mpsEquipProjectConfig = psEquipProjectConfig;
        GetLossConfigXmlPath();

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(mstrConfigFilePath.c_str())) return false;
        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseRootNode(xmlRoot))  return false;
        return true;
    }

    bool CCJGW_LoadCalDBLossConfig::SaveLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig)
    {
        mpsCalDBLossConfig = &sCalDBLossConfig;
        mpsEquipProjectConfig = psEquipProjectConfig;
        GetLossConfigXmlPath();

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(mstrConfigFilePath.c_str())) return false;
        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !SaveRootNode(xmlRoot))  return false;

        ptrXmlDoc->SaveFile(mstrConfigFilePath);

        return true;
    }

    bool CCJGW_LoadCalDBLossConfig::PareseRootNode(TiXmlElement* xmlRoot)
    {
        TiXmlElement* pCalConfigNode = NULL,*pCalPathNode = NULL,*pCalPointNode = NULL;
        S_CAL_CONFIG_NODE sCalConfigNode;
        S_CAL_PATH_NODE sCalPathNode;
        S_CAL_POINT_NODE sCalPointNode;
        std::wstring strTemp;

        pCalConfigNode = xmlRoot->FirstChildElement("CalConfig");
        while (pCalConfigNode)
        {  
            GetElementAttributeValue(pCalConfigNode,"name",sCalConfigNode.mstrCalConfigName);
            sCalConfigNode.mvCalPathNode.clear();
            pCalPathNode = pCalConfigNode->FirstChildElement("CalPath");
            while (pCalPathNode)
            {        
                GetElementAttributeValue(pCalPathNode,"name",sCalPathNode.mstrCalConfigName);
                GetElementAttributeValue(pCalPathNode,"number",strTemp);
                sCalPathNode.mnNumber = _ttoi(strTemp.c_str());
                sCalPathNode.mvCalPoint.clear();
                //! 排除掉不使用的LOSS配置
                if (!(sCalPathNode.mnNumber == 4 || sCalPathNode.mnNumber == 5 || sCalPathNode.mnNumber == 14)) 
                {
                    pCalPathNode = pCalPathNode->NextSiblingElement("CalPath");
                    continue;
                }

                pCalPointNode = pCalPathNode->FirstChildElement("CalPoint");
                while (pCalPointNode)
                {
                    GetElementAttributeValue(pCalPointNode,"freq",strTemp);
                    sCalPointNode.mfFreq = strTemp/*_ttof(strTmep.c_str())*/;
                    GetElementAttributeValue(pCalPointNode,"loss",strTemp);
                    sCalPointNode.mstrLoss = strTemp/*_ttof(strTmep.c_str())*/;
                    sCalPathNode.mvCalPoint.push_back(sCalPointNode);
                    pCalPointNode = pCalPointNode->NextSiblingElement("CalPoint");
                }
                sCalConfigNode.mvCalPathNode.push_back(sCalPathNode);
                pCalPathNode = pCalPathNode->NextSiblingElement("CalPath");
            }
            if (!sCalConfigNode.mvCalPathNode.empty()) mpsCalDBLossConfig->mvCalConfigNode.push_back(sCalConfigNode);
            pCalConfigNode = pCalConfigNode->NextSiblingElement("CalConfig");
        }

        return true;
    }

    bool CCJGW_LoadCalDBLossConfig::SaveRootNode(TiXmlElement* xmlRoot)
    {
        TiXmlElement* pCalConfigNode = NULL,*pCalPathNode = NULL,*pCalPointNode = NULL;
        std::wstring strTemp,strNumber;

        for (std::vector<S_CAL_CONFIG_NODE>::iterator it = mpsCalDBLossConfig->mvCalConfigNode.begin();
            it != mpsCalDBLossConfig->mvCalConfigNode.end();
            ++ it)
        {
            pCalConfigNode = xmlRoot->FirstChildElement("CalConfig");
            while (pCalConfigNode)
            {
                GetElementAttributeValue(pCalConfigNode,"name",strTemp);
                if (NULL == JGW_WStrComparenoCaseWStr(it->mstrCalConfigName.c_str(),strTemp.c_str()))
                {
                    for (std::vector<S_CAL_PATH_NODE>::iterator itPath = it->mvCalPathNode.begin();
                        itPath != it->mvCalPathNode.end();
                        ++ itPath)
                    {
                        pCalPathNode = pCalConfigNode->FirstChildElement("CalPath");
                        while (pCalPathNode)
                        {
                            GetElementAttributeValue(pCalPathNode,"name",strTemp);
                            GetElementAttributeValue(pCalPathNode,"number",strNumber);
                            if (NULL == JGW_WStrComparenoCaseWStr(itPath->mstrCalConfigName.c_str(),strTemp.c_str()) && itPath->mnNumber == _ttoi(strNumber.c_str()))
                            {
                                for (std::vector<S_CAL_POINT_NODE>::iterator itPoint = itPath->mvCalPoint.begin();
                                    itPoint != itPath->mvCalPoint.end();
                                    ++ itPoint)
                                {
                                    pCalPointNode = pCalPathNode->FirstChildElement("CalPoint");
                                    while (pCalPointNode)
                                    {
                                        GetElementAttributeValue(pCalPointNode,"freq",strTemp);
                                        if (0 == itPoint->mfFreq.compare(strTemp))
                                        {
                                            GetElementAttributeValue(pCalPointNode,"loss",strTemp);
                                            if (0 != itPoint->mstrLoss.compare(strTemp))
                                            {
                                                SetElementAttributeValue(pCalPointNode,"loss",itPoint->mstrLoss);
                                            }
                                        }
                                        pCalPointNode = pCalPointNode->NextSiblingElement("CalPoint");
                                    }
                                }
                            }
                            pCalPathNode = pCalPathNode->NextSiblingElement("CalPath");
                        }       
                    }
                }    
                pCalConfigNode = pCalConfigNode->NextSiblingElement("CalConfig");
            }
        }
        return true;
    }
}