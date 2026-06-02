#include "StdAfx.h"
#include "TSE_SequenceManageExport.h"
#include "TSE_SequenceManage.h"
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
	CCJGW_CriticalSectionLock gCriticalSectionLock;
    CTSE_SequenceManageImpl* CTSE_SequenceManageExport::GetSequenceManageImpl()
    {
		CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
        unsigned long token_id = mmapSequenceManage.size();
        CTSE_SequenceManageImpl* pSequenceManageImpl = new CTSE_SequenceManage(token_id);
        std::shared_ptr<CTSE_SequenceManageImpl> pShared(pSequenceManageImpl);
        mmapSequenceManage[token_id] = pShared;
        return pShared.get();
    }

    CTSE_SequenceManageImpl* CTSE_SequenceManageExport::GetSequenceManageImplToTokenID(unsigned long tokenID)
    {
#if 0
        if (mmapSequenceManage.end() == mmapSequenceManage.find(tokenID)) return NULL;
        return mmapSequenceManage[tokenID].get();
#else
		CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
		if (mmapSequenceManage.end() != mmapSequenceManage.find(tokenID))
		{
			return mmapSequenceManage[tokenID].get();
		}

		CTSE_SequenceManageImpl* pSequenceManageImpl = new CTSE_SequenceManage(tokenID);
		std::shared_ptr<CTSE_SequenceManageImpl> pShared(pSequenceManageImpl);
		mmapSequenceManage[tokenID] = pShared;
		return pShared.get();
#endif
    }

    void CTSE_SequenceManageExport::ReleaseSequenceManageImpl(CTSE_SequenceManageImpl* pSequenceManageImpl)
    {
		CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
        std::map<unsigned long,std::shared_ptr<CTSE_SequenceManageImpl>>::iterator it = mmapSequenceManage.begin();

        while (it != mmapSequenceManage.end())
        {
            if (it->second.get() == pSequenceManageImpl)
            {
                //! pSequenceManageImpl->GetSequenceTestImpl()->UnLoadSequenceTest();
                it = mmapSequenceManage.erase(it);
            }
        }
//         std::map<unsigned long,std::shared_ptr<CTSE_SequenceManageImpl>>::iterator it = mmapSequenceManage.find(tokenID);
//         if (mmapSequenceManage.end() == it) return;
//         mmapSequenceManage.erase(it);
    }
}