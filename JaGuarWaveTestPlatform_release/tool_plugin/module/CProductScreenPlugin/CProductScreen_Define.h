#pragma once
#include <string>
#define PRODUCT_SCREEN_XML_REPORT_FILE "ProductScreenToolReport.xml"
#define PRODUCT_SCREEN_VERBOSE_LOG_FILE "ProductScreenToolVerbose.log"
#define PRODUCT_SCREEN_TEST_RESULT_TXT "testresult.txt"
#define PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN 10 * 1024
#define PRODUCT_SCREEN_TEST_TIME_OUT_SEC 60
namespace JGW
{
    typedef struct 
    {
        HWND mhMainWnd;
        std::string mstrPDBImageFolder;
        std::wstring mstrTestLogFolder;
        std::wstring mstrProductScreenToolFolder;
        std::wstring mstrCMDFilePath;
        std::wstring mstrReportXmlFilePath;
        std::wstring mstrVerboseLogFilePath;
        std::wstring mstrTestResultFilePath;
    }S_PRODUCT_SCREEN_PARAM,*LPS_PRODUCT_SCREEN_PARAM;
}
