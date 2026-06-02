#pragma once
#ifdef JGW_SSHHELP_EXPORTS
#define JGW_SSHHELP_API __declspec(dllexport)
#else
#define JGW_SSHHELP_API __declspec(dllimport)
#pragma comment(lib,"JGW_SSHHelp.lib")
#endif


#ifdef __cplusplus
extern "C" 
{
#endif
    JGW_SSHHELP_API void* GetSSHHlepBase();

    JGW_SSHHELP_API bool ConnectSSHShell(void* pSSHHelpBase,const char* server_name,const char* strUserName,const char* strPassword,const char* source_addr = NULL,unsigned int port = 22);
    //!
    JGW_SSHHELP_API bool IsConnected(void* pSSHHelpBase);
    //!
    JGW_SSHHELP_API void DisConnectSSHShell(void* pSSHHelpBase);
    //!
    JGW_SSHHELP_API int ReadCommandResult(void* pSSHHelpBase,char* read_buf,unsigned int read_buf_size);
    //!
    JGW_SSHHELP_API bool WriteCommand(void* pSSHHelpBase,const char* command,unsigned int len,int time_outSec); 

    JGW_SSHHELP_API void ReleaseSSHHlepBase(void* pSSHHelpBase);

#ifdef __cplusplus
};
#endif