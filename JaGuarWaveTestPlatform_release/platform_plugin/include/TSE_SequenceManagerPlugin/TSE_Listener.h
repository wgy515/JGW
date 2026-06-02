#pragma once
#include <vector>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>

namespace JGW
{
    template < class T >
    class CTSE_Listener
    {
    public:
        //! Ìí¼Ó¼àÌý
        void AddListener(T* pListener)
        {
            CCJGW_CriticalSectionAutoLock csa(mcs);
            if (!pListener) return;
            if ( -1 == find_if(mvAwaitListener,std::bind2nd(std::equal_to<T*>(), pListener)) )
            {
                mvAwaitListener.push_back(pListener);
            }
        }
        //! ÒÆ³ý¼àÌý
        void RemoveListener(T* pListener)
        {
            CCJGW_CriticalSectionAutoLock csa(mcs);
            if (pListener) erase_if(mvRwaitListener,std::bind2nd(std::equal_to<T*>(), pListener));
        }

        void AddAwaitListener()
        {
            if (mvAwaitListener.empty() && mvRwaitListener.empty()) return;

            CCJGW_CriticalSectionAutoLock csa(mcs);
            std::vector<T*>::iterator it = mvAwaitListener.begin();

            for (;it != mvAwaitListener.end();it ++)
                mvListener.push_back(it[0]);
            mvAwaitListener.clear();

            it = mvRwaitListener.begin();
            for (;it != mvRwaitListener.end();it ++)
               erase_if(mvListener,std::bind2nd(std::equal_to<T*>(), it[0]));
            mvRwaitListener.clear();      
        }

    protected:
        std::vector<T*> mvAwaitListener;
        std::vector<T*> mvRwaitListener;
        std::vector<T*> mvListener;
        CCJGW_CriticalSectionLock mcs;
    };
}