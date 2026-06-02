#pragma once
#include <vector>
#include "CJGW_ConfigIni.h"

namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_DownloadPortSort
    {
    public:
        CCJGW_DownloadPortSort(void);
        ~CCJGW_DownloadPortSort(void);
    public:
        //! 初始化下载排序索引配置文件
        void InitDownloadPortSortConfig(const std::wstring& strDownloadPortSortConfigFilePath);
    public:
        //写端口记录文件
        void WriteFastPortRecordFile(int nTestThread,int nBindPortIndex/*,LPCTSTR strSerialNumber*/);
        //读取端口记录文件
        void ReadFastPortRecordFile(int nTestThread,int &nBindUsbPortIndex/*,CString &strSerialNumber*/);
        //获取记录数组
        void GetPortRecordVector(std::vector<int> &vThread);
        //添加端口记录数组
        void AddPortRecordVector(int nThreadIndex);
        //根据FastDB获取显示排序索引
        int GetFastPortShowIndex(int nBindPortIndex);

        int GetDevCounts();
        //! 清空排序记录
        void ClearSortRecorder();
    private:
        std::wstring mstrDownloadPortSortConfigFilePath;
        CCJGW_ConfigIni	m_cInifConfig;
    };
}