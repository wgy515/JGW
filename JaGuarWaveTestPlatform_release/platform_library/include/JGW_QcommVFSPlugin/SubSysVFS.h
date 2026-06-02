#pragma once
#include "SubSysVFSBase.h"
#include "SubSysVFSDefine.h"
class CSubSysVFS 
{
public:
	CSubSysVFS(void);
	~CSubSysVFS(void);
	 //初始化加载类
	 CSubSysVFSBase* InitClass(COMMUNICATIONSSYS eSys);
	 //获取加载类
	 CSubSysVFSBase* GetClass(){return m_lpSubSysBase;}
	 //获取频段相关信息
	 SBAND_INFO& GetBandInfo();
public:
	CSubSysVFSBase* m_lpSubSysBase;
	HANDLE	m_hResourceContext;
};
