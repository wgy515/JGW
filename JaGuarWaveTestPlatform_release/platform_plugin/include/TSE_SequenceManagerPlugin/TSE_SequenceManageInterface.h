#pragma once
#include "TSE_SequenceManageImpl.h"

namespace JGW
{
    class CTSE_SequenceManageInterface
    {
    public:
        virtual ~CTSE_SequenceManageInterface() {}
        virtual CTSE_SequenceManageImpl* GetSequenceManageImpl() = 0;

        virtual CTSE_SequenceManageImpl* GetSequenceManageImplToTokenID(unsigned long tokenID) = 0;
        virtual void ReleaseSequenceManageImpl(CTSE_SequenceManageImpl* pSequenceManageImpl) = 0;
    };
}