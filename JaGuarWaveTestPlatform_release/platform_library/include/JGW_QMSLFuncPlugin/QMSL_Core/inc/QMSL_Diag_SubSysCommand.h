/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Core/inc/QMSL_Diag_SubSysCommand.h#11 $
 * $DateTime: 2022/01/02 19:24:55 $
 *
 * DESCRIPTION: QMSL_Diag_SubSysCommand
 ******************************************************************************
 *
 * Copyright (c) 2014-2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#if !defined(_QMSL_DIAG_SUBCOMMAND_H)
#define _QMSL_DIAG_SUBCOMMAND_H

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
                  coreBSP/Diag service 
   *******************************************************************************/

   /******************************************************************************/
   /**
      Set/Override MPSS diag timer on Diag service to non-deferrable/deferrable

	  When the device/image is reset the diag timer settings will return to their default settings, as the setting is not persistent in the MPSS image. It can also be toggled back and forth using the command if needed. 

	  By setting the value to non-deferable, it will avoid any delay of MPSS async log sent to the host.  This API can be called before any RFVFS FTM sequencing to avoid any potential FTM async log (0x117c) processing delay due to modem sleep. 

	  This command does not affect delayed req/response behavior.

	  New taraget such as SM8350/8450 should support this command.

	  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	  \param bNonDeferrable = 0; # 0 is non-deferrable, 1 = deferrable (Diag default state for MPSS subsystem)

   *******************************************************************************/

   QLIB_API unsigned char QLIB_DIAG_SET_MPSS_NON_DEFERRABLE_TIMER(
	    HANDLE hResourceContext, unsigned char bNonDeferrable);

   /******************************************************************************
                  Diagnostic Subsystem (80-V1294-10)
   *******************************************************************************/

   /******************************************************************************/
   /**
   Send a "GPS Configure test paraemters" message, usuall used for a "GPS Cold start,"
   which is used in the GPS PERL scripts for GPS testing.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iDeleteAlm = 1; # 1 is to delete, 0 =no action
   \param iDeleteEph = 1; # 1 is to delete, 0 =no action
   \param iDeletePos = 1; # 1 is to delete, 0 =no action
   \param iDeleteTime = 1; # 1 is to delete, 0 =no action
   \param iDeleteIono= 1; # 1 is to delete, 0 =no action
   \param iTimeUnc = 0; #Inject Time uncertainity. Only applicable if delete_time value is 0
   \param iPosUnc = 0;  #Inject Position Uncertainity. Only applicable if delete pos is 0
   \param iTimeOffset = 0; #Add a time offset. Only applicable if delete_time value is 0
   \param iPosOffset = 0;  #Add a position offset. Only applicable if delete_pos value is 0


   GPS PERL script being emulated is pd_coldstart.pl, the section:

      # Build a diagnostic request for GPS_SET_TEST_MODE_PARAMETERS
      $req_testparm = new PD_COLDSTART;
      $req_testparm->{"cmd"}          = 75; #Diag Cmd
      $req_testparm->{"id"}           = 13; #Diag ID
      $req_testparm->{"sub_cmd"}      = 18; #Diag Subsystem ID
      $req_testparm->{"delete_alm"}  = 1; # 1 is to delete, 0 =no action
      $req_testparm->{"delete_eph"}  = 1; # 1 is to delete, 0 =no action
      $req_testparm->{"delete_pos"}  = 1; # 1 is to delete, 0 =no action
      $req_testparm->{"delete_time"}  = 1; # 1 is to delete, 0 =no action
      $req_testparm->{"delete_iono"}  = 1; # 1 is to delete, 0 =no action
      $req_testparm->{"time_unc"}  = 0; #Inject Time uncertainity. Only applicable if delete_time value is 0
      $req_testparm->{"pos_unc"}  = 0;  #Inject Position Uncertainity. Only applicable if delete pos is 0
      $req_testparm->{"time_offset"}  = 0; #Add a time offset. Only applicable if delete_time value is 0
      $req_testparm->{"pos_offset"}  = 0;  #Add a position offset. Only applicable if delete_pos value is 0
      $req_testparm->{"client_id"}  = 0; #Not used


   \return true if successful, false if fail.

   \warning None.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_GPS_SET_TEST_MODE_PARAMS(
      HANDLE hResourceContext, unsigned char   iDeleteAlm,  unsigned char   iDeleteEph,
      unsigned char   iDeletePos,  unsigned char   iDeleteTime,
      unsigned char   iDeleteIono, unsigned long iTimeUnc,
      unsigned long iPosUnc, unsigned long iTimeOffset, unsigned long iPosOffset
   );


   /******************************************************************************/
   /**
   Documentation for this is still pending as part of the Diag ICD.

   Flushes the diagnostic logpacket/event/message queue. All queued packets/messages are sent regardless of queue size or timer expiry

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_FLUSH_BUFFER
   (
      HANDLE hResourceContext
   );

   /******************************************************************************
                  Handset Diag
   *******************************************************************************/

   /******************************************************************************/
   /**
   Handset Diagnostic ICD, 3.1 HS_DISPLAY_GET_PARAMS_CMD Request/Response

     This command is used to request device-specific information (display).

      \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
      \param iDisplayID = input, the display # to get information for, 0 = primary, 1 = secondary

      \param piDisplayHeight = output, # of veritcal pixels  (description differs from ICD document rev B)
      \param piDisplayWidth = output, # of horizontal pixels (description differs from ICD document rev B)
      \param piPixelDepth = output, bits per pixel
      \param piPanelOrientation = output, 0 - Portrait, 1 - Landscape


   \return true if response packet was successfully returned, false if fail for any reason.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_DIAG_HS_DISPLAY_GET_PARAMS_CMD(
      HANDLE hResourceContext,
      unsigned char iDisplayID, unsigned short* piDisplayHeight, unsigned short* piDisplayWidth,
      unsigned char* piPixelDepth, unsigned char* piPanelOrientation
   );

   /******************************************************************************/
   /**
   Handset Diagnostic ICD, 3.3 HS_DISPLAY_CAPTURE_DELAYED_CMD 2 Request/Response

      This command requests the target to capture the display buffer region specified by
      [(STARTING_ROW, STARTING_COLUMN),(HEIGHT,WIDTH)] coordinates to a file. The
      file is written in Windows BMP 24-bit format to the EFS. Specifying [(0,0),(0xFFFF,0xFFFF)] would
      7 capture the whole display.

      This version uses the Diagnostic subsystem 2, so diag is not blocked while the
      command executes.  On the QLIB side, the function will block, and not return
      until the embedded side has completed.

      \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
      \param iDisplayID = input, the display # to get information for, 0 = primary, 1 = secondary

      \param iStartingRow = input, Upper left-hand horizontal coordinate

      \param iStartingCol = input, Upper left-hand vertical coordinate

      \param iHeight = input, Number of rows from the STARTING_ROW; according to BMP format,
                              this must be a multiple of 4
      \param iWidth = input, Number of columns from the STARTING_COLUMN; according to BMP format,
                             this must be a multiple of 4

      \param sFileName = input, NULL-terminated name of file to be written to in the EFS;
                         after the capture is completed successfully, this file will
                         be in 24-bit Windows BMP format

   \return true if response packet was successfully returned, false if fail for any reason.

   \warning Not Currently Implemented

   *******************************************************************************/
   QLIB_API unsigned char QLIB_HS_DISPLAY_CAPTURE_DELAYED_CMD
   (
      HANDLE hResourceContext,
      unsigned char iDisplayID, unsigned short iStartingRow, unsigned short iStartingCol,
      unsigned short iHeight, unsigned short iWidth, char* sFileName
   );
   /******************************************************************************/
   /**
   QLIB_DIAG_TOOLSDIAG_EXT_MOBILE_ID

   This command requests the target to get chipset id and chipset family
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param uiVersion = output, version 2 may be used

   \param uiChip_family = output, chip family
   \param uiChip_info_id = output, chip info id

   \return true if response packet was successfully returned, false if fail for any reason.

   ChipInfoId and familyID enum can be found in MPSS source code ( core\api\systemdrivers\ChipInfoDefs.h )
    
   typedef enum
   {
     CHIPINFO_ID_UNKNOWN             = 0,
     CHIPINFO_ID_MDM1000             = 1,
  
     ...
     ...
  
     CHIPINFO_NUM_IDS,
     CHIPINFO_ID_32BITS = 0x7FFFFFF
   } ChipInfoIdType;

   typedef enum
   {
     CHIPINFO_FAMILY_UNKNOWN         = 0,
     CHIPINFO_FAMILY_MSM6246         = 1,
     CHIPINFO_FAMILY_MSM6260         = 2,
  
     ...
     ...
  
     CHIPINFO_NUM_FAMILIES,
     CHIPINFO_FAMILY_32BITS = 0x7FFFFFF
   } ChipInfoFamilyType

   *******************************************************************************/

   QLIB_API unsigned char QLIB_DIAG_TOOLSDIAG_EXT_MOBILE_ID(HANDLE hResourceContext, unsigned int * uiVersion, unsigned int * uiChip_family, unsigned int * uiChip_info_id);

#ifdef __cplusplus
}   // extern "C"
#endif

#endif   // defined(_QLIB_DIAG_SUBCOMMAND_H)

