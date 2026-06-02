#include "StdAfx.h"

#include "CJGW_TestSuiteAssemblyConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_TestSuiteGlobalResourcesLibrary/CJGW_TestSuiteEnvironmentResourceImpl.h>
#include "CJGW_TestSuiteInterface.h"
#include "CJGW_SuiteEnvironment.h"
#include <WinBase.h>
#include <Shlwapi.h>
namespace JGW
{
    typedef void (*LPRegisterSuiteLogCallback_Func)(const int nIndex,jgw_suite_test_log_func& pSuiteLogResFunc);
    typedef void (*LPSetSuiteEnvironmentFunc_Func)(JGW_SetSuiteEnvironmentVarFunc pset_suite_env_func,JGW_GetSuiteEnvironmentVarFunc pget_suite_env_func);

    CCJGW_TestSuiteInterface::CCJGW_TestSuiteInterface(bool bMultiSuite,const int nIndex):m_nAssemblyIndex(nIndex),m_hMain(NULL),mbMultiSuite(bMultiSuite),mpSuiteTestLogFunc(NULL)
    {
    }


    CCJGW_TestSuiteInterface::~CCJGW_TestSuiteInterface(void)
    {
    }

    void CCJGW_TestSuiteInterface::SetSuiteTestLogFuncPtr(jgw_suite_test_log_func pSuiteTestLogFunc)
    {
        mpSuiteTestLogFunc = pSuiteTestLogFunc;
    }

    bool CCJGW_TestSuiteInterface::InitSuiteAssembly(HWND hMain,const std::string& strConfigFileBuf,LPCTSTR strConfigFilePath /* = NULL */)
    {
        m_hMain = hMain;
        mvSuiteTestName.clear();
        m_vFinalizeTest.clear();
        m_vSuiteTestCommand.clear();
        //! parese suite assembly
        {
            CCJGW_TestSuiteAssemblyConfig parese_suite_config(m_s_suite_config);
            if (NULL == strConfigFilePath)
            {
                if (!parese_suite_config.PareseAssemblyConfigToBuffer(strConfigFileBuf))  return false;
            }
            else
            {
                std::string strconfig = JGW_W2A_W(strConfigFilePath);
                if (!parese_suite_config.PareseAssemblyConfig(strconfig))
                {
                    return false;
                }
            }
        }
        return LoadTestSuiteModuleClassPtr();
    }

    bool CCJGW_TestSuiteInterface::LoadTestSuiteModuleClassPtr()
    {
        std::string strTestSuiteModuleFilePath,strSuiteClassName;
        size_t i = 0;
        //! frist load library
        for (_vsuitetestconfig::iterator it = m_s_suite_config.m_vSuiteTestConfig.begin();
            it != m_s_suite_config.m_vSuiteTestConfig.end();
            ++ it)
        {
            auto itparam = it[0]->find(L"ModuleName");
            if (itparam == it[0]->m_mTestParam.end()) continue;
            if (!LoadTestSuiteModule(itparam->mParamValue,strTestSuiteModuleFilePath)) return false;
            if (!SetTestSuiteGlobalResourcesInfo(strTestSuiteModuleFilePath)) return false;
        }
        //! load class name
        for (_vsuitetestconfig::iterator it = m_s_suite_config.m_vSuiteTestConfig.begin();
            it != m_s_suite_config.m_vSuiteTestConfig.end();
            ++ it,i ++)
        {
            if(it[0]->m_bFinalizeTest) m_vFinalizeTest.push_back(i);
            auto itparam = it[0]->find(L"RealName");
            if (itparam == it[0]->m_mTestParam.end())
            {
                PELOG4WW(L"Class Name is not Empty!!!");
                return false;
            }
            strSuiteClassName = JGW_W2A(itparam->mParamValue);
            std::shared_ptr<CCJGW_TestSuiteImpl> pTestSuiteImpl (mTestSuitePocoClassLoader.CreateTestSuitePtrClass(strSuiteClassName));
            if (!pTestSuiteImpl)
            {
                PELOG4WW(L"Class Name is not Find To Dll!!!");
                return false;
            }
            InitTestSuiteClassConfigParam(pTestSuiteImpl,it[0]);
            m_vSuiteTestCommand.push_back(pTestSuiteImpl);
        }
        //! init common param
        for (_mcommontestconfig::iterator it = m_s_suite_config.m_mCommonTestParam.begin();
            it != m_s_suite_config.m_mCommonTestParam.end();
            ++ it)
        {
            CCJGW_SuiteEnvironment::SetSuiteEnvironmentVar(it->first.c_str(),it->second.c_str());
        }

        return true;
    }

    bool CCJGW_TestSuiteInterface::SetTestSuiteGlobalResourcesInfo(const std::string& strTestSuiteModuleFilePath)
    {
        //!
        std::string strUtfTestSuiteModuleFilePath = U(strTestSuiteModuleFilePath.c_str());
        LPRegisterSuiteLogCallback_Func pRegisterSuiteLogCallback_Func = NULL;
        LPSetSuiteEnvironmentFunc_Func pSetSuiteEnvironmentFunc_Func = NULL;

        Poco::SharedLibrary sharedLibrary(strUtfTestSuiteModuleFilePath);
        if (!sharedLibrary.isLoaded()) return false;

        if (sharedLibrary.hasSymbol("RegisterSuiteLogCallback"))
        {
            pRegisterSuiteLogCallback_Func = (LPRegisterSuiteLogCallback_Func)sharedLibrary.getSymbol("RegisterSuiteLogCallback");
            if (pRegisterSuiteLogCallback_Func) pRegisterSuiteLogCallback_Func(m_nAssemblyIndex,mpSuiteTestLogFunc);
        }
        if (sharedLibrary.hasSymbol("SetSuiteEnvironmentFunc"))
        {
            pSetSuiteEnvironmentFunc_Func = (LPSetSuiteEnvironmentFunc_Func)sharedLibrary.getSymbol("SetSuiteEnvironmentFunc");
            if (pSetSuiteEnvironmentFunc_Func) pSetSuiteEnvironmentFunc_Func(CCJGW_SuiteEnvironment::SetSuiteEnvironmentVar,CCJGW_SuiteEnvironment::GetSuiteEnvironmentVar);
        }
        return true;
    }

