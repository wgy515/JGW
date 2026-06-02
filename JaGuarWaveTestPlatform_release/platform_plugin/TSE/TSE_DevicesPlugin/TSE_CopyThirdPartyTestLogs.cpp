#include "StdAfx.h"
#include "TSE_CopyThirdPartyTestLogs.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <regex>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
namespace JGW
{
	bool CTSE_CopyThirdPartyTestLogs::mbStartRecording = false;
	//bool CTSE_CopyThirdPartyTestLogs::mbEndRecording = false;
	//std::vector<std::wstring> gvStartRecordingFilePath;
	std::vector<std::wstring> gvEndRecordingFilePath;
	std::vector<std::wstring>* gpRecordingFilePath;

	CTSE_CopyThirdPartyTestLogs::CTSE_CopyThirdPartyTestLogs(void) : mnRecordingIndex(0),mstrMatchLogNameRegex(L"result*.csv")
	{
		//! TSE_TEST_PROJECT_NAME
		//! TSE_TEST_RESULT
		gpRecordingFilePath = NULL;
	}


	CTSE_CopyThirdPartyTestLogs::~CTSE_CopyThirdPartyTestLogs(void)
	{
	}

	const wchar_t* CTSE_CopyThirdPartyTestLogs::TSE_GetParamDescription()
	{
		return L"{\
				\"RealName\": \"CTSE_CopyThirdPartyTestLogs\",\
				\"TestName\": \"copy third party test logs\",\
				\"RecordingIndex\": \"0 : start recording 1 : stop recording\",\
				\"SrcThirdPartyTestLogsFolderEnvironmentName\": \"src folder Environment\",\
				\"MatchLogNameRegex\": \"match log name def:result*.csv\",\
				\"LogNameReplaceConfigCommand\": \"log name replace config def : DUT1,TSE_BRACODE;result,$$_TSE_TEST_RESULT_STRING_$$\"\
				}";
	}

