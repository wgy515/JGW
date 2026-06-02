// TSE_TestDebug.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CJGW_FtpClient.h"

#include <JGW_QMSLFuncPlugin/jgw_wr_nv_api.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_MES_CPK_LOG_DEFINE.h"
#include "CJGW_MESQSEQCpkLog.h"
#include "CJGW_ParseCPKLogInfo.h"
// 
// #include <poco/ThreadPool.h>
// #include <Poco/Net/FTPClientSession.h>
// #include <Poco/Net/NetException.h>
// #include <Poco/Thread.h>
// #include <Poco/Exception.h>
// #include <Poco/Timespan.h>
// #include <Poco/StreamCopier.h>

using namespace JGW;
std::vector<S_WCDMA_TRX_CPK_INFO> gvWCDMATRxCPKInfo;
std::vector<S_LTE_TRX_CPK_INFO> gvLTETRxCPKInfo;
S_WCDMA_TRX_CPK_INFO gsWCDMATRxCPKInfo;
S_LTE_TRX_CPK_INFO gsLTETRxCPKInfo;

void HandleTestMetricInit(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
{
    //! 判断测试状态
    if (NULL != JGW_WStrCaseWStr(szTestMetricInfo,L"PASS"))
    {
        sTestMetric.meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
    }
    else
    {
        sTestMetric.meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
    }

    sTestMetric.mstrMaxValue = L"";
    sTestMetric.mstrMinValue = L"";
    sTestMetric.mstrTestValue = L"";
}

void HandleTestMetricFormat(const wchar_t* szTestMetricInfo,std::vector<std::wstring>& vMetric)
{
    const wchar_t* pTemp = JGW_WStrCaseWStr(szTestMetricInfo,L",");
    if (NULL == pTemp) return;
    pTemp += 1;
    if (!pTemp || '\0' == pTemp[0]) return ;
    std::wstring strTemp = pTemp;
    JGW_ReplaceStringW(strTemp,L",",L" ");
    JGW_ReplaceStringW(strTemp,L"[",L"");
    JGW_ReplaceStringW(strTemp,L"]",L"");
    JGW_ParserStrW(strTemp.c_str(),L" ",vMetric);
}

void HandleTestWCDMAMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
{
    gsWCDMATRxCPKInfo.mbIsTest = true;
    HandleTestMetricInit(szTestMetricInfo,sTestMetric);
    std::vector<std::wstring> vstrTemp;
    HandleTestMetricFormat(szTestMetricInfo,vstrTemp);

    if (vstrTemp.size() < 3) return ;
    sTestMetric.mstrTestValue = vstrTemp[0];
    sTestMetric.mstrMinValue = vstrTemp[1];
    sTestMetric.mstrMaxValue = vstrTemp[2];
    //n/a
    JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"n/a",L"");
    JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"---",L"");
}

void HandleWCMDATestMetric_SEBER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
{
    gsWCDMATRxCPKInfo.mbIsTest = true;
    HandleTestMetricInit(szTestMetricInfo,sTestMetric);
    std::vector<std::wstring> vstrTemp;
    const wchar_t* szTestMetricInfoTemp = wcsstr(szTestMetricInfo,L"=");
    if (NULL == szTestMetricInfoTemp) return;
    szTestMetricInfoTemp += 1;
    if (!szTestMetricInfoTemp || '\0' == szTestMetricInfoTemp[0]) return ;
    HandleTestMetricFormat(szTestMetricInfoTemp,vstrTemp);
    if (vstrTemp.size() < 4) return ;

    sTestMetric.mstrTestValue = vstrTemp[1];
    sTestMetric.mstrMinValue = vstrTemp[2];
    sTestMetric.mstrMaxValue = vstrTemp[3];
}

