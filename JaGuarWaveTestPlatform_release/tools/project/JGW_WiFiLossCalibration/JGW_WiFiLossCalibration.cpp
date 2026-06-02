// JGW_WiFiLossCalibration.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include "JGW_WiFiLossCalibrationDefine.h"
#include "CJGW_ResolveWiFiTestLog.h"
#include "CJGW_ResolveStationCalDataFile.h"

// golden units 金机设备测试LOG
std::wstring gstrGoldenUnitsTestLogPath = L"D:\\Development\\RFNET\\2023-03-11\\R100-IQ\\JW\\exec\\R100_GU\\P_JR100PR2232304_21199272.log";
// 金机设备的线损文件
//std::wstring gstrGoldenUnitsStationCalDataFilePath = L"D:\\Development\\RFNET\\2023-03-11\\R100-IQ\\JW\\exec\\R100_GU\\R100_StationCalDataFile_Split8.xml";
// Device Under Test 被测样机测试LOG
std::wstring gstrDutTestLogPath = L"D:\\Development\\RFNET\\2023-03-11\\R100-IQ\\JW\\exec\\TestLog\\R100_keysight\\WiFi\\wifi_test_all.xml\\2024-08-23\\P_JR100PR2249127_16169125.log";
// 被测样机使用的线损文件
std::wstring gstrDutStationCalDataFilePath = L"D:\\Development\\RFNET\\2023-03-11\\R100-IQ\\JW\\exec\\equip\\R100_IQ\\R100_StationCalDataFile_Split8.xml";
// 默认CH1对应BH0_LP,CH1对应BH2_LP,CH1对应BH3_LP,CH1对应BH4_LP

int _tmain(int argc, _TCHAR* argv[])
{
    JGW::CCJGW_ResolveWiFiTestLog cResolveWiFiTestLog;
    JGW::CCJGW_ResolveStationCalDataFile cResolveStationCalDataFile;
    S_WIFI_TEST_INFO sTempWiFiTestInfo;
#if 1
    std::vector<S_WIFI_TEST_INFO> vsGoldenUnitsWiFiTestInfo;
    if (!cResolveWiFiTestLog.ResolveWiFiTestLog(gstrGoldenUnitsTestLogPath,vsGoldenUnitsWiFiTestInfo))
    {
        std::wcout << "Golden Units Test Log Parsing error : " << gstrGoldenUnitsTestLogPath << std::endl;
        return -1;
    }

    //std::vector<JGW::S_LOSS_PATH> vsGoldenUnitsLossPath;
    //if (!cResolveStationCalDataFile.ResolveStationCalDataFile(gstrGoldenUnitsStationCalDataFilePath,vsGoldenUnitsLossPath))
    //{
    //    std::wcout << "Dut Station Cal Data File Parsing error : " << gstrGoldenUnitsStationCalDataFilePath << std::endl;
    //    return -1;
    //}
    

    std::vector<S_WIFI_TEST_INFO> vsDutWiFiTestInfo;
    if (!cResolveWiFiTestLog.ResolveWiFiTestLog(gstrDutTestLogPath,vsDutWiFiTestInfo))
    {
        std::wcout << "Dut Test Log Parsing error : " << gstrDutTestLogPath << std::endl;
        return -1;
    }
#endif

    for (std::vector<S_WIFI_TEST_INFO>::iterator itDut = vsDutWiFiTestInfo.begin();itDut != vsDutWiFiTestInfo.end();++itDut)
    {
        for (std::vector<S_WIFI_TEST_INFO>::iterator itGoldenUnits = vsGoldenUnitsWiFiTestInfo.begin();itGoldenUnits != vsGoldenUnitsWiFiTestInfo.end();++itGoldenUnits)
        {
            // 线损校准默认与制式无关
            if (itDut->mnChannel == itGoldenUnits->mnChannel && itDut->meWlanChain == itGoldenUnits->meWlanChain && itDut->meUniversalDataRate == itGoldenUnits->meUniversalDataRate && itDut->meUniversalRateBW == itGoldenUnits->meUniversalRateBW)
            {
                std::cout << itDut->meWlanChain << "_" << "CH" << itDut->mnChannel << " (" << itGoldenUnits->mfPower << "-" << itDut->mfPower <<"):" << static_cast<float>(itGoldenUnits->mfPower - itDut->mfPower) << std::endl;
                if (WLAN_CHAIN_1 == itDut->meWlanChain)
                {
                    // power diff
                    cResolveStationCalDataFile.UpdateFreqLoss(L"BH0_LP",itDut->mnChannel,static_cast<float>(itGoldenUnits->mfPower - itDut->mfPower));
                }
                if (WLAN_CHAIN_2 == itDut->meWlanChain)
                {
                    cResolveStationCalDataFile.UpdateFreqLoss(L"BH1_LP",itDut->mnChannel,static_cast<float>(itGoldenUnits->mfPower - itDut->mfPower));
                }
                if (WLAN_CHAIN_3 == itDut->meWlanChain)
                {
                    cResolveStationCalDataFile.UpdateFreqLoss(L"BH2_LP",itDut->mnChannel,static_cast<float>(itGoldenUnits->mfPower - itDut->mfPower));
                }
                if (WLAN_CHAIN_4 == itDut->meWlanChain)
                {
                    cResolveStationCalDataFile.UpdateFreqLoss(L"BH3_LP",itDut->mnChannel,static_cast<float>(itGoldenUnits->mfPower - itDut->mfPower));
                }
                break;
            }
        }
    }

    if (!cResolveStationCalDataFile.SaveStationCalDataFile(gstrDutStationCalDataFilePath))
    {
        std::wcout << "Failed to save the file by calibrating line loss : " << gstrDutStationCalDataFilePath << std::endl;
        return -1;
    }

    std::wcout << L"Calibrating Loss File Save successfully" << std::endl;
    //std::vector<JGW::S_LOSS_PATH> vsDutLossPath;
    //if (!cResolveStationCalDataFile.ResolveStationCalDataFile(gstrDutStationCalDataFilePath,vsDutLossPath))
    //{
    //    std::wcout << "Dut Station Cal Data File Parsing error : " << gstrDutStationCalDataFilePath << std::endl;
    //    return -1;
    //}

	return 0;
}