    bool CCJGW_TestSuiteInterface::LoadTestSuiteModule(std::wstring& strTestSuiteModuleName,std::string& strTestSuiteModuleFilePath)
    {
        std::wstring wstrModuleName(strTestSuiteModuleName),wstrSrcModuleName;
        std::wstring strTemp(JGW_GetApplicationFolder());

        if(mbMultiSuite)
        {
            int extDotPos = wstrModuleName.rfind('.');
            if(extDotPos != std::wstring::npos)
            {
                wstrModuleName += L".dll";
                strTestSuiteModuleName += L".dll";
                extDotPos = wstrModuleName.rfind('.');
            }
            std::wstring ss;
            FormatWString(ss,L"_%d",m_nAssemblyIndex);
            wstrModuleName.insert(extDotPos,ss.c_str());
        }
        //! deal module path
        {
            wstrModuleName = JGW_RealativePathToAbsPath(wstrModuleName.c_str());
            wstrSrcModuleName = JGW_RealativePathToAbsPath(strTestSuiteModuleName.c_str());

            if (mbMultiSuite && !JGW_FileExistsToFilePath(wstrModuleName.c_str()))
            {
                if(!CopyFileW(wstrSrcModuleName.c_str(),wstrModuleName.c_str(),FALSE ))
                {
                    PELOG4WW_F(L"DLL文件复制失败，请检查文件是否存在(%s)",wstrSrcModuleName.c_str());
                    return false;
                }
            }
        }    
        strTestSuiteModuleFilePath = JGW_W2A(wstrModuleName);
        return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteModuleFilePath);
    }

    size_t CCJGW_TestSuiteInterface::GetSuiteClassPtrCount()
    {
        return m_vSuiteTestCommand.size();
    }

    void CCJGW_TestSuiteInterface::InitTestSuiteClassConfigParam(std::shared_ptr<CCJGW_TestSuiteImpl> ptrTestSuiteImpl,std::shared_ptr<s_suite_item_test_config> ptrSuiteItemTestConfig)
    {
        for (std::vector<s_suite_single_item_test_config>::iterator it = ptrSuiteItemTestConfig->m_mTestParam.begin();
            it != ptrSuiteItemTestConfig->m_mTestParam.end();
            ++ it)
        {
            if (NULL == it->mParamName.compare(L"ModuleName") 
                || NULL == it->mParamName.compare(L"RealName")) continue;

            if (NULL == it->mParamName.compare(L"TestName"))
            {
                mvSuiteTestName.push_back(it->mParamValue);
                continue;
            }

            ptrTestSuiteImpl->CreateParam(it->mParamName.c_str(),it->mParamValue.c_str());
        }
    }

    void CCJGW_TestSuiteInterface::UpdateEnviromentParam()
    {
        for ( auto it = m_vSuiteTestCommand.begin();
            it != m_vSuiteTestCommand.end();
            it ++)
        {
            it[0]->UpdateEnviromentParam();
        }
    }

    bool CCJGW_TestSuiteInterface::ExecuteAllTest(bool bErrorStop /* = true */)
    {
        bool result = true;
        size_t uIndex = 0;
        m_vAlreadyRunSuiteIndex.clear();

        for (auto it = m_vSuiteTestCommand.begin();
            it != m_vSuiteTestCommand.end();
            it ++,uIndex++)
        {
            result &= ExecuteSingleTest(uIndex);
            m_vAlreadyRunSuiteIndex.push_back(uIndex);
            if( !result && bErrorStop )
            {
                ExecuteTestFinalize();
                return false;
            }
        }
        return result;
    }

    bool CCJGW_TestSuiteInterface::ExecuteTestFinalize()
    {
        for(size_t i = 0; i < m_vFinalizeTest.size();i ++)
        {
            if( -1 == find_value(m_vAlreadyRunSuiteIndex,m_vFinalizeTest.at(i)) )
            {
                ExecuteSingleTest(m_vFinalizeTest.at(i));
            }
        }
        m_vAlreadyRunSuiteIndex.clear();
        return true;
    }

    bool CCJGW_TestSuiteInterface::ExecuteSingleTest(size_t nIndex)
    {
        if( nIndex >= m_vSuiteTestCommand.size() )
        {
            PELOG4WW(L"测试索引超过测试序列范围");
            return false;
        }
        m_vAlreadyRunSuiteIndex.push_back(nIndex);
        bool bSigleResult = true;
        if (mpSuiteTestLogFunc) mpSuiteTestLogFunc(m_nAssemblyIndex,E_TEST_SUITE_TEST_NAME,mvSuiteTestName[nIndex].c_str());

        try
        {
            for (size_t i = 0;i < m_vSuiteTestCommand[nIndex]->GetErrorRetryTestCount();i ++)
            {
                bSigleResult = true;
                bSigleResult &= m_vSuiteTestCommand[nIndex]->Init();
                bSigleResult &= m_vSuiteTestCommand[nIndex]->Run();
                bSigleResult &= m_vSuiteTestCommand[nIndex]->Exit();
                if (bSigleResult) break;
            }
        }
        catch(...)
        {
            return false;
        }

        return bSigleResult;
    }
}