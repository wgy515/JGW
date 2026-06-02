#include <CTF_QCSubSysVFS/SubSysVFS.h>
#include <CTF_QCSubSysVFS/subsysvfsbase.h>
#include <CTF_QCSubSysVFS/SubSysVFSLTE.h>
#include <CTF_QCSubSysVFS/SubSysVFSCDMA.h>
#include <CTF_QCSubSysVFS/SubSysVFSTDSCDMA.h>
#include <CTF_QCSubSysVFS/SubSysVFSWCDMA.h>
#include <CTF_QCSubSysVFS/SubSysVFSGSM.h>
#include <CTF_QCSubSysVFS/SubSysVFSLTEV1.h>

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
