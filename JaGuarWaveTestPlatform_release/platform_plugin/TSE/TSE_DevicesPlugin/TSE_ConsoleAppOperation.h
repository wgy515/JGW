#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <vector>
#include <map>
namespace JGW
{
    typedef struct  
    {
        //! 是否等待CMD命令结束
        bool mbWaitCommandEnd;
        //! 单次配置超时时间，默认超时 10s
        int mnTimeOut;
        
        //! 单次发送总次数，默认一次
        int mnErrorReTestCount;
        
        //! 出现错误的测试间隔时间毫秒
        int mnErrorTestIntervalMSec;
        
        //! 发送的命令
        std::wstring mstrCommand; 
        
        //! 查找到当前字符表示成功
        std::wstring mstrFindOk; 
        
        //! stl 正则表达式规则
        std::wstring mstrRegexRule;
        
        //! 保存接收到的字符串
        std::wstring mstrResultEnvironment; 
        
    }S_CONSOLE_COMMAND_INFO,*PS_CONSOLE_COMMAND_INFO;

    class CTSE_ConsoleAppOperation : public CTSE_TestBase
    {
    public:
        CTSE_ConsoleAppOperation(void);
        ~CTSE_ConsoleAppOperation(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Init();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool StartConsoleApplication(CCJGW_ProcessPipe* pProcessPipe);
        bool ReadWriteConsoleApplication(CCJGW_ProcessPipe* pProcessPipe,PS_CONSOLE_COMMAND_INFO psConsoleCommandInfo);
        bool ReadConsoleApplication(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec = 10);
        bool CheckReadResult(const std::wstring& strRead,PS_CONSOLE_COMMAND_INFO psConsoleCommandInfo);
        bool CheckReadConsoleResult(CCJGW_ProcessPipe* pProcessPipe,PS_CONSOLE_COMMAND_INFO psConsoleCommandInfo,std::wstring& strRead);
    private:
        //! 指定命令数量
        int m_nCommandCount;
        
        //! 进程操作保存环境变量
        std::wstring mstrProcessPipeEnvironment;
        
        //! 控制台进程路径
        std::wstring m_strConsoleAppPath;
        
        //! 管道进程类
        CCJGW_ProcessPipe mcProcessPipe;
        
        //! 配置参数集合
        std::map<std::wstring,std::wstring> m_mParam;
        
        //! 命令集合
        std::vector<S_CONSOLE_COMMAND_INFO> m_vCommand;
    };

}

