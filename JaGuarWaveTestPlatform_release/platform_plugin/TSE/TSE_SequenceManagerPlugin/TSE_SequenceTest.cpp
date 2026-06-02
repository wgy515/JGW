#include "StdAfx.h"
#include "TSE_SequenceTest.h"
#include "TSE_SequenceTestConfig.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define IS_OK(result,error_log) \
    if (!(result)) \
    { \
    mLogServices.ErrorLog(error_log);\
    return false;\
    }

namespace JGW
{
    CTSE_SequenceTest::CTSE_SequenceTest(CTSE_LogServices& logServices,CTSE_TestStatusService& testStatusService,CTSE_GlobalEnvironment& globalEnvironment,int token_id) : mLogServices(logServices),mTestStatusService(testStatusService),mGlobalEnvironment(globalEnvironment),mnTokenID(token_id),mbIsFinalizeTest(false)
    {
    }


    CTSE_SequenceTest::~CTSE_SequenceTest(void)
    {
        UnLoadTSETestPlugin();
    }

    void CTSE_SequenceTest::UnLoadTSETestPlugin()
    {
        if (!m_vAlreadyRunSuiteIndex.empty()) ExecTSETestFinalize();
        mvSuiteProjectTestBase.clear();  
		mvTSETestBase.clear();
        m_vAutoTSETestCommand.clear();
        m_vConfigViewTestCommand.clear();
        mvsTSETestIndexGroups.clear();
        m_vAlreadyRunSuiteIndex.clear();
        mmapTSETestGroup.clear();
        m_vFinalizeTest.clear();
        //! 清空全局资源
        for (size_t i = 0;i < mvSuiteGlobalResources.size();i ++)
        {
            mvSuiteGlobalResources.at(i)->TSE_AddParam(TSE_LOG_SERVICES,NULL);
            mvSuiteGlobalResources.at(i)->TSE_AddParam(TSE_GLOBAL_ENVIRONMENT,NULL);
            mvSuiteGlobalResources.at(i)->TSE_AddParam(TSE_TESTSTATUS_SERVICES,NULL);
        }
        mvSuiteGlobalResources.clear();
        mTestSuitePocoClassLoader.UnloadTestSuiteModule();
    }

    bool CTSE_SequenceTest::LoadTSESuiteConfig(const std::string& strResourceConfig)
    {
        m_vFinalizeTest.clear();
        mvsTSETestIndexGroups.clear();
        mvSuiteProjectTestBase.clear();  
		mvTSETestBase.clear();
        m_vAutoTSETestCommand.clear();
        m_vConfigViewTestCommand.clear();
        mvSuiteGlobalResources.clear();
        mmapTSETestGroup.clear();
        //! 解析SUITE 配置文件
        CTSE_SequenceTestConfig sequenceTestConfig(msSTESequenceConfig,mLogServices);
        mGlobalEnvironment.PutString(TSE_SUITE_FILE_PATH,JGW_A2W(strResourceConfig));
        mGlobalEnvironment.PutString(L"TSE_EXEC_FOLDER",JGW_GetApplicationFolder());

        if (!sequenceTestConfig.PareseAssemblyConfig(strResourceConfig)) return false;
        //! 加载TSE模块以及模块导出类指针
        return LoadTestSuiteModuleClassPtr();
    }

    bool CTSE_SequenceTest::LoadTestSuiteModuleClassPtr()
    {        
        //! frist load library 加载所需要的TSE Library
        if (!LoadTSEPlugin(msSTESequenceConfig.m_vAutoTestConfig)
            || !LoadTSEPlugin(msSTESequenceConfig.m_vConfigViewTestConfig)
            || !LoadSuiteProjectTSEPlugin(msSTESequenceConfig.mvsSuiteProjectConfig))
        {
            return false;
        }
		size_t testIndex = 0;
        //! load class name
        if (!LoadTSEPtrClassPlugin(msSTESequenceConfig.m_vAutoTestConfig,m_vAutoTSETestCommand)
            || !LoadTSEPtrClassPlugin(msSTESequenceConfig.m_vConfigViewTestConfig,m_vConfigViewTestCommand)
            || !LoadSuiteProjectTSEPtrClassPlugin(msSTESequenceConfig.mvsSuiteProjectConfig,mvSuiteProjectTestBase,testIndex))
        {
            return false;
        }
        //! init common param
        for (_mcommontestconfig::iterator it = msSTESequenceConfig.m_mCommonTestParam.begin();
            it != msSTESequenceConfig.m_mCommonTestParam.end();
            ++ it)
        {
            mGlobalEnvironment.PutString(it->first.c_str(),it->second.c_str());
        }

        LoadTSESuiteTestCommandGroupInfo();

        return true;
    }

