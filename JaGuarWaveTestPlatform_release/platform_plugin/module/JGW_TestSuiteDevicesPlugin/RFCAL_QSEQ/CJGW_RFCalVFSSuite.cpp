#include "StdAfx.h"
#include "CJGW_RFCalVFSSuite.h"
#include <Shlwapi.h>

namespace JGW
{
    CCJGW_RFCalVFSSuite::CCJGW_RFCalVFSSuite(void):m_iface(NULL)
        ,m_strQcServerName(L"")
        ,m_bInitOk(false)
        ,m_rfQualcommInter(m_strQcServerName)
        ,m_bErrorStop(true)
        ,m_nErrorReTest(3)
        ,m_bUserQpst(FALSE)
        ,m_strEquipPad("20")
        ,m_strEquipName("CMW500")
        ,m_strXttPath("")
    {
    }


    CCJGW_RFCalVFSSuite::~CCJGW_RFCalVFSSuite(void)
    {
    }

    bool CCJGW_RFCalVFSSuite::CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CCJGW_TestSuiteImpl::CreateParam(strParamName,strParamValue)) return true;
        if( 0 == JGW_WStrComparenoCaseWStr(_T("qcservername"),strParamName) )
        {
            m_strQcServerName = strParamValue;
            if( PathIsRelative(strParamValue) )
            {
                m_strQcServerName = JGW_GetApplicationFolder();
                m_strQcServerName += strParamValue;
            }
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("xttName"),strParamName) )
        {
            //! 获得配置XML文件路径
            std::wstring strXttName = strParamValue;
            if( PathIsRelative(strParamValue) )
            {
                strXttName = JGW_GetApplicationFolder();
                strXttName += strParamValue;
            }
            m_strXttPath = JGW_W2A(strXttName);
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("ErrorReTest"),strParamName) )
        {
            m_nErrorReTest = _ttoi(strParamValue);
            return true;
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("ErrorStop"),strParamName) )
        {
            m_bErrorStop = _ttoi(strParamValue)?true:false;
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("QMSLLibraryMode"),strParamName) )
        {
            m_bUserQpst = _ttoi(strParamValue)?TRUE:FALSE;
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("EquipName"),strParamName) )
        {
            m_strEquipName = JGW_W2A_W(strParamValue);
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("EquipPad"),strParamName) )
        {
            m_strEquipPad = JGW_W2A_W(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CCJGW_RFCalVFSSuite::UpdateEnviromentParam()
    {
        m_bInitOk = false;
        if(!m_rfQualcommInter.InitQCServerModule()) return false;
        m_rfQualcommInter.InitQCServerInterface(m_strXttPath,m_bUserQpst,m_strEquipPad,m_strEquipName);
        m_rfQualcommInter.SetErrorReTestCount(m_nErrorReTest);
        m_rfQualcommInter.SetErrorStop(m_bErrorStop);
        m_bInitOk = m_rfQualcommInter.LoadRefXtt(m_strXttPath);

        return m_bInitOk;
    }

    bool CCJGW_RFCalVFSSuite::Init()
    {
        if (!m_bInitOk) return UpdateEnviromentParam();
        return true;
    }

    bool CCJGW_RFCalVFSSuite::Run()
    {
        DEBUG_MSG(L"CCJGW_RFCalVFSSuite::Run\n");
        return m_rfQualcommInter.RunXtt();
    }
}

