#pragma once
#include <vector>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

class CSortFastPortRecord
{
public:
	CSortFastPortRecord();
	~CSortFastPortRecord(void);
	static void			SetFastPortRecordFilePath(LPCTSTR strSortRecordFile){m_strSortRecordFile = strSortRecordFile;}
	//写端口记录文件
	void				WriteFastPortRecordFile(int nTestThread,int nBindPortIndex/*,LPCTSTR strSerialNumber*/);
	//读取端口记录文件
	void				ReadFastPortRecordFile(int nTestThread,int &nBindUsbPortIndex/*,CString &strSerialNumber*/);
	//获取记录数组
	void				GetPortRecordVector(std::vector<int> &vThread);
	//添加端口记录数组
	void				AddPortRecordVector(int nThreadIndex);
	//根据FastDB获取显示排序索引
	int					GetFastPortShowIndex(int nBindPortIndex);

	int					GetDevCounts();

	void				ClearSortRecorder();
	void				SetFastPortIniPath(){ m_cInifConfig.InitIniFilePath(m_strSortRecordFile.c_str());}
private:
	static std::wstring		m_strSortRecordFile; 
	JGW::CCJGW_ConfigIni			m_cInifConfig;

};

