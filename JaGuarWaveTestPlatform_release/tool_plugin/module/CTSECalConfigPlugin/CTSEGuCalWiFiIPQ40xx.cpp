#include "StdAfx.h"
#include "CTSEGuCalWiFiIPQ40xx.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <tinyxml/tinyxml.h>
#include <fstream>
#include <algorithm>
#include <numeric>
#define WIFI_PATHNAME_MAX 4
namespace JGW
{
    CCTSEGuCalWiFiIPQ40xx::CCTSEGuCalWiFiIPQ40xx(void) : mn5GExpectPower(14),mn24GExpectPower(15),mfGuLoss(7.00f)
    {
        mstrDefaultGuLossConfigFilePath = JGW_RealativePathToAbsPath(L"equip\\StationCalDataFile_UC600AA_GU_Loss.xml");
    }


    CCTSEGuCalWiFiIPQ40xx::~CCTSEGuCalWiFiIPQ40xx(void)
    {
    }

    std::wstring CCTSEGuCalWiFiIPQ40xx::GetDutTestDataFilePath()
    {
        return mstrDutTestDataFilePath;
    }

    bool CCTSEGuCalWiFiIPQ40xx::LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam)
    {
        CCJGW_ConfigIni configIni;
        configIni.InitIniFilePath(strGuCalConfigPath.c_str());

        std::wstring strTemp = configIni.GetIniKeyValue(L"WIFI",L"gu_test_data_file",L"TestLog\\WIFI\\wifi_gu_cal.csv");
        mstrDutTestDataFilePath = JGW_RealativePathToAbsPath(strTemp.c_str());

        strTemp = configIni.GetIniKeyValue(L"WIFI",L"wifi_loss_path",L"equip\\StationCalDataFile_UC600.xml");
        mstrWiFiLossConfigFilePath = JGW_RealativePathToAbsPath(strTemp.c_str());

        mfGuLoss = configIni.GetIniKeyFloatValue(L"WIFI",L"GuLoss",L"7.0");
        std::map<int,std::map<int,float>> mapChainMaskChannelLoss;
        for (size_t i = 0;i < WIFI_PATHNAME_MAX;i ++)
        {
            std::map<int,float> mapTemp;
            //! 2412 - 2437 - 2462 - 4920 - 5180 - 5500 - 5825
            mapTemp.insert(std::make_pair<int,float>(2412,mfGuLoss));
            mapTemp.insert(std::make_pair<int,float>(2437,mfGuLoss));
            mapTemp.insert(std::make_pair<int,float>(2462,mfGuLoss));
            mapTemp.insert(std::make_pair<int,float>(4920,mfGuLoss));
            mapTemp.insert(std::make_pair<int,float>(5180,mfGuLoss));
            mapTemp.insert(std::make_pair<int,float>(5500,mfGuLoss));
            mapTemp.insert(std::make_pair<int,float>(5825,mfGuLoss));
            mapChainMaskChannelLoss.insert(std::make_pair<int,std::map<int,float>>(i,mapTemp));
        }

        return GenGuLossConfigFile(mstrDefaultGuLossConfigFilePath,mapChainMaskChannelLoss);
    }

    std::wstring CCTSEGuCalWiFiIPQ40xx::SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath)
    {
        //! key: ChainMask,Value: Channel Power List
        std::map<int,S_GU_CHANNLE_POWER_LIST> mapChainMaskChannelPowerList;
        //! 
        if (!LoadWiFiGuTestData(strDutTestDataFilePath,mapChainMaskChannelPowerList))
        {
            return L"读取校准测试数据失败，请点击Start Cal按钮开始测试金机";
        }

        std::map<int,std::map<int,float>> mapChainMaskChannelLoss;
        if (!MapChainMaskChannelPowerListConvertWiFiLossData(mapChainMaskChannelPowerList,mapChainMaskChannelLoss))
        {
            return L"计算金机校准线损失败";
        }
        return GenGuLossConfigFile(mstrWiFiLossConfigFilePath,mapChainMaskChannelLoss) ? L"" : L"生成金机线损配置文件失败";
    }

    bool CCTSEGuCalWiFiIPQ40xx::GenGuLossConfigFile(const std::wstring& strLossConfigFilePath,std::map<int, std::map<int, float>> mapChainMaskChannelLoss)
    {
#if 0
        if (JGW_PathFileExists(mstrDefaultGuLossConfigFilePath.c_str()))
        {
            JGW_DeleteFile(mstrDefaultGuLossConfigFilePath);
        }
        std::vector<int> vChannel;
        vChannel.push_back(2412);
        vChannel.push_back(2437);
        vChannel.push_back(2462);
        vChannel.push_back(4920);
        vChannel.push_back(5180);
        vChannel.push_back(5500);
        vChannel.push_back(5825);
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlDeclaration* pDeclaration= new TiXmlDeclaration("1.0","us-ascii",""); 
        ptrXmlDoc->LinkEndChild(pDeclaration);  

        TiXmlElement* pRoot = new TiXmlElement("PathList");
        pRoot->SetAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
        pRoot->SetAttribute("xmlns:xsd","http://www.w3.org/2001/XMLSchema");
        ptrXmlDoc->LinkEndChild(pRoot);
        for (int i = 0;i < 4 ;i ++)
        {
            TiXmlElement* pPathNode = new TiXmlElement("Path");
            SetElementTextValue(pPathNode,"PathName",JGW_GetFormatWString(L"BH%d_LP",i));
            SetElementTextValue(pPathNode,"PathID",L"0");
            SetElementTextValue(pPathNode,"AdditionalLoss",L"0");
            SetElementTextValue(pPathNode,"Cal_Date",L"27/4/2021 11:51 AM");
            SetElementTextValue(pPathNode,"TesterID",L"CNALLURI07291");
            SetElementTextValue(pPathNode,"CalCable_PathName",L"NA");
            SetElementTextValue(pPathNode,"UseCalCable",L"false");
            TiXmlElement* pDataListNode = new TiXmlElement("DataList");
            for (size_t index = 0;index < vChannel.size();index ++)
            {
                TiXmlElement* pDataNode = new TiXmlElement("Data");
                SetElementTextValue(pPathNode,"Frequency",JGW_GetFormatWString(L"%d",vChannel[index]));
                SetElementTextValue(pPathNode,"Value",JGW_GetFormatWString(L"%.2f",fLoss));
                SetElementTextValue(pPathNode,"Delta",L"0");
                pDataListNode->LinkEndChild(pDataListNode);
            }
            pPathNode->LinkEndChild(pDataListNode);
            pRoot->LinkEndChild(pPathNode);
        }

        return ptrXmlDoc->SaveFile();
#else
        if (JGW_PathFileExists(strLossConfigFilePath.c_str()))
        {
            JGW_DeleteFile(strLossConfigFilePath);
        }
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlDeclaration* pDeclaration= new TiXmlDeclaration("1.0","us-ascii",""); 
        ptrXmlDoc->LinkEndChild(pDeclaration);  

        TiXmlElement* pRoot = new TiXmlElement("PathList");
        pRoot->SetAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
        pRoot->SetAttribute("xmlns:xsd","http://www.w3.org/2001/XMLSchema");
        ptrXmlDoc->LinkEndChild(pRoot);

        for (std::map<int,std::map<int,float>>::iterator it = mapChainMaskChannelLoss.begin();
            it != mapChainMaskChannelLoss.end();
            ++ it)
        {
            TiXmlElement* pPathNode = new TiXmlElement("Path");
            SetElementTextValue(pPathNode,"PathName",JGW_GetFormatWString(L"BH%d_LP",it->first));
            SetElementTextValue(pPathNode,"PathID",L"0");
            SetElementTextValue(pPathNode,"AdditionalLoss",L"0");
            SetElementTextValue(pPathNode,"Cal_Date",L"27/4/2021 11:51 AM");
            SetElementTextValue(pPathNode,"TesterID",L"CNALLURI07291");
            SetElementTextValue(pPathNode,"CalCable_PathName",L"NA");
            SetElementTextValue(pPathNode,"UseCalCable",L"false");
            TiXmlElement* pDataListNode = new TiXmlElement("DataList");

            for (std::map<int,float>::iterator itChannelLoss = it->second.begin();
                itChannelLoss != it->second.end();
                ++ itChannelLoss)
            {
                TiXmlElement* pDataNode = new TiXmlElement("Data");
                SetElementTextValue(pDataNode,"Frequency",JGW_GetFormatWString(L"%d",itChannelLoss->first));
                SetElementTextValue(pDataNode,"Value",JGW_GetFormatWString(L"%.2f",itChannelLoss->second));
                SetElementTextValue(pDataNode,"Delta",L"0");
                pDataListNode->LinkEndChild(pDataNode);
            }
            /*for (size_t index = 0;index < vChannel.size();index ++)
            {
            TiXmlElement* pDataNode = new TiXmlElement("Data");
            SetElementTextValue(pPathNode,"Frequency",JGW_GetFormatWString(L"%d",vChannel[index]));
            SetElementTextValue(pPathNode,"Value",JGW_GetFormatWString(L"%.2f",fLoss));
            SetElementTextValue(pPathNode,"Delta",L"0");
            pDataListNode->LinkEndChild(pDataListNode);
            }*/
            pPathNode->LinkEndChild(pDataListNode);
            pRoot->LinkEndChild(pPathNode);
        }
        /*for (int i = 0;i < 4 ;i ++)
        {
            TiXmlElement* pPathNode = new TiXmlElement("Path");
            SetElementTextValue(pPathNode,"PathName",JGW_GetFormatWString(L"BH%d_LP",i));
            SetElementTextValue(pPathNode,"PathID",L"0");
            SetElementTextValue(pPathNode,"AdditionalLoss",L"0");
            SetElementTextValue(pPathNode,"Cal_Date",L"27/4/2021 11:51 AM");
            SetElementTextValue(pPathNode,"TesterID",L"CNALLURI07291");
            SetElementTextValue(pPathNode,"CalCable_PathName",L"NA");
            SetElementTextValue(pPathNode,"UseCalCable",L"false");
            TiXmlElement* pDataListNode = new TiXmlElement("DataList");
            for (size_t index = 0;index < vChannel.size();index ++)
            {
                TiXmlElement* pDataNode = new TiXmlElement("Data");
                SetElementTextValue(pPathNode,"Frequency",JGW_GetFormatWString(L"%d",vChannel[index]));
                SetElementTextValue(pPathNode,"Value",JGW_GetFormatWString(L"%.2f",fLoss));
                SetElementTextValue(pPathNode,"Delta",L"0");
                pDataListNode->LinkEndChild(pDataListNode);
            }
            pPathNode->LinkEndChild(pDataListNode);
            pRoot->LinkEndChild(pPathNode);
        }*/
        return ptrXmlDoc->SaveFile(JGW_W2A(strLossConfigFilePath));
#endif
    }

    bool CCTSEGuCalWiFiIPQ40xx::LoadWiFiGuTestData(const std::wstring& strDutTestDataFilePath,std::map<int, S_GU_CHANNLE_POWER_LIST>& mapChainMaskChannelPowerList)
    {
        if (!JGW_PathFileExists(strDutTestDataFilePath.c_str()))
        {
            PELOG4WW_F(L"Gu Test Data File Path(%s) Not Exists\n",strDutTestDataFilePath.c_str());
            return false;
        }
        std::wifstream guTestDatafstream;
        guTestDatafstream.open(strDutTestDataFilePath.c_str());
        if (!guTestDatafstream.is_open())
        {
            PELOG4WW_F(L"Open Gu Test Data File Path(%s) Fail\n",strDutTestDataFilePath.c_str());
            return false;
        }

        wchar_t szReadBuf[4096] = {0};
        guTestDatafstream.getline(szReadBuf,4096);
        std::vector<std::wstring> vHeader,vColTemp,vChainMaskChannelPower;
        JGW_ParserStrW(szReadBuf,L",",vHeader);
        if (vHeader.size() <= 2)
        {
            return false;
        }

        while (guTestDatafstream.getline(szReadBuf,4096))
        {
            vColTemp.clear();
            JGW_ParserStrW(szReadBuf,L",",vColTemp);
            if (vColTemp.size() <= 2 && vHeader.size() != vColTemp.size())
            {
                continue;
            }
            for (size_t i = 2;i < vColTemp.size();i ++)
            {
                vChainMaskChannelPower.clear();
                JGW_ParserStrW(vColTemp[i].c_str(),L"-",vChainMaskChannelPower);
                if (vChainMaskChannelPower.size() != 3)
                {
                    continue;
                }
                int nChainMask = _ttoi(vChainMaskChannelPower[0].c_str());
                int nChannel = _ttoi(vChainMaskChannelPower[1].c_str());
                float fPower = static_cast<float>(_ttof(vChainMaskChannelPower[2].c_str()));
                mapChainMaskChannelPowerList[nChainMask].mmapChannelPowerCollection[nChannel].push_back(fPower);
            }
        }

        return true;
    }

    bool CCTSEGuCalWiFiIPQ40xx::MapChainMaskChannelPowerListConvertWiFiLossData(std::map<int, S_GU_CHANNLE_POWER_LIST>& mapChainMaskChannelPowerList,std::map<int, std::map<int, float>>& mapChainMaskChannelLoss)
    {
        if (mapChainMaskChannelPowerList.empty())
        {
            return false;
        }


        for (std::map<int, S_GU_CHANNLE_POWER_LIST>::iterator it = mapChainMaskChannelPowerList.begin();
            it != mapChainMaskChannelPowerList.end();
            ++ it)
        {
            for (std::map<int,std::vector<float>>::iterator itChannelPowers = it->second.mmapChannelPowerCollection.begin();
                itChannelPowers != it->second.mmapChannelPowerCollection.end();
                ++ itChannelPowers)
            {
                float fLoss = 0.00;
                if (itChannelPowers->second.empty() || !GetLossToPowersGuLoss(itChannelPowers->first,itChannelPowers->second,mfGuLoss,fLoss))
                {
                    return false;
                }
                mapChainMaskChannelLoss[it->first][itChannelPowers->first] = fLoss;
            }
        }

        return !mapChainMaskChannelLoss.empty();
    }

    bool CCTSEGuCalWiFiIPQ40xx::GetLossToPowersGuLoss(int nChannel,std::vector<float>& vfPower,float fGuLoss,float& fLoss)
    {
        float sum = 0,mean = 0;
        sum = std::accumulate(std::begin(vfPower), std::end(vfPower), 0.00f);
        mean =  sum / vfPower.size(); //均值

        if (2412 <= nChannel && nChannel <= 2472)
        {
            fLoss = (mn24GExpectPower - mean) + fGuLoss;
        }
        else
        {
            fLoss = (mn5GExpectPower - mean) + fGuLoss;
        }
        return true;
    }
}

