#include "StdAfx.h"
#include "TSE_WriteInstrumentCommand.h"
#include "CJGW_TSEIntrument.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CTSE_WriteInstrumentCommand::CTSE_WriteInstrumentCommand(void) : mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT")
    {
    }


    CTSE_WriteInstrumentCommand::~CTSE_WriteInstrumentCommand(void)
    {
    }

    const wchar_t* CTSE_WriteInstrumentCommand::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WriteInstrumentCommand\",\
                \"TestName\": \"Write Instrument Command\",\
                \"InstrumentCommand\": \"Instrument Envrionment Command\",\
                \"FindOkString\": \"Instrument Command Succeed Flag\",\
                \"InstrumentEnvironment\":\"Save Instrument instance,default:TSE_INSTRUMENT_ENVIRONMENT\"\
                }";
    }

    bool CTSE_WriteInstrumentCommand::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("InstrumentCommand"),strParamName))
        {
            mstrInstrumentCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("FindOkString"),strParamName))
        {
            mstrFindOkString = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("InstrumentEnvironment"),strParamName))
        {
            mstrInstrumentEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WriteInstrumentCommand::TSE_Run()
    {
        std::string strCommand;
        std::wstring wstrCommand (mstrInstrumentCommand);
        GetEnvironmentCommandString(wstrCommand);
#if 1
        JGW_FormatString(strCommand,"%s;"
            "*CLS;"
            "*OPC?;:SYST:ERR?;"
            ,JGW_W2A(wstrCommand).c_str());
        CCJGW_TSEIntrument* pTSEIntrument = (CCJGW_TSEIntrument*)(GetGlobalEnvironment()->GetInt(mstrInstrumentEnvironment));
        if (NULL == pTSEIntrument)
        {
            LogE(L"Not Find TSEIntrument instance");
            return false;
        }

        IEquipCommTL* pEquipInter = pTSEIntrument->GetIEquipCommTL();
        if (!pEquipInter)
        {
            LogE(L"仪器未连接，请先连接仪器");
            return false;
        }

        pEquipInter->write(strCommand);
        pEquipInter->read(strCommand);

        if (strCommand.empty())
        {
            LogE(L"请检查仪器连接是否已经中断");
            return false;
        }

        if (!mstrFindOkString.empty() && NULL == JGW_StrCaseStr(strCommand.c_str(),mstrFindOkString.c_str()))
        {
            LogE_F("Error:%s", strCommand.c_str());
            return false;
        }
#else

        IEquipCommTL* pEquipInter = CCJGW_TSEIntrument::GetIEquipCommTL();
        if (!pEquipInter)
        {
            LogE(L"仪器未连接，请先连接仪器");
            return false;
        }
        strCommand = JGW_W2A(wstrCommand);
        pEquipInter->write(strCommand);

#endif
#if 0
        if ('+' == strCommand[0])
        {
            if ( '1' != strCommand[1] || '0' !=  strCommand[4] )
            {
                LogE_F("Error [%s],%s, %d ", strCommand.c_str(), __FUNCTION__, __LINE__ );
                return false;
            }
            return true;
        }

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            LogE_F("Error [%s],%s, %d ", strCommand.c_str(), __FUNCTION__, __LINE__ );
            return false;
        }
#endif

        return true;
    }
}