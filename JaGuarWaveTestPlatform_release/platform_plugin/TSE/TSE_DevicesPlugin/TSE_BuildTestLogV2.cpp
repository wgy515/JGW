#include "StdAfx.h"
#include "TSE_BuildTestLogV2.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <fstream>
namespace JGW
{
	CTSE_BuildTestLogV2::CTSE_BuildTestLogV2(void) : mstrSaveTestLogFilePathCommand(L"TestLog\\$$_TSE_TEST_PROJECT_NAME_$$\\$$_TIME_YEAR_MON_DAY_$$\\$$_TSE_TEST_RESULT_STRING_$$_$$_TSE_BARCODE_$$_$$_TSE_TIMESTAMP_$$.csv"),mnLineMaxStringLength(2048),mszLineStringBuffer(NULL)
	{

	}


	CTSE_BuildTestLogV2::~CTSE_BuildTestLogV2(void)
	{
		if (mszLineStringBuffer)
		{
			free(mszLineStringBuffer);
			mszLineStringBuffer = NULL;
		}
	}

	const wchar_t* CTSE_BuildTestLogV2::TSE_GetParamDescription()
	{
		return L"{\
				\"RealName\": \"CTSE_BuildTestLog\",\
				\"TestName\": \"构造log文件\",\
				\"LineMaxStringLength\": \"行最大字符串长度\",\
				\"TemplateTestLogFilePath\": \"模板测试日志文件路径\",\
				\"SaveTestLogFilePathCommand\": \"保存测试日志文件名变量\",\
				\"SaveTestLogFilePathEnvironmentName\": \"文件保存路径环境变量\"\
				}";
	}

	bool CTSE_BuildTestLogV2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
	{
		if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
		if (TSE_PARAM_NAME_EQUAL(L"TemplateTestLogFilePath",strParamName))
		{
			mstrTemplateTestLogFilePath = JGW_RealativePathToAbsPath(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"SaveTestLogFilePathCommand",strParamName))
		{
			mstrSaveTestLogFilePathCommand = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"SaveTestLogFilePathEnvironmentName",strParamName))
		{
			mstrSaveTestLogFilePathEnvironmentName = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"LineMaxStringLength",strParamName))
		{
			mnLineMaxStringLength = _ttoi(strParamValue);
		}	
		else
		{
			return false;
		}
		return true;
	}

	bool CTSE_BuildTestLogV2::TSE_Run()
	{
		std::wstring strSaveTestLogFilePath(mstrSaveTestLogFilePathCommand);
		//! TSE_TIMESTAMP
		GetGlobalEnvironment()->PutString(L"TSE_TEST_RESULT_STRING",1 == GetGlobalEnvironment()->GetInt(TSE_TEST_RESULT) ? L"P" : L"F");
		GetGlobalEnvironment()->PutString(TIME_YEAR_MON_DAY,JGW_GetTimeFolder());
		GetGlobalEnvironment()->PutInt64(L"TSE_TIMESTAMP",(__int64)JGW_GetTimeOfDay());
		GetEnvironmentCommandString(strSaveTestLogFilePath);
		strSaveTestLogFilePath = JGW_RealativePathToAbsPath(strSaveTestLogFilePath.c_str());
		JGW_CreateDirectory(strSaveTestLogFilePath.c_str(),false);

		if (NULL == mszLineStringBuffer)
		{
			mszLineStringBuffer = (wchar_t*)calloc(mnLineMaxStringLength,sizeof(wchar_t));
		}

		if (!JGW_FileExistsToFilePath(mstrTemplateTestLogFilePath.c_str()))
		{
			LogE_F(L"Create Save Test Log File(%s) Fail!!!",strSaveTestLogFilePath.c_str());
			return false;
		}

		//size_t uFileSize;
		//JGW_GetFileBufToPath_C(JGW_W2A(mstrTemplateTestLogFilePath).c_str(),uFileSize);
		std::wofstream fileStream;
		fileStream.imbue(std::locale(""));
		fileStream.open(strSaveTestLogFilePath.c_str(),std::ios_base::app);
		if (!fileStream.is_open())
		{
			LogE_F(L"The template file(%s) does not exist",strSaveTestLogFilePath.c_str());
			return false;
		}

		std::wifstream templateFileStream;
		templateFileStream.imbue(std::locale(""));
		templateFileStream.open(mstrTemplateTestLogFilePath.c_str());
		if (!templateFileStream.is_open())
		{
			LogE_F(L"Open template file(%s) Fail!!!",mstrTemplateTestLogFilePath.c_str());
			return false;
		}

		std::wstring strTemp;
		while (templateFileStream.getline(mszLineStringBuffer,mnLineMaxStringLength))
		{
			strTemp = mszLineStringBuffer;
			GetEnvironmentCommandString(strTemp);
			fileStream << strTemp << std::endl;
		}
		fileStream.close();

		if (!mstrSaveTestLogFilePathEnvironmentName.empty())
		{
			GetGlobalEnvironment()->PutString(mstrSaveTestLogFilePathEnvironmentName,strSaveTestLogFilePath);
			LogI_F(L"%s : %s",mstrSaveTestLogFilePathEnvironmentName.c_str(),strSaveTestLogFilePath.c_str());
		}


		return true;
	}
}
