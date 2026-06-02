#include "StdAfx.h"
#include "SendPipeCommand.h"
#include <Poco/Stopwatch.h>
#include <poco/String.h>
namespace JGW
{
	SendPipeCommand::SendPipeCommand(void)
	{
		mvEofChars.push_back('>');
		mvEofChars.push_back('#');
		mvEofChars.push_back('$');
	}


	SendPipeCommand::~SendPipeCommand(void)
	{
	}

	void SendPipeCommand::SetLaunchRedirectConsoleParam(const std::string& cmd,const std::vector<std::string>& args,const std::string& initialDirectory)
	{
		mCmd = cmd;
		mArgs.clear();
		for (size_t index = 0;index < args.size();index++)
		{
			mArgs.push_back(args[index]);
		}
		mInitialDirectory = initialDirectory;
	}

	bool SendPipeCommand::LaunchRedirectConsole(const std::string& cmd,const std::vector<std::string>& args,const std::string& initialDirectory)
	{
		SetLaunchRedirectConsoleParam(mCmd,mArgs,mInitialDirectory);
		return mRedirectConsole.LaunchConsole(cmd,args,initialDirectory);
	}

	bool SendPipeCommand::LaunchRedirectConsole(const std::wstring& cmd,const std::vector<std::wstring>& args,const std::wstring& initialDirectory)
	{
		std::string utf8Cmd = LocalToUTF8(cmd);
		std::string utf8InitialDirectory = LocalToUTF8(initialDirectory);
		std::vector<std::string> utf8Args;
		for (size_t index = 0;index < args.size();index++)
		{
			utf8Args.push_back(LocalToUTF8(args[index]));
		}
		return mRedirectConsole.LaunchConsole(utf8Cmd,utf8Args,utf8InitialDirectory);
	}

	bool SendPipeCommand::ReLaunchRedirectConsole()
	{
		try
		{
			if (LaunchRedirectConsole(mCmd,mArgs,mInitialDirectory))
			{
				std::wstring recv;
				ReadConsoleProcessPipeEOF(recv,5 * 1000);
				return true;
			}
		}
		catch (Poco::Exception& e)
		{
			gLog.get()->Error(e.message().c_str());
		}
		return false;
	}

	void SendPipeCommand::KillConsole()
	{
		mRedirectConsole.KillConsole();
	}

	bool SendPipeCommand::ConsoleisRunning()
	{
		return mRedirectConsole.ConsoleisRunning();
	}

	bool SendPipeCommand::SendConsoleCommand(const S_CONSOLE_COMMAND& sConsoleCommand,std::wstring& strRecv)
	{
		int count = 0;
		Poco::Stopwatch sw;
		sw.start();

		while (sw.elapsed() / 1000 <= sConsoleCommand.mnTimeOutMilliseconds && count++ < sConsoleCommand.mnSendCount)
		{
			if (!mRedirectConsole.ConsoleisRunning())
			{
				ReLaunchRedirectConsole();
			}
			if (mRedirectConsole.RedirectIn(W2A(sConsoleCommand.mstrCommand)) < 1)
			{
				Sleep(sConsoleCommand.mnErrorTestIntervalMilliseconds);
				continue;
			}

			if (!ReadConsoleProcessPipeEOF(strRecv,sConsoleCommand.mnTimeOutMilliseconds/* / sConsoleCommand.mnSendCount*/))
			{
				Sleep(sConsoleCommand.mnErrorTestIntervalMilliseconds);
				continue;
			}
			if (!strRecv.empty() && 0 == strRecv.find(sConsoleCommand.mstrCommand))
			{
				strRecv.erase(0,sConsoleCommand.mstrCommand.length());
				EraseLastAndFristTwoChars(strRecv);
			}
			//std::wcout << strRecv << std::endl;
			if (std::wstring::npos != StrCaseStr(strRecv,sConsoleCommand.mstrFindRecv))
			{
				return true;
			}
		}

		return false;
	}

