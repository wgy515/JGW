#include "StdAfx.h"
#include "TSE_ThroughputConfig.h"
#include "CJGW_ThroughTestConfigDlg.h"
#include "CJGW_ThroughputTestConfigXml.h"
namespace JGW
{
    CTSE_ThroughputConfig::CTSE_ThroughputConfig(void)
    {
        msThroughputConfig.mstrTestTimeEnvironment = L"TSE_THROGHPUT_TEST_TIME";
        msThroughputConfig.mstrWiFiScanIPAddressEnvironment = L"TSE_HOST_IP_NUMBER";
        msThroughputConfig.mstrIperfServerIPAddressEnvironment = L"TSE_IPERF_IP";
        msThroughputConfig.mstrTestRangeEnvironment = L"TSE_THROGHPUT_TEST_RANGE";
        msThroughputConfig.mstrIperfCommandEnvironment = L"TSE_IPERF_COMMAND";
        msThroughputConfig.mstrIperfCommandTimeOutEnvironment = L"TSE_THROGHPUT_TEST_TIME_OUT";
    }


    CTSE_ThroughputConfig::~CTSE_ThroughputConfig(void)
    {
    }

    const wchar_t* CTSE_ThroughputConfig::TSE_GetParamDescription()
    {
        return L"{\"RealName\":\"CTSE_ThroughputConfig\",\"TestName\":\"吞吐率参数配置\",\"TestTimeEnvironment\":\"吞吐率测试时间配置(单位:秒)\",\"HostIPEnvironment\":\"JGW_WlanServiceApplication运行主机的地址保存的环境变量\",\"IperfIPEnvironment\":\"iperf IP地址\",\"TestRangeEnvironment\":\"iperf大于等于测试范围(Mbits/sec)\",\"IperfCommandEnvironment\":\"iperf测试命令\",\"IperfCommandTimeOutEnvironment\":\"iperf测试命令运行后的超时时间\"}";
    }

    bool CTSE_ThroughputConfig::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestTimeEnvironment"))
        {
            msThroughputConfig.mstrTestTimeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostIPEnvironment"))
        {
            msThroughputConfig.mstrWiFiScanIPAddressEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfIPEnvironment"))
        {
            msThroughputConfig.mstrIperfServerIPAddressEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestRangeEnvironment"))
        {
            msThroughputConfig.mstrTestRangeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfCommandEnvironment"))
        {
            msThroughputConfig.mstrIperfCommandEnvironment =  strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfCommandTimeOutEnvironment"))
        {
            msThroughputConfig.mstrIperfCommandTimeOutEnvironment =  strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ThroughputConfig::TSE_Run()
    {
        msThroughputConfig.mstrIperfServerIPAddress = GetGlobalEnvironment()->GetString(msThroughputConfig.mstrIperfServerIPAddressEnvironment);
        msThroughputConfig.mstrTestRange = GetGlobalEnvironment()->GetString(msThroughputConfig.mstrTestRangeEnvironment);
        msThroughputConfig.mstrTestTime = GetGlobalEnvironment()->GetString(msThroughputConfig.mstrTestTimeEnvironment);
        msThroughputConfig.mstrWiFiScanIPAddress = GetGlobalEnvironment()->GetString(msThroughputConfig.mstrWiFiScanIPAddressEnvironment);
        msThroughputConfig.mstrIperfCommand = GetGlobalEnvironment()->GetString(msThroughputConfig.mstrIperfCommandEnvironment);

        CCJGW_ThroughTestConfigDlg dlg(GetGlobalMainHwnd(),msThroughputConfig);
        if (IDOK == dlg.DoModal())
        {
            GetGlobalEnvironment()->PutString(msThroughputConfig.mstrIperfServerIPAddressEnvironment,msThroughputConfig.mstrIperfServerIPAddress);
            GetGlobalEnvironment()->PutString(msThroughputConfig.mstrTestRangeEnvironment,msThroughputConfig.mstrTestRange);
            GetGlobalEnvironment()->PutString(msThroughputConfig.mstrTestTimeEnvironment,msThroughputConfig.mstrTestTime);
            GetGlobalEnvironment()->PutString(msThroughputConfig.mstrWiFiScanIPAddressEnvironment,msThroughputConfig.mstrWiFiScanIPAddress);
            GetGlobalEnvironment()->PutString(msThroughputConfig.mstrIperfCommandEnvironment,msThroughputConfig.mstrIperfCommand);
            CCJGW_ThroughputTestConfigXml xml;
            xml.SaveThroughputTestConfig(JGW_W2A(GetGlobalEnvironment()->GetString(TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH)),msThroughputConfig);

        }
        return true;
    }
}

