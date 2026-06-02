// JGW_PumaDownLoad.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PumaDownload_Define.h"
#include "CJGW_PumaImageFmtData.h"
#include "CJGW_DownloadPumaAsioSerialPort.h"
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>
using namespace JGW;
/*SPrsFlashCmdParms gsPrsFlashCmdParm = {0};*/

// void InitOpts(SPrsFlashCmdParms* a_pOpts)
// {
//     // Initialize defaults
//     a_pOpts->imagesFolder = ".";
//     a_pOpts->imageBinName = DEFAULT_IMAGE_BINARY_FILENAME;
//     a_pOpts->baudRate = "115200";
//     a_pOpts->serialDevice = "";
//     a_pOpts->bFlashOTP = 0;
//     a_pOpts->bFlashConfig = 0;
//     a_pOpts->bFlashImage = 0;
//     a_pOpts->bPrintMemory = 0;
//     a_pOpts->address = 0;
//     a_pOpts->len = 0;
//     a_pOpts->flashSizeKb = PRS_FLASH_DEFAULT_SIZE_KB;
//     a_pOpts->bErase = 0;
//     a_pOpts->bVersion = 0;
//     a_pOpts->bReset = 1;
// }


int _tmain(int argc, _TCHAR* argv[])
{
    CCJGW_PumaImageFmtData pumaImageFmtData;

    if (!pumaImageFmtData.FmtPumaImageFile("D:\\开发资料\\公司资料\\Write-MAC\\firmware\\image.bin"))
    {
        return -1;
    }

    S_PUMA_DOWNLOAD_INFO sPumaDownloadInfo;
    sPumaDownloadInfo.mbValidateImageDataInfo = true;
    sPumaDownloadInfo.mpPumaImageDataInfo = &pumaImageFmtData.GetPumaImageDataInfo();
    sPumaDownloadInfo.mpPumaValidateImageDataInfo = &pumaImageFmtData.GetPumaValidateImageDataInfo();
    boost::asio::io_service ios;
    CCJGW_DownloadPumaAsioSerialPort donwloadPumaAsioSerialPort(ios,0);

    donwloadPumaAsioSerialPort.GetDownladProgressInfo().mstrComPort = "COM33";
    donwloadPumaAsioSerialPort.StartDownloadPumaImgae(&sPumaDownloadInfo);

    ios.run();
	return 0;
}

