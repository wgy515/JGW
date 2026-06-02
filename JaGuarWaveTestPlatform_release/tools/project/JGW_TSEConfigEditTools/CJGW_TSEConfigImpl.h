#pragma once
#include <string>
#include "JGW_TSEConfigEditDefine.h"
#include <UiLib/CAsyncMsgDialog.h>

namespace JGW
{
    class CCJGW_TSEConfigImpl
    {
    public:
        virtual bool LoadTSEConfig(const std::string& strTSEConfigPath,S_TSE_CONFIG_INFOS& sTSEConfigInfos) = 0;

        virtual bool SaveTSEConfig(const std::string& strTSEConfigPath,CTreeNodeUI* pHomeTreeNode) = 0;
    };
}
