#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/ctrim.h>

#define TESTFAIL(x) \
    if( !(x) ) return false;

#define TESTFAILHADNLE(x) \
    if( !(x) || INVALID_HANDLE_VALUE == (x) ) return false;

#define JGW_CLOSEHANDLE(hHandle)\
    if( hHandle && hHandle != INVALID_HANDLE_VALUE ){\
    CloseHandle(hHandle);\
    hHandle = NULL;\
    }

namespace JGW
{
	std::wstring CCJGW_PeekNamePide::m_strCmdPath = JGW_GetExecConfigFilePath(L"cmd.exe");

	CCJGW_PeekNamePide::CCJGW_PeekNamePide(void):m_hWrite(NULL)
		,m_hRead(NULL)
		,m_hProcess(NULL)
		,m_nReadTimeout(30)
	{
	}

	CCJGW_PeekNamePide::~CCJGW_PeekNamePide(void)
	{
		ClosePideHandle();
	}

	VOID CCJGW_PeekNamePide::CloseCmdProcess()
	{
		if (m_hProcess)
		{
            char exitBuf[3] = {26,99,'\n'};
            DWORD dwNumberOfBytesWrite = 3;
            WriteFile(m_hWrite,exitBuf,3,&dwNumberOfBytesWrite,NULL);
			WritePideFile(_T("exit\n"),_tcslen(_T("exit\n")));
			TerminateProcess(m_hProcess,0);
			m_hProcess = NULL;
		}
	}

