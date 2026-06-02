#pragma once
#include "RedirectConsole.h"
namespace JGW
{
    typedef struct  
    {
		//! 命令写入次数
        int mnSendCount;
		//! 总超时时间毫秒
        int mnTimeOutMilliseconds;
		//! 测试错误重测间隔时间毫秒
        int mnErrorTestIntervalMilliseconds;
		//! 命令
        std::wstring mstrCommand;
		//! 查找结果
        std::wstring mstrFindRecv;
		//! 接收结果参数
        std::wstring mstrRecvResult;
    }S_CONSOLE_COMMAND;

    class SendPipeCommand
    {
    public:
        SendPipeCommand(void);
        ~SendPipeCommand(void);
    public:
		void SetLaunchRedirectConsoleParam(const std::string& cmd,const std::vector<std::string>& args,const std::string& initialDirectory);
		bool ReLaunchRedirectConsole();
        bool LaunchRedirectConsole(const std::wstring& cmd,const std::vector<std::wstring>& args,const std::wstring& initialDirectory);
        bool LaunchRedirectConsole(const std::string& cmd,const std::vector<std::string>& args,const std::string& initialDirectory);
        bool SendConsoleCommand(const S_CONSOLE_COMMAND& sConsoleCommand,std::wstring& strRecv);
        bool SendAdbShellCommand(const S_CONSOLE_COMMAND& sConsoleCommand,std::wstring& strRecv);

        bool ConsoleisRunning();
        void KillConsole();
      public:
        bool ReadConsoleProcessPipeEOF(std::wstring& strRecv,int nTimeOutMilliseconds = 3 * 1000);
        bool ReadAdbShellPipe(const std::wstring& strFind,const std::wstring& strCommand,std::wstring& strRecv,int nTimeOutMilliseconds = 3 * 1000);
    private:
		std::string mCmd;
        std::string mInitialDirectory;
		std::vector<std::string> mArgs;
        std::vector<char> mvEofChars;
        RedirectConsole mRedirectConsole;
    };
}
