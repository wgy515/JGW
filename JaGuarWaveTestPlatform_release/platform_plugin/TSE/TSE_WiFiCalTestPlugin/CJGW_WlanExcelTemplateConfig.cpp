#include "StdAfx.h"
#include "CJGW_WlanExcelTemplateConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_WlanExcelTemplateConfig::CCJGW_WlanExcelTemplateConfig(void)
    {
    }


    CCJGW_WlanExcelTemplateConfig::~CCJGW_WlanExcelTemplateConfig(void)
    {
        if (mcExcelLibXL.CheckExcelIsOpen())
        {
            mcExcelLibXL.CloseExcel();
        }
    }

    std::wstring CCJGW_WlanExcelTemplateConfig::GetErrorMessage()
    {
        return mstrErrorMessage;
    }

    bool CCJGW_WlanExcelTemplateConfig::LoadWlanExcelTemplateConfig(const std::wstring& strPath,const std::wstring& strExcelPath,std::vector<S_EXCEL_WIFI_POS>& vsExcelWifiInfo)
    {
        CCJGW_ConfigIni configIni;
        configIni.InitIniFilePath(strPath.c_str());

        std::vector<std::wstring> vSheetNameList;
        mstrErrorMessage = L"";
        std::wstring strTemp = configIni.GetIniKeyValue(L"MAIN",L"SheetNameList");
        JGW_ParserStrW(strTemp.c_str(),L",",vSheetNameList);
        vsExcelWifiInfo.clear();

        if (!mcExcelLibXL.OpenExcel(strExcelPath))
        {
            return false;
        }

        for (size_t i = 0;i < vSheetNameList.size();i++)
        {
            if (!mcExcelLibXL.SetExcelSheet(vSheetNameList[i]))
            {
                return false;
            }
            if (!LoadWlanProtocolInfoConfig(configIni,vSheetNameList[i],vsExcelWifiInfo))
            {
                return false;
            }
        }
        return true;
    }

    bool CCJGW_WlanExcelTemplateConfig::GetExcelSheetChannel(std::wstring& strStartChannelRowCol,std::vector<uint>& vChannel)
    {
        std::vector<int> vRowCol;
        JGW_ParserIntW(strStartChannelRowCol.c_str(),L",",vRowCol);
        if (vRowCol.size() >= 2)
        {
            std::wstring strTemp = mcExcelLibXL.ReadExcelContent(vRowCol[E_EXCEL_ROW],vRowCol[E_EXCEL_COL]);
            uint channel = _ttoi(strTemp.c_str());
            int index = 1;
            while (channel > 0)
            {
                vChannel.push_back(channel);
                strTemp = mcExcelLibXL.ReadExcelContent(vRowCol[E_EXCEL_ROW],vRowCol[E_EXCEL_COL] + index);
                channel = _ttoi(strTemp.c_str());
                index++;
            }
            return true;
        }
        return false;
    }

    bool CCJGW_WlanExcelTemplateConfig::GetExcelSheetRate(const std::wstring& strRateRow,const std::wstring& strRateCol,std::vector<WLAN_UniversalDataRate>& vRate)
    {
        std::wstring strTemp;
        std::vector<int> vRateRow,vRateCol;
        JGW_ParserIntW(strRateRow.c_str(),L",",vRateRow);
        JGW_ParserIntW(strRateCol.c_str(),L",",vRateCol);
        vRate.clear();
        if (vRateRow.size() != vRateCol.size())
        {
            return false;
        }

        for (size_t index = 0;index < vRateRow.size();index++)
        {
            strTemp = mcExcelLibXL.ReadExcelContent(vRateRow[index],vRateCol[index]);
            vRate.push_back(GetWlanUniversalDataRateToContent(strTemp));
        }

        return true;
    }

    void CCJGW_WlanExcelTemplateConfig::LoadWlanSysConfig(CCJGW_ConfigIni& configIni,const std::wstring& strAppName,std::map<std::wstring, std::wstring>& mmapWlanSysConfig)
    {
        std::wstring strImp = configIni.GetIniKeyValue(strAppName.c_str(),L"imp");
        if (!strImp.empty())
        {
            mmapWlanSysConfig[L"chainMask"] = configIni.GetIniKeyValue(strImp.c_str(),L"chainMask");
            mmapWlanSysConfig[L"rate_row"] = configIni.GetIniKeyValue(strImp.c_str(),L"rate_row");
            mmapWlanSysConfig[L"rate_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"rate_col");
            mmapWlanSysConfig[L"rate_bw"] = configIni.GetIniKeyValue(strImp.c_str(),L"rate_bw");
            mmapWlanSysConfig[L"channel_start_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"channel_start_row_col");
            mmapWlanSysConfig[L"powerLevel"] = configIni.GetIniKeyValue(strImp.c_str(),L"powerLevel");
            mmapWlanSysConfig[L"tx_power_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_power_row_col");
            mmapWlanSysConfig[L"tx_sem_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_sem_row_col");
            mmapWlanSysConfig[L"tx_freq_error_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_freq_error_row_col");
            mmapWlanSysConfig[L"tx_evm_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_evm_row_col");
            mmapWlanSysConfig[L"rx_per_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"rx_per_row_col");
            mmapWlanSysConfig[L"tx_hight_temp_power_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_hight_temp_power_row_col");
            mmapWlanSysConfig[L"tx_hight_temp_freq_error_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_hight_temp_freq_error_row_col");
            mmapWlanSysConfig[L"tx_low_temp_power_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_low_temp_power_row_col");
            mmapWlanSysConfig[L"tx_low_temp_freq_error_row_col"] = configIni.GetIniKeyValue(strImp.c_str(),L"tx_low_temp_freq_error_row_col");
            mmapWlanSysConfig[L"chain_interval"] = configIni.GetIniKeyValue(strImp.c_str(),L"chain_interval");
            mmapWlanSysConfig[L"RxParamName"] = configIni.GetIniKeyValue(strImp.c_str(),L"RxParamName");
            mmapWlanSysConfig[L"TxParamName"] = configIni.GetIniKeyValue(strImp.c_str(),L"TxParamName");
        }
        strImp = strAppName;
        std::wstring strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"chainMask");
        if (!strTemp.empty()) mmapWlanSysConfig[L"chainMask"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"rate_row");
        if (!strTemp.empty()) mmapWlanSysConfig[L"rate_row"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"rate_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"rate_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"rate_bw");
        if (!strTemp.empty()) mmapWlanSysConfig[L"rate_bw"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"channel_start_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"channel_start_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"powerLevel");
        if (!strTemp.empty()) mmapWlanSysConfig[L"powerLevel"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_power_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_power_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_sem_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_sem_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_freq_error_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_freq_error_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_evm_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_evm_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"rx_per_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"rx_per_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_hight_temp_power_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_hight_temp_power_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_hight_temp_freq_error_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_hight_temp_freq_error_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_low_temp_power_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_low_temp_power_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"tx_low_temp_freq_error_row_col");
        if (!strTemp.empty()) mmapWlanSysConfig[L"tx_low_temp_freq_error_row_col"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"chain_interval");
        if (!strTemp.empty()) mmapWlanSysConfig[L"chain_interval"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"TxParamName");
        if (!strTemp.empty()) mmapWlanSysConfig[L"TxParamName"] = strTemp;

        strTemp = configIni.GetIniKeyValue(strImp.c_str(),L"RxParamName");
        if (!strTemp.empty()) mmapWlanSysConfig[L"RxParamName"] = strTemp;
    }

    void CCJGW_WlanExcelTemplateConfig::GetArrayRowColToVector(const std::wstring& strValue,uint* uRowCol)
    {
        std::vector<int> vTemp;
        JGW_ParserIntW(strValue.c_str(),L",",vTemp);
        if (vTemp.size() >= 2)
        {
            uRowCol[E_EXCEL_ROW] = vTemp[E_EXCEL_ROW];
            uRowCol[E_EXCEL_COL] = vTemp[E_EXCEL_COL];
        }
        else
        {
            uRowCol[E_EXCEL_ROW] = 0;
            uRowCol[E_EXCEL_COL] = 0;
        }
    }

    void CCJGW_WlanExcelTemplateConfig::GetStartChannelExcelInfo(std::map<std::wstring, std::wstring>& mmapWlanSysConfig,S_EXCEL_WIFI_POS& sChannelExcelInfo)
    {
        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_power_row_col"],sChannelExcelInfo.muTxPowerPos);
        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_sem_row_col"],sChannelExcelInfo.muTxSemPos);
        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_freq_error_row_col"],sChannelExcelInfo.muTxFreqErrorPos);
        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_evm_row_col"],sChannelExcelInfo.muTxEvmPos);
        GetArrayRowColToVector(mmapWlanSysConfig[L"rx_per_row_col"],sChannelExcelInfo.muRxPerPos);

        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_hight_temp_power_row_col"],sChannelExcelInfo.muTxHighTmpPowerPos);
        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_hight_temp_freq_error_row_col"],sChannelExcelInfo.muTxHighTmpFreqErrorPos);

        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_low_temp_power_row_col"],sChannelExcelInfo.muTxLowTmpPowerPos);
        GetArrayRowColToVector(mmapWlanSysConfig[L"tx_low_temp_freq_error_row_col"],sChannelExcelInfo.muTxLowTmpFreqErrorPos);
    }

    void CCJGW_WlanExcelTemplateConfig::GetRxPowerPerRange(const std::wstring& strConfig,int& nRxStartPower,uint& uRxPerRange)
    {
        std::wstring strTemp (strConfig);
        std::vector<std::wstring> vTemp;
        JGW_ReplaceStringW(strTemp,L"<",L"");
        JGW_ReplaceStringW(strTemp,L">",L"");
        JGW_ReplaceStringW(strTemp,L"FER",L"");
        JGW_ReplaceStringW(strTemp,L" ",L"");
        JGW_ReplaceStringW(strTemp,L"dBm",L"");
        JGW_ParserStrW(strTemp.c_str(),L",",vTemp);
        if (vTemp.size() >= 2)
        {
            double temp = _ttof(vTemp[0].c_str());
            uRxPerRange = static_cast<uint>(temp > 1 ? temp : temp * 100);
            nRxStartPower = _ttoi(vTemp[1].c_str());
        }

    }

    bool CCJGW_WlanExcelTemplateConfig::LoadWlanProtocolInfoConfig(CCJGW_ConfigIni& configIni,const std::wstring& strAppName,std::vector<S_EXCEL_WIFI_POS>& sWlanProtocolInfo)
    {
#if 1
        std::map<std::wstring, std::wstring> mapWlanSysConfig;
        LoadWlanSysConfig(configIni,strAppName,mapWlanSysConfig);

        std::vector<int> vChainMasks;
        JGW_ParserIntW(mapWlanSysConfig[L"chainMask"].c_str(),L",",vChainMasks);

        WLAN_UniversalRateBW eRateBw = (WLAN_UniversalRateBW)_ttoi(mapWlanSysConfig[L"rate_bw"].c_str());
        size_t uPowerLevel = _ttoi(mapWlanSysConfig[L"powerLevel"].c_str());
        size_t uChainIntervalPos = _ttoi(mapWlanSysConfig[L"chain_interval"].c_str());

        std::vector<uint> vChannel;
        GetExcelSheetChannel(mapWlanSysConfig[L"channel_start_row_col"],vChannel);

        std::vector<WLAN_UniversalDataRate> veRate;
        GetExcelSheetRate(mapWlanSysConfig[L"rate_row"],mapWlanSysConfig[L"rate_col"],veRate);
        if (0 == vChannel.size())
        {
            mstrErrorMessage = L"Channel configuration error";
            return false;
        }
        if (0 == veRate.size())
        {
            mstrErrorMessage = L"Rate configuration error";
            return false;
        }

        S_EXCEL_WIFI_POS sStartChannelExcelInfo;
        S_EXCEL_WIFI_POS sChannelExcelInfo;
        uint startChannelPos = 0;
        GetStartChannelExcelInfo(mapWlanSysConfig,sStartChannelExcelInfo);

        for (size_t chainIndex = 0;chainIndex < vChainMasks.size();chainIndex++)
        {
            sChannelExcelInfo.meChainMask = (WLAN_CHAIN_MASK2)vChainMasks[chainIndex];
            sChannelExcelInfo.meRateBw = eRateBw;
            // 每个链路所占行数
            int chainRowCount = (sStartChannelExcelInfo.muTxLowTmpFreqErrorPos[E_EXCEL_ROW] + veRate.size() - sStartChannelExcelInfo.muTxPowerPos[E_EXCEL_ROW]) / veRate.size();
            startChannelPos = (chainRowCount * veRate.size() + uChainIntervalPos) * (chainIndex);

            for (size_t rateIndex = 0;rateIndex < veRate.size();rateIndex++)
            {
                sChannelExcelInfo.meDataRate = veRate[rateIndex];
                //! tx_power_row_col
                sChannelExcelInfo.muTxPowerPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxPowerPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muTxSemPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxSemPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muTxFreqErrorPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxFreqErrorPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muTxEvmPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxEvmPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muRxPerPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muRxPerPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muTxHighTmpPowerPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxHighTmpPowerPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muTxHighTmpFreqErrorPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxHighTmpFreqErrorPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muTxLowTmpPowerPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxLowTmpPowerPos[E_EXCEL_ROW] + rateIndex;
                sChannelExcelInfo.muTxLowTmpFreqErrorPos[E_EXCEL_ROW] = startChannelPos + sStartChannelExcelInfo.muTxLowTmpFreqErrorPos[E_EXCEL_ROW] + rateIndex;
                //! INSTRUMENT POWER and mnRxPerRange
                //GetRxPowerPerRange(mcExcelLibXL.ReadExcelContent(sChannelExcelInfo.muRxPerPos[E_EXCEL_ROW],sStartChannelExcelInfo.muRxPerPos[E_EXCEL_COL] - 1),sChannelExcelInfo.muRxStartPower,sChannelExcelInfo.mnRxPerRange);
                for (size_t channelIndex = 0;channelIndex < vChannel.size();channelIndex++)
                {
                    sChannelExcelInfo.muChannel = vChannel[channelIndex];
                    sChannelExcelInfo.mePhyID = sChannelExcelInfo.muChannel >= 4900 ? PHY_A0 : PHY_B;
                    //! tx_power_row_col
                    sChannelExcelInfo.muTxSemPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxSemPos[E_EXCEL_COL];
                    sChannelExcelInfo.muTxPowerPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxPowerPos[E_EXCEL_COL];
                    sChannelExcelInfo.muTxFreqErrorPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxFreqErrorPos[E_EXCEL_COL];
                    sChannelExcelInfo.muTxEvmPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxEvmPos[E_EXCEL_COL];
                    sChannelExcelInfo.muRxPerPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muRxPerPos[E_EXCEL_COL];
                    sChannelExcelInfo.muTxHighTmpPowerPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxHighTmpPowerPos[E_EXCEL_COL];
                    sChannelExcelInfo.muTxHighTmpFreqErrorPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxHighTmpFreqErrorPos[E_EXCEL_COL];
                    sChannelExcelInfo.muTxLowTmpPowerPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxLowTmpPowerPos[E_EXCEL_COL];
                    sChannelExcelInfo.muTxLowTmpFreqErrorPos[E_EXCEL_COL] = channelIndex + sStartChannelExcelInfo.muTxLowTmpFreqErrorPos[E_EXCEL_COL];
                    sChannelExcelInfo.mstrSheetName = JGW_W2A(strAppName);
                    sWlanProtocolInfo.push_back(sChannelExcelInfo);
                }
            }

        }

        return true;
#else
        std::wstring strTemp = configIni.GetIniKeyValue(strAppName.c_str(),L"chainMask",L"1");
        std::vector<int> vTemp;
        JGW_ParserIntW(strTemp.c_str(),L",",vTemp);
        for (size_t i = 0;i < vTemp.size();i++)
        {
            sWlanProtocolInfo.mvChainMask.push_back((WLAN_CHAIN_MASK2)vTemp[i]);
        }

        sWlanProtocolInfo.meRateBw = (WLAN_UniversalRateBW)configIni.GetIniKeyIntValue(strAppName.c_str(),L"rate_bw");
        GetExcelSheetChannel(configIni.GetIniKeyValue(strAppName.c_str(),L"channel_start_row_col"),sWlanProtocolInfo.mvChannel);
        GetExcelSheetRate(configIni.GetIniKeyValue(strAppName.c_str(),L"rate_row"),configIni.GetIniKeyValue(strAppName.c_str(),L"rate_col"),sWlanProtocolInfo.mveRate);
        sWlanProtocolInfo.muChainIntervalPos = configIni.GetIniKeyIntValue(strAppName.c_str(),L"chain_interval");

        if (0 == sWlanProtocolInfo.mvChannel.size())
        {
            mstrErrorMessage = L"Channel configuration error";
            return false;
        }
        if (0 == sWlanProtocolInfo.mveRate.size())
        {
            mstrErrorMessage = L"Rate configuration error";
            return false;
        }

        strTemp = configIni.GetIniKeyValue(strAppName.c_str(),L"tx_power_row_col");
        vTemp.clear();
        JGW_ParserIntW(strTemp.c_str(),L",",vTemp);

        for (size_t chainIndex = 0;chainIndex < sWlanProtocolInfo.mvChainMask.size();chainIndex++)
        {
            for (size_t rateIndex = 0;rateIndex < sWlanProtocolInfo.mveRate.size();rateIndex++)
            {
                for (size_t channelIndex = 0;channelIndex < sWlanProtocolInfo.mvChannel.size();channelIndex++)
                {
                    S_CHANNEL_EXCEL_INFO sChannelExcelInfo;
                    sChannelExcelInfo.meChainMask = sWlanProtocolInfo.mvChainMask[chainIndex];
                    sChannelExcelInfo.muChannel = sWlanProtocolInfo.mvChannel[channelIndex];
                    sChannelExcelInfo.meDataRate = sWlanProtocolInfo.mveRate[rateIndex];
                    //! tx_power_row_col
                    sChannelExcelInfo.muTxPowerPos











                        sWlanProtocolInfo.mvChainExcelInfo.push_back(sChannelExcelInfo);
                }
            }

        }

        return false;
#endif
    }

    WLAN_UniversalDataRate CCJGW_WlanExcelTemplateConfig::GetWlanUniversalDataRateToContent(const std::wstring& strContent)
    {
        if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"6Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"6Mbps"))
        {
            return WLAN_Universal_RATE_6Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"1Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"Long_1Mbps"))
        {
            return WLAN_Universal_RATE_Long_1Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"Long_2Mbps"))
        {
            return WLAN_Universal_RATE_Long_2Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"Long_5_5Mbps"))
        {
            return WLAN_Universal_RATE_Long_5_5Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"Long_11Mbps"))
        {
            return WLAN_Universal_RATE_Long_11Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"2Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"Short_2Mbps"))
        {
            return WLAN_Universal_RATE_Short_2Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"5.5Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"Short_5_5Mbps"))
        {
            return WLAN_Universal_RATE_Short_5_5Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"11Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"Short_11Mbps"))
        {
            return WLAN_Universal_RATE_Short_11Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"9Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"9Mbps"))
        {
            return WLAN_Universal_RATE_9Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"9Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"9Mbps"))
        {
            return WLAN_Universal_RATE_12Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"18Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"18Mbps"))
        {
            return WLAN_Universal_RATE_18Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"24Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"24Mbps"))
        {
            return WLAN_Universal_RATE_24Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"36Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"36Mbps"))
        {
            return WLAN_Universal_RATE_36Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"48Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"48Mbps"))
        {
            return WLAN_Universal_RATE_48Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"54Mb/s") || 0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"54Mbps"))
        {
            return WLAN_Universal_RATE_54Mbps;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS0"))
        {
            return RATE_MCS_0;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS1"))
        {
            return RATE_MCS_1;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS2"))
        {
            return RATE_MCS_2;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS3"))
        {
            return RATE_MCS_3;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS5"))
        {
            return RATE_MCS_5;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS4"))
        {
            return RATE_MCS_4;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS6"))
        {
            return RATE_MCS_6;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS7"))
        {
            return RATE_MCS_7;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS8"))
        {
            return RATE_MCS_8;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS9"))
        {
            return RATE_MCS_9;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS11"))
        {
            return RATE_MCS_11;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS10"))
        {
            return RATE_MCS_10;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS12"))
        {
            return RATE_MCS_12;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(strContent.c_str(),L"MCS13"))
        {
            return RATE_MCS_13;
        }
        return WLAN_Universal_CW;
    }
}
