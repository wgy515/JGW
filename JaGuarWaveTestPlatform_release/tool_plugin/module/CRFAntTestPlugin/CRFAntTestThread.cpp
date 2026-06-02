#include "StdAfx.h"
#include "CRFAntTestThread.h"
#include "CRFAntTestView.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>

#define GOFAIL(x)\
    if(!(x))\
    goto TESTFAIL;

#define TEST_NS_FUNCOK(x)\
    if( !(x) )\
    return false;

namespace JGW
{
    CCRFAntTestThread::CCRFAntTestThread(void):m_hResourceContext(NULL)
        ,m_psConfigInfo(NULL)
        ,m_eTxPort(ERFAC)
        ,m_eDrxPort(ERFAO)
        ,m_cRwnv(NULL,true)
        ,m_bGsmStartCall(false)
        ,m_lpGsmBandPwlInfo(NULL)
        ,m_bCdmaStartCall(false)
        ,m_bWcdmaStartCall(false)
        ,m_bTDSCdmaStartCall(false)
        ,m_pCMW500EquipmentControll(NULL)
        ,m_bInitEquipment(false)
        ,m_bExecTSETest(false)
		,mhWlanBatProcess(NULL)
    {
    }


    CCRFAntTestThread::~CCRFAntTestThread(void)
    {
        if ( m_pCMW500EquipmentControll )
        {
            delete m_pCMW500EquipmentControll;
            m_pCMW500EquipmentControll = NULL;
        }
		CloseWlanBatProcess();
    }
        
	std::wstring CCRFAntTestThread::GetSerialNumber()
	{
		return m_strSN;
	}


    BEGIN_MYTHREAD_MESSAGE_MAP(CCRFAntTestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCRFAntTestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCRFAntTestThread::OnRunThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RE_TEST,&CCRFAntTestThread::OnRunRetest)
    END_MYTHREAD_MESSAGE_MAP()

    void CCRFAntTestThread::OnRunRetest( WPARAM wParam,LPARAM lParam )
    {
        _ASSERT( m_psConfigInfo );
        Sleep(m_psConfigInfo->m_nReTestTimeOut);
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);
    }

    bool CCRFAntTestThread::CheckMonitorConfigIsSame()
    {
        if ( !m_pCMW500EquipmentControll ) return false;

        if ( -1 != m_psConfigInfo->m_sMonitorInfo.m_strModelName.find(L"CMW500") && m_pCMW500EquipmentControll->GetMontiorType() == E_CMW500 )
        {
            return true;
        }
        else if ( -1 != m_psConfigInfo->m_sMonitorInfo.m_strModelName.find(L"8960") && m_pCMW500EquipmentControll->GetMontiorType() == E_8960 )
        {
            return true;
        }
        else if ( -1 != m_psConfigInfo->m_sMonitorInfo.m_strModelName.find(L"CMU200") && m_pCMW500EquipmentControll->GetMontiorType() == E_CMU200 )
        {
            return true;
        }
        else
        {
            return (m_pCMW500EquipmentControll->GetMontiorType() == E_CMW500);
        }
    }

    void CCRFAntTestThread::SetTestSuiteXmlFilePath(const std::wstring& strConfigXmlPath)
    {
        mstrConfigXmlPath = strConfigXmlPath;
    }

    void CCRFAntTestThread::OnInitWnd( WPARAM wParam,LPARAM lParam )
    {
        if( wParam ) m_hMainWnd = (HWND)wParam;
        m_psConfigInfo = (S_RF_CONFIG_INFO*)lParam;
        _ASSERT( m_psConfigInfo );
        _ASSERT( IsWindow(m_hMainWnd) );

        if ( m_pCMW500EquipmentControll )
        {
            delete m_pCMW500EquipmentControll;
            m_pCMW500EquipmentControll = NULL;
        }

        if ( -1 != m_psConfigInfo->m_sMonitorInfo.m_strModelName.find(L"CMW500") )
        {
            m_subSysVfs.InitClass(TECHNOLOGY_WCDMA)->m_niDL_PriCPICH_Scr_Code = 0;
            m_pCMW500EquipmentControll = new JGW_CMW500EquipmentControll;
            _ASSERT(m_pCMW500EquipmentControll);
        }
        /*else if ( -1 != m_psConfigInfo->m_sMonitorInfo.m_strModelName.find(L"8960") )
        {
        m_subSysVfs.InitClass(TECHNOLOGY_WCDMA)->m_niDL_PriCPICH_Scr_Code = 8;
        m_pRfTestMonitor = new CAgilent8960_RFVFS;
        _ASSERT(m_pRfTestMonitor);
        }
        else if ( -1 != m_psConfigInfo->m_sMonitorInfo.m_strModelName.find(L"CMU200") )
        {
        m_subSysVfs.InitClass(TECHNOLOGY_WCDMA)->m_niDL_PriCPICH_Scr_Code = 8;
        m_pRfTestMonitor = new CCMU200_RFVFS;
        }*/
        else
        {
            m_subSysVfs.InitClass(TECHNOLOGY_WCDMA)->m_niDL_PriCPICH_Scr_Code = 0;
            m_pCMW500EquipmentControll = new JGW_CMW500EquipmentControll;
            _ASSERT(m_pCMW500EquipmentControll);
        }
        //! Init Suite XML
        {
            if (!JGW_FileExistsToFilePath(mstrConfigXmlPath.c_str())) return ;
            m_bExecTSETest = true;
            if (!InitSequenceTest(m_hMainWnd,JGW_W2A(mstrConfigXmlPath)))
            {
                MessageBox(m_hMainWnd,L"TSE测试配置文件加载失败",L"TSE初始化错误",MB_ICONERROR);
            }
        }
    }

    void CCRFAntTestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
    }

    void CCRFAntTestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))  PELOG4WW(strInfoLog);
    }

    void CCRFAntTestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        std::wstring strTemp;
        switch (eTestStatus)
        {
        case E_INIT_TEST_STATUS:
            m_strTestContent = L"";
            JGW_FormatWString(strTemp,_T("%s@---@---@---@W"),strTestName);
            SendMsgToList(strTemp.c_str());
            break;
        case E_RUN_TEST_STATUS:
            JGW_FormatWString(strTemp,_T("%s@---@%s@---@W"),strTestName,m_strTestContent.empty()?L"---":m_strTestContent.c_str());
            SendMsgToList(strTemp.c_str(),true);
            break;
        case E_PASS_TEST_STATUS:
            JGW_FormatWString(strTemp,_T("%s@---@%s@---@P"),strTestName,m_strTestContent.empty()?L"---":m_strTestContent.c_str());
            SendMsgToList(strTemp.c_str());
            break;
        case E_FAIL_TEST_STATUS:
            JGW_FormatWString(strTemp,_T("%s@---@%s@---@F"),strTestName,m_strTestContent.empty()?L"---":m_strTestContent.c_str());
            SendMsgToList(strTemp.c_str());
            break;
        }
    }

    void CCRFAntTestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        switch (id)
        {
        case E_USER_DEFINE_HTMLFILEPATH:
            PHLOG4WW(strCustomLog);
            break;
        case E_TSE_SHOW_CONTENT_INFO:
            m_strTestContent = strCustomLog;
            break;
        }
    }

    void CCRFAntTestThread::OnRunThread( WPARAM wParam,LPARAM lParam )
    {
        int nComPort = (int)wParam;
        m_strErrorCode = L"";
		m_strSN = L"";
        mstrStartTestTime = JGW_W2A(JGW_GetCurrentTimeStamp());
        //! 初始化GBIP 仪器信息
        GOFAIL(InitGBIPAndCheckMonitor());
        //! 连接设备
        GOFAIL(ConnectDiag(nComPort));
        //! 读取设备SN
        GOFAIL(ReadSNView());
        //! 检查SN
        GOFAIL(CheckDbSNView());
        //! 设置当前设备模式为FTM
        GOFAIL(SetFTMModeView());
        //Sleep(2 * 1000);
        GOFAIL(ReConnectDiag());
#ifdef GSP_NS_SNR
        if ( m_psConfigInfo->m_sModeSelecter.gps )
        {
            GOFAIL(GPS_NS_TestSequence());
        }
#endif
#ifdef LTE_NS_TEST_
        if( m_psConfigInfo->m_sModeSelecter.lte )
        {
            GOFAIL(LTE_NS_TestSequence());
        }
#endif
#ifdef TDSCDMA_NS_TEST_
        if( m_psConfigInfo->m_sModeSelecter.tds )
        {
            GOFAIL(TDSCDMA_NS_TestSequence());
        }
#endif
#ifdef WCDMA_NS_TEST_
        if( m_psConfigInfo->m_sModeSelecter.wcdma )
        {
            GOFAIL(WCDMA_NS_TestSequence());
        }
#endif
#ifdef CDMA_NS_TEST_
        if ( m_psConfigInfo->m_sModeSelecter.cdma )
        {
            GOFAIL(CDMA_NS_TestSequence());
        }
#endif
#ifdef GSM_NS_TEST_
        if ( m_psConfigInfo->m_sModeSelecter.gsm )
        {
            GOFAIL(GSM_NS_TestSequence());
        }
#endif
#ifdef WLAN_NS_TEST_
        if (m_psConfigInfo->m_sModeSelecter.wlan)
        {
            GOFAIL(WLAN_NS_TestSequence());
        }
#endif
        UnSetFTMMode();
        GOFAIL(RecyFtmNVView());
        m_cRwnv.DisConnectDevice();
#ifdef TSE_TEST_
        if (m_bExecTSETest)
        {
            GOFAIL(ExecSequenceTest());
        }
#endif 
        GOFAIL(UpdateDeviceSnStationInfoView());
        GOFAIL(MESSaveTestRecords(true));
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
        return ;
TESTFAIL:
        //DisConnectDevices();
        m_cRwnv.DisConnectDevice();
        GOFAIL(MESSaveTestRecords(false));
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
    }

    //! 初始化TDSCDMA设备类
    bool CCRFAntTestThread::InitSubsysVFS(COMMUNICATIONSSYS sys)
    {
        m_eSys = sys;
        TEST_NS_FUNCOK( m_subSysVfs.InitClass(m_eSys) );
        m_subSysVfs.GetClass()->SetResourceContext(m_hResourceContext);

        switch(m_eSys)
        {
        case  TECHNOLOGY_LTE:
            m_pCMW500EquipmentControll->SetInstrumentSys(E_LTE_NAME);
            break;
        case  TECHNOLOGY_WCDMA:
            m_pCMW500EquipmentControll->SetInstrumentSys(E_WCDMA_NAME);
            break;
        case  GSM:
            m_pCMW500EquipmentControll->SetInstrumentSys(E_GSM_NAME);
            break;
        case  TECHNOLOGY_CDMA:
            m_pCMW500EquipmentControll->SetInstrumentSys(E_CDMA_NAME);
            break;
        case  TECHNOLOGY_TD_SCDMA:
            m_pCMW500EquipmentControll->SetInstrumentSys(E_TDSCDMA_NAME);
            break;
        }

        return true;
    }

    bool CCRFAntTestThread::RecyFtmNVView()
    {
        unsigned char szdata = 0x00;
        for (int i = 0;i < 10;i ++)
        {
            if ( m_cRwnv.SetPhoneMode(MODE_ONLINE_F) && m_cRwnv.SyncNv() )
            {
                return true;
            }
            else
            {
                if ( m_cRwnv.WriterNV(453,&szdata,1) && m_cRwnv.SyncNv() )
                {
                    return true;
                }
            }
            Sleep(200);
        }
        SendMsgToList(_T("同步NV@---@失败@---@F"),true);
        return false;
    }

    void CCRFAntTestThread::DisConnectDevices()
    {
        m_cRwnv.DisconnectServer();
    }

    bool CCRFAntTestThread::CheckTestResult(bool bIsTest,const wchar_t* strTestContent,float fMinValue,float fTestValue,float fMaxValue)
    {
        if(!bIsTest) return true;

        std::wstring strTemp;
        bool bResult = (fTestValue < fMaxValue && fTestValue > fMinValue);
        JGW_FormatWString(strTemp,L"%s@%.1f@%.2f@%.1f@%s",strTestContent,fMinValue, fTestValue, fMaxValue,bResult?L"P":L"F");
        SendMsgToList(strTemp.c_str(),true);

        return bResult;
    }

    bool CCRFAntTestThread::SYS_NS_StartCall(const std::wstring& strSysName)
    {
        int i = 4;
        std::wstring strTemp;

        JGW_FormatWString(strTemp,L"%s_NS_StartCall@1@---@1@W",strSysName.c_str());
        SendMsgToList(strTemp.c_str(),true);
        while( i-- > 0 )
        {
            if( m_subSysVfs.GetClass()->StartCall() )
            {
                JGW_FormatWString(strTemp,L"%s_NS_StartCall@1@1@1@P",strSysName.c_str());
                SendMsgToList(strTemp.c_str());
                return true;
            }
            EnterFTMMode();
            Sleep(1000);
        }
        JGW_FormatWString(strTemp,L"%s_NS_StartCall@1@Fail@1@F",strSysName.c_str());
        SendMsgToList(strTemp.c_str());
        return false;
    }

    bool CCRFAntTestThread::SYS_NS_HandOver(const std::wstring& strSysName)
    {
        int i = 4;
        std::wstring strTemp;

        JGW_FormatWString(strTemp,L"%s_NS_HandOver@1@---@1@W",strSysName.c_str());
        SendMsgToList(strTemp.c_str(),true);
        while( i-- > 0 )
        {
            if( m_subSysVfs.GetClass()->HandOverChannel() )
            {
                JGW_FormatWString(strTemp,L"%s_NS_HandOver@1@1@1@P",strSysName.c_str());
                SendMsgToList(strTemp.c_str());
                return true;
            }
            EnterFTMMode();
            Sleep(1000);
        }
        JGW_FormatWString(strTemp,L"%s_NS_HandOver@1@Fail@1@F",strSysName.c_str());
        SendMsgToList(strTemp.c_str());
        return false;
    }

    bool CCRFAntTestThread::ReConnectDiag()
    {
        if( !m_cRwnv.ConnectDevice(QLIB_COM_AUTO_DETECT,10) )
        {
            SendMsgToList(_T("连接设备@---@设备连接失败@---@F"),true);
            return false;
        }
        m_hResourceContext = m_cRwnv.GetResourceContext();

        return true;
    }

    bool CCRFAntTestThread::InitGBIPAndCheckMonitor()
    {
        SendMsgToList(_T("初始化仪器@---@正在初始化仪器..@---@W"),true);
        if (!m_bInitEquipment)
        {

            switch (m_psConfigInfo->m_sMonitorInfo.m_cInterfaceType)
            {
            case E_GPIB_HIVE:
                m_pCMW500EquipmentControll->SetGPIB(
                    m_psConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType,
                    m_psConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface,
                    m_psConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress,
                    m_psConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress);
                m_bInitEquipment = true;
                break;
            case E_VISA_HIVE:
                m_pCMW500EquipmentControll->SetVisa(
                    m_psConfigInfo->m_sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress.c_str());
                m_bInitEquipment = true;
                break;
            case E_TCP_IP_HIVE:
                m_pCMW500EquipmentControll->SetTCPIP(
                    m_psConfigInfo->m_sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress.c_str(),
                    m_psConfigInfo->m_sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument);
                m_bInitEquipment = true;
                break;
            default:
                m_bInitEquipment = false;
            }     
        }

        if (m_bInitEquipment && m_pCMW500EquipmentControll->ResetMode())
        {
            SendMsgToList(_T("初始化仪器@---@OK@---@P"));
            return true;
        }
        SendMsgToList(_T("初始化仪器@---@FAIL@---@F"));
        m_bInitEquipment = false;
        return false;
    }

    //     bool CCRFAntTestThread::InitGBIPAndCheckMonitor()
    //     {
    //         std::wstring strTemp(L"");
    // 
    //         JGW_FormatWString(strTemp,L"BoardIndex@---@%d@---@P",m_psConfigInfo->m_sMonitorInfo.m_nGpibInterface);
    //         SendMsgToDisplay( strTemp.c_str() );
    // 
    //         JGW_FormatWString(strTemp,L"PrimaryAddress@---@%d@---@P",m_psConfigInfo->m_sMonitorInfo.m_nPrimaryAddress);
    //         SendMsgToDisplay( strTemp.c_str() );
    // 
    //         /*try
    //         {
    //         m_pRfTestMonitor->SetGPIB(NI_GPIB_TYPE,m_psConfigInfo->m_sMonitorInfo.m_nGpibInterface,m_psConfigInfo->m_sMonitorInfo.m_nPrimaryAddress);
    //         }
    //         catch( char* szBuffer)
    //         {
    //         MessageBoxA(m_hMainWnd,szBuffer,"仪器初始化失败",MB_ICONERROR);
    //         return false;
    //         }*/
    //         try
    //         {
    //             m_pCMW500EquipmentControll->SetVisa("TCPIP0::192.168.133.22::inst0::INSTR");
    //         }
    //         catch( char* szBuffer)
    //         {
    //             MessageBoxA(m_hMainWnd,szBuffer,"仪器初始化失败",MB_ICONERROR);
    //             return false;
    //         }
    // 
    // 
    //         return m_pCMW500EquipmentControll->InitMode();
    //     }


    bool CCRFAntTestThread::ConnectDiag(int nComPort)
    {
        m_cRwnv.SetResourceContex(m_hResourceContext);
        m_cRwnv.DisconnectServer(); 
        SendMsgToList(_T("连接设备@---@ 正在连接设备..@---@W"),true);
        
        if( !m_cRwnv.ConnectDevice() )
        {
            SendMsgToList(_T("连接设备@---@设备连接失败@---@F"));
            return false;
        }
        SendMsgToList( _T("连接设备@---@连接设备成功@---@P"));
        m_hResourceContext = m_cRwnv.GetResourceContext();

        return true;
    }

    void CCRFAntTestThread::SendMsgToList(const wchar_t* szBuffer,bool bInsertItem /* = false */)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,szBuffer,LPARAM(bInsertItem?1:0));
    }

    bool CCRFAntTestThread::UnSetFTMMode()
    {
        unsigned char szdata = 0x00;
        return m_cRwnv.WriterNV(453,&szdata,1)?true:false;
    }

    bool CCRFAntTestThread::SetFTMModeView()
    {
        SendMsgToList(_T("设置FTM模式@---@正在设置FTM模式..@---@W"),true);
        if( !SetFTMMode() )
        {
            SendMsgToList( _T("设置FTM模式@---@设置FTM模式失败@---@F"));
            return false;
        }
        SendMsgToList(_T("设置FTM模式@---@设置FTM模式成功@---@P"));
        return true;
    }


    bool CCRFAntTestThread::SetFTMMode(void)
    {
        for (int i = 0;i < 4;i ++)
        {
            if ( m_cRwnv.EnterFTMMode() )
            {
                return true;
            }
            Sleep(1000);
        }
        return false;
    }

    bool CCRFAntTestThread::EnterFTMMode(void)
    {
        return m_cRwnv.EnterFTMMode();
    }

