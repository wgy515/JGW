#include "StdAfx.h"
#include "TSE_RFQualcommInterface.h"
#import "QSPR3xttParse.tlb" no_namespace named_guids
#include <Shlwapi.h>
#include "TSE_RFCalVFSConfig.h"
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>

namespace JGW
{
    CTSE_RFQualcommInterface::CTSE_RFQualcommInterface(PS_RFQUALCOMM_INTERFACE_INFO psRFQcommInterInfo) : mpsRFQcommInterInfo(psRFQcommInterInfo)
        ,m_nCurrentIndex(0),mbFristLoad(false)
    {
    }

    CTSE_RFQualcommInterface::~CTSE_RFQualcommInterface(void)
    {
        FreeRFCalVertestDll();
    }

    void CTSE_RFQualcommInterface::FreeRFCalVertestDll()
    {
        if(NULL != mpsRFQcommInterInfo && NULL != mpsRFQcommInterInfo->m_iface)
        {
//             mpsRFQcommInterInfo->m_sti_func_decl.sti_clear_equip_config(mpsRFQcommInterInfo->m_iface);
//             mpsRFQcommInterInfo->m_sti_func_decl.sti_clear_rf_net_loss(mpsRFQcommInterInfo->m_iface);
//             mpsRFQcommInterInfo->m_sti_func_decl.sti_clear_station_config(mpsRFQcommInterInfo->m_iface);
//             mpsRFQcommInterInfo->m_sti_func_decl.sti_clear_test_db(mpsRFQcommInterInfo->m_iface);
            mpsRFQcommInterInfo->m_sti_func_decl.sti_release_suite_interface(&mpsRFQcommInterInfo->m_iface);
            mpsRFQcommInterInfo->m_iface = NULL;
        }
        if (mpsRFQcommInterInfo->m_dll_ptr)
        {
            //FreeLibrary(mpsRFQcommInterInfo->m_dll_ptr);
            mpsRFQcommInterInfo->m_dll_ptr = NULL;
        }
    }

