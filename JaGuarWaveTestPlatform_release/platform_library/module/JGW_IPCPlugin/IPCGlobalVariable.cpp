#include "StdAfx.h"
#include "IPCGlobalVariable.h"
#include <sstream>
//! {F0E2EA84-53CE-456E-BBFA-5DBAD0CA8050} 在文件映射对象名称前加上“Global\”允许进程彼此通信
#define MEM_MAP_FILE_NAME_ROOT L"Local\\F0E2EA84-53CE-456E-BBFA-5DBAD0CA8050-"

namespace JGW
{
    CCJGW_IPCGlobalVariable::CCJGW_IPCGlobalVariable(void)
    {
        //CreateSemaphore
    }


    CCJGW_IPCGlobalVariable::~CCJGW_IPCGlobalVariable(void)
    {
    }

    std::wstring CCJGW_IPCGlobalVariable::GetGlobalVariableValue(const std::wstring& strGlobalVariableName)
    {
        bool done = false;
        unsigned int cnt = 0;
        std::wstring strVaule,strTemp;

        // make it case-insensitive
        std::wstring id = AllCAPSRemoveWhiteSpace(strGlobalVariableName);

        std::map<std::wstring,std::shared_ptr<CCJGW_IPCSharedStruct<IPCBuffer>>>::iterator itr;

        do 
        {
            //
            // generate a unique key for this item by combining the lookup root with the index
            // that we are attempting to fetch.
            std::wostringstream indID;
            indID << id;
            // the first index is written w/o the appended array string identifier
            // other indexes must indicate what element they are
            if( cnt > 0 )
            {
                indID << std::wstring(L"_#IPC_ARR[") << cnt << L"]#_";
            }

            // see if the value is already in our local map
            itr = mmapGlobalVariables.find( indID.str() );
            if( itr != mmapGlobalVariables.end() )
            {
                // make a copy of the value
                strTemp = itr->second->mpDataItem->buff;
                // append it to the return value (large items take multiple reads)
                strVaule += strTemp;
                // we'll consider ourselves done if the last read was less than the max buffer size
                done = strTemp.size() < MAX_IPC_BUFFER_SIZE - 1;
            }
            else
            {
                // generate a global file access key based on the item name
                std::wstring strName = MEM_MAP_FILE_NAME_ROOT + GetProcessAndThreadId() + indID.str();
                HANDLE openFile = OpenFileMapping( FILE_MAP_READ, FALSE, strName.c_str() );
                // See if this item was added by another process
                if( openFile )
                {
                    // declare a new shared struct using the global access key
                    std::shared_ptr<CCJGW_IPCSharedStruct<IPCBuffer>> pItem( new CCJGW_IPCSharedStruct< IPCBuffer >( strName ) );

                    // read the item from global memory
                    strTemp = pItem->mpDataItem->buff;
                    // append it to the return value (large items take multiple reads)
                    strVaule += strTemp;
                    // we'll consider ourselves done if the last read was less than the max buffer size
                    done = strTemp.size() < MAX_IPC_BUFFER_SIZE-1;

                    // add to local map for quick look-up in the future
                    mmapGlobalVariables[indID.str()] = pItem;
                }
                else // non-existent, don't search anymore
                {
                    if( cnt==0 ) // throw an exception if we didn't find this anywhere on the first index
                    {
                        //throw CApplicationException( g_exc_general_configNoGet, string("IPC: ") + itemName, __FILE__, __LINE__);
                    }
                    done = true;
                }
                CloseHandle(openFile);
            }
            ++cnt;

        } while (!done);

        return strVaule;
    }

