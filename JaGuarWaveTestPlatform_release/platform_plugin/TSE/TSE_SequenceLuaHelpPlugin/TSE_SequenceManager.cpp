#include "StdAfx.h"
#include "TSE_SequenceManager.h"

namespace JGW
{
    CTSE_SequenceManager::CTSE_SequenceManager(int nIndex) : mnIndex(0)
    {
    }


    CTSE_SequenceManager::~CTSE_SequenceManager(void)
    {
		mvTSETestBase.clear();
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

	void CTSE_SequenceManager::AddLogListener(CTSE_LogListener* pLogListener)
	{
		mcLogServicesImpl.AddListener(pLogListener);
	}

	void CTSE_SequenceManager::AddTestStatusListener(CTSE_TestStatusListener* mpTestStatusListener)
	{
		mcTestStatusServiceImpl.AddListener(mpTestStatusListener);
	}

	CTSE_GlobalEnvironmentImpl* CTSE_SequenceManager::GetGlobalEnvironmentImpl()
	{
		return &mcGlobalEnvironmentImpl;
	}

	CTSE_TestStatusServiceImpl* CTSE_SequenceManager::GetTestStatusServiceImpl()
	{
		return &mcTestStatusServiceImpl;
	}

	CTSE_TestBase* CTSE_SequenceManager::CreateSequence(const std::wstring& strModuleName,const std::string& strRealName,const std::wstring& strTestName)
	{
		std::wstring strTestSuiteModuleFilePath;
		//! 1、加载TSE模块
		if (!LoadTSETestModule(strModuleName,strTestSuiteModuleFilePath))
		{
			return NULL;
		}
		//! 2、设置log、全局变量、以及测试状态类指针
		if (!SetTSEModuleGlobalResourcesInfo(strTestSuiteModuleFilePath))
		{
			return NULL;
		}
		//! 3、加载TSE测试类
		CTSE_TestBase* pTestSuiteImpl (const_cast<CTSE_TestBase*>(mTestSuitePocoClassLoader.CreateTestSuitePtrClass(strRealName)));
		if (!pTestSuiteImpl)
		{
			mcLogServicesImpl.ErrorLogFormat(L"Class Name(%s) is not Find To Dll!!!",strRealName.c_str());
			return NULL;
		}
		pTestSuiteImpl->TSE_AddParam(TSE_CONFIG_TEST_NAME,strTestName.c_str());
		mvTSETestBase.push_back(pTestSuiteImpl);

		return pTestSuiteImpl;
	}

	const std::wstring& CTSE_SequenceManager::GetErrorMessage()
	{
		return mstrErrorMsg;
	}

	bool CTSE_SequenceManager::LoadTSETestModule(const std::wstring& strTestSuiteModuleName,std::wstring& strTestSuiteModuleFilePath)
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
		if(mcGlobalEnvironmentImpl.GetBool(MULTIUNIT_TSE_TEST))
		{
			std::wstring ss;
			FormatWString(ss,L"_%d",mnIndex);
			wstrModuleName = wstrSrcModuleName;
			wstrModuleName.insert(extDotPos,ss.c_str());

			if (!JGW_FileExistsToFilePath(wstrModuleName.c_str()))
			{
				if(!CopyFileW(wstrSrcModuleName.c_str(),wstrModuleName.c_str(),FALSE ))
				{
					mcLogServicesImpl.ErrorLogFormat(L"DLL file copy failed, please check whether the file exists(%s - s)",wstrSrcModuleName.c_str(),JGW_GetSystemErrorString( GetLastError() ).c_str());
					return false;
				}
			}
			//DeleteFileW(wstrModuleName.c_str());
			wstrSrcModuleName = wstrModuleName;
		}
#if 0
		//! 是否使用多路测试方案 MULTIUNIT_TSE_TEST  多路测试全局变量
		if(mnIndex > 0)
		{
			std::wstring ss;
			FormatWString(ss,L"_%d",mnIndex);
			wstrModuleName = wstrSrcModuleName;
			wstrModuleName.insert(extDotPos,ss.c_str());
			DeleteFileW(wstrModuleName.c_str());

			if(!CopyFileW(wstrSrcModuleName.c_str(),wstrModuleName.c_str(),FALSE ))
			{
				mcLogServicesImpl.ErrorLogFormat(L"DLL file copy failed, please check whether the file exists(%s)",wstrSrcModuleName.c_str());
				return false;
			}
			wstrSrcModuleName = wstrModuleName;
		}
#endif
		strTestSuiteModuleFilePath = wstrSrcModuleName;

		return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteModuleFilePath);
	}

	bool CTSE_SequenceManager::SetTSEModuleGlobalResourcesInfo(const std::wstring& strTSEModuleFilePath)
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
			ptrTestBase->TSE_AddParam(TSE_LOG_SERVICES,(const wchar_t*)&mcLogServicesImpl);
			ptrTestBase->TSE_AddParam(TSE_GLOBAL_ENVIRONMENT,(const wchar_t*)&mcGlobalEnvironmentImpl);
			ptrTestBase->TSE_AddParam(TSE_TESTSTATUS_SERVICES,(const wchar_t*)&mcTestStatusServiceImpl);
			mvSuiteGlobalResources.push_back(ptrTestBase);
		}
		return true;
	}
















	//CTSE_GlobalEnvironmentImpl* CTSE_SequenceManager::GetGlobalEnvironmentImpl()
	//{
	//	return mpGlobalEnvironmentImpl;
	//}

	//CTSE_TestStatusServiceImpl* CTSE_SequenceManager::GetTestStatusServiceImpl()
	//{
	//	return mpTestStatusServiceImpl;
	//}

	//CTSE_LogServicesImpl* CTSE_SequenceManager::GetLogServicesImpl()
	//{
	//	return mpLogServicesImpl;
	//}

	//void CTSE_SequenceManager::SetGlobalEnvironmentImpl(CTSE_GlobalEnvironmentImpl* pGlobalEnvironmentImpl)
	//{
	//	mpGlobalEnvironmentImpl = pGlobalEnvironmentImpl;
	//}

	//void CTSE_SequenceManager::SetLogServicesImpl(CTSE_LogServicesImpl* pLogServicesImpl)
	//{
	//	mpLogServicesImpl = pLogServicesImpl;
	//}

	//void CTSE_SequenceManager::SetTestStatusServiceImpl(CTSE_TestStatusServiceImpl* pTestStatusServiceImpl)
	//{
	//	mpTestStatusServiceImpl = pTestStatusServiceImpl;
	//}
}