	bool SendPipeCommand::ReadConsoleProcessPipeEOF(std::wstring& strRecv,int nTimeOutMilliseconds /* = 3 * 1000 */)
	{
		std::string strRecvA;
		Poco::Stopwatch sw;
		std::vector<char> vRemoveChars;
		int totalCount = 0;

		sw.start();
		vRemoveChars.push_back('\r');
		vRemoveChars.push_back('\n');
		vRemoveChars.push_back(' ');
		vRemoveChars.push_back('\t');

		while (sw.elapsed() / 1000 <= nTimeOutMilliseconds)
		{
			//std::cout << sw.elapsed() << std::endl;
			std::string temp;
			int count = mRedirectConsole.RedirectOut(temp);
			if (-1 == count)
			{
				strRecv = A2W(strRecvA);
				mRedirectConsole.KillConsole();
				return false;
			}
			else if (count > 0)
			{
				totalCount += count;
				strRecvA += temp;
				std::string result(strRecvA);
				EraseChars(result,vRemoveChars);

				if (!strRecvA.empty() && strRecvA.at(strRecvA.length() - 1) == _T('>'))
				{
					strRecvA.erase(0,strRecvA.find('\n') + 1);
					strRecvA.erase(strRecvA.find_last_of('\n') + 1);
				}
				if (result.length() > 1 && IsFindCharVector(result[result.length() - 1],mvEofChars))
				{
					EraseChars(strRecvA,vRemoveChars);
					break;
				}
			}
			Sleep(100);
			//std::cout << sw.elapsed() << std::endl;
		}
		strRecv = A2W(strRecvA);
		//if (0 == totalCount) mRedirectConsole.KillConsole();
		return totalCount > 0;
	}

	bool SendPipeCommand::SendAdbShellCommand(const S_CONSOLE_COMMAND& sConsoleCommand,std::wstring& strRecv)
	{
		int count = 0;
		Poco::Stopwatch sw;
		sw.start();

		while (sw.elapsed() / 1000 <= sConsoleCommand.mnTimeOutMilliseconds && count++ < sConsoleCommand.mnSendCount && mRedirectConsole.ConsoleisRunning())
		{
			if (!sConsoleCommand.mstrCommand.empty())
			{
				if (mRedirectConsole.RedirectIn(W2A(sConsoleCommand.mstrCommand)) < 1)
				{
					Sleep(sConsoleCommand.mnErrorTestIntervalMilliseconds);
					continue;
				}
			}

			if (!ReadAdbShellPipe(sConsoleCommand.mstrFindRecv,sConsoleCommand.mstrCommand,strRecv,sConsoleCommand.mnTimeOutMilliseconds/* / sConsoleCommand.mnSendCount*/))
			{
				Sleep(sConsoleCommand.mnErrorTestIntervalMilliseconds);
				continue;
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	bool SendPipeCommand::ReadAdbShellPipe(const std::wstring& strFind,const std::wstring& strCommand,std::wstring& strRecv,int nTimeOutMilliseconds /* = 3 * 1000 */)
	{
		std::string strRecvA;
		Poco::Stopwatch sw;
		std::vector<char> vRemoveChars;
		int totalCount = 0;
		bool bResult = false;

		sw.start();
		vRemoveChars.push_back('\r');
		vRemoveChars.push_back('\n');
		vRemoveChars.push_back(' ');
		vRemoveChars.push_back('\t');

		while (sw.elapsed() / 1000 <= nTimeOutMilliseconds)
		{
			//std::cout << sw.elapsed() << std::endl;
			std::string temp;
			int count = mRedirectConsole.RedirectOut(temp);
			if (-1 == count)
			{
				strRecv = A2W(strRecvA);
				mRedirectConsole.KillConsole();
				return false;
			}
			else if (count > 0)
			{
				totalCount += count;
				strRecvA += temp;
				std::string result(strRecvA);
				EraseChars(result,vRemoveChars);
#if 0
				if (!strRecvA.empty() && strRecvA.at(strRecvA.length() - 1) == _T('>'))
				{
					strRecvA.erase(0,strRecvA.find('\n') + 1);
					strRecvA.erase(strRecvA.find_last_of('\n') + 1);
				}
#endif
				if (strFind.empty())
				{
					if (result.length() > 1 && IsFindCharVector(result[result.length() - 1],mvEofChars))
					{
						EraseChars(strRecvA,vRemoveChars);
						bResult = true;
						break;
					}
				}
				else
				{
					if (!strRecvA.empty() && 0 == strRecvA.find(W2A(strCommand)))
					{
						strRecvA.erase(0,strCommand.length());
						EraseLastAndFristTwoChars(strRecv);
					}

					if (std::string::npos != StrCaseStr(result,W2A(strFind)))
					{
						EraseChars(strRecvA,vRemoveChars);
						bResult = true;
						break;
					}
				}
			}
			Sleep(100);
			//std::cout << sw.elapsed() << std::endl;
		}
		strRecv = A2W(strRecvA);
		if (0 == totalCount) mRedirectConsole.KillConsole();
		return bResult;
	}
}
