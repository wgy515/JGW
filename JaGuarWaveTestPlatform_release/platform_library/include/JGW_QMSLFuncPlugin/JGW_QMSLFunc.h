#pragma once
#include "QMSL_inc/QLib.h"
#include "QMSL_inc/QLib_Defines.h"
#include <JGW_QMSLFuncPlugin\jgw_device_api_define.h>
#include <JGW_QMSLFuncPlugin\JGW_QMSLFuncPlugin_Define.h>

namespace JGW
{
    class JGWQMSLFUNCPLUGIN_API CJGW_QMSLFunc
    {
    public:
        CJGW_QMSLFunc(HANDLE& hResourceContext);
        ~CJGW_QMSLFunc(void);
        
        //! GPS
    public:
        bool GPS_GEN8_Get_GNSS_Engine_Revision(long& EngineRevision);
        bool GPS_GEN8_GNSS_NAV_SET_CONFIG(unsigned char  Requested_Constellation_Mask,unsigned char& Supportted_Contellation_Mask,unsigned char& Actual_Constellsation_Mask );
        //! \param piStatus = output, 0=Command Accepted, 1=Command Rejected \return true if successful, false if fail.
        bool DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(unsigned char iFlag,unsigned char &piStatus);
        //!
        bool DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(unsigned char iCollectMode,unsigned short iCaptureSize_kSamples,unsigned char iNumberOfIntegrations,double& pdCNoDBHz, int& piFreqInHz,bool bExitSARFOnComplete = false);
    private:
        HANDLE& mhResourceContext;
    };
}

