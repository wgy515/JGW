#include "StdAfx.h"
#include "CJGW_PropertyFileConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCJGW_PropertyFileConfig::CCJGW_PropertyFileConfig(const std::wstring& strPropertyFilleConfigName) : mstrPropertyFilleConfigName(strPropertyFilleConfigName),mProperty(strPropertyFilleConfigName,mmapPropertyConfig)
    {

    }

    CCJGW_PropertyFileConfig::~CCJGW_PropertyFileConfig(void)
    {
    }

    void CCJGW_PropertyFileConfig::ReloadPropertyFile()
    {
        mProperty.ReLoadPropertyFilleConfig();
    }

    void CCJGW_PropertyFileConfig::PutString(const std::wstring& itemName,const std::wstring& itemValue)
    {
        //! 写文件的时候先比较数据是否相同，相同的数据不重复写入配置文件
        {
            writeLock lockWrite(mrwMutex); 
            std::unordered_map<std::wstring,std::wstring>::iterator it = mmapPropertyConfig.find(itemName);
            if (mmapPropertyConfig.end() != it && NULL == itemValue.compare(it->second))
            {
                return ;
            }
        }
        mProperty.WritePropertyValueToKey(itemName,itemValue);
        CCJGW_PropertyMemoryConfig::PutString(itemName,itemValue);
    }

    void CCJGW_PropertyFileConfig::SetPropertyAppName(const std::wstring& strPropertyAppName /* = L"propety" */)
    {
        mProperty.SetPropertyAppName(strPropertyAppName);
    }
}