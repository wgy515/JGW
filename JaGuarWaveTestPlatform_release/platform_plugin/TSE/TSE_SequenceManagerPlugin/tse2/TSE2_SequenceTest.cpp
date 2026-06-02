#include "StdAfx.h"
#include "TSE2_SequenceTest.h"
#include "TSE2_XmlConfig.h"
namespace JGW
{
    CTSE2_SequenceTest::CTSE2_SequenceTest(CTSE_LogServices& logServices,CTSE_TestStatusService& testStatusService,CTSE_GlobalEnvironment& globalEnvironment,int token_id) : mLogServices(logServices),mTestStatusService(testStatusService),mGlobalEnvironment(globalEnvironment),mnTokenID(token_id),mnCurrentTestIndex(0),mbIsFinalizeTest(false),mbInitTSETest(false),mnPrevTestIndex(0)
    {
    }


    CTSE2_SequenceTest::~CTSE2_SequenceTest(void)
    {
        UnLoadTSETestPlugin();
    }

    bool CTSE2_SequenceTest::LoadTSESuiteConfig(const std::string& strResourceConfig)
    {
        //! init tse2 test
        mnCurrentTestIndex = 0;
        mnPrevTestIndex = 0;
        mbIsFinalizeTest = false;
        mbInitTSETest = false;
        mstrPrevModulePath = "";
        m_vFinalizeTest.clear();
        msTse2Struct.mmapGlobalVariables.clear();
        msTse2Struct.msAutoTestStruct.msTse2Test.clear();
        msTse2Struct.msRootTestStruct.msTse2Test.clear();
        msTse2Struct.msUiConfigTestStruct.msTse2Test.clear();
        mvSuiteGlobalResources.clear();
        mvstrTSEModulePath.clear();

        mGlobalEnvironment.PutString(TSE_SUITE_FILE_PATH,JGW_A2W(strResourceConfig));
        mGlobalEnvironment.PutString(L"TSE_EXEC_FOLDER",JGW_GetApplicationFolder());

        CTSE2_XmlConfig xmlConfig(&mLogServices);
        if (!xmlConfig.LoadXmlConfig(strResourceConfig,msTse2Struct))
        {
            return false;
        }

        for (std::map<std::wstring,std::wstring>::iterator it = msTse2Struct.mmapGlobalVariables.begin();
            it != msTse2Struct.mmapGlobalVariables.end();++it)
        {
            mGlobalEnvironment.PutString(it->first,it->second);
        }

        return LoadTestSuiteModuleClassPtr(msTse2Struct.msAutoTestStruct.msTse2Test) && LoadTestSuiteModuleClassPtr(msTse2Struct.msUiConfigTestStruct.msTse2Test) && LoadTestSuiteModuleClassPtr(msTse2Struct.msRootTestStruct.msTse2Test,true);
    }
    //! 1、加载TSE MODULE
    //! 2、
    bool CTSE2_SequenceTest::LoadTestSuiteModuleClassPtr(std::vector<S_TSE2_TEST>& sTseTest,bool bIsRootTestNode /* = false */)
    {
        bool result = true;
        for (std::vector<S_TSE2_TEST>::iterator it = sTseTest.begin();it != sTseTest.end();++ it)
        {
            it->mnTestIndex = mnCurrentTestIndex++;
            //! 是测试目录
            if (it->mbIsFolderTest)
            {
                result = LoadTestSuiteModuleClassPtr(it->msFolderTest.mvTseTest,bIsRootTestNode);
            }
            else
            {
                //! 1、加载TSE MODULE
                if (!LoadTSEPlugin(it->msTestBase.mstrModuleName))
                {
                    return false;
                }
                //! 2、加载TSE实例类
                CTSE_TestBase* pTestBase = LoadSingeTSEPtrClassPlugin(mstrPrevModulePath,it[0].msTestBase);
                if (NULL == pTestBase)
                {
                    return false;
                }

                if (it->msTestBase.mbIsFinalize)
                {
                    m_vFinalizeTest.push_back(&it[0]);
                }

                it->msTestBase.mptrTestBase.reset(pTestBase);
                result = LoadTestSuiteModuleClassPtr(it->msTestBase.mvTseTest,bIsRootTestNode);
            }
            if (!result)
            {
                return false;
            }
        }
        return true;
    }

