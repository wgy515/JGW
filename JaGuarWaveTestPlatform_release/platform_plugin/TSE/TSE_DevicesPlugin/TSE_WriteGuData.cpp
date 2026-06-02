#include "StdAfx.h"
#include "TSE_WriteGuData.h"
#include <fstream>
namespace JGW
{
    CTSE_WriteGuData::CTSE_WriteGuData(void)
    {
    }


    CTSE_WriteGuData::~CTSE_WriteGuData(void)
    {
    }

    const wchar_t* CTSE_WriteGuData::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_WriteGuData::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("WriteDataCommand"),strParamName))
        {
            mstrWriteDataCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("GuDataFilePathEnvionment"),strParamName))
        {
            mstrGuDataFilePathEnvionment = strParamValue;
        }
        else
        {
            mmapParamNames[strParamName] = strParamValue;
        }
        return true;
    }

    std::wstring CTSE_WriteGuData::FormatWriteDataCommand(const std::wstring& strWriteDataCommand)
    {
        std::wstring strWriteData(strWriteDataCommand);
        for (std::map<std::wstring,std::wstring>::iterator it = mmapParamNames.begin();
            it != mmapParamNames.end();++ it)
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"##_%s_##",it->first.c_str());
            JGW_ReplaceStringW(strWriteData,strTemp,it->second);
        }
        JGW_ReplaceStringW(strWriteData,L"\\t",L"\t");
        JGW_ReplaceStringW(strWriteData,L"\\r",L"\r");
        JGW_ReplaceStringW(strWriteData,L"\\n",L"\n");
        GetEnvironmentCommandString(strWriteData);
        return strWriteData;
    }

    bool CTSE_WriteGuData::TSE_Run()
    {
        std::wstring strGuDataFilePath = GetGlobalEnvironment()->GetString(mstrGuDataFilePathEnvionment);
        strGuDataFilePath = JGW_RealativePathToAbsPath(strGuDataFilePath.c_str());
        JGW_CreateDirectory(strGuDataFilePath.c_str(),false);

        std::wofstream fileStream;
        fileStream.imbue( std::locale(""));
        fileStream.open( strGuDataFilePath.c_str(),std::ios::out | std::ios::ate | std::ios::app);
        if (!fileStream.is_open())
        {
            LogE_F(L"Open Gu Test Data File(%s) Error",strGuDataFilePath.c_str());
            return false;
        }

        if (mstrWriteDataCommand.empty())
        {
            fileStream << std::endl;
        }
        else
        {
            std::wstring strWriteData = FormatWriteDataCommand(mstrWriteDataCommand);
            fileStream << strWriteData;
        }
        fileStream.flush();
        fileStream.close();
        return true;
    }
}
