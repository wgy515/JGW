#pragma once
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteImpl.h>
#include "CJGW_RFQualcommInterface.h"
namespace JGW
{
    class CCJGW_RFCalVFSSuite:public CCJGW_TestSuiteImpl
    {
    public:
        CCJGW_RFCalVFSSuite(void);
        ~CCJGW_RFCalVFSSuite(void);
    private:
        bool CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool UpdateEnviromentParam();
        bool Init();
        bool Run();
    private:
        void*						m_iface;
        CCJGW_RFQualcommInterface		m_rfQualcommInter;
        std::wstring			    m_strQcServerName;
        bool						m_bInitOk;
        int							m_nErrorReTest;
        bool						m_bErrorStop;
        unsigned char				m_bUserQpst;
        std::string					m_strEquipPad;
        std::string					m_strEquipName;
        std::string					m_strXttPath;
    };
}


