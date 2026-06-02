#include "stdafx.h"
#include <JGW_QMSLFuncPlugin/SubSysVFS.h>
#include <JGW_QMSLFuncPlugin/subsysvfsbase.h>
#include <JGW_QMSLFuncPlugin/SubSysVFSLTE.h>
#include <JGW_QMSLFuncPlugin/SubSysVFSCDMA.h>
#include <JGW_QMSLFuncPlugin/SubSysVFSTDSCDMA.h>
#include <JGW_QMSLFuncPlugin/SubSysVFSWCDMA.h>
#include <JGW_QMSLFuncPlugin/SubSysVFSGSM.h>
#include <JGW_QMSLFuncPlugin/SubSysVFSLTEV1.h>

namespace JGW
{
    CSubSysVFS::CSubSysVFS(void):m_lpSubSysBase(NULL)
        ,m_hResourceContext(NULL)
    {
    }

    CSubSysVFS::~CSubSysVFS(void)
    {
    }

    CSubSysVFSBase* CSubSysVFS::InitClass(COMMUNICATIONSSYS eSys)
    {
        switch ( eSys )
        {
        case TECHNOLOGY_LTE:
            m_lpSubSysBase = CSubSysVFSLTEV1::GetInstance();
            break;
        case TECHNOLOGY_WCDMA:
            m_lpSubSysBase = CSubSysVFSWCDMA::GetInstance();
            break;
        case TECHNOLOGY_CDMA:
            m_lpSubSysBase = CSubSysVFSCDMA::GetInstance();
            break;
        case GSM:
            m_lpSubSysBase = CSubSysVFSGSM::GetInstance();
            break;
        case TECHNOLOGY_TD_SCDMA:
            m_lpSubSysBase = CSubSysVFSTDSCDMA::GetInstance();
            break;
        default:
            m_lpSubSysBase = NULL;
            return NULL;
        }

        return m_lpSubSysBase;
    }

    SBAND_INFO& CSubSysVFS::GetBandInfo()
    {
        return m_lpSubSysBase->m_sBandInfo;
    }
}