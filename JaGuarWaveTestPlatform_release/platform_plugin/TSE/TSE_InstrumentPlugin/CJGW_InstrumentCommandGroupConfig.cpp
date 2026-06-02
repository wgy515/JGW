#include "StdAfx.h"
#include "CJGW_InstrumentCommandGroupConfig.h"
#include <tinyxml/tinyxml.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    std::map<std::wstring,std::vector<S_INSTRUMENT_COMMAND_GROUP>> CCJGW_InstrumentCommandGroupConfig::mmapConfigPathCommandGroup;

    CCJGW_InstrumentCommandGroupConfig::CCJGW_InstrumentCommandGroupConfig(void)
    {
    }


    CCJGW_InstrumentCommandGroupConfig::~CCJGW_InstrumentCommandGroupConfig(void)
    {
    }

    bool CCJGW_InstrumentCommandGroupConfig::GetCommandGroup(const std::wstring& strConfigPath,const std::wstring& strCommandGroupName,std::vector<S_INSTRUMENT_COMMAND>& vsInstrumentCommand)
    {
        std::map<std::wstring,std::vector<S_INSTRUMENT_COMMAND_GROUP>>::iterator it = mmapConfigPathCommandGroup.find(strConfigPath);
        if (it == mmapConfigPathCommandGroup.end())
        {
            CCJGW_InstrumentCommandGroupConfig instrumentCommandGroupConfig;
            if (!instrumentCommandGroupConfig.LoadInstrumentCommandGroupConfig(strConfigPath))
            {
                return false;
            }
			
			it = mmapConfigPathCommandGroup.find(strConfigPath);
	        if (it == mmapConfigPathCommandGroup.end())
	        {
	            return false;
	        }
        }
  
        for (std::vector<S_INSTRUMENT_COMMAND_GROUP>::iterator itSInstrumentCommand = it->second.begin();
            itSInstrumentCommand != it->second.end(); ++itSInstrumentCommand)
        {
            if (0 == itSInstrumentCommand->strName.compare(strCommandGroupName))
            {
                vsInstrumentCommand.clear();
                for (std::vector<S_INSTRUMENT_COMMAND>::iterator itCommand = itSInstrumentCommand->vsInstrumentCommands.begin();
                    itCommand != itSInstrumentCommand->vsInstrumentCommands.end();
                    ++ itCommand)
                {
                    vsInstrumentCommand.push_back(itCommand[0]);
                }
                return true;
            }
        }
        return false;
    }

    bool CCJGW_InstrumentCommandGroupConfig::LoadInstrumentCommandGroupConfig(const std::wstring& strConfigPath)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot,*pCommandGroup = NULL, *pCommnadElement = NULL;

        if (!ptrXmlDoc->LoadFile(JGW_W2A(strConfigPath).c_str())) return false;

        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;

        std::vector<S_INSTRUMENT_COMMAND_GROUP> vsInstrumentCommandGroup;
        std::wstring strTemp,strName;
        pCommandGroup = pRoot->FirstChildElement("CommandGroup");
        while (pCommandGroup)
        {
            S_INSTRUMENT_COMMAND_GROUP sInstrumentCommandGroup;
            GetElementAttributeValue(pCommandGroup,"name",strName,L"");
            if (!strName.empty())
            {
                pCommnadElement = pCommandGroup->FirstChildElement("Command");
                while (pCommnadElement)
                {
                    S_INSTRUMENT_COMMAND sInstrumentCommand;
                    GetElementAttributeValue(pCommnadElement,"input",sInstrumentCommand.mstrInput,L"");
                    GetElementAttributeValue(pCommnadElement,"find_output",sInstrumentCommand.mstrFindOutPut,L"");
                    GetElementAttributeValue(pCommnadElement,"timeout_sec",strTemp,L"1");
                    sInstrumentCommand.mnTimeOutSec = _ttoi(strTemp.c_str());
                    if (!sInstrumentCommand.mstrInput.empty())
                    {
                        sInstrumentCommandGroup.vsInstrumentCommands.push_back(sInstrumentCommand);
                    }
                    pCommnadElement = pCommnadElement->NextSiblingElement("Command");
                }
                sInstrumentCommandGroup.strName = strName;
                vsInstrumentCommandGroup.push_back(sInstrumentCommandGroup);
            }
            pCommandGroup = pCommandGroup->NextSiblingElement("CommandGroup");
        }
        mmapConfigPathCommandGroup[strConfigPath] = vsInstrumentCommandGroup;
        return true;
    }
}

