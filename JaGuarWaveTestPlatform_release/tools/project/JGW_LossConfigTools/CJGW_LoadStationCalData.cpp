#include "StdAfx.h"
#include "CJGW_LoadStationCalData.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_LoadStationCalData::CCJGW_LoadStationCalData(void)
    {
    }


    CCJGW_LoadStationCalData::~CCJGW_LoadStationCalData(void)
    {
    }

    std::wstring CCJGW_LoadStationCalData::GetLossConfigXmlPath()
    {
        std::wstring strTemp = JGW_GetApplicationFolder();
        strTemp += mpsEquipProjectConfig->mstrModuleFolder;
        strTemp += L"\\";;
        strTemp += mpsEquipProjectConfig->mstrConfigFileName;
        mstrConfigFilePath = JGW_W2A(strTemp);
        return strTemp;
    }


    bool CCJGW_LoadStationCalData::LoadLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig)
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

    bool CCJGW_LoadStationCalData::PareseRootNode(TiXmlElement* xmlRoot)
    {
        //! Path
        S_CAL_CONFIG_NODE sCalConfigNode;
        sCalConfigNode.mstrCalConfigName = L"Path";
        TiXmlElement* pPathNode = xmlRoot->FirstChildElement("Path");
        while (pPathNode)
        {
            S_CAL_PATH_NODE sCalPathNode;
            sCalPathNode.mnNumber = 0;
            GetElementTextValue(pPathNode,"PathName",sCalPathNode.mstrCalConfigName);
            TiXmlElement* pDataListNode = pPathNode->FirstChildElement("DataList");
            if (sCalPathNode.mstrCalConfigName.empty() || !pDataListNode)
            {
                pPathNode = pPathNode->NextSiblingElement("Path");
                continue;
            }
            TiXmlElement* pDataNode = pDataListNode->FirstChildElement("Data");
            while (pDataNode)
            {
                S_CAL_POINT_NODE sCalPointNode;
                GetElementTextValue(pDataNode,"Frequency",sCalPointNode.mfFreq);
                GetElementTextValue(pDataNode,"Value",sCalPointNode.mstrLoss);
                sCalPathNode.mvCalPoint.push_back(sCalPointNode);
                pDataNode = pDataNode->NextSiblingElement("Data");
            }
            sCalConfigNode.mvCalPathNode.push_back(sCalPathNode);
            pPathNode = pPathNode->NextSiblingElement("Path");
        }
        mpsCalDBLossConfig->mvCalConfigNode.push_back(sCalConfigNode);
        return true;
    }

    bool CCJGW_LoadStationCalData::SaveLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig)
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

    bool CCJGW_LoadStationCalData::SaveRootNode(TiXmlElement* xmlRoot)
    {
        if (mpsCalDBLossConfig->mvCalConfigNode.empty() || mpsCalDBLossConfig->mvCalConfigNode[0].mvCalPathNode.empty())
        {
            return false;
        }
        TiXmlElement* pPathNode = xmlRoot->FirstChildElement("Path");
        if (NULL == pPathNode)
        {
            return false;
        }
        for (std::vector<S_CAL_PATH_NODE>::iterator it = mpsCalDBLossConfig->mvCalConfigNode[0].mvCalPathNode.begin();it != mpsCalDBLossConfig->mvCalConfigNode[0].mvCalPathNode.end();++ it)
        {
            pPathNode = xmlRoot->FirstChildElement("Path");
            while (pPathNode)
            {
                std::wstring strPathName;
                GetElementTextValue(pPathNode,"PathName",strPathName);
                if (0 == strPathName.compare(it->mstrCalConfigName))
                {
                    TiXmlElement* pDataListNode = pPathNode->FirstChildElement("DataList");
                    if (NULL == pDataListNode)
                    {
                        return false;    ;
                    }
                    for (std::vector<S_CAL_POINT_NODE>::iterator itPoint = it->mvCalPoint.begin();
                        itPoint != it->mvCalPoint.end();
                        ++ itPoint)
                    {
                        TiXmlElement* pDataNode = pDataListNode->FirstChildElement("Data");
                        while (pDataNode)
                        {
                            std::wstring strFrequency;
                            GetElementTextValue(pDataNode,"Frequency",strFrequency);
                            if (0 == strFrequency.compare(itPoint->mfFreq))
                            {
                                SetElementTextValue(pDataNode,"Value",itPoint->mstrLoss);
                            }
                            pDataNode = pDataNode->NextSiblingElement("Data");
                        }
                    }
                }
                pPathNode = pPathNode->NextSiblingElement("Path");
            }
        }

        return true;
    }
}

