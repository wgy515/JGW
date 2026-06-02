#include "StdAfx.h"
#include "CJGW_ResolveStationCalDataFile.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <algorithm>

namespace JGW
{
    CCJGW_ResolveStationCalDataFile::CCJGW_ResolveStationCalDataFile(void)
    {
    }


    CCJGW_ResolveStationCalDataFile::~CCJGW_ResolveStationCalDataFile(void)
    {
    }

    bool CCJGW_ResolveStationCalDataFile::ResolveStationCalDataFile(const std::wstring& strStationCalDataFilePath,std::vector<S_LOSS_PATH>& vsLossPath)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc(new TiXmlDocument);
        if (!ptrXmlDoc->LoadFile(JGW_W2A(strStationCalDataFilePath).c_str()))
        {
            return false;
        }

        TiXmlElement* xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            return false;
        }

        vsLossPath.clear();
        TiXmlElement* xmlPathNode = xmlRoot->FirstChildElement("Path");
        std::wstring strTemp;
        while (xmlPathNode)
        {
            S_LOSS_PATH sLossPath;

            GetElementTextValue(xmlPathNode,"PathName",strTemp);
            sLossPath.mstrPathName = JGW_W2A(strTemp);

            TiXmlElement* dataListNode = xmlPathNode->FirstChildElement("DataList");
            if (NULL == dataListNode)
            {
                return false;
            }
            TraverseDataListNodeNextDataNode(dataListNode,sLossPath.mvsLossData);
            std::sort(sLossPath.mvsLossData.begin(), sLossPath.mvsLossData.end(), std::less<S_LOSS_DATA>());
            vsLossPath.push_back(sLossPath);
            xmlPathNode = xmlPathNode->NextSiblingElement("Path");
        }
        return true;
    }

    float CCJGW_ResolveStationCalDataFile::GetLossToToFreq(const std::vector<S_LOSS_DATA>& vsLossData,unsigned int fFreq)
    {
        if (vsLossData.empty())
        {
            return 0.00f;
        }
       
        if (vsLossData.size() < 2)
        {
            return vsLossData[0].mfLoss;
        }

        if (vsLossData[0].muFrequency >= fFreq)
        {
            return vsLossData[0].mfLoss;
        }

        if (vsLossData[vsLossData.size() - 1].muFrequency <= fFreq)
        {
            return vsLossData[vsLossData.size() - 1].mfLoss;
        }

        for (size_t index = 1;index < vsLossData.size();index++)
        {
            if (fFreq == vsLossData[index-1].muFrequency) return vsLossData[index-1].mfLoss;
            if (fFreq == vsLossData[index].muFrequency) return vsLossData[index].mfLoss;
            if (fFreq > vsLossData[index-1].muFrequency && fFreq <  vsLossData[index].muFrequency)
            {
                if (vsLossData[index-1].mfLoss == vsLossData[index].mfLoss) return vsLossData[index-1].mfLoss;
                return (fFreq - vsLossData[index - 1].muFrequency) * (vsLossData[index].mfLoss - vsLossData[index-1].mfLoss) / (vsLossData[index].muFrequency - vsLossData[index-1].muFrequency) + vsLossData[index - 1].mfLoss;
            }
        }
        return 0.00;
    }

    void CCJGW_ResolveStationCalDataFile::UpdateFreqLoss(const std::wstring& strPathName,unsigned int uFrequency,float fPowerDiff)
    {
        for (std::vector<S_LOSS_PATH>::iterator it = mvsLossPath.begin();it != mvsLossPath.end();++it)
        {
            if (0 == JGW_StrComparenoCaseStr(JGW_W2A(strPathName).c_str(),it->mstrPathName.c_str()))
            {
                for (std::vector<S_LOSS_DATA>::iterator itLossData = it->mvsLossData.begin();itLossData != it->mvsLossData.end();++itLossData)
                {
                    if (itLossData->muFrequency == uFrequency)
                    {
                        itLossData->mfLoss += fPowerDiff;
                        itLossData->mnPosIndex ++;
                        return ;
                    }
                }
                S_LOSS_DATA sLossData = {uFrequency,fPowerDiff,0.00f};
                sLossData.mnPosIndex = 1;
                it->mvsLossData.push_back(sLossData);
                return ;
            }
        }
        S_LOSS_PATH sLossPath;
        sLossPath.mstrPathName = JGW_W2A(strPathName);
        S_LOSS_DATA sLossData = {uFrequency,fPowerDiff,0.00f};
        sLossData.mnPosIndex = 1;
        sLossPath.mvsLossData.push_back(sLossData);
        mvsLossPath.push_back(sLossPath);
        return ;
    }


    bool CCJGW_ResolveStationCalDataFile::SaveStationCalDataFile(std::wstring& strTestLogPath)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(JGW_W2A(strTestLogPath))) return false;
        if (!(xmlRoot = ptrXmlDoc->RootElement()))  return false;

        for (std::vector<S_LOSS_PATH>::iterator it = mvsLossPath.begin();it != mvsLossPath.end();++it)
        {
            for (std::vector<S_LOSS_DATA>::iterator itLossData = it->mvsLossData.begin();itLossData != it->mvsLossData.end();++itLossData)
            {
                if (itLossData->mnPosIndex > 0)
                {
                    itLossData->mfLoss /= itLossData->mnPosIndex ;
                }
            }
        }

        TiXmlElement* pPathNode = NULL,*pDataListNode = NULL,*pDataNode = NULL;
        std::wstring strTemp,strNumber,strLoss;

        for (std::vector<S_LOSS_PATH>::iterator it = mvsLossPath.begin();it != mvsLossPath.end();++it)
        {
            bool bFindPathNode = false;
            // 遍历PATH节点，检查子节点PathName的值是否一致
            pPathNode = xmlRoot->FirstChildElement("Path");
            while (pPathNode)
            {
                GetElementTextValue(pPathNode,"PathName",strTemp);
                if (0 == it->mstrPathName.compare(JGW_W2A(strTemp)))
                {
                    bFindPathNode = true;
                    break;
                }
                pPathNode = pPathNode->NextSiblingElement("Path");
            }
            // 当没有找到Path节点下子节点PathName的值，则新增Path节点
            if (!bFindPathNode)
            {
                pPathNode = AddElementTextValue(xmlRoot,"Path",L"");
                SetElementTextValue(pPathNode,"PathName",JGW_A2W(it->mstrPathName));
                SetElementTextValue(pPathNode,"PathID",L"0");
                SetElementTextValue(pPathNode,"AdditionalLoss",L"0");
                SetElementTextValue(pPathNode,"Cal_Date",L"27/4/2021 11:51 AM");
                SetElementTextValue(pPathNode,"TesterID",L"CNALLURI07291");
                SetElementTextValue(pPathNode,"CalCable_PathName",L"NA");
                SetElementTextValue(pPathNode,"UseCalCable",L"false");
            }
            // 检查Path节点下DataList节点是否存在
            pDataListNode = pPathNode->FirstChildElement("DataList");
            if (NULL == pDataListNode)
            {
                // 不存在则新增DataList节点
                pDataListNode = AddElementTextValue(pPathNode,"DataList",L"");
            }

            ChangeDataListNode(pDataListNode,it->mvsLossData);
#if 0
            for (std::vector<S_LOSS_DATA>::iterator itLossData = it->mvsLossData.begin();
                itLossData != it->mvsLossData.end();
                ++itLossData)
            {
                AddDataNode(pDataListNode,itLossData[0]);

                bool bFindDataNode = false;
                pDataNode = pDataListNode->FirstChildElement("Data");
                while(pDataNode)
                {
                    GetElementTextValue(pDataNode,"Frequency",strNumber);
                    GetElementTextValue(pDataNode,"Value",strLoss);
                    if (itLossData->muFrequency == _ttoi(strNumber.c_str()))
                    {
                        bFindDataNode = true;
                    }



                    GetElementAttributeValue(pCalPathNode,"number",strNumber);
                    if (0 == strNumber.compare(itCalPath->mstrNumber))
                    {
                        AddCalPointNode(pCalPathNode,itCalPath->mvsCalPoint);
                        bFindDataNode = true;
                        break;
                    }
                    pCalPathNode = pCalPathNode->NextSiblingElement("CalPath");
                }
                if (!bFindDataNode)
                {
                    pCalPathNode = AddElementTextValue(pCalConfigNode,"CalPath",L"");
                    AddElementAttributeValue(pCalPathNode,"number",itCalPath->mstrNumber);
                    AddElementAttributeValue(pCalPathNode,"name",itCalPath->mstrNumber);
                    AddElementAttributeValue(pCalPathNode,"date",GetLossDateTimeStampYMDHMS());
                    AddElementAttributeValue(pCalPathNode,"dayLimit",L"999");
                    AddCalPointNode(pCalPathNode,itCalPath->mvsCalPoint,bAddConfigLossOffset);
                }
            }
#endif
        }
        return ptrXmlDoc->SaveFile(JGW_W2A(strTestLogPath));
    }

    void CCJGW_ResolveStationCalDataFile::ChangeDataListNode(TiXmlElement* pDataListNode,std::vector<S_LOSS_DATA>& vsLossData,bool bAddConfigLossOffset /* = true */)
    {
        std::vector<S_LOSS_DATA> vsConfigLossData;
        // 读取CONFIG Loss
        TraverseDataListNodeNextDataNode(pDataListNode,vsConfigLossData);
        // 频率从小到大的排序
        std::sort(vsConfigLossData.begin(), vsConfigLossData.end(), std::less<S_LOSS_DATA>());

        for (size_t index = 0;index < vsLossData.size();index++)
        {
            if (bAddConfigLossOffset) vsLossData[index].mfLoss += GetLossToToFreq(vsConfigLossData,vsLossData[index].muFrequency);
        }
        // 频率从小到大的排序
        std::sort(vsLossData.begin(), vsLossData.end(), std::less<S_LOSS_DATA>());

        if (vsConfigLossData.empty())
        {
            for (size_t index = 0;index < vsLossData.size();index++)
            {
                TiXmlElement* pDataNode = new TiXmlElement("Data");
                SetElementTextValue(pDataNode,"Frequency",JGW_GetFormatWString(L"%d",vsLossData[index].muFrequency));
                SetElementTextValue(pDataNode,"Value",JGW_GetFormatWString(L"%.2f",vsLossData[index].mfLoss));
                SetElementTextValue(pDataNode,"Delta",JGW_GetFormatWString(L"%.2f",vsLossData[index].mfDelta));
                pDataListNode->LinkEndChild(pDataNode);
            }
        }
        else
        {
            // recall loss
            for (size_t index = 0;index < vsConfigLossData.size();index++)
            {
                vsConfigLossData[index].mfLoss = GetLossToToFreq(vsLossData,vsConfigLossData[index].muFrequency);
                SetElementTextValue(vsConfigLossData[index].mpDataNode,"Value",JGW_GetFormatWString(L"%.2f",vsConfigLossData[index].mfLoss));
            }
        }
    }

    void CCJGW_ResolveStationCalDataFile::TraverseDataListNodeNextDataNode(TiXmlElement* pDataListNode,std::vector<S_LOSS_DATA>& vsLossData)
    {
        // 1、遍历DataList节点下data节点
        TiXmlElement* pDataNode = pDataListNode->FirstChildElement("Data");
        std::wstring strFreq,strValue,strDelta;
        while (pDataNode)
        {
            GetElementTextValue(pDataNode,"Frequency",strFreq);
            GetElementTextValue(pDataNode,"Value",strValue,L"1");
            GetElementTextValue(pDataNode,"Delta",strDelta,L"0");
            if (!strFreq.empty())
            {
                S_LOSS_DATA sLossData = {0};
                sLossData.muFrequency = _ttoi(strFreq.c_str());
                sLossData.mfLoss = static_cast<float>(_ttof(strValue.c_str()));
                sLossData.mfDelta = static_cast<float>(_ttof(strDelta.c_str()));
                sLossData.mpDataNode = pDataNode;
                vsLossData.push_back(sLossData);
            }
            pDataNode = pDataNode->NextSiblingElement("Data");
        }
    }
}
