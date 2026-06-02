#include "StdAfx.h"
#include "TSE_FTPUpdateFile.h"
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include "CJGW_FtpClient.h"
#include <WinBase.h>
namespace JGW
{
    CTSE_FTPUpdateFile::CTSE_FTPUpdateFile(void) : mstrFTPListName(L"JW")
    {
        TCHAR szComputerName[260] = {0};
        DWORD dwSize = 260;
        GetComputerNameW(szComputerName,&dwSize);
        mstrComputeName = szComputerName;
    }

    CTSE_FTPUpdateFile::~CTSE_FTPUpdateFile(void)
    {
    }

    const wchar_t* CTSE_FTPUpdateFile::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTPUpdateFile\",\
                \"TestName\": \"上传FTP CPK文件\",\
                \"FTPListName\": \"jgw_mes_config.data FTP List 的名称\",\
                \"FTPUploadDirEnvironment\": \"FTP上传目录环境变量\",\
                \"UploadFileEnvironment\": \"上传文件路径的环境变量\",\
                \"SNEnvironment\": \"SN环境变量名称\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\"\
                }";
    }

    bool CTSE_FTPUpdateFile::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"FTPUploadDirEnvironment",strParamName))
        {
            mstrFTPUploadDirEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"FTPListName",strParamName))
        {
            mstrFTPListName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SNEnvironment",strParamName))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"OperateID",strParamName))
        {
            mstrOperateID = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"UploadFileEnvironment",strParamName))
        {
            mstrUploadFileEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTPUpdateFile::TSE_Run()
    {
        std::wstring strUploadFilePath = GetGlobalEnvironment()->GetString(mstrUploadFileEnvironment);
        std::wstring strUploadFileName = JGW_GetFileNameToFilePath(strUploadFilePath.c_str());

        LogD_F(L"Upload FTP File Path(%s) Name(%s)",strUploadFilePath.c_str(),strUploadFileName.c_str());
        if (!JGW_FileExistsToFilePath(strUploadFilePath.c_str()))
        {
            LogE_F(L"FTP上传文件不存在(%s) fail ",strUploadFilePath.c_str());
            return false;
        }

        //! 获取连接信息
        std::map<std::wstring,S_FTP_CONNECT_INFO>::iterator it = GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mmapFTPConnectInfo.find(mstrFTPListName);
        LogD_F(L"FTP List Name : %s",mstrFTPListName.c_str());
        if (it == GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mmapFTPConnectInfo.end())
        {
            LogE_F(L"当前FTP List对应的名称(%s)，FTP连接信息不存在 fail\n",mstrFTPListName.c_str());
            return false;
        }
        LogD_F("FTP UserInfo host: %s,name: %s,pwd: %s",it->second.mstrHost.c_str(),it->second.mstrUsername.c_str(),it->second.mstrUserpwd.c_str());

        CCJGW_FtpClient ftpClient;
        if (!ftpClient.OpenFTPClient(it->second.mstrHost,it->second.mstrUsername,it->second.mstrUserpwd))
        {
            LogE(L"FTP登录连接失败 fail");
            return false;
        }

        if (mstrOperateID.empty()) mstrOperateID = JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId);
        //! 构造FTP路径
        std::wstring strFTPUploadDir = GetGlobalEnvironment()->GetString(mstrFTPUploadDirEnvironment);
        JGW_ReplaceStringW(strFTPUploadDir,L"%TASK%",JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrTaskNumber));
        JGW_ReplaceStringW(strFTPUploadDir,L"%OPERATEID%",mstrOperateID);
        JGW_ReplaceStringW(strFTPUploadDir,L"%SN%",GetGlobalEnvironment()->GetString(mstrSNEnvironment));
        JGW_ReplaceStringW(strFTPUploadDir,L"%ComputerName%",mstrComputeName);
        LogD_F(L"FTP Upload Dir : %s,File Name: %s",strFTPUploadDir.c_str(),strUploadFileName.c_str());

		std::wstring strFtpUploadPath (strFTPUploadDir);
		strFtpUploadPath += strUploadFileName;
		LogI_F(L"Update FTP File(%s -> %s)\n",strUploadFilePath.c_str(),strFtpUploadPath.c_str());
		if (!ftpClient.UpdateFTPFile(JGW_W2A(strFtpUploadPath),JGW_W2A(strUploadFilePath)))
		{
			ftpClient.CreateFTPDirectory(JGW_W2A(strFTPUploadDir));
			if (!ftpClient.UpdateFTPFile(JGW_W2A(strFtpUploadPath),JGW_W2A(strUploadFilePath)))
			{
				LogE_F(L"上传FTP路径失败(%s -> %s) fail \n",strUploadFilePath.c_str(),strFTPUploadDir.c_str());
				return false;
			}
		}
        return true;
    }
}