    //! 加载校准DLL
    HMODULE CTSE_RFQualcommInterface::LoadRFCalVerTestDll()
    {
        std::wstring strpath = PathFindFileName(mpsRFQcommInterInfo->mstrRFCalQSEQDllFilePath.c_str());
        mpsRFQcommInterInfo->m_dll_ptr = GetModuleHandle(strpath.c_str());
        //! 如果已经加载
        if(mpsRFQcommInterInfo->m_dll_ptr) return mpsRFQcommInterInfo->m_dll_ptr;
        mbFristLoad = true;
        return (mpsRFQcommInterInfo->m_dll_ptr = LoadLibrary(mpsRFQcommInterInfo->mstrRFCalQSEQDllFilePath.c_str()));
    }
    //! 初始化校准接口文件
    bool CTSE_RFQualcommInterface::LoadQCServerModule()
    {
        if(mpsRFQcommInterInfo->m_dll_ptr && mpsRFQcommInterInfo->m_iface) return true;
        //! STL本地化设置切回默认值
        std::locale::global(std::locale("C"));

        if(!LoadRFCalVerTestDll())
        {
            LogE_F(L"加载DLL失败(%s),Error:%s",mpsRFQcommInterInfo->mstrRFCalQSEQDllFilePath.c_str(),JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }

        if(!sti_initialize_export_decl(mpsRFQcommInterInfo->m_dll_ptr,&mpsRFQcommInterInfo->m_sti_func_decl))  // Unload DLL if interfaces are not resolved
        {
            FreeRFCalVertestDll();
            LogE_F(L"初始化校准Server函数失败,Error:%s",JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }
        return (NULL != (mpsRFQcommInterInfo->m_iface = mpsRFQcommInterInfo->m_sti_func_decl.sti_create_suite_interface())); 
    }

    void CTSE_RFQualcommInterface::InitQCServerInterface(unsigned char bUseQPST)
    {
        // Check and set phone mode
        if (mbFristLoad) mpsRFQcommInterInfo->m_sti_func_decl.sti_clear_test_db(mpsRFQcommInterInfo->m_iface);
        mpsRFQcommInterInfo->m_sti_func_decl.sti_set_log_filename(mpsRFQcommInterInfo->m_iface,mpsRFQcommInterInfo->m_strXttPath.c_str());
        // SUITE default is to use QPST, override default if QPhoneMS.
        mpsRFQcommInterInfo->m_sti_func_decl.sti_add_station_config_item(mpsRFQcommInterInfo->m_iface,2,"QMSLLibraryMode",bUseQPST?"1":"0");
        //! 注册Log信息
        mpsRFQcommInterInfo->m_sti_func_decl.sti_register_system_event_handler(mpsRFQcommInterInfo->m_iface,this,DutTestEventHandler);

        mpsRFQcommInterInfo->m_sti_func_decl.sti_set_environment_var(mpsRFQcommInterInfo->m_iface, "XTT_TIMEOUT", XTT_TIMEOUT);
        mpsRFQcommInterInfo->m_sti_func_decl.sti_set_environment_var(mpsRFQcommInterInfo->m_iface,  "XTT_TIMEOUT", XTT_TIMEOUT);
        mpsRFQcommInterInfo->m_sti_func_decl.sti_set_environment_var(mpsRFQcommInterInfo->m_iface,  "XTT_TIMEOUT_OFFSET", XTT_TIMEOUT_OFFSET);
        mpsRFQcommInterInfo->m_sti_func_decl.sti_set_environment_var(mpsRFQcommInterInfo->m_iface, "XTT_TIMEOUT_OFFSET", XTT_TIMEOUT_OFFSET); 


        //! 设置仪器端口地址
//         m_sti_func_decl.sti_add_gpib_equip_config_item(m_iface,"Call_Processor1",const_cast<char*>(strEquipName.c_str()),"CP",const_cast<char*>(strEquipPad.c_str()),"96","");
//         m_sti_func_decl.sti_add_gpib_equip_config_item(m_iface,"Signal_Generator4",const_cast<char*>(strEquipName.c_str()),"DSG",const_cast<char*>(strEquipPad.c_str()),"96","");

        //! IP地址
        //m_sti_func_decl.sti_add_ip_equip_config_item(m_iface,"Call_Processor1",const_cast<char*>(strEquipName.c_str()),"CP",const_cast<char*>(strEquipPad.c_str()),"");
        //m_sti_func_decl.sti_add_ip_equip_config_item(m_iface,"Signal_Generator4",const_cast<char*>(strEquipName.c_str()),"DSG",const_cast<char*>(strEquipPad.c_str()),"");

        //EQUIPCONFIGPATH
        //m_sti_func_decl.sti_set_environment_var(m_iface, "EQUIPCONFIGPATH", "C:\\Program Files (x86)\\Qualcomm\\QDART\\Databases");
        //m_sti_func_decl.sti_set_environment_var(m_iface, "TesterType", "0");

        //disableDebugOut
          
    }
    //! 加载Xtt文件
    bool CTSE_RFQualcommInterface::LoadRFCalVerTestConfig()
    {
        m_vDestory.clear();
        size_t nIndex = mpsRFQcommInterInfo->m_strXttPath.find_last_of('.');
        if(std::string::npos != mpsRFQcommInterInfo->m_strXttPath.find(".xml",nIndex)) return LoadRFCalVerTestXMLConfig(mpsRFQcommInterInfo->m_strXttPath);
        //! 以XTT方式加载测试XTT
        SUCCEEDED(CoInitialize(NULL));
        _QSPR3XTTPtr parser(__uuidof(QSPR3XTT));

        VARIANT_BOOL loadOk = parser->LoadQSPRTreeFile(bstr_t(mpsRFQcommInterInfo->m_strXttPath.c_str()));
        if(loadOk)
        {
            for(long testIndex=0; testIndex< parser->GetNumOfTests(); ++testIndex)
            {
                long numParams = 0;
                bstr_t testname;
                bstr_t realname;
                S_QCOMM_FRCALVFS_CONFIG stestProject;

                parser->GetTestInfo(testIndex,testname.GetAddress(),realname.GetAddress(),&numParams);
                TestStatus((LPCTSTR)testname,E_INIT_TEST_STATUS);
                //! TEST_PROJECT_NAME_MSG((LPCTSTR)testname);
                int testId = mpsRFQcommInterInfo->m_sti_func_decl.sti_add_test(mpsRFQcommInterInfo->m_iface,testname,realname);
                stestProject.m_lNumParams = numParams;
                stestProject.m_bTest = true;
                stestProject.TestName = /*(LPCTSTR)testname/*/JGW_A2W_A((LPCSTR)testname,CP_UTF8).c_str();
                stestProject.RealName = (LPCTSTR)realname;
                for(int paramIndex=0; paramIndex < numParams; ++paramIndex)
                {
                    SParameters_STRUCT sparam;
                    //bstr_t paramName,paramVal,unit,upper,lower,type,mode; 

                    parser->GetTestParameterInfo(
                        testIndex,paramIndex,
                        sparam.paramName.GetAddress(),
                        sparam.paramVal.GetAddress(),
                        sparam.unit.GetAddress(),
                        sparam.upper.GetAddress(),
                        sparam.lower.GetAddress(),
                        sparam.type.GetAddress(),
                        sparam.mode.GetAddress());

                    if(!sparam.upper || !sparam.lower)
                    {
                        mpsRFQcommInterInfo->m_sti_func_decl.sti_add_parameter(mpsRFQcommInterInfo->m_iface,testId,sparam.paramName,sparam.paramVal,"","",sparam.mode);
                    }
                    else
                    {
                        mpsRFQcommInterInfo->m_sti_func_decl.sti_add_parameter(mpsRFQcommInterInfo->m_iface,testId,sparam.paramName,sparam.paramVal,sparam.upper,sparam.lower,sparam.mode);
                    }
                    stestProject.m_vParameters.push_back(sparam);
                }
                mvsQcommRFCalVFSConfig.push_back(stestProject);
            }
        }
        else
        {  
            LogE_F(_T("LoadQSPRTreeFile Fail"));
        }
        CoUninitialize();

        return (loadOk!=0);
    }


    bool CTSE_RFQualcommInterface::LoadRFCalVerTestXMLConfig(const std::string filename/*, const stm_assembly_info & ai*/)
    {
        CTSE_RFCalVFSConfig rfCalVFSConfig(mvsQcommRFCalVFSConfig);
        rfCalVFSConfig.PareseXmlToXttConfig(filename);
        //CPareseCalXml pareseXml ;
        ///加载CalConfig.xml配置文件
        //LoadUserConfig(&pareseXml,filename);
        //加载校准xml文件
        //pareseXml.LoadCalXml(filename);
        for (auto it = mvsQcommRFCalVFSConfig.begin();it != mvsQcommRFCalVFSConfig.end();it ++)
        {
            int testId = mpsRFQcommInterInfo->m_sti_func_decl.sti_add_test(mpsRFQcommInterInfo->m_iface,it[0].TestName,it[0].RealName);
            TestStatus((LPCTSTR)it[0].TestName,E_INIT_TEST_STATUS);
            if( it[0].m_bDestory ) m_vDestory.push_back(testId);
            for(int paramIndex = 0; paramIndex < it[0].m_lNumParams; ++paramIndex)
            {
                mpsRFQcommInterInfo->m_sti_func_decl.sti_add_parameter(mpsRFQcommInterInfo->m_iface,testId,
                    it[0].m_vParameters.at(paramIndex).paramName
                    ,it[0].m_vParameters.at(paramIndex).paramVal
                    ,it[0].m_vParameters.at(paramIndex).upper
                    ,it[0].m_vParameters.at(paramIndex).lower
                    ,it[0].m_vParameters.at(paramIndex).mode
                    );
            }
        }

        return true;
    }

    /******************************************************************************
    * FUNCTION: DutThread::DutTestEventHandler
    *
    * DESCRIPTION: See header file
    *****************************************************************************/
    void CTSE_RFQualcommInterface::DutTestEventHandler(void * context, int type, const char * msg)
    {	
        std::wstring strTemp = JGW_A2W_A(msg);
        
        CTSE_RFQualcommInterface* psRFQualcommInterface = (CTSE_RFQualcommInterface*)context;

        switch(type)
        {
        case E_RFCAL_QSEQ_DEBUG_CAL: // debug
            LogI(strTemp.c_str());
            break;
        case E_RFCAL_QSEQ_LOG_CAL: // log - > QSEQ LOG
            LogI(strTemp.c_str());
            JGW_EraseLastAndFristTwoCharsW(strTemp);
            psRFQualcommInterface->HandleCPKLogInfo(strTemp);
            break;
        case E_RFCAL_QSEQ_ERROR_CAL: // error
            LogE(strTemp.c_str());
            break;
        case E_RFCAL_QSEQ_TPL3_CAL: // tpl3
            break;
        case E_RFCAL_QSEQ_HTML_CAL: // html
            psRFQualcommInterface->SaveHtmlLogging(strTemp);
            LogU(E_USER_DEFINE_HTMLFILEPATH,strTemp.c_str());
            break;
        default:
            break;
        }
    }

    void CTSE_RFQualcommInterface::SaveHtmlLogging(const std::wstring& strLogFilePath)
    {
        if (mpsRFQcommInterInfo->mbSaveTestLogging) 
        {
            mMoveTestLogging.MoveTestLogging(GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),mpsRFQcommInterInfo->mwstrSN.c_str(),strLogFilePath.c_str(),mpsRFQcommInterInfo->gbQcommTestResult);
            if (!mpsRFQcommInterInfo->mbIsQSEQCore) return ;
            //! 保存_Log_ConsoleApp文件
            std::wstring strLogConsoleApp(strLogFilePath);
            JGW_ReplaceStringW(strLogConsoleApp,L".html",L"");
            strLogConsoleApp += L"_Log_ConsoleApp.txt";
            mMoveTestLogging.MoveTestLogging(GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),mpsRFQcommInterInfo->mwstrSN.c_str(),strLogConsoleApp.c_str(),mpsRFQcommInterInfo->gbQcommTestResult);
        }
    }


