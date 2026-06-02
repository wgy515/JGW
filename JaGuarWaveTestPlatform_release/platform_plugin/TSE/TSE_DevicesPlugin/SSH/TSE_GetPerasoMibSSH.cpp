#include "StdAfx.h"
#include "TSE_GetPerasoMibSSH.h"
#include "../TSE_DevicesGlobalResource.h"
#define MAX_PERASO_CONNECT 20
namespace JGW
{
    CTSE_GetPerasoMibSSH::CTSE_GetPerasoMibSSH(void) : mstrMibOperationCommand(L"mib q b"),mstrPrsBinPath(L"prs_serial")
    {
    }


    CTSE_GetPerasoMibSSH::~CTSE_GetPerasoMibSSH(void)
    {
    }

    const wchar_t* CTSE_GetPerasoMibSSH::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetPerasoMibSSH\",\
                \"TestName\": \"读写peraso mib指令(涉及ROW)\",\
                \"EraseStringSplit\": \"delete string split example:AT+GSN,OK,\\r,\\n, \",\
                \"PrsBinPath\": \"prs_serial bin path\",\
                \"TestWaitResult\": \"发送测试信息等待结果生产时间MS\",\
                \"ReadResultEnvironment\": \"保存命令返回值到环境变量中\",\
                \"Class\": \"Decimal\",\
                \"Group\": \"Decimal\",\
                \"Entry;\": \"Decimal\",\
                \"WriteOnly\": \"只写不读标记位\",\
                \"MibOperationCommand\": \"mib operation command\",\
                \"CheckResultTimeOut\": \"检查测试结果超时时间\",\
                \"CompareStringCount\": \"比较多少项字符串\",\
                \"CompareStringName1\": \"key值 1是索引\",\
                \"CompareStringValue1\": \"与KEY值对应VALUE进行比对\",\
                \"CompareStringErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"CompareIntCount\": \"比对多少项INT类型\",\
                \"CompareIntName1\": \"key值 1是索引\",\
                \"CompareIntValue1\": \"与KEY值对应VALUE进行比对,A~C,当前值在大于等于A小于等于C即可\",\
                \"CompareIntErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"FindStringCount\": \"查找字符串,需要匹配多少项\",\
                \"FindStringName1\": \"key值 1是索引\",\
                \"FindStringValue1\": \"查找当前值是否在key值对应字符串中\",\
                \"FindStringErrorMsg1\": \"比对失败后显示的错误字符串\"\
                }";
    }

    bool CTSE_GetPerasoMibSSH::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("EraseStringSplit"),strParamName) )
        {
            mstrEraseStringSplit = strParamValue;
        } 
        else if( TSE_PARAM_NAME_EQUAL(_T("MibOperationCommand"),strParamName) )
        {
            mstrMibOperationCommand = strParamValue;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("PrsBinPath"),strParamName) )
        {
            mstrPrsBinPath = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Class"))
        {
            mnClass = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Group"))
        {
            mnGroup = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Entry"))
        {
            mnEntry = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestWaitResult"))
        {
            mnTestWaitResult = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckResultTimeOut"))
        {
            mnCheckResultTimeOut = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultEnvironment"))
        {
            mstrReadResultEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WriteOnly"))
        {
            mbWriteOnly = (1 == _ttoi(strParamValue));
        }
        else
        {
            mCheckTestResult.AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_GetPerasoMibSSH::TSE_CheckTestResult(const std::string& strRead)
    {
        std::wstring strReadBuf = JGW_A2W(strRead);
        LogD(strReadBuf.c_str());

        if(!mCheckTestResult.CheckCompareResult(strReadBuf))
        {
            Sleep(mnIntervalTimeMS);
            return false;
        }
        return true;
    }

    int CTSE_GetPerasoMibSSH::GetPerasoStationRow()
    {
        std::wstring strCommand;
        std::string strRead;
        for (int i = 0;i < MAX_PERASO_CONNECT;i ++)
        {
            JGW_FormatWString(strCommand,L"%s 'station %d'",mstrPrsBinPath.c_str(),i);
            if (!CTSE_DevicesGlobalResource::GetSSHShell()->RWSSHShell(JGW_W2A(strCommand),strRead))
            {
                LogE(L"Sending the SSH command failed\n");
                return false;
            }
            std::string strState = JGW_GetSubStrToStartEndStr(strRead,"State:");
            if (std::string::npos != strState.find("0x4"))
            {
                return i + 1;
            }
        }
        return -1;
    }


    bool CTSE_GetPerasoMibSSH::TSE_Run()
    {
        std::wstring strCommand;
        int row = GetPerasoStationRow();
        if (row < 0)
        {
            LogE(L"get peraso row fail.");
            return false;
        }
        //! prs_serial "mib q b 12-8-1-c"
        JGW_FormatWString(strCommand,L"%s '%s %x-%x-%x-%x'",mstrPrsBinPath.c_str(),mstrMibOperationCommand.c_str(),mnClass,mnGroup,row,mnEntry);
        if (!CTSE_DevicesGlobalResource::GetSSHShell()->SendSSHShell(JGW_W2A(strCommand)))
        {
            LogE(L"Sending the SSH command failed\n");
            return false;
        }

        if (mbWriteOnly)
        {
            return true;
        }

        Sleep(mnTestWaitResult);
        std::string strRead;
        CTSE_DevicesGlobalResource::GetSSHShell()->RecvSSHShell(strRead,mnCheckResultTimeOut);
        JGW_EraseFristToRemoveChar(strRead,L'\n');
        JGW_EraseAfterToRemoveChar(strRead,L'\n'); 
        JGW_EraseLastAndFristTwoChars(strRead);
        LogI(JGW_A2W(strRead).c_str());

        for (size_t i = 0;i < mvstrEraseString.size();i ++)
        {
            JGW_ReplaceStringA(strRead,mvstrEraseString[i],"");
        }

        if (!mstrReadResultEnvironment.empty())
        {
            LogI_F(L"%s:%s",mstrReadResultEnvironment.c_str(),JGW_A2W(strRead).c_str());
            GetGlobalEnvironment()->PutAnsiString(mstrReadResultEnvironment,strRead);
        }

        return TSE_CheckTestResult(strRead);
    }

    bool CTSE_GetPerasoMibSSH::TSE_Init()
    {
        mCheckTestResult.InitCompareParam();
        std::string strEraseStringSplit (JGW_W2A(mstrEraseStringSplit));
        JGW_ReplaceStringA(strEraseStringSplit,"\\\\r","\r");
        JGW_ReplaceStringA(strEraseStringSplit,"\\\\n","\n");
        JGW_ReplaceStringA(strEraseStringSplit,"\\\\t","\t");
        JGW_ReplaceStringA(strEraseStringSplit,"\\\\,","\''");
        mvstrEraseString.clear();
        JGW_ParserStrA(strEraseStringSplit.c_str(),",",mvstrEraseString);
        for (size_t i = 0;i < mvstrEraseString.size();i ++)
        {
            JGW_ReplaceStringA(mvstrEraseString[i],"\''",",");
        }
        return true;
    }

}
