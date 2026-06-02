#pragma once

namespace JGW
{
    class CCJGW_SortUsbRecordImpl
    {
    public:
        virtual ~CCJGW_SortUsbRecordImpl();
        //! 加载USB排序的记录配置文件
        virtual bool LoadSortUsbRecordConfig(const wchar_t* strModuleFolder);
        //! 
        virtual size_t GetUsbSortIndexTo();
    };
}