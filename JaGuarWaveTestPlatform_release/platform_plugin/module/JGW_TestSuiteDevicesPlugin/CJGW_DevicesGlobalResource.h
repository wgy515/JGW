#pragma once
#include <JGW_QMSLFuncPlugin/jgw_wr_nv_api.h>
#include <JGW_WindowsFuncPlugin/CJGW_DownloadPDBImage.h>

namespace JGW
{
    class CCJGW_DevicesGlobalResource
    {
    public:
        static jgw_wr_nv_api* GetConnetQcommDiagActionIsntance(HANDLE hResourceContext = NULL,unsigned char chUseQPST = FALSE);
        static jgw_wr_nv_api* GetQcommDiagActionIsntance();

        static CCJGW_DownloadPDBImage* GetDownloadPDBInstance();
    };
}



