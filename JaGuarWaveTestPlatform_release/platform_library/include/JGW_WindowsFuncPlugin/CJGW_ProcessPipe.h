#pragma once
#include <string>
#include "JGW_WindowsFuncPlugin_Define.h"
namespace JGW
{
	class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_ProcessPipe
	{
	public:
		CCJGW_ProcessPipe(void);
		~CCJGW_ProcessPipe(void);
        //! 初始化进程管道
		bool	InitProcessPipe(LPCTSTR strprocesspath,const std::wstring& strcmdline,const std::wstring& strWorkDir);
        //! 返回-1 表示管道异常
		int		recv(std::wstring& strReadPipe); 
        int		recv(std::string& strread);
        //! 发送命令
		bool	send(const std::wstring& strWritePipe);
        bool	send(std::string& strwrite);
        //! 发送命令 设置超时时间
		bool	timed_send(const std::wstring& strwrite,int ntimeout = 50 );
        bool	timed_send(std::string& strwrite,int ntimeout = 50 );
        //! 接受命令 超时
		int		timed_recv(std::wstring& strread,int ntimeout = 50 );
		int		timed_recv(std::string& strread,int ntimeout = 50 ); 
        //! 关闭进程管道
		void	CloseProcessPipe();
        //! 检查进程是否正在运行
		bool	CheckProcessIsRun(); 
        //! 
		HANDLE	GetWriteHandle(){return m_hWrite;}  
        //! 
		HANDLE	GetReadHandle(){return m_hRead;}
	private: 
		HANDLE	m_hWrite;
		HANDLE	m_hRead;
		HANDLE	m_hProcess;
	};
}
