#pragma once
#include <Poco/Process.h>
#include <Poco/Pipe.h>

namespace JGW
{
    class RedirectConsole
    {
    public:
        RedirectConsole(void);
        ~RedirectConsole(void);
    public:
        bool LaunchConsole(const std::wstring& cmd,const std::vector<std::wstring>& args,const std::wstring& initialDirectory);
        bool LaunchConsole(const std::string& cmd,const std::vector<std::string>& args,const std::string& initialDirectory);
        void KillConsole();
        bool ConsoleisRunning();
        int WaitConsole();
    public:
        //int RedirectIn(const std::wstring& in);
        int RedirectIn(const std::string& in);
        int RedirectOut(std::string& out);
        int RedirectOutTimeOut(std::string& out,int timeOutMilliseconds = 100);
    private:
        HANDLE mhProcess;
        Poco::Pipe mInPipe;
        Poco::Pipe mOutPipe;
        Poco::ProcessHandle* mpProcessHandle;
    };
}