	bool CTSE_SequenceTest::LoadSuiteProjectTSEPlugin(_vs_suite_projectconfig& vsSuiteProjectConf)
	{
		for (size_t i = 0;i < vsSuiteProjectConf.size();i ++)
		{
			if (!LoadSingleTSEPlugin(vsSuiteProjectConf[i]->msSuiteItemTestConfig))
			{
				return false;
			}

			if (!LoadSuiteProjectTSEPlugin(vsSuiteProjectConf[i]->m_vChildTestSuiteProjectConfig))
			{
				return false;
			}
		}	
		return true;
	}

	bool CTSE_SequenceTest::LoadSingleTSEPlugin(s_suite_item_test_config& suiteItemTestConfig)
	{
		std::wstring strTestSuiteModuleFilePath;
		auto itparam = suiteItemTestConfig.find(TSE_CONFIG_MODULENAME);
		if (itparam == suiteItemTestConfig.m_mTestParam.end()) return true;
		if (!LoadTSETestModule(itparam->mParamValue,strTestSuiteModuleFilePath)) 
		{
			mLogServices.ErrorLogFormat(L"LoadTSETestModule(%s) Fail",strTestSuiteModuleFilePath.c_str());
			return false;
		}
		//strTestSuiteModuleFilePath = JGW_W2A(strTestSuiteModuleFilePath,CP_UTF8);
		if (!SetTSEModuleGlobalResourcesInfo(strTestSuiteModuleFilePath))
		{
			mLogServices.ErrorLog(L"SetTSEModuleGlobalResourcesInfo Fail");
			return false;
		}
		return true;
	}

    bool CTSE_SequenceTest::LoadTSEPlugin(_vsuitetestconfig& vsuiteTestConfig,bool bSuiteProject /* = false */)
    {
        std::wstring strTestSuiteModuleFilePath;
        for (_vsuitetestconfig::iterator it = vsuiteTestConfig.begin();
            it != vsuiteTestConfig.end();
            ++ it)
        {
            auto itparam = it[0]->find(TSE_CONFIG_MODULENAME);
            if (itparam == it[0]->m_mTestParam.end()) continue;
            if (!LoadTSETestModule(itparam->mParamValue,strTestSuiteModuleFilePath)) 
            {
                mLogServices.ErrorLogFormat("LoadTSETestModule(%s) Fail",strTestSuiteModuleFilePath.c_str());
                return false;
            }
            //strTestSuiteModuleFilePath = U(strTestSuiteModuleFilePath.c_str());
            if (!SetTSEModuleGlobalResourcesInfo(strTestSuiteModuleFilePath))
            {
                mLogServices.ErrorLog(L"SetTSEModuleGlobalResourcesInfo Fail");
                return false;
            }
        }
        return true;
    }