    void CCJGW_IPCGlobalVariable::PutGlobalVariableNameValue(const std::wstring& strGlobalVariableName,const std::wstring& strGlobalVariableValue)
    {
        std::map<std::wstring,std::shared_ptr<CCJGW_IPCSharedStruct<IPCBuffer>>>::iterator itr;

        // make it case-insensitive
        std::wstring id = AllCAPSRemoveWhiteSpace(strGlobalVariableName);

        unsigned int pos = 0;
        unsigned int cnt = 0;
        std::wstring strTemp;

        //
        // do-while is needed because the value may have to be spread across multiple
        // entries since each entry has a max size limit.
        do
        {
            //
            // generate a unique id for the current write index
            std::wostringstream indID;
            indID << id;

            // the first index is written w/o the appended array string identifier
            // other indexes must indicate what element they are
            if( cnt > 0 )
            {
                indID << std::wstring(L"_#IPC_ARR[") << cnt <<L"]#_";
            }

            // store a temp that meets the min buff size req
            strTemp = strGlobalVariableValue.substr(pos,  MAX_IPC_BUFFER_SIZE-1);

            // see if the value is already in our local map
            itr = mmapGlobalVariables.find( indID.str() );
            if( itr != mmapGlobalVariables.end() )
            {
                // copy the new value into IPC buffer
                //strncpy_s(itr->second->mpDataItem->buff, MAX_IPC_BUFFER_SIZE,(temp + char(0)).c_str(), temp.size()+1 );
                wcsncpy_s(itr->second->mpDataItem->buff,MAX_IPC_BUFFER_SIZE,(strTemp + wchar_t(0)).c_str(), strTemp.size()+1);
            }
            else
            {
                // generate a global file access key based on the item name
                std::wstring strName = MEM_MAP_FILE_NAME_ROOT + GetProcessAndThreadId() + indID.str();

                // declare a new shared struct using the global access key
                std::shared_ptr<CCJGW_IPCSharedStruct<IPCBuffer>> pItem( new CCJGW_IPCSharedStruct< IPCBuffer >( strName ) );

                // copy the new value into IPC buffer
                wcsncpy_s(pItem->mpDataItem->buff, MAX_IPC_BUFFER_SIZE, (strTemp + wchar_t(0)).c_str(), strTemp.size()+1 );

                // add to local map
                mmapGlobalVariables[indID.str()] = pItem;
            }

            // move on to the next chunk for write
            pos += MAX_IPC_BUFFER_SIZE-1;
            ++cnt;
        }
        while( pos < strGlobalVariableValue.size()+1 );    // we're done if we've iterated through the entire input value
    }

    bool CCJGW_IPCGlobalVariable::ExistsGlobalVariableName(const std::wstring& strGlobalVariableName)
    {
        bool retVal = false;;
        // make it case-insensitive
        std::wstring id = AllCAPSRemoveWhiteSpace(strGlobalVariableName);
        // see if the value is already in our local map
        std::map<std::wstring,std::shared_ptr<CCJGW_IPCSharedStruct<IPCBuffer>>>::const_iterator itr = mmapGlobalVariables.find(id);
        if (itr != mmapGlobalVariables.end())
        {
            return true;
        }
        else // otherwise look for it in the file mapping
        {
            // generate a global file access key based on the item name
            std::wstring strName = MEM_MAP_FILE_NAME_ROOT + id;
            // See if this item was added by another process
            HANDLE openFile;
            openFile = OpenFileMapping( FILE_MAP_READ, FALSE, strName.c_str() );
            retVal = openFile != NULL;
            CloseHandle(openFile);
        }

        return retVal;
    }

    void CCJGW_IPCGlobalVariable::PutGlobalVariableNameValueSync(const std::wstring& strGlobalVariableName,const std::wstring& strGlobalVariableValue)
    {
        
    }

	std::wstring CCJGW_IPCGlobalVariable::GetProcessAndThreadId(void ) const
	{
#if 0
		static unsigned int processID = 0;
		static unsigned int threadID = 0;
		static std::wostringstream pid;

		if ( processID == 0 )
		{
			processID = GetCurrentProcessId();
			threadID = GetCurrentThreadId();
			pid.str(L"");
			pid << processID << L"_" << threadID << L"-";
		}

		return pid.str();
#else
		return L"";
#endif
	}
}
