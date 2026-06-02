#include "StdAfx.h"
#include "TSE_QIATest.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_QIATest::CTSE_QIATest(void) : mbIsCalTest(false)£¬mnShowTestLogType(1)
    {
    }


    CTSE_QIATest::~CTSE_QIATest(void)
    {
        mcQIARFTestHelp.UninitializeQIARFTest();
    }

    const wchar_t* CTSE_QIATest::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_QIATest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"IsCalTest",strParamName))
        {
            mbIsCalTest = _ttoi(strParamValue) == 1;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SerialNumberEnvironment",strParamName))
        {
            mstrSerialNumberEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ShowTestLogType",strParamName))
        {
            mnShowTestLogType = _ttoi(strParamValue);
            if (mnShowTestLogType < 0 || mnShowTestLogType > 3) 
                mnShowTestLogType = 1;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"QCServerDllPath",strParamName))
        {
            mstrQCServerDllPath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"XttFilePathCommand",strParamName))
        {
            mstrXttFilePathCommand = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }


    bool CTSE_QIATest::TSE_Run()
    {
        if (!mcQIARFTestHelp.InitializeQIARFTest(mbIsCalTest,JGW_W2A(mstrQCServerDllPath)))
        {
            LogE(L"Initialize QIA Test Envrionment Fail");
            return false;
        }
        mcQIARFTestHelp.ClearGlobalVariable();
        std::wstring strXttFilePathCommand(mstrXttFilePathCommand);
        GetEnvironmentCommandString(strXttFilePathCommand);
        std::string strXttFilePath = JGW_W2A(JGW_RealativePathToAbsPath(strXttFilePathCommand.c_str()));

        if (mstrSerialNumberEnvironment.empty())
        {
            mcQIARFTestHelp.SetGlobalVariable("SN","");
        }
        else
        {
            mcQIARFTestHelp.SetGlobalVariable("SN",GetGlobalEnvironment()->GetAnsiString(mstrSerialNumberEnvironment));
        }
        mcQIARFTestHelp.SetGlobalVariable("XTT_PATH_PASSED_TO_TEST_SERVER",strXttFilePath.c_str());
        if (!mbIsCalTest)
        {
            mcQIARFTestHelp.SetGlobalVariable("RunByTestServer","1");
        }
        return mcQIARFTestHelp.ExecutionEngineThread(strXttFilePath);
    }
}
