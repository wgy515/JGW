// StreamingDownload.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ss_download_asio_serial_port.h"
#include "CJGW_SSDownloadConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

//! StreamingDownload.exe -p COM16 -f D:\\Development\\UC600AA\\UC600AA_20200824_0625\\ -e ENANDPRG40xx.mbn -n ENANDPRG40xx.mbn -c D:\\Development\\UC600AA\\UC600AA_20200824_0625\\jw_download_qpst_nand_img.xml
int _tmain(int argc, _TCHAR* argv[])
{
    JGW::CCJGW_SSDownloadConfig downloadConfig;
    s_download_config_info sDownloadConfigInfo;
    bool bStreamingDownload = true;
    sDownloadConfigInfo.mbReset = true;
    std::string strComPort = "COM16";
    sDownloadConfigInfo.mstrDownloadPath = "D:\\Development\\UC600AA\\UC600AA_20200824_0625\\";
    sDownloadConfigInfo.mstrEnArmPrgFileName = "ENANDPRG40xx.mbn";
    sDownloadConfigInfo.mstrNArmPrgFileName = "ENANDPRG40xx.mbn";
    sDownloadConfigInfo.mstrContentXMLFilePath = "D:\\Development\\UC600AA\\UC600AA_20200824_0625\\jw_download_qpst_nand_img.xml";

    for (int i = 1;i < argc - 1;i ++)
    {
        if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-p",argv[i]))
        {
            strComPort = JGW::JGW_W2A(argv[i + 1]);
            i++;
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-f",argv[i]))
        {
            sDownloadConfigInfo.mstrDownloadPath = JGW::JGW_W2A(argv[i + 1]);
            i++;
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-e",argv[i]))
        {
            sDownloadConfigInfo.mstrEnArmPrgFileName = JGW::JGW_W2A(argv[i + 1]);
            i++;
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-n",argv[i]))
        {
            sDownloadConfigInfo.mstrNArmPrgFileName = JGW::JGW_W2A(argv[i + 1]);
            i++;
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-c",argv[i]))
        {
            sDownloadConfigInfo.mstrContentXMLFilePath = JGW::JGW_W2A(argv[i + 1]);
            i++;
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-bs",argv[i]))
        {
            bStreamingDownload = 1 == _ttoi(argv[i + 1]);
            i++;
        }
    }


    if (!downloadConfig.LoadSSDwonloadConfig(&sDownloadConfigInfo))
    {
        dbg(LOG_INFO,ss_download.GetDownladProgressInfo().mnIndex,"Load Download Config Fail\n");
        return 0;
    }
    //! return 0;
    boost::asio::io_service ios;

    JGW::ss_download_asio_serial_port ss_download(ios,1);
    ps_download_all_command psDownloadAllCommad = &downloadConfig.GetDownloadAllCommand();

    ss_download.GetDownladProgressInfo().mstrComPort = strComPort;
    if (!ss_download.start_download(psDownloadAllCommad->mnFileSizeCounts,&psDownloadAllCommad->msSaharahProtocolArmPrgFile,&psDownloadAllCommad->mvStreamProtCommand,bStreamingDownload))
    {
        dbg(LOG_INFO,ss_download.GetDownladProgressInfo().mnIndex,"Download File Fail\n");
        return 0;
    }

    //ss_download.start_streaming_download(psDownloadAllCommad->mnFileSizeCounts,&psDownloadAllCommad->mvStreamProtCommand);
    try
    {
        ios.run();
        if (ss_download.GetDownladProgressInfo().meDownloadStatus == E_SUCCESSFUL_DOWNLOAD)
        {
            dbg(LOG_INFO,ss_download.GetDownladProgressInfo().mnIndex,"File transferred successfully\n");
        }
        else
        {
            dbg(LOG_INFO,ss_download.GetDownladProgressInfo().mnIndex,"Download File Fail\n");
        }
    }
    catch (...)
    {
        dbg(LOG_INFO,ss_download.GetDownladProgressInfo().mnIndex,"Download File Fail\n");
    }
    return 0;
}

