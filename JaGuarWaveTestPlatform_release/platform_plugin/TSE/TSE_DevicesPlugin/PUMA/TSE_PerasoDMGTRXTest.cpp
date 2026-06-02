#include "StdAfx.h"
#include "TSE_PerasoDMGTRXTest.h"
#include "CJGW_DMGRxResult.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include <fstream>
#include <TSE_FoundationLib/TSE_BaseConfigXML.h>
namespace JGW
{
    CTSE_PerasoDMGTRXTest::CTSE_PerasoDMGTRXTest(void) : mnRecvTimeMs(4 * 1000),mbTestResult(false),mnItemTestErrorRetryCount(1),mbInitializeOnce(false),mnPPDUDetecs(10000),mnRssiTestRange(3),mbAutomaticCalibrationConfigurationTest(false)
    {
        //std::wstring strProjectName = GetGlobalEnvironment()->GetString(L"TSE_TEST_PROJECT_NAME");
        JGW_FormatWString(mstrDMGCsvLogFilePath,L"%s%s%s\\dmg_log.csv",JGW_GetApplicationFolder(),L"TestLog\\DMG\\",JGW_GetTimeFolder().c_str());
        JGW_CreateDirectory(mstrDMGCsvLogFilePath.c_str(),false);
    }


    CTSE_PerasoDMGTRXTest::~CTSE_PerasoDMGTRXTest(void)
    {
    }

