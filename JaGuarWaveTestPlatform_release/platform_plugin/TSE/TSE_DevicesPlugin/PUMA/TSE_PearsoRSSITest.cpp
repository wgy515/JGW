#include "StdAfx.h"
#include <numeric>
#include "TSE_PearsoRSSITest.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <fstream>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    std::wstring CTSE_PearsoRSSITest::mstrPerasoTestLogPath;
    CTSE_PearsoRSSITest::CTSE_PearsoRSSITest(void) : mbSaveLog(true),mnTestCount(10),mnMinRSSI(-60),
        mnMaxRSSI(-50),mbInitLogFile(false)
    {
    }


    CTSE_PearsoRSSITest::~CTSE_PearsoRSSITest(void)
    {
    }

    const wchar_t* CTSE_PearsoRSSITest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PearsoRSSITest\",\
                \"TestName\": \"测试RSSI(Peraso)\",\
                \"RSSICommand\": \"测试RSSI命令\",\
                \"Sector\": \"测试前提条件写入到Log中\",\
                \"TestCount\": \"测试次数\",\
                \"SerialNumberEnvironment\": \"SN号段环境变量，保存测试LOG路径\",\
                \"TestStandard\": \"测试标准以~作为分隔符(表示最低和最高值)\",\
                \"InitLogFile\": \"是否是初次初始化log，初次初始化log则会删除以前的log数据\",\
                \"SaveLog\": \"保存测试log,默认保存测试数据\",\
                \"SaveLogPathEnvironment\": \"当前LOG保存的路径写入到指定环境变量中\"\
                }";
    }

    bool CTSE_PearsoRSSITest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"RSSICommand",strParamName))
        {
            mstrRssiCommandEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Sector",strParamName))
        {
            mstrSectorLog = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TestCount",strParamName))
        {
            mnTestCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SerialNumberEnvironment",strParamName))
        {
            mstrSerialNumberEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TestStandard",strParamName))
        {
            mstrTestStandard = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveLog",strParamName))
        {
            mbSaveLog = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"InitLogFile",strParamName))
        {
            mbInitLogFile = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveLogPathEnvironment",strParamName))
        {
            mstrSaveLogPathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    void CTSE_PearsoRSSITest::BuildTestStandard()
    {
        std::wstring strTemp(mstrTestStandard),strValue;
        GetEnvironmentCommandString(strTemp);
        mnMinRSSI = _ttoi(strTemp.c_str());

        if (std::wstring::npos == strTemp.find(L"~"))
        {
            mnMaxRSSI = mnMinRSSI;
        }
        else
        {
            size_t i = strTemp.find(L"~");
            strValue = strTemp.substr(i + 1);
            mnMaxRSSI = _ttoi(strValue.c_str());
        }

        //         if (NULL == wcsstr(strParamValue,L"~"))
        //         {
        //             mnMaxRSSI = mnMinRSSI;
        //         }
        //         else
        //         {
        //             mnMaxRSSI = _ttoi(wcsstr(strParamValue,L"~") + 1);
        //         }
    }

    bool CTSE_PearsoRSSITest::GetRSSI(std::vector<int>& vRSSI,int& rssi)
    {
        int min_rssi = 0,max_rssi = -128;
        std::wstring strRssiCommand(mstrRssiCommandEnvironment);

        mvRSSI.clear();
        GetEnvironmentCommandString(strRssiCommand);
        mstrRssiCommand = JGW_W2A(strRssiCommand);

        for (int i = 0;i < mnTestCount;i ++)
        {
            if (!CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->ReadRssiToCommand(mstrRssiCommand,rssi))
            {
                LogE("获取RSSI失败");
                return false;
            }
            LogI_F(L"rssi%d : %d",i,rssi);
            vRSSI.push_back(rssi);
            if (min_rssi > rssi) min_rssi = rssi;
            if (max_rssi < rssi) max_rssi = rssi;
            Sleep(mnIntervalTimeMS);
        }
        rssi = CalcRSSIAverage(vRSSI,max_rssi,min_rssi);
        return true;
    }

    bool CTSE_PearsoRSSITest::TSE_Run()
    {
        BuildTestStandard();
        if (!GetRSSI(mvRSSI,mnTestRSSI)) return false;

        if (mnTestRSSI < mnMinRSSI || mnTestRSSI > mnMaxRSSI)
        {
            LogE_F(L"读取: %d ，配置: 最大值: %d,最小值: %d",mnTestRSSI,mnMaxRSSI,mnMinRSSI);
            return false;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"%d",mnTestRSSI);
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
        return true;
    }

    bool CTSE_PearsoRSSITest::TSE_Exit()
    {
        if (!mbSaveLog) return true;
        //! 重新初始化LOG或者当前log文件为空则重新构造
        if (mbInitLogFile || mstrPerasoTestLogPath.empty()) 
        {
            JGW_DeleteFile(mstrPerasoTestLogPath);
            BuildIperfTestLogPath();
        }

        std::wofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(mstrPerasoTestLogPath.c_str(),std::ios_base::out | std::ios_base::app);
        if (!fileStream.is_open()) return false;

        if (mbInitLogFile)
        {
            fileStream << GetGlobalEnvironment()->GetString(mstrSerialNumberEnvironment) << L",";
        }
        fileStream << mstrSectorLog << L",";

        for (size_t i = 0;i < mvRSSI.size();i ++)
        {
            fileStream << mvRSSI[i] << L",";
        }
        fileStream << mnTestRSSI << L",";
        fileStream.flush();
        fileStream.close();
        return true;
    }

    int CTSE_PearsoRSSITest::CalcRSSIAverage(std::vector<int>& vRSSI,const int max_rssi,const int min_rssi)
    {
        int rssi = 0,count = 0;
        for (std::vector<int>::iterator it = vRSSI.begin();
            it != vRSSI.end();
            ++ it)
        {
            count ++;
            rssi += it[0];
        }
        //         int rssi = std::accumulate(vRSSI.begin() , vRSSI.end() , 0) - max_rssi - min_rssi;
        return (rssi - max_rssi - min_rssi) / (count - 2);
    }

    void CTSE_PearsoRSSITest::BuildIperfTestLogPath()
    {
        JGW_FormatWString(mstrPerasoTestLogPath,L"%s\\log\\rssi\\%s\\%s_result_%d.csv",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),GetGlobalEnvironment()->GetString(mstrSerialNumberEnvironment).c_str(),JGW_GetTimeOfDay());
        JGW_CreateDirectory(mstrPerasoTestLogPath.c_str(),false);
        GetGlobalEnvironment()->PutString(mstrSaveLogPathEnvironment,mstrPerasoTestLogPath);
    }
}