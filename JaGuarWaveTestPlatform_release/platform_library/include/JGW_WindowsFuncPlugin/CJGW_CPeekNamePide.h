#pragma once
#include <string>
#include "JGW_WindowsFuncPlugin_Define.h"
namespace JGW
{
	class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_PeekNamePide
	{
	public:
		CCJGW_PeekNamePide(void);
		~CCJGW_PeekNamePide(void);

		bool CheckProcessIsRun();
		//! 设置管道cmd.exe路径（可以不调用）
		static void	 SetInitPideCMDExePath(std::wstring& strCmdPath){ m_strCmdPath = strCmdPath;}
		//! 设置超时时间  s为单位
		void	SetTimeoutSecond(int nReadTimeout = 6);
		//! 初始化管道命令 pstrCurrentDirectory 当前命令运行路径 一般都设置为fh_loader.exe 的目录地址
		//! 比如pstrCurrentDirectory="c:/123/subfolder/"
		bool	InitPideCMD( const wchar_t* pstrCurrentDirectory = NULL );
		//! 关闭管道
		bool	ClosePideHandle();

		//! 发送单条CMD命令  超时时间S
		bool	SendDosCmds(std::wstring& strCmd, int nTimeOut = 3 );//只能读
        //!
        bool	SendDosCmds(const std::wstring& strProcess, const std::wstring& strCmd, int nTimeOut = 3 );//只能读
		//! 读取发送的单条CMD命令  读取完成后自动关闭
		bool	ReadDosCmd(std::wstring& strRead);

		//! 读取管道文件信息
		bool	ReadPideFile(std::wstring& strRead);
		//! 接收管道消息
		int		recv(std::string& strread);
		//! 写入管道信息
		bool	WritePideFile(LPCTSTR strWrite,size_t nSize);
		//! 读取后是否在进行比较
		bool	WRPideFile(LPCTSTR strWrite,size_t nSize,std::wstring& strRead,LPCTSTR strComp = NULL,int nCount = 5);
		bool	WrPideFile(std::wstring strWrite,LPCTSTR strComp = NULL,int nCount = 3);
		bool	WrPideFile(std::wstring strWrite,std::wstring &strRead,LPCTSTR strComp = NULL,int nCount = 3);

		//! 读取管道超时时间
		bool	ReadPideFileIsTimeOut(std::wstring &strRead);
		bool	SwitchCmdCurrentDir();

		bool	AdbGetDeviceState(int nTimeOuts = 20 );//获取设备状态
	private:
		//! Kill掉CMD进程
		VOID	CloseCmdProcess();

		HANDLE m_hRead;
		HANDLE m_hWrite;
		HANDLE m_hProcess;
		int	   m_nReadTimeout;
		static std::wstring m_strCmdPath;
	};

	inline void CCJGW_PeekNamePide::SetTimeoutSecond(int nReadTimeout /* = 6 */)
	{
		m_nReadTimeout = nReadTimeout * 5;
	}

}