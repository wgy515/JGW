#pragma once
#include "sti_export_func_decl.h"
#include <string>
#include <vector>
#include "TSE_RFCalVFS_Define.h"
#include "CJGW_ParseCPKLogInfo.h"
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>

#define XTT_TIMEOUT 	"100"
#define XTT_TIMEOUT_OFFSET		"5"

namespace JGW
{
    class CTSE_RFQualcommInterface
    {
    public:
        CTSE_RFQualcommInterface(PS_RFQUALCOMM_INTERFACE_INFO psRFQcommInterInfo);
        ~CTSE_RFQualcommInterface(void);
    public:    //! QC Server初始化
        //! 初始化校准接口
        bool LoadQCServerModule();
        //! 初始化QCServer接口信息
        void InitQCServerInterface(unsigned char bUseQPST);
        //! 加载校准或者综测配置文件
        bool LoadRFCalVerTestConfig();
    public:
        //! 运行校准或者综测测试
        bool ExecuteRFCalVerTest();
        //! 
        CCJGW_ParseCPKLogInfo& GetParseCPKData();
    private:
        //!  保存LOG文件
        void SaveHtmlLogging(const std::wstring& strLogFilePath);

        //! 分发CPK LOG信息
        void HandleCPKLogInfo(const std::wstring& strCPKLogInfo);
        //xtt文件名
        bool LoadRFCalVerTestXMLConfig(const std::string  filename/*, const stm_assembly_info & ai*/);
        //! 注册测试函数log
        static void DutTestEventHandler(void * context, int type, const char * msg);
        //! 执行单个测试项目
        bool ExecuteSuiteIndex(int nIndex);
    private:
        //! 执行所有测试项目
        bool ExecuteTestAll();
        //! 执行测试过后销毁动作
        bool ExecuteDestoryTest();
        //! 加载校准或综测DLL
        HMODULE LoadRFCalVerTestDll();
        //! 释放已经加载的DLL
        void FreeRFCalVertestDll();
    private:     
        bool mbFristLoad;
        size_t m_nCurrentIndex;//! 当前测试的索引ID
        PS_RFQUALCOMM_INTERFACE_INFO mpsRFQcommInterInfo;
        CCJGW_MoveTestLogging mMoveTestLogging; //! 移动测试Log
        CCJGW_ParseCPKLogInfo mParseCPKLogInfo;
        std::vector<S_QCOMM_FRCALVFS_CONFIG> mvsQcommRFCalVFSConfig;
        std::vector<size_t>	m_vDestory;
    };
}
