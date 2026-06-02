#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialPort.h>
#include "sw_sb3_download_define.h"
#include "CJGW_QcommDownloadSerialPort.h"

namespace JGW
{
    class CTSE_SaharaFileDownload : public CTSE_TestBase
    {
    public:
        CTSE_SaharaFileDownload(void);
        ~CTSE_SaharaFileDownload(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Init();
    private:
        //! 
        bool ReadSerialPort(char* buffer, size_t bytes_to_read, size_t& bytes_read);
        //! 
        bool WriteSerialPort(char* buffer,size_t bytes_to_send);
        //! size 0 means we don't know what to expect. So we'll just try to read the 8 byte header 
        bool ReadPacket(size_t bytes_to_read);
        //!
        bool ReadHandlePacket();
        //! read target
        //! bool ReadHelloPacket();
        //! 
        bool SendHelloResp(boot_sahara_mode mode);
        //! 
        //! bool ReadDataPacket();
        //! 
        //! bool ReadEndImageTxPacket();
        //! If target has not received all images inorder to boot, target sends IMAGE_TX_PENDING status withDONE_RESP; else target sends IMAGE_TX_COMPLETE status with DONE_RESP and exits Sahara protocol
        bool SendDone();
    private:
        bool is_ack_successful (int status);
        //! start_image_transfer
        bool SaharaStartImageTransfer();
        //!
        void CloseDownloadFile();
    private:
        char* mRxBuf;
        char* mTxBuf;
        FILE* mpDownloadFile;
        int mnCurrentImageId;
        boot_sahara_state msahara_state;
        sahara_packet_header* mpsahara_header;
        sahara_packet_hello* mpsahara_hello;
        sahara_packet_hello_resp* mpsahara_hello_resp;
        sahara_packet_end_image_tx* mpsahara_end_image_tx;
        sahara_packet_read_data* mpsahara_read_data;
        sahara_packet_cmd_switch_mode* mpsahara_cmd_switch_mode ;
        sahara_packet_done* mpsahara_done;
        sahara_packet_done_resp* mpsahara_done_resp;
        sahara_packet_reset_resp* mpsahara_reset_resp;
        int mnNumberOfSaharaProtocolFileMapping;
        CCJGW_QcommDownloadSerialPort* mpCSerialPort;
        std::wstring mstrSaharaSerialClassEnv;
        std::wstring mstrDownloadFolderPathEnv;
        std::string mstrDownloadFolder;
        std::map<std::string,std::string> mmapParameters;
        std::map<int,std::string> mmapImageIdFileName;
    };
}