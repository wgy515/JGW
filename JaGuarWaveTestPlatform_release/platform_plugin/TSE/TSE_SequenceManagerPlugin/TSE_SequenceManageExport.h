#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <map>

namespace JGW
{
    class CTSE_SequenceManageExport : public CTSE_SequenceManageInterface
    {
    public:
         virtual CTSE_SequenceManageImpl* GetSequenceManageImpl();

         virtual CTSE_SequenceManageImpl* GetSequenceManageImplToTokenID(unsigned long tokenID);
         void ReleaseSequenceManageImpl(CTSE_SequenceManageImpl* pSequenceManageImpl);
    private:
        std::map<unsigned long,std::shared_ptr<CTSE_SequenceManageImpl>> mmapSequenceManage;
    };
}