#ifdef GSP_NS_SNR
    bool CCRFAntTestThread::GPS_InitMonitor()
    {
        if ( !m_pCMW500EquipmentControll->GPS_InitBSE(m_psConfigInfo->m_sGpsInfo.m_ePort,m_psConfigInfo->m_sGpsInfo.m_fCellPwr,m_psConfigInfo->m_sGpsInfo.m_fFreq) )
        {
            return false;
        }
        m_pCMW500EquipmentControll->SetTxLoss( m_psConfigInfo->m_sGpsInfo.m_floss );

        return true;
    }

    bool CCRFAntTestThread::GPS_NS_TestSequence( void )
    {
        bool bResult = false;
        SendMsgToDisplay(L"GPS SNR测试开始@1@1@1@P");

        if (!GPS_InitMonitor())
        {
            SendErrMsgToDisplay(L"仪器初始化@1@Fail@1@F");
            return false;
        }

        if (!GPS_NS_StartCall())
        {
            SendErrMsgToDisplay(L"设备初始化@1@Fail@1@F");
            return false;
        }

        bResult &= GPS_NS_StartTest();

        bResult &= GPS_NS_EndCall();

        bResult &= m_pCMW500EquipmentControll->FinalizeBSE();  //释放仪器
        TEST_NS_FUNCOK(bResult);

        return true;
    }

    bool CCRFAntTestThread::GPS_NS_StartCall()
    {
        unsigned char bOk = true;
        long EngineRevision = 0;
        unsigned char piStatus = 0;


        bOk &= QLIB_GPS_GEN8_Get_GNSS_Engine_Revision(m_hResourceContext,&EngineRevision);
        TEST_NS_FUNCOK(bOk);

        unsigned char  Requested_Constellation_Mask = 0;//GPS
        unsigned char  Supportted_Contellation_Mask = 0;
        unsigned char  Actual_Constellsation_Mask = 0;

        bOk &= QLIB_GPS_GEN8_GNSS_NAV_SET_CONFIG( m_hResourceContext,Requested_Constellation_Mask,&Supportted_Contellation_Mask,&Actual_Constellsation_Mask);
        TEST_NS_FUNCOK(bOk);

        for (int nIndex = 0;nIndex < 50;nIndex ++)
        {
            bOk &= QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(m_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE,&piStatus);
            if (bOk && !piStatus)
            {
                return true;
            }
            Sleep(100);
        }

        return false;
    }

    bool CCRFAntTestThread::GPS_NS_EndCall()
    {
        unsigned char piStatus = false;

        QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(m_hResourceContext, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE,&piStatus);

        return true;
    }

    bool CCRFAntTestThread::GPS_NS_CheckCN(const double pdCNoDBHz)
    {
        wchar_t szBuffer[MAX_PATH] = {0};
        float maxCN = m_psConfigInfo->m_sGpsInfo.m_fSnrLim[MAX_VALUE]/*m_modelParam.gps_test_limit.m_fSNRUp*/;//-60
        float minCN = m_psConfigInfo->m_sGpsInfo.m_fSnrLim[MIN_VALUE]/*m_modelParam.gps_test_limit.m_fSNRLow*/;//-65

        _stprintf_s(szBuffer,MAX_PATH,L"GPS SNR @%.2f@%.2lf@%.2f",minCN, pdCNoDBHz, maxCN);
        if (pdCNoDBHz >= minCN && pdCNoDBHz <= maxCN)
        {
            SendMsgToDisplay(szBuffer);
            return true;
        }
        SendErrMsgToDisplay(szBuffer);

        return false;
    }

    bool CCRFAntTestThread::GPS_NS_StartTest()
    {
        double pdCNoDBHz = 0;
        int piFreqInHz = 0;
        unsigned char bOk = true;
        unsigned char iCollectMode = 1;//gps Prx
        unsigned short iCaptureSize_kSamples = 32;
        unsigned char  iNumberOfIntegrations = 10;

        for (int nIndex = 0;nIndex < 10;nIndex ++)
        {
            if( QLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON( m_hResourceContext,iCollectMode,iCaptureSize_kSamples,iNumberOfIntegrations,&pdCNoDBHz,&piFreqInHz) )
            {
                if( GPS_NS_CheckCN(pdCNoDBHz) )
                {
                    return true;

                }
            }

            Sleep(1000);
        }

        return false;
    }

#endif

#ifdef LTE_NS_TEST_
    bool CCRFAntTestThread::LTE_NS_TestSequence()
    {
        SendMsgToList(L"LTE Start Test@1@1@1@P",true);
        bool bResult = true;

        TEST_NS_FUNCOK( InitSubsysVFS(TECHNOLOGY_LTE) );

        bResult &= LTE_NS_Test();

        bResult &= m_pCMW500EquipmentControll->FinalizeBSE();

        return bResult;
    }

    bool CCRFAntTestThread::LTE_NS_InitMonitor()
    {
        int nCount = 4;
     
        while(nCount-- > 0)
        {
            if( m_pCMW500EquipmentControll->SetInOutPutPort(m_eTxPort,m_eTxPort) && m_pCMW500EquipmentControll->LTE_SetBandAndMode(m_nBand) )
            {
                return true;
            }
            SendMsgToList(L"LTE_NS_InitMonitor@1@Fail@1@F",true);
            Sleep(100);
        }

        return false;
    }

    bool CCRFAntTestThread::LTE_NS_Test()
    {
        std::wstring strTemp( L"" );
        bool bResult = true;
        int nCounts = m_psConfigInfo->m_nErrorReTestCount;

        for (std::vector<S_BAND_INFO>::iterator it = m_psConfigInfo->m_sLteInfo.m_sTestBandInfo.m_vBandInfo.begin();
            it != m_psConfigInfo->m_sLteInfo.m_sTestBandInfo.m_vBandInfo.end();
            it ++ ,nCounts = m_psConfigInfo->m_nErrorReTestCount)
        {
            m_eTxPort = it->m_ePort;
            m_eDrxPort = it->m_eDrxPort;
            m_nBand = it[0].m_nBand;
            m_lpBandInfo = &it[0];
            TEST_NS_FUNCOK( LTE_NS_InitMonitor() );
            nCounts = m_psConfigInfo->m_nErrorReTestCount;

            for ( std::vector<S_BAND_CHANNEL_INFO>::iterator itChannel = it->m_vBandChannelInfo.begin();
                itChannel != it->m_vBandChannelInfo.end() && nCounts > 0;
                nCounts--/*itChannel ++ */)
            {
                m_fPTxLoss = itChannel->m_fPTxLoss;
                m_fPRxLoss = itChannel->m_fPRxLoss;
                m_fDRxLoss = itChannel->m_fDRxLoss;
                TEST_NS_FUNCOK( LTE_NS_SetBandInfo(itChannel) );

                if ( itChannel == it->m_vBandChannelInfo.begin() && m_psConfigInfo->m_nErrorReTestCount == nCounts )
                {
                    TEST_NS_FUNCOK(SYS_NS_StartCall(L"LTE"));
                }
                else
                {
                    TEST_NS_FUNCOK(SYS_NS_HandOver(L"LTE"));
                }

                bResult = LTE_NS_StartTest();

                if( bResult )
                {
                    itChannel ++;
                    nCounts = m_psConfigInfo->m_nErrorReTestCount;
                }
            }

            m_subSysVfs.GetClass()->EndCall();
            if( !bResult )
            {
                return false;
            }
        }

        return bResult;
    }

    bool CCRFAntTestThread::LTE_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel)
    {
        std::wstring strTemp( L"" );

        m_subSysVfs.GetClass()->InitBandInfo(m_nBand,itChannel->m_nUpLinkChannel,itChannel->m_nDownLinkChannel,itChannel->m_fUpLinkFreq,itChannel->m_fDownLinkFreq);
        FormatWString(strTemp,L"LTE Test Begin@Band=%d@Channel=%d@Frequency=%f@P",m_nBand, itChannel->m_nUpLinkChannel, itChannel->m_fUpLinkFreq );
        SendMsgToList(strTemp.c_str(),true);

        if( !m_pCMW500EquipmentControll->SetGPRFDlUlFreq(itChannel->m_fDownLinkFreq,itChannel->m_fUpLinkFreq) )
        {
            SendMsgToList(L"LTE_NS_SetMonitorDlUlFrq@1@Fail@1@F",true);
            return false;
        }

        m_pCMW500EquipmentControll->SetTxLoss( m_fPRxLoss ) ;
        m_pCMW500EquipmentControll->SetRxLoss( m_fPTxLoss );

        return true;
    }

    bool CCRFAntTestThread::LTE_NS_StartTest()
    {
        bool bResult = true;
        TEST_NS_FUNCOK( LTE_NS_TxRfNoSignalTest() );

        bResult = LTE_NS_RxRfNoSignalTest() ;

        if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort))
        {
            SendMsgToList(L"There're no Two Sub Instruments@1@0@1@F",true);
            return false;
        }

        return bResult;
    }

    bool CCRFAntTestThread::LTE_NS_TxRfNoSignalTest()
    {
        TEST_NS_FUNCOK( LTE_NS_MeasuresAll() );
        Sleep(500);
        //TEST_NS_FUNCOK( LTE_NS_TestMinPwr() );
        return true;
    }

    bool CCRFAntTestThread::LTE_NS_MeasuresAll( void )
    {
        if( !m_psConfigInfo->m_sLteInfo.m_bTestMaxPwr )
        {
            return true;
        }
        bool bResult = false;
        LTE_NS_MaxPwrTestValue testValue = {0};

        SendMsgToList(L"Start MaxP test@1@---@1@W",true);
        for (int nIndex = 0;nIndex < 4;nIndex ++)
        {
            if ( !m_subSysVfs.GetClass()->Check_NS_Is_Connected() )
            {
                SendMsgToList(L"LTE-MaxPwr Ns Unconnected!@1@0@1@F",true);
                return false;
            }

            if( m_pCMW500EquipmentControll->LTE_NS_MesurmentAll(testValue) )
            {
                if (LTE_NS_CheckMaxPwr(testValue.maxPwr))
                {
                    bResult = true;
                    break;
                }
            }

            if (!m_subSysVfs.GetClass()->LTE_NS_SetUpLinkWaveform(1, 0, 0, 50, 5))
            {
                SendMsgToList(L"Set Maxp-Waveform Fail!@1@0@1@F",true);
                return false;
            }

            if ( !m_subSysVfs.GetClass()->SetTargetTxPower(250, 1) )
            {
                SendMsgToList(L"Set MaxPwr Fail!@1@0@1@F",true);
                return false;
            }

            Sleep(500);
        }

        if (!bResult)
        {
            SendMsgToList(L"Get LTE MAX-Mesurment Failed!@1@0@1@F",true);
        }
        else
        {
            /**********确保UE设备功率下降 OK***************/
            if (!m_subSysVfs.GetClass()->LTE_NS_SetUpLinkWaveform(1, 0, 0, 50, 5))
            {
                SendMsgToList(L"LTE_NS_SetUpLinkWaveform@---@Fail@---@F",true);
                return false;
            }

            if ( !m_subSysVfs.GetClass()->SetTargetTxPower(-450, 1) )
            {
                SendMsgToList(L"Set MaxPwr Fail!@---@Fail@---@F",true);
                return false;
            }
        }

        return bResult;
    }


    bool CCRFAntTestThread::LTE_NS_CheckMaxPwr( float maxPwr )
    {
        return CheckTestResult(m_psConfigInfo->m_sLteInfo.m_bTestMaxPwr,L"Max Power",m_psConfigInfo->m_sLteInfo.m_fMaxPwrLim[MIN_VALUE], maxPwr, m_psConfigInfo->m_sLteInfo.m_fMaxPwrLim[MAX_VALUE]);
    }
