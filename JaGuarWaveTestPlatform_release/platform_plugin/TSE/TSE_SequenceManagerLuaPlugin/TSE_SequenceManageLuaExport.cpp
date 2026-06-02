#include "StdAfx.h"
#include "TSE_SequenceManageLuaExport.h"
#include "TSE_LuaSequenceManage.h"
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
	CCJGW_CriticalSectionLock gCriticalSectionLock;
	CTSE_SequenceManageLuaExport::CTSE_SequenceManageLuaExport(void)
	{
	}


	CTSE_SequenceManageLuaExport::~CTSE_SequenceManageLuaExport(void)
	{
	}

	CTSE_SequenceManageImpl* CTSE_SequenceManageLuaExport::GetSequenceManageImpl()
	{
		CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
		unsigned long token_id = mmapSequenceManage.size();
		CTSE_SequenceManageImpl* pSequenceManageImpl = new CTSE_LuaSequenceManage(token_id);
		std::shared_ptr<CTSE_SequenceManageImpl> pShared(pSequenceManageImpl);
		mmapSequenceManage[token_id] = pShared;
		return pShared.get();
	}

	CTSE_SequenceManageImpl* CTSE_SequenceManageLuaExport::GetSequenceManageImplToTokenID(unsigned long tokenID)
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

		CTSE_SequenceManageImpl* pSequenceManageImpl = new CTSE_LuaSequenceManage(tokenID);
		std::shared_ptr<CTSE_SequenceManageImpl> pShared(pSequenceManageImpl);
		mmapSequenceManage[tokenID] = pShared;
		return pShared.get();
#endif
	}

	void CTSE_SequenceManageLuaExport::ReleaseSequenceManageImpl(CTSE_SequenceManageImpl* pSequenceManageImpl)
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
	}
}

