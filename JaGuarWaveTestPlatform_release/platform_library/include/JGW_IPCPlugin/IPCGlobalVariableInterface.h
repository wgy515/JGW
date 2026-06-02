#pragma once
#include <string>

#ifdef JGW_IPCPLUGIN_EXPORTS
#define IPCPLUGIN_API __declspec(dllexport)
#else
#define IPCPLUGIN_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"JGW_IPCPlugin_d.lib")
#else
#pragma comment(lib,"JGW_IPCPlugin.lib")
#endif
#endif


namespace JGW
{
    class IPCGlobalVariableInterface
    {
    public:
        /// @brief 获取IPC全局变量值
        /// @param strGlobalVariableName IPC全局变量名称
        /// @return 返回全局变量值
        virtual std::wstring GetGlobalVariableValue(const std::wstring& strGlobalVariableName) = 0;
        /// @brief 设置IPC全局变量KEY-VALUE
        /// @param strGlobalVariableName 全局变量名
        /// @param strGlobalVariableValue 全局变量对应的值
        virtual void PutGlobalVariableNameValue(const std::wstring& strGlobalVariableName,const std::wstring& strGlobalVariableValue) = 0;
        /// @brief 全局变量名是否存在
        /// @param strGlobalVariableName 全局变量名
        /// @return true : 存在 false : 不存在
        virtual bool ExistsGlobalVariableName(const std::wstring& strGlobalVariableName) = 0;
    };
}

template  class __declspec( dllexport ) std::allocator<wchar_t>;
template  class __declspec( dllexport ) std::allocator<int>;
template  class __declspec( dllexport ) std::allocator<float>;
template  class __declspec( dllexport ) std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >; 


#ifdef __cplusplus
extern "C" {
#endif
    /****************************************** 跨进程全局变量名称与值集合 *******************************************************************************/
    IPCPLUGIN_API unsigned int GetIPCGlobalVariableValue(const wchar_t* strGlobalVariableName,wchar_t* strGlobalVariableValue,unsigned int nGlobalVariableValueLen);

    IPCPLUGIN_API void PutIPCGlobalVariableNameValue(const wchar_t* strGlobalVariableName,const wchar_t* strGlobalVariableValue);

    IPCPLUGIN_API bool ExistsIPCGlobalVariableName(const wchar_t* strGlobalVariableName);

    IPCPLUGIN_API JGW::IPCGlobalVariableInterface* GetIPCGlobalVariable();


    /***************************************** 进程与进程之间单工通讯 ************************************************************************************************/
    //! IPC单项通讯
    IPCPLUGIN_API void* CreateIPCSimplex(bool bIsTx,const wchar_t* strIPCSimplexName,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen);
    //! 传输IPC单向通讯消息
    IPCPLUGIN_API void TransmitIPCSimplexMessage(void* pICPSimplexContext,const char* strMessage);
    //! 接收IPC单向通讯消息  [DllImport("MyDLL.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void __GetValue__(StringBuilder str, int strlen);
    IPCPLUGIN_API unsigned int ReceiveIPCSimplexMessage(void* pICPSimplexContext,char* pstrReceiveMessage,unsigned int nReceiveMessageLen);
    //! 
    IPCPLUGIN_API void ReleaseIPCSimplex(void* pICPSimplexContext);


    /********************************************* 只支持进程内Simplex通讯 **************************************************************************************************/
    //! 
    IPCPLUGIN_API void* CreateProcessSimplexCommunication(bool bIsTx,unsigned int nProcessSimplexBufferLen,const wchar_t* strProcessSimplexName,unsigned char* szMessageSeparator,unsigned int nMessageSeparatorLen);
    //! 
    IPCPLUGIN_API void TransmitProcessSimplexMessage(void* pProcessSimplexContext,const char* strMessage);
    //! 
    IPCPLUGIN_API unsigned int ReceiveProcessSimplexMessage(void* pProcessSimplexContext,char* pstrReceiveMessage,unsigned int nReceiveMessageLen);
    //! 
    IPCPLUGIN_API void ReleaseProcessSimplex(void* pProcessSimplexContext);
    //! 
    IPCPLUGIN_API void ReleaseProcessSimplexRingBuffer(const wchar_t* strProcessSimplexName);



    /****************************************** 只进程内全局变量名称与值集合 *******************************************************************************/
    IPCPLUGIN_API unsigned int GetProcessGlobalVariableValue(const wchar_t* strGlobalVariableName,wchar_t* strGlobalVariableValue,unsigned int nGlobalVariableValueLen);

    IPCPLUGIN_API void PutProcessGlobalVariableNameValue(const wchar_t* strGlobalVariableName,const wchar_t* strGlobalVariableValue);

    IPCPLUGIN_API bool ExistsProcessGlobalVariableName(const wchar_t* strGlobalVariableName);

#ifdef __cplusplus
}
#endif

