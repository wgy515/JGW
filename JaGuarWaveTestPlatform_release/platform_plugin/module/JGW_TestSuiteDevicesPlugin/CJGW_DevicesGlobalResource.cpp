#include "StdAfx.h"
#include "CJGW_DevicesGlobalResource.h"

namespace JGW
{
    jgw_wr_nv_api* CCJGW_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(HANDLE hResourceContext/* = NULL*/,unsigned char chUseQPST/* = FALSE*/)
    {
        static unsigned char gOldchUseQPST = FALSE;
        if (gOldchUseQPST != chUseQPST)
        {
            GetQcommDiagActionIsntance()->DisConnectDevice();
            GetQcommDiagActionIsntance()->InitializeQMSL(chUseQPST);
        }
        return GetQcommDiagActionIsntance();
    }

    jgw_wr_nv_api* CCJGW_DevicesGlobalResource::GetQcommDiagActionIsntance()
    {
        static jgw_wr_nv_api gpQcommWRNVApi;
        //! if (!gpQcommWRNVApi) gpQcommWRNVApi = new jgw_wr_nv_api(NULL,false);
        return &gpQcommWRNVApi;
    }

    CCJGW_DownloadPDBImage* CCJGW_DevicesGlobalResource::GetDownloadPDBInstance()
    {
        //! static CCJGW_DownloadPDBImage gDownloadPDBImage;
        //! return &gDownloadPDBImage;
        return NULL;
    }
}

