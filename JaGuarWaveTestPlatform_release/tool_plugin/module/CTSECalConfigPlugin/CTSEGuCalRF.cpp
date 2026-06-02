#include "StdAfx.h"
#include "CTSEGuCalRF.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <tinyxml/tinyxml.h>
#include <fstream>
#include <algorithm>
#include <numeric>

namespace JGW
{
    CCTSEGuCalRF::CCTSEGuCalRF(void) : mnMaxPath(1)
    {
        mstrDefaultGuLossConfigFilePath = JGW_RealativePathToAbsPath(L"equip\\StationCalDataFile_WLAN_GU_Loss.xml");
    }


    CCTSEGuCalRF::~CCTSEGuCalRF(void)
    {
    }

    bool CCTSEGuCalRF::LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam)
    {
        CCJGW_ConfigIni configIni;
        configIni.InitIniFilePath(strGuCalConfigPath.c_str());
        mpsCalConfigTestConfigParam = psCalConfigTestConfigParam;

        std::wstring strTemp = configIni.GetIniKeyValue(L"WLAN_RF",L"gu_test_data_file",L"TestLog\\GU\\GuTestData.txt");
        mstrDutTestDataFilePath = JGW_RealativePathToAbsPath(strTemp.c_str());
		JGW_DeleteFile(mstrDutTestDataFilePath);

        strTemp = configIni.GetIniKeyValue(L"WLAN_RF",L"wifi_loss_path",L"equip\\StationCalDataFile_P760.xml");
        mstrWiFiLossConfigFilePath = JGW_RealativePathToAbsPath(strTemp.c_str());
        //mstrDefaultGuLossConfigFilePath = mstrWiFiLossConfigFilePath;

        mstrGuDefaultFreq = configIni.GetIniKeyValue(L"WLAN_RF",L"GuDefaultFreq",L"2000,2010,2019.9,1627,1643.5,1660.9,1525,1542,1558.9,2170,2185,2199.5");

        mnMaxPath = configIni.GetIniKeyIntValue(L"WLAN_RF",L"MaxPath",1);
        return true;
    }

    bool CCTSEGuCalRF::OnStartDutTestThread()
    {
        std::vector<std::wstring> vstrGuDefaultFreq;
        JGW_ParserStrW(mstrGuDefaultFreq.c_str(),L",",vstrGuDefaultFreq);

        std::map<int,std::map<std::wstring,float>> mapChainMaskChannelLoss;
        for (size_t i = 0;i < mnMaxPath;i ++)
        {
            std::map<std::wstring,float> mapTemp;
            //! 2412 - 2437 - 2462 - 4920 - 5180 - 5500 - 5825
            for (std::vector<std::wstring>::iterator it = vstrGuDefaultFreq.begin();
                it != vstrGuDefaultFreq.end();
                ++ it)
            {
                mapTemp.insert(std::make_pair<std::wstring,float>(it[0],mpsCalConfigTestConfigParam->mfGuStartLoss));
            }
            //mapTemp.insert(std::make_pair<int,float>(2412,mfGuLoss));
            //mapTemp.insert(std::make_pair<int,float>(2437,mfGuLoss));
            //mapTemp.insert(std::make_pair<int,float>(2462,mfGuLoss));
            //mapTemp.insert(std::make_pair<int,float>(4920,mfGuLoss));
            //mapTemp.insert(std::make_pair<int,float>(5180,mfGuLoss));
            //mapTemp.insert(std::make_pair<int,float>(5500,mfGuLoss));
            //mapTemp.insert(std::make_pair<int,float>(5825,mfGuLoss));
            mapChainMaskChannelLoss.insert(std::make_pair<int,std::map<std::wstring,float>>(i,mapTemp));
        }

        return GenDutLossConfigFile(mstrDefaultGuLossConfigFilePath,mapChainMaskChannelLoss);
    }

    bool CCTSEGuCalRF::GenDutLossConfigFile(const std::wstring& strLossConfigFilePath,std::map<int, std::map<std::wstring, float>> mapChainMaskChannelLoss)
    {
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

        for (std::map<int,std::map<std::wstring,float>>::iterator it = mapChainMaskChannelLoss.begin();
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

            for (std::map<std::wstring,float>::iterator itChannelLoss = it->second.begin();
                itChannelLoss != it->second.end();
                ++ itChannelLoss)
            {
                TiXmlElement* pDataNode = new TiXmlElement("Data");
                SetElementTextValue(pDataNode,"Frequency",itChannelLoss->first);
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
    }

    std::wstring CCTSEGuCalRF::GetDutTestDataFilePath()
    {
        return mstrDutTestDataFilePath;
    }

    std::wstring CCTSEGuCalRF::SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath)
    {
        if (mpsCalConfigTestConfigParam->mvsGuData.empty())
        {
            return L"请导入金机数据文件";
        }
        //! TestLog\GU\GuTestData.txt
        _mapFreqPathIDPower mmapTestData;
        if (!LoadDutTestDataFile(strDutTestDataFilePath,mmapTestData))
        {
            return L"读取校准测试数据失败，请点击Start Cal按钮开始测试金机";
        }

        std::map<int,std::map<std::wstring,float>> mapChainMaskChannelLoss;
        if (!MapChainMaskChannelPowerListConvertWiFiLossData(mmapTestData,mapChainMaskChannelLoss))
        {
            return L"计算金机校准线损失败";
        }
        if (GenDutLossConfigFile(mstrWiFiLossConfigFilePath,mapChainMaskChannelLoss))
        {
            return L"";
        }
        return L"生成金机线损配置文件失败";
    }

    bool CCTSEGuCalRF::LoadDutTestDataFile(const std::wstring& strDutTestDataFilePath,_mapFreqPathIDPower& mmapTestData)
    {
        std::wifstream guTestDatafstream;
        guTestDatafstream.open(strDutTestDataFilePath.c_str());
        if (!guTestDatafstream.is_open())
        {
            PELOG4WW_F(L"Open Gu Test Data File Path(%s) Fail\n",strDutTestDataFilePath.c_str());
            return false;
        }

        wchar_t szReadBuf[4096] = {0};
        while (guTestDatafstream.getline(szReadBuf,4096))
        {
            std::vector<std::wstring> vColTemp;
            JGW_ParserStrW(szReadBuf,L",",vColTemp);
            for (std::vector<std::wstring>::iterator it = vColTemp.begin();
                it != vColTemp.end();
                ++ it)
            {
                std::vector<std::wstring> vPathFreqPower;
                JGW_ParserStrW(it->c_str(),L"-",vPathFreqPower);
                if (vPathFreqPower.size() == 3)
                {
                    int nPathID = _ttoi(vPathFreqPower[0].c_str());
                    float fPower = static_cast<float>(_ttof(vPathFreqPower[2].c_str()));
                    _mapFreqPathIDPower::iterator itFreqPathPower = mmapTestData.find(vPathFreqPower[1]);
                    if (itFreqPathPower == mmapTestData.end())
                    {
                        _mapPathIDPowerVec vsPathPower;
                        std::vector<float> vfPower;
                        vfPower.push_back(fPower);
                        vsPathPower.insert(std::make_pair<int,std::vector<float>>(nPathID,vfPower));
                        mmapTestData.insert(std::make_pair<std::wstring,std::map<int,std::vector<float>>>(vPathFreqPower[1],vsPathPower));
                    }
                    else
                    {
                        _mapPathIDPowerVec::iterator itPathIDPowerVec = itFreqPathPower->second.find(nPathID);
                        if (itPathIDPowerVec == itFreqPathPower->second.end())
                        {
                            std::vector<float> vfPower;
                            vfPower.push_back(fPower);
                            itFreqPathPower->second.insert(std::make_pair<int,std::vector<float>>(nPathID,vfPower));
                        }
                        else
                        {
                            itPathIDPowerVec->second.push_back(fPower);
                        }
                        //! itFreqPathPower->second.insert(std::make_pair<int,float>(nPathID,fPower));
                    }

#if 0
                    S_PATH_POWER sPathPower = {_ttoi(vPathFreqPower[0].c_str()),static_cast<float>(_ttof(vPathFreqPower[2].c_str()))};
                    std::map<std::wstring, std::vector<S_PATH_POWER>>::iterator itFreqPathPower = mmapTestData.find(vPathFreqPower[1]);
                    if (itFreqPathPower == mmapTestData.end())
                    {
                        std::vector<S_PATH_POWER> vsPathPower;
                        vsPathPower.push_back(sPathPower);
                        mmapTestData.insert(std::make_pair<std::wstring,std::vector<S_PATH_POWER>>(vPathFreqPower[1],vsPathPower));
                    }
                    else
                    {
                        itFreqPathPower->second.push_back(sPathPower);
                    }
#endif
                }
            }
        }
        return true;
    }

    bool CCTSEGuCalRF::MapChainMaskChannelPowerListConvertWiFiLossData(_mapFreqPathIDPower& mmapTestData,std::map<int, std::map<std::wstring, float>>& mapChainMaskChannelLoss)
    {
        if (mmapTestData.empty())
        {
            return false;
        }

        for (_mapFreqPathIDPower::iterator it = mmapTestData.begin();
            it != mmapTestData.end();
            ++ it)
        {
            for (_mapPathIDPowerVec::iterator itChannelPowers = it->second.begin();
                itChannelPowers != it->second.end();
                ++ itChannelPowers)
            {
                float fGuPower = GetPowerToGuData(itChannelPowers->first,it->first);
                if (fGuPower == INVAID_POWER)
                {
                    PELOG4WW(L"金机数据未包含当前频率");
                    continue;
                }
                float fLoss = 1.00;
                if (itChannelPowers->second.empty() || !GetLossToPowersGuLoss(itChannelPowers->second,fGuPower,mpsCalConfigTestConfigParam->mfGuStartLoss,fLoss))
                {
                    return false;
                }
                mapChainMaskChannelLoss[itChannelPowers->first][it->first] = fLoss;
            }
        }
#if 0
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
#endif
        return !mapChainMaskChannelLoss.empty();
    }

    float CCTSEGuCalRF::GetPowerToGuData(int pathID,const std::wstring& strFreq)
    {
        for (std::vector<S_GU_DATA>::iterator it = mpsCalConfigTestConfigParam->mvsGuData.begin();
            it != mpsCalConfigTestConfigParam->mvsGuData.end();
             ++ it)
        {
            if (0 == JGW_WStrComparenoCaseWStr(strFreq.c_str(),it->mstrFreq.c_str()))
            {
                return it->mstrPower;
            }
        }
        return INVAID_POWER;
    }

    bool CCTSEGuCalRF::GetLossToPowersGuLoss(std::vector<float>& vfPower,float fGuPower,float fGuLoss,float& fLoss)
    {
        float sum = 0,mean = 0;
        sum = std::accumulate(std::begin(vfPower), std::end(vfPower), 0.00f);
        mean =  sum / vfPower.size(); //均值
        fLoss = (fGuPower - mean) + fGuLoss;
        return true;
    }
}

