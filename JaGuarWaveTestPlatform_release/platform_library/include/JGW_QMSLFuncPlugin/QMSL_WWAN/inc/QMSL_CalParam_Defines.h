/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_WWANincQMSL_CalResult_Defines.h
 * $DateTime: 2021/12/20 23:00:50 $
 *
 * DESCRIPTION: Calibration parameter definitions
 ******************************************************************************
 *
 * Copyright (c) 2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#pragma once
#ifndef QMSL_LINUX
    #include "..\..\QMSL_inc\QLib_Defines.h"
#else
	#error code not present
#endif	

const int kMaxNumCCs = 16;
const int kMaxNumModTypes = 2;
const int kMaxNumGainStates = 32;
const int kMaxNumRGIs = 64;
const int kMaxFreqCompNumRfFreqs = 32;
const int kMaxFreqCompNumBbOffets = 64;
const int kMaxNumLinearizerRegions = 10;
const int kMaxNumNodes = 200;
const int kRfcMaxAntennaGroupsPerRfTrxDevice = 2;
const int kRfcMaxRfTrxDevices = 8;

//Template function to compare contents of two arrays
//Used for AutoPack validation
template <class T, class R, size_t N, size_t M> bool arrCompare(T(&arr1)[N], uint16 arr1_Size, R(&arr2)[M], uint16 arr2_Size)
{
   if (arr1_Size != arr2_Size) return false;

   for (uint32 index = 0; index != arr1_Size; index++)
      if (arr1[index] != arr2[index])
         return false;

   return true;
}

/******************************************************************************
                  Packet Datatypes
*******************************************************************************/

#pragma region COMMON

typedef struct ftm_rfcal_NR5GMMW_Options
{
   ftm_rfcal_NR5GMMW_Options()
   {
      executeImmediate = false;

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif
   }

   bool executeImmediate;

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif
} ftm_rfcal_NR5GMMW_Options;

typedef struct ftm_rfcal_Attributes
{
   uint16 band;
   uint16 sub_band;
   int16 techEnum;
} ftm_rfcal_Attributes;

typedef struct ftm_rfcal_CalType
{
   ftm_rfcal_CalType()
   {
      Enable_RxAmpMismatchCal = false;
      Enable_RxRfAbsoluteGainCal = false;
      Enable_RxRfRelativeGainCal = false;
      Enable_TxMismatch = false;
      Enable_TxAptLinearizer = false;
      Enable_RxIfDroopCal = false;
      Enable_TxPdetVariance = false;
      Enable_TxFmcwCal = false;

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif
   }

   // Rx
   bool Enable_RxAmpMismatchCal;
   bool Enable_RxRfAbsoluteGainCal;
   bool Enable_RxRfRelativeGainCal;
   bool Enable_RxIfDroopCal;

   // Tx
   bool Enable_TxMismatch;
   bool Enable_TxAptLinearizer;
   bool Enable_TxPdetVariance;
   bool Enable_TxFmcwCal;

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif
} ftm_rfcal_CalType;

typedef struct ftm_rfcal_Common
{
   ftm_rfcal_Common()
   {
#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif
      Skip_NV_Write = false;
   }

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif

   bool En_Debug_Mode;
   uint32 timestamp;
   uint32 Ref_Frequency;
   uint16 num_Freqs;
   uint16 num_Ccs;
   uint16 num_OverrideMIMOChains;
   uint16 num_override_rf_trx_devices;
   uint16 rf_trx_device_override_list[kRfcMaxRfTrxDevices];
   uint32 Frequency_Per_CC_List[kMaxNumCCs];
   uint16 RB_Count_Per_CC_List[kMaxNumCCs];
   uint16 RB_Start_Per_CC_List[kMaxNumCCs];
   uint32 BW_Per_CC_kHz_List[kMaxNumCCs];
   uint32 SCS_Per_CC_List[kMaxNumCCs];
   uint16 MIMO_Chain_Override_List[kRfcMaxAntennaGroupsPerRfTrxDevice];
   bool Skip_NV_Write;

} ftm_rfcal_Common;

#pragma endregion

/************************ Rx items ****************************/

#pragma region RX

typedef struct ftm_rfcal_NR5GMMW_RxAmpMismatch
{
   uint16 num_GainStates;
   uint16 Gain_State_List[kMaxNumGainStates];
} ftm_rfcal_NR5GMMW_RxAmpMismatch;

typedef struct ftm_rfcal_NR5GMMW_RxAmpMismatchLimits
{
   int32 Max_Gain_dB10;
   int32 Min_Gain_dB10;
   int32 Max_Delta_Limit_dB10;
} ftm_rfcal_NR5GMMW_RxAmpMismatchLimits;

typedef struct ftm_rfcal_NR5GMMW_RxAbsoluteGain
{
   uint16 num_GainStates;
   uint16 Gain_State_List[kMaxNumGainStates];
} ftm_rfcal_NR5GMMW_RxAbsoluteGain;

typedef struct ftm_rfcal_NR5GMMW_RxRelativeGain
{
   uint16 num_GainStates;
   uint16 num_RGIs;
   uint16 Gain_State_List[kMaxNumGainStates];
   uint16 Rgi_List[kMaxNumRGIs];
   uint8 num_char_gain_states;
   uint8 char_gain_states[kMaxNumGainStates];
} ftm_rfcal_NR5GMMW_RxRelativeGain;