    bool CTSE_RFQualcommInterface::ExecuteRFCalVerTest()
    {
        if (!mpsRFQcommInterInfo || !mpsRFQcommInterInfo->m_iface) return false;

        if (mpsRFQcommInterInfo->mbRecordCPKData)
        {
            mParseCPKLogInfo.GetLTECPKLogInfos().clear();
            mParseCPKLogInfo.GetWCDMACPKLogInfos().clear();
        }
     
        __try
        {
            mpsRFQcommInterInfo->m_sti_func_decl.sti_set_environment_var(mpsRFQcommInterInfo->m_iface,"SN",mpsRFQcommInterInfo->mstrSN.c_str());
            mpsRFQcommInterInfo->m_sti_func_decl.sti_set_environment_var(mpsRFQcommInterInfo->m_iface,"QSEQ_INTERNAL_UUT_ID",mpsRFQcommInterInfo->mstrSN.c_str());
           ExecuteTestAll(); 
        }
        __except( EXCEPTION_CONTINUE_EXECUTION )
        {
            LogE(_T("subsysCal异常"));
            mpsRFQcommInterInfo->gbQcommTestResult = false;
            return false;
        }
        if (mpsRFQcommInterInfo->mbRecordCPKData) mParseCPKLogInfo.EndHandleQSEQTestInfo();
        return mpsRFQcommInterInfo->gbQcommTestResult;
    }

