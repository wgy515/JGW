#pragma once
#include "subsysvfslte.h"
namespace JGW
{
    class CSubSysVFSLTEV1 :
        public CSubSysVFSLTE
    {
    public:
        CSubSysVFSLTEV1(void);
        /*public:*/
        ~CSubSysVFSLTEV1(void);

        static CSubSysVFSBase *GetInstance()
        {
            if( !m_lpInstance )
                m_lpInstance = new CSubSysVFSLTEV1;
            return m_lpInstance;
        }

        virtual bool HandOverChannel();
        virtual bool StartCall();

        bool HandChannel();

        static CSubSysVFSBase *m_lpInstance;
    };
}