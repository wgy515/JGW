#include "StdAfx.h"
#include "TSE_StreamingDownloadUserPartitions.h"
#include <JGW_QMSLFuncPlugin/jgw_device_api.h>
namespace JGW
{
    CTSE_StreamingDownloadUserPartitions::CTSE_StreamingDownloadUserPartitions(void) : mstrResourceContextEnv(L"TSE_RESOURCE_CONTEXT"),mnNumberOfPartitionFiles(0),mstrDownloadFolderPathEnv(L"TSE_DOWNLOAD_FOLDER_PATH"),mbOverridePrtnTable(true),mbSkipGoToDownload(true)
    {
    }


    CTSE_StreamingDownloadUserPartitions::~CTSE_StreamingDownloadUserPartitions(void)
    {
    }

    const wchar_t* CTSE_StreamingDownloadUserPartitions::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_StreamingDownloadUserPartitions::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResourceContextEnv"))
        {
            mstrResourceContextEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DownloadFolderPathEnv"))
        {
            mstrDownloadFolderPathEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PartitionTableFileName"))
        {
            mstrPartitionTableFileName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"OverridePrtnTable"))
        {
            mbOverridePrtnTable = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SkipGoToDownload"))
        {
            mbSkipGoToDownload = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"NumberOfPartitionFiles"))
        {
            mnNumberOfPartitionFiles = _ttoi(strParamValue);
        }
        else
        {
            mmapParameters[JGW_W2A(strParamName)] = JGW_W2A(strParamValue);
        }
        return true;
    }

    bool CTSE_StreamingDownloadUserPartitions::TSE_Run()
    {
        if (mnNumberOfPartitionFiles <= 0 || mnNumberOfPartitionFiles > 15)
        {
            LogE(L"Number Of Partition Files out of Range(1-15)");
            return false;
        }

        SWDL_UserPartitionList pPartitonList = {0};
        std::string strDownloadFolderPath = GetGlobalEnvironment()->GetAnsiString(mstrDownloadFolderPathEnv);
        std::string strPartitionName,strPartitionFile,strPartitionNameKey,strPartitionFileKey;
        std::string strPartitionTableFileName;
        if (!mstrPartitionTableFileName.empty())
        {
            strPartitionTableFileName = strDownloadFolderPath + "\\" + JGW_W2A(mstrPartitionTableFileName);
            if (!JGW_FileExistsToFilePathA(strPartitionTableFileName.c_str()))
            {
                LogE_F(L"Partition File(%s) not Exists",JGW_A2W(strPartitionTableFileName).c_str());
                return false;
            }
        }

        pPartitonList.iNumOfPartitionEntry = mnNumberOfPartitionFiles;
        for (int i = 1;i <= mnNumberOfPartitionFiles;i ++)
        {
            JGW_FormatString(strPartitionNameKey,"PartitionName%d",i);
            JGW_FormatString(strPartitionFileKey,"PartitionFile%d",i);
            strPartitionName = mmapParameters[strPartitionNameKey];
            strPartitionFile =  mmapParameters[strPartitionFileKey];
            if (strPartitionName.empty() || strPartitionFile.empty())
            {
                LogE(L"PartitionName And PartitionFile config Is not empty");
                return false;
            }
            strcpy_s(pPartitonList.oPartitionList[i - 1].sPartitionName,129,strPartitionName.c_str());
            std::string strPartitionFilePath = strDownloadFolderPath + "\\" + strPartitionFile;
            strcpy_s(pPartitonList.oPartitionList[i - 1].sPartitionMBNPath,256,strPartitionFilePath.c_str());
            if (!JGW_FileExistsToFilePathA(pPartitonList.oPartitionList[i - 1].sPartitionMBNPath))
            {
                LogE_F(L"Partition File(%s) not Exists",JGW_A2W(pPartitonList.oPartitionList[i - 1].sPartitionMBNPath).c_str());
                return false;
            }
        }

        HANDLE hResourceContext = (HANDLE)GetGlobalEnvironment()->GetInt(mstrResourceContextEnv);
        if (NULL == hResourceContext)
        {
            LogE(L"The phonehandle for the phone object is null");
            return false;
        }

        unsigned char bOverridePrtnTable = mbOverridePrtnTable ? 1 : 0;
        unsigned char bUseTrustedMode = 1;
        unsigned char bSkipGoToDownload = mbSkipGoToDownload ? 1 : 0;
        unsigned long iSleepTimeToDOWNLOOADmode = 500;
        unsigned long iSleepTimeToGOcommand = 500;
        unsigned long iErrorCode = 0;

        QLIB_QPHONEMS_UploadSBMultiImage_ConfigureCallBack(hResourceContext,swdlQPHONEMSCB_Func);
        unsigned char result = QLIB_QPHONEMS_DownloadUserPartitions(hResourceContext, "", const_cast<char*>(strPartitionTableFileName.data()), &pPartitonList, bOverridePrtnTable, bUseTrustedMode, bSkipGoToDownload, iSleepTimeToDOWNLOOADmode, iSleepTimeToGOcommand, &iErrorCode);
        if (TRUE != result)
        {
            LogE(L"Failed to download the partition files onto the device");
            return false;
        }
        return true;
    }
}

