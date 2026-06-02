#ifndef __CMD_HANDLER_H__
#define __CMD_HANDLER_H__
 
#include <Windows.h>
#include <string>
 
/* buffer的最大长度 */
#define PIPE_BUFFER_SIZE 4096
#define PORT  12306
 
/* 命令参数 */
typedef struct _CHCmdParam CHCmdParam;
 
struct _CHCmdParam
{
	/* 初始化为 sizeof(CommandParam) */
	int iSize;
 
	/* 外部命令，不需要用则设置为-1, 提供给外部使用
	*/
	int iCommand;
 
	/* 超时时间，单位秒 */
	long long iTimeOut;
 
	/* 命令行要执行的命令 */
	char* szCommand;
 
	/* 用户数据 */
	void* pUserData;
 
	/* 命令执行后的回调 */
	void(*OnCmdEvent)(const CHCmdParam* pParam, HRESULT hResultCode, char* szResult);
};
 
class CCmdHandler
{
private:
	BOOL m_bInit;
	DWORD m_port;
	STARTUPINFO m_startupInfo;
	PROCESS_INFORMATION m_processInfo;
	SECURITY_ATTRIBUTES m_saOutPipe;
	DWORD m_dwErrorCode;
	HANDLE m_hPipeRead;
	HANDLE m_hPipeWrite;
	CHCmdParam m_CommandParam;
	TCHAR m_szReadBuffer[PIPE_BUFFER_SIZE];
	TCHAR m_szWriteBuffer[PIPE_BUFFER_SIZE];
	char m_szPipeOut[PIPE_BUFFER_SIZE];
	HRESULT ExecuteCmdWait();
public:
	CCmdHandler();
	~CCmdHandler();
	/*
	* 客户端初始化接口，调用其余接口之前调用
	* 成功返回S_OK
	*/
	HRESULT Cli_Initalize();
	/*
	* 服务器初始化接口，调用其余接口之前调用
	* 成功返回S_OK
	*/
	HRESULT Ser_Initalize(DWORD port = PORT);
	/*
	* 结束接口
	*/
	HRESULT Finish();
	/*
	* 执行命令接口，接口调用成功返回S_OK
	* param[in] pCommmandParam: 指向一个CHCmdParam命令参数结构的指针
	*/
	HRESULT HandleCommand(CHCmdParam* pCommmandParam);
	/*
	* 返回错误码，便于差距接口调用失败后产生什么错误
	*/
	DWORD GetErrorCode() { return m_dwErrorCode; }
};
 
#endif // !__CMD_HANDLER_H__
 