#if 0
    bool CCRFAntTestThread::LTE_NS_CheckACLR( float ACLR[6] )
    {
        if ( !m_psConfigInfo->m_sLteInfo.m_bTestAclr )
        {
            return true;
        }

        wchar_t szMsgBuffer[MAX_PATH] = {0};
        float fAclrValue[6] = {0};
        float fEutraAclrMin = 0.0;
        float fUtraOneAclrMin = 0.0;
        float fUtraTwoAclrMin = 0.0;
        bool bResult = false;

        fEutraAclrMin = m_psConfigInfo->m_sLteInfo.m_fEutraAclrLim[MIN_VALUE]/*m_modelParam.lte_test_limit.m_EutraAclrMin*/;
        fUtraOneAclrMin = m_psConfigInfo->m_sLteInfo.m_fUtra1AclrLim[MIN_VALUE]/*m_modelParam.lte_test_limit.m_UtraOneAclrMin*/;
        fUtraTwoAclrMin = m_psConfigInfo->m_sLteInfo.m_fUtra2AclrLim[MIN_VALUE]/*m_modelParam.lte_test_limit.m_UtraTwoAclrMin*/;

        for (int nIndex = 0; nIndex <= 5; nIndex++)
        {
            bResult =  true;
            if (nIndex >= 0 && nIndex <= 1)
            {
                _stprintf_s(szMsgBuffer, MAX_PATH,L"EutraACLR@ %.1f @ %.2f @ 9999", fEutraAclrMin, ACLR[nIndex]);

                if (ACLR[nIndex] < fEutraAclrMin)
                {
                    SendErrMsgToDisplay(szMsgBuffer,true);
                    bResult =  false;
                }
                else
                {
                    SendMsgToDisplay(szMsgBuffer,true);
                }
            }
            else if (nIndex >= 2 && nIndex <= 3)
            {
                _stprintf_s(szMsgBuffer, MAX_PATH,L"Utra1ACLR@ %.1f @ %.2f @ 9999", fUtraOneAclrMin, ACLR[nIndex]);

                if (ACLR[nIndex] < fUtraOneAclrMin)
                {
                    SendErrMsgToDisplay(szMsgBuffer,true);
                    bResult =  false;
                }
                else
                {
                    SendMsgToDisplay(szMsgBuffer,true);
                }
            }
            else
            {
                _stprintf_s(szMsgBuffer, MAX_PATH,L"Utra2ACLR@ %.1f @ %.2f @ 9999", fUtraTwoAclrMin, ACLR[nIndex]);
                if (ACLR[nIndex] < fUtraTwoAclrMin)
                {
                    SendErrMsgToDisplay(szMsgBuffer,true);
                    bResult =  false;
                }
                else
                {
                    SendMsgToDisplay(szMsgBuffer,true);	
                }
            }

            TEST_NS_FUNCOK(bResult);
        }

        return true;
    }

    bool CCRFAntTestThread::LTE_NS_CheckSEM( int sem )
    {
        if ( !m_psConfigInfo->m_sLteInfo.m_bTestSem )
        {
            return true;
        }

        wchar_t szMsgBuffer[MAX_PATH] = {0};

        _stprintf_s(szMsgBuffer, MAX_PATH,L"SEM @ %d @ %d @ %d",sem ,sem,sem);

        if (sem)
        {
            SendMsgToDisplay(szMsgBuffer);
            return true;
        }
        SendErrMsgToDisplay(szMsgBuffer);

        return false;
    }

    bool CCRFAntTestThread::LTE_NS_CheckEvm( float evm )
    {
        if ( !m_psConfigInfo->m_sLteInfo.m_bTestEvm )
        {
            return true;
        }

        bool bResult = false;
        wchar_t szMsgBuffer[MAX_PATH]={0};

        _stprintf_s(szMsgBuffer, MAX_PATH,L"EVM@%.1f@%.2f@%.1f",m_psConfigInfo->m_sLteInfo.m_fEvmLim[MIN_VALUE], evm,  m_psConfigInfo->m_sLteInfo.m_fEvmLim[MAX_VALUE]);

        if ( evm > m_psConfigInfo->m_sLteInfo.m_fEvmLim[MIN_VALUE] && evm <= m_psConfigInfo->m_sLteInfo.m_fEvmLim[MAX_VALUE])
        {
            bResult = true;
            SendMsgToDisplay(szMsgBuffer);
        }
        else
        {
            bResult = false;
            SendErrMsgToDisplay(szMsgBuffer);
        }

        return bResult;
    }

    bool CCRFAntTestThread::LTE_NS_CheckFreqError( float freqError )
    {
        if ( !m_psConfigInfo->m_sLteInfo.m_bTestFrqErr )
        {
            return true;
        }

        float fFqerLimMax = 0.0;
        float fFqerLimMin = 0.0;
        wchar_t szMsgBuffer[MAX_PATH] = {0};
        bool bResult = false;

        fFqerLimMax = m_psConfigInfo->m_sLteInfo.m_fFreqErrLim[MAX_VALUE];
        fFqerLimMin = m_psConfigInfo->m_sLteInfo.m_fFreqErrLim[MIN_VALUE];

        _stprintf_s(szMsgBuffer, MAX_PATH,L"Frequency Error @%.1f@%.2f@%.1f", fFqerLimMin, freqError, fFqerLimMax);

        if (freqError > fFqerLimMax)
        {
            bResult = false;
            SendErrMsgToDisplay(szMsgBuffer);
        }
        else
        {
            bResult = true;
            SendMsgToDisplay(szMsgBuffer);
        }	

        return bResult; 
    }

    bool  CCRFAntTestThread::LTE_NS_CheckOBW( float OBW )
    {
        if ( !m_psConfigInfo->m_sLteInfo.m_bTestOBW )
        {
            return true;
        }

        bool bResult = false;
        wchar_t szMsgBuffer[MAX_PATH] = {0};
        float obwLimMax = 0;
        float obwLimMin = 0;

        obwLimMin = m_psConfigInfo->m_sLteInfo.m_fObwLim[MIN_VALUE];
        obwLimMax = m_psConfigInfo->m_sLteInfo.m_fObwLim[MAX_VALUE];

        float OBWMHZ = OBW/1000000.0f;

        _stprintf_s(szMsgBuffer, MAX_PATH,L"OBW @%.1f@%.2f@%.1f", obwLimMin, OBWMHZ, obwLimMax);

        if (OBWMHZ > obwLimMin && OBWMHZ<obwLimMax)
        {
            bResult = true;
            SendMsgToDisplay(szMsgBuffer);
        }
        else
        {
            bResult = false;
            SendErrMsgToDisplay(szMsgBuffer);
        }
        return bResult;
    }

    bool CCRFAntTestThread::LTE_NS_TestMinPwr()
    {
        if ( !m_psConfigInfo->m_sLteInfo.m_bTestMinPwr )
        {
            return true;
        }

        float fMinPower = 0.0;
        float fPwrHigh = m_psConfigInfo->m_sLteInfo.m_fMinPwrLim[MAX_VALUE];
        float fPwrLow = m_psConfigInfo->m_sLteInfo.m_fMinPwrLim[MIN_VALUE];
        wchar_t szMsgBuffer[MAX_PATH]={0};
        bool bResult = false;

        SendMsgToDisplay(L"Start MinP test@---@---@---");

        for (int nIndex = 0;nIndex < 3;nIndex ++)
        {
            if ( !m_subSysVfs.GetClass()->Check_NS_Is_Connected() )
            {
                SendErrMsgToDisplay(L"LTE-MaxPwr Ns Unconnected!@---@---@---");
                return false;
            }

            bResult = m_pCMW500EquipmentControll->LTE_NS_GetMinPower(fMinPower);

            if(bResult)
            {
                _stprintf_s(szMsgBuffer, MAX_PATH,L"Min Power @ %.1f @ %.2f @ %.1f", fPwrLow, fMinPower, fPwrHigh);
                if (fMinPower < fPwrLow || fMinPower > fPwrHigh)
                {	
                    bResult = false;
                }
                else
                {
                    bResult = true;
                    SendMsgToDisplay(szMsgBuffer);
                    break;
                }	
            }

            /**********确保UE设备功率下降 OK***************/
            if (!m_subSysVfs.GetClass()->LTE_NS_SetUpLinkWaveform(1, 0, 0, 50, 5))
            {
                SendErrMsgToDisplay(L"Set Maxp-Waveform Fail!@---@---@---");
                return false;
            }

            if ( !m_subSysVfs.GetClass()->SetTargetTxPower(-450, 1) )
            {
                SendErrMsgToDisplay(L"Set MaxPwr Fail!@---@---@---");
                return false;
            }

            Sleep(500);
        }


        if (!bResult)
        {
            SendErrMsgToDisplay(szMsgBuffer);
            SendErrMsgToDisplay(L"Get Min Power Fail!@---@---@---");
        }
        else
        {
            if (!m_subSysVfs.GetClass()->LTE_NS_SetUpLinkWaveform(1, 0, 0, 50, 5))
            {
                SendErrMsgToDisplay(L"Set Maxp-Waveform Fail!@---@---@---");
                return false;
            }

            if ( !m_subSysVfs.GetClass()->SetTargetTxPower(25, 1) )
            {
                SendErrMsgToDisplay(L"Set MaxPwr Fail!@---@---@---");
                return false;
            }
        }

        return bResult;
    }
#endif
    bool CCRFAntTestThread::LTE_NS_RxRfNoSignalTest()
    {
        m_pCMW500EquipmentControll->CloseMEValuation();

        if ( m_psConfigInfo->m_sLteInfo.m_bTestRxLevel )
        {
            TEST_NS_FUNCOK( LTE_NS_TestRxLevel(0) );
            TEST_NS_FUNCOK( LTE_NS_TestRxLevel(1) );
        }

        if( !m_psConfigInfo->m_bIsTestRx || !m_psConfigInfo->m_sLteInfo.m_bTestBler )
        {
            return true;
        }

        wchar_t szMsg[MAX_PATH] = {0};

        TEST_NS_FUNCOK(LTE_NS_TestFTMBler(0,szMsg));
        TEST_NS_FUNCOK(LTE_NS_TestFTMBler(1,szMsg));
        m_pCMW500EquipmentControll->SetCellPower();

        return true;
    }

    bool CCRFAntTestThread::LTE_NS_TestRxLevel(int nChain)
    {
        if ( nChain == 0)
        {
            if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort))
            {
                SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
                return false;
            }

            m_pCMW500EquipmentControll->SetTxLoss(m_fPRxLoss);
        }
        else
        {
            if(ERF_UNKNOW == m_lpBandInfo->m_eDrxPort)
            {
                return true;
            }

            if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_eDrxPort,m_lpBandInfo->m_ePort))
            {
                SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
                return false; 
            }
            m_pCMW500EquipmentControll->SetTxLoss(m_fDRxLoss);
        }

        if ( !m_pCMW500EquipmentControll->SetCellPower(m_psConfigInfo->m_sLteInfo.m_fRxLevelCellPwr) )
        {
            SendMsgToList(L"Set Cell Pwr@---@Fail@---@F",true);
            return false; 
        }
        short iRxAGC = 0,iRxLeveldBm10 = 0;
        unsigned long pStatus = 0;

        for (int i = 0;i < 20;i ++)
        {
            m_cRwnv.FTM_LTE_NS_GET_DL_LEVEL(nChain,&iRxAGC,&iRxLeveldBm10, &pStatus );
            if ( 0 == pStatus && LTE_NS_CheckRxLevel(iRxLeveldBm10) )
            {
                return true;
            }
            Sleep(200);
        }

        return false;
    }

    bool CCRFAntTestThread::LTE_NS_TestFTMBler(int nChain,wchar_t* szMsg)
    {
        if ( nChain == 0)
        {
            if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort))
            {
                SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
                return false;
            }
        }
        else
        {
            if(ERF_UNKNOW == m_lpBandInfo->m_eDrxPort)
            {
                return true;
            }

            if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_eDrxPort,m_lpBandInfo->m_ePort))
            {
                SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
                return false; 
            }
        }

        if ( !m_pCMW500EquipmentControll->SetCellPower(m_psConfigInfo->m_sLteInfo.m_fBlerCellPwr) )
        {
             SendMsgToList(L"Set Cell Pwr@---@Fail@---@F",true);
            return false; 
        }

        float fBlerErr = 0;
        for (int i = 0;i < m_psConfigInfo->m_nErrorReTestCount;i ++)
        {
            if ( m_subSysVfs.GetClass()->TestBER(nChain,m_psConfigInfo->m_sLteInfo.m_fBlerCellPwr,m_psConfigInfo->m_sLteInfo.m_fBerLim[MAX_VALUE],fBlerErr,szMsg) )
            {
                wcscat_s(szMsg,260,L"@P");
                SendMsgToList(szMsg,true);
                return true;
            }
            wcscat_s(szMsg,260,L"@F");
            SendMsgToList(szMsg,true);
        }

        return false;
    }

    bool CCRFAntTestThread::LTE_NS_CheckRxLevel(short iRxLeveldBm10)
    {
        float testRssi = (float)iRxLeveldBm10/(float)10;
        return CheckTestResult(true,L"RSSI LEVEL",m_psConfigInfo->m_sLteInfo.m_fRxLevelLim[MIN_VALUE], testRssi, m_psConfigInfo->m_sLteInfo.m_fRxLevelLim[MAX_VALUE]);
    }

#endif