    bool CTSE2_SequenceTest::LoadTSEPlugin(const std::wstring& strModuleName)
    {
        //! 1、加载TSE MODULE
        if (!LoadTSEModule(strModuleName))
        {
            return false;
        } 
        //! 2、检查TSE模块名称是否为空
        if (strModuleName.empty())
        {
            return true;
        }
        //！检查当前模块是否已经初始化GlobalResources模块
        for (size_t index = 0;index < mvstrTSEModulePath.size();index++)
        {
            if (0 == JGW_StrComparenoCaseStr(mstrPrevModulePath.c_str(),mvstrTSEModulePath[index].c_str()))
            {
                return true;
            }
        }
        //! 初始化GlobalResources模块
        if (!SetTSEModuleGlobalResourcesInfo())
        {
            mLogServices.ErrorLog(L"SetTSEModuleGlobalResourcesInfo Fail");
            return false;
        }
        mvstrTSEModulePath.push_back(mstrPrevModulePath);
        return true;
    }

    bool CTSE2_SequenceTest::SetTSEModuleGlobalResourcesInfo()
    {
        std::string strTSEModuleFilePathA = mstrPrevModulePath;
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

    CTSE_TestBase* CTSE2_SequenceTest::LoadSingeTSEPtrClassPlugin(const std::string& strModuleName,S_TEST_BASE& sTestBase)
    {
        std::string strRealName = JGW_W2A(sTestBase.mstrRealName,CP_UTF8);
        if (strRealName.empty())
        {
            mLogServices.ErrorLog(L"Class Name is not Empty!!!");
            return NULL;
        }
        CTSE_TestBase* pTestBase = NULL;
        try
        {
            pTestBase = mTestSuitePocoClassLoader.CreateTestSuitePtrClassToModuleName(strRealName,strModuleName);
        }
        catch (...)
        {
            mLogServices.ErrorLogFormat(L"Class Name(%s) create Fail!!!",sTestBase.mstrRealName.c_str());
            return NULL;
        }
        if (NULL == pTestBase)
        {
            mLogServices.ErrorLogFormat(L"Class Name(%s) is not Find To Dll!!!",sTestBase.mstrRealName.c_str());
            return NULL;
        }
        //! TSE ADD PARAM
        for (std::map<std::wstring,std::wstring>::iterator it = sTestBase.mmapParam.begin();
            it != sTestBase.mmapParam.end();
            ++ it)
        {
            if (NULL == it->first.compare(TSE_CONFIG_MODULENAME) 
                || NULL == it->first.compare(TSE_CONFIG_REALNAME)) continue;
            pTestBase->TSE_AddParam(it->first.c_str(),it->second.c_str());
        }
        pTestBase->TSE_Init();
        return pTestBase;
    }

    bool CTSE2_SequenceTest::LoadTSEModule(const std::wstring& strModuleName)
    {
        if (strModuleName.empty())
        {
            if (mstrPrevModulePath.empty())
            {
                mLogServices.ErrorLog(L"Module name cannot be empty");
                return false;
            }
            return true;
        }
        std::wstring wstrModuleName,wstrSrcModuleName = JGW_RealativePathToAbsPath(strModuleName.c_str());
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
            DeleteFileW(wstrModuleName.c_str());

            if(!CopyFileW(wstrSrcModuleName.c_str(),wstrModuleName.c_str(),FALSE ))
            {
                mLogServices.ErrorLogFormat(L"DLL file copy failed, please check whether the file exists(%s)",wstrSrcModuleName.c_str());
                return false;
            }
            wstrSrcModuleName = wstrModuleName;
        }
        std::wstring strTestSuiteModuleFilePath = wstrSrcModuleName;
        mstrPrevModulePath = JGW_W2A(strTestSuiteModuleFilePath,CP_UTF8);
        return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteModuleFilePath);
    }

    bool CTSE2_SequenceTest::ExecuteSigleTestBase(S_TEST_BASE& sTestBase)
    {
        bool bSigleResult = true;
        std::wstring strTestName = sTestBase.mstrTestName;
        try
        {
            for (std::map<std::wstring,std::wstring>::iterator it = mmapLoopParam.begin();
                it != mmapLoopParam.end();
                ++ it)
            {
                sTestBase.mptrTestBase->TSE_AddParam(it->first.c_str(),it->second.c_str());
                //strTestName += JGW_GetFormatWString(L" %s_%s_",it->first.c_str(),it->second.c_str());
            }
            
            if (mmapLoopParam.size() > 0)
            {
                strTestName += mstrLoopParamShow;
            }

            if (mbInitTSETest)
            {
                mTestStatusService.TestIndexChange(mnCurrentTestIndex);
                mTestStatusService.TestStatusChange(strTestName.c_str(),E_INIT_TEST_STATUS);
                return true;
            }

            mTestStatusService.TestIndexChange(mnCurrentTestIndex);
            if (!mbIsFinalizeTest) mTestStatusService.TestStatusChange(strTestName.c_str(),E_RUN_TEST_STATUS);

            //! 错误重测次数
            for (size_t i = 0;i < sTestBase.mptrTestBase->TSE_GetErrorRetryTestCount();i ++)
            {           
                if (0 == i && !mbIsFinalizeTest) mTestStatusService.TestStatusChange(strTestName.c_str(),E_RUNING_TEST_STATUS);
                bSigleResult = true;
                bSigleResult &= sTestBase.mptrTestBase->TSE_Run();
                bSigleResult &= sTestBase.mptrTestBase->TSE_Exit();
                if (sTestBase.mbReversedResult) bSigleResult = !bSigleResult;
                if (sTestBase.mbIfFalse)
                {
                    //if (!mbIsFinalizeTest && (bSigleResult || i + 1 == ptrSuiteProjectTestBase->mpsTestBase->TSE_GetErrorRetryTestCount())) mTestStatusService.TestStatusChange(strTestName.c_str(),bSigleResult?E_PASS_TEST_STATUS:E_FAIL_TEST_STATUS);
                }
                else
                {
                    if (!mbIsFinalizeTest && (bSigleResult || i + 1 == sTestBase.mptrTestBase->TSE_GetErrorRetryTestCount())) mTestStatusService.TestStatusChange(strTestName.c_str(),bSigleResult?E_PASS_TEST_STATUS:E_FAIL_TEST_STATUS);
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

    bool CTSE2_SequenceTest::ExecuteTSE2Test(S_TSE2_TEST& sTse2Test)
    {
        if (sTse2Test.mbIsFolderTest)
        {
            {
                std::vector<std::wstring> vstrParamHeader;
                JGW_ParserStrW(sTse2Test.msFolderTest.mstrParamHeader.c_str(),L",",vstrParamHeader);
                if (!sTse2Test.msFolderTest.mstrParamHeaderEnvironment.empty())
                {
                    sTse2Test.msFolderTest.mvstrParamValues.clear();
                    std::vector<std::wstring> vstrParamHeaderEnvironment;
                    JGW_ParserStrW(sTse2Test.msFolderTest.mstrParamHeaderEnvironment.c_str(),L",",vstrParamHeaderEnvironment);
                    while (vstrParamHeaderEnvironment.empty()) vstrParamHeaderEnvironment.push_back(L"Def");

                    std::vector<std::wstring> vstrFristParamHeader;
                    JGW_ParserStrW(mGlobalEnvironment.GetString(vstrParamHeaderEnvironment[0]).c_str(),L",",vstrFristParamHeader);

                    for (size_t index = 1;index < vstrParamHeaderEnvironment.size();index++)
                    {
                        std::vector<std::wstring> vstrTempParamHeader;
                        JGW_ParserStrW(mGlobalEnvironment.GetString(vstrParamHeaderEnvironment[index]).c_str(),L",",vstrTempParamHeader);
                        while (vstrTempParamHeader.empty()) vstrTempParamHeader.push_back(L"Def");
                        while (vstrFristParamHeader.size() > vstrTempParamHeader.size()) vstrTempParamHeader.push_back(vstrTempParamHeader[0]);
                        for (size_t fristIndex = 0;fristIndex < vstrTempParamHeader.size();fristIndex++)
                        {
                            vstrFristParamHeader[fristIndex] += L",";
                            vstrFristParamHeader[fristIndex] += vstrTempParamHeader[fristIndex];
                        }
                    }
                    
                    for (size_t fristIndex = 0;fristIndex < vstrFristParamHeader.size();fristIndex++)
                    {
                        sTse2Test.msFolderTest.mvstrParamValues.push_back(vstrFristParamHeader[fristIndex]);
                    }
                }
                for (size_t index = 0;index < sTse2Test.msFolderTest.mvstrParamValues.size();index++)
                {
                    //! 
                    std::vector<std::wstring> vstrTemp;
                    JGW_ParserStrW(sTse2Test.msFolderTest.mvstrParamValues[index].c_str(),L",",vstrTemp);

                    mstrLoopParamShow = sTse2Test.msFolderTest.mstrParamHeaderShow;
                    for (size_t paramCount = 0;paramCount < vstrParamHeader.size();paramCount++)
                    {
                        mmapLoopParam[vstrParamHeader[paramCount]] = vstrTemp.size() > paramCount ? vstrTemp[paramCount] : L"0";
                        std::wstring strTemp;
                        JGW_FormatWString(strTemp,L"$$_%s_$$",vstrParamHeader[paramCount].c_str());
                        JGW_ReplaceStringW(mstrLoopParamShow,strTemp,mmapLoopParam[vstrParamHeader[paramCount]]);
                    }
					
                    for (std::vector<S_TSE2_TEST>::iterator it = sTse2Test.msFolderTest.mvTseTest.begin();
                        it != sTse2Test.msFolderTest.mvTseTest.end();
                        ++ it)
                    {
                        if (!ExecuteTSE2Test(it[0]))
                        {
                            return false;
                        }
                    }
                }
            }
            mmapLoopParam.clear();
        }
        else
        {
            mnCurrentTestIndex = sTse2Test.mnTestIndex;
            if (!ExecuteSigleTestBase(sTse2Test.msTestBase))
            {
                //! 1 : 如果测试结果为false 则运行子测试序列
                if (!sTse2Test.msTestBase.mbIfFalse)
                {
                    return false;
                }     
            }
            else
            {
                if (sTse2Test.msTestBase.mbIfFalse)
                {
                    return true;
                }
            }

            for (std::vector<S_TSE2_TEST>::iterator it = sTse2Test.msTestBase.mvTseTest.begin();
                it != sTse2Test.msTestBase.mvTseTest.end();
                ++ it)
            {
                if (!ExecuteTSE2Test(it[0]))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool CTSE2_SequenceTest::ExecTSEAllSuiteTest(bool bErrorStop /* = true */)
    {
        mbIsFinalizeTest = false;
        bool result = true;
        mGlobalEnvironment.PutInt(TSE_TEST_RESULT,1);
        for (std::vector<S_TSE2_TEST>::iterator it = msTse2Struct.msRootTestStruct.msTse2Test.begin();
            it != msTse2Struct.msRootTestStruct.msTse2Test.end();
            ++ it)
        {
            mmapLoopParam.clear();
            result = ExecuteTSE2Test(it[0]);
            if (!bErrorStop) continue;
            if(!result) { mGlobalEnvironment.PutInt(TSE_TEST_RESULT,0); mbIsFinalizeTest = true;  ExecTSETestFinalize(); break; }
        }
        mbIsFinalizeTest = false;
        return result;
    }

    bool CTSE2_SequenceTest::ExecTSEConfigTest()
    {
        mbIsFinalizeTest = true;
        for (std::vector<S_TSE2_TEST>::iterator it = msTse2Struct.msUiConfigTestStruct.msTse2Test.begin();
            it != msTse2Struct.msUiConfigTestStruct.msTse2Test.end();
            ++ it)
        {
            mmapLoopParam.clear();
            ExecuteTSE2Test(it[0]);
        }
        mbIsFinalizeTest = false;
        return true;
    }

    bool CTSE2_SequenceTest::ExecTSEAutoTest()
    {
        mbIsFinalizeTest = false;
        for (std::vector<S_TSE2_TEST>::iterator it = msTse2Struct.msAutoTestStruct.msTse2Test.begin();
            it != msTse2Struct.msAutoTestStruct.msTse2Test.end();
            ++ it)
        {
            mmapLoopParam.clear();
            if (!ExecuteTSE2Test(it[0]))
            {
                return false;
            }
        }
        return true;
    }

    bool CTSE2_SequenceTest::ExecTSETestToIndex(size_t index)
    {
        return false;
    }

    bool CTSE2_SequenceTest::ExecTSEGroupTestToIndex(size_t index)
    {
        return false;
    }

    bool CTSE2_SequenceTest::ExecTSETestFinalize(void)
    {
        mbIsFinalizeTest = true;
        mnPrevTestIndex = mnCurrentTestIndex;
        for (std::vector<S_TSE2_TEST*>::iterator it = m_vFinalizeTest.begin();
            it != m_vFinalizeTest.end();
            ++ it)
        {
            mmapLoopParam.clear();
            if (it[0]->mnTestIndex > mnPrevTestIndex)
            {
                ExecuteTSE2Test(*it[0]);
            }        
        }
        return true;
    }

    bool CTSE2_SequenceTest::InitTSETestConfig(void)
    {
        mbInitTSETest = true;
        for (std::vector<S_TSE2_TEST>::iterator it = msTse2Struct.msUiConfigTestStruct.msTse2Test.begin();
            it != msTse2Struct.msUiConfigTestStruct.msTse2Test.end();
            ++ it)
        {
            mmapLoopParam.clear();
            ExecuteTSE2Test(it[0]);
        }

        for (std::vector<S_TSE2_TEST>::iterator it = msTse2Struct.msAutoTestStruct.msTse2Test.begin();
            it != msTse2Struct.msAutoTestStruct.msTse2Test.end();
            ++ it)
        {
            mmapLoopParam.clear();
            ExecuteTSE2Test(it[0]);
        }

        for (std::vector<S_TSE2_TEST>::iterator it = msTse2Struct.msRootTestStruct.msTse2Test.begin();
            it != msTse2Struct.msRootTestStruct.msTse2Test.end();
            ++ it)
        {
            mmapLoopParam.clear();
            ExecuteTSE2Test(it[0]);
        }

        mbInitTSETest = false;
        return true;
    }

    bool CTSE2_SequenceTest::TSEAutoTestNotEmpty()
    {
        return !msTse2Struct.msAutoTestStruct.msTse2Test.empty();
    }

    void CTSE2_SequenceTest::ClearTSEExistedTestArrayIndex()
    {
        mnCurrentTestIndex = 0;
    }

    const wchar_t* CTSE2_SequenceTest::GetTSETestNameToIndex(size_t nIndex)
    {
        return L"";
    }

    void CTSE2_SequenceTest::UnLoadTSETestPlugin()
    {
        if (mnCurrentTestIndex != 0) ExecTSETestFinalize();
        mbIsFinalizeTest = false;
        mnCurrentTestIndex = 0;
        mnPrevTestIndex = 0;
        mstrPrevModulePath = "";
        mvSuiteGlobalResources.clear();
        m_vFinalizeTest.clear();
        mvstrTSEModulePath.clear();
        msTse2Struct.mmapGlobalVariables.clear();
        msTse2Struct.msAutoTestStruct.msTse2Test.clear();
        msTse2Struct.msRootTestStruct.msTse2Test.clear();
        msTse2Struct.msUiConfigTestStruct.msTse2Test.clear();

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
}