	bool CTSE_CopyThirdPartyTestLogs::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
	{
		if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
		if (TSE_PARAM_NAME_EQUAL(L"RecordingIndex",strParamName))
		{
			mnRecordingIndex = _ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"SrcThirdPartyTestLogsFolderEnvironmentName",strParamName))
		{
			mstrSrcThirdPartyTestLogsFolderEnvironmentName = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"MatchLogNameRegex",strParamName))
		{
			mstrMatchLogNameRegex = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"LogNameReplaceConfigCommand",strParamName))
		{
			std::vector<std::wstring> vstrGroupTemp;
			std::vector<std::wstring> vstrReplaceConfig;
			JGW_ParserStrW(strParamValue,L";",vstrGroupTemp);
			for (std::vector<std::wstring>::iterator itGroup = vstrGroupTemp.begin();itGroup != vstrGroupTemp.end();++itGroup)
			{
				vstrReplaceConfig.clear();
				JGW_ParserStrW(itGroup->c_str(),L",",vstrReplaceConfig);
				if (2 == vstrReplaceConfig.size())
				{
					mmapLogNameReplaceConfigCommand[vstrReplaceConfig[0]] = vstrReplaceConfig[1];
				}
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	void ScanThirdPartyTestLogs(const wchar_t* filepath,const wchar_t* filename,void* lpData)
	{
#if 1
		const wchar_t* strMatchLogNameRegex = (const wchar_t*)lpData;
		std::wregex pattern(strMatchLogNameRegex);
		std::wstring strFilePathTemp (filename);
		//! 对当前文件名称进行判断
		if (regex_match(strFilePathTemp,pattern))
		{
			strFilePathTemp = filepath;
			strFilePathTemp += L"\\";
			strFilePathTemp += filename;
			//! 删除无效的测试结果文件
			if (NULL == gpRecordingFilePath)
			{
				JGW_DeleteFile(strFilePathTemp);
			}
			else
			{
				gpRecordingFilePath->push_back(strFilePathTemp);
				LogI_F(L"Scan Third Party Test Logs : %s",strFilePathTemp.c_str());
			}			
		}
#else
		std::wstring strFilePathTemp (filename);
		strFilePathTemp = filepath;
		strFilePathTemp += L"\\";
		strFilePathTemp += filename;
		if (NULL == gpRecordingFilePath)
		{
			JGW_DeleteFile(strFilePathTemp);
		}
		else
		{
			gpRecordingFilePath->push_back(strFilePathTemp);
		}	
#endif
	}

	bool CTSE_CopyThirdPartyTestLogs::TSE_Run()
	{
		//! 开始收集当前LOG信息
		if (0 == mnRecordingIndex)
		{
			int numFiles = 0;
			mbStartRecording = true;
			std::wstring strPath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrSrcThirdPartyTestLogsFolderEnvironmentName).c_str());
			gpRecordingFilePath = NULL;
			JGW_TraverseFolder_VC(strPath,numFiles,L"\\*.*"/*"/ *QSEQ*.html"*/,ScanThirdPartyTestLogs,(void*)mstrMatchLogNameRegex.c_str());
			return true;
		}
		//! 如果没有当前收集则直接返回
		if (!mbStartRecording) return true;
		//! 遍历当前第三方测试目录
		int numFiles = 0;
		std::wstring strPath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrSrcThirdPartyTestLogsFolderEnvironmentName).c_str());
		gpRecordingFilePath = &gvEndRecordingFilePath;
		gvEndRecordingFilePath.clear();
		JGW_TraverseFolder_VC(strPath,numFiles,L"\\*.*",ScanThirdPartyTestLogs,(void*)mstrMatchLogNameRegex.c_str());
		//! PASS或者FAIL的字符串标识
		GetGlobalEnvironment()->PutString(L"TSE_TEST_RESULT_STRING",1 == GetGlobalEnvironment()->GetInt(TSE_TEST_RESULT) ? L"P" : L"F");
		//! 构造保存目录 
		std::wstring strMESLogFilePath;
		std::wstring strOperateID = JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId);
		JGW_FormatWString(strMESLogFilePath,L"%sTestLog\\%s\\%s\\%s\\",JGW_GetApplicationFolder(),GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),JGW_GetTimeFolder().c_str(),strOperateID.c_str());
		JGW_CreateDirectory(strMESLogFilePath.c_str(),true);

		bool bResult = false;
		//! 开始处理测试日志文件
		for (std::vector<std::wstring>::iterator it = gvEndRecordingFilePath.begin();it != gvEndRecordingFilePath.end();++it)
		{
			//! 对测试日志文件名称进行处理，已符合自定义的日志文件名称
			std::wstring strLogName = JGW_GetFileNameToFilePath(it[0].c_str());
			for (std::map<std::wstring,std::wstring>::iterator itMap = mmapLogNameReplaceConfigCommand.begin();itMap != mmapLogNameReplaceConfigCommand.end();++itMap)
			{
				std::wstring strValue = itMap->second;
				GetEnvironmentCommandString(strValue);
				JGW_ReplaceStringW(strLogName,itMap->first,strValue);
			}
			//! 检查当前文件是否已经存在
			std::wstring strNewFilePath = strMESLogFilePath + strLogName;
			{
				int index = 1;
				std::wstring strNewFilePath1 = strNewFilePath;
				size_t pos = strNewFilePath.find_last_of(L'.');
				while (JGW::JGW_FileExistsToFilePath(strNewFilePath.c_str()))
				{
					strNewFilePath = strNewFilePath1;
					if (std::wstring::npos == pos)
					{
						strNewFilePath += JGW::JGW_GetFormatWString(L"_%d",index++);
					}
					else
					{
						strNewFilePath.insert(pos,JGW::JGW_GetFormatWString(L"_%d",index++));
					}
				}
			}
			
			//! move file 
			LogI_F(L"Copy src File : %s",it->c_str());
			LogI_F(L"Copy dest File : %s",strNewFilePath.c_str());
			if (!MoveFileEx(it->c_str(),strNewFilePath.c_str(),MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
			{
				LogE(L"Move File Fail\n");
				return false;
			}
			else
			{
				bResult = true;
				LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strNewFilePath.c_str());
			}
		}
		LogE(L"Copy Third Party Test logs Fail");
		return bResult;
#if 0
		result
		匹配测试日志文件
		^result_.*?.csv$
		测试文件名替换
		DUT1 - TSE_BRACODE
		测试文件复制到testlog目录

		GetGlobalEnvironment()->PutInt(mstrRecordingTimerEnvionment,(int)&mTimer);
		JGW_FormatWString(strMESLogFilePath,L"%sTestLog\\CPK\\%s\\%s\\%s_%%s_%s",JGW_GetApplicationFolder(),strTaskName.c_str(),JGW_GetTimeFolder().c_str(),strOperateID.c_str(),JGW_GetCurrentTimeStampYMDHMS().c_str());
		JGW_CreateDirectory(strMESLogFilePath.c_str(),false);
		CopyFile(mstrSrcTempleteFilePath.c_str(),strMESLogFilePath.c_str(),FALSE);
#endif

		//mbStartRecording = false;
	}
}
