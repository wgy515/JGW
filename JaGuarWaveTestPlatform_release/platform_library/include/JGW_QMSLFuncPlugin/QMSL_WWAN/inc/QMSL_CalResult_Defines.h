/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_WWANincQMSL_CalResult_Defines.h
 * $DateTime: 2021/12/20 23:00:50 $
 *
 * DESCRIPTION: Calibration Result structure definitions
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

const unsigned int kMaxNumResultsperNode = 50;
const unsigned int kAmplitudeMismatchAntennaListMaxSize = 8;
const unsigned int kLinTableEntryListMaxSize = 64;
const unsigned int kRxGainPerStateMaxSize = 16;
const unsigned int kRxCarrierFrequencyListMaxSize = 16;
const unsigned int kRxGainVsFreqListMaxSize = 16;
const unsigned int kRxGainStateListMaxSize = 16;
const unsigned int kTxFmcwMaxLayer = 2;
const unsigned int kMaxMpeBeamPair = 8;
const unsigned int kRxIfDroopGainOffset = 32;
const unsigned int kRxIfDroopFreqListMaxSize = 16;

typedef enum FtmSequenceStatus
{
   kSequenceFail = 0,
   kSuccess = 1,
   kResultLimitFail = 2,
   kTimeout = 3,
   kMaxSequenceStatus = 0xFF,
} FtmSequenceStatus;

typedef enum FtmRfCalResultType
{
   kMmwTxAmplitudeMismatch = 0,
   kMmwTxLinearizerResponse = 1,
   kMmwRxMismatchResponse = 2,
   kMmwRxGainResponse = 3,

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif

   kMmwTxPdetVarianceResponse = 5,
   kMmwRxIfDroopResponse = 6,
   kMmwTxFmcwCalResponse = 7,
   kMaxResultType = 0xFFFF,
} FtmRfCalResultType;

#pragma region RESULT_DEFS

typedef struct FtmCalibrationRadioConfig
{
   uint16 technology;
   uint16 band;
   uint8 sub_band;
   uint32 ref_frequency;
} FtmCalibrationRadioConfig;

typedef struct FtmCalibrationMmwPathConfig
{
   uint8 rf_trx_id;
   uint8 antenna_group;
   rfm_path_type rfm_path;
   size_t mod_type;
   uint8 rx_mode;
} FtmCalibrationMmwPathConfig;

typedef struct FtmCalibrationResult
{
   uint32 nv_id;
   FtmSequenceStatus sequence_status;
   FtmRfCalResultType result_type;
   FtmCalibrationRadioConfig ftm_cal_radio_config;
   FtmCalibrationMmwPathConfig ftm_cal_mmw_path_config;
   void* limits;
   void* nv_result;
   void* result;
} FtmCalibrationResult;

typedef struct FtmCalibrationResults
{
   uint32 time_stamp;
   uint32 num_results;
   FtmCalibrationResult* ftm_cal_result;
} FtmCalibrationResults;

#pragma endregion

#pragma region NV_DEFS

typedef struct NvVariant_AmplitudeMismatchData
{
   uint8 antennaGroupId;
   uint16 antennaCount;
   int16 amplitude[kAmplitudeMismatchAntennaListMaxSize];
} NvVariant_AmplitudeMismatchData;

typedef struct NvVariant_LinTableEntry
{
   uint16 rgi;
   uint16 bias;
   int16 power;
} NvVariant_LinTableEntry;

typedef struct NvVariant_LinTable_V3
{
   uint8 antennaGroupId;
   uint16 entryCount;
   uint16 modType;
   uint32 carrierFrequency_kHz;
   uint32 intermediateFrequency_kHz;
   NvVariant_LinTableEntry linTable[kLinTableEntryListMaxSize];
} NvVariant_LinTable_V3;

typedef struct NvVariant_RxRfGainPerState_V2
{
   uint8 gainState;
   int16 gain_dB10;
} NvVariant_RxRfGainPerState_V2;

typedef struct NvVariant_RxGainTablePerAntennaGroup_V2
{
   uint8 rx_mode;
   uint8 antennaGroupId;
   uint32 carrierFrequency_kHz;
   uint32 intermediateFrequency_kHz;
   uint8 gainStateCount;
   NvVariant_RxRfGainPerState_V2 rxRfGainVsFreqPerGainState[kRxGainStateListMaxSize];
} NvVariant_RxGainTablePerAntennaGroup_V2;

typedef struct NvVariant_RxGainMismatchDataPerAntennaGroup
{
   uint8 antennaGroupId;
   uint16 antennaCount;
   int16 gain_delta_dB10[kAmplitudeMismatchAntennaListMaxSize];
} NvVariant_RxGainMismatchDataPerAntennaGroup;

typedef struct NvVariant_PdetVarianceData
{
   uint8 antennaGroupId;
   int16 pdet_variance;
} NvVariant_PdetVarianceData;

typedef struct NvVariant_RxIfDroopFrequency
{
   uint32 ifFreqKhz;
   uint8 numBBOffset;
   int32 bbDroopOffsetListHz[kRxIfDroopGainOffset];
   int16 gainBBOffsetdB10[kRxIfDroopGainOffset];
} NvVariant_RxIfDroopFrequency;

typedef struct NvVariant_RxIfDroopTablePerAntennaGroup
{
   uint8 antennaGroupId;
   uint8 rxMode;
   uint8 numFreq;
   NvVariant_RxIfDroopFrequency ifFrequencyList[kRxIfDroopFreqListMaxSize];
} NvVariant_RxIfDroopTablePerAntennaGroup;

typedef struct FtmRfCalNr5gMmwRxIfDroopSpurResultperFrequency
{
   uint8 num_entries;
   int16 spur_level[kRxIfDroopGainOffset];
}FtmRfCalNr5gMmwRxIfDroopSpurResultperFrequency;

typedef struct FtmRfCalNr5gMmwRxIfDroopSpurResult
{
   uint8 num_freq;
   FtmRfCalNr5gMmwRxIfDroopSpurResultperFrequency spur_level_per_frequency[kRxIfDroopFreqListMaxSize];
}FtmRfCalNr5gMmwRxIfDroopSpurResult;

typedef struct
{
   uint16 beam_id[kTxFmcwMaxLayer];
   uint8 rxagc[kTxFmcwMaxLayer];
   uint8 rx_gs_snr_pk[kTxFmcwMaxLayer];
   int32 cable_len_hz[kTxFmcwMaxLayer];
   uint8 mpe_rai;
   uint8 tx_rgi;
   uint8 rgi_snr_pk;
   int16 trgt_pwr;
   int16 est_pwr;
   int16 est_pwr_snr_pk;
} NvVariant_TxFmcwCal_Entry;

typedef struct
{
   uint8 num_beam_pairs;
   NvVariant_TxFmcwCal_Entry beamData[kMaxMpeBeamPair];
} NvVariant_TxFmcwCalData;

#ifdef QDART_FEATURE_NR5G_MMW
#error code not present
#endif

#pragma endregion
