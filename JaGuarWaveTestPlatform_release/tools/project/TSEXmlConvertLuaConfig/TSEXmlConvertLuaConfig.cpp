// TSEXmlConvertLuaConfig.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TSE_SequenceTestConfig.h"
#include <string>
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <fstream>
#include <codecvt>

using namespace JGW;
std::wstring gstrSuiteConfigPath;
std::wstring gstrLuaFilePath;
int nIndex = 0;
std::wstring gstrModeleName;

typedef struct  
{
	std::wstring strLocalClass;
	std::wstring strTestName;
}S_LOCALCLASS_TESTNAME;


bool ParseLocalClass(std::wofstream& fLuaFileStream,S_Test_SuiteProjectConfig* psTestSuiteProjectConfig)
{
	std::vector<s_suite_single_item_test_config>::iterator itItem = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"RealName");
	if (itItem != psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end())
	{
		fLuaFileStream << L"local "<<  itItem->mParamValue << L"_" << nIndex++ << std::endl;
	}
	else
	{
		return false;
	}

	for (std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>::iterator it = psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.begin();it != psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.end();++it)
	{
		if (!ParseLocalClass(fLuaFileStream,it->get())) return false;
	}
	return true;
}

bool ParseCreateTestID(std::wofstream& fLuaFileStream,S_Test_SuiteProjectConfig* psTestSuiteProjectConfig)
{
	std::vector<s_suite_single_item_test_config>::iterator itItem = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"RealName");
	std::vector<s_suite_single_item_test_config>::iterator itModule = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"ModuleName");
	std::vector<s_suite_single_item_test_config>::iterator itTest = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"TestName");

	if (itModule == psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end())
	{
	}
	else
	{
		gstrModeleName = itModule->mParamValue;
	}

	if (itItem == psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end() || itTest == psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end())
	{
		return false;
	}
	std::wstring strRealName = itItem->mParamValue;
	//std::wstring strModuleName = itItem->mParamValue;
	std::wstring strTestName = itTest->mParamValue;

	fLuaFileStream << JGW_GetFormatWString(L"	%s_%d,error_msg = seqmanager.new(SEQ_ID_ARRAY[index + 1],\"%s\",\"%s\",\"%s\")",strRealName.c_str(),nIndex++,gstrModeleName.c_str(),strRealName.c_str(),strTestName.c_str()) << std::endl;
	fLuaFileStream << JGW_GetFormatWString(L"	if (0 == %s_%d)",strRealName.c_str(),nIndex - 1) << std::endl;;
	//fLuaFileStream << L"	printBarcode,error_msg = seqmanager.new(SEQ_ID_ARRAY[index + 1],"TSE_DevicesPlugin.dll","CTSE_PrintInfo","Print Barcode")" << std::endl;
	//fLuaFileStream << L"	if (0 == printBarcode)" << std::endl;
	fLuaFileStream << L"	then" << std::endl;
	fLuaFileStream << L"		print(error_msg)" << std::endl;
	fLuaFileStream << L"		return false" << std::endl;
	fLuaFileStream << L"	end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	for (std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>::iterator it = psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.begin();it != psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.end();++it)
	{
		if (!ParseCreateTestID(fLuaFileStream,it->get())) return false;
	}
	return true;
}

bool ParseAddParam(std::wofstream& fLuaFileStream,S_Test_SuiteProjectConfig* psTestSuiteProjectConfig)
{
	std::vector<s_suite_single_item_test_config>::iterator itItem = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"RealName");
	std::wstring strLocalClassName = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);
	std::vector<s_suite_single_item_test_config>::iterator itTest = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"TestName");
	if (itTest != psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end())
	{
		fLuaFileStream << JGW_GetFormatWString(L"	-- %s",itTest->mParamValue.c_str()) << std::endl;
	}

	for (std::vector<s_suite_single_item_test_config>::iterator itSingleItem = psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.begin();itSingleItem != psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end();++itSingleItem)
	{
		if (0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"RealName") 
			&& 0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"ModuleName") 
			&& 0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"TestName"))
		{
			std::wstring strValue = itSingleItem->mParamValue;
			JGW_ReplaceStringW(strValue,L"\\",L"\\\\");
			JGW_ReplaceStringW(strValue,L"\"",L"\\\"");
			fLuaFileStream << JGW_GetFormatWString(L"	result = seqmanager.add_param(%s,\"%s\",\"%s\") and result",strLocalClassName.c_str(),itSingleItem->mParamName.c_str(),strValue.c_str()) << std::endl;
		}	
		//fLuaFileStream << std::endl;
	}
	

	for (std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>::iterator it = psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.begin();it != psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.end();++it)
	{
		if (!ParseAddParam(fLuaFileStream,it->get())) return false;
	}
	return true;
}