#ifdef TDSCDMA_NS_TEST_
    bool CCRFAntTestThread::TDSCDMA_NS_TestSequence()
    {
        SendMsgToList(L"TDSCDMA Start Test@1@1@1@P",true);
        //! TDSCDMA 序列初始化测试
        bool bResult = true;
        m_bTDSCdmaStartCall = false;
        TEST_NS_FUNCOK(InitSubsysVFS(TECHNOLOGY_TD_SCDMA));

        //! 初始化仪器
        TEST_NS_FUNCOK( TDSCDMA_InitMonitor() );
        //! 开始测试TDSCDMA
        bResult &= TDSCDMA_NS_Test();
        //! 挂断TDSCDMA测试
        m_subSysVfs.GetClass()->EndCall();
        //! 释放仪器
        bResult &= m_pCMW500EquipmentControll->FinalizeBSE();

        return bResult;
    }

    bool CCRFAntTestThread::TDSCDMA_InitMonitor()
    {
        if ( !m_pCMW500EquipmentControll->TDSCDMA_InitBSE() || !m_pCMW500EquipmentControll->SetInOutPutPort(ERFAC,ERFAC) )
        {
            SendMsgToList(L"TDSCDMA 仪器初始化失败@0@Fail@0@F",true);
            return false;
        }

        return true;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_Test()
    {
        std::wstring strTemp( L"" );
        bool bResult = true;
        int nCounts = m_psConfigInfo->m_nErrorReTestCount;

        for (std::vector<S_BAND_INFO>::iterator it = m_psConfigInfo->m_sTdscdmaInfo.m_sTestBandInfo.m_vBandInfo.begin();
            it != m_psConfigInfo->m_sTdscdmaInfo.m_sTestBandInfo.m_vBandInfo.end();
            it ++ )
        {
            m_nBand = it[0].m_nBand;
            m_lpBandInfo = &it[0];
            nCounts = m_psConfigInfo->m_nErrorReTestCount;

            for ( std::vector<S_BAND_CHANNEL_INFO>::iterator itChannel = it->m_vBandChannelInfo.begin();
                itChannel != it->m_vBandChannelInfo.end() && nCounts > 0;
                nCounts--/*itChannel ++ */)
            {
                m_fPTxLoss = itChannel->m_fPTxLoss;
                m_fPRxLoss = itChannel->m_fPRxLoss;
                m_fDRxLoss = itChannel->m_fDRxLoss;
                TEST_NS_FUNCOK( TDSCDMA_NS_SetBandInfo(itChannel) );
                if ( !m_bTDSCdmaStartCall )
                {
                    TEST_NS_FUNCOK(SYS_NS_StartCall(L"TDSCDMA"));
                    m_bTDSCdmaStartCall = true;
                }
                else
                {
                    TEST_NS_FUNCOK(SYS_NS_HandOver(L"TDSCDMA"));
                }

                bResult = TDSCDMA_NS_StartTest();

                if(bResult)
                {
                    itChannel ++;
                    nCounts = m_psConfigInfo->m_nErrorReTestCount;
                }
            }
            TEST_NS_FUNCOK( bResult );
        }

        return bResult;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel)
    {
        std::wstring strTemp( L"" );

        m_subSysVfs.GetClass()->InitBandInfo(m_nBand,itChannel->m_nUpLinkChannel,itChannel->m_nDownLinkChannel,itChannel->m_fUpLinkFreq,itChannel->m_fDownLinkFreq);
        FormatWString(strTemp,L"TDSCDMA Test Begin@Band=%d@Channel=%d@Frequency=%f@P",m_nBand, itChannel->m_nUpLinkChannel, itChannel->m_fUpLinkFreq );
        SendMsgToList(strTemp.c_str(),true);

        if( m_pCMW500EquipmentControll->SetGPRFBand(m_nBand) 
            && m_pCMW500EquipmentControll->SetGPRFDlUlFreq(itChannel->m_fDownLinkFreq,itChannel->m_fUpLinkFreq) 
            )
        {
            m_pCMW500EquipmentControll->SetTxLoss( m_fPRxLoss ) ;
            m_pCMW500EquipmentControll->SetRxLoss( m_fPTxLoss );
            return true;
        }
        SendMsgToList(L"仪器初始化失败@1@Fail@1@F",true);
        return false;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_StartTest()
    {
        TEST_NS_FUNCOK(TDSCDMA_NS_TxRfNoSignalTest());

        TEST_NS_FUNCOK(TDSCDMA_NS_RxRfNoSignalTest());

        return true;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_TxRfNoSignalTest()
    {
        if(	!(TDSCDMA_NS_TestMinPwr() && TDSCDMA_NS_MeasuresAll()))
        {
            return false;
        }

        return true;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_CheckRxLevel(short iRxLeveldBm10)
    {
        float testRssi = (float)iRxLeveldBm10/(float)10;
        return CheckTestResult(true,L"RSSI LEVEL",m_psConfigInfo->m_sTdscdmaInfo.m_fRxLevelLim[MIN_VALUE] , testRssi, m_psConfigInfo->m_sTdscdmaInfo.m_fRxLevelLim[MAX_VALUE]);
    }

    bool CCRFAntTestThread::TDSCDMA_NS_TestRxLevel(int nChain)
    {
        if ( nChain == 0)
        {
            if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort))
            {
                SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
                return false;
            }

            m_pCMW500EquipmentControll->SetTxLoss(m_fPRxLoss);
        }
        else
        {
            if(ERF_UNKNOW == m_lpBandInfo->m_eDrxPort)
            {
                return true;
            }

            if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_eDrxPort,m_lpBandInfo->m_ePort))
            {
                SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
                return false; 
            }
            m_pCMW500EquipmentControll->SetTxLoss(m_fDRxLoss);
        }

        if ( !m_pCMW500EquipmentControll->SetCellPower(m_psConfigInfo->m_sTdscdmaInfo.m_fRxLevelCellPwr) )
        {
             SendMsgToList(L"Set Cell Pwr@---@Fail@---@F",true);
            return false; 
        }
        short iRxAGC = 0,iRxLeveldBm10 = 0;
        unsigned short pStatus = 0;

        for (int i = 0;i < 20;i ++)
        {
            m_cRwnv.FTM_TDSCDMA_BER_RX_LEVEL_REQ(nChain,&pStatus,&iRxAGC,&iRxLeveldBm10);
            if ( 0 == pStatus && TDSCDMA_NS_CheckRxLevel(iRxLeveldBm10) )
            {
                return true;
            }
            Sleep(200);
        }

        return false;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_RxRfNoSignalTest()
    {
        m_pCMW500EquipmentControll->CloseMEValuation();

        if ( m_psConfigInfo->m_sTdscdmaInfo.m_bTestRxLevel )
        {
            TEST_NS_FUNCOK( TDSCDMA_NS_TestRxLevel(0) );
            TEST_NS_FUNCOK( TDSCDMA_NS_TestRxLevel(1) );
        }

        if ( !m_psConfigInfo->m_bIsTestRx || !m_psConfigInfo->m_sTdscdmaInfo.m_bTestBler)
        {
            return true;
        }

        if ( !m_pCMW500EquipmentControll->SetCellPower(m_psConfigInfo->m_sTdscdmaInfo.m_fBlerCellPwr) )
        {
             SendMsgToList(L"Set Cell Pwr@---@Fail@---@F",true);
            return false; 
        }

        if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort))
        {
            SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
            return false;
        }

        float fBlerErr = 0;
        wchar_t szMsg[MAX_PATH] = {0};

        if ( m_subSysVfs.GetClass()->TestBER(m_psConfigInfo->m_nErrorReTestCount,m_psConfigInfo->m_sTdscdmaInfo.m_fBlerCellPwr,m_psConfigInfo->m_sTdscdmaInfo.m_fBerLim[MAX_VALUE],fBlerErr,szMsg) )
        {
            wcscat_s(szMsg,260,L"@P");
            SendMsgToList(szMsg,true);
            return true;
        }
        wcscat_s(szMsg,260,L"@F");
        SendMsgToList(szMsg,true);
        return false;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_TestMinPwr()
    {
        if (!m_psConfigInfo->m_sTdscdmaInfo.m_bTestMinPwr) return true;
        bool bResult = false;
        wchar_t buffer[MAX_PATH] = {0};
        double fChannPwr = 0.0;

        float MinPwrHigh = m_psConfigInfo->m_sTdscdmaInfo.m_fMinPwrLim[MAX_VALUE];
        float MinPwrLow = m_psConfigInfo->m_sTdscdmaInfo.m_fMinPwrLim[MIN_VALUE];
        int triggerCounter = 3;

        while ( triggerCounter-- > 0 )
        {
            bResult = m_pCMW500EquipmentControll->TDSCDMA_NS_GetMinPower(fChannPwr);

            if (bResult && fChannPwr > MinPwrLow && fChannPwr < MinPwrHigh)
            {
                m_subSysVfs.GetClass()->SetTargetTxPower(280);
                swprintf_s(buffer, L"TDS Min Power @%.1f@%.2f@%.1f@P", MinPwrLow, fChannPwr, MinPwrHigh);
                SendMsgToList(buffer,true);
                return true;
            }

            swprintf_s(buffer, L"TDS Min Power @%.1f@%.2f@%.1f@F", MinPwrLow, fChannPwr, MinPwrHigh);
            SendMsgToList(buffer,true);
            m_subSysVfs.GetClass()->SetTargetTxPower(550);
            Sleep(500);
        }
        return false;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_MeasuresAll()
    {
        bool bResult = true;
        TDS_NS_MaxPwrTestValue testValue = {0};

        for(int nIndex = 0; nIndex < 3; nIndex++)
        {
            bResult = m_pCMW500EquipmentControll->TDSCDMA_NS_MesurmentAll(testValue);
            if(bResult)
                if (TDSCDMA_NS_CheckMaxPwr(testValue.maxPwr)) return true;

            m_subSysVfs.GetClass()->SetTargetTxPower(280);
            Sleep(500);
        }
        SendMsgToList(L"Get TDS's MAXPowerValue Fail From Instrument! @1@Fail@1@F",true);

        return false;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_CheckMaxPwr(float fMaxPwr)
    {
        return CheckTestResult(m_psConfigInfo->m_sTdscdmaInfo.m_bTestMaxPwr,L"TDS_Max Power",m_psConfigInfo->m_sTdscdmaInfo.m_fMaxPwrLim[MIN_VALUE],fMaxPwr,m_psConfigInfo->m_sTdscdmaInfo.m_fMaxPwrLim[MIN_VALUE]);
    }
#if 0
    wchar_t ACLRMsg[4][4] = {L"L32",L"L16",L"R16",L"R32" };
    bool CCRFAntTestThread::TDSCDMA_NS_CheckACLR(float fACLR[4])
    {
        if (  !m_psConfigInfo->m_sTdscdmaInfo.m_bTestAclr )
        {
            return true;
        }

        wchar_t szMsgBuffer[MAX_PATH] = {0};

        double  AclrMin[4] = {0.0};
        AclrMin[0] = m_psConfigInfo->m_sTdscdmaInfo.m_fAclrNega32Lim[MIN_VALUE]/*m_modelParam.tds_test_limit.m_AclrNega32MMin*/;
        AclrMin[1] = m_psConfigInfo->m_sTdscdmaInfo.m_fAclrNega16Lim[MIN_VALUE]/* m_modelParam.tds_test_limit.m_AclrNega16MMin*/;
        AclrMin[2] = m_psConfigInfo->m_sTdscdmaInfo.m_fAclrPosi16Lim[MIN_VALUE];
        AclrMin[3] = m_psConfigInfo->m_sTdscdmaInfo.m_fAclrPosi32Lim[MIN_VALUE]/* m_modelParam.tds_test_limit.m_AclrPosi32MMin*/;

        int nIndex = 0;

        for (nIndex = 0; nIndex < 4; nIndex++)
        {
            swprintf_s(szMsgBuffer, MAX_PATH,L"TDS_ACLR%s@ %.1f(dB)@ %.2f (dB)@ 9999(dB)",ACLRMsg[nIndex], AclrMin[nIndex], fACLR[nIndex]);
            if (fACLR[nIndex] > AclrMin[nIndex])
            {
                SendMsgToDisplay(szMsgBuffer);
            }
            else
            {
                SendErrMsgToDisplay(szMsgBuffer);
                return false;
            }
        }

        return true;
    }

    wchar_t SEMMsg[6][3] = {L"AB",L"CD",L"DE",L"ED",L"DC",L"BA"};
    bool CCRFAntTestThread::TDSCDMA_NS_CheckSEM(float fsem[6])
    {
        if ( !m_psConfigInfo->m_sTdscdmaInfo.m_bTestSem )
        {
            return TRUE;
        }

        wchar_t szMsgBuffer[MAX_PATH] = {0};
        double SEMLim[6] = {0};

        SEMLim[0] = m_psConfigInfo->m_sTdscdmaInfo.m_nSemABLim[MAX_VALUE]/*m_modelParam.tds_test_limit.m_SEMABMargin*/;
        SEMLim[1] = m_psConfigInfo->m_sTdscdmaInfo.m_nSemCDLim[MAX_VALUE]/*m_modelParam.tds_test_limit.m_SEMCDMargin*/;
        SEMLim[2] = m_psConfigInfo->m_sTdscdmaInfo.m_nSemDELim[MAX_VALUE]/*m_modelParam.tds_test_limit.m_SEMDEMargin*/;
        SEMLim[3] = m_psConfigInfo->m_sTdscdmaInfo.m_nSemEDLim[MAX_VALUE]/*m_modelParam.tds_test_limit.m_SEMEDMargin*/;
        SEMLim[4] = m_psConfigInfo->m_sTdscdmaInfo.m_nSemDCLim[MAX_VALUE]/*m_modelParam.tds_test_limit.m_SEMDCMargin*/;
        SEMLim[5] = m_psConfigInfo->m_sTdscdmaInfo.m_nSemBALim[MAX_VALUE]/*m_modelParam.tds_test_limit.m_SEMBAMargin*/;

        int nIndex = 0;
        for (nIndex = 0; nIndex < 6; nIndex++)
        {
            swprintf_s(szMsgBuffer, L"SEM%sMargin @ -100@ %.2f @ %.1f", SEMMsg[nIndex],fsem[nIndex],SEMLim[nIndex]);
            if (fsem[nIndex] < SEMLim[nIndex])
            {
                SendMsgToDisplay(szMsgBuffer);
            }
            else
            {
                SendErrMsgToDisplay(szMsgBuffer);
                return false;
            }
        }

        return true;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_CheckOBW(float fOBWValue)
    {
        if ( !m_psConfigInfo->m_sTdscdmaInfo.m_bTestOBW )
        {
            return true;
        }

        wchar_t szMsgBuffer[MAX_PATH] = {0};
        float obwLimMax = 0;
        float obwLimMin = 0;

        obwLimMin = m_psConfigInfo->m_sTdscdmaInfo.m_fObwLim[MIN_VALUE];
        obwLimMax = m_psConfigInfo->m_sTdscdmaInfo.m_fObwLim[MAX_VALUE];
        fOBWValue = fOBWValue/1000000.0f;

        swprintf_s(szMsgBuffer, L"TDS OBW @%.1f@%.2f@%.1f", obwLimMin, fOBWValue, obwLimMax);

        if (fOBWValue > obwLimMin && fOBWValue<obwLimMax)
        {
            SendMsgToDisplay(szMsgBuffer);
            return true;
        }

        return false;
    }
    bool CCRFAntTestThread::TDSCDMA_NS_CheckEVM(float fEVM)
    {
        if (  !m_psConfigInfo->m_sTdscdmaInfo.m_bTestEvm )
        {
            return true;
        }

        wchar_t szMsgBuffer[MAX_PATH]={0};
        float evmLimMax = 0.0;
        float evmLimMin = 0.0;

        evmLimMin = m_psConfigInfo->m_sTdscdmaInfo.m_fEvmLim[MIN_VALUE];
        evmLimMax = m_psConfigInfo->m_sTdscdmaInfo.m_fEvmLim[MAX_VALUE];

        swprintf_s(szMsgBuffer, L"EVM@%.1f@%.2f@%.1f", evmLimMin, fEVM, evmLimMax);

        if (fEVM > evmLimMin && fEVM <=evmLimMax)
        {
            SendMsgToDisplay(szMsgBuffer);
            return true;
        }

        SendErrMsgToDisplay(szMsgBuffer);
        return false;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_CheckFreqError(float fFreqError)
    {
        if ( !m_psConfigInfo->m_sTdscdmaInfo.m_bTestFrqErr )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};

        float fFreqLimUp = 0.0;
        float fFreqLimDown = 0.0;
        fFreqLimUp   = m_psConfigInfo->m_sTdscdmaInfo.m_fFreqErrLim[MAX_VALUE];
        fFreqLimDown = m_psConfigInfo->m_sTdscdmaInfo.m_fFreqErrLim[MIN_VALUE];

        swprintf_s(buffer,L"TDS_FreqErr @%.2f@%.2f@%.2f", fFreqLimDown,fFreqError, fFreqLimUp);
        if ( fFreqLimDown < fFreqError && fFreqError < fFreqLimUp )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::TDSCDMA_NS_CheckPeakCDE(float fPCDE)
    {
        if ( !m_psConfigInfo->m_sTdscdmaInfo.m_bTestPCde)
        {
            return true;
        }

        wchar_t szMsgBuffer[MAX_PATH]={0};
        float fPeakCDELim = 0.0;

        fPeakCDELim = m_psConfigInfo->m_sTdscdmaInfo.m_fPeakCDELim[MAX_VALUE];
        swprintf_s(szMsgBuffer, L"PeakCDE @ -60 @%.2f@%.1f", fPCDE, fPeakCDELim);
        if (fPCDE < fPeakCDELim)
        {
            SendMsgToDisplay(szMsgBuffer);
            return true;
        }
        SendErrMsgToDisplay(szMsgBuffer);

        return false;
    }
    bool CCRFAntTestThread::TDSCDMA_NS_CheckPVTime(int status)
    {
        if ( !m_psConfigInfo->m_sTdscdmaInfo.m_bTestPvt )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH]={0};
        swprintf_s(buffer, L"TDS_PVTime @ %d @ %d @ %d", status, status, status);

        if (status)
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);
        return false;
    }
#endif
#endif

#ifdef WCDMA_NS_TEST_
    bool CCRFAntTestThread::WCDMA_NS_TestSequence()
    {
        SendMsgToList(L"WCDMA Start Test@1@1@1@P",true);
        bool bResult = true;
        m_bWcdmaStartCall = false;

        TEST_NS_FUNCOK( InitSubsysVFS(TECHNOLOGY_WCDMA) );
        TEST_NS_FUNCOK( WCDMA_InitMonitor() );
        bResult &= WCDMA_NS_Test();
        m_subSysVfs.GetClass()->EndCall();
        bResult &= m_pCMW500EquipmentControll->FinalizeBSE();

        return bResult;
    }

    bool CCRFAntTestThread::WCDMA_InitMonitor()
    {
        if ( !m_pCMW500EquipmentControll->WCDMA_InitBSE() || !m_pCMW500EquipmentControll->SetInOutPutPort(ERFAC,ERFAC) )
        {
            SendMsgToList(L"WCDMA 仪器初始化失败@0@失败@0@F",true);
            return false;
        }

        return true;
    }

    bool CCRFAntTestThread::WCDMA_NS_Test()
    {
        std::wstring strTemp( L"" );
        bool bResult = true;
        int nCounts = m_psConfigInfo->m_nErrorReTestCount;

        for (std::vector<S_BAND_INFO>::iterator it = m_psConfigInfo->m_sWcdmaInfo.m_sTestBandInfo.m_vBandInfo.begin();
            it != m_psConfigInfo->m_sWcdmaInfo.m_sTestBandInfo.m_vBandInfo.end();
            it ++ )
        {
            m_nBand = it[0].m_nBand;
            m_lpBandInfo = &it[0];
            nCounts = m_psConfigInfo->m_nErrorReTestCount;

            for ( std::vector<S_BAND_CHANNEL_INFO>::iterator itChannel = it->m_vBandChannelInfo.begin();
                itChannel != it->m_vBandChannelInfo.end() && nCounts > 0 ;
                nCounts--/*itChannel ++ */)
            {
                m_fPTxLoss = itChannel->m_fPTxLoss;
                m_fPRxLoss = itChannel->m_fPRxLoss;
                m_fDRxLoss = itChannel->m_fDRxLoss;

                TEST_NS_FUNCOK( WCDMA_NS_SetBandInfo(itChannel) );
                if ( !m_bWcdmaStartCall )
                {
                    TEST_NS_FUNCOK(SYS_NS_StartCall(L"WCDMA"));
                    m_bWcdmaStartCall = true;
                }
                else
                {
                    TEST_NS_FUNCOK(SYS_NS_HandOver(L"WCDMA"));
                }

                bResult = WCDMA_NS_StartTest();

                if ( bResult )
                {
                    itChannel ++;
                    nCounts = m_psConfigInfo->m_nErrorReTestCount;
                }
            }
            TEST_NS_FUNCOK(bResult);
        }

        return bResult;
    }

    bool CCRFAntTestThread::WCDMA_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel)
    {
        std::wstring strTemp( L"" );

        m_subSysVfs.GetClass()->InitBandInfo(m_nBand,itChannel->m_nUpLinkChannel,itChannel->m_nDownLinkChannel,itChannel->m_fUpLinkFreq,itChannel->m_fDownLinkFreq);
        FormatWString(strTemp,L"WCDMA Test Begin@Band=%d@Channel=%d@Frequency=%f@P",m_nBand, itChannel->m_nUpLinkChannel, itChannel->m_fUpLinkFreq );
        SendMsgToList(strTemp.c_str(),true);

        if( m_pCMW500EquipmentControll->SetGPRFBand(m_nBand) 
            && m_pCMW500EquipmentControll->SetGPRFDlUlFreq(itChannel[0]) 
            )
        {
            m_pCMW500EquipmentControll->SetTxLoss( m_fPRxLoss ) ;
            m_pCMW500EquipmentControll->SetRxLoss( m_fPTxLoss );
            return true;
        }
        SendMsgToList(L" 仪器初始化失败@1@Fail@1@F",true);
        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_StartTest()
    {
        TEST_NS_FUNCOK( WCDMA_NS_TxRfNoSignalTest() );

        TEST_NS_FUNCOK( WCDMA_NS_RxRfNoSignalTest() );

        return true;
    }

    bool CCRFAntTestThread::WCDMA_NS_TxRfNoSignalTest()
    {
        return WCDMA_NS_MeasuresAll();
    }

    bool CCRFAntTestThread::WCDMA_NS_RxRfNoSignalTest()
    {
        m_pCMW500EquipmentControll->CloseMEValuation();
        if ( m_psConfigInfo->m_sWcdmaInfo.m_bTestRxLevel )
        {
            TEST_NS_FUNCOK( WCDMA_NS_TestRxLevel(0) );
            TEST_NS_FUNCOK( WCDMA_NS_TestRxLevel(1) );
        }

        if ( !m_psConfigInfo->m_bIsTestRx || !m_psConfigInfo->m_sWcdmaInfo.m_bTestBler )
        {
            return true;
        }

        if ( !m_pCMW500EquipmentControll->SetCellPower(m_psConfigInfo->m_sWcdmaInfo.m_fBlerCellPwr) )
        {
             SendMsgToList(L"Set Cell Pwr@---@Fail@---@F",true);
            return false; 
        }

        if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort))
        {
            SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
            return false;
        }

        float fBlerErr = 0;
        wchar_t szMsg[MAX_PATH] = {0};

        if ( m_subSysVfs.GetClass()->TestBER(m_psConfigInfo->m_nErrorReTestCount,m_psConfigInfo->m_sWcdmaInfo.m_fBlerCellPwr,m_psConfigInfo->m_sWcdmaInfo.m_fBerLim[MAX_VALUE],fBlerErr,szMsg) )
        {
            wcscat_s(szMsg,MAX_PATH,L"@P");
            SendMsgToList(szMsg,true);
            return true;
        }
        wcscat_s(szMsg,MAX_PATH,L"@F");
        SendMsgToList(szMsg,true);
        return false;
    }
