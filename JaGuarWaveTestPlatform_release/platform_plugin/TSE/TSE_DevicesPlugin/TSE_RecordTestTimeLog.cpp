#include "StdAfx.h"
#include "TSE_RecordTestTimeLog.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <fstream>
namespace JGW
{
    CTSE_RecordTestTimeLog::CTSE_RecordTestTimeLog(void) : mbIsETime(false),mbIsNewLine(false) ,mbIsRecordTime(true)
    {
    }


    CTSE_RecordTestTimeLog::~CTSE_RecordTestTimeLog(void)
    {
    }

    const wchar_t* CTSE_RecordTestTimeLog::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RecordTestTimeLog\",\
                \"TestName\": \"Record Test Time Log\",\
                \"TestTimeLogEnvironment\": \"Environment Save Test Time Log\",\
                \"TestLog\": \"Test Log Command\",\
                \"IsRecordTime\": \"Is Record Time\",\
                \"StartTestTimeEnvironment\": \"Save Start Test Time Environment\",\
                \"IsETime\": \"Calculated time interval\",\
                \"IsNewLine\": \"new line\"\
                }";
    }

    bool CTSE_RecordTestTimeLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestTimeLogEnvironment"))
        {
            mstrTestTimeLogEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsNewLine"))
        {
            mbIsNewLine = _ttoi(strParamValue) == 1;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsETime"))
        {
            mbIsETime = _ttoi(strParamValue) == 1;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsRecordTime"))
        {
            mbIsRecordTime = _ttoi(strParamValue) == 1;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"StartTestTimeEnvironment"))
        {
            mstrStartTestTimeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestLog"))
        {
            mstrTestLog = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_RecordTestTimeLog::TSE_Run()
    {
        std::wofstream iFileStream;
        std::wstring strTestTimeLogEnvironment = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrTestTimeLogEnvironment).c_str());
        iFileStream.open(strTestTimeLogEnvironment.c_str(), std::ios::app);
        if (!iFileStream.is_open()) return false;
        if (mbIsNewLine)
        {
            iFileStream << std::endl << L"S" << JGW_GetTimeFolder() << L",";
        }
        SYSTEMTIME sTime1;
        GetLocalTime(&sTime1);
        ULARGE_INTEGER fTime1;
        SystemTimeToFileTime(&sTime1,(FILETIME*)&fTime1);
        std::wstring strTmep;
        //! 是否记录时间
        if (mbIsRecordTime)
        {
            JGW_FormatWString(strTmep,L"S%02d:%02d:%02d.%03d",sTime1.wHour,sTime1.wMinute,sTime1.wSecond,sTime1.wMilliseconds);
            iFileStream << strTmep << L",";
        }
        //! 是否记录时间差
        if (mbIsETime)
        {
            ULARGE_INTEGER fTimeStart;
            std::string strTimeStart = GetGlobalEnvironment()->GetAnsiString(mstrStartTestTimeEnvironment);
            sscanf_s(strTimeStart.c_str(),"%llu",&fTimeStart.QuadPart);
            JGW_FormatWString(strTmep,L"%llu ms",(fTime1.QuadPart - fTimeStart.QuadPart) / 10000);
            iFileStream << strTmep << L",";
        }
        //! 是否保存起始时间
        if (!mstrStartTestTimeEnvironment.empty())
        {
            JGW_FormatWString(strTmep,L"%llu",fTime1.QuadPart);
            GetGlobalEnvironment()->PutString(mstrStartTestTimeEnvironment,strTmep);
        }

        //! 指定测试log信息
        if (!mstrTestLog.empty())
        {
            std::wstring strTestLog (mstrTestLog);
            GetEnvironmentCommandString(strTestLog);
            iFileStream << strTestLog << L",";
        }

        iFileStream.close();
        return true;
    }
}
