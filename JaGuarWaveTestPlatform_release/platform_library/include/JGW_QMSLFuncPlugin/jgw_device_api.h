#pragma once
#include <string>

#include "QMSL_inc/QLib.h"
#include "QMSL_inc/QLib_Defines.h"
#include <JGW_QMSLFuncPlugin\jgw_device_api_define.h>
#include <JGW_QMSLFuncPlugin\JGW_QMSLFuncPlugin_Define.h>
typedef unsigned char			byte;
#define OPEN_QMSL_LOG

namespace JGW
{
    class JGWQMSLFUNCPLUGIN_API jgw_device_api
    {
    public:
        jgw_device_api(HANDLE hResourceContext = NULL,unsigned char chUseQPST = FALSE);
        ~jgw_device_api(void);
    public:
        //! 
        bool QUTS_Start_TCP_Server(int type = 0, unsigned int port = 5000);
        //! 
        void QUTS_Stop_TCP_Server(unsigned int port);
        //! 
        //! bool ConnectStandaloneWlanCardQUTS(std::string strCom);
    public:
        //! 连接9008下载端口（处于sahara协议）
        bool ConnetSaharaPort(__in unsigned int& iComPort,swdlQPHONEMSCB pSWDL_QPHONEMS_CB = NULL);
        bool QPHONEMS_Sahara_FlashProgrammer(char* sARMPRG_FileName);
        void DisconnectServerSahara();
		//! firehose 协议
        bool QPHONEMS_ConnectServer_FireHose(unsigned int iComPort,swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
        void QPHONEMS_FireHoseConfigureCallback(swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
        bool QPHONEMS_FireHoseConfigure(char* MaxPayloadSizeToTargetInBytes = "49152",
            char* memory_name = "eMMC",
            char* target_name = "8916",
            char* AckPacketNumber = "100",
            unsigned char bSpecifyMaxPayloadSizeToTargetInBytes = 0,
            unsigned char bUseSkipWriteAttr = 0,
            unsigned char bUseAlwaysValidateAttr = 0,
            unsigned char bUseVerboseAttr = 0,
            unsigned char bUseAckRawDataEveryNumPackets = 0);
        bool QPHONEMS_FireHoseNOP(void);
        bool QPHONEMS_SetSendDataDelay_FireHose(int timeinMilliSeconds = 0);
        bool QPHONEMS_EnableDataLogToFile_FireHose(unsigned char enabled = 0);
        bool QPHONEMS_EnableReadDataValidation_FireHose(unsigned char mode = 0);
        bool QPHONEMS_UploadEmmcImage_FireHose(char* rawprogramfile, char* patchfile,float* imageSizeInMB, float* throughput);
        //! 暂定 此函数有问题
        bool QPHONEMS_DownloadUserPartitions(char* sARMPRG_FileName,char* sPartitionFileName,SWDL_UserPartitionList* pPartitonList,unsigned char bOverridePrtnTable = 1);
        void QPHONEMS_UploadSBMultiImage_ConfigureCallBack(swdlQPHONEMSCB pSWDL_QPHONEMS_CB);
        bool QPHONEMS_DLoad_SendNop(void);
        void DisconnectServerFireHose(void);
        //! 获取GSM RX LEVEL
        bool FTM_GSM_BER_RX_LEVEL_REQ(unsigned short* iStatus, unsigned long *iRSSIValue, short* iRxPwr_in16dBm);
        //! 获取TDS对应Chain的RX LEVEL
        bool FTM_TDSCDMA_BER_RX_LEVEL_REQ(unsigned char iRxChain, unsigned short* iStatus, short*iRxAGC, short* iRxPowerdBm);
        //! 获取WCDMA 对应Chain 的RX LEVEL
        bool FTM_WCDMA_BER_RX_LEVEL_REQ(unsigned char iRxChain, unsigned short* iStatus, short*iRxAGC, short* iRxPowerdBm);
        //! 获取LET 对应Chain的的RX LEVEL
        bool FTM_LTE_NS_GET_DL_LEVEL(unsigned char iRxChain, short* iRxAGC, short* iRxLeveldBm10, unsigned long* pStatus);



        /**********************FAST MMI*************************/
        //! 删除FASTMMI 测试项数据文件
        bool FTM_AP_ERASE_ALL_FILES();
        //!
        bool FTM_AP_CLEAR_RESULTS();
        //! 执行AP单个测试项目
        bool FTM_AP_EXECUTE_SINGLE_TEST(const char* testCase);
        //!
        bool FTM_AP_READ_FILE(const char* strSavePath = "c:\\mmi.res",const char* sRemoteFileName = "mmi.res");
        //!




    public:
        //! 初始化QMSL 库
        static void	InitializeQMSL(unsigned char iConnectionEnum = eQPhoneMS);
        //! 连接设备
        bool ConnectPhone(unsigned int uiTimeOut = 60,unsigned short comPort = QLIB_COM_AUTO_DETECT);
        //! 检查设备是否连接
        bool IsMobileConnected(void);
        //! qmsl  Start logging
        void StartLogging(const char* sLogFile = "C:\\Factory_log.txt",unsigned int uiLogFlags = LOG_DEFAULT/*0xFFFF*/);
        //! qmsl Stop logging
        void StopLogging(void);
        //! Disconnect Server
        void DisconnectServer(void);
        //! 获取设备句柄
        HANDLE GetResourceContext(void){ return m_hResourceContext; }
        //! 设置设备句柄
        void SetResourceContex(void* hResourceContext){ m_hResourceContext = hResourceContext; }

        bool JGW_QLIB_SendSync(
            short iRequestSize,
            unsigned char* piRequestBytes,
            short* piResponseSize,
            unsigned char* piResponseBytes,
            unsigned long iTimeout);

        //! 将修改的NV备份保存,防止修改错误
        bool SyncNv(void);
        void TerminateQPSTServer(void);
        //! 读取NV
        bool ReadNV(word iItemID,byte *pItemData,int iLength);
        //! 读取NV INDEX 
        bool ReadNV(word iItemID,byte *pItemData,int iLength,word iContextID);
        //! 写NV
        bool WriterNV(word iItemID,unsigned char *pItemData,int iLength);
        //ID of SIM index NV Item pertains to ( 0 or 1)
        bool WriterNV(word iItemID,unsigned char *pItemData,int iLength,word iContextID);
        //! 打开权限控制 可以对高通系统NV修改
        bool OpenDMSSSecurityPlan(std::string sSPC = "000000");
        //! 获取错误消息
        //!LPCTSTR GetErrorMsg(void) { return m_strErrorMsg.c_str(); }
        //! Load Rf NV
        bool LoadRfNV();
        //! 发送M网USB消息到设备中 0: Qcomm Usb swicth  1: mcwill usb Switch 2:McWill Reset  3:switch mcwill and reset
        bool SendMcwillUsbMsgToDevice(E_MCWILL_USB_MSG cmd_id, int time_out = 2000);
        //! 读取MCWILL信号强度
        bool ReadMcwillNetCSQToDevice(int& csq,int time_out = 2000);
        //! 读取子版本号
        bool ReadQcommSubsysVersion(int& sub_version,int time_out = 2000);
        //! 读取硬件类型
        bool ReadMcwillHWType(std::string& strHWType,int time_out = MCWLL_AT_COMMAND_TIME_OUT);
        //! 读取PID
        bool ReadMcwillPid(std::string& strPid,int time_out = MCWLL_AT_COMMAND_TIME_OUT);
        //! 读取PID
        bool WriteMcwillPid(const std::string strPid,int time_out = MCWLL_AT_COMMAND_TIME_OUT);
        //! 写USB MAC地址 strUSBMac : 001218145623
        bool WriteMcwillUSBMac(const std::string strUSBMac,int time_out = MCWLL_AT_COMMAND_TIME_OUT);
        //! 读USB MAC地址
        bool ReadMcwillUSBMac(std::string& strUsbMac,int time_out = MCWLL_AT_COMMAND_TIME_OUT);
        //! 
        bool WriteMcwillAtCommand(E_MCWILL_USB_MSG cmd_id,const std::string strNumber,int time_out = MCWLL_AT_COMMAND_TIME_OUT);

        //! 
        bool WriteMcwillAtCommand(E_MCWILL_USB_MSG cmd_id,const std::string strNumber,std::string& strReturnValue,int time_out = MCWLL_AT_COMMAND_TIME_OUT);
        //! 
        bool ReadMcwillAtCommand(E_MCWILL_USB_MSG cmd_id,std::string& strReturnValue,int time_out = MCWLL_AT_COMMAND_TIME_OUT);
        //! mode_enum_type
        bool SetPhoneMode(mode_enum_type ePhoneMode);
        //! 重启设备 uiTimeOut 等待重启时间（以秒为单位）
        bool ResetPhone(unsigned int uiTimeOut = 5);
        void NVConfigureCallBack(nvToolCB nvToolCBFunc);
        //! 从设备中备份QCN
        bool BackupQCNToMobile(std::string backupQcnPath);
        //! QLIB_NV_WriteNVsToQCN  从PC QCN还原到设备中
        bool RestoreQCN(std::string uploadQcnPath);
        //! 下载QCN文件 pc to phone
        bool UploadQcnFile(std::string QcnPath,
            std::string sSPC = "000000",
            bool bAutoRestore = true,
            bool bSkipReset = true,
            bool bAllowEsnMismatch = true,
            bool bIgnorePhoneModel = true);
        //! phone to pc
        bool DownloadQcnFile(std::string backupQcnPath,int*count = NULL);
        //! 设置设备工厂测试模式
        bool SetUEFFBMModel(unsigned char iBootMode,unsigned char iSubBootMode = 0);
        //! 建厂当前设备是否处于FFBM模式
        bool CheckUEFFBMModel(int &nCurrentBootMode,int &nNextBootMode = m_nNextBootModeTemp);
        //! 切换到9008模式
        bool SwitchPhoneEDL(void);
        //! 切换到9008模式(慎用)
        bool SwitchToDownloadMode(void);
    private:
        const wchar_t* GetQCNResultCodeAsString(int iResultCode);
        //! 获取NV状态错误字符串
        const wchar_t* GetNVStatusAsString(word result);
        //!
        const wchar_t* GetRfNVErrorCodeAsString(word errorCode);
        //QMSL_DownloadAgent_Error_Enum
        const wchar_t* GetQMSLDownloadAgentErrorString(long errorCode);
    public:
        static unsigned char mbUseQPST;
    private:
        static bool		m_bInitQMSL;
        static int		m_nNextBootModeTemp;
    protected:
        //! std::wstring    m_strErrorMsg;
        HANDLE          m_hResourceContext;
    };
}