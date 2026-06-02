#include "StdAfx.h"
#include "CJGW_RFQualcommInterface.h"
#import "QSPR3xttParse.tlb" no_namespace named_guids
#include <Shlwapi.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>

namespace JGW
{

    CCJGW_RFQualcommInterface::CCJGW_RFQualcommInterface(std::wstring& wstrDllSrc):m_wstrDllSrc(wstrDllSrc),m_strSn("123456789123456")
        ,m_dll_ptr(NULL)
        ,m_iface(NULL)
        ,m_nXttTestCount(0)
        ,m_nErrorRetest(3)
        ,m_bErrorStop(true)
        ,m_bFristLoadInterfaceModule(false)
        ,m_nCurrentIndex(0)
    {
        memset(&m_sti_func_decl,0x00,sizeof(m_sti_func_decl));
    }

    CCJGW_RFQualcommInterface::~CCJGW_RFQualcommInterface(void)
    {
        FreeCalDll();
    }

    void CCJGW_RFQualcommInterface::FreeCalDll()
    {
        if(m_iface != NULL)
        {
            m_sti_func_decl.sti_clear_equip_config(m_iface);
            m_sti_func_decl.sti_clear_rf_net_loss(m_iface);
            m_sti_func_decl.sti_clear_station_config(m_iface);
            m_sti_func_decl.sti_clear_test_db(m_iface);
            m_sti_func_decl.sti_release_suite_interface(&m_iface);
            m_iface = NULL;
        }

        if (NULL != m_dll_ptr)
        {
            FreeLibrary(m_dll_ptr);
            m_dll_ptr = NULL;
            m_bFristLoadInterfaceModule = false;
        }
    }

