#pragma once
#include <map>

class CComPortSort
{
public:
	CComPortSort(void);
	~CComPortSort(void);
private:
	void LoadSortPortInfoToIniConfig();

	int UpdatePortInfoToIniConfig(  int port );
public:
	int GetSortIndexToComPort( int port );
private:
	//! key port¶Ë¿Ú£¬indexÊÇÅÅÐòË÷Òý
	std::map<int,int>	mport_index_map;
	LPCTSTR				mstrIniPath;
};


extern int GetSortIndexToComport( int port );
