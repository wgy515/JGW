#pragma once
#include <JGW_IPCPlugin/IPCGlobalVariableInterface.h>
#include <map>
#include <memory>
#include "CJGW_IPCSharedStruct.h"

namespace JGW
{
    /*!
    The max size of individual IPC buffers.  Larger strings can be stored, but they
    will be broken up into smaller buffers each of size MAX_IPC_BUFFER_SIZE or less.
    */
    const int MAX_IPC_BUFFER_SIZE = 1024 / 2;

    /*!
    IPCBuffer

    A structure to wrap an IPC buffer (a byte array of size MAX_IPC_BUFFER_SIZE).
    */
    struct IPCBuffer
    {
        wchar_t buff[MAX_IPC_BUFFER_SIZE];
    };


    class CCJGW_IPCGlobalVariable : public IPCGlobalVariableInterface
    {
    public:
        CCJGW_IPCGlobalVariable(void);
        ~CCJGW_IPCGlobalVariable(void);
    public:
        /// @brief 继承IPCGlobalVariableInterface接口
        /// @param strGlobalVariableName 
        /// @return 
        std::wstring GetGlobalVariableValue(const std::wstring& strGlobalVariableName);
        /// @brief 继承IPCGlobalVariableInterface接口
        /// @param strGlobalVariableName 
        /// @param strGlobalVariableValue 
        void PutGlobalVariableNameValue(const std::wstring& strGlobalVariableName,const std::wstring& strGlobalVariableValue);
        /// @brief 继承IPCGlobalVariableInterface接口
        /// @param strGlobalVariableName 
        /// @return 
        bool ExistsGlobalVariableName(const std::wstring& strGlobalVariableName);
	private:
		std::wstring GetProcessAndThreadId( void ) const;
    public:
        /// 暂时不提供进程同步功能，因为使用频率不高.正常情况下是不会出现冲突问题
        void PutGlobalVariableNameValueSync(const std::wstring& strGlobalVariableName,const std::wstring& strGlobalVariableValue);
    private:
        std::map<std::wstring,std::shared_ptr<CCJGW_IPCSharedStruct<IPCBuffer>>> mmapGlobalVariables;
    };
}