    //! 加载校准DLL
    HMODULE CCJGW_RFQualcommInterface::LoadCalDll()
    {
        //! 如果已经实例化DLL 测释放  然后Load
        if (NULL != m_dll_ptr)
        {
            FreeCalDll();
            m_dll_ptr = NULL;
        }
        std::wstring strpath = PathFindFileName(m_wstrDllSrc.c_str());

        m_dll_ptr = GetModuleHandle(strpath.c_str());
        if( m_dll_ptr ) return m_dll_ptr;

        m_dll_ptr = LoadLibrary(m_wstrDllSrc.c_str());
        return m_dll_ptr;
    }
    //! 初始化校准接口文件
    bool CCJGW_RFQualcommInterface::InitQCServerModule()
    {
        if( m_bFristLoadInterfaceModule ) return true;

        if(!LoadCalDll())
        {
            PELOG4WW_F(L"加载DLL失败(%s),Error:%s",m_wstrDllSrc.c_str(),JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }

        if(!sti_initialize_export_decl(m_dll_ptr,&m_sti_func_decl))  // Unload DLL if interfaces are not resolved
        {
            FreeLibrary(m_dll_ptr);
            m_dll_ptr = NULL;
            PELOG4WW_F(L"初始化校准Server函数失败,Error:%s",JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }

        m_iface = m_sti_func_decl.sti_create_suite_interface();
        //! STL本地化设置切回默认值
        std::locale::global(std::locale("C"));

        if( !m_iface )
        {
            PELOG4WW(L"Qualcomm接口创建失败!!!");
            return false;
        }
        m_bFristLoadInterfaceModule = true;
        return true;
    }

    void CCJGW_RFQualcommInterface::InitQCServerInterface(const std::string  filename,unsigned char bUseQPST,std::string& strEquipPad,std::string& strEquipName)
    {
        // Check and set phone mode
        m_sti_func_decl.sti_clear_test_db(m_iface);
        m_sti_func_decl.sti_set_log_filename(m_iface,filename.c_str());
        // SUITE default is to use QPST, override default if QPhoneMS.
        m_sti_func_decl.sti_add_station_config_item(m_iface,2,"QMSLLibraryMode",bUseQPST?"1":"0");
        m_sti_func_decl.sti_register_system_event_handler(m_iface,this,DutTestEventHandler);
        m_sti_func_decl.sti_clear_test_db(m_iface);
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
        m_sti_func_decl.sti_set_environment_var(m_iface, "XTT_TIMEOUT", XTT_TIMEOUT);
        m_sti_func_decl.sti_set_environment_var(m_iface,  "XTT_TIMEOUT", XTT_TIMEOUT);
        m_sti_func_decl.sti_set_environment_var(m_iface,  "XTT_TIMEOUT_OFFSET", XTT_TIMEOUT_OFFSET);
        m_sti_func_decl.sti_set_environment_var(m_iface, "XTT_TIMEOUT_OFFSET", XTT_TIMEOUT_OFFSET);   
    }
    //! 加载Xtt文件
    bool CCJGW_RFQualcommInterface::LoadRefXtt(const std::string  filename/*, const stm_assembly_info & ai*/)
    {
        m_vDestory.clear();
        size_t nIndex = filename.find_last_of('.');
        if(std::string::npos != filename.find(".xml",nIndex))
        {
            return LoadRefXml(filename/*,ai*/);
        }
        //! 以XTT方式加载测试XTT
        SUCCEEDED(CoInitialize(NULL));
        _QSPR3XTTPtr parser(__uuidof(QSPR3XTT));

        std::vector<STestProject_STRUCT>& stest = m_rfPareseConfig.GetTestProject();
        VARIANT_BOOL loadOk = parser->LoadQSPRTreeFile(bstr_t(filename.c_str()));
        if(loadOk)
        {
            m_nXttTestCount = parser->GetNumOfTests();
            for(ULONG testIndex=0; testIndex< m_nXttTestCount; ++testIndex)
            {
                long numParams = 0;
                bstr_t testname;
                bstr_t realname;
                STestProject_STRUCT stestProject;

                parser->GetTestInfo((long)testIndex,testname.GetAddress(),realname.GetAddress(),&numParams);
                //! TEST_PROJECT_NAME_MSG((LPCTSTR)testname);
                int testId = m_sti_func_decl.sti_add_test(m_iface,testname,realname);
                stestProject.m_lNumParams = numParams;
                stestProject.m_bTest = true;
                stestProject.TestName = /*(LPCTSTR)testname/*/(W((LPCSTR)testname));
                stestProject.RealName = (LPCTSTR)realname;
                for(int paramIndex=0; paramIndex < numParams; ++paramIndex)
                {
                    SParameters_STRUCT sparam;
                    //bstr_t paramName,paramVal,unit,upper,lower,type,mode; 

                    parser->GetTestParameterInfo(testIndex,paramIndex,sparam.paramName.GetAddress(),
                        sparam.paramVal.GetAddress(),sparam.unit.GetAddress(),sparam.upper.GetAddress(),sparam.lower.GetAddress(),
                        sparam.type.GetAddress(),sparam.mode.GetAddress());

                    if(!sparam.upper || !sparam.lower)
                    {
                        m_sti_func_decl.sti_add_parameter(m_iface,testId,sparam.paramName,sparam.paramVal,"","",sparam.mode);
                    }
                    else
                    {
                        m_sti_func_decl.sti_add_parameter(m_iface,testId,sparam.paramName,sparam.paramVal,sparam.upper,sparam.lower,sparam.mode);
                    }
                    stestProject.m_vParameters.push_back(sparam);
                }
                stest.push_back(stestProject);
            }
        }
        else
        {  
            PELOG4WW(_T("LoadQSPRTreeFile Fail"));
        }
        CoUninitialize();

        return (loadOk!=0);
    }


    bool CCJGW_RFQualcommInterface::LoadRefXml(const std::string filename/*, const stm_assembly_info & ai*/)
    {
        m_rfPareseConfig.PareseXmlToXttConfig(filename);
        //CPareseCalXml pareseXml ;
        ///加载CalConfig.xml配置文件
        //LoadUserConfig(&pareseXml,filename);
        //加载校准xml文件
        //pareseXml.LoadCalXml(filename);
        m_nXttTestCount = m_rfPareseConfig.GetTestProject().size();

        for (auto it = m_rfPareseConfig.GetTestProject().begin();it != m_rfPareseConfig.GetTestProject().end();it ++)
        {
            int testId = m_sti_func_decl.sti_add_test(m_iface,it[0].TestName,it[0].RealName);
            //!TEST_PROJECT_NAME_MSG((LPCTSTR)it[0].TestName);
            if( it[0].m_bDestory ) m_vDestory.push_back(testId);
            for(int paramIndex = 0; paramIndex < it[0].m_lNumParams; ++paramIndex)
            {
                m_sti_func_decl.sti_add_parameter(m_iface,testId,
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
    void CCJGW_RFQualcommInterface::DutTestEventHandler(void * context, int type, const char * msg)
    {	
        std::wstring strTemp = JGW_A2W_A(msg);
        switch(type)
        {
        case E_RFCAL_QSEQ_DEBUG_CAL: // debug
            DEBUG_MSG(strTemp.c_str());
            break;
        case E_RFCAL_QSEQ_LOG_CAL: // log
            INFO_MSG(strTemp.c_str());
            break;
        case E_RFCAL_QSEQ_ERROR_CAL: // error
            ERROR_MSG(strTemp.c_str());
            break;
        case E_RFCAL_QSEQ_TPL3_CAL: // tpl3
            break;
        case E_RFCAL_QSEQ_HTML_CAL: // html
            HTML_FILE_PATH_MSG(strTemp.c_str());
            break;
        default:
            break;
        }
    }


    bool CCJGW_RFQualcommInterface::RunXtt()
    {
        if ( m_iface == NULL ) return false;
        __try
        {
            m_sti_func_decl.sti_set_environment_var(m_iface,"SN",m_strSn.c_str());
            return ExecuteTestAll(); 
        }
        __except( EXCEPTION_CONTINUE_EXECUTION )
        {
            ERROR_MSG(_T("subsysCal异常"));
            return false;
        }

        return false;
    }

    bool CCJGW_RFQualcommInterface::ExecuteTestAll()
    {
        bool bTestResult = true;
        bool bOK = false;

        for (m_nCurrentIndex = 0;m_nCurrentIndex < m_nXttTestCount;++m_nCurrentIndex)
        {
            TEST_PROJECT_NAME_FORMAT(_T("%s runing\n"),(LPCTSTR)(m_rfPareseConfig.GetTestProject().at(m_nCurrentIndex).TestName));
            bOK = ExecuteSuiteIndex(m_nCurrentIndex);
            bTestResult &= bOK;
            TEST_PROJECT_NAME_FORMAT(bOK?_T("%s pass\n"):_T("%s fail\n"),(LPCTSTR)(m_rfPareseConfig.GetTestProject().at(m_nCurrentIndex).TestName));
            if( !bTestResult && m_bErrorStop )//fail
            {
                ExecuteDestoryTest();
                return false;
            }
        }

        return bTestResult;
    }

    bool CCJGW_RFQualcommInterface::ExecuteDestoryTest()
    {
        if( m_nCurrentIndex == m_nXttTestCount - 1 ) return true;
        bool bTestResult = true;
        bool bOK = false;

        for (auto it = m_vDestory.begin();it != m_vDestory.end();it++)
        {
            if( it[0] > m_nCurrentIndex && it[0] < m_nXttTestCount )
            {
                //ALERT_LOG_FORMAT(_T("%s runing\n"),(LPCTSTR)(m_rfPareseConfig.GetTestProject().at(it[0]).TestName));
                bOK = ExecuteSuiteIndex(it[0]);
                bTestResult &= bOK;
                TEST_PROJECT_NAME_FORMAT(bOK?_T("%s pass\n"):_T("%s fail\n"),(LPCTSTR)(m_rfPareseConfig.GetTestProject().at(it[0]).TestName));
            }
        }

        return bTestResult;
    }

    bool CCJGW_RFQualcommInterface::ExecuteSuiteIndex(int nIndex)
    {
        int result = 0;
        for (int x = 0;x < m_nErrorRetest ;x ++)
        {
            __try
            {
                result = m_sti_func_decl.sti_execute_test_id(m_iface,(int)nIndex);
            }
            __except( EXCEPTION_EXECUTE_HANDLER )
            {
                ERROR_MSG(_T("未捕获异常\n"));
                result = 0;
            }
            if( 0 != result )
            {
                return true;
            }
        }
        return false;
    }

    size_t CCJGW_RFQualcommInterface::GetXttTestCount()
    {
        return m_nXttTestCount;
    }

    // bool ctf_rfqualcomminterface::LoadUserConfig(CPareseCalXml *pCalConfig,const std::string  filename)
    // {
    // 	std::string strCalConfigFile( filename );
    // 	size_t nIndex = strCalConfigFile.find_last_of('\\');
    // 	strCalConfigFile.erase(nIndex);
    // 	strCalConfigFile.append(USECONFIGNAME);
    // 
    // 	pCalConfig->LoadCalConfigFile(strCalConfigFile);
    // 
    // 	return true;
    // }

    // bool ctf_rfqualcomminterface::LoadRfCalLoss(std::wstring strLossPath)
    // {
    // 	//RFCAL_LTE "CMW_AUXLoss" Tx,"CMW_DLLoss"  4 Tx ,相对RX线损 "CMW_ULLoss"  5 Rx 相对Tx线损.
    // 	CCalLossTable calLoss(m_iface,m_sti_func_decl,strLossPath);
    // 
    // 	for (_bandMAP::iterator it = CPareseCalXml::m_sCalBandMap.begin();it != CPareseCalXml::m_sCalBandMap.end();it ++)
    // 	{
    // 		if( !calLoss.LoadSysLossTable(it->first,it->second) )
    // 		{
    // 			return false;
    // 		}
    // 	}
    // 
    // 	return true;
    // }

}