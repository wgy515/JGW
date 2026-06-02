#include "StdAfx.h"
#include "TSE_LuaSequenceTest.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "TSE_GlobalEnvironmentConfig.h"
#include "TSE_LuaSequenceManage.h"
#include <map>
namespace JGW
{
	CTSE_LuaSequenceTest::CTSE_LuaSequenceTest(unsigned long ulTokenID,CTSE_LogListener* pLogListener,CTSE_TestStatusListener* pTestStatusListener,CTSE_SequenceManageImpl* pSequenceManageImpl,CCJGW_LuaHelp& cLuaHelp) : mulTokenID(ulTokenID),mcLuaHelp(cLuaHelp),mpLogListener(pLogListener),mpTestStatusListener(pTestStatusListener),mpGlobalEnvironmentImpl(NULL),mpSequenceManageImpl(pSequenceManageImpl)
	{

	}


	CTSE_LuaSequenceTest::~CTSE_LuaSequenceTest(void)
	{
	}

	CTSE_GlobalEnvironmentImpl* CTSE_LuaSequenceTest::GetGlobalEnvironmentImpl()
	{
		if (NULL != mpGlobalEnvironmentImpl) return mpGlobalEnvironmentImpl;
		std::string strGlobalEnvironmentPoint = JGW::JGW_GetFormatString("GlobalEnvironmentPoint%d",(int)mulTokenID);
		mpGlobalEnvironmentImpl = (CTSE_GlobalEnvironmentImpl*)mcLuaHelp.GetLuaGlobalVariableInteger(strGlobalEnvironmentPoint,NULL);
		return mpGlobalEnvironmentImpl;
	}

	bool CTSE_LuaSequenceTest::LoadTSESuiteConfig(const std::string& strResourceConfig)
	{
		mcLuaHelp.UnInitLuaEnvironment();
		if (NULL == mpLogListener || NULL == mpTestStatusListener)
		{
			Log4WE(L"LogListener and TestStatusListener Instances cannot be empty");
			return false;
		}

		if (!JGW_FileExistsToFilePathA(strResourceConfig.c_str()))
		{
			mpLogListener->OnErrorLog(JGW_GetFormatWString(L"The configuration file(%s) does not exist",JGW_A2W(strResourceConfig).c_str()).c_str());
			return false;
		}

		if (!mcLuaHelp.InitLuaEnvironment())
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}
		//! G:\git_code_debug\FTM-tools\JaGuarWaveTestPlatform\bin\debug\exec_debug\新建文件夹\Test6.lua
		if (!mcLuaHelp.LoadLuaFile(strResourceConfig.c_str()))
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}
		//! 注册实例指针
		std::string strTestStatusServicePoint = JGW_GetFormatString("TestStatusServicePoint%d",(int)mulTokenID);
		mcLuaHelp.SetLuaGlobalVariableInteger(strTestStatusServicePoint,(int)mpTestStatusListener);
		std::string strLogServicesPoint = JGW::JGW_GetFormatString("LogServicesPoint%d",(int)mulTokenID);
		mcLuaHelp.SetLuaGlobalVariableInteger(strLogServicesPoint,(int)mpLogListener);

		bool bResult = true;
		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("PreInitializationSequenceManager",(int)mulTokenID,bResult))
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			//std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
			return false;
		}

		if (!bResult)
		{
			mpLogListener->OnErrorLog(L"Failed to load the configuration file");
			return false;
		}

		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("CreateTestID",(int)mulTokenID,bResult))
		{
			mpLogListener->OnErrorLog(L"Failed to obtain the instance of the global variable");
			//std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
			return false;
		}
		//std::wcout << L"CreateTestID : " << (result ? L"PASS" : L"FAIL") << std::endl;

		if (NULL == GetGlobalEnvironmentImpl())
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}

		((CTSE_LuaSequenceManage*)mpSequenceManageImpl)->SetGlobalEnvironmentImpl(mpGlobalEnvironmentImpl);
		//! LOAD <TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH>config\DAVINCI\check_suite.xml</TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH>
        std::string strEnvrionmentConfig (strResourceConfig);
        JGW_ReplaceStringA(strEnvrionmentConfig,"\\cache\\","\\config\\");
        mpGlobalEnvironmentImpl->PutAnsiString(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH",strEnvrionmentConfig);

		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("AddGlobalEnvironment",(int)mulTokenID,bResult))
		{
			//std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}

        {
            std::map<std::wstring,std::wstring> mapNameValue;
            CTSE_GlobalEnvironmentConfig cGlobalEnvironmentConfig(mpLogListener);
            cGlobalEnvironmentConfig.LoadGlobalEnvironmentConfigXml(mpGlobalEnvironmentImpl->GetString(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH"),mapNameValue);
            for (std::map<std::wstring,std::wstring>::iterator it = mapNameValue.begin();it != mapNameValue.end();++it)
            {
                mpGlobalEnvironmentImpl->PutString(it->first,it->second);
            }
        }


		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("AddParam",(int)mulTokenID,bResult))
		{
			//std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}

		return true;
	}

	bool CTSE_LuaSequenceTest::InitTSETestConfig()
	{
		bool bResult = true;
		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("InitTSETest",(int)mulTokenID,bResult))
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}
		return bResult;
	}

	bool CTSE_LuaSequenceTest::TSEAutoTestNotEmpty()
	{
		bool bResult = true;
		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("AutoTestNotEmpty",(int)mulTokenID,bResult))
		{
			//! mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}
		return bResult;
	}

	bool CTSE_LuaSequenceTest::ExecTSEAutoTest()
	{
		bool bResult = true;
		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("AutoTSETest",(int)mulTokenID,bResult))
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}
		return bResult;
	}

	bool CTSE_LuaSequenceTest::ExecTSEAllSuiteTest(bool bErrorStop /* = true */)
	{
		std::string strErrorStop = JGW::JGW_GetFormatString("ErrorStop%d",(int)mulTokenID);
		mcLuaHelp.SetLuaGlobalVariableInteger(strErrorStop,bErrorStop ? 1 : 0);

		bool result = true;
		try
		{
			if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("ExecuteTSETest",(int)mulTokenID,result))
			{
				//std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
				mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
				return false;
			}
		}
		catch (...)
		{
			result = false;
		}

		return result;
	}

	bool CTSE_LuaSequenceTest::ExecTSEConfigTest()
	{
		bool bResult = true;
		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("ExecuteTSEConfigTest",(int)mulTokenID,bResult))
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
			return false;
		}
		return bResult;
	}

	bool CTSE_LuaSequenceTest::ExecTSETestToIndex(size_t index)
	{
		return false;
	}

	bool CTSE_LuaSequenceTest::ExecTSEGroupTestToIndex(size_t index)
	{
		return false;
	}

	bool CTSE_LuaSequenceTest::ExecTSETestFinalize(void)
	{
		return false;
	}

	void CTSE_LuaSequenceTest::ClearTSEExistedTestArrayIndex()
	{
		return ;
	}

	const wchar_t* CTSE_LuaSequenceTest::GetTSETestNameToIndex(size_t nIndex)
	{
		return L"";
	}

	void CTSE_LuaSequenceTest::UnLoadTSETestPlugin()
	{
		bool bResult = true;
		if (!mcLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("CloseSequenceManager",(int)mulTokenID,bResult))
		{
			mpLogListener->OnErrorLog(mcLuaHelp.GetErrorMessage().c_str());
		}
	}
}