	bool CTSE_SequenceTest::LoadSuiteProjectTSEPtrClassPlugin(_vs_suite_projectconfig &vsSuiteProjectConfig,std::vector<std::shared_ptr<S_SuiteProject_TestBase>>& vsSuiteProjectTestBase,size_t& testIndex)
	{
		for (size_t i = 0;i < vsSuiteProjectConfig.size();i ++)
		{
			std::shared_ptr<S_SuiteProject_TestBase> ptrSuiteProjectTestBase(new S_SuiteProject_TestBase);
			ptrSuiteProjectTestBase->mpsTestBase = LoadSingeTSEPtrClassPlugin(vsSuiteProjectConfig[i]->msSuiteItemTestConfig,testIndex);
            ptrSuiteProjectTestBase->mbIfFalse = vsSuiteProjectConfig[i]->msSuiteItemTestConfig.mbIfFalse;
            ptrSuiteProjectTestBase->mbReversedResult = vsSuiteProjectConfig[i]->msSuiteItemTestConfig.mbReversedResult;
			ptrSuiteProjectTestBase->mnTestIndex = testIndex ++;
			if (NULL == ptrSuiteProjectTestBase->mpsTestBase)
			{
				return false;
			}
			mvTSETestBase.push_back(ptrSuiteProjectTestBase->mpsTestBase);
			if (!LoadSuiteProjectTSEPtrClassPlugin(vsSuiteProjectConfig[i]->m_vChildTestSuiteProjectConfig,ptrSuiteProjectTestBase->mvChildTestBase,testIndex))
			{
				return false;
			}

			vsSuiteProjectTestBase.push_back(ptrSuiteProjectTestBase);
		}
		return true;
	}

	CTSE_TestBase* CTSE_SequenceTest::LoadSingeTSEPtrClassPlugin(s_suite_item_test_config& sSuiteItemTestConfig,size_t index)
	{
		std::string strSuiteClassName;
		//! 确认TSE CLASS NAME 是否存在
		auto itparam = sSuiteItemTestConfig.find(TSE_CONFIG_REALNAME);
		if (itparam == sSuiteItemTestConfig.m_mTestParam.end())
		{
			mLogServices.ErrorLog(L"Class Name is not Empty!!!");
			return false;
		}
		//! 根据TSE_CONFIG_REALNAME名 加载TSE 类指针
		strSuiteClassName = JGW_W2A(itparam->mParamValue);
		CTSE_TestBase* pTestSuiteImpl (const_cast<CTSE_TestBase*>(mTestSuitePocoClassLoader.CreateTestSuitePtrClass(strSuiteClassName)));
		if (!pTestSuiteImpl)
		{
			mLogServices.ErrorLogFormat(L"Class Name(%s) is not Find To Dll!!!",itparam->mParamValue.c_str());
			return false;
		}

		if (sSuiteItemTestConfig.m_bFinalizeTest) m_vFinalizeTest.push_back(index);
		InitTestSuiteClassConfigParam(pTestSuiteImpl,sSuiteItemTestConfig);

		return pTestSuiteImpl;
	}

    bool CTSE_SequenceTest::LoadTSEPtrClassPlugin(_vsuitetestconfig& vsuiteTestConfig,std::vector<CTSE_TestBase *>& vTSETestBase,bool bSuiteProject /* = false */)
    {
        std::string strSuiteClassName;
        /*S_TSE_TEST_GROUP sTSETestGroup;*/
        size_t i = 0;
        for (_vsuitetestconfig::iterator it = vsuiteTestConfig.begin();
            it != vsuiteTestConfig.end();
            ++ it,i ++)
        {
            //! 确认TSE CLASS NAME 是否存在
            auto itparam = it[0]->find(TSE_CONFIG_REALNAME);
            if (itparam == it[0]->m_mTestParam.end())
            {
                mLogServices.ErrorLog(L"Class Name is not Empty!!!");
                return false;
            }
            //! 根据TSE_CONFIG_REALNAME名 加载TSE 类指针
            strSuiteClassName = JGW_W2A(itparam->mParamValue);
            CTSE_TestBase* pTestSuiteImpl (const_cast<CTSE_TestBase*>(mTestSuitePocoClassLoader.CreateTestSuitePtrClass(strSuiteClassName)));
            if (!pTestSuiteImpl)
            {
                mLogServices.ErrorLogFormat(L"Class Name(%s) is not Find To Dll!!!",itparam->mParamValue.c_str());
                return false;
            }

            if (bSuiteProject && it[0]->m_bFinalizeTest) m_vFinalizeTest.push_back(i);
            InitTestSuiteClassConfigParam(pTestSuiteImpl,it[0]);
            vTSETestBase.push_back(pTestSuiteImpl);
        }
        return true;
    }