void HandleMeasWCDMA(const wchar_t* szMeasWCDMA)
{
    const wchar_t* pPos = NULL;
    if (NULL != (pPos = wcsstr(szMeasWCDMA,L"BAND_CHANNEL")))
    {
        if (E_NOT_CPK_TEST_STATUS != gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_TX_POWER].meCPKTestStatus)
        {
            if (_ttoi(gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_TX_POWER].mstrMinValue.c_str()) < 20) gsWCDMATRxCPKInfo.mbIsTest = false;
        }
        if (gsWCDMATRxCPKInfo.mbIsTest) gvWCDMATRxCPKInfo.push_back(gsWCDMATRxCPKInfo);
        //!
        gsWCDMATRxCPKInfo.Init();
        const wchar_t* pTemp = wcsstr(szMeasWCDMA,L"=");
        pTemp += 1;
        gsWCDMATRxCPKInfo.mnBand = _ttoi(pTemp);
        pTemp = wcsstr(pTemp,L",");
        pTemp += 1;
        gsWCDMATRxCPKInfo.mnChannel = _ttoi(pTemp);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"UE_Power")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_TX_POWER]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"EVM")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_EVM_RMS]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"Mag_Error")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_MAG_ERROR_RMS]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"Phase_Error")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_PHASE_ERROR_RMS]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"Freq_Error")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_FREQ_ERROR_RMS]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[-10]")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_MINUS10]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[-05]")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_MINUS5]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[+05]")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_PLUS5]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[+10]")))
    {
        HandleTestWCDMAMetric(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_PLUS10]);
    }
    else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"SE_BER")))
    {
        HandleWCMDATestMetric_SEBER(pPos,gsWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_SEBER]);
    }
}

void HandleLTETestMetric_EVM(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
{
    gsLTETRxCPKInfo.mbIsTest = true;
    HandleTestMetricInit(szTestMetricInfo,sTestMetric);
    std::vector<std::wstring> vstrTemp;
    HandleTestMetricFormat(szTestMetricInfo,vstrTemp);

    if (vstrTemp.size() < 4) return ;
    sTestMetric.mstrTestValue = vstrTemp[0];
    sTestMetric.mstrMinValue = vstrTemp[2];
    sTestMetric.mstrMaxValue = vstrTemp[3];
    //n/a
    JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"n/a",L"");
    JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"---",L"");
}

void HandleTestLTEMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
{
    gsLTETRxCPKInfo.mbIsTest = true;
    HandleTestMetricInit(szTestMetricInfo,sTestMetric);
    std::vector<std::wstring> vstrTemp;
    HandleTestMetricFormat(szTestMetricInfo,vstrTemp);

    if (vstrTemp.size() < 3) return ;
    sTestMetric.mstrTestValue = vstrTemp[0];
    sTestMetric.mstrMinValue = vstrTemp[1];
    sTestMetric.mstrMaxValue = vstrTemp[2];
    //n/a
    JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"n/a",L"");
    JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"---",L"");
}

void HandleTestMetricFormat_Float(const wchar_t* szTestMetricInfo,std::vector<float>& vMetric)
{
    const wchar_t* pTemp = JGW_WStrCaseWStr(szTestMetricInfo,L",");
    if (NULL == pTemp) return;
    pTemp += 1;
    if (!pTemp || '\0' == pTemp[0]) return ;
    std::wstring strTemp = pTemp;
    JGW_ReplaceStringW(strTemp,L",",L" ");
    JGW_ReplaceStringW(strTemp,L"[",L"");
    JGW_ReplaceStringW(strTemp,L"]",L"");
    JGW_ParserFloatW(strTemp.c_str(),L" ",vMetric);
}

