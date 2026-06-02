#include "StdAfx.h"
#include "CJGW_WlanConfigFile.h"
#include <tinyxml/tinyxml.h>

namespace JGW
{
    CCJGW_WlanConfigFile::CCJGW_WlanConfigFile(void)
    {
    }


    CCJGW_WlanConfigFile::~CCJGW_WlanConfigFile(void)
    {
    }

    void CCJGW_WlanConfigFile::SetWlanConfigFilePath(const std::wstring& strWlanConfigFilePath)
    {
        mstrWlanConfigFilePath = strWlanConfigFilePath;
    }

    bool CCJGW_WlanConfigFile::GetTxParamToPatnName(const std::wstring& strPathName,S_TxTestParam& sTxTestParam)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        if (!ptrXmlDoc->LoadFile(JGW_W2A(mstrWlanConfigFilePath).c_str()))
        {
            LogE_F(L"Load Wlan Config File(%s) Fail",mstrWlanConfigFilePath.c_str());
            return false;
        }

        TiXmlElement* xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            LogE_F(L"Load Wlan Config XML File(%s) Fail",mstrWlanConfigFilePath.c_str());
            return false;
        }
        TiXmlElement* testParamNode =  xmlRoot->FirstChildElement("TestParam");
        if (NULL == testParamNode)
        {
            LogE(L"Load TestParam  Node Fail");
            return false;
        }

        TiXmlElement* txNode = testParamNode->FirstChildElement("Tx");
        std::wstring strPathNameTemp,strTemp;
        while (txNode)
        {
            GetElementAttributeValue(txNode,"PathName",strPathNameTemp);
            if (0 == strPathNameTemp.compare(strPathName))
            {
                GetElementTextValue(txNode,"PwrMode",strTemp,L"3");
                sTxTestParam.mePwrMode = (TxPowerMode)_ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"nss",strTemp,L"1");
                sTxTestParam.meNss = (WLAN_UniversalNSS)_ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"TxMode",strTemp,L"3");
                sTxTestParam.meTxMode = (WLAN_TX_Mode)_ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"Gain",strTemp,L"23");
                sTxTestParam.muGain = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"DigitalGain",strTemp,L"0");
                sTxTestParam.mnDigitalGain = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"PAcfg",strTemp,L"7");
                sTxTestParam.muPASetting = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"PayLoadSize",strTemp,L"65536");
                sTxTestParam.muPayLoadSize = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"Short11b_nGuard",strTemp,L"0");
                sTxTestParam.mbShort11b_nGuard = 1 == _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"LdpcRate",strTemp,L"1");
                sTxTestParam.mbLdpcRate = 1 == _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"StbcMode",strTemp,L"0");
                sTxTestParam.mbSTBCmode = 1 == _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"ackRequest",strTemp,L"0");
                sTxTestParam.mbackRequest = 1 == _ttoi(strTemp.c_str());
               
                GetElementTextValue(txNode,"Aggregation",strTemp,L"4");
                sTxTestParam.muAggregation = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"DpdMode",strTemp,L"0");
                sTxTestParam.meDpdMode = (DPDSetting)_ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"ifs",strTemp,L"10");
                sTxTestParam.mnIfs = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"DutyCycle",strTemp,L"10");
                sTxTestParam.mnDutyCycle = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"nPattern",strTemp,L"4");
                sTxTestParam.mePattern = (TxDataPattern)_ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"pattern",strTemp,L"0");
                sTxTestParam.mvPattern.clear();
                std::vector<int> vTemp;
                JGW_ParserIntW(strTemp.c_str(),L",",vTemp);
                for (size_t i = 0;i < vTemp.size();i ++)
                {
                    sTxTestParam.mvPattern.push_back(static_cast<byte>(vTemp[i]));
                }


                GetElementTextValue(txNode,"Packet",strTemp,L"0");
                sTxTestParam.muPacket = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"BroadcastMode",strTemp,L"3");
                sTxTestParam.mbBroadcastMode = 1 == _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"AIFSN",strTemp,L"0");
                sTxTestParam.muiAIFSN = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"DutTxSettlingDelayMs",strTemp,L"500");
                sTxTestParam.muDutTxSettlingDelayMs = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"ScramblerDisable",strTemp,L"0");
                sTxTestParam.mbScramblerDisable = 1 == _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"Channel2",strTemp,L"0");
                sTxTestParam.mnChannel2 = _ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"HC_Enable",strTemp,L"0");
                sTxTestParam.meHC_Enable = (HCSetting)_ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"DpdMode",strTemp,L"0");
                sTxTestParam.meDpdMode = (DPDSetting)_ttoi(strTemp.c_str());

                GetElementTextValue(txNode,"guardInterval",strTemp,L"0");
                sTxTestParam.meGuardInterval = (WLAN_UniversalGuardInterval)_ttoi(strTemp.c_str());

                return true;
            }
            txNode = txNode->NextSiblingElement("Tx");
        }
        LogE_F(L"Get Tx Param(PathName:%s) Fail",strPathName.c_str());
        return false;
    }

    bool CCJGW_WlanConfigFile::GetTestingStandardToPatnName(const std::wstring& strPathName,S_TX_TestingStandard& sTxTestingStandard)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        if (!ptrXmlDoc->LoadFile(JGW_W2A(mstrWlanConfigFilePath).c_str()))
        {
            LogE_F(L"Load Wlan Config File(%s) Fail",mstrWlanConfigFilePath.c_str());
            return false;
        }

        TiXmlElement* xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            LogE_F(L"Load Wlan Config XML File(%s) Fail",mstrWlanConfigFilePath.c_str());
            return false;
        }
        TiXmlElement* testingStandardNode =  xmlRoot->FirstChildElement("TestingStandard");
        if (NULL == testingStandardNode)
        {
            LogE(L"Load Testing Standard  Node Fail");
            return false;
        }

        TiXmlElement* txNode = testingStandardNode->FirstChildElement("Tx");
        std::wstring strPathNameTemp,strLL,strUL;
        while (txNode)
        {
            GetElementAttributeValue(txNode,"PathName",strPathNameTemp);
            if (0 == strPathNameTemp.compare(strPathName))
            {
                TiXmlElement* avgTxPowerDeltaNode = txNode->FirstChildElement("AvgTxPowerDelta");
                sTxTestingStandard.mbTestAvgTxPowerDelta = false;
                if (avgTxPowerDeltaNode)
                {
                    GetElementAttributeValue(avgTxPowerDeltaNode,"LL",strLL,L"-99999");
                    sTxTestingStandard.mvAvgTxPowerDelta[E_MIN_Collars] = _ttof(strLL.c_str());
                    GetElementAttributeValue(avgTxPowerDeltaNode,"UL",strUL,L"-99999");
                    sTxTestingStandard.mvAvgTxPowerDelta[E_MAX_Collars] = _ttof(strUL.c_str());
                    sTxTestingStandard.mbTestAvgTxPowerDelta = true;
                }
                TiXmlElement* pAmplErrNode = txNode->FirstChildElement("AmplErr");
                sTxTestingStandard.mbTestAmplErr = false;
                if (pAmplErrNode)
                {
                    GetElementAttributeValue(pAmplErrNode,"LL",strLL,L"-99999");
                    GetElementAttributeValue(pAmplErrNode,"UL",strUL,L"-99999");
                    sTxTestingStandard.mvAmplErr[E_MIN_Collars] = _ttof(strLL.c_str());
                    sTxTestingStandard.mvAmplErr[E_MAX_Collars] = _ttof(strUL.c_str());
                    sTxTestingStandard.mbTestAmplErr = true;
                }
                TiXmlElement* pDataRateMB = txNode->FirstChildElement("DataRateMB");
                sTxTestingStandard.mbTestDataRateMB = false;
                if (pDataRateMB)
                {
                    GetElementAttributeValue(pDataRateMB,"LL",strLL,L"-99999");
                    GetElementAttributeValue(pDataRateMB,"UL",strUL,L"-99999");
                    sTxTestingStandard.mvDataRateMB[E_MIN_Collars] = _ttof(strLL.c_str());
                    sTxTestingStandard.mvDataRateMB[E_MAX_Collars] = _ttof(strUL.c_str());
                    sTxTestingStandard.mbTestDataRateMB = true;
                }
                TiXmlElement* pSymbolClockError = txNode->FirstChildElement("SymbolClockError");
                sTxTestingStandard.mbTestSymbolClockError = false;
                if (pSymbolClockError)
                {
                    GetElementAttributeValue(pSymbolClockError,"LL",strLL,L"-99999");
                    GetElementAttributeValue(pSymbolClockError,"UL",strUL,L"-99999");
                    sTxTestingStandard.mvSymbolClockError[E_MIN_Collars] = _ttof(strLL.c_str());
                    sTxTestingStandard.mvSymbolClockError[E_MAX_Collars] = _ttof(strUL.c_str());
                    sTxTestingStandard.mbTestSymbolClockError = true;
                }
                TiXmlElement* pFreqErrNode = txNode->FirstChildElement("FreqErr");
                sTxTestingStandard.mbTestFreqErr = false;
                if (pFreqErrNode)
                {
                    GetElementAttributeValue(pFreqErrNode,"LL",strLL,L"-99999");
                    GetElementAttributeValue(pFreqErrNode,"UL",strUL,L"-99999");
                    sTxTestingStandard.mvFreqErr[E_MIN_Collars] = _ttof(strLL.c_str());
                    sTxTestingStandard.mvFreqErr[E_MAX_Collars] = _ttof(strUL.c_str());
                    sTxTestingStandard.mbTestFreqErr = true;
                }
                TiXmlElement* pPhaseErrNode = txNode->FirstChildElement("PhaseErr");
                sTxTestingStandard.mbTestPhaseErr = false;
                if (pPhaseErrNode)
                {
                    GetElementAttributeValue(pPhaseErrNode,"LL",strLL,L"-99999");
                    GetElementAttributeValue(pPhaseErrNode,"UL",strUL,L"-99999");
                    sTxTestingStandard.mvPhaseErr[E_MIN_Collars] = _ttof(strLL.c_str());
                    sTxTestingStandard.mvPhaseErr[E_MAX_Collars] = _ttof(strUL.c_str());
                    sTxTestingStandard.mbTestPhaseErr = true;
                }
                TiXmlElement* pLoLeakageNode = txNode->FirstChildElement("LoLeakage");
                sTxTestingStandard.mbTestLoLeakage = false;
                if (pLoLeakageNode)
                {
                    GetElementAttributeValue(pLoLeakageNode,"LL",strLL,L"-99999");
                    GetElementAttributeValue(pLoLeakageNode,"UL",strUL,L"-99999");
                    sTxTestingStandard.mvLoLeakage[E_MIN_Collars] = _ttof(strLL.c_str());
                    sTxTestingStandard.mvLoLeakage[E_MAX_Collars] = _ttof(strUL.c_str());
                    sTxTestingStandard.mbTestLoLeakage = true;
                }
                return true;
            }
            txNode = txNode->NextSiblingElement("Tx");
        }
        LogE_F(L"Get Tx Testing Standard(PathName:%s) Fail",strPathName.c_str());
        return false;
    }
}
