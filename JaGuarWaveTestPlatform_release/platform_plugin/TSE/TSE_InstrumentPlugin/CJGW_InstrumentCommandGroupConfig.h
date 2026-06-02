#pragma once
#include <map>
#include <string>
#include <vector>
namespace JGW
{
    typedef struct  
    {
        int mnTimeOutSec;
        std::wstring mstrInput;
        std::wstring mstrFindOutPut;
    }S_INSTRUMENT_COMMAND;

    typedef struct  
    {
        std::wstring strName;
        std::vector<S_INSTRUMENT_COMMAND> vsInstrumentCommands;
    }S_INSTRUMENT_COMMAND_GROUP;

    class CCJGW_InstrumentCommandGroupConfig
    {
    public:
        CCJGW_InstrumentCommandGroupConfig(void);
        ~CCJGW_InstrumentCommandGroupConfig(void);
    public:
        static bool GetCommandGroup(const std::wstring& strConfigPath,const std::wstring& strCommandGroupName,std::vector<S_INSTRUMENT_COMMAND>& vsInstrumentCommand);
    private:
        bool LoadInstrumentCommandGroupConfig(const std::wstring& strConfigPath);
    private:
        static std::map<std::wstring,std::vector<S_INSTRUMENT_COMMAND_GROUP>> mmapConfigPathCommandGroup;
    };
}
