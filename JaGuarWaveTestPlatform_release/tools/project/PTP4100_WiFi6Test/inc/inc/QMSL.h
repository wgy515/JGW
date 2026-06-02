/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_inc/QMSL.h#24 $
 * $DateTime: 2022/05/18 13:39:50 $
 *
 * DESCRIPTION: QMSL Includes
 ******************************************************************************
 *
 * Copyright (c) 2014-2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

//
// qmsl-lib-gps_gnss
//
#include "QMSL_CGPS_GNSS\inc\QMSL_CGPS_GNSS.h"

//
// qmsl-lib-connectivity
//
#include "QMSL_Connectivity\inc\QMSL_Connectivity.h"

//
// qmsl-lib-core
//
#include "QMSL_Core\inc\QMSL_Core.h"
#include "QMSL_Core\inc\QMSL_Diag.h"
#include "QMSL_Core\inc\QMSL_Diag_SubSysCommand.h"
#include "QMSL_Core\inc\QMSL_EFS.h"
#include "QMSL_Core\inc\QMSL_FTM_GPS.h"
#include "QMSL_Core\inc\QMSL_FTM_Log.h"
#include "QMSL_Core\inc\QMSL_FTM_Sequencer.h"
#include "QMSL_Core\inc\QMSL_QCN.h"
#include "QMSL_Core\inc\QMSL_QFUSE.h"
#include "QMSL_Core\inc\QMSL_QPST_PortManagement.h"
#include "QMSL_Core\inc\QMSL_Quts_Intf.h"
//
// qmsl-lib-miscellaenous
//
#include "QMSL_Misc\QMSL_AP\inc\QMSL_FTM_AP.h"
#include "QMSL_Misc\QMSL_AP\inc\QMSL_FTM_FFBM.h"
#include "QMSL_Misc\QMSL_Audio\inc\QMSL_FTM_Audio.h"
#include "QMSL_Misc\QMSL_Camera\inc\QMSL_FTM_Camera.h"
#include "QMSL_Misc\QMSL_Femto\inc\QMSL_Femto.h"
#include "QMSL_Misc\QMSL_MatlabHelpers\inc\QLib_MatlabHelpers.h"
#include "QMSL_Misc\QMSL_PMIC\inc\QMSL_FTM_PMIC.h"
#include "QMSL_Misc\QMSL_SECURITY_ID_SENSOR\inc\QMSL_FTM_SECURITY_ID_SENSOR.h"

//
// qmsl-lib-UIM
// qmsl-lib-Windows-phones 
// Note: 5/2022 all qmal-lib-SWDL APIs are removed
//
#include "QMSL_UIM\inc\QMSL_GSDI_Diag.h"
#include "QMSL_WP\inc\QMSL_WP7_PVK.h"
#include "QMSL_WP\inc\QMSL_FTM_WP.h"


//
// qmsl-lib-cellular
//
#include "QMSL_WWAN\inc\QMSL_BandHelper.h"
#include "QMSL_WWAN\inc\QMSL_FTM_AGPS.h"
#include "QMSL_WWAN\inc\QMSL_FTM_CDMA2000_NS.h"
#include "QMSL_WWAN\inc\QMSL_FTM_CDMA_Intelliceiver_RF.h"
#include "QMSL_WWAN\inc\QMSL_FTM_Common_Dispatch.h"
#include "QMSL_WWAN\inc\QMSL_FTM_Diagnostic_Subsystem.h"
#include "QMSL_WWAN\inc\QMSL_FTM_EGPRS_NS.h"
#include "QMSL_WWAN\inc\QMSL_FTM_EVDO_NS.h"
#include "QMSL_WWAN\inc\QMSL_FTM_GSM_RF.h"
#include "QMSL_WWAN\inc\QMSL_FTM_LTE_NS.h"
#include "QMSL_WWAN\inc\QMSL_FTM_LTE_RF.h"
#include "QMSL_WWAN\inc\QMSL_FTM_RF.h"
#include "QMSL_WWAN\inc\QMSL_FTM_TDSCDMA_NS.h"
#include "QMSL_WWAN\inc\QMSL_FTM_TDSCDMA_RF.h"
#include "QMSL_WWAN\inc\QMSL_FTM_WCDMA_NS.h"
#include "QMSL_WWAN\inc\QMSL_FTM_XO_CAL.h"
#include "QMSL_WWAN\inc\QMSL_NVTool.h"
#include "QMSL_WWAN\inc\QMSL_RFCAL_NVManager.h"
#include "QMSL_WWAN\inc\QMSL_CalParamPackUnpack_INTF.h"
#ifndef QMSL_FR72012
#else
#include "QMSL_WWAN\inc\QMSL_FTM_RF_NR5G_mmW.h"
#endif

//
// qmsl-lib-internal_extensions
//
//#include "QMSL_InternalExtensions\QMSL_InternalExtensions.h"
