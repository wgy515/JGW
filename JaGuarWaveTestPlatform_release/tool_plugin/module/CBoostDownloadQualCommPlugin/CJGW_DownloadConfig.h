#pragma once
#include <string>
#include <vector>
#include "download_define.h"

namespace JGW
{
    class CCJGW_DownloadConfig
    {
    public:
        CCJGW_DownloadConfig(void);
        ~CCJGW_DownloadConfig(void);
        //! 加载下载配置文件
        bool LoadDonwloadConfig(ps_download_config_info psDonwloadConfigInfo,const char* szMemoryName = "emmc");
        //! 获取下载所有文件信息
        s_download_all_file_info& GetDownloadAllFileInfo() { return msDownloadAllInfo; }
        //! 获取加载下载配置错误消息
        const char* GetErrorMsg(){ return mstrErrorMsg.c_str(); }

        static void  GetProgramNodeTag(char* szprogramNode,const char* szAttr,char* tag);
    private:
        //! 加载ARM PRG文件
        bool LoadArmPrgFile();
        //! 清除下载配置文件信息
        void CleanLoadDownloadFileInfo();
        //! 创建firehose XML配置文件
        bool BuildFireHoseXMLConfig();   
    private:
        bool BuildFristXMLConfig();
        bool BuildRawProgramXMLConfig(std::vector<std::string>& vstrRawProgram);
        bool BuildPatchXMLConfig(std::vector<std::string>& vstrPatch);
        bool BuildSetActivePartitionXMLConfig();
        bool BuildPowerResetXMLConfig(bool bReset);
    private:
        bool ParesePatchXMLConfigToFile(std::string strPatch);
        bool PareseRawProgramXMLConfigToFile(std::string strRawProgram);
        char* GetFileBufferToFilePath(std::string& strPath);
        bool GetFileBufferToFilePath(std::string& strPath,void* file_buf);
        bool PareseProgramNode(char* szprogramNode);
        
    private:
        s_download_all_file_info                msDownloadAllInfo;
        ps_download_config_info                 mpsDonwloadConfigInfo;
        std::string                             mstrErrorMsg;
    };
}