    bool CCJGW_PeekNamePide::SendDosCmds(const std::wstring& strProcess, const std::wstring& strCmd, int nTimeOut /* = 3 */ )
    {
        ClosePideHandle();
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        //创建命名管道
        if ( !CreatePipe( &m_hRead, &m_hWrite, &sa, 0 ) ) 
        {
            Log4WE_F( L"JGW_PeekNamePide::SendDosCmds CreatePipe(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
            goto TESTFAIL;
        }

        GetStartupInfo(&si);
        si.hStdError   = m_hWrite;//数据输出用的文件句柄
        si.hStdOutput  = m_hWrite;//数据输出用的文件句柄
        si.wShowWindow = SW_HIDE;
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES | CREATE_NEW_CONSOLE;

        if ( !CreateProcess(strProcess.c_str(),const_cast<wchar_t*>(strCmd.c_str())//执行cmd命令,并在命名中管道中写入cmd命令返回的串
            ,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi ) ) 
        {
            Log4WE_F( L"JGW_PeekNamePide::SendDosCmds CreateProcess(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
            goto TESTFAIL;
        }
        Log4WD_F(L"JGW_PeekNamePide::SendDosCmds(path:%s):command:%s",m_strCmdPath.c_str(), strCmd.c_str() );
        //不再需要在读写管道的时候已经等待，更安全以及不会超时
        if ( WaitForSingleObject(pi.hProcess, nTimeOut*1000 )==WAIT_FAILED )//这里决定是否要等整个命令执行完,如ping等耗时较多的命令.
        {
            Log4WE_F( L"JGW_PeekNamePide::SendDosCmds WaitForSingleObject(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
            goto TESTFAIL;
        }
        // 		JGW_CLOSEHANDLE( m_hRead );
        // 		JGW_CLOSEHANDLE( m_hWrite );
        return true;
TESTFAIL:
        return ClosePideHandle();
    }

	bool CCJGW_PeekNamePide::SendDosCmds(std::wstring& strCmd, int nTimeOut /* = 3 */ )
	{	
		ClosePideHandle();
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		STARTUPINFO si = {0};
		PROCESS_INFORMATION pi = {0};
		//创建命名管道
		if ( !CreatePipe( &m_hRead, &m_hWrite, &sa, 0 ) ) 
		{
			Log4WE_F( L"JGW_PeekNamePide::SendDosCmds CreatePipe(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
			goto TESTFAIL;
		}

		GetStartupInfo(&si);
		si.hStdError   = m_hWrite;//数据输出用的文件句柄
		si.hStdOutput  = m_hWrite;//数据输出用的文件句柄
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES | CREATE_NEW_CONSOLE;

		if ( !CreateProcess( m_strCmdPath.c_str(),const_cast<wchar_t*>(strCmd.c_str())//执行cmd命令,并在命名中管道中写入cmd命令返回的串
			,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi ) ) 
		{
			Log4WE_F( L"JGW_PeekNamePide::SendDosCmds CreateProcess(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
			goto TESTFAIL;
		}
		Log4WD_F(L"JGW_PeekNamePide::SendDosCmds(path:%s):command:%s",m_strCmdPath.c_str(), strCmd.c_str() );
		//不再需要在读写管道的时候已经等待，更安全以及不会超时
		if ( WaitForSingleObject(pi.hProcess, nTimeOut*1000 )==WAIT_FAILED )//这里决定是否要等整个命令执行完,如ping等耗时较多的命令.
		{
			Log4WE_F( L"JGW_PeekNamePide::SendDosCmds WaitForSingleObject(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
			goto TESTFAIL;
		}
// 		JGW_CLOSEHANDLE( m_hRead );
// 		JGW_CLOSEHANDLE( m_hWrite );
		return true;
TESTFAIL:
		return ClosePideHandle();
	}

	bool CCJGW_PeekNamePide::ReadDosCmd(std::wstring &strRead)
	{
		TESTFAILHADNLE(m_hRead);
		char szBuffer[4096] = {0};
		DWORD bytesRead = 0;

		if ( NULL != ReadFile( m_hRead, szBuffer, sizeof(szBuffer) - 1, &bytesRead, NULL ) )//从命名管道中读取数据
		{
			szBuffer[bytesRead] = '\0';
			strRead += a2w(szBuffer);
			Log4WD_F( L"JGW_PeekNamePide::ReadDosCmd:%s",strRead.c_str() );
		}
		ClosePideHandle();
		return true;
	}

	int CCJGW_PeekNamePide::recv(std::string& strread)
	{
		TESTFAILHADNLE( m_hRead );
		DWORD	bytesRead = 0;
		strread.resize(0);

		if ( FALSE == PeekNamedPipe( m_hRead, NULL, 0, NULL, &bytesRead, NULL ) )
		{
			Log4WE_F( L"JGW_PeekNamePide::recv PeekNamedPipe(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
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

	bool CCJGW_PeekNamePide::ReadPideFile(std::wstring& strRead)
	{
		TESTFAILHADNLE( m_hRead );
		char	szBuffer[MAX_PATH] = {0};
		DWORD	bytesRead = 0;
		int		nReadCount = 0;
		strRead	= _T("");

		while( nReadCount++ < m_nReadTimeout )
		{
			if ( !::PeekNamedPipe( m_hRead, NULL, 0, NULL, &bytesRead, NULL ) )
			{
				Log4WE(L"JGW_PeekNamePide::ReadPideFileIsTimeOut PeekNamedPipe fail");
				break;
			}
			if ( bytesRead > 0 )
			{
				if ( !ReadFile( m_hRead, szBuffer, sizeof(szBuffer) - 1, &bytesRead, NULL ) != NULL )//从命名管道中读取数据
				{
					Log4WE_F( L"JGW_PeekNamePide::ReadPideFile ReadFile(ERROR:%s)",JGW_GetSystemErrorString( GetLastError() ).c_str() );
					break;
				}
				szBuffer[bytesRead] = '\0';
				strRead += a2w(szBuffer);
				Log4WD_F( L"JGW_PeekNamePide::ReadPideFile %s",strRead.c_str() );

				trim::ctrim(strRead);
				if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
				{
					return true;
				}
			}
			Sleep(200);
		}
		ClosePideHandle();
		InitPideCMD();
		return false;
	}

	bool CCJGW_PeekNamePide::WritePideFile(LPCTSTR strWrite,size_t nSize)
	{
		TESTFAILHADNLE( m_hWrite );
		TESTFAIL( nSize );
		std::wstring strTemp(strWrite);

		if ('\n' != strWrite[nSize - 1])
		{
			strTemp += _T("\n");
		}

		std::string strWriteBuffer = w2a(strTemp);
		DWORD dwNumberOfBytesWrite = 0;

		Log4WD_F( L"JGW_PeekNamePide::WritePideFile:%s",strTemp.c_str() );
		if ( !WriteFile(
			m_hWrite,				 // handle of the write end of our pipe
			strWriteBuffer.c_str(),               // address of buffer that send data
			strWriteBuffer.length(),                   // number of bytes to write
			&dwNumberOfBytesWrite,  // address of number of bytes read
			NULL                   // non-overlapped.
			) && !dwNumberOfBytesWrite)
		{
			Log4WE_F( L"JGW_PeekNamePide::WritePideFile fail(err:%s)",JGW_GetSystemErrorString(GetLastError()));
			return false;
		}

		return true;
	}

	bool CCJGW_PeekNamePide::ClosePideHandle()
	{
		Log4WD( L"JGW_PeekNamePide::ClosePideHandle");
		CloseCmdProcess();
		JGW_CLOSEHANDLE( m_hRead );
		JGW_CLOSEHANDLE( m_hWrite );
		return false;
	}

	bool CCJGW_PeekNamePide::CheckProcessIsRun()
	{
		if( NULL == m_hProcess ) 
			return false;

		DWORD dwExitCode = 0;
		GetExitCodeProcess(m_hProcess,&dwExitCode);

		return ( STILL_ACTIVE == dwExitCode );
	}

	bool CCJGW_PeekNamePide::InitPideCMD ( const wchar_t* pstrCurrentDirectory /* = NULL */ )
	{	
		SECURITY_ATTRIBUTES		sa          = {0};
		STARTUPINFOW			si          = {0};
		PROCESS_INFORMATION		pi          = {0};
		HANDLE					hPipeOutputWrite = NULL;
		HANDLE					hPipeInputRead   = NULL;

		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

		// Create pipe for standard output redirection.
		if( !CreatePipe(&m_hRead,  // read handle
			&hPipeOutputWrite, // write handle
			&sa,      // security attributes
			0      // number of bytes reserved for pipe - 0 default
			) )
		{
			Log4WE_F( L"JGW_PeekNamePide::InitPideCMD  CreatePipe fail(err:%s)",JGW_GetSystemErrorString(GetLastError()));
			return false;
		}

		// Create pipe for standard input redirection.
		if( !CreatePipe(&hPipeInputRead,  // read handle
			&m_hWrite, // write handle
			&sa,      // security attributes
			0      // number of bytes reserved for pipe - 0 default
			))
		{
			Log4WE_F( L"JGW_PeekNamePide::InitPideCMD  CreatePipe fail(err:%s)",JGW_GetSystemErrorString(GetLastError()));
			CloseHandle(m_hRead);
			m_hRead = NULL;
			CloseHandle(hPipeOutputWrite);
			return false;
		}

		// Make child process use hPipeOutputWrite as standard out,
		// and make sure it does not show on screen.
		si.cb = sizeof(si);
		si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.hStdInput   = hPipeInputRead;
		si.hStdOutput  = hPipeOutputWrite;
		si.hStdError   = hPipeOutputWrite;

		Log4WD_F(L"JGW_PeekNamePide::InitPideCMD  CreateProcess Command:%s",m_strCmdPath.c_str());
		if ( !CreateProcess (
			NULL, /*"cmd.exe"*/const_cast<wchar_t*>(m_strCmdPath.c_str()),
			NULL, NULL,
			TRUE, 0,
			NULL, pstrCurrentDirectory,
			&si, &pi))
		{
			Log4WE_F( L"JGW_PeekNamePide::InitPideCMD  CreateProcess fail(err:%s)",JGW_GetSystemErrorString(GetLastError()));
			goto TESTFAIL;
		}

		m_hProcess = pi.hProcess;
		CloseHandle(hPipeOutputWrite);
		CloseHandle(hPipeInputRead);
		hPipeOutputWrite = NULL;
		hPipeInputRead = NULL;

		if ( NULL == pstrCurrentDirectory )
			if( !SwitchCmdCurrentDir())
				goto TESTFAIL;

		return true; 


TESTFAIL:
		return ClosePideHandle();
	}

	bool CCJGW_PeekNamePide::WrPideFile(std::wstring strWrite,LPCTSTR strComp /* = NULL */,int nCount/* = 5*/)
	{
		std::wstring strRead(_T(""));
		return WRPideFile(strWrite.c_str(),strWrite.length(),strRead,strComp,nCount);
	}

	bool CCJGW_PeekNamePide::WrPideFile(std::wstring strWrite,std::wstring &strRead,LPCTSTR strComp /* = NULL */,int nCount /* = 5 */)
	{
		return WRPideFile(strWrite.c_str(),strWrite.length(),strRead,strComp,nCount);
	}

	bool CCJGW_PeekNamePide::WRPideFile(LPCTSTR strWrite,size_t nSize,std::wstring &strRead,LPCTSTR strComp/* = NULL*/,int nCount/* = 5*/)
	{
		TESTFAILHADNLE(m_hWrite);
		TESTFAILHADNLE(m_hRead);
		BOOL bResult = 0;
		int nIndex = 0;

		for (nIndex = 0;nIndex < nCount;nIndex ++)
		{
			strRead = _T("");
			if( WritePideFile(strWrite,nSize) )
			{
				Sleep( 50 );
				if( ReadPideFile(strRead) && !strComp)
				{
					return true;
				}
			}
			JGW_MakeWstringToLower(strRead);
			if (strComp)
			{
				if (-1 != strRead.find(strComp))
				{
					return true;
				}
			}

			Sleep(1000);
		}

		return false;
	}

	bool CCJGW_PeekNamePide::AdbGetDeviceState(int nTimeOuts /* = 20 */)
	{
		std::wstring strOut = _T("");
		int iTemp = m_nReadTimeout;
		m_nReadTimeout = 1;

		for (int i = 0;i < nTimeOuts;i ++)
		{	
			if( SendDosCmds( std::wstring(_T("adb get-state")) ))
			{
				ReadPideFile(strOut);

				if (-1 != strOut.find(_T("device")))
				{
					m_nReadTimeout = iTemp;
					return true;
				}
				Sleep(1000);
			}
		}
		m_nReadTimeout = iTemp;

		return false;
	}
	
	bool CCJGW_PeekNamePide::SwitchCmdCurrentDir()
	{
		std::wstring strCmdDir(m_strCmdPath);
		std::wstring strTempCmd(_T(""));

		// 	if (0 != strCmdDir.CompareNoCase(_T("cmd.exe")))
		// 	{
		// 		return TRUE;
		// 	}

		strCmdDir = strCmdDir.substr(0,strCmdDir.rfind(_T('\\')) + 1);
		FormatWString(strTempCmd,_T("%c:\n"),strCmdDir.at(0));

		while ('\\' == strCmdDir.at(strCmdDir.length() - 1))
		{
			strCmdDir.erase(strCmdDir.length() - 1);
		}

		if ( !WritePideFile(strTempCmd.c_str(),strTempCmd.length()) )
		{
			return false;
		}

		strTempCmd = _T("cd \"");
		strTempCmd += strCmdDir;
		strTempCmd += _T("\"\n");

		if ( !WritePideFile(strTempCmd.c_str(),strTempCmd.length()))
		{
			return false;
		}

		if ( !ReadPideFile(strTempCmd) )
		{
			return false;
		}
		JGW_MakeWstringToLower(strCmdDir);
		JGW_ReplaceStringW(strCmdDir,_T("\\\\"),_T("\\"));

		if ( !WrPideFile(_T("\n"),strCmdDir.c_str()))
		{
			return false;
		}
		WritePideFile(_T("\n"),1);

		return true;
	}

	bool CCJGW_PeekNamePide::ReadPideFileIsTimeOut(std::wstring &strRead)
	{
		TESTFAILHADNLE(m_hRead);
		char	szBuffer[MAX_PATH] = {0};
		DWORD	bytesRead = 0;
		int		nReadCount = 0;
		strRead = _T("");

		while( nReadCount++ < m_nReadTimeout )
		{
			if ( !::PeekNamedPipe( m_hRead, NULL, 0, NULL, &bytesRead, NULL ) )
			{
				Log4WE(L"JGW_PeekNamePide::ReadPideFileIsTimeOut PeekNamedPipe fail");
				break;
			}

			if ( bytesRead > 0 )
			{
				if ( !ReadFile( m_hRead, szBuffer, sizeof(szBuffer) - 1, &bytesRead, NULL ) )//从命名管道中读取数据
				{
					Log4WD_F(L"JGW_PeekNamePide::ReadPideFileIsTimeOut %s",strRead.c_str());
					break;
					
				}
				szBuffer[bytesRead] = '\0';
				strRead += a2w(szBuffer);

				trim::ctrim(strRead);
				if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
				{
					Log4WD_F(L"JGW_PeekNamePide::ReadPideFileIsTimeOut %s",strRead.c_str());
					return false;
				}
			}
			Sleep(200);
		}

		return true;
	}

}