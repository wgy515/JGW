#include "StdAfx.h"
#include "CTSEGuCalWlanLoss.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <map>
#include "JGW_WiFiLossCalibration\JGW_WiFiLossCalibrationDefine.h"
#include "JGW_WiFiLossCalibration\CJGW_ResolveWiFiTestLog.h"
#include "JGW_WiFiLossCalibration\CJGW_ResolveStationCalDataFile.h"

namespace JGW
{
    CCTSEGuCalWlanLoss::CCTSEGuCalWlanLoss(void)
    {
    }


    CCTSEGuCalWlanLoss::~CCTSEGuCalWlanLoss(void)
    {
    }

    std::string strFilePathTemp;
    const std::string strFilterFileName = "P_";

    std::string PareseSerialNumber(const std::string& fileName)
    {
        int start = 0,end = 0;
        start = fileName.find('_');
        end = fileName.find('_',start + 1);
        if (start >= 0 && end > 0 && start != end)
        {
            return fileName.substr(start + 1,end - start - 1);
        }
        return "0000000000000";
    }

    void ScanTestDataFile(const char* filepath,const char* filename,void* lpData)
    {
        if (NULL != strstr(filename,strFilterFileName.c_str()) && NULL != lpData)
        {
            strFilePathTemp = filepath;
            strFilePathTemp += "\\";
            strFilePathTemp += filename;
            S_QIA_WLAN_TESTDATA_FILE_INFO sQiaWlanTestDataInfo = {PareseSerialNumber(filename),strFilePathTemp};
            ((std::vector<S_QIA_WLAN_TESTDATA_FILE_INFO>*)lpData)->push_back(sQiaWlanTestDataInfo);
            //((std::map<std::string,std::string>*)lpData)->insert(std::make_pair<std::string,std::string>(PareseSerialNumber(filename),strFilePathTemp));
        }
    }