#if 0
    bool CCRFAntTestThread::WCDMA_NS_TestMinPwr()
    {
        if ( !m_psConfigInfo->m_sWcdmaInfo.m_bTestMinPwr )
        {
            return true;
        }

        float MinPwrHigh = m_psConfigInfo->m_sWcdmaInfo.m_fMinPwrLim[MAX_VALUE];
        float MinPwrLow = m_psConfigInfo->m_sWcdmaInfo.m_fMinPwrLim[MIN_VALUE];
        wchar_t buffer[MAX_PATH]={0};
        float fChannPwr = 0.0;

        int triggerCounter = 3;
        do 
        {
            m_subSysVfs.GetClass()->SetTargetTxPower(-550);

            if ( !m_pCMW500EquipmentControll->WCDMA_NS_MinPwr(fChannPwr) )
            {
                SendErrMsgToDisplay(L"Get WCDMA's Min Power Fail From Instrument!@0@0@0");
                return false;
            }

            swprintf_s(buffer, L"WCDMA Min Power @%.1f@%.2f@%.1f", MinPwrLow, fChannPwr, MinPwrHigh);

            if (fChannPwr > MinPwrLow && fChannPwr < MinPwrHigh)
            {
                SendMsgToDisplay(buffer);
                return true;
            }

            SendErrMsgToDisplay(buffer);

        } while (--triggerCounter, triggerCounter > 0);

        return false;
    }
