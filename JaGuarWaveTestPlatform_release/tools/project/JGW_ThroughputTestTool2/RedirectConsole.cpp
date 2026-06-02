#include "StdAfx.h"
#include "RedirectConsole.h"

namespace JGW
{
    RedirectConsole::RedirectConsole(void) : mpProcessHandle(NULL),mhProcess(NULL)
    {

    }


    RedirectConsole::~RedirectConsole(void)
    {
        KillConsole();
    }

    void RedirectConsole::KillConsole()
    {
        if (NULL != mpProcessHandle)
        {
            try
            {
                char szBuf[2] = {0x03,'\n'};
                if (ConsoleisRunning()) 
                {
                    mInPipe.writeBytes(szBuf,2);
                    mInPipe.writeBytes(szBuf,2);
                }
                Sleep(100);
                Poco::Process::kill(*mpProcessHandle);
                mpProcessHandle = NULL;
            }
            catch (Poco::SystemException& )
            {

            }
            catch (Poco::WriteFileException&)
            {

            }
            catch (...)
            {

            }
            mInPipe.close();
            mOutPipe.close();
            delete mpProcessHandle;
            mpProcessHandle = NULL;
        }
    }

    bool RedirectConsole::LaunchConsole(const std::wstring& cmd,const std::vector<std::wstring>& args,const std::wstring& initialDirectory)
    {
        std::string utf8Cmd = LocalToUTF8(cmd);
        std::string utf8InitialDirectory = LocalToUTF8(initialDirectory);
        std::vector<std::string> utf8Args;
        for (size_t index = 0;index < args.size();index++)
        {
            utf8Args.push_back(LocalToUTF8(args[index]));
        }

        return LaunchConsole(utf8Cmd,utf8Args,utf8InitialDirectory);
    }

    bool RedirectConsole::LaunchConsole(const std::string& cmd,const std::vector<std::string>& args,const std::string& initialDirectory)
    {
        KillConsole();
        Poco::Pipe inPipe;
        Poco::Pipe outPipe;
        Poco::ProcessHandle ph = Poco::Process::launch(cmd, args,initialDirectory, &inPipe, &outPipe, &mOutPipe);
        mInPipe = inPipe;
        mOutPipe = outPipe;
        mpProcessHandle = new Poco::ProcessHandle(ph);
        mhProcess = OpenProcess(PROCESS_TERMINATE, FALSE, mpProcessHandle->id());
        return mpProcessHandle != NULL && mpProcessHandle->id() > 0 && mhProcess != NULL;
    }

    bool RedirectConsole::ConsoleisRunning()
    {
		if (NULL == mpProcessHandle || NULL == mhProcess)
		{
			return false;
		}
        return Poco::Process::isRunning(*mpProcessHandle);
    }

    int RedirectConsole::WaitConsole()
    {
        if (!ConsoleisRunning())
        {
            return -1;
        }
        return mpProcessHandle->wait();
    }

    int RedirectConsole::RedirectIn(const std::string& in)
    {
        if (!ConsoleisRunning())
        {
            return -1;
        }
        std::string str (in);
        if (str.empty() || '\n' != str[str.length() - 1]) str.append("\n");

        return mInPipe.writeBytes(str.c_str(),str.length());
    }

    int RedirectConsole::RedirectOut(std::string& out)
    {
        if (!ConsoleisRunning())
        {
            return -1;
        }

        DWORD bytesRead = 0;
        if (FALSE == PeekNamedPipe(mOutPipe.readHandle(), NULL, 0, NULL, &bytesRead, NULL))
        {
            return -1;
        }
        if (bytesRead > 0)
        {
            out.resize(bytesRead);
            return mOutPipe.readBytes(const_cast<char*>(out.data()),bytesRead);
        }
        return 0;
    }

    int RedirectConsole::RedirectOutTimeOut(std::string& out,int timeOutMilliseconds /* = 1000 */)
    {
        if (!ConsoleisRunning())
        {
            return -1;
        }

        for (int index = 0;index < timeOutMilliseconds/100 + 1;index++)
        {
            DWORD bytesRead = 0;
            if (FALSE == PeekNamedPipe(mOutPipe.readHandle(), NULL, 0, NULL, &bytesRead, NULL))
            {
                return -1;
            }
            if (bytesRead > 0)
            {
                out.resize(bytesRead);
                return mOutPipe.readBytes(const_cast<char*>(out.data()),bytesRead);
            }
            Sleep(100);
        }
        return 0;
#if 0
        if (WAIT_OBJECT_0 == WaitForSingleObject(mOutPipe.readHandle(), timeOutMilliseconds))
        {
            DWORD bytesRead = 0;
            if (FALSE == PeekNamedPipe(mOutPipe.readHandle(), NULL, 0, NULL, &bytesRead, NULL))
            {
                return -1;
            }
            if (bytesRead > 0)
            {
                out.resize(bytesRead);
                return mOutPipe.readBytes(const_cast<char*>(out.data()),bytesRead);
            }
        }
#endif
        return 0;
    }
}

