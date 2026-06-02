#include "StdAfx.h"
#include "TSE_SaveProductScreenLogPeraso.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

#define PRODUCT_SCREEN_XML_REPORT_FILE "ProductScreenToolReport.xml"
#define PRODUCT_SCREEN_VERBOSE_LOG_FILE "ProductScreenToolVerbose.log"
//! #define PRODUCT_SCREEN_TEST_RESULT_TXT "testresult.csv"
#define PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN 10 * 1024
#define PRODUCT_SCREEN_TEST_RESULT_TXT _T("PRODUCT_SCREEN_TEST_RESULT_TXT")
namespace JGW
{
    CTSE_SaveProductScreenLogPeraso::CTSE_SaveProductScreenLogPeraso(void) : mstrSerialNumberPos("2.3.9,")
    {
        mstrLogFolder = JGW_GetApplicationFolder();
        mstrLogFolder += L"dongles\\";
        mszTestResultBuf = (char*)calloc(0x01,PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN);
    }


    CTSE_SaveProductScreenLogPeraso::~CTSE_SaveProductScreenLogPeraso(void)
    {
        free(mszTestResultBuf);
    }

    const wchar_t* CTSE_SaveProductScreenLogPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SaveProductScreenLogPeraso\",\
                \"TestName\": \"保存ProductScreen测试Log(Peraso)\",\
                \"Folder\": \"log目录\",\
                \"SerialNumberPos\": \"SN字符串定位\",\
                \"SNEnvironment\": \"将解析到的字符串保存在指定环境变量中\"\
                }";
    }

    bool CTSE_SaveProductScreenLogPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Folder"))
        {
            mstrLogFolder = JGW_RealativePathToAbsPath(strParamValue);
            mstrLogFolder += L"\\";
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SerialNumberPos"))
        {
            mstrSerialNumberPos = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SNEnvironment"))
        {
            mstrSNEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaveProductScreenLogPeraso::TSE_Init()
    {
        //! mstrTestResultFilePath = JGW_W2A(mstrLogFolder + _T(PRODUCT_SCREEN_TEST_RESULT_TXT));
        mstrProductScreenToolVerboseFilePath = mstrLogFolder + _T(PRODUCT_SCREEN_VERBOSE_LOG_FILE);
        mstrProductScreenToolReport = mstrLogFolder + _T(PRODUCT_SCREEN_XML_REPORT_FILE);
        return true;
    }

    bool CTSE_SaveProductScreenLogPeraso::GetSerialNumber()
    {
        size_t buf_len = PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN;
        memset(mszTestResultBuf,0x00,PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN);
        char* pBuf = NULL;
        char szSerialNumber[30] = {0};

        if (!JGW_GetFileBufToPath_BufLen_C(mstrTestResultFilePath.c_str(),mszTestResultBuf,buf_len))
        {
            LogE(L"Open testresult.txt Failed\n");
            return false;
        }

        if (PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN < buf_len)
        {
            LogE(L"testresult.txt file size Too Large\n");
            return false;
        }

        pBuf = strstr(mszTestResultBuf,mstrSerialNumberPos.c_str());
        if (NULL == pBuf)
        {
            LogE(L"Get Serail Number Failed\n");
            return false;
        }

        pBuf += strlen(mstrSerialNumberPos.c_str());
        for (int i = 0;i < 29;i ++)
        {
            if (',' == *pBuf) break;
            szSerialNumber[i] = *pBuf;
            ++ pBuf;
        }
        mstrSerialNumber = JGW_A2W_A(szSerialNumber);
        return true;
    }

    bool CTSE_SaveProductScreenLogPeraso::TSE_Run()
    {
        mstrTestResultFilePath = JGW_W2A(mstrLogFolder + GetGlobalEnvironment()->GetString(PRODUCT_SCREEN_TEST_RESULT_TXT));
        //! 获取SN
        if (mstrSNEnvironment.empty())
        {
            if (!GetSerialNumber()) return false;
        }
        else
        {
            mstrSerialNumber = GetGlobalEnvironment()->GetString(mstrSNEnvironment);
        }
        bool bResult = GetGlobalEnvironment()->GetInt(TSE_TEST_RESULT)?true:false;

        mMoveTestLoggin.MoveTestLogging(GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),mstrSerialNumber.c_str(),mstrProductScreenToolReport.c_str(),bResult);
        mMoveTestLoggin.MoveTestLogging(GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),mstrSerialNumber.c_str(),mstrProductScreenToolVerboseFilePath.c_str(),bResult);
        mMoveTestLoggin.MoveTestLogging(GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),mstrSerialNumber.c_str(),JGW_A2W(mstrTestResultFilePath).c_str(),bResult);
        DeleteFile(mstrProductScreenToolReport.c_str());
        DeleteFile(mstrProductScreenToolVerboseFilePath.c_str());
        DeleteFileA(mstrTestResultFilePath.c_str());

        return true;
    }
}