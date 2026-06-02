#include "StdAfx.h"
#include "TSE_LoadWlanLossConfig.h"
#include <tinyxml/tinyxml.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CTSE_LoadWlanLossConfig::CTSE_LoadWlanLossConfig(void)
    {
    }


    CTSE_LoadWlanLossConfig::~CTSE_LoadWlanLossConfig(void)
    {
    }

    const wchar_t* CTSE_LoadWlanLossConfig::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_LoadWlanLossConfig::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("WlanLossPath"),strParamName))
        {
            mstrWlanLossPath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("LossInstanceEnvionment"),strParamName))
        {
            mstrLossInstanceEnvionment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_LoadWlanLossConfig::TSE_Run()
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        GetGlobalEnvironment()->PutInt(mstrLossInstanceEnvionment,(int)this);
        if (!ptrXmlDoc->LoadFile(JGW_W2A(mstrWlanLossPath).c_str()))
        {
            LogE_F(L"Load Loss File(%s) Fail",mstrWlanLossPath.c_str());
            return false;
        }
        TiXmlElement* xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            LogE_F(L"Load Loss File(%s) Fail",mstrWlanLossPath.c_str());
            return false;
        }
        mvsLossDataList.clear();
        TiXmlElement* xmlPathNode = xmlRoot->FirstChildElement("Path");
        while (xmlPathNode)
        {
            S_LOSS_DATA_LIST sLossDataList;
            GetElementTextValue(xmlPathNode,"PathName",sLossDataList.mstrPathName);
            TiXmlElement* dataListNode = xmlPathNode->FirstChildElement("DataList");
            if (NULL == dataListNode)
            {
                LogE(L"Not Find DataList XML Node Fail");
                return false;
            }

            TiXmlElement* dataNode = dataListNode->FirstChildElement("Data");
            std::wstring strFreq,strValue,strDelta;
            while (dataNode)
            {
                GetElementTextValue(dataNode,"Frequency",strFreq);
                GetElementTextValue(dataNode,"Value",strValue,L"1");
                GetElementTextValue(dataNode,"Delta",strDelta,L"0");
                if (!strFreq.empty())
                {
                    //S_LOSS_Data sLossData = { _ttoi(strFreq.c_str()),static_cast<float>(_ttof(strValue.c_str())),static_cast<float>(_ttof(strDelta.c_str()))};
                    //sStationCalPath.mvsLossData.push_back(sLossData);
                    sLossDataList.mmapFreqLoss.insert(std::make_pair<int,float>(_ttoi(strFreq.c_str()),static_cast<float>(_ttof(strValue.c_str()))) );
                }
                dataNode = dataNode->NextSiblingElement("Data");
            }
            mvsLossDataList.push_back(sLossDataList);
            xmlPathNode = xmlPathNode->NextSiblingElement("Path");
        }
        return true;
    }

    LPS_LOSS_DATA_LIST CTSE_LoadWlanLossConfig::GetLossDataList(const std::wstring& strPathName)
    {
        for (size_t index = 0;index < mvsLossDataList.size();index++)
        {
            if (0 == JGW_WStrComparenoCaseWStr(strPathName.c_str(),mvsLossDataList[index].mstrPathName.c_str()))
            {
                return &mvsLossDataList[index];
            }
        }
        if (mvsLossDataList.size() > 0) return &mvsLossDataList[0];
        return NULL;
    }

    float CTSE_LoadWlanLossConfig::CalcLoss(int fLFreq,int fHFreq,float fLLoss,float fHLoss)
    {
        int detal = fLFreq - fHFreq;
        float lossDetal = fLLoss - fHLoss;
        if (0 == lossDetal || 0 == detal) return fLLoss;
        return fLLoss + (lossDetal / detal);
    }

    float CTSE_LoadWlanLossConfig::GetWlanLoss(const std::wstring& strPathName,int dFrequency)
    {
        LPS_LOSS_DATA_LIST psLossDataList = GetLossDataList(strPathName);
        if (NULL == psLossDataList || 0 == psLossDataList->mmapFreqLoss.size())
        {
            return 1.00f;
        }
        //! 频率一致
        std::map<int,float>::iterator it = psLossDataList->mmapFreqLoss.find(dFrequency);
        if (it != psLossDataList->mmapFreqLoss.end())
        {
            return it->second;
        }
        //! 当频率不在MAP区间内就返回第一个频率对应线损
        it = psLossDataList->mmapFreqLoss.begin();
        if (dFrequency < it->first)
        {
            return it->second;
        }
        //! 当频率超过MAP区间内就返回最后频率对应线损
        it = psLossDataList->mmapFreqLoss.end();
        -- it;
        if (dFrequency > it->first)
        {
            return it->second;
        }
        int fLowFreq = 0;;
        float fLoss = 0.00f;
        for (it = psLossDataList->mmapFreqLoss.begin();it != psLossDataList->mmapFreqLoss.end();++it)
        {
            if (dFrequency > it->first)
            {
                fLowFreq = it->first;
                fLoss = it->second;
            }
            if (dFrequency < it->first)
            {
                return CalcLoss(fLowFreq,it->first,fLoss,it->second);
            }
        }

        return 1.00f;
    }
}
