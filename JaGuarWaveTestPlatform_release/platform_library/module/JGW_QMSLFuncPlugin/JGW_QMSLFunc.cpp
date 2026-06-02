#include "StdAfx.h"
#include <JGW_QMSLFuncPlugin/JGW_QMSLFunc.h>

namespace JGW
{
    CJGW_QMSLFunc::CJGW_QMSLFunc(HANDLE& hResourceContext) : mhResourceContext(hResourceContext)
    {
    }


    CJGW_QMSLFunc::~CJGW_QMSLFunc(void)
    {
    }

    bool CJGW_QMSLFunc::GPS_GEN8_Get_GNSS_Engine_Revision(long& EngineRevision)
    {
        return (TRUE == QLIB_GPS_GEN8_Get_GNSS_Engine_Revision(mhResourceContext,&EngineRevision));
    }

    bool CJGW_QMSLFunc::GPS_GEN8_GNSS_NAV_SET_CONFIG(unsigned char Requested_Constellation_Mask,unsigned char& Supportted_Contellation_Mask,unsigned char& Actual_Constellsation_Mask )
    {
        return (TRUE == QLIB_GPS_GEN8_GNSS_NAV_SET_CONFIG(mhResourceContext,Requested_Constellation_Mask,&Supportted_Contellation_Mask,&Actual_Constellsation_Mask));
    }

    bool CJGW_QMSLFunc::DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(unsigned char iFlag,unsigned char &piStatus)
    {
        return (TRUE == QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(mhResourceContext,iFlag,&piStatus));
    }

    bool CJGW_QMSLFunc::DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(unsigned char iCollectMode,unsigned short iCaptureSize_kSamples,unsigned char iNumberOfIntegrations,double& pdCNoDBHz, int& piFreqInHz,bool bExitSARFOnComplete /* = false */)
    {
        return (TRUE == QLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(mhResourceContext,iCollectMode,iCaptureSize_kSamples,iNumberOfIntegrations,&pdCNoDBHz,&piFreqInHz,bExitSARFOnComplete));
    }
}