bool ParseInitTSETest(std::wofstream& fLuaFileStream,S_Test_SuiteProjectConfig* psTestSuiteProjectConfig)
{
	std::vector<s_suite_single_item_test_config>::iterator itItem = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"RealName");
	std::wstring strLocalClassName = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);

	std::vector<s_suite_single_item_test_config>::iterator itTest = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"TestName");
	if (itTest != psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end())
	{
		fLuaFileStream << JGW_GetFormatWString(L"	-- %s",itTest->mParamValue.c_str()) << std::endl;
	}
	fLuaFileStream << JGW_GetFormatWString(L"	seqmanager.change_test_status(SEQ_ID_ARRAY[index + 1],%s,E_INIT_TEST_STATUS)",strLocalClassName.c_str()) << std::endl;
	fLuaFileStream << JGW_GetFormatWString(L"	result = seqmanager.init(%s) and result",strLocalClassName.c_str()) << std::endl;
	fLuaFileStream << std::endl;

	for (std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>::iterator it = psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.begin();it != psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.end();++it)
	{
		if (!ParseInitTSETest(fLuaFileStream,it->get())) return false;
	}
	return true;
}

bool ParseExecuteTSETest(std::wofstream& fLuaFileStream,S_Test_SuiteProjectConfig* psTestSuiteProjectConfig,std::map<int,S_LOCALCLASS_TESTNAME>& mapIndexLocalClassName)
{
	/*
	result = ExecuteSingleTSETest(index,inputBarcode,false)
	if (true ~= result)
	then
	return result
	end
	*/

	std::vector<s_suite_single_item_test_config>::iterator itItem = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"RealName");
	std::wstring strLocalClassName = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);
	std::vector<s_suite_single_item_test_config>::iterator itTest = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"TestName");
	if (itTest != psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end())
	{
		fLuaFileStream << JGW_GetFormatWString(L"	-- %s",itTest->mParamValue.c_str()) << std::endl;
	}

	fLuaFileStream << JGW_GetFormatWString(L"	result = ExecuteSingleTSETest(index,%s,%s)",strLocalClassName.c_str(),psTestSuiteProjectConfig->msSuiteItemTestConfig.mbReversedResult ? L"true" : L"false") << std::endl;
	fLuaFileStream << L"	if (true ~= result)" << std::endl;
	fLuaFileStream << L"	then" << std::endl;
	for (std::map<int,S_LOCALCLASS_TESTNAME>::iterator it = mapIndexLocalClassName.begin();it != mapIndexLocalClassName.end();++it)
	{
		if (nIndex <= it->first)
		{
			fLuaFileStream << JGW_GetFormatWString(L"		-- %s",it->second.strTestName.c_str()) << std::endl;
			fLuaFileStream << JGW_GetFormatWString(L"		ExecuteSingleTSEConfigAutoTest(index,%s)",it->second.strLocalClass.c_str()) << std::endl;
		}
	}
	fLuaFileStream << L"		return result" << std::endl;
	fLuaFileStream << L"	end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	for (std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>::iterator it = psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.begin();it != psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.end();++it)
	{
		if (!ParseExecuteTSETest(fLuaFileStream,it->get(),mapIndexLocalClassName)) return false;
	}
	return true;
}