    void CTSE_SequenceTest::LoadTSESuiteTestCommandGroupInfo()
    {
#ifndef TDO_TSE_GROUP_CODE
        S_TSE_TEST_GROUP sTSETestGroup;
        std::vector<int> vnTemp;
        std::vector<std::string> vstrTemp;
        S_TSE_SUITE_GROUPS sTSETestIndexGroups;
        for (size_t i = 0;i < msSTESequenceConfig.m_vSuiteTestConfig.size();i ++)
        {
            vstrTemp.clear();
            sTSETestIndexGroups.mvpsTSETestIndexGroups.clear();
            vnTemp.clear();
            JGW_ParserStrA(msSTESequenceConfig.m_vSuiteTestConfig[i]->mstrGroupName.c_str(),",",vstrTemp);
            JGW_ParserIntA(msSTESequenceConfig.m_vSuiteTestConfig[i]->mstrPriority.c_str(),",",vnTemp);
            if (vstrTemp.empty()) vstrTemp.push_back("default");
            if (vnTemp.empty()) vnTemp.push_back(0);
            while (vnTemp.size() < vstrTemp.size()) vnTemp.push_back(vnTemp[0]);

            for (size_t x = 0;x < vstrTemp.size();x ++)
            {
                sTSETestIndexGroups.mvpsTSETestIndexGroups.push_back(&mmapTSETestGroup[vstrTemp[x]]);
                sTSETestGroup.mnPriority = vnTemp[x];
                sTSETestGroup.m_bFinalizeTest = msSTESequenceConfig.m_vSuiteTestConfig[i]->m_bFinalizeTest;
                sTSETestGroup.mnTestIndex = i;
                sTSETestIndexGroups.mvpsTSETestIndexGroups[x]->push_back(sTSETestGroup);
            }

            mvsTSETestIndexGroups.push_back(sTSETestIndexGroups);
        }
#endif
    }

	void CTSE_SequenceTest::InitTestSuiteClassConfigParam(CTSE_TestBase* ptrTestSuiteImpl,s_suite_item_test_config& sSuiteItemTestConfig)
	{
		for (std::vector<s_suite_single_item_test_config>::iterator it = sSuiteItemTestConfig.m_mTestParam.begin();
			it != sSuiteItemTestConfig.m_mTestParam.end();
			++ it)
		{
			if (NULL == it->mParamName.compare(TSE_CONFIG_MODULENAME) 
				|| NULL == it->mParamName.compare(TSE_CONFIG_REALNAME)) continue;
			ptrTestSuiteImpl->TSE_AddParam(it->mParamName.c_str(),it->mParamValue.c_str());
		}
	}

    void CTSE_SequenceTest::InitTestSuiteClassConfigParam(CTSE_TestBase* ptrTestSuiteImpl,std::shared_ptr<s_suite_item_test_config> ptrSuiteItemTestConfig)
    {
        for (std::vector<s_suite_single_item_test_config>::iterator it = ptrSuiteItemTestConfig->m_mTestParam.begin();
            it != ptrSuiteItemTestConfig->m_mTestParam.end();
            ++ it)
        {
            if (NULL == it->mParamName.compare(TSE_CONFIG_MODULENAME) 
                || NULL == it->mParamName.compare(TSE_CONFIG_REALNAME)) continue;
            ptrTestSuiteImpl->TSE_AddParam(it->mParamName.c_str(),it->mParamValue.c_str());
        }
    }

