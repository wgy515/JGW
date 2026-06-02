#include "StdAfx.h"
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    std::wstring mstrTestFolder;

    CCJGW_MoveTestLogging::CCJGW_MoveTestLogging(void)
    {
        if (mstrTestFolder.empty())
        {
            mstrTestFolder = JGW_GetApplicationFolder();
            mstrTestFolder += L"TestLog\\";
        }   
    }

    CCJGW_MoveTestLogging::~CCJGW_MoveTestLogging(void)
    {
    }

    void CCJGW_MoveTestLogging::MoveTestLogging(const wchar_t* szTestName,const wchar_t* strSN,const wchar_t* strFilePath,bool bTestStatus /* = true */)
    {
        //! check file is exits
    	if (!JGW_FileExistsToFilePath(strFilePath)) return ;
        std::wstring strSaveFilePath(mstrTestFolder);
        strSaveFilePath += szTestName;
        strSaveFilePath += JGW_GetTimeFolder();
        if (strSN) { strSaveFilePath += L"\\"; strSaveFilePath += strSN;}
        //!if (bTestStatus)
        strSaveFilePath += bTestStatus?L"\\P_":L"\\F_";
        strSaveFilePath += JGW_GetFileNameToFilePath(strFilePath);
        JGW_CreateDirectory(strSaveFilePath.c_str(),false);
        CopyFile(strFilePath,strSaveFilePath.c_str(),FALSE);
    }
}