bool ParseFinalizeTest(std::wofstream& fLuaFileStream,S_Test_SuiteProjectConfig* psTestSuiteProjectConfig,std::map<int,S_LOCALCLASS_TESTNAME>& mapIndexLocalClassName)
{
	std::vector<s_suite_single_item_test_config>::iterator itItem = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"RealName");
	std::wstring strLocalClassName = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);

	if (psTestSuiteProjectConfig->msSuiteItemTestConfig.m_bFinalizeTest)
	{
		S_LOCALCLASS_TESTNAME sLocalClassTestName;
		sLocalClassTestName.strLocalClass = strLocalClassName;
		std::vector<s_suite_single_item_test_config>::iterator itTest = psTestSuiteProjectConfig->msSuiteItemTestConfig.find(L"TestName");
		if (itTest != psTestSuiteProjectConfig->msSuiteItemTestConfig.m_mTestParam.end())
		{
			sLocalClassTestName.strTestName = itTest->mParamValue;
		}
		mapIndexLocalClassName.insert(std::make_pair<int,S_LOCALCLASS_TESTNAME>(nIndex - 1,sLocalClassTestName));
	}
	
	for (std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>::iterator it = psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.begin();it != psTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig.end();++it)
	{
		if (!ParseFinalizeTest(fLuaFileStream,it->get(),mapIndexLocalClassName)) return false;
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcout.imbue(std::locale(""));
	if (argc > 1)
	{
		gstrSuiteConfigPath = JGW_RealativePathToAbsPath(argv[1]);
	}
	if (argc > 2)
	{
		gstrLuaFilePath = JGW_RealativePathToAbsPath(argv[2]);
	}

	if (gstrLuaFilePath.empty())
	{
		gstrLuaFilePath = gstrSuiteConfigPath + L".lua";
	}


	S_TSE_SequenceConfig sTseSequenceConfig;
	CTSE_SequenceTestConfig cSequenceTestConfig(sTseSequenceConfig);

	if (!cSequenceTestConfig.PareseAssemblyConfig(JGW_W2A(gstrSuiteConfigPath)))
	{
		std::wcout << L"PareseAssemblyConfig Fail" << std::endl;
		return -1;
	}

	std::wofstream fLuaFileStream;
	//fLuaFileStream.imbue(std::locale(""));
	fLuaFileStream.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));
	fLuaFileStream.open(gstrLuaFilePath.c_str());
	if (!fLuaFileStream.is_open())
	{
		std::wcout << JGW_GetFormatWString(L"Warning:Open DMG CSV(%s - %s) Log File Faill",gstrLuaFilePath.c_str(),JGW_GetSystemErrorString(GetLastError()).c_str()) << std::endl;
		return -1;
	}
	fLuaFileStream << L"local seqmanager = require \"TSE_SequenceLuaHelpPlugin\"" << std::endl;
	fLuaFileStream << L"-- return myarray=fnluaopen_myLualib() -- 执行luaopen_myLualib函数" << std::endl;
	//fLuaFileStream << JGW_W2A(L"local seqmanager = require \"TSE_SequenceLuaHelpPlugin\"",CP_UTF8) << std::endl;
	//fLuaFileStream << JGW_W2A(L"-- return myarray=fnluaopen_myLualib() -- 执行luaopen_myLualib函数",CP_UTF8).c_str() << std::endl;
	//fLuaFileStream << L"print(\"执行luaopen_myLualib函数\")" << std::endl;
	fLuaFileStream << L"local SEQ_ID_ARRAY = {}" << std::endl;
	fLuaFileStream << L"local E_INIT_TEST_STATUS = 0" << std::endl;
	fLuaFileStream << L"local E_RUN_TEST_STATUS = 1" << std::endl;
	fLuaFileStream << L"local E_RUNING_TEST_STATUS = 2" << std::endl;
	fLuaFileStream << L"local E_FAIL_TEST_STATUS = 3" << std::endl;
	fLuaFileStream << L"local E_PASS_TEST_STATUS = 4" << std::endl;

	fLuaFileStream << std::endl;
	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vConfigViewTestConfig.begin();it != sTseSequenceConfig.m_vConfigViewTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		if (itItem != it[0]->m_mTestParam.end())
		{
			fLuaFileStream << L"local "<<  itItem->mParamValue << L"_" << nIndex++ << std::endl;
		}
		else
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
	}
	
	fLuaFileStream << std::endl;
	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vAutoTestConfig.begin();it != sTseSequenceConfig.m_vAutoTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		if (itItem != it[0]->m_mTestParam.end())
		{
			fLuaFileStream << L"local "<<  itItem->mParamValue << L"_" << nIndex++ << std::endl;
		}
		else
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
	}


	fLuaFileStream << std::endl;
	for (_vs_suite_projectconfig::iterator it = sTseSequenceConfig.mvsSuiteProjectConfig.begin();it != sTseSequenceConfig.mvsSuiteProjectConfig.end();++it)
	{
#if 0
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->msSuiteItemTestConfig.find(L"RealName");
		if (itItem != it[0]->msSuiteItemTestConfig.m_mTestParam.end())
		{
			fLuaFileStream << L"local "<<  itItem->mParamValue << L"_" << nIndex++ << std::endl;
		}
#else
		if (!ParseLocalClass(fLuaFileStream,it->get()))
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
#endif
	}
	fLuaFileStream << L"" << std::endl;

	//! -- 先进行预初始化获取
	fLuaFileStream << L"function PreInitializationSequenceManager(index)" << std::endl;
	fLuaFileStream << L"	local sequenceManager,error_msg = seqmanager.pre_initialization(index)" << std::endl;
	fLuaFileStream << L"	SEQ_ID_ARRAY[#SEQ_ID_ARRAY + index + 1] = sequenceManager" << std::endl;
	fLuaFileStream << L"	if (0 == SEQ_ID_ARRAY[index + 1])" << std::endl;
	fLuaFileStream << L"	then" << std::endl;
	fLuaFileStream << L"		return false" << std::endl;
	fLuaFileStream << L"	end" << std::endl;
	fLuaFileStream << L"	return true" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	//! function GetEnv(index,envName)
	fLuaFileStream << L"function GetEnv(index,envName)" << std::endl;
	fLuaFileStream << L"	return seqmanager.get_env(SEQ_ID_ARRAY[index + 1],envName)" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	//! function SetEnv(index,envName,envValue)
	fLuaFileStream << L"function SetEnv(index,envName,envValue)" << std::endl;
	fLuaFileStream << L"	seqmanager.set_env(SEQ_ID_ARRAY[index + 1],envName,envValue)" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	//! function AddGlobalEnvironment(index)
	fLuaFileStream << L"function AddGlobalEnvironment(index)" << std::endl;
	for (_mcommontestconfig::iterator it = sTseSequenceConfig.m_mCommonTestParam.begin();it != sTseSequenceConfig.m_mCommonTestParam.end();++it)
	{
		std::wstring strValue = it->second;
		JGW_ReplaceStringW(strValue,L"\\",L"\\\\");
		JGW_ReplaceStringW(strValue,L"\"",L"\\\"");
		fLuaFileStream << L"	SetEnv(index,\"" << it->first << L"\",\"" << strValue << L"\")" << std::endl;
	}
	//! fLuaFileStream << L"	SetEnv(index,"ApplicationCMD","cmd.exe")" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	//! function CreateTestID(index)
	nIndex = 0;
	fLuaFileStream << L"function CreateTestID(index)" << std::endl;
	

	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vConfigViewTestConfig.begin();it != sTseSequenceConfig.m_vConfigViewTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::vector<s_suite_single_item_test_config>::iterator itModule = it[0]->find(L"ModuleName");
		std::vector<s_suite_single_item_test_config>::iterator itTest = it[0]->find(L"TestName");

		if (itModule != it[0]->m_mTestParam.end())
		{
			gstrModeleName = itModule->mParamValue;
		}

		if (itItem == it[0]->m_mTestParam.end() || gstrModeleName.empty() || itTest == it[0]->m_mTestParam.end())
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}

		std::wstring strRealName = itItem->mParamValue;
		std::wstring strTestName = itTest->mParamValue;

		fLuaFileStream << JGW_GetFormatWString(L"	%s_%d,error_msg = seqmanager.new(SEQ_ID_ARRAY[index + 1],\"%s\",\"%s\",\"%s\")",strRealName.c_str(),nIndex++,gstrModeleName.c_str(),strRealName.c_str(),strTestName.c_str()) << std::endl;
		fLuaFileStream << JGW_GetFormatWString(L"	if (0 == %s_%d)",strRealName.c_str(),nIndex - 1) << std::endl;;
		//fLuaFileStream << L"	printBarcode,error_msg = seqmanager.new(SEQ_ID_ARRAY[index + 1],"TSE_DevicesPlugin.dll","CTSE_PrintInfo","Print Barcode")" << std::endl;
		//fLuaFileStream << L"	if (0 == printBarcode)" << std::endl;
		fLuaFileStream << L"	then" << std::endl;
		fLuaFileStream << L"		print(error_msg)" << std::endl;
		fLuaFileStream << L"		return false" << std::endl;
		fLuaFileStream << L"	end" << std::endl;
		fLuaFileStream << L"" << std::endl;
	}

	fLuaFileStream << std::endl;
	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vAutoTestConfig.begin();it != sTseSequenceConfig.m_vAutoTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::vector<s_suite_single_item_test_config>::iterator itModule = it[0]->find(L"ModuleName");
		std::vector<s_suite_single_item_test_config>::iterator itTest = it[0]->find(L"TestName");

		if (itModule != it[0]->m_mTestParam.end())
		{
			gstrModeleName = itModule->mParamValue;
		}

		if (itItem == it[0]->m_mTestParam.end() || gstrModeleName.empty() || itTest == it[0]->m_mTestParam.end())
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}

		std::wstring strRealName = itItem->mParamValue;
		std::wstring strTestName = itTest->mParamValue;
		

		fLuaFileStream << JGW_GetFormatWString(L"	%s_%d,error_msg = seqmanager.new(SEQ_ID_ARRAY[index + 1],\"%s\",\"%s\",\"%s\")",strRealName.c_str(),nIndex++,gstrModeleName.c_str(),strRealName.c_str(),strTestName.c_str()) << std::endl;
		fLuaFileStream << JGW_GetFormatWString(L"	if (0 == %s_%d)",strRealName.c_str(),nIndex - 1) << std::endl;;
		//fLuaFileStream << L"	printBarcode,error_msg = seqmanager.new(SEQ_ID_ARRAY[index + 1],"TSE_DevicesPlugin.dll","CTSE_PrintInfo","Print Barcode")" << std::endl;
		//fLuaFileStream << L"	if (0 == printBarcode)" << std::endl;
		fLuaFileStream << L"	then" << std::endl;
		fLuaFileStream << L"		print(error_msg)" << std::endl;
		fLuaFileStream << L"		return false" << std::endl;
		fLuaFileStream << L"	end" << std::endl;
		fLuaFileStream << L"" << std::endl;
	}

	

	for (_vs_suite_projectconfig::iterator it = sTseSequenceConfig.mvsSuiteProjectConfig.begin();it != sTseSequenceConfig.mvsSuiteProjectConfig.end();++it)
	{
		if (!ParseCreateTestID(fLuaFileStream,it->get()))
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
		fLuaFileStream << L"" << std::endl;
	}
	fLuaFileStream << L"	return true" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;


	//! function AddParam(index)
	nIndex = 0;
	fLuaFileStream << L"function AddParam(index)" << std::endl;
	fLuaFileStream << L"	local result = true" << std::endl;
	fLuaFileStream << L"" << std::endl;

	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vConfigViewTestConfig.begin();it != sTseSequenceConfig.m_vConfigViewTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::wstring strLocalClassName = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);
		std::vector<s_suite_single_item_test_config>::iterator itTest = it[0]->find(L"TestName");
		if (itTest != it[0]->m_mTestParam.end())
		{
			fLuaFileStream << JGW_GetFormatWString(L"	-- %s",itTest->mParamValue.c_str()) << std::endl;
		}

		for (std::vector<s_suite_single_item_test_config>::iterator itSingleItem = it[0]->m_mTestParam.begin();itSingleItem != it[0]->m_mTestParam.end();++itSingleItem)
		{
			if (0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"RealName") 
				&& 0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"ModuleName") 
				&& 0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"TestName"))
			{
				std::wstring strValue = itSingleItem->mParamValue;
				JGW_ReplaceStringW(strValue,L"\\",L"\\\\");
				JGW_ReplaceStringW(strValue,L"\"",L"\\\"");
				fLuaFileStream << JGW_GetFormatWString(L"	result = seqmanager.add_param(%s,\"%s\",\"%s\") and result",strLocalClassName.c_str(),itSingleItem->mParamName.c_str(),strValue.c_str()) << std::endl;
			}	
		}
		fLuaFileStream << std::endl;
	}

	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vAutoTestConfig.begin();it != sTseSequenceConfig.m_vAutoTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::wstring strLocalClassName = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);
		std::vector<s_suite_single_item_test_config>::iterator itTest = it[0]->find(L"TestName");
		if (itTest != it[0]->m_mTestParam.end())
		{
			fLuaFileStream << JGW_GetFormatWString(L"	-- %s",itTest->mParamValue.c_str()) << std::endl;
		}

		for (std::vector<s_suite_single_item_test_config>::iterator itSingleItem = it[0]->m_mTestParam.begin();itSingleItem != it[0]->m_mTestParam.end();++itSingleItem)
		{
			if (0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"RealName") 
				&& 0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"ModuleName") 
				&& 0 != JGW_WStrComparenoCaseWStr(itSingleItem->mParamName.c_str(),L"TestName"))
			{
				std::wstring strValue = itSingleItem->mParamValue;
				JGW_ReplaceStringW(strValue,L"\\",L"\\\\");
				JGW_ReplaceStringW(strValue,L"\"",L"\\\"");
				fLuaFileStream << JGW_GetFormatWString(L"	result = seqmanager.add_param(%s,\"%s\",\"%s\") and result",strLocalClassName.c_str(),itSingleItem->mParamName.c_str(),strValue.c_str()) << std::endl;
			}	
		}
		fLuaFileStream << std::endl;
	}


	for (_vs_suite_projectconfig::iterator it = sTseSequenceConfig.mvsSuiteProjectConfig.begin();it != sTseSequenceConfig.mvsSuiteProjectConfig.end();++it)
	{
		if (!ParseAddParam(fLuaFileStream,it->get()))
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
		fLuaFileStream << L"" << std::endl;
	}
	fLuaFileStream << L"	return result" << std::endl;
	fLuaFileStream << L"end" << std::endl;

	//! function AutoTestNotEmpty(index)
	fLuaFileStream << L"function AutoTestNotEmpty(index)" << std::endl;
	if (sTseSequenceConfig.m_vAutoTestConfig.empty())
	{
		fLuaFileStream << L"	return false" << std::endl;
	}
	else
	{
		fLuaFileStream << L"	return true" << std::endl;
	}
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	//! function ExecuteSingleTSEConfigAutoTest(index,tseInstance)
	fLuaFileStream << L"function ExecuteSingleTSEConfigAutoTest(index,tseInstance)" << std::endl;
	fLuaFileStream << L"	local retest_count = seqmanager.retest_count(tseInstance)" << std::endl;
	fLuaFileStream << L"	for i=1,retest_count do" << std::endl;
	fLuaFileStream << L"		local result = seqmanager.run(tseInstance)" << std::endl;
	fLuaFileStream << L"		result = seqmanager.exit(tseInstance) and result" << std::endl;
	fLuaFileStream << L"		if (true == result)" << std::endl;
	fLuaFileStream << L"		then" << std::endl;
	fLuaFileStream << L"			return true" << std::endl;
	fLuaFileStream << L"		end" << std::endl;
	fLuaFileStream << L"	end" << std::endl;
	fLuaFileStream << L"	return false" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;



	//! function ExecuteTSEConfigTest(index)
	nIndex = 0;
	fLuaFileStream << L"function ExecuteTSEConfigTest(index)" << std::endl;
	fLuaFileStream << L"	local result = true" << std::endl;
	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vConfigViewTestConfig.begin();it != sTseSequenceConfig.m_vConfigViewTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::wstring strLocalClass = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);

		fLuaFileStream << JGW_GetFormatWString(L"	result = ExecuteSingleTSEConfigAutoTest(index,%s)",strLocalClass.c_str()) << std::endl;
		fLuaFileStream << L"	if (true ~= result)" << std::endl;
		fLuaFileStream << L"	then" << std::endl;
		fLuaFileStream << L"		return result" << std::endl;
		fLuaFileStream << L"	end" << std::endl;
		fLuaFileStream << L"" << std::endl;
	}
	fLuaFileStream << L"	return true" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	//! function AutoTSETest(index)
	fLuaFileStream << L"function AutoTSETest(index)" << std::endl;
	fLuaFileStream << L"	local result = true" << std::endl;
	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vAutoTestConfig.begin();it != sTseSequenceConfig.m_vAutoTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::wstring strLocalClass = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);

		fLuaFileStream << JGW_GetFormatWString(L"	result = ExecuteSingleTSEConfigAutoTest(index,%s)",strLocalClass.c_str()) << std::endl;
		fLuaFileStream << L"	if (true ~= result)" << std::endl;
		fLuaFileStream << L"	then" << std::endl;
		fLuaFileStream << L"		return result" << std::endl;
		fLuaFileStream << L"	end" << std::endl;
		fLuaFileStream << L"" << std::endl;
	}
	fLuaFileStream << L"	return true" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	
	//! function InitTSETest(index)
	nIndex = 0;
	fLuaFileStream << L"function InitTSETest(index)" << std::endl;
	fLuaFileStream << L"	local result = true" << std::endl;
	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vConfigViewTestConfig.begin();it != sTseSequenceConfig.m_vConfigViewTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::wstring strLocalClass = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);
		std::vector<s_suite_single_item_test_config>::iterator itTest = it[0]->find(L"TestName");
		if (itTest != it[0]->m_mTestParam.end())
		{
			fLuaFileStream << JGW_GetFormatWString(L"	-- %s",itTest->mParamValue.c_str()) << std::endl;
		}
		fLuaFileStream << JGW_GetFormatWString(L"	seqmanager.change_test_status(SEQ_ID_ARRAY[index + 1],%s,E_INIT_TEST_STATUS)",strLocalClass.c_str()) << std::endl;
		fLuaFileStream << JGW_GetFormatWString(L"	result = seqmanager.init(%s) and result",strLocalClass.c_str()) << std::endl;
		fLuaFileStream << L"" << std::endl;
	}
	for (_vsuitetestconfig::iterator it = sTseSequenceConfig.m_vAutoTestConfig.begin();it != sTseSequenceConfig.m_vAutoTestConfig.end();++it)
	{
		std::vector<s_suite_single_item_test_config>::iterator itItem = it[0]->find(L"RealName");
		std::wstring strLocalClass = JGW_GetFormatWString(L"%s_%d",itItem->mParamValue.c_str(),nIndex++);
		std::vector<s_suite_single_item_test_config>::iterator itTest = it[0]->find(L"TestName");
		if (itTest != it[0]->m_mTestParam.end())
		{
			fLuaFileStream << JGW_GetFormatWString(L"	-- %s",itTest->mParamValue.c_str()) << std::endl;
		}
		fLuaFileStream << JGW_GetFormatWString(L"	seqmanager.change_test_status(SEQ_ID_ARRAY[index + 1],%s,E_INIT_TEST_STATUS)",strLocalClass.c_str()) << std::endl;
		fLuaFileStream << JGW_GetFormatWString(L"	result = seqmanager.init(%s) and result",strLocalClass.c_str()) << std::endl;
		fLuaFileStream << L"" << std::endl;
	}
	for (_vs_suite_projectconfig::iterator it = sTseSequenceConfig.mvsSuiteProjectConfig.begin();it != sTseSequenceConfig.mvsSuiteProjectConfig.end();++it)
	{
		if (!ParseInitTSETest(fLuaFileStream,it->get()))
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
		fLuaFileStream << L"" << std::endl;
	}
	


	fLuaFileStream << L"" << std::endl;
	fLuaFileStream << L"	return result" << std::endl;
	fLuaFileStream << L"end" << std::endl;

	//! function ExecuteSingleTSETest(index,tseInstance)
	fLuaFileStream << L"function ExecuteSingleTSETest(index,tseInstance,reversedResult)" << std::endl;
	fLuaFileStream << L"	seqmanager.change_test_status(SEQ_ID_ARRAY[index + 1],tseInstance,E_RUN_TEST_STATUS)" << std::endl;
	fLuaFileStream << L"	local retest_count = seqmanager.retest_count(tseInstance)" << std::endl;
	fLuaFileStream << L"	seqmanager.change_test_status(SEQ_ID_ARRAY[index + 1],tseInstance,E_RUNING_TEST_STATUS)" << std::endl;
	fLuaFileStream << L"" << std::endl;
	fLuaFileStream << L"	for i=1,retest_count do" << std::endl;
	fLuaFileStream << L"		local result = seqmanager.run(tseInstance)" << std::endl;
	fLuaFileStream << L"		result = seqmanager.exit(tseInstance) and result" << std::endl;
	fLuaFileStream << L"		if (true == reversedResult)" << std::endl;
	fLuaFileStream << L"		then" << std::endl;
	fLuaFileStream << L"			result = not result" << std::endl;
	fLuaFileStream << L"		end" << std::endl;
	fLuaFileStream << L"		if (true == result)" << std::endl;
	fLuaFileStream << L"		then" << std::endl;
	fLuaFileStream << L"			seqmanager.change_test_status(SEQ_ID_ARRAY[index + 1],tseInstance,E_PASS_TEST_STATUS)" << std::endl;
	fLuaFileStream << L"			return true" << std::endl;
	fLuaFileStream << L"		end" << std::endl;
	fLuaFileStream << L"	end" << std::endl;
	fLuaFileStream << L"	seqmanager.change_test_status(SEQ_ID_ARRAY[index + 1],tseInstance,E_FAIL_TEST_STATUS)" << std::endl;
	fLuaFileStream << L"	seqmanager.set_env(SEQ_ID_ARRAY[index + 1],\"TSE_TEST_RESULT\",\"0\")" << std::endl;
	fLuaFileStream << L"	return false" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;

	//! 解析测试完成后销毁测试动作
	nIndex = sTseSequenceConfig.m_vAutoTestConfig.size() + sTseSequenceConfig.m_vConfigViewTestConfig.size();
	std::map<int,S_LOCALCLASS_TESTNAME> mapIndexLocalClassName;
	for (_vs_suite_projectconfig::iterator it = sTseSequenceConfig.mvsSuiteProjectConfig.begin();it != sTseSequenceConfig.mvsSuiteProjectConfig.end();++it)
	{
		if (!ParseFinalizeTest(fLuaFileStream,it->get(),mapIndexLocalClassName))
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
	}

	//! function ExecuteTSETest(index)
	nIndex = sTseSequenceConfig.m_vAutoTestConfig.size() + sTseSequenceConfig.m_vConfigViewTestConfig.size();
	fLuaFileStream << L"function ExecuteTSETest(index)" << std::endl;
	fLuaFileStream << L"	local result = true" << std::endl;
	fLuaFileStream << L"	seqmanager.set_env(SEQ_ID_ARRAY[index + 1],\"TSE_TEST_RESULT\",\"1\")" << std::endl;

	for (_vs_suite_projectconfig::iterator it = sTseSequenceConfig.mvsSuiteProjectConfig.begin();it != sTseSequenceConfig.mvsSuiteProjectConfig.end();++it)
	{
		if (!ParseExecuteTSETest(fLuaFileStream,it->get(),mapIndexLocalClassName))
		{
			std::wcout << L"Invalid test configuration" << std::endl;
			return -1;
		}
		fLuaFileStream << L"" << std::endl;
	}

	fLuaFileStream << L"	return true" << std::endl;
	fLuaFileStream << L"end" << std::endl;
	fLuaFileStream << L"" << std::endl;


	//! function CloseSequenceManager(index)
	fLuaFileStream << L"function CloseSequenceManager(index)" << std::endl;
	fLuaFileStream << L"	return true" << std::endl;
	fLuaFileStream << L"end" << std::endl;

	fLuaFileStream << L"" << std::endl;
	fLuaFileStream.flush();
	fLuaFileStream.close();

	return 0;
}

