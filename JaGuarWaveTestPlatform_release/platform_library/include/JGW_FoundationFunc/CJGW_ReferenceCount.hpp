#pragma once

namespace JGW
{
    class CCJGW_ReferenceCount
    {
    public:
        CCJGW_ReferenceCount() : m_nRefCount(0)
        {

        }

        void AddReferenceCount()
        {
            if ( m_nRefCount < 0)
            {
                InterlockedExchange((LPLONG)&m_nRefCount,0);
            }
            InterlockedIncrement(&m_nRefCount); 
        }

        void DecReferenceCount()
        {
            InterlockedDecrement(&m_nRefCount);
        }

        long GetReferenceCount()
        {
            return m_nRefCount;
        }

    private:
        long m_nRefCount;
    };

    class CCJGW_ReferenceAutoCount
    {
    public:
        CCJGW_ReferenceAutoCount(CCJGW_ReferenceCount& referenceCount) : mReferenceCount(referenceCount)
        {
            mReferenceCount.AddReferenceCount();
        }
        ~CCJGW_ReferenceAutoCount()
        {
            mReferenceCount.DecReferenceCount();
        }
    private:
        CCJGW_ReferenceCount& mReferenceCount;
    };
}