    bool CTSE_SequenceTest::LoadTSETestModule(std::wstring& strTestSuiteModuleName,std::wstring& strTestSuiteModuleFilePath)
    {
        std::wstring wstrModuleName,wstrSrcModuleName = JGW_RealativePathToAbsPath(strTestSuiteModuleName.c_str());
        //! 模块名称是否已添加后缀，确定最后的小数点位置
        int extDotPos = wstrSrcModuleName.rfind('.');
        if(extDotPos == std::wstring::npos)
        {
            wstrSrcModuleName += L".dll";
            extDotPos = wstrSrcModuleName.rfind('.');
        }
        //! 是否使用多路测试方案 MULTIUNIT_TSE_TEST  多路测试全局变量
        if(mGlobalEnvironment.GetBool(MULTIUNIT_TSE_TEST))
        {
            std::wstring ss;
            FormatWString(ss,L"_%d",mnTokenID);
            wstrModuleName = wstrSrcModuleName;
            wstrModuleName.insert(extDotPos,ss.c_str());

			if (!JGW_FileExistsToFilePath(wstrModuleName.c_str()))
			{
				if(!CopyFileW(wstrSrcModuleName.c_str(),wstrModuleName.c_str(),FALSE ))
				{
					mLogServices.ErrorLogFormat(L"DLL file copy failed, please check whether the file exists(%s - s)",wstrSrcModuleName.c_str(),JGW_GetSystemErrorString( GetLastError() ).c_str());
					return false;
				}
			}
            //DeleteFileW(wstrModuleName.c_str());
            wstrSrcModuleName = wstrModuleName;
        }
        strTestSuiteModuleFilePath = wstrSrcModuleName;

        return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteModuleFilePath);
    }

    bool CTSE_SequenceTest::SetTSEModuleGlobalResourcesInfo(std::wstring strTSEModuleFilePath)
    {
        std::string strTSEModuleFilePathA = JGW_W2A(strTSEModuleFilePath,CP_UTF8); 
        const Poco::Manifest<CTSE_TestBase>* pManif = mTestSuitePocoClassLoader.GetClassLoaderModule().findManifest(strTSEModuleFilePathA);
        if (!pManif) return false;
        Poco::Manifest<CTSE_TestBase>::Iterator itm = pManif->find(TSE_MODULE_GLOBAL_RESOURCES_CLSSNAM);
        if (pManif->end() == itm) return true;
        std::shared_ptr<CTSE_TestBase> ptrTestBase (itm->create());
        //! 将service下发下去
        if (ptrTestBase)
        {
            ptrTestBase->TSE_AddParam(TSE_LOG_SERVICES,(const wchar_t*)&mLogServices);
            ptrTestBase->TSE_AddParam(TSE_GLOBAL_ENVIRONMENT,(const wchar_t*)&mGlobalEnvironment);
            ptrTestBase->TSE_AddParam(TSE_TESTSTATUS_SERVICES,(const wchar_t*)&mTestStatusService);
            mvSuiteGlobalResources.push_back(ptrTestBase);
        }
        return true;
    }

	bool CTSE_SequenceTest::ExecuteSingleSuiteProject(size_t testIndex,std::shared_ptr<S_SuiteProject_TestBase> ptrSuiteProjectTestBase)
	{
		bool bSigleResult = true;
		std::wstring strTestName = ptrSuiteProjectTestBase->mpsTestBase->TSE_GetTestName();
		try
		{
			m_vAlreadyRunSuiteIndex.push_back(testIndex);
			mTestStatusService.TestIndexChange(testIndex);
			if (!mbIsFinalizeTest) mTestStatusService.TestStatusChange(strTestName.c_str(),E_RUN_TEST_STATUS);
			//! 错误重测次数
			for (size_t i = 0;i < ptrSuiteProjectTestBase->mpsTestBase->TSE_GetErrorRetryTestCount();i ++)
			{           
				if (0 == i && !mbIsFinalizeTest) mTestStatusService.TestStatusChange(strTestName.c_str(),E_RUNING_TEST_STATUS);
				bSigleResult = true;
				bSigleResult &= ptrSuiteProjectTestBase->mpsTestBase->TSE_Run();
				bSigleResult &= ptrSuiteProjectTestBase->mpsTestBase->TSE_Exit();
                if (ptrSuiteProjectTestBase->mbReversedResult) bSigleResult = !bSigleResult;
                if (ptrSuiteProjectTestBase->mbIfFalse)
                {
                    //if (!mbIsFinalizeTest && (bSigleResult || i + 1 == ptrSuiteProjectTestBase->mpsTestBase->TSE_GetErrorRetryTestCount())) mTestStatusService.TestStatusChange(strTestName.c_str(),bSigleResult?E_PASS_TEST_STATUS:E_FAIL_TEST_STATUS);
                }
                else
                {
                    if (!mbIsFinalizeTest && (bSigleResult || i + 1 == ptrSuiteProjectTestBase->mpsTestBase->TSE_GetErrorRetryTestCount())) mTestStatusService.TestStatusChange(strTestName.c_str(),bSigleResult?E_PASS_TEST_STATUS:E_FAIL_TEST_STATUS);
                }
				if (bSigleResult) break;
			}
		}
		catch(...)
		{
			return false;
		}
		return bSigleResult;
	}

	bool CTSE_SequenceTest::ExecuteSuiteProjectTestBase(std::shared_ptr<S_SuiteProject_TestBase> ptrSuiteProjectTestBase)
	{
        //！ 如果为false 就不执行子序列，否则执行子序列测试
        if (!ptrSuiteProjectTestBase->mbIfFalse)
        {
            //if (dynamic_cast<CTSE_TestBase*>(ptrSuiteProjectTestBase->mpsTestBase))

            //! 执行自身测试
            if (!ExecuteSingleSuiteProject(ptrSuiteProjectTestBase->mnTestIndex,ptrSuiteProjectTestBase))
            {
                return false;
            }

            if (ptrSuiteProjectTestBase->mvChildTestBase.empty())
            {
                return true;
            }

            for (size_t i = 0;i < ptrSuiteProjectTestBase->mpsTestBase->TSE_GetErrorRetryTestCount();i++)
            {
                bool result = true;
                for (size_t index = 0;index < ptrSuiteProjectTestBase->mvChildTestBase.size();index++)
                {
                    //! 测试失败重新开始测试
                    result = ExecuteSuiteProjectTestBase(ptrSuiteProjectTestBase->mvChildTestBase[index]);
                    if (!result)
                    {
                        break;
                    }
                }
                //! 测试成功直接返回true
                if (result)
                {
                    return true;
                }
                //! if i + 1
                if (i + 1 < ptrSuiteProjectTestBase->mpsTestBase->TSE_GetErrorRetryTestCount())
                {
                    if (!ExecuteSingleSuiteProject(ptrSuiteProjectTestBase->mnTestIndex,ptrSuiteProjectTestBase))
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            //! 执行自身测试  返回ture就直接返回
            if (ExecuteSingleSuiteProject(ptrSuiteProjectTestBase->mnTestIndex,ptrSuiteProjectTestBase))
            {
                return true;
            }

            for (size_t i = 0;i < ptrSuiteProjectTestBase->mpsTestBase->TSE_GetErrorRetryTestCount();i++)
            {
                bool result = true;
                for (size_t index = 0;index < ptrSuiteProjectTestBase->mvChildTestBase.size();index++)
                {
                    //! 测试失败重新开始测试
                    result = ExecuteSuiteProjectTestBase(ptrSuiteProjectTestBase->mvChildTestBase[index]);
                    if (!result)
                    {
                        break;
                    }
                }
                //! 测试成功直接返回true
                if (result)
                {
                    return true;
                }
            }
        }
		return false;
	}

    bool CTSE_SequenceTest::ExecTSEAllSuiteTest(bool bErrorStop /* = true */)
    {
        bool result = true;
        m_vAlreadyRunSuiteIndex.clear();
        mbIsFinalizeTest = false;
        //! 结果默认为1
        mGlobalEnvironment.PutInt(TSE_TEST_RESULT,1);

		for (size_t nIndex = 0;nIndex < mvSuiteProjectTestBase.size();nIndex ++)
		{
			result &= ExecuteSuiteProjectTestBase(mvSuiteProjectTestBase[nIndex]);
			if (!bErrorStop) continue;
			if(!result) { mGlobalEnvironment.PutInt(TSE_TEST_RESULT,0); mbIsFinalizeTest = true;  ExecTSETestFinalize(); break; }
		}
        mbIsFinalizeTest = false;
		//for (size_t nIndex = 0;nIndex < m_vSuiteTestCommand.size();nIndex ++)
		//{
		//	result &= ExecTSETestToIndex(nIndex);
		//	//! m_vAlreadyRunSuiteIndex.push_back(nIndex);
		//	if (!bErrorStop) continue;
		//	if(!result) { mGlobalEnvironment.PutInt(TSE_TEST_RESULT,0);  ExecTSETestFinalize(); break; }
		//}
        return result;
    }

    bool CTSE_SequenceTest::ExecTSEConfigTest()
    {
        bool bSigleResult = true;
        for (size_t nIndex = 0;nIndex < m_vConfigViewTestCommand.size();nIndex ++)
        {
            bSigleResult = true;
            bSigleResult &= m_vConfigViewTestCommand[nIndex]->TSE_Run();
            bSigleResult &= m_vConfigViewTestCommand[nIndex]->TSE_Exit();
            if (!bSigleResult) return false;
        }
        return true;
    }

    bool CTSE_SequenceTest::TSEAutoTestNotEmpty()
    {
        return !m_vAutoTSETestCommand.empty();
    }

    bool CTSE_SequenceTest::ExecTSEAutoTest()
    {
        if (m_vAutoTSETestCommand.empty()) return true;
        bool bSigleResult = true;
        for (size_t nIndex = 0;nIndex < m_vAutoTSETestCommand.size();nIndex ++)
        {
            bSigleResult = true;
            bSigleResult &= m_vAutoTSETestCommand[nIndex]->TSE_Run();
            bSigleResult &= m_vAutoTSETestCommand[nIndex]->TSE_Exit();
            if (!bSigleResult) return false;
        }
        return true;
    }

    bool CTSE_SequenceTest::ExecTSETestFinalize()
    {
        for(size_t i = 0; i < m_vFinalizeTest.size();i ++)
        {
            if( -1 == find_value(m_vAlreadyRunSuiteIndex,m_vFinalizeTest.at(i)) )
            {
                ExecTSETestToIndex(m_vFinalizeTest.at(i));
            }
        }
        return true;
    }

	CTSE_TestBase* CTSE_SequenceTest::GetTestBaseToIndex(size_t index)
	{
		if (index >= mvTSETestBase.size())
		{
			return NULL;
		}
		return mvTSETestBase[index];
	}

    std::shared_ptr<S_SuiteProject_TestBase> CTSE_SequenceTest::GetSuiteProjectTestBaseToIndex(size_t nIndex,std::vector<std::shared_ptr<S_SuiteProject_TestBase>>& vsSuiteProjectTestBase)
    {
        for (size_t i = 0;i < vsSuiteProjectTestBase.size();i ++)
        {
            if (vsSuiteProjectTestBase[i]->mnTestIndex == nIndex)
            {
                return vsSuiteProjectTestBase[i];
            }
            std::shared_ptr<S_SuiteProject_TestBase> ptrSuiteProject_TestBase = GetSuiteProjectTestBaseToIndex(nIndex,vsSuiteProjectTestBase[i]->mvChildTestBase);
            if (ptrSuiteProject_TestBase)
            {
                return ptrSuiteProject_TestBase;
            }
        }
        return NULL;
    }

    bool CTSE_SequenceTest::ExecTSETestToIndex(size_t nIndex)
    {
        /*CTSE_TestBase* pTestBase = GetTestBaseToIndex(nIndex);
        if (NULL == pTestBase)
        {
        mLogServices.ErrorLog(L"The test Index exceeds the test sequence range");
        return false;
        }*/
        bool bReversedResult = false;
        std::shared_ptr<S_SuiteProject_TestBase> ptrSuiteProject_TestBase = GetSuiteProjectTestBaseToIndex(nIndex,mvSuiteProjectTestBase);
        if (!ptrSuiteProject_TestBase)
        {
            mLogServices.ErrorLog(L"The test Index exceeds the test sequence range");
            return false;
        }
		return ExecuteSuiteProjectTestBase(ptrSuiteProject_TestBase);
        //IS_OK(!(nIndex >= m_vSuiteTestCommand.size()),L"测试索引超过测试序列范围");
        //m_vAlreadyRunSuiteIndex.push_back(nIndex);
        //bool bSigleResult = true;
        //try
        //{
        //    mTestStatusService.TestIndexChange(nIndex);
        //    mTestStatusService.TestStatusChange(m_vSuiteTestCommand[nIndex]->TSE_GetTestName().c_str(),E_RUN_TEST_STATUS);
        //    //! 错误重测次数
        //    for (size_t i = 0;i < m_vSuiteTestCommand[nIndex]->TSE_GetErrorRetryTestCount();i ++)
        //    {           
        //        if (0 != i) mTestStatusService.TestStatusChange(m_vSuiteTestCommand[nIndex]->TSE_GetTestName().c_str(),E_RUNING_TEST_STATUS);
        //        bSigleResult = true;
        //        bSigleResult &= m_vSuiteTestCommand[nIndex]->TSE_Run();
        //        bSigleResult &= m_vSuiteTestCommand[nIndex]->TSE_Exit();
        //        mTestStatusService.TestStatusChange(m_vSuiteTestCommand[nIndex]->TSE_GetTestName().c_str(),bSigleResult?E_PASS_TEST_STATUS:E_FAIL_TEST_STATUS);
        //        if (bSigleResult) break;
        //    }
        //}
        //catch(...)
        //{
        //    return false;
        //}

        //return bSigleResult;
    }

    const wchar_t* CTSE_SequenceTest::GetTSETestNameToIndex(size_t nIndex)
    {
        if (nIndex >= mvTSETestBase.size()) return L"The test Index exceeds the test sequence range";
        return mvTSETestBase[nIndex]->TSE_GetTestName().c_str();
    }

    void CTSE_SequenceTest::ClearTSEExistedTestArrayIndex()
    {
        m_vAlreadyRunSuiteIndex.clear();
    }

    bool CTSE_SequenceTest::InitTSETestConfig(void)
    {
        bool result = true;
        for (size_t nIndex = 0;nIndex < mvTSETestBase.size();nIndex ++)
        {

            for (size_t i = 0;i < 1;i ++)
            {
                mTestStatusService.TestStatusChange(mvTSETestBase[nIndex]->TSE_GetTestName().c_str(),E_INIT_TEST_STATUS);
                try
                {
                    result &= mvTSETestBase[nIndex]->TSE_Init();
                }
                catch (...)
                {
                    continue;
                }
            }       
        }
        return result;
    }

    bool CTSE_SequenceTest::ExecTSEGroupTestToIndex(size_t index)
    {
        IS_OK(!(index >= mvsTSETestIndexGroups.size() || index < 0),L"The test Index exceeds the test sequence range");
        //! 重新组合需要运行的测试索引
        PS_TSE_TEST_INDEX_GROUPS psTSETestIndexGroups = &mvsTSETestIndexGroups[index];
        std::vector<size_t> vTSERunTestIndex;
        for (std::vector<std::vector<S_TSE_TEST_GROUP>*>::iterator it = psTSETestIndexGroups->mvpsTSETestIndexGroups.begin();
            it != psTSETestIndexGroups->mvpsTSETestIndexGroups.end();
            ++ it)
        {
            for (std::vector<S_TSE_TEST_GROUP>::iterator ittse = it[0]->begin();
                ittse != it[0]->end();
                ++ ittse)
            {
                if (index < ittse->mnTestIndex) break;
                //! 添加当前测试项目
                if (index == ittse->mnTestIndex || 1 == ittse->mnPriority) 
                {
                    //! 单个测试项目只能添加一次测试
                    if (-1 == find_value(vTSERunTestIndex,ittse->mnTestIndex)) vTSERunTestIndex.push_back(ittse->mnTestIndex);
                }
            }
        }
        //! 排序  测试suite必须按照顺序执行 从小到大
        sort(vTSERunTestIndex.begin(), vTSERunTestIndex.end());

        bool bResult = true;
        for (size_t i = 0;i < vTSERunTestIndex.size() && bResult;i ++)
        {
            if (-1 != find_value(m_vAlreadyRunSuiteIndex,vTSERunTestIndex[i])) continue;
            bResult &= ExecTSETestToIndex(vTSERunTestIndex[i]);
            //! 是否是销毁测试项目
            if (-1 == find_value(m_vFinalizeTest,i))
            {
                if (bResult) m_vAlreadyRunSuiteIndex.push_back(i);
            }
            else
            {
                m_vAlreadyRunSuiteIndex.push_back(i);
            }
        }

        return bResult;
    }
}