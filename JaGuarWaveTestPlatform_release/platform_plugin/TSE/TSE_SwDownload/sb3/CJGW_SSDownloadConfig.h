#pragma once
#include "sw_sb3_download_define.h"
#include "CJGW_SSDownloadFramePacket.h"
//! HDLC 
//! async_HDLC
namespace JGW
{
    class CCJGW_SSDownloadConfig
    {
    public:
        CCJGW_SSDownloadConfig(void);
        ~CCJGW_SSDownloadConfig(void);

        bool LoadSSDwonloadConfig(ps_download_config_info pSDownloadConfigInfo);

        s_download_all_command& GetDownloadAllCommand() { return msDownloadAllCommand; }
    private:
        //! 加载ARM PRG文件
        bool    LoadArmPrgFile();
        //! 加载Content XML
        bool    LoadContentXMLFile();
        //! 构造流命令
        bool    BuildStreamCommand();
        //! 
        bool    BuildHelloCmd();

        bool    BuildStreamSecurityModeCmd();

        bool    BuildStreamPartitionCmd();

        bool    BuildStreamWriteCmd();

        bool    BuildClosePacketCmd();

        bool    BuildResetPacketCmd();
    private:
        ps_download_config_info mpSDownloadConfigInfo;

        s_download_all_command msDownloadAllCommand;
        
        std::vector<S_PARTITION_INFO>	mvPartitionInfo;

        CCJGW_SSDownloadFramePacket mFramePacket;
    };
}
