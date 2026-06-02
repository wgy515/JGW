#pragma once
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>

namespace JGW
{
    class CCJGW_MESRestoreOperateID
    {
    public:
        CCJGW_MESRestoreOperateID(std::string strOperateID)
        {
            mstrOldOperateID = GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId;
            GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId = strOperateID;
        }
        ~CCJGW_MESRestoreOperateID(void)
        {
            GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId = mstrOldOperateID;
        }
    private:
        std::string mstrOldOperateID;
    };
}


