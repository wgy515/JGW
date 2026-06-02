#include "StdAfx.h"
#include "TSE_MultiWriteInstrumentCommand.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_InstrumentCommandGroupConfig.h"

namespace JGW
{
    CTSE_MultiWriteInstrumentCommand::CTSE_MultiWriteInstrumentCommand(void) : mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT"),mstrInstrumentConfigFilePathEnvironment(L"TSE_INSTRUMENT_CONFIG_PATH")
    {
    }


    CTSE_MultiWriteInstrumentCommand::~CTSE_MultiWriteInstrumentCommand(void)
    {
    }

    const wchar_t* CTSE_MultiWriteInstrumentCommand::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_MultiWriteInstrumentCommand::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("InstrumentEnvironment"),strParamName))
        {
            mstrInstrumentEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("CommandGroupName"),strParamName))
        {
            mstrCommandGroupName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("InstrumentConfigFilePath"),strParamName))
        {
            mstrInstrumentConfigFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("InstrumentConfigFilePathEnvironment"),strParamName))
        {
            mstrInstrumentConfigFilePathEnvironment = strParamValue;
        }
        else
        {
            mmapParam[strParamName] = strParamValue;
        }
        return true;
    }

    void CTSE_MultiWriteInstrumentCommand::GetInstrumentEnvironmentCommandString(std::wstring& strCommand)
    {
        std::wstring strTemp = strCommand,strEnvironmentValue,strEnvironmentName,strReplaceSrc;
        int nStart = strTemp.find(L"$$_"),nEnd = 0;

        while (std::wstring::npos != nStart)
        {
            nEnd = strTemp.find(L"_$$",nStart + 3);
            if (nEnd == std::wstring::npos) return ;
            strEnvironmentName = strTemp.substr(nStart + 3,nEnd - nStart - 3);
            if (mmapParam.end() == mmapParam.find(strEnvironmentName))
            {
                strEnvironmentValue = GetGlobalEnvironment()->GetString(strEnvironmentName);
            }
            else
            {
                strEnvironmentValue = mmapParam[strEnvironmentName];
                GetEnvironmentCommandString(strEnvironmentValue);
            }
            JGW_FormatWString(strReplaceSrc,L"$$_%s_$$",strEnvironmentName.c_str());
            JGW_ReplaceStringW(strCommand,strReplaceSrc,strEnvironmentValue);
            nStart = strTemp.find(L"$$_",nEnd + 3);
        } 
        GetLogServices()->DebugLogFormat(L"Environment Command : %s",strCommand.c_str());
    }

    bool CTSE_MultiWriteInstrumentCommand::WriteReadCommand(IEquipCommTL* pEquipInter,const std::string& strCommand,const std::string& strFindOutput,int timeOutSec)
    {
        std::string retVal;
        for (int i = 0;i < timeOutSec * 5;i ++)
        {
            pEquipInter->query(strCommand,retVal);
            if (std::string::npos != retVal.find(strFindOutput.c_str()))
            {
                return true;
            }
            Sleep(200);
        }
        LogE_F("Send Equip Command Fail : %s(Recv : %s)",strCommand.c_str(),retVal.c_str());
        return false;
    }

    bool CTSE_MultiWriteInstrumentCommand::TSE_Run()
    {
        CCJGW_TSEIntrument* pTSEIntrument = (CCJGW_TSEIntrument*)(GetGlobalEnvironment()->GetInt(mstrInstrumentEnvironment));
		if (!pTSEIntrument)
		{
			LogE(L"仪器未连接，请先连接仪器");
			return false;
		}
        IEquipCommTL* pEquipInter = pTSEIntrument->GetIEquipCommTL();
        if (!pEquipInter)
        {
            LogE(L"仪器未连接，请先连接仪器");
            return false;
        }

        if (!mstrInstrumentConfigFilePathEnvironment.empty())
        {
            mstrInstrumentConfigFilePath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrInstrumentConfigFilePathEnvironment).c_str());
        }

        std::vector<S_INSTRUMENT_COMMAND> vInstrumentCommand;
        if (!CCJGW_InstrumentCommandGroupConfig::GetCommandGroup(mstrInstrumentConfigFilePath,mstrCommandGroupName,vInstrumentCommand))
        {
            LogE(L"获取仪器命令组失败，请检查相关仪器配置");
            return false;
        }

        for (std::vector<S_INSTRUMENT_COMMAND>::iterator it = vInstrumentCommand.begin();
            it != vInstrumentCommand.end();
            ++it)
        {
            std::wstring strCommand(it->mstrInput);
            GetInstrumentEnvironmentCommandString(strCommand);
            if (it->mstrFindOutPut.empty())
            {
                pEquipInter->write(JGW_W2A(strCommand).c_str());
            }
            else
            {
                if (!WriteReadCommand(pEquipInter,JGW_W2A(strCommand),JGW_W2A(it->mstrFindOutPut),it->mnTimeOutSec))
                {
                    Sleep(200);
                    return false;
                }
            }
        }
        return true;
    }
}
