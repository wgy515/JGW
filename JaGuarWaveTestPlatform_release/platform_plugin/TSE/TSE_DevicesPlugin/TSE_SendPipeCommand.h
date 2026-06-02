#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <map>

namespace JGW
{
    struct  S_COMMAND_PIPE
    {
        int m_nTimeOut;
        int m_nSendCounts;
        std::wstring mstrCommand; //! 发送的命令
        std::wstring mstrRecv; //! 接收
		std::wstring mstrErrorRecv;
        std::wstring mstrEnvironment; //! 保存接收到的字符串
    };

    class CTSE_SendPipeCommand : public CTSE_TestBase
    {
    public:
        CTSE_SendPipeCommand(void);
        ~CTSE_SendPipeCommand(void);
    public:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Init();

        bool TSE_Run();
    private:
        bool WriteReadPide(S_COMMAND_PIPE scommand);
		void GetEnvironmentCommand(std::wstring& strCommand);
        //! true比对一致 false比对不一致
        bool FindRecvResult(const std::wstring& strRead,const std::wstring& strCompare);
    private:
        std::map<std::wstring,std::wstring> m_mParam;
        int m_nCommandCount;
        std::wstring m_strApplicationPath;
        std::wstring m_strCommandPath;
        std::wstring m_strCommandLine;
        std::vector<S_COMMAND_PIPE> m_vCommand;
        CCJGW_ProcessPipe* m_pCMDPipe;
    };
}


