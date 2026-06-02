/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_WWAN/inc/QMSL_BandHelper.h#15 $
 * $DateTime: 2021/12/20 23:00:50 $
 *
 * DESCRIPTION: QMSL_Bandhelper
 ******************************************************************************
 *
 * Copyright (c) 2014-2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#include "QLib_Defines.h"
#if !defined(_QLIB_BandHelper_H)
#define _QLIB_BandHelper_H

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

/**
   Establish whether the library is being compiled into a DLL (exporting),
   or being included from a client (importing)

   When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API __declspec(dllimport)
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
//#define QLIB_API __cdecl
#endif


#ifdef __cplusplus
extern "C" {
#endif

   /******************************************************************************
                     Band Class Helper
   *******************************************************************************/

   /******************************************************************************/
   /**
      Calculate the downlink (forward) frequency

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iChannel, downlink channel number

      \return frequencey in MHz.  0 is returned for invalid band class or channel number

   *******************************************************************************/
   QLIB_API double QLIB_Band_Helper_CalculateDLFrequency(unsigned short iBand, unsigned long iChannel);

   /******************************************************************************/
   /**
      Calculate the uplink (reverse) frequency

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iChannel, uplink channel number

      \return frequencey in MHz.  0 is returned for invalid band class or channel number

   *******************************************************************************/
   QLIB_API double QLIB_Band_Helper_CalculateULFrequency(unsigned short iBand, unsigned long iChannel);

   /******************************************************************************/
   /**
      Calculate the downlink (reverse) channel number

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iUpLinkd_Channel, uplink channel number

      \return Downlink channel number.  0 is returned for invalid band class or uplink channel number

   *******************************************************************************/
   QLIB_API unsigned long QLIB_Band_Helper_CalculateDLChannel(unsigned short iBand, unsigned long iUpLink_Channel);

   /******************************************************************************/
   /**
      Calculate the UL channel number according to band and bandwidth

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iRange, Channel range enumeration.  See CHANNEL_RANGE_ENUM in QLib_Defines.h

      \param bandwidth, channel bandwidth, only required for LTE

      \return Uplink channel number. 65535 is returned for invalid band class or uplink channel number

   *******************************************************************************/
   QLIB_API unsigned long QLIB_Band_Helper_CalculateULChanBW(unsigned short usBand, unsigned int iRange, unsigned int iChBW);

   /******************************************************************************/
   /**
      Calculate the DL channel number according to band and bandwidth

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \param iRange, Channel range enumeration.  See CHANNEL_RANGE_ENUM in QLib_Defines.h

      \param bandwidth, channel bandwidth, only required for LTE

      \return Downlink channel number. 65535 is returned for invalid band class or downlink channel number

   *******************************************************************************/
   QLIB_API unsigned long QLIB_Band_Helper_CalculateDLChanBW(unsigned short usBand, unsigned int iRange, unsigned int iChBW);

   /******************************************************************************/
   /**
      Convert tech band to rf mode

      \param iTech. Tech nuber TECHNOLOGY_FAMILY_TYPE

      \param iBand, Band number

      \param iSubband, Subband number

      \return rfmode

   *******************************************************************************/
    QLIB_API  FTM_RF_Mode_Enum QLIB_Band_Helper_Techband_to_Rfmode(int iTech, int iBand, int iSubband);

   /******************************************************************************/
   /**
      Convert band class to rf mode

      \param iBand, Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

      \return rfmode

   *******************************************************************************/
    QLIB_API  FTM_RF_Mode_Enum QLIB_Band_Helper_Bandclass_to_Rfmode(BAND_CLASS_ENUM bandClass);

   /******************************************************************************/
   /**
      Convert rfMode to technology family

      \param rfMode, rf mode

      \return technology enum 

   *******************************************************************************/
    QLIB_API  int QLIB_Band_Helper_Rfmode_to_TechnologyFamily(FTM_RF_Mode_Enum rfMode);

   /******************************************************************************/
   /**
      Convert rfMode to band number

      \param rfMode, rf mode

      \return band number 

   *******************************************************************************/
    QLIB_API  int QLIB_Band_Helper_Rfmode_to_Bandnumber(FTM_RF_Mode_Enum rfMode);

   /******************************************************************************/
   /**
      Convert rfMode to band number

      \param rfMode, rf mode

      \return Band class enumeration.  See BAND_CLASS_ENUM in QLib_Defines.h

   *******************************************************************************/
    QLIB_API  BAND_CLASS_ENUM QLIB_Band_Helper_Rfmode_to_Bandclass(FTM_RF_Mode_Enum rfMode);

   /******************************************************************************/
   /**
      Convert UL channel to DL channel (or DL channel to UL channel)

      \param rfMode, rf mode

      \param channel

      \param IsULChanToDLChan, Set true if converting UL channel to DL channel

      \return DL channel or UL Channel. Failure: -1.

   *******************************************************************************/   
    QLIB_API  int QLIB_Band_Helper_Channel_to_Channel(FTM_RF_Mode_Enum rfMode, int orgChan, unsigned char isULChanToDLChan);

   /******************************************************************************/
   /**
      Convert UL Frequency to UL channel (or DL Frequency to DL channel)

      \param rfMode, rf mode

      \param freqMHz, frequency in MHz

      \param isUplink, Set true if converting UL Frequency to UL channel.

      \return UL channel or DL Channel. Failure: -1.

   *******************************************************************************/   
    QLIB_API  int QLIB_Band_Helper_Frequency_to_Channel(FTM_RF_Mode_Enum rf_mode, double freqMHz, unsigned char isUplink);

   /******************************************************************************/
   /**
      Convert UL Channel to UL Frequency (or DL Channel to DL Frequency)

      \param rfMode, rf mode

      \param channel

      \param isUplink, Set true if converting to UL Frequency.

      \return UL frequency in MHz or DL frequency in MHz . Failure: -1.

   *******************************************************************************/   
    QLIB_API  double QLIB_Band_Helper_Channel_to_Frequency(FTM_RF_Mode_Enum rfMode, int channel, unsigned char isUplink);

   /******************************************************************************/
   /**
      Convert UL Frequency to UL Frequency (or DL Frequency to DL Frequency)

      \param rfMode, rf mode

      \param freqMHz, frequency in MHz

      \param isUplink, Set true if converting to UL Frequency.

      \return UL frequency in MHz or DL frequency in MHz . Failure: -1.

   *******************************************************************************/  
    QLIB_API  double QLIB_Band_Helper_Frequency_to_Frequency(FTM_RF_Mode_Enum rfMode, double freqMHz, unsigned char isULFreqToDLFreq);

    /******************************************************************************/
    /**
       QLIB_Band_Helper_GetMinMaxFrequencies
       Get frequency range given a band

       \param iTech. Tech nuber TECHNOLOGY_FAMILY_TYPE

       \param iBand, Band number. Note subband is not supported, check your RFC for frequency ranges of each subbaands

       \param isUplink, Set true if converting to UL Frequency.

       \param max_freq_in_MHz, frequency in MHz

       \param min_freq_in_MHz, frequency in MHz

       \return true if success. false if failure

    *******************************************************************************/
    QLIB_API unsigned char QLIB_Band_Helper_GetMinMaxFrequencies(int tech, int bandNumber, boolean isUL, double * max_freq_in_MHz, double * min_freq_in_MHz);

    QLIB_API unsigned long QLIB_Band_Helper_Band_Tech_MAX_BW(unsigned short iBand, unsigned short iTech, unsigned short iSubband, int scs);

    QLIB_API unsigned long QLIB_Band_Helper_Band_Tech_MIN_BW(unsigned short iBand, unsigned short iTech, unsigned short iSubband, int scs);

    //   FTM_RF_Mode_Enum CBandHelper::BandNumberToRFMode(int bandNumber, TECHNOLOGY_FAMILY_TYPE tech)
#ifdef __cplusplus
}   // extern "C"
#endif

#endif   // defined(_QLIB_FBandHelper_H)