typedef struct ftm_rfcal_NR5GMMW_RxRfGainLimits
{
   uint8 num_GainStates;
   uint8 Gain_State_List[kMaxNumGainStates];
   int16 Min_Abs_Limit_dB10_List[kMaxNumGainStates];
   int16 Max_Abs_Limit_dB10_List[kMaxNumGainStates];
} ftm_rfcal_NR5GMMW_RxRfGainLimits;

typedef struct ftm_rfcal_NR5GMMW_RxIfDroop
{
   uint32 AGC_Meas_Duration;
   uint32 Step_Duration;
} ftm_rfcal_NR5GMMW_RxIfDroop;

typedef struct ftm_rfcal_NR5GMMW_RxIfDroopLimits
{
   ftm_rfcal_NR5GMMW_RxIfDroopLimits() : ripple_limit_db10(SHRT_MAX) {};

   int16 Min_RxIfDroop_Limit_dB10;
   int16 Max_RxIfDroop_Limit_dB10;

   //Optional field
   //Set to SHRT_MAX or skip serialization of this field, to deactivate the Spur check algorithm
   int16 ripple_limit_db10; 
} ftm_rfcal_NR5GMMW_RxIfDroopLimits;

#pragma endregion

/************************ Tx items ****************************/

#pragma region TX

typedef struct ftm_rfcal_NR5GMMW_TxMismatch
{
   uint16 Rgi;
} ftm_rfcal_NR5GMMW_TxMismatch;

typedef struct ftm_rfcal_NR5GMMW_TxMismatchLimits
{
   int16 Min_Power_Limit_db10;
   int16 Max_Delta_Limit_dB10;
} ftm_rfcal_NR5GMMW_TxMismatchLimits;

typedef struct ftm_rfcal_NR5GMMW_LinearizerRegion
{
   uint16 Measurement_Type;
   uint16 Gain_State;
   uint16 numRGIs;
   uint16 Rgi_List[kMaxNumRGIs];

   //Operator overload for AutoPack Validation
   inline bool operator != (const ftm_rfcal_NR5GMMW_LinearizerRegion& rhs) const
   {
      return(Measurement_Type != rhs.Measurement_Type &&
             Gain_State != rhs.Gain_State &&
             numRGIs != rhs.numRGIs &&
             !arrCompare(Rgi_List, numRGIs, rhs.Rgi_List, rhs.numRGIs));
   }

} ftm_rfcal_NR5GMMW_LinearizerRegion;

typedef struct ftm_rfcal_NR5GMMW_TxLinearizer
{
   uint16 numModTypes;
   uint16 Modulation_Type_List[kMaxNumModTypes];
   uint16 numLinearizerRegions;
   ftm_rfcal_NR5GMMW_LinearizerRegion linearizerRegion[kMaxNumLinearizerRegions];
} ftm_rfcal_NR5GMMW_TxLinearizer;

typedef struct ftm_rfcal_NR5GMMW_TxLinearizerLimits
{
   int16 Max_Power_limit;
   int16 Min_Power_limit;
   int16 Min_Delta_Power_limit;
} ftm_rfcal_NR5GMMW_TxLinearizerLimits;

typedef struct ftm_rfcal_NR5GMMW_TxPdetVarianceLimits
{
   int32 Min_Limit;
   int32 Max_Limit;
} ftm_rfcal_NR5GMMW_TxPdetVarianceLimits;

typedef struct ftm_rfcal_NR5GMMW_TxFmcwCal
{
   int16 Target_Pwr;
} ftm_rfcal_NR5GMMW_TxFmcwCal;

typedef struct ftm_rfcal_NR5GMMW_TxFmcwCalLimits
{
   int32 Cbl_Upper_Limit;
   int32 Cbl_Lower_Limit;
} ftm_rfcal_NR5GMMW_TxFmcwCalLimits;

#pragma endregion

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif

#pragma region NODE

typedef struct ftm_rfcal_Node
{
   ftm_rfcal_Node()
   {
      attributes = NULL;
      calType = NULL;
      common = NULL;
      rxAmpMismatch = NULL;
      rxAmpMismatchLimits = NULL;
      rxAbsoluteGain = NULL;
      rxRelativeGain = NULL;
      rxRfGainLimits = NULL;
      txMismatch = NULL;
      txMismatchLimits = NULL;
      txLinearizer = NULL;
      txLinearizerLimits = NULL;
      rxIfDroop = NULL;
      rxIfDroopLimits = NULL;

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif
   }

   ftm_rfcal_Attributes* attributes;
   ftm_rfcal_CalType* calType;
   ftm_rfcal_Common* common;

   ftm_rfcal_NR5GMMW_RxAmpMismatch* rxAmpMismatch;
   ftm_rfcal_NR5GMMW_RxAmpMismatchLimits* rxAmpMismatchLimits;
   ftm_rfcal_NR5GMMW_RxAbsoluteGain* rxAbsoluteGain;
   ftm_rfcal_NR5GMMW_RxRelativeGain* rxRelativeGain;
   ftm_rfcal_NR5GMMW_RxRfGainLimits* rxRfGainLimits;

   ftm_rfcal_NR5GMMW_TxMismatch* txMismatch;
   ftm_rfcal_NR5GMMW_TxMismatchLimits* txMismatchLimits;
   ftm_rfcal_NR5GMMW_TxLinearizer* txLinearizer;
   ftm_rfcal_NR5GMMW_TxLinearizerLimits* txLinearizerLimits;
   ftm_rfcal_NR5GMMW_RxIfDroop* rxIfDroop;
   ftm_rfcal_NR5GMMW_RxIfDroopLimits* rxIfDroopLimits;

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif

} ftm_rfcal_Node;

#pragma endregion
