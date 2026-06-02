#include "StdAfx.h"
#include "TSE_RFCalVFSTest.h"
#include "CJGW_MESQSEQCpkLog.h"
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_RFCalVFSTest::CTSE_RFCalVFSTest(void) : m_bInitOk(false)
        ,m_rfQualcommInter(&m_sRFQcommInterInfo)
        ,m_bUserQpst(FALSE)
        ,mstrSNEnvironment(L"TSE_READER_SN_NUMBER")
        ,m_strCPKFilePathEnvironment(L"TSE_CPK_FILE_PATH")
    {
        m_sRFQcommInterInfo.m_bErrorStop = true;
        m_sRFQcommInterInfo.m_nErrorReTest = 3;
        m_sRFQcommInterInfo.mbIsQSEQCore = false;
        m_sRFQcommInterInfo.m_dll_ptr = NULL;
        m_sRFQcommInterInfo.m_iface = NULL;
        memset(&m_sRFQcommInterInfo.m_sti_func_decl,0x00,sizeof(m_sRFQcommInterInfo.m_sti_func_decl));
        m_sRFQcommInterInfo.gbQcommTestResult = false;
        m_sRFQcommInterInfo.mbSaveTestLogging = false;
        m_sRFQcommInterInfo.mbRecordCPKData = false;
    }


    CTSE_RFCalVFSTest::~CTSE_RFCalVFSTest(void)
    {
    }

    const wchar_t* CTSE_RFCalVFSTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RFCalVFSTest\",\
                \"TestName\": \"校准综测(Qualcomm)\",\
                \"qcservername\": \"校准或者综测DLL名称\",\
                \"xttName\": \"xtt路径\",\
                \"ErrorReTest\": \"单项测试出现失败后重测次数\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\",\
                \"ErrorStop\": \"出现错误是否停止\",\
                \"QMSLLibraryMode\": \"0:QPHONEMS, 1:QPST 2:UserDefinedTransport. User should also set QMSLUserDefinedTransportDLLPath and QMSLLibraryModes\",\
                \"RecordCPKData\": \"是否保存CPK数据\",\
                \"SaveTestLogging\": \"保存测试记录\",\
                \"CPKFilePathEnvironment\": \"CPK数据路径保存的环境变量，默认:TSE_CPK_FILE_PATH\"\
                }";
    }

    bool CTSE_RFCalVFSTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("qcservername"),strParamName))
        {
            if (NULL != JGW_WStrCaseWStr(strParamValue,L"QSEQ_Core_Server.dll")) m_sRFQcommInterInfo.mbIsQSEQCore = true;
            m_sRFQcommInterInfo.mstrRFCalQSEQDllFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("xttName"),strParamName))
        {
            //! 获得配置XML文件路径
            m_sRFQcommInterInfo.m_strXttPath = JGW_W2A(JGW_RealativePathToAbsPath(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("ErrorReTest"),strParamName))
        {
            m_sRFQcommInterInfo.m_nErrorReTest = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("OperateID"),strParamName))
        {
            m_strOperateID = strParamValue;
            return true;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("ErrorStop"),strParamName))
        {
            m_sRFQcommInterInfo.m_bErrorStop = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("QMSLLibraryMode"),strParamName))
        {
            m_bUserQpst = _ttoi(strParamValue)?TRUE:FALSE;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("RecordCPKData"),strParamName))
        {
            m_sRFQcommInterInfo.mbRecordCPKData = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveTestLogging"),strParamName))
        {
            m_sRFQcommInterInfo.mbSaveTestLogging = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("CPKFilePathEnvironment"),strParamName))
        {
            m_strCPKFilePathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_RFCalVFSTest::TSE_Init()
    {
        m_bInitOk = false;
        if(!m_rfQualcommInter.LoadQCServerModule()) return false;
        m_rfQualcommInter.InitQCServerInterface(m_bUserQpst);
        m_bInitOk = m_rfQualcommInter.LoadRFCalVerTestConfig();
        if (m_strOperateID.empty()) m_strOperateID = JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId);
        return m_bInitOk;
    }

    bool CTSE_RFCalVFSTest::TSE_Run()
    {
        if (!m_bInitOk) return TSE_Init();
        return true;
    }

    bool CTSE_RFCalVFSTest::TSE_Exit()
    {
        if (!m_bInitOk) return false;
        m_sRFQcommInterInfo.mwstrSN = (GetGlobalEnvironment()->GetString(mstrSNEnvironment));
        m_sRFQcommInterInfo.mstrSN = JGW_W2A(m_sRFQcommInterInfo.mwstrSN);
        LogI(L"CTSE_RFCalVFSTest::TSE_Run\n");
        jgw_timer jt;
        bool bResult = m_rfQualcommInter.ExecuteRFCalVerTest();
        SaveCPKDataFile(bResult,(int)jt.elapsed());
        return bResult;
    }

    void CTSE_RFCalVFSTest::SaveCPKDataFile(bool bResult,int nTotalTime)
    {
        if (!m_sRFQcommInterInfo.mbRecordCPKData) return ;
        CCJGW_MESQSEQCpkLog mesQSEQCpkLog;
        mesQSEQCpkLog.BuildTestStation(
            JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrTaskNumber),
            m_strOperateID,
            JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->msMesConnectInfo.mstrUsername),
            m_sRFQcommInterInfo.mwstrSN
            ,bResult);
        mesQSEQCpkLog.BuildLotinfo(m_sRFQcommInterInfo.mwstrSN,nTotalTime,bResult);
        mesQSEQCpkLog.BuildTDSCDMATestInfo(m_rfQualcommInter.GetParseCPKData().GetTDSCDMACPKLogInfos());
        mesQSEQCpkLog.BuildWCDMATestInfo(m_rfQualcommInter.GetParseCPKData().GetWCDMACPKLogInfos());
        mesQSEQCpkLog.BuildLTETestInfo(m_rfQualcommInter.GetParseCPKData().GetLTECPKLogInfos());
        GetGlobalEnvironment()->PutString(m_strCPKFilePathEnvironment,mesQSEQCpkLog.GetCPKLogFilePath());
    }
}
