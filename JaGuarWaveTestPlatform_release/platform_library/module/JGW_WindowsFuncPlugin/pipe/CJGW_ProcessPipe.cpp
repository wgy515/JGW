#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/process/detail/pipe.hpp>
#include <boost/timer.hpp>

namespace JGW
{
	CCJGW_ProcessPipe::CCJGW_ProcessPipe(void):m_hRead(INVALID_HANDLE_VALUE)
		,m_hWrite(INVALID_HANDLE_VALUE)
		,m_hProcess(NULL)
	{
	}


	CCJGW_ProcessPipe::~CCJGW_ProcessPipe(void)
	{ 
		CloseProcessPipe();
	}

	void CCJGW_ProcessPipe::CloseProcessPipe()
	{
		if ( INVALID_HANDLE_VALUE != m_hRead )
		{
			boost::process::detail::file_handle hread(m_hRead);
			m_hRead = INVALID_HANDLE_VALUE;
		}
		if( INVALID_HANDLE_VALUE != m_hWrite )
		{
			boost::process::detail::file_handle hwrite(m_hWrite);
			m_hWrite = INVALID_HANDLE_VALUE;
		}
		if( NULL != m_hProcess )
		{
            if (CheckProcessIsRun())
            {
                char exitBuf[3] = {26,99,'\n'};
                DWORD dwNumberOfBytesWrite = 3;
                WriteFile(m_hWrite,exitBuf,3,&dwNumberOfBytesWrite,NULL);
                send(L"exit\n");
            }
			TerminateProcess(m_hProcess,-1);
			m_hProcess = NULL;
		}
	}

	bool CCJGW_ProcessPipe::CheckProcessIsRun()
	{
		if( NULL == m_hProcess ) 
			return false;

		DWORD dwExitCode = 0;
		GetExitCodeProcess(m_hProcess,&dwExitCode);

		return ( STILL_ACTIVE == dwExitCode );
	}

	bool CCJGW_ProcessPipe::InitProcessPipe(LPCTSTR strprocesspath,const std::wstring& strcmdline,const std::wstring& strWorkDir)
	{
		STARTUPINFOW si = {0};
		PROCESS_INFORMATION pi = {0};
		HANDLE hread = INVALID_HANDLE_VALUE;
		HANDLE hwrite = INVALID_HANDLE_VALUE;
		CloseProcessPipe();
		try
		{
			boost::process::detail::pipe readPipe;
			boost::process::detail::pipe writePipe;

			m_hRead = readPipe.rend().get();
			m_hWrite = writePipe.wend().get();
			hread = writePipe.rend().get();
			hwrite = readPipe.wend().get();

			si.hStdInput   = writePipe.rend().get();
			si.hStdOutput  = readPipe.wend().get();
			si.hStdError   = readPipe.wend().get();

			writePipe.rend().release();
			writePipe.wend().release();
			readPipe.rend().release();
			readPipe.wend().release();
		}
		catch (boost::system::system_error& e)
		{
			Log4WE_F(L"JGW_process_pide::InitProcessPipe error:%s",JGW_A2W_A(e.what()).c_str());
			return false;
		}
		boost::process::detail::file_handle hfileread(hread);
		boost::process::detail::file_handle hfilewrite(hwrite);
		si.cb = sizeof(si);
		si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;

		if ( !CreateProcess (
			strprocesspath,
			const_cast<wchar_t*>(strcmdline.c_str()),
			NULL, NULL,
			TRUE, 0,
			NULL,strWorkDir.c_str(),
			&si, &pi) )
		{
			CloseProcessPipe();
			Log4WE_F(L"JGW_process_pide::InitProcessPipe CreateProcess faile(%s)",JGW_GetSystemErrorString(GetLastError()).c_str());
			return false;
		}
		m_hProcess = pi.hProcess;

		return true;
	}

	int CCJGW_ProcessPipe::recv(std::wstring& strReadPipe)
	{
		int		nReadCount = 0;
		std::string strrecv("");

		nReadCount = recv(strrecv);
		if( nReadCount > 0 ) strReadPipe = a2w(strrecv);

		return nReadCount;
	}

	bool CCJGW_ProcessPipe::send(const std::wstring& strWritePipe)
	{
		//std::string strwrite(strWritePipe.begin(),strWritePipe.end());
		return send(w2a(strWritePipe));
	}

	int CCJGW_ProcessPipe::recv(std::string& strread)
	{
		if( INVALID_HANDLE_VALUE == m_hRead  )
		{
			return false;
		}

		DWORD	bytesRead = 0;
		strread.resize(0);

		if (FALSE == PeekNamedPipe(m_hRead, NULL, 0, NULL, &bytesRead, NULL))
		{
			//! THROW_JGW_EXCEPTION(L"PeekNamedPipe",GetLastError());
			return -1;
		}

		if ( bytesRead <= 0 )
		{
			return 0;
		}

		strread.resize(bytesRead);
		if ( ReadFile( m_hRead, const_cast<char*>(strread.data()), bytesRead, &bytesRead, NULL ) )//从命名管道中读取数据
		{
			return bytesRead;
		}

		return 0;
	}

	bool CCJGW_ProcessPipe::send(std::string& strwrite)
	{
		if(strwrite.empty()) return false;
		if('\n' != strwrite.at(strwrite.size() - 1))
		{
			strwrite += "\n";
		}
		DWORD dwNumberOfBytesWrite = 0;

		if ( !WriteFile(
			m_hWrite,				 // handle of the write end of our pipe
			strwrite.c_str(),               // address of buffer that send data
			strwrite.length(),                   // number of bytes to write
			&dwNumberOfBytesWrite,  // address of number of bytes read
			NULL                   // non-overlapped.
			) && !dwNumberOfBytesWrite)
		{
			return false;
		}

		return true;
	}

	bool CCJGW_ProcessPipe::timed_send(const std::wstring& strwrite,int ntimeout)
	{
		if( strwrite.empty() ) return false;
		std::string strw = w2a(strwrite);
		return timed_send(strw);
	}

	int CCJGW_ProcessPipe::timed_recv(std::wstring& strread,int ntimeout)
	{
		std::string strr;
		int nread = timed_recv(strr,ntimeout);

		if( nread > 0 )
		{
			strread = a2w(strr);
		}

		return nread;
	}

	bool CCJGW_ProcessPipe::timed_send(std::string& strwrite,int ntimeout)
	{
		if( strwrite.empty() ) return false;
		boost::timer tim;
		while ( tim.elapsed() <= ntimeout )
		{
			if( send(strwrite) )
			{
				return true;
			}
			Sleep(100);
		}
		return false;
	}

	int CCJGW_ProcessPipe::timed_recv(std::string& strread,int ntimeout)
	{
		boost::timer tim;
		int nread = 0;

		while ( tim.elapsed() <= ntimeout )
		{
			nread = recv(strread);
			if( nread > 0 )
			{
				return nread;
			}
			else if ( -1 == nread )
			{
				return -1;
			}
			Sleep(100);
		}
		return 0;
	}
}