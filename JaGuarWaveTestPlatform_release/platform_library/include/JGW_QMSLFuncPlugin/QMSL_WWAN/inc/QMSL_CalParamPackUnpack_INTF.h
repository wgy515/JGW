/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_WWAN/inc/QMSL_CalParamPackUnpack_INTF.h#10 $
 * $DateTime: 2021/08/12 11:15:06 $
 *
 * DESCRIPTION: Public QMSL API for managing CPS interface packet structures
 ******************************************************************************
 *
 * Copyright (c) 2020-2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#pragma once

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
 //#define QLIB_API __cdecl
#endif

#include "QMSL_CalParam_Defines.h"
#include "QMSL_CalResult_Defines.h"

#ifdef __cplusplus
extern "C" {
#endif

   /******************************************************************************/
   /**

   \return 1 if successful, 0 if unsuccessful

   \warning - This is a beta API and is subject to change

   *******************************************************************************/   

   //Translates input structure ftm_rfcal_NR5GMMW_Options/ftm_rfcal_Node (pChild) into a sereialized AutoPack buffer (ap)
   QLIB_API unsigned char QLIB_FTM_CALPARAM_AUTOPACK(void* ap, uint32 datatype, void* pChild);
   
   //Extracts the ftm_rfcal_NR5GMMW_Options/ftm_rfcal_Node structure (pChild) from the serialized AutoUnpack buffer (au)
   QLIB_API unsigned char QLIB_FTM_CALPARAM_AUTOUNPACK(void* au, void* pChild);

   //Extracts the FtmCalibrationResults structure (pChild) from the serialized AutoUnpack buffer (au)
   QLIB_API unsigned char QLIB_FTM_CALRESULT_AUTOUNPACK(void* au, void* pChild);
   
   //Unified API that performs the following:
   //1. Calls QLIB_FTM_CALPARAM_AUTOPACK() to AutoPack (serialize) ftm_rfcal_NR5GMMW_Options/ftm_rfcal_Node structures for diag transport
   //2. Calls QLIB_FTM_RFCAL_CONFIG() to execute mmW RF calibration
   //3. Calls QLIB_FTM_CALRESULT_AUTOUNPACK() to AutoUnpack (deserialize) the response bystream into FtmCalibrationResults structure
   QLIB_API unsigned char QLIB_FTM_CPS(HANDLE hResourceContext, ftm_rfcal_NR5GMMW_Options* options, ftm_rfcal_Node* node, FtmCalibrationResults* result);

   //Frees dynamic memory allocated within ftm_rfcal_Node object
   QLIB_API void QLIB_FREE_CPS_NODE(ftm_rfcal_Node* node);

   //Frees dynamic memory allocated within FtmCalibrationResults object
   QLIB_API void QLIB_FREE_CPS_RESULTS(FtmCalibrationResults* result);

#ifdef __cplusplus
}   // extern "C"
#endif