void HandleLTETestMetric_SEM(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
{
    gsLTETRxCPKInfo.mbIsTest = true;

    //! OK,-26.66,-19.74,-16.67,-27.62,-28.35,-16.33,-20.18,-26.36
    std::vector<float> vfTemp;
    HandleTestMetricFormat_Float(szTestMetricInfo,vfTemp);
    sTestMetric.mstrMinValue = L"1";
    sTestMetric.mstrMaxValue = L"1";

    if (vfTemp[0] < -23.50 
        && vfTemp[1] < -11.50
        && vfTemp[2] < -8.50
        && vfTemp[3] < -16.50
        && vfTemp[4] < -16.50
        && vfTemp[5] < -8.50
        && vfTemp[6] < -11.50
        && vfTemp[7] < -23.50)
    {
        sTestMetric.meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
        sTestMetric.mstrTestValue = L"1";
    }
    else
    {
        sTestMetric.meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
        sTestMetric.mstrTestValue = L"0";
    }
}
//! -2 -1 1 2  //! OK,39.96,37.36,32.68,32.81,37.24,39.98
void HandleTestLTEMetric_ACLR(const wchar_t* szTestMetricInfo)
{
    gsLTETRxCPKInfo.mbIsTest = true;
    std::vector<float> vfTemp;
    std::vector<std::wstring> vstrTemp;
    HandleTestMetricFormat(szTestMetricInfo,vstrTemp);
    HandleTestMetricFormat_Float(szTestMetricInfo,vfTemp);
    if (vfTemp.size() < 6) return ;
    //! Offset  -2
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].mstrMinValue = L"35.20";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].mstrMaxValue = L"";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].mstrTestValue = vstrTemp[0];
    if (vfTemp[0] > 35.20)
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
    }
    else
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
    }
    //! Offset -1
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].mstrMinValue = L"32.20";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].mstrMaxValue = L"";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].mstrTestValue = vstrTemp[1];
    if (vfTemp[1] > 32.20)
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
    }
    else
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
    }
    //! Offset  1
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].mstrMinValue = L"32.20";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].mstrMaxValue = L"";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].mstrTestValue = vstrTemp[4];
    if (vfTemp[4] > 32.20)
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
    }
    else
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
    }
    //! Offset 2
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].mstrMinValue = L"35.20";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].mstrMaxValue = L"";
    gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].mstrTestValue = vstrTemp[5];
    if (vfTemp[5] > 35.20)
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
    }
    else
    {
        gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
    }
}

void HandleTestLTEMetric_BLER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
{
    gsLTETRxCPKInfo.mbIsTest = true;
    HandleTestMetricInit(szTestMetricInfo,sTestMetric);
    std::vector<std::wstring> vstrTemp;
    const wchar_t* szTestMetricInfoTemp = wcsstr(szTestMetricInfo,L"=");
    if (NULL == szTestMetricInfoTemp) return;
    szTestMetricInfoTemp += 1;
    if (!szTestMetricInfoTemp || '\0' == szTestMetricInfoTemp[0]) return ;

    std::wstring strTemp = szTestMetricInfoTemp;
    JGW_ReplaceStringW(strTemp,L",",L" ");
    JGW_ReplaceStringW(strTemp,L"[",L"");
    JGW_ReplaceStringW(strTemp,L"]",L"");
    JGW_ParserStrW(strTemp.c_str(),L" ",vstrTemp);

    if (vstrTemp.size() < 3) return ;

    sTestMetric.mstrTestValue = vstrTemp[0];
    sTestMetric.mstrMinValue = vstrTemp[1];
    sTestMetric.mstrMaxValue = vstrTemp[2];
}

