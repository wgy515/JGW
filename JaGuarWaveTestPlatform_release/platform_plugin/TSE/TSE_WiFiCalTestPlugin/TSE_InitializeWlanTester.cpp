#include "StdAfx.h"
#include "TSE_InitializeWlanTester.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_WlanInstrumentConfig.h"
#include "JGW_IPQ40xxGuTestData.h"

namespace JGW
{
    CJGW_IWlanInstrumentInterface* gpWlanInstrumentInterface = NULL;

    CJGW_IWlanInstrumentInterface* GetWlanInstrumentInterface()
    {
        if (NULL == gpWlanInstrumentInterface)
        {
            throw L"UnInitialize Wlan Tester(CTSE_InitializeWlanTester)";
        }
        return gpWlanInstrumentInterface;
    }

    CCJGW_WlanConfigFile* gpWlanConfigFile = NULL;

    CCJGW_WlanConfigFile* GetWlanConfigFile()
    {
        if (NULL == gpWlanConfigFile)
        {
            throw L"UnInitialize Wlan Config(CTSE_InitializeWlanTester)";
        }
        return gpWlanConfigFile;
    }


    CTSE_InitializeWlanTester::CTSE_InitializeWlanTester(void) : mstrTxStationCalPathName(L"BH0_LP,BH0_LP,BH0_LP,BH0_LP"),mstrRxStationCalPathName(L"BH0_LP,BH0_LP,BH0_LP,BH0_LP"),mstrTRxChainPort(L"3,3,3,3")
    {
        gpWlanInstrumentInterface = &mWCNTesterCommonSCPIWLAN;
        gpWlanConfigFile = &mWlanConfigFile;
    }


    CTSE_InitializeWlanTester::~CTSE_InitializeWlanTester(void)
    {
        gpWlanInstrumentInterface = NULL;
        gpWlanConfigFile = NULL;
    }

    const wchar_t* CTSE_InitializeWlanTester::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_InitializeWlanTester\",\
                \"TestName\": \"Initialize Wlan Tester\",\
                \"StationCalFileName\": \"Station Cal File name including path\",\
                \"TesterConfigFileName\": \"WLAN Tester Config File name including path(instrument)\",\
                \"TxStationCalPathName\": \"TX station cal path name\",\
                \"RxStationCalPathName\": \"RX station cal path name\",\
                \"WlanConfigFileName\": \"wlan config file name\",\
                \"TRxChainPort\": \"3,3,3,3,RF Port Index\",\
                \"TRxChainPortEnv\": \"RF Port Index Environment\"\
                }";
    }

    bool CTSE_InitializeWlanTester::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"StationCalFileName",strParamName))
        {
            mstrStationCalFileName = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TesterConfigFileName",strParamName))
        {
            mstrTesterConfigFileName = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanConfigFileName",strParamName))
        {
            mstrWlanConfigFileName = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxStationCalPathName",strParamName))
        {
            mstrTxStationCalPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RxStationCalPathName",strParamName))
        {
            mstrRxStationCalPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TRxChainPort",strParamName))
        {
            mstrTRxChainPort = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TRxChainPortEnv",strParamName))
        {
            mstrTRxChainPortEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_InitializeWlanTester::TSE_Init()
    {
         mWlanConfigFile.SetWlanConfigFilePath(mstrWlanConfigFileName);
         return true;
    }

    bool CTSE_InitializeWlanTester::TSE_Run()
    {
        if (!JGW_PathFileExists(mstrStationCalFileName.c_str()))
        {
            LogE_F(L"InitializeWlanTester() failed: could not find stationCalFileName %s",mstrStationCalFileName.c_str());
            return false;
        }
        if (!JGW_PathFileExists(mstrTesterConfigFileName.c_str()))
        {
            LogE_F(L"InitializeWlanTester() failed: could not find testerConfigFileName %s",mstrTesterConfigFileName.c_str());
            return false;
        }

       /* if (!JGW_PathFileExists(mstrWlanConfigFileName.c_str()))
        {
            LogE_F(L"InitializeWlanTester() failed: could not find wlanConfigFileName %s",mstrWlanConfigFileName.c_str());
            return false;
        }*/
        std::wstring strTRxChainPort (mstrTRxChainPort);
        if (!mstrTRxChainPortEnv.empty())
        {
            strTRxChainPort = GetGlobalEnvironment()->GetString(mstrTRxChainPortEnv);
        }

        if (!mWlanInstrumentConfig.LoadWlanInstrumentConfig(mstrTesterConfigFileName,strTRxChainPort))
        {
            LogE_F(L"InitializeWlanTester() failed: could not load testerConfigFileName %s",mstrTesterConfigFileName.c_str());
            return false;
        }

        if (!mWlanInstrumentConfig.LoadWlanLossConfig(mstrStationCalFileName,mstrTxStationCalPathName,mstrRxStationCalPathName))
        {
            LogE_F(L"InitializeWlanTester() failed: could not load stationCalFileName %s",mstrStationCalFileName.c_str());
            return false;
        }

        CJGW_IPQ40xxGuTestData::GetInstance()->ClearIPQ40xxGuTestData();
        CJGW_IPQ40xxGuTestData::GetInstance()->SetTxStationCalPathName(mstrTxStationCalPathName);
        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Connect Wlan Instrument");
        if (!GETINSTRUMENT->ConnectWlanInstrument(mWlanInstrumentConfig.GetWlanMonitorInfo()))
        {
            LogE(L"Connect Wlan Instrument failed");
            return false;
        }

        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Init Wlan Instrument");
        if (!GETINSTRUMENT->InitWlanInstrument(strTRxChainPort,mWlanInstrumentConfig.GetTxStationCalPath(),mWlanInstrumentConfig.GetRxStationCalPath()))
        {
            LogE(L"Init Wlan Instrument failed");
            return false;
        }

        return true;
    }

}
