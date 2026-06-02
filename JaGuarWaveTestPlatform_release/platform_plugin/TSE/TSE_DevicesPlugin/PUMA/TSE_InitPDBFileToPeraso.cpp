#include "StdAfx.h"
#include "TSE_InitPDBFileToPeraso.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "..\TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_InitPDBFileToPeraso::CTSE_InitPDBFileToPeraso(void) : mbInitPDBFile(false)
    {
    }


    CTSE_InitPDBFileToPeraso::~CTSE_InitPDBFileToPeraso(void)
    {
    }

    const wchar_t* CTSE_InitPDBFileToPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_InitPDBFileToPeraso\",\
                \"TestName\": \"初始化PDB文件(Peraso)\",\
                \"Folder\": \"PDB文件目录\",\
                \"FolderEnvironment\": \"PDB文件目录对应的环境变量为空则取Folder配置\"\
                }";
    }

    bool CTSE_InitPDBFileToPeraso::TSE_Init()
    {
        mbInitPDBFile = false;
        return true;
    }

    bool CTSE_InitPDBFileToPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Folder"))
        {
            mstrFolder = JGW_RealativePathToAbsPath(strParamValue);
            mstrFolder += L"\\";
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"FolderEnvironment"))
        {
            mstrFolderEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_InitPDBFileToPeraso::TSE_Run()
    {
        if (mbInitPDBFile) return true;

        if (mstrFolderEnvironment.empty())
        {
            LogD_F(L"Load PDB Image Folder: %s",mstrFolder.c_str());
            mbInitPDBFile = CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->LoadPDBImageFile(JGW_W2A(mstrFolder));
        }
        else
        {
            std::string strFolder = JGW_RealativePathToAbsPathA(GetGlobalEnvironment()->GetAnsiString(mstrFolderEnvironment));
            strFolder += "\\";
            LogD_F(L"Load PDB Image Folder: %s",strFolder.c_str());
            mbInitPDBFile = CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->LoadPDBImageFile(strFolder);
        }

        return mbInitPDBFile;
    }
}