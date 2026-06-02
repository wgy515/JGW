#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_RFQualcommInterface.h"

namespace JGW
{
    class CTSE_RFCalVFSTest : public CTSE_TestBase
    {
    public:
        CTSE_RFCalVFSTest(void);
        ~CTSE_RFCalVFSTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
        bool TSE_Exit();
    private:
        void SaveCPKDataFile(bool bResult,int nTotalTime);
    private:         
        bool						m_bInitOk; 
        unsigned char				m_bUserQpst;
        std::wstring                mstrSNEnvironment;
        std::wstring                m_strOperateID;
        std::wstring                m_strCPKFilePathEnvironment;
        S_RFQUALCOMM_INTERFACE_INFO m_sRFQcommInterInfo;
        CTSE_RFQualcommInterface	m_rfQualcommInter;
    };
}