#endif
    bool CCRFAntTestThread::WCDMA_NS_MeasuresAll()
    {	
        if( !m_psConfigInfo->m_sWcdmaInfo.m_bTestMaxPwr ) return true;
   
        wchar_t buffer[MAX_PATH]={0};
        W_NS_MaxPwrTestValue testValue = {0};
        m_subSysVfs.GetClass()->SetTargetTxPower(250);

        //Sleep(300 * 1000);
        if ( !m_pCMW500EquipmentControll->WCDMA_NS_MesurmentAll(testValue) )
        {
            SendMsgToList(L"Get WCDMA's MAXPowerValue Fail From Instrument!@0@Fail@0@F",true);
            return false;
        }

        return WCDMA_NS_CheckMaxPwr(testValue.maxPwr);
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckMaxPwr(float fChannalPwr)
    {
        return CheckTestResult(m_psConfigInfo->m_sWcdmaInfo.m_bTestMaxPwr,L"W_Max Power",m_psConfigInfo->m_sWcdmaInfo.m_fMaxPwrLim[MIN_VALUE],fChannalPwr,m_psConfigInfo->m_sWcdmaInfo.m_fMaxPwrLim[MAX_VALUE]);
    }
#if 0
    wchar_t ACLRmsg[4][8]  = {L"minus2",L"minus1",L"plus1",L"plus2"};
    bool CCRFAntTestThread::WCDMA_NS_CheckACLR( float fACLRValue[4] )
    {
        if( !m_psConfigInfo->m_sWcdmaInfo.m_bTestAclr )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float ACLRLim[4] = {0.0};

        ACLRLim[0] = m_psConfigInfo->m_sWcdmaInfo.m_fAclrNega32Lim[MIN_VALUE]/*m_modelParam.tds_test_limit.m_AclrNega32MMin*/;
        ACLRLim[1] = m_psConfigInfo->m_sWcdmaInfo.m_fAclrNega16Lim[MIN_VALUE]/* m_modelParam.tds_test_limit.m_AclrNega16MMin*/;
        ACLRLim[2] = m_psConfigInfo->m_sWcdmaInfo.m_fAclrPosi16Lim[MIN_VALUE];
        ACLRLim[3] = m_psConfigInfo->m_sWcdmaInfo.m_fAclrPosi32Lim[MIN_VALUE]/* m_modelParam.tds_test_limit.m_AclrPosi32MMin*/;

        for ( int i = 0 ; i < 4 ; i++ )
        {
            swprintf_s(buffer,260, L"W_ACLR%s @%.2f(dB)@%.2f(dB)@9999(dB)",ACLRmsg[i], ACLRLim[i],fACLRValue[i] );
            if ( ACLRLim[i] < fACLRValue[i] && fACLRValue[i] < 9999 )
            {
                SendMsgToDisplay(buffer);
            }
            else
            {
                SendErrMsgToDisplay(buffer);
                return false;
            }
        }
        return true;
    }

    wchar_t SEMmsg[8][3]  = {L"AB",L"BC",L"CD",L"EF",L"FE",L"DC",L"CB",L"BA"};
    bool CCRFAntTestThread::WCDMA_NS_CheckSEM( float fSEMValue[8] )
    {
        if( !m_psConfigInfo->m_sWcdmaInfo.m_bTestSem )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float SEMLim[8] = {0.0};

        SEMLim[0] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemABLim[MAX_VALUE];
        SEMLim[1] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemBCLim[MAX_VALUE];
        SEMLim[2] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemCDLim[MAX_VALUE];
        SEMLim[3] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemEFLim[MAX_VALUE];
        SEMLim[4] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemFELim[MAX_VALUE];
        SEMLim[5] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemDCLim[MAX_VALUE];
        SEMLim[6] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemCBLim[MAX_VALUE];
        SEMLim[7] = (float)m_psConfigInfo->m_sWcdmaInfo.m_nSemBALim[MAX_VALUE];

        for ( int i = 0 ; i < 8 ; i++ )
        {
            swprintf_s(buffer,L"W_SEM%s @-100db@%.2f@%.1f",SEMmsg[i], fSEMValue[i], SEMLim[i]);
            if ( -100 < fSEMValue[i] && fSEMValue[i] < SEMLim[i] )
            {
                SendMsgToDisplay(buffer);
            }
            else
            {
                SendErrMsgToDisplay(buffer);
                return false;
            }
        }
        return true;	
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckEvm( float fEvmValue )
    {
        if( !m_psConfigInfo->m_sWcdmaInfo.m_bTestEvm )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float fEVMLimMax = 0.0;
        float fEVMLimMin = 0.0;
        fEVMLimMax = m_psConfigInfo->m_sWcdmaInfo.m_fEvmLim[MAX_VALUE];
        fEVMLimMin = m_psConfigInfo->m_sWcdmaInfo.m_fEvmLim[MIN_VALUE];

        swprintf_s(buffer, L"W_EVM @%.2f@%.2f@%.2f",fEVMLimMin, fEvmValue, fEVMLimMax);
        if ( fEVMLimMin < fEvmValue && fEvmValue < fEVMLimMax )
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckFreqError( float fFreqError )
    {
        if( !m_psConfigInfo->m_sWcdmaInfo.m_bTestFrqErr )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};

        float fFreqLimUp = 0.0;
        float fFreqLimDown = 0.0;
        fFreqLimUp   = m_psConfigInfo->m_sWcdmaInfo.m_fFreqErrLim[MAX_VALUE];
        fFreqLimDown = m_psConfigInfo->m_sWcdmaInfo.m_fFreqErrLim[MIN_VALUE];

        swprintf_s(buffer,260,L"W_FreqError @%.2f@%.2f@%.2f", fFreqLimDown,fFreqError, fFreqLimUp);
        if ( fFreqLimDown < fFreqError && fFreqError < fFreqLimUp )
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckOBW( float fOBWValue )
    {
        if(!m_psConfigInfo->m_sWcdmaInfo.m_bTestOBW)
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};

        float fOBWLimMax = m_psConfigInfo->m_sWcdmaInfo.m_fObwLim[MAX_VALUE];
        float fOBWLimMin = m_psConfigInfo->m_sWcdmaInfo.m_fObwLim[MIN_VALUE];
        float fOBWValueMHZ = fOBWValue/1000000;

        swprintf_s(buffer, L"W_OBW @%.2f@%.2f@%.2f",fOBWLimMin,fOBWValueMHZ, fOBWLimMax);
        if ( fOBWLimMin < fOBWValueMHZ && fOBWValueMHZ < fOBWLimMax )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckPCDE( float fPCDEValue )
    {
        if(!m_psConfigInfo->m_sWcdmaInfo.m_bTestPCde)
        {
            return true;
        }
        wchar_t buffer[MAX_PATH] = {0};
        float fPCDELim = m_psConfigInfo->m_sWcdmaInfo.m_fPeakCDELim[MAX_VALUE];

        swprintf_s(buffer,260,L"W_PCDE @-100@%.2f@%.2f",fPCDEValue, fPCDELim);
        if ( -100 < fPCDEValue && fPCDEValue < fPCDELim )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckPhaseError( float fPhaseError )
    {
        if(!m_psConfigInfo->m_sWcdmaInfo.m_bTestPhaseError)
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float fPhaseLim = m_psConfigInfo->m_sWcdmaInfo.m_fPhaseErrRmsLim[MAX_VALUE];

        swprintf_s(buffer, L"W_PhaseErr @0@%.2f@%.2f",fPhaseError, fPhaseLim);
        if ( 0 < fPhaseError && fPhaseError < fPhaseLim )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckIQoffset( float fIQoffset )
    {
        if(!m_psConfigInfo->m_sWcdmaInfo.m_bTestIQOffset)
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float fIQOffsetLim = m_psConfigInfo->m_sWcdmaInfo.m_fIQOffsetLim[MAX_VALUE];

        swprintf_s(buffer,L"W_IQOffset @-100@%.2f@%.2f",fIQoffset, fIQOffsetLim);
        if ( -100 < fIQoffset && fIQoffset < fIQOffsetLim )
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckIQBalance( float fIQBalance )
    {
        if(!m_psConfigInfo->m_sWcdmaInfo.m_bTestIQBalance)
        {
            return true;
        }
        wchar_t buffer[MAX_PATH] = {0};
        float fIQBalanceLim = m_psConfigInfo->m_sWcdmaInfo.m_fIQBalance[MAX_VALUE];

        swprintf_s(buffer, 260,L"W_IQBalance @-100@%.2f@%.2f",fIQBalance, fIQBalanceLim);
        if ( -100 < fIQBalance && fIQBalance < fIQBalanceLim )
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);
        return false;
    }
#endif

    bool CCRFAntTestThread::WCDMA_NS_TestRxLevel(int nChain)
    {
        if ( nChain != 0)
        {
            if(ERF_UNKNOW == m_lpBandInfo->m_eDrxPort)
            {
                return true;
            }
        }

        if (!m_pCMW500EquipmentControll->SetInOutPutPort(nChain == 0?m_lpBandInfo->m_ePort:m_lpBandInfo->m_eDrxPort,m_lpBandInfo->m_ePort))
        {
            SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
            return false;
        }
        if( E_8960 == m_pCMW500EquipmentControll->GetMontiorType() )
        {
            if( m_fPRxLoss != m_fPTxLoss )
                m_pCMW500EquipmentControll->SetRxLoss(nChain == 0?m_fPRxLoss:m_fDRxLoss);
        }
        else 
        {
            m_pCMW500EquipmentControll->SetTxLoss(nChain == 0?m_fPRxLoss:m_fDRxLoss);
        }
        if ( !m_pCMW500EquipmentControll->SetCellPower(m_psConfigInfo->m_sWcdmaInfo.m_fRxLevelCellPwr) )
        {
             SendMsgToList(L"Set Cell Pwr@---@Fail@---@F",true);
            return false; 
        }
        short iRxAGC = 0,iRxLeveldBm10 = 0;
        unsigned short pStatus = 0;

        for (int i = 0;i < 20;i ++)
        {
            m_cRwnv.FTM_WCDMA_BER_RX_LEVEL_REQ(nChain,&pStatus,&iRxAGC,&iRxLeveldBm10);
            if ( 0 == pStatus && WCDMA_NS_CheckRxLevel(iRxLeveldBm10) )
            {
                return true;
            }
            Sleep(200);
        }

        return false;
    }

    bool CCRFAntTestThread::WCDMA_NS_CheckRxLevel(short iRxLeveldBm10)
    {
        float testRssi = ((float)iRxLeveldBm10)/10.0f;
        return CheckTestResult(true,L"RSSI LEVEL",m_psConfigInfo->m_sWcdmaInfo.m_fRxLevelLim[MIN_VALUE],testRssi,m_psConfigInfo->m_sWcdmaInfo.m_fRxLevelLim[MAX_VALUE]);
    }

#endif


#ifdef CDMA_NS_TEST_
    bool CCRFAntTestThread::CDMA_NS_TestSequence()
    {
        SendMsgToDisplay(L"CDMA Start Test@1@1@1");
        bool bResult = true;
        m_bCdmaStartCall = false;

        TEST_NS_FUNCOK( InitSubsysVFS(TECHNOLOGY_CDMA) );

        TEST_NS_FUNCOK( CDMA_InitMonitor() );

        bResult &= CDMA_NS_Test();

        m_subSysVfs.GetClass()->EndCall();

        bResult &= m_pCMW500EquipmentControll->FinalizeBSE();

        return bResult;
    }

    bool CCRFAntTestThread::CDMA_InitMonitor()
    {
        if ( !m_pCMW500EquipmentControll->CDMA_InitBSE() || !m_pCMW500EquipmentControll->SetInOutPutPort(ERFAC,ERFAC) )
        {
            SendMsgToDisplay(L"CDMA 仪器初始化失败@0@失败@0");
            return false;
        }

        return true;
    }

    bool CCRFAntTestThread::CDMA_NS_Test()
    {
        std::wstring strTemp( L"" );
        bool bResult = true;
        int nCounts = m_psConfigInfo->m_nErrorReTestCount;

        for (std::vector<S_BAND_INFO>::iterator it = m_psConfigInfo->m_sCdmaInfo.m_sTestBandInfo.m_vBandInfo.begin();
            it != m_psConfigInfo->m_sCdmaInfo.m_sTestBandInfo.m_vBandInfo.end();
            it ++ )
        {
            m_nBand = it[0].m_nBand;
            m_lpBandInfo = &it[0];
            nCounts = m_psConfigInfo->m_nErrorReTestCount;

            for ( std::vector<S_BAND_CHANNEL_INFO>::iterator itChannel = it->m_vBandChannelInfo.begin();
                itChannel != it->m_vBandChannelInfo.end() && nCounts > 0;
                nCounts--/*itChannel ++ */)
            {
                m_fPTxLoss = itChannel->m_fPTxLoss;
                m_fPRxLoss = itChannel->m_fPRxLoss;
                m_fDRxLoss = itChannel->m_fDRxLoss;
                TEST_NS_FUNCOK( CDMA_NS_SetBandInfo(itChannel) );
                if ( !m_bCdmaStartCall )
                {
                    TEST_NS_FUNCOK( CDMA_NS_StartCall() );
                    m_bCdmaStartCall = true;
                }
                else
                {
                    TEST_NS_FUNCOK( CDMA_NS_HandOver() );
                }

                bResult = CDMA_NS_StartTest();

                if( bResult )
                {
                    itChannel ++;
                    nCounts = m_psConfigInfo->m_nErrorReTestCount;
                }
            }
            TEST_NS_FUNCOK( bResult );
        }

        return bResult;
    }

    bool CCRFAntTestThread::CDMA_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel)
    {
        std::wstring strTemp( L"" );

        m_subSysVfs.GetClass()->InitBandInfo(m_nBand,itChannel->m_nUpLinkChannel,itChannel->m_nDownLinkChannel,itChannel->m_fUpLinkFreq,itChannel->m_fDownLinkFreq);
        FormatWString(strTemp,L"CDMA Test Begin@Band=%d@Channel=%d@Frequency=%f",m_nBand, itChannel->m_nUpLinkChannel, itChannel->m_fUpLinkFreq );
        SendMsgToDisplay(strTemp.c_str());

        if( m_pCMW500EquipmentControll->SetGPRFBand(m_nBand) 
            && m_pCMW500EquipmentControll->SetGPRFDlUlFreq(itChannel->m_fDownLinkFreq,itChannel->m_fUpLinkFreq) 
            )
        {
            m_pCMW500EquipmentControll->SetTxLoss( m_fPRxLoss ) ;
            m_pCMW500EquipmentControll->SetRxLoss( m_fPTxLoss );
            return true;
        }

        SendErrMsgToDisplay(L"仪器初始化失败@1@Fail@1");
        return false;
    }

    bool CCRFAntTestThread::CDMA_NS_StartCall()
    {
        int i = 4;
        while( i-- > 0 )
        {
            if( m_subSysVfs.GetClass()->StartCall() )
            {
                SendMsgToDisplay(L"CDMA_NS_StartCall@1@1@1");
                return true;
            }
            Sleep(1000);
        }
        SendErrMsgToDisplay(L"CDMA_NS_StartCall@1@Fail@1");
        return false;
    }

    bool CCRFAntTestThread::CDMA_NS_StartTest()
    {
        TEST_NS_FUNCOK(CDMA_NS_TxRfNoSignalTest());
        TEST_NS_FUNCOK(CDMA_NS_RxRfNoSignalTest());

        return true;
    }

    bool CCRFAntTestThread::CDMA_NS_HandOver()
    {
        int i = 4;
        while( i-- > 0 )
        {
            if( m_subSysVfs.GetClass()->HandOverChannel() )
            {
                SendMsgToDisplay(L"CDMA_NS_HandOver@1@1@1");
                return true;
            }

            Sleep(1000);
        }
        SendErrMsgToDisplay(L"CDMA_NS_HandOver@1@Fail@1");

        return false;
    }

    bool CCRFAntTestThread::CDMA_NS_TxRfNoSignalTest()
    {
        if(	!(TDSCDMA_NS_TestMinPwr() && TDSCDMA_NS_MeasuresAll()))
        {
            return false;
        }

        if( !CDMA_2000_NS_TestACPR() )
        {
            return false;
        }

        return true;
    }

    bool CCRFAntTestThread::CDMA_NS_RxRfNoSignalTest()
    {
        if ( !m_psConfigInfo->m_bIsTestRx || !m_psConfigInfo->m_sCdmaInfo.m_bTestBler)
        {
            return true;
        }
        m_pCMW500EquipmentControll->CloseMEValuation();
        if ( !m_pCMW500EquipmentControll->SetCellPower(m_psConfigInfo->m_sCdmaInfo.m_fBlerCellPwr) )
        {
             SendMsgToList(L"Set Cell Pwr@---@Fail@---@F",true);
            return false; 
        }

        if (!m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort))
        {
            SendMsgToList(L"There're no Two Sub Instruments@---@Fail@---@F",true);
            return false;
        }

        float fBlerErr = 0;
        wchar_t szMsg[MAX_PATH] = {0};

        if ( m_subSysVfs.GetClass()->TestBER(m_psConfigInfo->m_nErrorReTestCount,m_psConfigInfo->m_sCdmaInfo.m_fBlerCellPwr,m_psConfigInfo->m_sCdmaInfo.m_fBerLim[MAX_VALUE],fBlerErr,szMsg) )
        {
            SendMsgToDisplay(szMsg);
            return true;
        }

        SendErrMsgToDisplay(szMsg);
        return false;
    }

    bool CCRFAntTestThread::CDMA_NS_TestMinPwr()
    {
        if ( !m_psConfigInfo->m_sCdmaInfo.m_bTestMinPwr )
        {
            return true;
        }

        float MinPwrHigh = m_psConfigInfo->m_sCdmaInfo.m_fMinPwrLim[MAX_VALUE];
        float MinPwrLow =m_psConfigInfo->m_sCdmaInfo.m_fMinPwrLim[MIN_VALUE];
        wchar_t buffer[MAX_PATH]={0};
        float fChannPwr = 0.0;
        int triggerCounter = 3;

        do 
        {
            m_subSysVfs.GetClass()->SetTargetTxPower(-550);
            if (!m_pCMW500EquipmentControll->CDMA_NS_MinPwr(fChannPwr))
            {
                SendErrMsgToDisplay(L"Get CDMA's Min Power(1.25Mhz) Fail From Instrument!");
                return false;
            }

            swprintf_s(buffer, MAX_PATH,L"CDMA Min Power(1.25Mhz) @%.1f@%.2f@%.1f", MinPwrLow, fChannPwr, MinPwrHigh);
            if (fChannPwr > MinPwrLow && fChannPwr < MinPwrHigh)
            {
                m_subSysVfs.GetClass()->SetTargetTxPower(250);
                SendMsgToDisplay(buffer);
                return true;
            }
            else
            {
                SendErrMsgToDisplay(buffer);
            }

        } while (--triggerCounter, triggerCounter > 0);

        return false;
    }

    bool CCRFAntTestThread::CDMA_NS_MeasuresAll()
    {
        wchar_t buffer[MAX_PATH]={0};
        C_NS_MaxPwrTestValue testValue = {0};
        m_subSysVfs.GetClass()->SetTargetTxPower(280);

        if (!m_pCMW500EquipmentControll->CDMA_2000_NS_MesurmentAll(testValue))
        {
            SendErrMsgToDisplay(L"Get CDMA's MAXPowerValue Fail From Instrument!@ 0@ 0@ 0");
            return false;
        }
        else
        {
            if (     CDMA_2000_NS_CheckMaxPwr( testValue.maxPwr )
                &&	 CDMA_2000_NS_CheckEvm( testValue.evm )
                &&	 CDMA_2000_NS_CheckFreqError( testValue.freqError )
                &&   CDMA_2000_NS_CheckWFQ( testValue.wfQuality )
                &&	 CDMA_2000_NS_CheckPhaseError(  testValue.PhaseError )
                &&   CDMA_2000_NS_CheckIQBalance(  testValue.IQbalance )
                )
            {
                return true;
            }
        }

        return false;
    }

    bool CCRFAntTestThread::CDMA_2000_NS_TestACPR(void)
    {
        if( !m_psConfigInfo->m_sCdmaInfo.m_bTestAcpr )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        m_subSysVfs.GetClass()->SetTargetTxPower(280);
        int OutOfTolerance = -1;
        bool bResult = false;

        bResult = m_pCMW500EquipmentControll->CDMA_NS_ACPR(OutOfTolerance);
        swprintf_s(buffer, MAX_PATH,L"C_ACPR's OutOfTolerance@0@%d@0", OutOfTolerance);

        if ( bResult && ( 0 == OutOfTolerance ) )
        {	
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::CDMA_2000_NS_CheckMaxPwr( float fChannalPwr )
    {
        if( !m_psConfigInfo->m_sCdmaInfo.m_bTestMaxPwr )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float pwrDownLim = m_psConfigInfo->m_sCdmaInfo.m_fMaxPwrLim[MIN_VALUE];
        float pwrUpLim = m_psConfigInfo->m_sCdmaInfo.m_fMaxPwrLim[MAX_VALUE];

        swprintf_s(buffer,260,L"C_Max Power @%.1f@%.2f@%.1f", pwrDownLim, fChannalPwr, pwrUpLim);
        if ( fChannalPwr < pwrUpLim && fChannalPwr > pwrDownLim )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::CDMA_2000_NS_CheckEvm( float fEvmValue )
    {
        if( !m_psConfigInfo->m_sCdmaInfo.m_bTestEvm )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float fEVMLimMax = m_psConfigInfo->m_sCdmaInfo.m_fEvmLim[MAX_VALUE];
        float fEVMLimMin = m_psConfigInfo->m_sCdmaInfo.m_fEvmLim[MIN_VALUE];

        swprintf_s(buffer,MAX_PATH,L"C_EVM @%.2f@%.2f@%.2f",fEVMLimMin, fEvmValue, fEVMLimMax);
        if ( fEVMLimMin < fEvmValue && fEvmValue < fEVMLimMax )
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::CDMA_2000_NS_CheckFreqError( float fFreqError )
    {
        if( !m_psConfigInfo->m_sCdmaInfo.m_bTestFrqErr )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float fFreqLimUp = m_psConfigInfo->m_sCdmaInfo.m_fFreqErrLim[MAX_VALUE];
        float fFreqLimDown = m_psConfigInfo->m_sCdmaInfo.m_fFreqErrLim[MIN_VALUE];

        swprintf_s(buffer, MAX_PATH,L"C_FreqError @%.2f@%.2f@%.2f", fFreqLimDown,fFreqError, fFreqLimUp);
        if ( fFreqLimDown < fFreqError && fFreqError < fFreqLimUp )
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);
        return false;
    }

    bool CCRFAntTestThread::CDMA_2000_NS_CheckWFQ( float wfq )
    {
        if( !m_psConfigInfo->m_sCdmaInfo.m_bTestWFQ )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float minWfq = m_psConfigInfo->m_sCdmaInfo.m_fWFQLim[MIN_VALUE];

        swprintf_s(buffer, MAX_PATH,L"C_FreqError @%.2f@%.6f@1", minWfq,wfq );
        if ( minWfq < wfq && wfq < 1.0 )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);
        return false;
    }


    bool CCRFAntTestThread::CDMA_2000_NS_CheckIQBalance( float fIQBalance )
    {
        if( !m_psConfigInfo->m_sCdmaInfo.m_bTestIQBalance )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float fIQBalanceLim = m_psConfigInfo->m_sCdmaInfo.m_fIQBalance[MAX_VALUE];

        swprintf_s(buffer,260, L"C_IQBalance @-100@%.2f@%.2f",fIQBalance, fIQBalanceLim);
        if ( -100 < fIQBalance && fIQBalance < fIQBalanceLim )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::CDMA_2000_NS_CheckPhaseError( float fPhaseError )
    {
        if( !m_psConfigInfo->m_sCdmaInfo.m_bTestPhaseError )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH] = {0};
        float fPhaseLim = m_psConfigInfo->m_sCdmaInfo.m_fPhaseErrRmsLim[MAX_VALUE];

        swprintf_s(buffer,260, L"C_PhaseErr @0@%.2f@%.2f",fPhaseError, fPhaseLim);
        if ( 0 < fPhaseError && fPhaseError < fPhaseLim )
        {
            SendMsgToDisplay(buffer);
            return true;
        }

        SendErrMsgToDisplay(buffer);

        return false;
    }
#endif

#ifdef GSM_NS_TEST_
    bool CCRFAntTestThread::GSM_NS_TestSequence()
    {
        SendMsgToList(L"GSM Start Test@1@1@1@P",true);
        bool bResult = true;
        m_bGsmStartCall = false;
        TEST_NS_FUNCOK( InitSubsysVFS(GSM) );

        TEST_NS_FUNCOK( GSM_InitMonitor() );

        bResult &= GSM_NS_Test();

        m_subSysVfs.GetClass()->EndCall();

        bResult &= m_pCMW500EquipmentControll->FinalizeBSE();

        return bResult;
    }

    bool CCRFAntTestThread::GSM_InitMonitor()
    {
        if ( !m_pCMW500EquipmentControll->GSM_InitBSE() || !m_pCMW500EquipmentControll->SetInOutPutPort(ERFAC,ERFAC) )
        {
            SendMsgToList(L"GSM 仪器初始化失败@0@失败@0@F",true);
            return false;
        }

        return true;
    }

    bool CCRFAntTestThread::GSM_NS_Test()
    {
        std::wstring strTemp( L"" );
        bool bResult = true;
        int nCounts = m_psConfigInfo->m_nErrorReTestCount;

        for (std::vector<S_BAND_INFO>::iterator it = m_psConfigInfo->m_sGsmInfo.m_sTestBandInfo.m_vBandInfo.begin();
            it != m_psConfigInfo->m_sGsmInfo.m_sTestBandInfo.m_vBandInfo.end();
            it ++ )
        {
            m_nBand = it[0].m_nBand;
            m_lpBandInfo = &it[0];

            std::map<int,S_GSM_BAND_NORM_INFO>::iterator itGsm = m_psConfigInfo->m_sGsmInfo.m_mGsmBandNormInfo.find(m_nBand);
            if( itGsm == m_psConfigInfo->m_sGsmInfo.m_mGsmBandNormInfo.end() )
            {
                SendMsgToList(L"GSM Band Config@1@Fail@1@F",true);
                return false;
            }

            m_lpGsmBandPwlInfo = &itGsm->second;

            nCounts = m_psConfigInfo->m_nErrorReTestCount;

            for ( std::vector<S_BAND_CHANNEL_INFO>::iterator itChannel = it->m_vBandChannelInfo.begin();
                itChannel != it->m_vBandChannelInfo.end() && nCounts > 0;
                nCounts--/*itChannel ++ */)
            {
                m_fPTxLoss = itChannel->m_fPTxLoss;
                m_fPRxLoss = itChannel->m_fPRxLoss;
                m_fDRxLoss = itChannel->m_fDRxLoss;
                TEST_NS_FUNCOK( GSM_NS_SetBandInfo(itChannel) );
                if ( !m_bGsmStartCall )
                {
                    TEST_NS_FUNCOK(SYS_NS_StartCall(L"GSM"));
                    m_bGsmStartCall = true;
                }
                else
                {
                    TEST_NS_FUNCOK(SYS_NS_HandOver(L"GSM"));
                }

                bResult = GSM_NS_StartTest();

                if( bResult )
                {
                    itChannel ++;
                    nCounts = m_psConfigInfo->m_nErrorReTestCount;
                }
            }
            TEST_NS_FUNCOK( bResult );
        }

        return bResult;
    }

    bool CCRFAntTestThread::GSM_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel)
    {
        std::wstring strTemp( L"" );

        m_subSysVfs.GetClass()->InitBandInfo(m_nBand,itChannel->m_nUpLinkChannel,itChannel->m_nDownLinkChannel,itChannel->m_fUpLinkFreq,itChannel->m_fDownLinkFreq);
        FormatWString(strTemp,L"GSM Test Begin@Band=%d@Channel=%d@Frequency=%f@P",m_nBand, itChannel->m_nUpLinkChannel, itChannel->m_fUpLinkFreq );
        SendMsgToList(strTemp.c_str());

        if( m_pCMW500EquipmentControll->SetGPRFBand(m_nBand) 
            && m_pCMW500EquipmentControll->SetGPRFDlUlFreq(itChannel[0]) 
            )
        {
            m_pCMW500EquipmentControll->SetTxLoss( m_fPRxLoss ) ;
            m_pCMW500EquipmentControll->SetRxLoss( m_fPTxLoss );
            return true;
        }

        SendMsgToList(L"仪器初始化失败@1@Fail@1@F",true);
        return false;
    }



    bool CCRFAntTestThread::GSM_NS_StartTest()
    {
        TEST_NS_FUNCOK( GSM_NS_TxRfNoSignalTest() );

        //	TEST_NS_FUNCOK( GSM_NS_RxRfNoSignalTest() );

        return true;
    }

    bool CCRFAntTestThread::GSM_NS_TxRfNoSignalTest()
    {
        wchar_t buffer[MAX_PATH] = {0};
        int iBand = m_nBand;
        GetGsmBand(iBand);

        for (size_t nPowerLevelIndex = 0;nPowerLevelIndex < m_lpGsmBandPwlInfo->m_vGsmBandPwl.size();nPowerLevelIndex ++)
        {
            int nPowerLevel = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(nPowerLevelIndex).m_nPwrLevel;
            m_nGsmPowerLevelIndex = nPowerLevelIndex;
            swprintf_s(buffer, 260,L"GSM PowerLevel @---@PowerLevel = %d@---@P",nPowerLevel);
            SendMsgToList(buffer,true);

            if (!m_subSysVfs.GetClass()->SetTargetTxPower(nPowerLevel))
            {
                SendMsgToList(L"GSM PowerLevel@0@Switch PowerLevel Fail@0@F",true);
                return false;
            }

            if ( !GSM_NS_MeasuresAll(nPowerLevel))
            {
                return false;
            }

            if ( !GSM_NS_MeasRxLevel(nPowerLevel))
            {
                return false;
            }

            if ( !GSM_NS_MeasuresBer(nPowerLevel))
            {
                return false;
            }
        }

        return true;
    }

    bool CCRFAntTestThread::GSM_NS_RxRfNoSignalTest()
    {
        return true;
    }	

    bool CCRFAntTestThread::GSM_NS_MeasuresAll(int nPowerLevel)
    {
        int nCounts = 3;
        bool bResult = true;
        GSM_NS_TestValue testValue = {0};
        int iBand = m_nBand;
        GetGsmBand(iBand);
        wchar_t szBer[80] = {0};

        for(int x = 0;x < nCounts;x ++)
        {
            bResult = m_pCMW500EquipmentControll->GSM_MesurmentAll(nPowerLevel,m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fBlerCellPwr,testValue);

            if ( !bResult )
            {
                SendMsgToList(L"Get GSM MAXPowerValue Fail From Instrument!@ 0 @ 0 @ 0@F",true);
                return false;
            }
            else
            {
                if (GSM_NS_CheckPower(testValue.fPowerlevel))
                {
                    return true;
                }
            }
            Sleep(500);
        }

        return false;
    }

    const wchar_t  szGSMTestInfo[][40] = {L"G_Max Power @%.1f@%.2f@%.1f",L"FreqError @%.1f@%.2f@%.1f",L"EVM @%.1f@%.2f@%.1f",L"Phase Error RMS @%.1f@%.2f@%.1f",L"Phase Error Peak @%.1f@%.2f@%.1f",L"PvtTime @1@%d@1",L"Spectrum Switching @1@%d@1",L"Spectrum Modulation @1@%d@1",L""};

    bool CCRFAntTestThread::GSM_NS_CheckPower(float fMaxPwr)
    {
        return CheckTestResult(m_lpGsmBandPwlInfo->m_bTestPwrLevel,L"G_Max Power",m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fPwrLim[MIN_VALUE],fMaxPwr,m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fPwrLim[MAX_VALUE]);
    }
#if 0
    bool CCRFAntTestThread::GSM_NS_CheckFreqError(float fFreqErr)
    {
        if (  !m_lpGsmBandPwlInfo->m_bTestFrqErr )
        {
            return true;
        }

        float pwrUpLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fFreqErrLim[MAX_VALUE];
        float pwrDownLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fFreqErrLim[MIN_VALUE];
        wchar_t buffer[MAX_PATH]={0};

        swprintf_s(buffer, szGSMTestInfo[GSM_TestFreqError], pwrDownLim, fFreqErr, pwrUpLim);
        if (fFreqErr > pwrDownLim && fFreqErr < pwrUpLim)
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::GSM_NS_CheckEvm(float fEvm)
    {
        if (  !m_lpGsmBandPwlInfo->m_bTestEvm )
        {
            return true;
        }

        float pwrUpLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fEvmLim[MAX_VALUE];
        float pwrDownLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fEvmLim[MIN_VALUE];
        wchar_t buffer[MAX_PATH]={0};

        swprintf_s(buffer, szGSMTestInfo[GSM_TestEvm], pwrDownLim, fEvm, pwrUpLim);
        if (fEvm > pwrDownLim && fEvm < pwrUpLim)
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::GSM_NS_CheckPhaseErrorRMS(float fPhaseError)
    {
        if (  !m_lpGsmBandPwlInfo->m_bTestPhaseError )
        {
            return true;
        }

        float pwrUpLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fPhaseErrRmsLim[MAX_VALUE];
        float pwrDownLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fPhaseErrRmsLim[MIN_VALUE];
        wchar_t buffer[MAX_PATH]={0};

        swprintf_s(buffer, szGSMTestInfo[GSM_TestPhaseErrorRMS], pwrDownLim, fPhaseError, pwrUpLim);

        if (fPhaseError > pwrDownLim && fPhaseError < pwrUpLim)
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::GSM_NS_CheckPhaseErrorPeak(float fPhasePeak)
    {
        if (  !m_lpGsmBandPwlInfo->m_bTestPhaseErrorPeak )
        {
            return true;
        }

        float pwrUpLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fPhaseErrPeakLim[MAX_VALUE];
        float pwrDownLim = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fPhaseErrPeakLim[MIN_VALUE];
        wchar_t buffer[MAX_PATH]={0};

        swprintf_s(buffer, szGSMTestInfo[GSM_TestPhaseErrorPeak], pwrDownLim, fPhasePeak, pwrUpLim);
        if (fPhasePeak > pwrDownLim && fPhasePeak < pwrUpLim)
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::GSM_NS_CheckPVTime(bool bPvt)
    {
        if (  !m_lpGsmBandPwlInfo->m_bTestPVT )
        {
            return true;
        }
        wchar_t buffer[MAX_PATH]={0};
        swprintf_s(buffer, szGSMTestInfo[GSM_TESTTestPVtime],bPvt?1:0);

        if ( bPvt )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::GSM_NS_CheckSpectrumSwitching(bool bSpmSwitch)
    {
        if (  !m_lpGsmBandPwlInfo->m_bTestSpectrumSwitch )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH]={0};
        swprintf_s(buffer, szGSMTestInfo[GSM_TestSpectrumSwitching], bSpmSwitch?1:0);

        if ( bSpmSwitch )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

    bool CCRFAntTestThread::GSM_NS_CheckSpectrumModulation(bool bSpmModulation)
    {
        if (  !m_lpGsmBandPwlInfo->m_bTestSpectrumModulation )
        {
            return true;
        }

        wchar_t buffer[MAX_PATH]={0};

        swprintf_s(buffer, szGSMTestInfo[GSM_TestSpectrumModulation], bSpmModulation?1:0);
        if ( bSpmModulation )
        {
            SendMsgToDisplay(buffer);
            return true;
        }
        SendErrMsgToDisplay(buffer);

        return false;
    }

#endif
    bool CCRFAntTestThread::GSM_NS_MeasuresBer(int nPowerLevel )
    {
        if (!m_lpGsmBandPwlInfo->m_bTestBler) return true;

        int iBand = m_nBand;
        GetGsmBand(iBand);

        float fber = 0.00f;
        wchar_t szBer[80] = {0};
        float nCellPower = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fBlerCellPwr;

        swprintf_s(szBer,80,L"Ber (power = %.1f) ",nCellPower);
        wcscat_s(szBer,80,L"@0%%@%.2f%%@%.2f%%@%s");

        for (int nTestIndex = 0;nTestIndex < 10;nTestIndex ++)
        {
            m_pCMW500EquipmentControll->GSM_MesurmentBer(nCellPower,131,fber);
            if ( GSM_NS_CheckBer(fber,m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fBerLim[MAX_VALUE],szBer) )
            {
                return true;
            }

            Sleep(1000);
        }

        return false;
    }

    bool CCRFAntTestThread::GSM_NS_CheckBer(float fTestValue,float fupLimit,const wchar_t * szBuffer)
    {
        bool bResult = (fTestValue < fupLimit && fTestValue >= 0);
        std::wstring strTemp;

        JGW_FormatWString(strTemp,szBuffer,fTestValue,fupLimit,bResult?L"P":L"F");
        SendMsgToList(strTemp.c_str(),true);

        return bResult;
    }


    bool CCRFAntTestThread::GSM_NS_MeasRxLevel( int nPowerLevel )
    {
        int iBand = m_nBand;
        GetGsmBand(iBand);

        if ( !m_lpGsmBandPwlInfo->m_bTestRxLevel )
        {
            return true;
        }

        float fber = 0.00f;
        float fCellPower = m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fRxLevelPwr;
        float fRxLevel = 0;
        m_pCMW500EquipmentControll->CloseMEValuation();

        if ( !( m_pCMW500EquipmentControll->SetInOutPutPort(m_lpBandInfo->m_ePort,m_lpBandInfo->m_ePort) && m_pCMW500EquipmentControll->SetCellPower( fCellPower ) ) )
        {
            SendMsgToList(L"RxLevelCellPowerAndPort@0@Fail@0@F",true);
            return false;
        }

        if( E_8960 == m_pCMW500EquipmentControll->GetMontiorType() )
        {
            m_pCMW500EquipmentControll->SetRxLoss(m_fPRxLoss);
        }
        else 
        {
            m_pCMW500EquipmentControll->SetTxLoss(m_fPRxLoss);
        }

        for (int nTestIndex = 0;nTestIndex < 10;nTestIndex ++)
        {
            if ( GSM_NS_GetRxLevel(fRxLevel,m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fRxLevelLim[MAX_VALUE],m_lpGsmBandPwlInfo->m_vGsmBandPwl.at(m_nGsmPowerLevelIndex).m_fRxLevelLim[MIN_VALUE]) )
            {
                return true;
            }
            Sleep(500);
        }
        SendMsgToList(L"GetRxLevelFunctions@0@Fail@0@F",true);
        return false;
    }

    bool CCRFAntTestThread::GSM_NS_GetRxLevel(float &fRxLevel,float fRxLevelup,float fRxLevelLow)
    {
        unsigned short iStatus = 0;
        unsigned long iRSSIValue = 0;
        short iRxPwr_in16dBm = 0;
        wchar_t buffer[MAX_PATH] = {0};
        wchar_t szRxLevel[80] = {0};

        if( E_8960 == m_pCMW500EquipmentControll->GetMontiorType() )
        {
            if( m_fPRxLoss != m_fPTxLoss )
                m_pCMW500EquipmentControll->SetRxLoss(m_fPRxLoss);
        }
        else 
        {
            m_pCMW500EquipmentControll->SetTxLoss(m_fPRxLoss);
        }
        CHECKFUNRES(m_cRwnv.IsMobileConnected());
        CHECKFUNRES(m_cRwnv.FTM_GSM_BER_RX_LEVEL_REQ(&iStatus,&iRSSIValue,&iRxPwr_in16dBm));
        CHECKFUNRES(iStatus == 0);
        fRxLevel = iRxPwr_in16dBm/16.00f;
        Log4WD_F(L"QLIB_FTM_GSM_BER_RX_LEVEL_REQ(iRSSI = %ld iRxPwr_in16dBm = %d )",iRSSIValue,iRxPwr_in16dBm);

        bool bResult = (fRxLevel < fRxLevelup && fRxLevel >= fRxLevelLow);
        swprintf_s(szRxLevel,80,L"RxLevel (power = %.1f) ",(fRxLevelup + fRxLevelLow)/2);
        wcscat_s(szRxLevel,80,L"@%.2f@%.2f@%.2f@%s");
        swprintf_s(buffer,260,szRxLevel, fRxLevelLow,fRxLevel, fRxLevelup,bResult?L"P":L"F");

        return bResult;
    }
#endif

#ifdef WLAN_NS_TEST_
    void CCRFAntTestThread::CloseWlanBatProcess()
    {
        JGW_TerminateProcess(mhWlanBatProcess);
    }

    bool CCRFAntTestThread::LoadWiFiDut()
    {
        SendMsgToList(_T("Load WiFi Dut@---@---@---@W"),true);
        for (int i = 0;i < 20;i ++,Sleep(1000))
        {
            if (mWiFiQmsl.LoadWiFiDut(m_psConfigInfo->m_sWlanInfo.mstrBoardDataFilePath.c_str()))
            {
                SendMsgToList(_T("Load WiFi Dut@---@---@---@P"));
                return true;
            }
        }
        SendMsgToList(_T("Load WiFi Dut@---@FAIL@---@F"));

        return false;
    }

    bool CCRFAntTestThread::InitWlanEquipment()
    {
        if (m_pCMW500EquipmentControll->ResetMode() && m_pCMW500EquipmentControll->WLAN_InitMode())
        {
            SendMsgToList(_T("Init Wlan Instrument@---@OK@---@P"),true);
            return true;
        }
        SendMsgToList(_T("Init Wlan Instrument@---@FAIL@---@F"),true);
        return false;
    }

    bool CCRFAntTestThread::InitWlanModuleView()
    {
		SendMsgToList(_T("Init Wlan Module@---@正在初始化....@---@W"),true);
		for (int i = 0;i < 3;i ++)
		{
			mhWlanBatProcess = JGW_CreateProcess(m_psConfigInfo->m_sWlanInfo.mstrWlanBatFilePath.c_str(),m_psConfigInfo->m_sWlanInfo.mstrWlanBatDir.c_str(),STARTF_USESHOWWINDOW,SW_HIDE);
			//! 等待Bat文件运行成功
			Sleep(3000);
			if (JGW_CheckProcessIsRun(mhWlanBatProcess))
			{
				SendMsgToList(_T("Init Wlan Module@---@PASS@---@P"));
				return true;
			}
		}
		SendMsgToList(_T("Init Wlan Module@---@FAIL@---@F"));

        return false;
    }

    bool CCRFAntTestThread::InitWlanInfoToBand(int band)
    {
        switch (band)
        {
        case 1:
            meWlanMode = E_B_WLAN_MODE;
            meWlanHtMode = E_WLAN_CCK;
            meWlanTxDataRate = E_RATE_11MBPS_S_CCK;
            meWlanRxDataRate = E_RX_RATE_11B_SHORT_11_MBPS_CCK;
            break;
        case 2:
            meWlanMode = E_G_WLAN_MODE;
            meWlanHtMode = E_WLAN_NO_HT_DSRC10;
            meWlanTxDataRate = E_RATE_54MBPS_NO_HT;
            meWlanRxDataRate = E_RX_RATE_11A_54_MBPS_NO_HT;
            break;
        case 3:
            meWlanMode = E_N_WLAN_MODE;
            meWlanHtMode = E_WLAN_HT20;
            meWlanTxDataRate = E_RATE_MCS_7_20_HT20;
            meWlanRxDataRate = E_RX_RATE_11N_HT20_MCS7;
            break;
        case 4:
            meWlanMode = E_AC_WLAN_MODE;
            meWlanHtMode = E_WLAN_VHT20;
            meWlanTxDataRate = E_RATE_AC_MCS_7_20_VHT20;
            meWlanRxDataRate = E_RX_RATE_11AC_HT20_MCS7;
            break;
        default:
            return false;
        }
        return true;
    }

    bool CCRFAntTestThread::WLAN_NS_TestSequence()
    {
        bool bResult = false;

        SendMsgToList(L"Wlan Start Test@1@1@1@P",true);
        m_pCMW500EquipmentControll->SetInstrumentSys(E_WLAN_NAME);
        m_eSys = TECHNOLOGY_WLAN;
        //! 初始化仪器以及设备
        if (!InitWlanEquipment() || !InitWlanModuleView() || !ReConnectDiag()) goto ERROR_RESULT;

        mWiFiQmsl.SetResourceContex(m_hResourceContext);
        //! 加载WIFI DUT
		if (!LoadWiFiDut()) goto ERROR_RESULT;

        bResult = WLAN_NS_Test();

        bResult &= m_pCMW500EquipmentControll->FinalizeBSE();
ERROR_RESULT:
        mWiFiQmsl.UnLoadWiFiDut();
        CloseWlanBatProcess();
        return bResult;
    }

    bool CCRFAntTestThread::WLAN_NS_Test()
    {
        for (std::vector<S_BAND_INFO>::iterator it = m_psConfigInfo->m_sWlanInfo.m_sTestBandInfo.m_vBandInfo.begin();
            it != m_psConfigInfo->m_sWlanInfo.m_sTestBandInfo.m_vBandInfo.end();
            it ++ )
        {
            m_lpBandInfo = &it[0];
            mpsWlanBandNormInfo = &m_psConfigInfo->m_sWlanInfo.m_mWlanBandNormInfo[m_lpBandInfo->m_nBand];
            //! 根据WIFI制式初始化WIFI相关信息
            InitWlanInfoToBand(m_lpBandInfo->m_nBand);

            for (std::vector<S_BAND_CHANNEL_INFO>::iterator itChannel = it->m_vBandChannelInfo.begin();
                itChannel != it->m_vBandChannelInfo.end();
                itChannel ++ )
            {
                //! 初始化参数
                mpsWlanChannelNormInfo = &mpsWlanBandNormInfo->mvWlanChannelInfo.at(itChannel - it->m_vBandChannelInfo.begin());
                m_fPTxLoss = itChannel->m_fPTxLoss;
                m_fPRxLoss = itChannel->m_fPRxLoss;
                m_fDRxLoss = itChannel->m_fDRxLoss;
                mnFrequency = itChannel->m_nDownLinkChannel;

                if (!WiFiFrequencyTest()) return false;
            }
        }
        return true;
    }

    bool CCRFAntTestThread::WiFiFrequencyTest()
    {
        bool bResult = true;
        //! 测试天线1
        if (ERF_UNKNOW != m_lpBandInfo->m_ePort)
        {
            for (int i = 0 ;i < m_psConfigInfo->m_nErrorReTestCount;i ++)
            {
                bResult = WiFiFrequencyTestToChain(1);
                if (bResult) break;
            }
            if (!bResult) return false;
        }
        
        //! 是否测试天线2
        if (ERF_UNKNOW == m_lpBandInfo->m_eDrxPort) return true;

        //! 测试天线2
        for (int i = 0 ;i < m_psConfigInfo->m_nErrorReTestCount;i ++)
        {
            bResult = WiFiFrequencyTestToChain(2);
            if (bResult) break;
        }

        return bResult;
    }

    bool CCRFAntTestThread::WiFiFrequencyTestToChain(int nTxChain)
    {
        if (1 == nTxChain)
        {
            mnTxChain = nTxChain;
            mfWlanLoss = m_fPTxLoss;
            meWlanPort = m_lpBandInfo->m_ePort;
        }
        else
        {
            mnTxChain = 2;
            meWlanPort = m_lpBandInfo->m_eDrxPort;
            mfWlanLoss = m_fDRxLoss;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"WiFi_%s TxChain: %d@--@%d@--@P",gszWlanModeName[m_lpBandInfo->m_nBand - 1],mnTxChain,mnFrequency);
        SendMsgToList(strTemp.c_str(),true);
        //! 测试TX
        if (!WLAN_Frequency_Tx_Test())
        {
            DisableWiFiTxTestDevice();
            return false;
        }

        if (1 == nTxChain) mfWlanLoss = m_fPRxLoss;
        //! 测试RX
        if (!WLAN_Frequency_Rx_Test())
        {
            return false;
        }

        return true;
    }

    bool CCRFAntTestThread::WLAN_Frequency_Tx_Test()
    {
        //! 初始化仪器
        InitWiFiTxTestEquipment();
        if (!mpsWlanBandNormInfo->m_bTestPwrLevel) return true;
        //! 初始化设备
        return InitWiFiTxTestDevice() && CheckWiFiTxTestData();
    }

    bool CCRFAntTestThread::WLAN_Frequency_Rx_Test()
    {
        if (!m_psConfigInfo->m_bIsTestRx || (!mpsWlanBandNormInfo->m_bTestRxLevel && !mpsWlanBandNormInfo->m_bTestBler)) return true;

        m_pCMW500EquipmentControll->SetTxLoss(mfWlanLoss);
        m_pCMW500EquipmentControll->SetTxOutPutPort(meWlanPort);
        if (!m_pCMW500EquipmentControll->WLAN_InitRxTest(mnFrequency,mpsWlanChannelNormInfo->m_fRxCellPwr,meWlanRxDataRate))
        {
            SendMsgToList(_T("Init WiFi Rx Equipment@---@FAIL@---@F"),true);
            return false;
        }

        if (!mWiFiQmsl.StartWiFiRx(mnTxChain,mnFrequency,meWlanHtMode,meWlanRxDataRate))
        {
            SendMsgToList(_T("Init WiFi Rx Device@---@FAIL@---@F"),true);
            m_pCMW500EquipmentControll->WLAN_CloseRxTest();
            return false;
        }

        Sleep(1000);

        float rssi = 0;
        int txbTotalPackets = 0,txbGoodPackets = 0;
        if (!mWiFiQmsl.GetWiFiRxData(rssi,txbTotalPackets,txbGoodPackets))
        {
            SendMsgToList(_T("Get WiFi Rx Data@---@FAIL@---@F"),true);
            m_pCMW500EquipmentControll->WLAN_CloseRxTest();
            return false;
        }

        m_pCMW500EquipmentControll->WLAN_CloseRxTest();

        if (mpsWlanBandNormInfo->m_bTestRxLevel 
            && (!CheckWiFiRxRssi(rssi,mpsWlanChannelNormInfo->m_fRxLevelLim[MIN_VALUE],mpsWlanChannelNormInfo->m_fRxLevelLim[MAX_VALUE])))
        {
            return false;
        }

        if ( mpsWlanBandNormInfo->m_bTestBler
            && !CheckWiFiPER(txbGoodPackets,txbTotalPackets,mpsWlanChannelNormInfo->m_fBer))
        {
            return false;
        }
        return true;
    }

    bool CCRFAntTestThread::CheckWiFiRxRssi(float value,float minValue,float maxValue)
    {
        std::wstring strTemp;
        bool bResult = (value >= minValue && value <= maxValue);

        JGW_FormatWString(strTemp,L"WiFi Rssi(%.2f)@%.2f@%.2f@%.2f@%s",mpsWlanChannelNormInfo->m_fRxCellPwr,minValue,value,maxValue,bResult?L"P":L"F");
        SendMsgToList(strTemp.c_str(),true);

        return bResult;
    }

    bool CCRFAntTestThread::CheckWiFiPER(int txbGoodPackets,int txbTotalPackets,float fPercentValue)
    {
        std::wstring strTemp;
        int txFailPackets = txbTotalPackets - txbGoodPackets;
        float fPerValue = (float)txFailPackets * 100.00f/ (float) txbTotalPackets;
        bool bResult = (fPerValue >= 0 && fPerValue <= fPercentValue);

       //! JGW_FormatWString(strTemp,L"WiFi PER(%.2f recv=%d,total=%d)@0%%@%.2f%%@%.2f%%@%s",mpsWlanChannelNormInfo->m_fRxCellPwr,txbGoodPackets,txbTotalPackets,fPerValue,fPercentValue,bResult?L"P":L"F");
        JGW_FormatWString(strTemp,L"WiFi PER(cell=%.2f)@0%%@%.2f%%@%.2f%%@%s",mpsWlanChannelNormInfo->m_fRxCellPwr,fPerValue,fPercentValue,bResult?L"P":L"F");
        SendMsgToList(strTemp.c_str(),true);

        return bResult;
    }

    bool CCRFAntTestThread::InitWiFiTxTestEquipment()
    {
        m_pCMW500EquipmentControll->SetRxLoss(mfWlanLoss);
        m_pCMW500EquipmentControll->SetRxInPutPort(meWlanPort);
        m_pCMW500EquipmentControll->WLAN_SetExpectedPowerAndUserMarg(mpsWlanChannelNormInfo->m_fExportPower,mpsWlanChannelNormInfo->m_nPwrLevel - mpsWlanChannelNormInfo->m_fPwrLim[MIN_VALUE] + 2);
        m_pCMW500EquipmentControll->WLAN_SetWlanMode(meWlanHtMode);
        m_pCMW500EquipmentControll->WLAN_SetFrequency(mnFrequency);
        return true;
    }

    bool CCRFAntTestThread::CheckWiFiTxTestData()
    {
        float fModuleData[20] = {0};
        float fSpeMask[20] = {0};
        float fPower = 0.00;
        //! 延时等待切换稳定
        Sleep(200);
        //! 获取测试数据
        if (!m_pCMW500EquipmentControll->WLAN_GetTestResult(meWlanMode,fModuleData,fSpeMask))
        {
            SendMsgToList(_T("Get WiFi Tx Test Data@---@FAIL@---@F"),true);
            return false;
        }

        if (E_B_WLAN_MODE == meWlanMode || E_AC_WLAN_MODE == meWlanMode)
        {
            fPower = fModuleData[3];
        }
        else if (E_G_WLAN_MODE == meWlanMode || E_N_WLAN_MODE == meWlanMode)
        {
            fPower = fModuleData[2];
        }
        return (CheckTxPower(mpsWlanChannelNormInfo->m_fPwrLim[MIN_VALUE],mpsWlanChannelNormInfo->m_fPwrLim[MAX_VALUE],fPower));
    }

    bool CCRFAntTestThread::CheckTxPower(float fMinPower,float fMaxPower,float fPower)
    {
        std::wstring strTemp;
        bool bResult = (fPower >= fMinPower && fPower <= fMaxPower);

        JGW_FormatWString(strTemp,L"Tx Power@%.2f dBm@%.2f dBm@%.2f dBm@%s",fMinPower,fPower,fMaxPower,bResult?L"P":L"F");
        SendMsgToList(strTemp.c_str(),true);

        return bResult;
    }

    bool CCRFAntTestThread::InitWiFiTxTestDevice()
    {
        if (!mWiFiQmsl.StartWiFiTx(mnTxChain,mpsWlanChannelNormInfo->m_nPwrLevel,mnFrequency,meWlanHtMode,meWlanTxDataRate))
        {
            SendMsgToList(_T("Start WiFi Tx Device@---@FAIL@---@F"),true);
            return false;
        }
        return true;
    }

    bool CCRFAntTestThread::DisableWiFiTxTestDevice()
    {
        if (!mWiFiQmsl.DisableWiFiTx())
        {
            SendMsgToList(_T("Disbale WiFi Tx Device@---@FAIL@---@F"),true);
            return false;
        }
        return true;
    }

#endif
    bool CCRFAntTestThread::ReadSNView()
    {
        SendMsgToList(_T("读取设备SN@---@正在读取设备SN..@---@W"),true);
        if( !m_cRwnv.ReadSN(m_strSN,14) )
        {
            SendMsgToList( _T("读取设备SN@---@读取设备SN失败@---@F"));
            return false;
        }
        FormatWString(m_strErrorCode,_T("读取设备SN@---@%s@---@P"),m_strSN.c_str());
        SendMsgToList(m_strErrorCode.c_str());
        m_strErrorCode = L"";

        return true;
    }

    bool CCRFAntTestThread::CheckDbSNView()
    {
        m_bNotNextStation = false;
        SendMsgToList(_T("Check MES SN Status@---@---@---@W"),true);

        if (GetMESDataBaseAdo()->CheckSNStatus(JGW_W2A(m_strSN)))
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,_T("Check MES SN Status@---@%s@---@P"),m_strSN.c_str());
            SendMsgToList(strTemp.c_str());
            return true;
        }

        if (IDOK == MessageBox(m_hMainWnd,L"当前号段下一站点不属于当前测试站点\n重复测试请点击确定按钮(不会过MES站点)",L"当前号段不属于当前站点",MB_OKCANCEL))
        {
            m_bNotNextStation = true;
            return true;
        }

        PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
        SendMsgToList(_T("Check MES SN Status@---@Fail@---@F"));
        return false;
    }

    bool CCRFAntTestThread::UpdateDeviceSnStationInfoView()
    {
        if (m_bNotNextStation) return true;

        SendMsgToList(_T("SN Next Station@---@---@---@W"),true);
        if (!GetMESDataBaseAdo()->NextStationToSN(JGW_W2A(m_strSN)))
        {
			PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            SendMsgToList(_T("SN Next Station@---@Fail@---@F"));
            return false;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("SN Next Station@---@%s@---@P"),m_strSN.c_str());
        SendMsgToList(strTemp.c_str());
        return true;
    }

    bool CCRFAntTestThread::MESSaveTestRecords(bool bTestStatus)
    {
        if (m_bNotNextStation) return true;
		if (m_strSN.empty()) return true;
        mstrEndTestTime = JGW_W2A(JGW_GetCurrentTimeStamp());
        SendMsgToList(_T("MES SaveTestRecords@---@---@---@W"),true);
        if (!GetMESDataBaseAdo()->SaveTestRecordsMES(JGW_W2A(m_strSN),mstrStartTestTime,mstrEndTestTime,bTestStatus?"PASS":"FAIL","","ADD"))
        {
			PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            SendMsgToList(_T("MES SaveTestRecords@---@Fail@---@F"));
            return false;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("MES SaveTestRecords@---@%s@---@P"),m_strSN.c_str());
        SendMsgToList(strTemp.c_str());
        return true;
    }
}