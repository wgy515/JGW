#include "StdAfx.h"
#include "CStationCalDataFile.h"

namespace JGW
{
    CCStationCalDataFile::CCStationCalDataFile(void)
    {
    }


    CCStationCalDataFile::~CCStationCalDataFile(void)
    {
    }

    bool CCStationCalDataFile::LoadStationCalDataFile(const std::string& strFilePath,std::vector<S_STATION_CAL_PATH>& vsStationCalPath)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if ( !ptrXmlDoc->LoadFile(strFilePath.c_str()))
        {
            return false;
        }

        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllPathParamNode(xmlRoot,vsStationCalPath) )
        {
            return false;
        }	

        return true;
    }

    bool CCStationCalDataFile::PareseAllPathParamNode(const TiXmlElement* xmlRoot,std::vector<S_STATION_CAL_PATH>& vsStationCalPath)
    {
        const TiXmlElement*	pTempXmlNode = NULL,*pDataListXmlNode = NULL,*pDataXmlNode = NULL;
        pTempXmlNode = xmlRoot->FirstChildElement("Path");
        S_LOSS_Data sLossData ={0};
        std::string strTemp;
        S_STATION_CAL_PATH sStationCalPath;

        while (NULL != pTempXmlNode)
        {
            sStationCalPath.mvsLossData.clear();
            GetElementTextValueA(pTempXmlNode,"PathName",sStationCalPath.mstrPathName,"BH0_LP");
            pDataListXmlNode = pTempXmlNode->FirstChildElement("DataList");

            if (NULL != pDataListXmlNode)
            {
                pDataXmlNode = pDataListXmlNode->FirstChildElement("Data");
                while (pDataXmlNode)
                {
                    GetElementTextValueA(pDataXmlNode,"Frequency",strTemp,"2412.00");
                    sLossData.mnChannel = atoi(strTemp.c_str());

                    GetElementTextValueA(pDataXmlNode,"Value",strTemp,"0");
                    sLossData.mfLoss = static_cast<float>(atof(strTemp.c_str()));

                    GetElementTextValueA(pDataXmlNode,"Delta",strTemp,"0");
                    sLossData.mfDelta = static_cast<float>(atof(strTemp.c_str()));

                    sStationCalPath.mvsLossData.push_back(sLossData);
                    pDataXmlNode = pDataXmlNode->NextSiblingElement("Data");
                }
                vsStationCalPath.push_back(sStationCalPath);
            }
            pTempXmlNode = pTempXmlNode->NextSiblingElement("Path");
        }

        return true;
    }
}