    bool CCTSEGuCalWlanLoss::LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam)
    {
        CCJGW_ConfigIni configIni;
        configIni.InitIniFilePath(strGuCalConfigPath.c_str());
        // 金机测试LOG目录
        mvGoldenUnitsDataFiles.clear();
        mstrGoldenUnitsTestDataFolder = configIni.GetIniKeyValue(L"QIA_WLAN",L"GoldenUnitsTestDataFloder");
        mstrGoldenUnitsTestDataFolder = JGW_RealativePathToAbsPath(mstrGoldenUnitsTestDataFolder.c_str());
        if (!JGW_IsFolderExist(mstrGoldenUnitsTestDataFolder.c_str()))
        {
            MessageBox(FindWindow(NULL,NULL),L"金机测试数据目录不存在",L"Error",MB_OK);
            return false;
        }
        //! 检查金机测试数据文件是否存在
        int numFiles = 0;
        JGW_TraverseFolder_C(JGW_W2A(mstrGoldenUnitsTestDataFolder),numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanTestDataFile,&mvGoldenUnitsDataFiles);
        if (numFiles < 1)
        {
            MessageBox(FindWindow(NULL,NULL),(mstrGoldenUnitsTestDataFolder + L"，此目录没有找到金机测试数据").c_str(),L"Error",MB_OK);
            return false;
        }

        // 待测样机线损文件路径
        mstrDutStationCalDataFilePath = configIni.GetIniKeyValue(L"QIA_WLAN",L"DutStationCalDataFilePath");
        mstrDutStationCalDataFilePath = JGW_RealativePathToAbsPath(mstrDutStationCalDataFilePath.c_str());
        if (!JGW_FileExistsToFilePath(mstrDutStationCalDataFilePath.c_str()))
        {
            MessageBox(FindWindow(NULL,NULL),L"待测样机线损文件不存在",L"Error",MB_OK);
            return false;
        }
        // 生成新的校准后的线损文件路径,默认情况下待测样机线损文件路径与生成新的校准后的线损文件路径一致
        mstrSaveDutStationCalDataFilePath = configIni.GetIniKeyValue(L"QIA_WLAN",L"SaveDutStationCalDataFilePath");
        if (mstrSaveDutStationCalDataFilePath.empty())
        {
            mstrSaveDutStationCalDataFilePath = mstrDutStationCalDataFilePath;
        }
        else
        {
            mstrSaveDutStationCalDataFilePath = JGW_RealativePathToAbsPath(mstrSaveDutStationCalDataFilePath.c_str());
        }
        // 开始校准后待测样机测试LOG保存目录
        mstrDutTestLogFloder = configIni.GetIniKeyValue(L"QIA_WLAN",L"DutTestLogFloder");
        mstrDutTestLogFloder = JGW_RealativePathToAbsPath(mstrDutTestLogFloder.c_str());
        mstrDutTestLogFloder = JGW_GetFormatWString(L"%s\\%s",mstrDutTestLogFloder.c_str(),JGW_GetTimeFolder().c_str());
        // 默认删除当前测试目录，每次加载的时候
#ifndef _DEBUG
        JGW_DeleteDirectoryW(mstrDutTestLogFloder);
#endif
        return true;
    }

    std::wstring CCTSEGuCalWlanLoss::GetDutTestDataFilePath()
    {
        return mstrDutTestLogFloder;
    }

    typedef struct  
    {
        std::string mstrSerialNumber;
        std::string mstrGoldenUnitsTestDataFilePath;
        std::vector<std::string> mvstrDutTestDataFilePath;
    }S_GU_QIA_WLAN_TESTDATA_INFO;

    std::wstring CCTSEGuCalWlanLoss::SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath)
    {
        int numFiles = 0;
        std::string strPath = JGW_W2A(strDutTestDataFilePath);
        std::vector<S_QIA_WLAN_TESTDATA_FILE_INFO> vDutTestDataFiles; 
        //! 1、检查Dut校准测试数据文件数量
        JGW_TraverseFolder_C(strPath,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanTestDataFile,&vDutTestDataFiles);
        if (numFiles < 1)
        {
            return JGW_A2W(strPath) + L"，此目录没有找到DUT测试数据" ;
        }
    

        //! 3、分类，同一个SN的DUT测试数据和一个SN的金机数据进行分类
        std::vector<S_GU_QIA_WLAN_TESTDATA_INFO> vsDutGuGoldenUnitsDataFiles;

        for (std::vector<S_QIA_WLAN_TESTDATA_FILE_INFO>::iterator itGoldenUnitsTestDataFiles = mvGoldenUnitsDataFiles.begin();itGoldenUnitsTestDataFiles != mvGoldenUnitsDataFiles.end();itGoldenUnitsTestDataFiles++)
        {
            S_GU_QIA_WLAN_TESTDATA_INFO sGuQiaWlanTestDataInfo;
            sGuQiaWlanTestDataInfo.mstrSerialNumber = itGoldenUnitsTestDataFiles->mstrSerialNumber;
            sGuQiaWlanTestDataInfo.mstrGoldenUnitsTestDataFilePath = itGoldenUnitsTestDataFiles->mstrTestDataFilePath;
            // 检查是否有相同SN的DUT测试数据
            for (std::vector<S_QIA_WLAN_TESTDATA_FILE_INFO>::iterator itDutTestDataFiles = vDutTestDataFiles.begin();itDutTestDataFiles != vDutTestDataFiles.end();itDutTestDataFiles++)
            {
                if (0 == JGW_StrComparenoCaseStr(itDutTestDataFiles->mstrSerialNumber.c_str(),itGoldenUnitsTestDataFiles->mstrSerialNumber.c_str()))
                {
                    sGuQiaWlanTestDataInfo.mvstrDutTestDataFilePath.push_back(itDutTestDataFiles->mstrTestDataFilePath);
                }
            }
            // 判断DUT测试数据是否为空
            if (!sGuQiaWlanTestDataInfo.mvstrDutTestDataFilePath.empty())
            {
                vsDutGuGoldenUnitsDataFiles.push_back(sGuQiaWlanTestDataInfo);
            }
        }
#if 0
        //! 3、检查DUT测试号段是否在金机测试号段范围内,在里面就进行处理，不在里面就直接过滤掉
        std::vector<S_GU_QIA_WLAN_TESTDATA_INFO> vsDutGuGoldenUnitsDataFiles;
        CCJGW_ResolveWiFiTestLog cResolveWiFiTestLog;

        for (std::vector<S_QIA_WLAN_TESTDATA_INFO>::iterator itDutTestDataFiles = vDutTestDataFiles.begin();itDutTestDataFiles != vDutTestDataFiles.end();itDutTestDataFiles++)
        {
            for (std::vector<S_QIA_WLAN_TESTDATA_INFO>::iterator itGoldenUnitsTestDataFiles = vGoldenUnitsDataaFiles.begin();itGoldenUnitsTestDataFiles != vGoldenUnitsDataaFiles.end();itGoldenUnitsTestDataFiles++)
            {
                if (0 == JGW_StrComparenoCaseStr(itDutTestDataFiles->mstrSerialNumber.c_str(),itGoldenUnitsTestDataFiles->mstrSerialNumber.c_str()))
                {
                    S_GU_QIA_WLAN_TESTDATA_INFO sGuQiaWlanTestDataInfo;
                    sGuQiaWlanTestDataInfo.mstrSerialNumber = itDutTestDataFiles->mstrSerialNumber;
                    sGuQiaWlanTestDataInfo.mstrDutTestDataFiles = itDutTestDataFiles->mstrTestDataFilePath;
                    sGuQiaWlanTestDataInfo.mstrGoldenUnitsTestDataFilePath = itGoldenUnitsTestDataFiles->mstrTestDataFilePath;
                    sGuQiaWlanTestDataInfo.mpvsGoldenUnitsWiFiTestInfo = NULL;
                    // 判断是否加载金机的测试数据
                    for (std::vector<S_GU_QIA_WLAN_TESTDATA_INFO>::iterator it = vsDutGuGoldenUnitsDataFiles.begin();it != vsDutGuGoldenUnitsDataFiles.end();++it)
                    {
                        if (0 == JGW_StrComparenoCaseStr(sGuQiaWlanTestDataInfo.mstrGoldenUnitsTestDataFilePath.c_str(),it->mstrGoldenUnitsTestDataFilePath.c_str()))
                        {
                            sGuQiaWlanTestDataInfo.mpvsGoldenUnitsWiFiTestInfo = it->mpvsGoldenUnitsWiFiTestInfo;
                            continue;
                        }
                    }
                    // 加载DUT测试数据
                    if (!cResolveWiFiTestLog.ResolveWiFiTestLog(JGW_A2W(sGuQiaWlanTestDataInfo.mstrDutTestDataFiles),sGuQiaWlanTestDataInfo.mvsDutWiFiTestInfo))
                    {
                        //std::wcout << "Golden Units Test Log Parsing error : " << gstrGoldenUnitsTestLogPath << std::endl;
                        return JGW_GetFormatWString(L"%s%s",L"Dut Test Log Parsing error : ",JGW_A2W(sGuQiaWlanTestDataInfo.mstrDutTestDataFiles).c_str());
                    }
                    // 加载金机测试数据
                    if (NULL == sGuQiaWlanTestDataInfo.mpvsGoldenUnitsWiFiTestInfo)
                    {
                        sGuQiaWlanTestDataInfo.mpvsGoldenUnitsWiFiTestInfo = new std::vector<S_WIFI_TEST_INFO>;
                        if (!cResolveWiFiTestLog.ResolveWiFiTestLog(JGW_A2W(sGuQiaWlanTestDataInfo.mstrGoldenUnitsTestDataFilePath),*sGuQiaWlanTestDataInfo.mpvsGoldenUnitsWiFiTestInfo))
                        {
                            return JGW_GetFormatWString(L"%s%s",L"Golden Units Test Log Parsing error : ",JGW_A2W(sGuQiaWlanTestDataInfo.mstrGoldenUnitsTestDataFilePath).c_str());
                        }
                    }
                    
                    vsDutGuGoldenUnitsDataFiles.push_back(sGuQiaWlanTestDataInfo);
                }
            }
        }
#endif
        //! 4、进行数据整合启动
        CCJGW_ResolveWiFiTestLog cResolveWiFiTestLog;
        std::vector<S_WIFI_TEST_INFO> vsGoldenUnitsWiFiTestInfo;
        CCJGW_ResolveStationCalDataFile cResolveStationCalDataFile;

        for (std::vector<S_GU_QIA_WLAN_TESTDATA_INFO>::iterator it = vsDutGuGoldenUnitsDataFiles.begin();it != vsDutGuGoldenUnitsDataFiles.end();++it)
        {
            // 解析金机测试数据LOG
            vsGoldenUnitsWiFiTestInfo.clear();
            if (!cResolveWiFiTestLog.ResolveWiFiTestLog(JGW_A2W(it->mstrGoldenUnitsTestDataFilePath),vsGoldenUnitsWiFiTestInfo) && vsGoldenUnitsWiFiTestInfo.empty())
            {
                return JGW_GetFormatWString(L"%s%s",L"Golden Units Test Log Parsing error : ",JGW_A2W(it->mstrGoldenUnitsTestDataFilePath).c_str());
            }
            // 遍历DUT测试数据
            std::vector<S_WIFI_TEST_INFO> vsDutTempWiFiTestInfo;
            std::vector<S_WIFI_TEST_INFO> vsDutWiFiTestInfo;
            for (std::vector<std::string>::iterator itDutTestDataFile = it->mvstrDutTestDataFilePath.begin();itDutTestDataFile != it->mvstrDutTestDataFilePath.end();++itDutTestDataFile)
            {
                // 解析DUT测试数据
                vsDutTempWiFiTestInfo.clear();
                if (!cResolveWiFiTestLog.ResolveWiFiTestLog(JGW_A2W(itDutTestDataFile[0]),vsDutTempWiFiTestInfo) && vsDutTempWiFiTestInfo.empty())
                {
                    return JGW_GetFormatWString(L"%s%s",L"Dut Test Log Parsing error : ",JGW_A2W(itDutTestDataFile[0]).c_str());
                }
                // 所有DUT测试数据的功率进行相加
                for (std::vector<S_WIFI_TEST_INFO>::iterator itWiFiTestInfo = vsDutTempWiFiTestInfo.begin();itWiFiTestInfo != vsDutTempWiFiTestInfo.end();++itWiFiTestInfo)
                {
                    bool bFindWiFiTestInfo = false;
                    for (std::vector<S_WIFI_TEST_INFO>::iterator itDutWiFiTestInfo = vsDutWiFiTestInfo.begin();itDutWiFiTestInfo != vsDutWiFiTestInfo.end();++itDutWiFiTestInfo)
                    {
                        if (itDutWiFiTestInfo->mnChannel == itWiFiTestInfo->mnChannel && itDutWiFiTestInfo->meWlanChain == itWiFiTestInfo->meWlanChain && itWiFiTestInfo->meUniversalRateBW == itDutWiFiTestInfo->meUniversalRateBW && itWiFiTestInfo->meUniversalDataRate == itDutWiFiTestInfo->meUniversalDataRate)
                        {
                            itDutWiFiTestInfo->mfPower += itWiFiTestInfo->mfPower;
                            itDutWiFiTestInfo->mnPosIndex++;
                            bFindWiFiTestInfo = true;
                        }
                    }
                    if (!bFindWiFiTestInfo)
                    {
                        itWiFiTestInfo->mnPosIndex = 1;
                        vsDutWiFiTestInfo.push_back(itWiFiTestInfo[0]);
                    }
                } 
            }
            // 对功率值进行平均
            for (std::vector<S_WIFI_TEST_INFO>::iterator itDutWiFiTestInfo = vsDutWiFiTestInfo.begin();itDutWiFiTestInfo != vsDutWiFiTestInfo.end();++itDutWiFiTestInfo)
            {
                if (itDutWiFiTestInfo->mnPosIndex > 0)
                {
                    itDutWiFiTestInfo->mfPower /= itDutWiFiTestInfo->mnPosIndex;
                }
            }
            // 更新金机线损数据
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
        }

        if (!cResolveStationCalDataFile.SaveStationCalDataFile(mstrSaveDutStationCalDataFilePath))
        {
            return JGW_GetFormatWString(L"%s%s",L"Failed to save the file by calibrating line loss : ",mstrSaveDutStationCalDataFilePath.c_str());
            //std::wcout << "Failed to save the file by calibrating line loss : " << gstrDutStationCalDataFilePath << std::endl;
            //return -1;
        }

        return L"";
    }
}
