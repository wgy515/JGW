#include "StdAfx.h"
#include "TSE_CheckThroughputTestResult.h"
#include <fstream>
namespace JGW
{
    CTSE_CheckThroughputTestResult::CTSE_CheckThroughputTestResult(void) : mstrlocationString(L"0.0-%d.0")
    {
    }


    CTSE_CheckThroughputTestResult::~CTSE_CheckThroughputTestResult(void)
    {
    }

    const wchar_t* CTSE_CheckThroughputTestResult::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CheckThroughputTestResult\",\
                \"TestName\": \"检查吞吐率测试结果\",\
                \"TestTimeEnvironment\": \"测试时间环境变量\",\
                \"TestRangeEnvironment\": \"测试范围环境变量\",\
                \"LogEnvironment\": \"吞吐率测试log文件路径环境变量\",\
                \"locationString\": \"结果查询定位字符串位置\"\
                }";
    }

    bool CTSE_CheckThroughputTestResult::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestTimeEnvironment"))
        {
            mstrTestTimeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestRangeEnvironment"))
        {
            mstrTestRangeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"LogEnvironment"))
        {
            mstrLogEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"locationString"))
        {
            mstrlocationString = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CheckThroughputTestResult::TSE_Run()
    {
        float fMin =  GetGlobalEnvironment()->GetFloat(mstrTestRangeEnvironment);
        wchar_t szReadBuf[1024] = {0};
        const wchar_t* pBuf  = NULL;
        std::wstring strTemp;
        size_t nTestTime = GetGlobalEnvironment()->GetInt(mstrTestTimeEnvironment);
        std::wstring strLogPath = GetGlobalEnvironment()->GetString(mstrLogEnvironment);

        std::wifstream mFileStream;
        mFileStream.open(strLogPath.c_str());
        if (!mFileStream.is_open())
        {
            LogE_F(L"ThroughputTest Log文件无法打开(%s)\n",strLogPath.c_str());
            return false;
        }

        while (mFileStream.getline(szReadBuf,1024))
        {
            if ('[' == szReadBuf[0]) strTemp = szReadBuf;
        }

        pBuf = wcsstr(strTemp.c_str(),mstrlocationString.c_str());
        if (NULL == pBuf || nTestTime != _ttoi(pBuf + mstrlocationString.length()))
        {
            LogE(L"ThroughputTest 未查询到测试结果\n");
            return false;
        }

        pBuf = wcsstr(strTemp.c_str(),L"Bytes");
        float fTestResult = static_cast<float>(_ttof(pBuf + 5));
        JGW_FormatWString(strTemp,L"%.0f Mbits/sec",fTestResult);
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());

        if (fTestResult >= fMin)
        {
            return true;
        }

        return false;
    }
}