void HandleMeasLTE(const wchar_t* szMeasLTE)
{
    const wchar_t* pPos = NULL;
    if (NULL != (pPos = wcsstr(szMeasLTE,L"BAND_CHANNEL")))
    {
        //! 确认是否测试TX POWER
        if (E_NOT_CPK_TEST_STATUS != gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_TX_POWER].meCPKTestStatus)
        {
            if (_ttoi(gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_TX_POWER].mstrMinValue.c_str()) < 18) gsLTETRxCPKInfo.mbIsTest = false;
        }
        if (gsLTETRxCPKInfo.mbIsTest) gvLTETRxCPKInfo.push_back(gsLTETRxCPKInfo);
        //!
        gsLTETRxCPKInfo.Init();
        const wchar_t* pTemp = wcsstr(pPos,L"=");
        if (!pTemp) return ;
        pTemp += 1;
        std::vector<std::wstring> vstrTemp;
        JGW_ParserStrW(pTemp,L",",vstrTemp);
        if (vstrTemp.size() < 5) return ;
        gsLTETRxCPKInfo.mnBand = _ttoi(vstrTemp[0].c_str());
        gsLTETRxCPKInfo.mnChannel = _ttoi(vstrTemp[1].c_str());
        gsLTETRxCPKInfo.mnStartRB = _ttoi(vstrTemp[2].c_str());
        gsLTETRxCPKInfo.mnNumRB = _ttoi(vstrTemp[3].c_str());
        gsLTETRxCPKInfo.mstrMCS = vstrTemp[4].c_str();
        if (vstrTemp.size() > 5) gsLTETRxCPKInfo.mnSelectedRxChain = _ttoi(vstrTemp[5].c_str()) - 1;
    }
    else if (NULL != (pPos = wcsstr(szMeasLTE,L"UL_Power(avg)")))
    {
        HandleTestLTEMetric(pPos,gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_TX_POWER]);
    }
    else if (NULL != (pPos = wcsstr(szMeasLTE,L"FreqErr (ppm) average")))
    {
        HandleTestLTEMetric(pPos,gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_FREQ_ERROR]);
    }
    else if (NULL != (pPos = wcsstr(szMeasLTE,L"EVM ")))
    {
        HandleLTETestMetric_EVM(pPos,gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_EVM_POWER]);
    }
    else if (NULL != (pPos = wcsstr(szMeasLTE,L"SEM")))
    {
        HandleLTETestMetric_SEM(pPos,gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_SEM]);
    }
    else if (NULL != (pPos = wcsstr(szMeasLTE,L"ACLR")))
    {
        HandleTestLTEMetric_ACLR(pPos);
    }
    else if (NULL != (pPos = wcsstr(szMeasLTE,L"Carrier[0]_BLER")))
    {
        HandleTestLTEMetric_BLER(pPos,gsLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_BLER]);
    }
}

void HandleMeasureEQP(const wchar_t* szMeasEQP)
{
    //! 处理WCDMA CPK数据解析
    if (NULL != wcsstr(szMeasEQP,L"WCDMA"))
    {
        HandleMeasWCDMA(szMeasEQP);
    }
    //! 处理LTE CPK数据解析
    else if (NULL != wcsstr(szMeasEQP,L"LTE"))
    {
        HandleMeasLTE(szMeasEQP);
    }
}

void HandleMeasureMOB(const wchar_t* szMeasMOB)
{

}