    bool CTSE_RFQualcommInterface::ExecuteTestAll()
    {
        mpsRFQcommInterInfo->gbQcommTestResult = true;
        for (m_nCurrentIndex = 0;m_nCurrentIndex < mvsQcommRFCalVFSConfig.size();++m_nCurrentIndex)
        {      
            mpsRFQcommInterInfo->gbQcommTestResult &= ExecuteSuiteIndex(m_nCurrentIndex);    
            if( !mpsRFQcommInterInfo->gbQcommTestResult && mpsRFQcommInterInfo->m_bErrorStop )//fail
            {
                ExecuteDestoryTest();
                return mpsRFQcommInterInfo->gbQcommTestResult;
            }
        }
        return mpsRFQcommInterInfo->gbQcommTestResult;
    }

    bool CTSE_RFQualcommInterface::ExecuteDestoryTest()
    {
        if( m_nCurrentIndex == mvsQcommRFCalVFSConfig.size() - 1 ) return true;
        bool bTestResult = true;
        for (auto it = m_vDestory.begin();it != m_vDestory.end();it++)
        {
            if( it[0] > m_nCurrentIndex && it[0] < mvsQcommRFCalVFSConfig.size() )
            {
                bTestResult &= ExecuteSuiteIndex(it[0]);
            }
        }
        return bTestResult;
    }

    bool CTSE_RFQualcommInterface::ExecuteSuiteIndex(int nIndex)
    {
        int result = 0;
        for (int x = 0;x < mpsRFQcommInterInfo->m_nErrorReTest ;x ++)
        {
            __try
            {
                TestStatus((LPCTSTR)(mvsQcommRFCalVFSConfig.at(nIndex).TestName),E_RUN_TEST_STATUS);
                result = mpsRFQcommInterInfo->m_sti_func_decl.sti_execute_test_id(mpsRFQcommInterInfo->m_iface,(int)nIndex);
                TestStatus((LPCTSTR)(mvsQcommRFCalVFSConfig.at(nIndex).TestName),result?E_PASS_TEST_STATUS:E_FAIL_TEST_STATUS);
            }
            __except( EXCEPTION_EXECUTE_HANDLER )
            {
                LogE(_T("未捕获异常\n"));
                result = 0;
            }
            if(0 != result) return true;
        }
        return false;
    }

    void CTSE_RFQualcommInterface::HandleCPKLogInfo(const std::wstring& strCPKLogInfo)
    {
        if (!mpsRFQcommInterInfo->mbRecordCPKData) return ;
        mParseCPKLogInfo.StartHandleQSEQTestInfo(strCPKLogInfo.c_str());
    }

    CCJGW_ParseCPKLogInfo& CTSE_RFQualcommInterface::GetParseCPKData()
    {
        return mParseCPKLogInfo;
    }
}