    const wchar_t* CTSE_PerasoDMGTRXTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PerasoDMGTRXTest\",\
                \"TestName\": \"DMG TRX Test\",\
                \"GuSerialComPortEnv\": \"Golden SerailComPort\",\
                \"DutSerialComPortEnv\": \"Dut SerailComPort\",\
                \"GoldenXmlConfigPath\": \"Golden Xml Config Path\",\
                \"AutomaticCalibrationConfigurationTest\": \"Golden Auto Cal Test\",\
                \"RssiTestRange\": \"Rssi Test Range\",\
                \"Channels\": \"测试信道(以逗号作为分隔符),1,2,3,4,5,6\",\
                \"GoldenSector\": \"金机Sector\",\
                \"DutSector\": \"DUT Sector\",\
                \"RecvTimeMs\": \"RX接收时间:单位毫秒\",\
                \"PhyTestTxTestCommand\": \"TX PHY 命令\",\
                \"PPDUDetectEnv\": \"PPDU Detects 环境变量\",\
                \"RssiRxGuTxDut\": \"RSSI RX-GU TX-DUT(为空则代表不测试),以逗号作为分隔符代表每个信道对应的测试标准环境变量\",\
                \"FerRxGuTxDut\": \"FER RX-GU TX-DUT(千分比)以逗号作为分隔符代表每个信道对应的测试标准环境变量\",\
                \"RssiTxGuRxDut\": \"RSSI TX-GU RX-DUT(为空则代表不测试) 以逗号作为分隔符代表每个信道对应的测试标准环境变量\",\
                \"FerTxGuRxDut\": \"FER TX-GU RX-DUT(千分比)以逗号作为分隔符代表每个信道对应的测试标准环境变量\",\
                \"ItemTestErrorRetryCount\": \"单项测试错误重测次数\"\
                }";
    }

    bool CTSE_PerasoDMGTRXTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"GuSerialComPortEnv"))
        {
            mstrGuSerialComPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DutSerialComPortEnv"))
        {
            mstrDutSerialComPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"AutomaticCalibrationConfigurationTest"))
        {
            mbAutomaticCalibrationConfigurationTest = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"GoldenXmlConfigPath"))
        {
            mstrGoldenXmlConfigPath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"RssiTestRange"))
        {
            mnRssiTestRange = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Channels"))
        {
            mstrChannelsEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"GoldenSector"))
        {
            mstrGuSectorEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PPDUDetectEnv"))
        {
            mstrPPDUDetectEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DutSector"))
        {
            mstrDutSectorEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"RecvTimeMs"))
        {
            mstrRecvTimeMsEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PhyTestTxTestCommand"))
        {
            mstrPhyTestTxCommandEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"RssiRxGuTxDut"))
        {
            mstrRssiRxGuTxDutEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"FerRxGuTxDut"))
        {
            mstrFerRxGuTxDutEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"RssiTxGuRxDut"))
        {
            mstrRssiTxGuRxDutEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"FerTxGuRxDut"))
        {
            mstrFerTxGuRxDutEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ItemTestErrorRetryCount"))
        {
            mnItemTestErrorRetryCount = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    void CTSE_PerasoDMGTRXTest::InitDMGPerasoInfo(S_DMG_PERASO_INFO& sDMGPerasoInfo)
    {
        sDMGPerasoInfo.mstrConfigPdbVersion = L"";
        sDMGPerasoInfo.mstrFirewareVersion = L"";
        sDMGPerasoInfo.mstrMacAddress = L"";
        sDMGPerasoInfo.mstrSector = L"";
        sDMGPerasoInfo.mstrSerialNumber = L"";
    }

    void CTSE_PerasoDMGTRXTest::InitDMGTRxResults(LPS_DMG_TRX_RESULT psDMGTRxLogResults)
    {
        for (int channel = 1;channel <= PERASO_MAX_CHANNEL;channel ++)
        {
            psDMGTRxLogResults[channel - 1].mnChannel = channel;
            psDMGTRxLogResults[channel - 1].mnRssi = -99999;
            psDMGTRxLogResults[channel - 1].mnFerPerimile = -99999;
            psDMGTRxLogResults[channel - 1].mnUgain = -99999;
            psDMGTRxLogResults[channel - 1].mstrFerPermile = L"";
            psDMGTRxLogResults[channel - 1].mstrRssi = L"";
            psDMGTRxLogResults[channel - 1].mstrUgain = L"";
        }
    }

    bool CTSE_PerasoDMGTRXTest::InitTestParamEnvironment()
    {
        if (!mbInitializeOnce)
        {
            std::wstring strProjectName = GetGlobalEnvironment()->GetString(L"TSE_TEST_PROJECT_NAME");
            JGW_FormatWString(mstrDMGCsvLogFilePath,L"%sTestLog\\%s\\%s\\dmg_log.csv",JGW_GetApplicationFolder(),strProjectName.empty()?L"DMG":strProjectName.c_str(),JGW_GetTimeFolder().c_str());
            JGW_CreateDirectory(mstrDMGCsvLogFilePath.c_str(),false);
            mbInitializeOnce = true;
        }
        //!mstrGuSerialNumber = GetGlobalEnvironment()->GetAnsiString(mstrGuSerialNumberEnv);
        //CCJGW_CSerialComPort* guSerialComPort = NULL,*dutSerialComPort = NULL;
        //guSerialComPort = (CCJGW_CSerialComPort*)GetGlobalEnvironment()->GetInt(mstrGuSerialComPortEnv);
        //dutSerialComPort = (CCJGW_CSerialComPort*)GetGlobalEnvironment()->GetInt(mstrDutSerialComPortEnv);
        InitDMGPerasoInfo(msDmgTRxLog.msDutDmgPerasoInfo);
        InitDMGPerasoInfo(msDmgTRxLog.msGuDmgPerasoInfo);
        InitDMGTRxResults(msDmgTRxLog.mvsDutTxGuRxResults);
        InitDMGTRxResults(msDmgTRxLog.mvsGuTxDutRxResults);

        std::wstring strTemp;
        mvChannels.clear();
        JGW_ParserIntW(GetGlobalEnvironment()->GetString(mstrChannelsEnv).c_str(),L",",mvChannels);
        for (size_t i = 0;i < mvChannels.size();i ++)
        {
            if (mvChannels[i] < 1 || mvChannels[i] > 6)
            {
                LogE_F(L"11AD Channel Range:1~6,Current Channel:%d",mvChannels[i]);
                return false;
            }
        }

        msDmgTRxLog.msGuDmgPerasoInfo.mstrSector = GetGlobalEnvironment()->GetString(mstrGuSectorEnv);
        msDmgTRxLog.msDutDmgPerasoInfo.mstrSector = GetGlobalEnvironment()->GetString(mstrDutSectorEnv);
        mnRecvTimeMs = GetGlobalEnvironment()->GetInt(mstrRecvTimeMsEnv);
        mstrPhyTestTxCommand = GetGlobalEnvironment()->GetString(mstrPhyTestTxCommandEnv);

        strTemp = GetGlobalEnvironment()->GetString(mstrRssiRxGuTxDutEnv);
        if (!strTemp.empty())
        {
            mvRssiRxGuTxDut.clear();
            JGW_ParserIntW(strTemp.c_str(),L",",mvRssiRxGuTxDut);
            while (mvRssiRxGuTxDut.size() < mvChannels.size())
            {
                mvRssiRxGuTxDut.push_back(mvRssiRxGuTxDut[0]);
            }
        }

        strTemp = GetGlobalEnvironment()->GetString(mstrFerRxGuTxDutEnv);
        if (!strTemp.empty())
        {
            mvFerRxGuTxDut.clear();
            JGW_ParserIntW(strTemp.c_str(),L",",mvFerRxGuTxDut);
            while (mvFerRxGuTxDut.size() < mvChannels.size())
            {
                mvFerRxGuTxDut.push_back(mvFerRxGuTxDut[0]);
            }
        }

        strTemp = GetGlobalEnvironment()->GetString(mstrRssiTxGuRxDutEnv);
        if (!strTemp.empty())
        {
            mvRssiTxGuRxDut.clear();
            JGW_ParserIntW(strTemp.c_str(),L",",mvRssiTxGuRxDut);
            while (mvRssiTxGuRxDut.size() < mvChannels.size())
            {
                mvRssiTxGuRxDut.push_back(mvRssiTxGuRxDut[0]);
            }
        }

        strTemp = GetGlobalEnvironment()->GetString(mstrFerTxGuRxDutEnv);
        if (!strTemp.empty())
        {
            mvFerTxGuRxDut.clear();
            JGW_ParserIntW(strTemp.c_str(),L",",mvFerTxGuRxDut);
            while (mvFerTxGuRxDut.size() < mvChannels.size())
            {
                mvFerTxGuRxDut.push_back(mvFerTxGuRxDut[0]);
            }
        }

        return true;
    }

    bool CTSE_PerasoDMGTRXTest::DisableTestMode(CCJGW_DonglesComPort& donglesComPort)
    {
#if 0
        std::string strRead;

        //SendViewTestLog(L">phy test off");
        bool result = donglesComPort.ReadWriteCommands("phy test off\n",strRead);
        //SendViewTestLog(JGW_A2W(strRead));

        return result && strRead.empty();
#else
        return true;
#endif
    }

    bool CTSE_PerasoDMGTRXTest::EnableTestMode(CCJGW_DonglesComPort& donglesComPort)
    {
        std::string strRead;
        std::wstring strLog;

        for (int i = 0;i < 3;i ++)
        {
            //SendViewTestLog(L">phy test on");
            bool result = donglesComPort.ReadWriteCommands("phy test on\n",strRead);
            if (!(result && strRead.empty()))
            {
                JGW_FormatWString(strLog,L"phy test on result:%s fail",JGW_A2W(strRead).c_str());
                SendViewTestLog(strLog);
                Sleep(1500);
                continue;
            }
            //SendViewTestLog(JGW_A2W(strRead));
            result = donglesComPort.ReadWriteCommands("status\n",strRead);
            if (!result || NULL == JGW_StrCaseStr(strRead.c_str(),"Phy Test"))
            {
                //JGW_FormatWString(strLog,L"get phy test status:%s fail",JGW_A2W(strRead).c_str());
                //JGW_FormatWString(strLog,L"Peraso Mode Error")
                SendViewTestLog(L"Peraso Mode Error");
                Sleep(1500);
                continue;
            }
            return true;
        }
       
        return false;
    }

    bool CTSE_PerasoDMGTRXTest::ConfigureChannel(CCJGW_DonglesComPort& donglesComPort,int channel /* = 1 */)
    {
        ///std::wstring strCommand(mstrPhyTestTxCommand);
        ///JGW_ReplaceStringW(strCommand,L"$$sector$$",);
        std::string strCommand,strRead;

        JGW_FormatString(strCommand,"mib s 62-1-1-2 %02d\n",channel);
        //SendViewTestLogFormat(L">mib s 62-1-1-2 %02d",channel);
        bool result = donglesComPort.ReadWriteCommands(strCommand.c_str(),strRead);
        //SendViewTestLog(JGW_A2W(strRead));

        return result && strRead.empty();
    }

    bool CTSE_PerasoDMGTRXTest::TransmitDMGPackets(CCJGW_DonglesComPort& donglesComPort,const std::wstring& strSector)
    {
        std::string strRead;
        std::wstring strCommand(mstrPhyTestTxCommand);

        JGW_ReplaceStringW(strCommand,L"$$sector$$",strSector.c_str());
        //SendViewTestLogFormat(L">%s",strCommand.c_str());
        bool result = donglesComPort.ReadWriteCommands(JGW_W2A(strCommand + L"\n"),strRead);
        //SendViewTestLog(JGW_A2W(strRead));

        return result && strRead.empty();
    }

    bool CTSE_PerasoDMGTRXTest::ReceiveDMGPackets(CCJGW_DonglesComPort& donglesComPort,const std::wstring& strSector)
    {
        std::string strRead,strCommand;

        if (strSector.empty() || 0 == strSector.compare(L"99"))
        {
            strCommand = "phy test rx start";
        }
        else
        {
            JGW_FormatString(strCommand,"phy test rx start sector=%s\n",JGW_W2A(strSector).c_str());
        }
        
        //SendViewTestLogFormat(L">phy test rx start sector=%s",strSector.c_str());
        bool result = donglesComPort.ReadWriteCommands(strCommand,strRead);
        //SendViewTestLog(JGW_A2W(strRead));

        return result && strRead.empty();
    }
#if 0
    bool CTSE_PerasoDMGTRXTest::StopTxRxTest(CCJGW_DonglesComPort& donglesComPort)
    {
        std::string rsp;
        std::wstring strLog;

        donglesComPort.ReadWriteCommands("phy test off\n",rsp);

        bool result = donglesComPort.ReadWriteCommands("phy test on\n",rsp);
        result &= !rsp.empty();
        JGW_FormatWString(strLog,L"phy test on result:%s",JGW_A2W(rsp).c_str());
        SendViewTestLog(strLog);
        //Sleep(1500);

        result = donglesComPort.ReadWriteCommands("phy test tx stop\n",rsp);
        result &= !rsp.empty();
        JGW_FormatWString(strLog,L"phy test tx stop result:%s",JGW_A2W(rsp).c_str());
        SendViewTestLog(strLog);
        //Sleep(1500);

        result = donglesComPort.ReadWriteCommands("phy test rx stop\n",rsp);
        result &= !rsp.empty();
        JGW_FormatWString(strLog,L"phy test rx stop result:%s",JGW_A2W(rsp).c_str());
        SendViewTestLog(strLog);
        //Sleep(1500);
        return result;
    }
#endif

    std::string CTSE_PerasoDMGTRXTest::StopRxAndQueryResultsAfterSomeDelay(CCJGW_DonglesComPort& donglesComPort)
    {
        std::string rsp;

        //SendViewTestLogFormat(L"Sleep Time(millisecond):%d",mnRecvTimeMs);
        Sleep(mnRecvTimeMs);

        //SendViewTestLog(L">phy test rx stop");
        bool result = donglesComPort.ReadWriteCommands("phy test rx stop\n",rsp);
        //SendViewTestLog(L">phy test rx status");
        result = donglesComPort.ReadWriteCommands("phy test rx status\n",rsp);
        //SendViewTestLog(JGW_A2W(rsp).c_str());

        return rsp;
    }

    bool CTSE_PerasoDMGTRXTest::GetRxResultsToTestStandards(const std::string& strRxResult,int nRssiStandard,int nFerStandard,int& nRssi,int& FerPerimile)
    {
        CCJGW_DMGRxResult dmgRxResult(strRxResult);
        std::wstring strLog;
        if (0 == dmgRxResult.GetPPDUDetects() || mnPPDUDetecs > dmgRxResult.GetPPDUDetects())
        {
            SendViewTestLog(L"Receive PPDU packet exception!!!");
            return false;
        }
        nRssi = dmgRxResult.GetRssi();
        JGW_FormatWString(strLog,L"rssi\t%d\t%d\t%d\t",nRssiStandard, nRssi,0);
        SendViewTestLog(strLog);

        nFerStandard = dmgRxResult.GetFerPerimile();
        JGW_FormatWString(strLog,L"FerPerimile\t%d\t%d\t%d\t",0, FerPerimile,nFerStandard);
        SendViewTestLog(strLog);
        return true;
    }

    bool CTSE_PerasoDMGTRXTest::CheckRxResultsToTestStandards(const std::string& strRxResult,int nRssiStandard,int nFerStandard,S_DMG_TRX_RESULT& sDMGTRxResult)
    {
        CCJGW_DMGRxResult dmgRxResult(strRxResult);

        sDMGTRxResult.mnRssi = dmgRxResult.GetRssi();
        sDMGTRxResult.mstrRssi = dmgRxResult.GetRssiString();
        sDMGTRxResult.mnFerPerimile = dmgRxResult.GetFerPerimile();
        sDMGTRxResult.mstrFerPermile = dmgRxResult.GetFerPerimileString();
        sDMGTRxResult.mnUgain = dmgRxResult.GetUgain();
        sDMGTRxResult.mstrUgain = dmgRxResult.GetUgainString();
        std::wstring strLog;

        SendViewTestLogFormat(L"rssi:%s,fer:%s,ugain:%s,PPDU Detects:%s,PPDU Success:%s",dmgRxResult.GetRssiString().c_str(),dmgRxResult.GetFerPerimileString().c_str(),dmgRxResult.GetUgainString().c_str(),dmgRxResult.GetPPDUDetectsString().c_str(),dmgRxResult.GetPPDUSuccessString().c_str());

        if (0 == dmgRxResult.GetPPDUDetects() || mnPPDUDetecs > dmgRxResult.GetPPDUDetects())
        {
            SendViewTestLog(L"Receive PPDU packet exception!!!");
            return false;
        }
        //! check rssi
        //! min testValue,max
        JGW_FormatWString(strLog,L"rssi\t%d\t%d\t%d\t",nRssiStandard, sDMGTRxResult.mnRssi,0);
        if (nRssiStandard > sDMGTRxResult.mnRssi)
        {
            //SendViewTestLogFormat(L"读取RSSI:%d,RSSI测试标准:%d,RSSI测测试失败，测试值必须大于或者等于测试标准",sDMGTRxResult.mnRssi,nRssiStandard);
            strLog += L"FAIL";
            SendViewTestLog(strLog);
            return false;
        }
        strLog += L"PASS";
        SendViewTestLog(strLog);

        JGW_FormatWString(strLog,L"FerPerimile\t%d\t%d\t%d\t",0, sDMGTRxResult.mnFerPerimile,nFerStandard);
        if (sDMGTRxResult.mnFerPerimile < 0 || nFerStandard < sDMGTRxResult.mnFerPerimile)
        {
            //SendViewTestLogFormat(L"读取FER(千分比):%d,FER测试标准:%d,FER测测试失败，测试值必须小于或者等于测试标准",sDMGTRxResult.mnFerPerimile,nFerStandard);
            strLog += L"FAIL";
            SendViewTestLog(strLog);
            return false;
        }
        strLog += L"PASS";
        SendViewTestLog(strLog);
        return true;
    }

    bool CTSE_PerasoDMGTRXTest::StartDMGTRxAutomaticCalibrationConfigurationTest(CCJGW_DonglesComPort& txDonglesComPort,S_DMG_PERASO_INFO& sTxPerasoInfo,CCJGW_DonglesComPort& rxDonglesComPort,S_DMG_PERASO_INFO& sRxPerasoInfo,std::vector<int>& vRssiRange,std::vector<int>& vFerRange,LPS_DMG_TRX_RESULT psDMGTRxLogResults)
    {
        int count = 0;
        std::string strRsp;
        SendViewTestLog(L"Automatic Calibration Configuration");
        SendViewTestLogFormat(L"Tx SerialNumber:%s",sTxPerasoInfo.mstrSerialNumber.c_str());
        EnableTestMode(rxDonglesComPort);
        SendViewTestLogFormat(L"Rx SerialNumber:%s",sRxPerasoInfo.mstrSerialNumber.c_str());
        EnableTestMode(txDonglesComPort);
        int nRssi = 0,nFerPerimile = 0,nRssiCount,nFerPerimileCount,nNumberValidTests = 0;

        for (size_t index = 0;index < mvChannels.size();index ++)
        {
            nNumberValidTests = 0;
            nRssiCount = nFerPerimileCount = 0;
            for (count = 0;count < mnItemTestErrorRetryCount;count ++)
            {
                psDMGTRxLogResults[index].mnChannel = mvChannels[index];
                //SendViewTestLogFormat(L"Ch%dRx:",mvChannels[index]);
                EnableTestMode(rxDonglesComPort);
                ConfigureChannel(rxDonglesComPort,mvChannels[index]);
                ReceiveDMGPackets(rxDonglesComPort,sRxPerasoInfo.mstrSector);

                //SendViewTestLogFormat(L"Ch%dTx:",mvChannels[index]);
                ConfigureChannel(txDonglesComPort,mvChannels[index]);
                TransmitDMGPackets(txDonglesComPort,sTxPerasoInfo.mstrSector.c_str());

                SendViewTestLogFormat(L"Ch%dRx:",mvChannels[index]);
                strRsp = StopRxAndQueryResultsAfterSomeDelay(rxDonglesComPort);

                if (!GetRxResultsToTestStandards(strRsp,vRssiRange[index],vFerRange[index],nRssi,nFerPerimile))
                {
                    SendViewTestLog(L"Get DMG TRX Test Result Fail!!!");
                    return false;
                }
                nNumberValidTests++;
                nRssiCount += nRssi;
                nFerPerimileCount += nFerPerimile;
            }

            if (0 == nNumberValidTests)
            {
                SendViewTestLog(L"Get DMG TRX Test Result Fail!!!");
                return false;
            }
            psDMGTRxLogResults[index].mnRssi = nNumberValidTests > 0 ? nRssiCount /nNumberValidTests - mnRssiTestRange : 0;
            //! 测试次数超过限制失败返回fail
            /*if (count >= mnItemTestErrorRetryCount)
            {
            DisableTestMode(rxDonglesComPort);
            DisableTestMode(txDonglesComPort);
            return false;
            }*/
        }
        DisableTestMode(rxDonglesComPort);
        DisableTestMode(txDonglesComPort);
        return true;
    }

    bool CTSE_PerasoDMGTRXTest::StartDMGTRxTest(CCJGW_DonglesComPort& txDonglesComPort,S_DMG_PERASO_INFO& sTxPerasoInfo,CCJGW_DonglesComPort& rxDonglesComPort,S_DMG_PERASO_INFO& sRxPerasoInfo,std::vector<int>& vRssiRange,std::vector<int>& vFerRange,LPS_DMG_TRX_RESULT psDMGTRxLogResults)
    {
        int count = 0;
        std::string strRsp;
        SendViewTestLogFormat(L"Tx SerialNumber:%s",sTxPerasoInfo.mstrSerialNumber.c_str());
        //EnableTestMode(rxDonglesComPort);
        SendViewTestLogFormat(L"Rx SerialNumber:%s",sRxPerasoInfo.mstrSerialNumber.c_str());
        //EnableTestMode(txDonglesComPort);

        for (size_t index = 0;index < mvChannels.size();index ++)
        {
            for (count = 0;count < mnItemTestErrorRetryCount;count ++)
            {
                psDMGTRxLogResults[index].mnChannel = mvChannels[index];
                //SendViewTestLogFormat(L"Ch%dRx:",mvChannels[index]);
                //EnableTestMode(rxDonglesComPort);
                ConfigureChannel(rxDonglesComPort,mvChannels[index]);
                ReceiveDMGPackets(rxDonglesComPort,sRxPerasoInfo.mstrSector);

                //SendViewTestLogFormat(L"Ch%dTx:",mvChannels[index]);
                ConfigureChannel(txDonglesComPort,mvChannels[index]);
                TransmitDMGPackets(txDonglesComPort,sTxPerasoInfo.mstrSector.c_str());

                SendViewTestLogFormat(L"Ch%dRx:",mvChannels[index]);
                strRsp = StopRxAndQueryResultsAfterSomeDelay(rxDonglesComPort);

                if (CheckRxResultsToTestStandards(strRsp,vRssiRange[index],vFerRange[index],psDMGTRxLogResults[index]))
                {
                    break;
                }
            }
            //! 测试次数超过限制失败返回fail
            if (count >= mnItemTestErrorRetryCount)
            {
                DisableTestMode(rxDonglesComPort);
                DisableTestMode(txDonglesComPort);
                return false;
            }
        }
        DisableTestMode(rxDonglesComPort);
        DisableTestMode(txDonglesComPort);
        return true;
    }

    bool CTSE_PerasoDMGTRXTest::TSE_Run()
    {
        CCJGW_CSerialComPort* guSerialComPort = NULL,*dutSerialComPort = NULL;
        mbTestResult = false;
        //! 获取串口资源实例
        guSerialComPort = (CCJGW_CSerialComPort*)GetGlobalEnvironment()->GetInt(mstrGuSerialComPortEnv);
        dutSerialComPort = (CCJGW_CSerialComPort*)GetGlobalEnvironment()->GetInt(mstrDutSerialComPortEnv);
        if (!mstrPPDUDetectEnv.empty())
        {
            mnPPDUDetecs = GetGlobalEnvironment()->GetInt(mstrPPDUDetectEnv);
        }
        if (NULL == guSerialComPort || NULL == dutSerialComPort)
        {
            LogE(L"获取SerialComPort资源失败");
            return false;
        }
        //! 检查串口打开情况
        if (!guSerialComPort->IsOpenComPort() || !dutSerialComPort->IsOpenComPort())
        {
            LogE(L"串口打开失败");
            return false;
        }
        ///! 初始化测试参数环境变量
        if (!InitTestParamEnvironment())
        {
            return false;
        }

        CCJGW_DonglesComPort guDonglesComPort(*guSerialComPort),dutDonglesComPort(*dutSerialComPort);
        std::wstring strLog;
        //! 读取Golden Unit 相关信息
        guDonglesComPort.ReadSerialNumberAndMacAddress(msDmgTRxLog.msGuDmgPerasoInfo.mstrSerialNumber,msDmgTRxLog.msGuDmgPerasoInfo.mstrMacAddress);
        /*guDonglesComPort.ReadSerialNumber(msDmgTRxLog.msGuDmgPerasoInfo.mstrSerialNumber);
        guDonglesComPort.ReadMacAddress(msDmgTRxLog.msGuDmgPerasoInfo.mstrMacAddress);*/
        guDonglesComPort.ReadFirmwareVersion(msDmgTRxLog.msGuDmgPerasoInfo.mstrFirewareVersion);
        guDonglesComPort.ReadConfigPdbVersion(msDmgTRxLog.msGuDmgPerasoInfo.mstrConfigPdbVersion);
        /*JGW_FormatWString(strLog,L"Golden Unit SerialNumber:%s\r\n \
        Golden Unit Mac:%s\r\n \
        Golden Unit FWVersion:%s\r\n \
        Golden Unit Sector:%s\r\n \
        Golden Unit Config PDB Versin:%s"
        ,msDmgTRxLog.msGuDmgPerasoInfo.mstrSerialNumber.c_str()
        ,msDmgTRxLog.msGuDmgPerasoInfo.mstrMacAddress.c_str()
        ,msDmgTRxLog.msGuDmgPerasoInfo.mstrFirewareVersion.c_str()
        ,msDmgTRxLog.msGuDmgPerasoInfo.mstrSector.c_str()
        ,msDmgTRxLog.msGuDmgPerasoInfo.mstrConfigPdbVersion.c_str());
        SendViewTestLog(strLog);*/
        //! 读取Dut 相关信息
        dutDonglesComPort.ReadSerialNumberAndMacAddress(msDmgTRxLog.msDutDmgPerasoInfo.mstrSerialNumber,msDmgTRxLog.msDutDmgPerasoInfo.mstrMacAddress);
        /*dutDonglesComPort.ReadSerialNumber(msDmgTRxLog.msDutDmgPerasoInfo.mstrSerialNumber);
        dutDonglesComPort.ReadMacAddress(msDmgTRxLog.msDutDmgPerasoInfo.mstrMacAddress);*/
        dutDonglesComPort.ReadFirmwareVersion(msDmgTRxLog.msDutDmgPerasoInfo.mstrFirewareVersion);
        dutDonglesComPort.ReadConfigPdbVersion(msDmgTRxLog.msDutDmgPerasoInfo.mstrConfigPdbVersion);
        //StopTxRxTest(dutDonglesComPort);
        //StopTxRxTest(guDonglesComPort);
        if (!EnableTestMode(dutDonglesComPort) || !EnableTestMode(guDonglesComPort))
        {
            return false;
        }
       
        /*JGW_FormatWString(strLog,L"Dut SerialNumber:%s\r\n \
        Dut Mac:%s\r\n \
        Dut FWVersion:%s\r\n \
        Dut Sector:%s\r\n \
        Dut Config PDB Versin:%s"
        ,msDmgTRxLog.msDutDmgPerasoInfo.mstrSerialNumber.c_str()
        ,msDmgTRxLog.msDutDmgPerasoInfo.mstrMacAddress.c_str()
        ,msDmgTRxLog.msDutDmgPerasoInfo.mstrFirewareVersion.c_str()
        ,msDmgTRxLog.msDutDmgPerasoInfo.mstrSector.c_str()
        ,msDmgTRxLog.msDutDmgPerasoInfo.mstrConfigPdbVersion.c_str());
        SendViewTestLog(strLog);*/

        //! DUT TX -> GU RX
        if (!mvRssiRxGuTxDut.empty() && !mvFerRxGuTxDut.empty())
        {
            SendViewTestLog(L"===========================");
            SendViewTestLog(L"DUT TX => GU RX");
            SendViewTestLog(L"===========================");
            if (mbAutomaticCalibrationConfigurationTest)
            {
                if (!StartDMGTRxAutomaticCalibrationConfigurationTest(dutDonglesComPort,msDmgTRxLog.msDutDmgPerasoInfo,guDonglesComPort,msDmgTRxLog.msGuDmgPerasoInfo,mvRssiRxGuTxDut,mvFerRxGuTxDut,msDmgTRxLog.mvsDutTxGuRxResults))
                {
                    return false;
                }
            }
            else
            {
                if (!StartDMGTRxTest(dutDonglesComPort,msDmgTRxLog.msDutDmgPerasoInfo,guDonglesComPort,msDmgTRxLog.msGuDmgPerasoInfo,mvRssiRxGuTxDut,mvFerRxGuTxDut,msDmgTRxLog.mvsDutTxGuRxResults))
                {
                    return false;
                }
            }  
        }
        //! DUT RX -> GU TX
        if (!mvRssiTxGuRxDut.empty() && !mvFerTxGuRxDut.empty())
        {
            SendViewTestLog(L"===========================");
            SendViewTestLog(L"DUT RX => GU TX");
            SendViewTestLog(L"===========================");
            if (mbAutomaticCalibrationConfigurationTest)
            {
                if (!StartDMGTRxAutomaticCalibrationConfigurationTest(guDonglesComPort,msDmgTRxLog.msGuDmgPerasoInfo,dutDonglesComPort,msDmgTRxLog.msDutDmgPerasoInfo,mvRssiTxGuRxDut,mvFerTxGuRxDut,msDmgTRxLog.mvsGuTxDutRxResults))
                {
                    return false;
                }
            }
            else
            {
                if (!StartDMGTRxTest(guDonglesComPort,msDmgTRxLog.msGuDmgPerasoInfo,dutDonglesComPort,msDmgTRxLog.msDutDmgPerasoInfo,mvRssiTxGuRxDut,mvFerTxGuRxDut,msDmgTRxLog.mvsGuTxDutRxResults))
                {
                    return false;
                }
            } 
        }

        return (mbTestResult = true);
    }

    bool CTSE_PerasoDMGTRXTest::TSE_Exit()
    {
        if (mbAutomaticCalibrationConfigurationTest)
        {
            std::vector<S_TSE_BASE_ITEM> vTSEBaseItems;
            std::wstring strTestingStandard ;
            S_TSE_BASE_ITEM sTestBaseItem;
            //! GU RX DUT TX
            for (size_t channel = 1;channel <= PERASO_MAX_CHANNEL && channel <= mvChannels.size();channel ++)
            {
                if (1 == channel)
                {
                    strTestingStandard = JGW_GetFormatWString(L"%d",msDmgTRxLog.mvsDutTxGuRxResults[channel - 1].mnRssi);
                }
                else
                {
                    strTestingStandard += JGW_GetFormatWString(L",%d",msDmgTRxLog.mvsDutTxGuRxResults[channel - 1].mnRssi);
                }  
            }
            GetGlobalEnvironment()->PutString(mstrRssiRxGuTxDutEnv,strTestingStandard);
            sTestBaseItem.mstrValueEnvironment = mstrRssiRxGuTxDutEnv;
            vTSEBaseItems.push_back(sTestBaseItem);
            SendViewTestLogFormat(L"Rssi(DutTx-GuRx):%s",strTestingStandard);

            //! GU TX DUT RX
            for (size_t channel = 1;channel <= PERASO_MAX_CHANNEL && channel <= mvChannels.size();channel ++)
            {
                if (1 == channel)
                {
                    strTestingStandard = JGW_GetFormatWString(L"%d",msDmgTRxLog.mvsGuTxDutRxResults[channel - 1].mnRssi);
                }
                else
                {
                    strTestingStandard += JGW_GetFormatWString(L",%d",msDmgTRxLog.mvsGuTxDutRxResults[channel - 1].mnRssi);
                }  
            }
            GetGlobalEnvironment()->PutString(mstrRssiTxGuRxDutEnv,strTestingStandard);
            sTestBaseItem.mstrValueEnvironment = mstrRssiTxGuRxDutEnv;
            vTSEBaseItems.push_back(sTestBaseItem);
            SendViewTestLogFormat(L"Rssi(DutRx-GuTx):%s",mstrRssiTxGuRxDutEnv.c_str());

            CTSE_BaseConfigXML baseConfigXml(vTSEBaseItems);
            if (mstrGoldenXmlConfigPath.empty())
            {
                baseConfigXml.SaveLanSuiteConfig(GetGlobalEnvironment()->GetAnsiString(TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH));
            }
            else
            {
                baseConfigXml.SaveLanSuiteConfig(JGW_W2A(mstrGoldenXmlConfigPath));
            }

            return true;
        }

        //! dmg csv文件不存在，则创建表头
        bool bDmgLogFileExists = JGW_FileExistsToFilePath(mstrDMGCsvLogFilePath.c_str());

        std::wofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(mstrDMGCsvLogFilePath.c_str(),std::ios_base::app);
        if (!fileStream.is_open())
        {
            SendViewTestLogFormat(L"Warning:Open DMG CSV(%s - %s) Log File Faill",mstrDMGCsvLogFilePath.c_str(),JGW_GetSystemErrorString(GetLastError()).c_str());
            return true;
        }
        //! => 创建表头
        if (!bDmgLogFileExists)
        {
            std::wstring strHeader(L"GUFwVersion,GUSN,GUMac,GUSector,GUConfigPdbVersion,DutFwVersion,DutSN,DutMac,DutSector,DutConfigPdbVersion,TestDateTime,TestStatus");
            for (int channel = 1;channel <= PERASO_MAX_CHANNEL;channel ++)
            {
                strHeader += JGW_GetFormatWString(L",DutTxGuRxCh%dRssi,DutTxGuRxCh%dFer,DutTxGuRxCh%dUgain",channel,channel,channel);
            }
            for (int channel = 1;channel <= PERASO_MAX_CHANNEL;channel ++)
            {
                strHeader += JGW_GetFormatWString(L",DutRxGuTxCh%dRssi,DutRxGuTxCh%dFer,DutRxGuTxCh%dUgain",channel,channel,channel);
            }
            fileStream << strHeader << std::endl;
        }
        std::wstring strContent;
        JGW_FormatWString(strContent,L"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,T%s,%s",msDmgTRxLog.msGuDmgPerasoInfo.mstrFirewareVersion.c_str()
            ,msDmgTRxLog.msGuDmgPerasoInfo.mstrSerialNumber.c_str()
            ,msDmgTRxLog.msGuDmgPerasoInfo.mstrMacAddress.c_str()
            ,msDmgTRxLog.msGuDmgPerasoInfo.mstrSector.c_str()
            ,msDmgTRxLog.msGuDmgPerasoInfo.mstrConfigPdbVersion.c_str()
            ,msDmgTRxLog.msDutDmgPerasoInfo.mstrFirewareVersion.c_str()
            ,msDmgTRxLog.msDutDmgPerasoInfo.mstrSerialNumber.c_str()
            ,msDmgTRxLog.msDutDmgPerasoInfo.mstrMacAddress.c_str()
            ,msDmgTRxLog.msDutDmgPerasoInfo.mstrSector.c_str()
            ,msDmgTRxLog.msDutDmgPerasoInfo.mstrConfigPdbVersion.c_str()
            ,JGW_GetCurrentTimeStampYMDHMS().c_str()
            ,mbTestResult?L"PASS":L"FAIL");
        for (int channel = 1;channel <= PERASO_MAX_CHANNEL;channel ++)
        {
            strContent += JGW_GetFormatWString(L",%s,%s,%s",msDmgTRxLog.mvsDutTxGuRxResults[channel - 1].mstrRssi.c_str()
                ,msDmgTRxLog.mvsDutTxGuRxResults[channel - 1].mstrFerPermile.c_str()
                ,msDmgTRxLog.mvsDutTxGuRxResults[channel - 1].mstrUgain.c_str());
        }
        for (int channel = 1;channel <= PERASO_MAX_CHANNEL;channel ++)
        {
            strContent += JGW_GetFormatWString(L",%s,%s,%s",msDmgTRxLog.mvsGuTxDutRxResults[channel - 1].mstrRssi.c_str()
                ,msDmgTRxLog.mvsGuTxDutRxResults[channel - 1].mstrFerPermile.c_str()
                ,msDmgTRxLog.mvsGuTxDutRxResults[channel - 1].mstrUgain.c_str());
        }
        fileStream << strContent << std::endl;;
        fileStream.close();
        return true;
    }

    void CTSE_PerasoDMGTRXTest::SendViewTestLog(const std::wstring& strLog)
    {
        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strLog.c_str());
    }

    void CTSE_PerasoDMGTRXTest::SendViewTestLogFormat(const wchar_t* strCustomLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strCustomLog);
        SendViewTestLog(strLog.c_str());
    }
}