int _tmain(int argc, _TCHAR* argv[])
{
#if 0
/*    std::wifstream qseqFile("C:\\Qualcomm\\Log\\C1889-2018_09_15-QSEQ_LYNX_L_TEST\\UserDefine--2018_09_15_03_46_12_320-W-_Log_ConsoleApp.txt");*/
/*    std::wifstream qseqFile("C:\\Qualcomm\\Log\\C1889-2018_09_15-QSEQ_LYNX_L_TEST\\UserDefine--2018_09_15_03_39_20_009-L-_Log_ConsoleApp.txt");*/
    std::wifstream qseqFile("C:\\JW\\exec\\TestLog\\CAL_L2018-09-20\\LE241837000001\\P_UserDefine-LE241837000001-2018_09_20_06_38_16_202-LTW-_Log_ConsoleApp.txt");
    
    if (!qseqFile.is_open()) return -1;
    wchar_t szFileBuf[1024] = {0};
    gsWCDMATRxCPKInfo.Init();

    CCJGW_MESQSEQCpkLog mesQSEQCpkLog;
    CCJGW_ParseCPKLogInfo mParseCPKLogInfo;

    mesQSEQCpkLog.BuildTestStation(L"DHW180037",L"083",L"jdmesuser",L"LE241837000000",true);
    mesQSEQCpkLog.BuildLotinfo(L"LE241837000000",12);
    while (qseqFile.getline(szFileBuf,1024))
    {
        mParseCPKLogInfo.StartHandleQSEQTestInfo(szFileBuf);
    }
    //! 确认相关参数指标都已经加入OK
    mParseCPKLogInfo.EndHandleQSEQTestInfo();
//     if (gsWCDMATRxCPKInfo.mbIsTest) gvWCDMATRxCPKInfo.push_back(gsWCDMATRxCPKInfo);
//     gsWCDMATRxCPKInfo.Init();
//     if (gsLTETRxCPKInfo.mbIsTest) gvLTETRxCPKInfo.push_back(gsLTETRxCPKInfo);
//     gsLTETRxCPKInfo.Init();
     mesQSEQCpkLog.BuildTDSCDMATestInfo(mParseCPKLogInfo.GetTDSCDMACPKLogInfos());
    mesQSEQCpkLog.BuildWCDMATestInfo(mParseCPKLogInfo.GetWCDMACPKLogInfos());
    mesQSEQCpkLog.BuildLTETestInfo(mParseCPKLogInfo.GetLTECPKLogInfos());
#else
    CCJGW_FtpClient ftpClient;
    if (!ftpClient.OpenFTPClient("192.168.133.36","user","password")) return -1;

    ftpClient.GetSession()->setWorkingDirectory("/");
    std::istream& istr = ftpClient.GetSession()->beginList("/",true);
    std::ostringstream dataStr;
    Poco::StreamCopier::copyStream(istr, dataStr);
    ftpClient.GetSession()->endList();
    std::string s(dataStr.str());
    ftpClient.CloseFTPClient();
    
    //ftpClient.CreateFTPDirectory("123\\456");

   //if (!ftpClient.OpenFTPClient("ftp.tinno.com","eng","eng")) return -1;
   // ftpClient.DownloadFTPFile("开发资料\\linuxpg.tar.bz2.bin","d:\\download_linuxformat.bin");

    //ftpClient.UpdateFTPFile("123\\456\\dev0_tdscdma_dc_cal_data222.dat","e:\\dev0_tdscdma_dc_cal_data.dat");
    //outf.close();
    /*
    Poco::Net::FTPClientSession* pSession = NULL;
    try
    {
        //pSession = new Poco::Net::FTPClientSession("ftp.tinno.com",21,"eng","eng");
        pSession = new Poco::Net::FTPClientSession("192.168.133.36",21,"user","password");
        if (pSession == NULL) return 0;

        std::cout<<"system is timeout is  : "<< pSession->getTimeout().totalSeconds()<<std::endl;

        std::string ROOTPWD="";
        ROOTPWD = pSession->getWorkingDirectory();
        std::cout<< "should be the root dir == " << ROOTPWD << std::endl; //打印目录

        //std::istream &ftpin = pSession->beginList("",true);//下载目录中文件列表，只有当前的文件名,true 带扩展的属性信息
        std::istream &ftpin = pSession->beginList("",false);//下载目录中文件列表 ,这样list的文件带前面的绝对路径
        //std::istream &ftpin = pSession->beginList();
        std::string str;
        std::vector<std::string> filelist;
        while(ftpin >> str)
        {//存储文件列表
            filelist.push_back(str);
        }
        pSession->endList();//关闭目录下载连接

        std::cout<<" total file number is "<< filelist.size() << std::endl;
        for(size_t i=0; i!=filelist.size(); i++)
        {
            std::cout<<"rm file "<<filelist[i]<<std::endl;
            //!pSession->remove(filelist[i]); //删除文件，注意如果是文件夹则失败
            //! std::string content;
            //下载每个文件并存储到字符串content中
            // Poco::StreamCopier::copyToString( pSession->beginDownload( filelist[i] ), content);
            // pSession->endDownload();//关闭下载连接
        }




        pSession->setWorkingDirectory("开发资料");
        ROOTPWD = pSession->getWorkingDirectory();
        std::cout<< "should be the root dir == " << ROOTPWD << std::endl; //打印目录

        std::ofstream  outf("d:\\download_linuxformat.bin", std::ios::binary|std::ios::trunc);
        if(!outf.is_open())
        {
            std::cout << "源文件打开失败" << std::endl;
            return 0;
        }

        std::istream& istr = pSession->beginDownload("linuxpg.tar.bz2");
        Poco::StreamCopier::copyStream(istr, outf);
        pSession->endDownload();
        outf.close();
// 
//         std::ifstream inputfile = pSession->beginDownload("linuxpg.tar.bz2");
    }
    catch (Poco::Net::FTPException& e)
    {
        std::cout<<"FTP ERROR:"<<e.displayText()<< std::endl;
    }
    catch (Poco::Net::NetException& e1)
    {
        std::cout<<"NET ERROR:"<<e1.displayText()<<std::endl;
    }

    if (pSession)
    {
        pSession->close();//断开FTP
        delete pSession;
    }*/
#endif
	return 0;
}

