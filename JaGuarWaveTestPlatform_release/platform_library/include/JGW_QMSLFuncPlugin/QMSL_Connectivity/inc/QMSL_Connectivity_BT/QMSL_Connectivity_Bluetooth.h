/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Connectivity/inc/QMSL_Connectivity_BT/QMSL_Connectivity_Bluetooth.h#68 $
 * $DateTime: 2021/12/03 02:39:59 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2014-2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#if !defined(_QLIB_BLUETOOTH_H)
#define _QLIB_BLUETOOTH_H

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
typedef unsigned char			byte;

#ifdef __cplusplus
extern "C" {
#endif


   /******************************************************************************
                     Bluetooth FTM
   *******************************************************************************/
  

    /**  
   Configures the Bluetooth chip ID

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param bluetoothChipID = set chip ID,
   bluetoothChipID = 0, for WCN2243 and WCN36xx
   bluetoothChipID = 3002, for AR3002 chipset
  
   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE( PHONE_MODE_BLUETOOTH ) must be called first

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SET_CHIP_ID
   ( HANDLE hResourceContext, int bluetoothChipID);
    /******************************************************************************/ 
   /**  
   Configures the Bluetooth logging mode

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param bluetoothLegacyLoggingEnable = if enabled uses legacy 0x117C log code,else uses Bluetooth log code 0x1366
   bluetoothLegacyLoggingEnable = 0, uses Bluetooth log code = 0x1366
   bluetoothLegacyLoggingEnable = 1, uses legacy FTM log code = 0x117C 
  
   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE( PHONE_MODE_BLUETOOTH ) must be called first

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SET_LegacyLoggingMode
   ( HANDLE hResourceContext, int bluetoothLegacyLoggingEnable);

    /******************************************************************************/ 
   /**  
   Enables the Bluetooth HCI event logging mode for QRCT , logCode = 0x1366

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
    
   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE( PHONE_MODE_BLUETOOTH ) must be called first

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_EnableLogging
   ( HANDLE hResourceContext);

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, Config XTAL FTrim Command

   \param iXtalFtrim= xtal ftrim value

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_ConfigXtalFTrim(HANDLE hResourceContext, unsigned char iCapValue);

     /******************************************************************************/
   /**

   BT Commands:  QLIB_FTM_BT_GET_BD_ADDR

   Get the BD address.

   \param macAddr = pointer to 6 byte array to hold the BD address.

   \return true if successful, false if failure
  
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_GET_BD_ADDR
   (
      HANDLE hResourceContext,
      unsigned char *bdAddr
   );

   /******************************************************************************/
   /**

   BT Commands:  QLIB_FTM_BT_SET_BD_ADDR

   Set the BD address.

   \param macAddr = pointer to 6 byte array holding the desired BD address.

   \return true if successful, false if failure
  
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SET_BD_ADDR
   (
      HANDLE hResourceContext,
      unsigned char* bdAddr
   );

   /******************************************************************************/
   /**
   BT Commands: QLIB_FTM_BT_SET_BOARD_ID

   Set the BOARD_ID.
   
   \param writeBoardID = pointer to 2 byte array holding the board ID.
   \param readBoardID = pointer to 2 byte array holding the board ID data written by this API for verification.

   \return true if successful, false if failure

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SET_BOARD_ID
   (
     HANDLE hResourceContext,
     unsigned char* writeBoardID,
     unsigned char* readBoardID
   );
   /******************************************************************************/
   /**
   BT Commands: QLIB_FTM_BT_GET_BOARD_ID

   Get the BD address.
   
   \param readBoardID = pointer to 2 byte array holding the boardID.
   
   \return true if successful, false if failure
   
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_GET_BOARD_ID
   (
     HANDLE hResourceContext,
     unsigned char* readBoardID
     );
   
   /******************************************************************************/
   /**
   BT Commands:  QLIB_FTM_BT_SET_PERSIST_ACCESS_OPT

   Set the BD address or Board ID.

   \param cfgType = 1 = Primary address, 2 = custom bd address 1 , 3 = custom bd address, 4 = Board ID

   \param data = pointer to 6 byte array holding the desired BD address or 2 bytes for Board ID

   \return true if successful, false if failure

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SET_PERSIST_ACCESS_OPT
   (
     HANDLE hResourceContext,
     unsigned char cfgType,
     unsigned char* data
   );

   /******************************************************************************/
   /**

   BT Commands:  QLIB_FTM_BT_GET_PERSIST_ACCESS_OPT

   Get the BD address or BoardID

   \param cfgType = 1 = Primary address, 2 = custom bd address 1 , 3 = custom bd address,  4 = Board ID, 5 = Get Output power calibration values

   \param data = pointer to 6 byte array holding the desired BD address or 2 bytes for Board ID

   \return true if successful, false if failure

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_GET_PERSIST_ACCESS_OPT
   (
     HANDLE hResourceContext,
     unsigned char cfgType,
     unsigned char* data
   );

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth Commands 1.2.1.1 User-specified HCI command

   Send a host controller interface (HCI) command to a device

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param pBuffer = pointer to a series of bytes that will be sent as an HCI command.
   \param iLength = number of bytes to send, from pBuffer

   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE( PHONE_MODE_BLUETOOTH ) must be called first

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_USER_CMD
   ( HANDLE hResourceContext, unsigned char* pBuffer, unsigned char iLength);
   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth,QLIB_FTM_BT_INQUIRY_CMD Command

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param inquiryData = inquiryData (Inquiry Command data contains : Lap Value , Inquiry Length and Number of Response)
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_INQUIRY_CMD
   (HANDLE hResourceContexts, unsigned char *inquiryData);
   
   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth,QLIB_FTM_BT_WRITE_INQUIRY_MODE Command

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param inquiryMode = Set the requested inquiry result mode,
  0x0 = Standard_Inquiry_Result_event_format
  0x1 = Inquiry_Result_format_with_RSSI
  0x2 = Inquiry_Result_with_RSSI_format_or_Extended_Inquiry_Result_format
  0x3 - 0xFF = Reserved.
   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_WRITE_INQUIRY_MODE(HANDLE hResourceContexts, unsigned int inquiryMode);
   /******************************************************************************/
   /** 

  Factory Test Mode Bluetooth, QLIB_FTM_BT_CREATE_CONNECTION Command

     \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
     \param BdAddress = BD Device address .
     \Param ConnectionHandler = to get the connection handler.
     \return true if successful, false if fail.

     \warning

     *******************************************************************************/
     QLIB_API unsigned char QLIB_FTM_BT_CREATE_CONNECTION
     (HANDLE hResourceContexts, unsigned char *BdAddress , int *pConnectionHandler);

   /******************************************************************************/

   /**

   Factory Test Mode Bluetooth, QLIB_FTM_BT_DISCONNECT_COMMAND Command

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param ConnectionHandler = Connection handler of connected device.
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
     QLIB_API unsigned char QLIB_FTM_BT_DISCONNECT_COMMAND
     (HANDLE hResourceContexts, int ConnectionHandler);

     /******************************************************************************/
   /**
   Factory Test Mode Bluetooth Commands to Set BT baudRate 

   Send a host controller interface (HCI) command to a device

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param BaudRateID = Baud Rate ID can be :
   Description:
   0x0 = BaudRate_115200_bps
   0x1 = BaudRate_57600_bps
   0x2 = BaudRate_38400_bps
   0x3 = BaudRate_19200_bps
   0x4 = BaudRate_9600_bps
   0x5 = BaudRate_230400_bps
   0x6 = BaudRate_250000_bps
   0x7 = BaudRate_460800_bps
   0x8 = BaudRate_500000_bps
   0x9 = BaudRate_720000_bps
   0xA = BaudRate_921600_bps
   0xB = BaudRate_1000000_bps
   0xC = BaudRate_125000_bps
   0xD = BaudRate_2000000_bps
   0xE = BaudRate_3000000_bps
   0xF = BaudRate_4000000_bps
   0x10 = BaudRate_1600000_bps
   0x11 = BaudRate_3200000_bps
   0x12 = BaudRate_3500000_bps
   \return true if successful, false if fail.
   \warning
   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SET_BAUDRATE(HANDLE hResourceContext, unsigned char BTbaudRateId);
   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, custom command for Vendor specific Voice LoopBack.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param =loopBackMode = value of LoopBack Mode.  [Description: 0x0 = Disable loopback mode, 0x1 = Local loopback at ACI,
                          0x2 = Local loopback at CODEC,
                            0x5 = Local loopback at BB, ]
   \param voicesetting = voice setting details.

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_VOICE_LOOPBACK(HANDLE hResourceContext, unsigned char loopBackMode, unsigned short voicesetting);
   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, custom command to activate Bluetooth mode

   Sends: 1) HCI Enable DUT, 2)HCI Write Scan Enable, 3) HCI Set Event Filter Conn 2

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param =pBuffer = pointer to a series of bytes that will be sent as an HCI command.
   \param iLength = number of bytes to send, from pBuffer

   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE( PHONE_MODE_BLUETOOTH ) must be called first

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_Enable_Bluetooth( HANDLE hResourceContext );

    /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, Write Scan Enable Command
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iScanEnableOption=                               
                              0x00 No Scans enabled. Default.
                              0x01 Inquiry Scan enabled,Page Scan disabled.
                              0x02 Inquiry Scan disabled,Page Scan enabled.
                              0x03 Inquiry Scan enabled,Page Scan enabled.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_WriteScanEnable(  HANDLE hResourceContext, unsigned char iScanEnableOption );

     /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, Set Event Filter
   
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFilterType=                               
                              0x00 Clear All Filters
                              0x01 Inquiry result
                              0x02 Connection setup                        

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SetEventFilter(  HANDLE hResourceContext, unsigned char iFilterType );

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, custom command turn on BT transmitter to a specific channel number

   The underlying HCI command is a vendor-specific command called BT_HCI_TX_CONTINUOUS_FREQ.
   This does not actually produce a CW signal, rather a a DH5 packet is transmitted every 6 slots.
   The result is a bursted signal.  It is apparently not possible to transmit just a CW signal
   using the available HCI commands.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iChannel= channel number for Tx. (Tx frequency = 2402 + chan_num, in MHz)

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SetCW(  HANDLE hResourceContext, unsigned char iChannel );

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, custom command turn on BT transmitter with a CW,
   to a specific channel number

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iAddr = 6 digit Bluetooth address.  This will temporarily load the BT address
                  into embedded RAM.  When modes are reset or the phone is restarted,
               the address will be gone.

                   The address will be transformed as follows:
               Input array: { 0x12, 0x90, 0x78, 0x56 0x34, 0x12 }

               Resulting BT address: 123456789012


   \return true if successful, false if fail.

   \warning If the NV_BD_ADDR_I NV item has a value then the NV address will always
            be used and the value specified in this command will be ignored.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SetAddr(  HANDLE hResourceContext, unsigned char iAddr[6] );

   /******************************************************************************/
   /**

   This command issues a Bluetooth "Read_Local_Version_Information" HCI command to
   the Bluetooth module and then capture the resulting Bluetooth event log, which
   contains the Local_Version of the Bluetooth module.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param pBluetootHCI_Version = reference to the structure that will be filled in with the local version information.
                                 use the type Bluetooth_LocalVersion_struct, defined in QLIB_Defines.h

   \return true if successful, false if fail.

   \warning Will reset the log code and logging state



   *******************************************************************************/
   QLIB_API unsigned char QLIB_BT_GetModuleVersion( HANDLE hResourceContext, unsigned char* pBluetootHCI_Version  );

    /******************************************************************************/
   /**

   This command issues a enabled BT HCI event logging for 0x1366 log code       

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()   
   \param pBluetoothHCI_Logs
        1. First byte number of HCI logs
        2. Next byte = length of log payload, followed by payload bytes
          3. Step 2 continues till number of logs are not complete
    \param iNumberOfBytes
        Total number of received bytes

   \return true if successful, false if fail.
  

   \warning Will reset the log code and logging state

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_GetHCILogs( HANDLE hResourceContext, unsigned char* pBluetoothHCI_Logs , unsigned short* iNumberOfBytes);
   

   /******************************************************************************/
   /**

   This function sends BT HCI command and returns the first HCI event/response received

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_BT_HCI_Response, TimeOut) to configure the timeout value for waiting for BT response

   See Test_FTM_BT() in QLibDemo.cpp on how to use this function

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param pHCI_Buffer [in] = HCI command buffer

   \param iHCI_Length [in] = HCI command buffer length

   \param pBTResponseBuffer [in/out] = HCI response buffer

   \param iBTResponseLength [in/out] = [in] is the size of pBTResponseBuffer,
                              [out] is the length of valid data returned by pBTResponseBuffer, 0 means that there is no BT response

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_USER_CMD_WithEventResponse( HANDLE hResourceContext, unsigned char* pHCI_Buffer, unsigned short iHCI_Length, unsigned char* pBTResponseBuffer, unsigned short* iBTResponseLength );
   /******************************************************************************/
   /**

   This function sends BT HCI command and returns the first and second HCI event/response received

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_BT_HCI_Response, TimeOut) to configure the timeout value for waiting for BT response

   See Test_FTM_BT() in QLibDemo.cpp on how to use this function

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param pHCI_Buffer [in] = HCI command buffer

   \param iHCI_Length [in] = HCI command buffer length

   \param pBTResponseBuffer [in/out] = HCI response buffer

   \param iBTResponseLength [in/out] = [in] is the size of pBTResponseBuffer,
                              [out] is the length of valid data returned by pBTResponseBuffer, 0 means that there is no BT response

   \param pSecondBTResponseBuffer2 [in/out] = Second HCI response buffer

   \param iSecondBTResponseLength2 [in/out] = [in] is the size of pSecondBTResponseBuffer,
                              [out] is the length of valid data returned by pSecondBTResponseBuffer, 0 means that there is no BT response

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_USER_CMD_WithTwoEventResponse( HANDLE hResourceContext, unsigned char* pHCI_Buffer, unsigned short iHCI_Length, unsigned char* pBTResponseBuffer, unsigned short* iBTResponseLength, unsigned char* pSecondBTResponseBuffer2, unsigned short* iSecondBTResponseLength );

   /******************************************************************************/
   /**

   This function sends BT HCI command and returns the first and second HCI event/response received

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_BT_HCI_Response, TimeOut) to configure the timeout value for waiting for BT response

   See Test_FTM_BT() in QLibDemo.cpp on how to use this function

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param pHCI_Buffer [in] = HCI command buffer

   \param iHCI_Length [in] = HCI command buffer length

   \param pBTResponseBuffer [in/out] = HCI response buffer

   \param iBTResponseLength [in/out] = [in] is the size of pBTResponseBuffer,
                              [out] is the length of valid data returned by pBTResponseBuffer, 0 means that there is no BT response

   \param pSecondBTResponseBuffer2 [in/out] = Second HCI response buffer

   \param iSecondBTResponseLength2 [in/out] = [in] is the size of pSecondBTResponseBuffer,
                              [out] is the length of valid data returned by pSecondBTResponseBuffer, 0 means that there is no BT response

   \param pThirdBTResponseBuffer3 [in/out] = Third HCI response buffer

   \param iThirdBTResponseLength3 [in/out] = [in] is the size of pThirdBTResponseBuffer,
                              [out] is the length of valid data returned by pThirdBTResponseBuffer, 0 means that there is no BT response


   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_USER_CMD_WithThreeEventResponse( HANDLE hResourceContext, unsigned char* pHCI_Buffer, unsigned short iHCI_Length, unsigned char* pBTResponseBuffer, unsigned short* iBTResponseLength,unsigned char* pSecondBTResponseBuffer, unsigned short* iSecondBTResponseLength, unsigned char* pThirdBTResponseBuffer, unsigned short* iThirdBTResponseLength  );

   /******************************************************************************/
   /**

   This function disables the sleep feature of the BT device. Method should be called before calling QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_TXC_ONLY.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_DisableSleep(HANDLE hResourceContext);

   /******************************************************************************/
   /**

   This function will reset the BT controller.  This method should be utilized to stop the continouse Transmit of the device.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_Reset(HANDLE hResourceContext);

   /******************************************************************************/
   /**

   This method provides user to be able to poke 32 bit value into the memory.  The method sends the "EDL_POKE32_REQ" command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param memAddress [in] = memory location where the data will be written

   \param datalength [in] = number of bytes taht need to be written. number of bytes must be multiple of 4 and must be >=  4.

   \param data [in] = data that is going to be written into the memory.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_EDL_POKE32_REQ(HANDLE hResourceContext, unsigned long memAddress, unsigned char datalength, unsigned char *data);

   /******************************************************************************/
   /**

   This method provides user to be able to poke 16 bit value into the memory.  The method sends the "EDL_POKE16_REQ" command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param memAddress [in] = memory location where the data will be written

   \param datalength [in] = number of bytes taht need to be written. number of bytes must be multiple of 2 and must be >=  2.

   \param data [in] = data that is going to be written into the memory.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_EDL_POKE16_REQ(HANDLE hResourceContext, unsigned long memAddress, unsigned char datalength, unsigned char *data);


   /******************************************************************************/
   /**

   This method provides user to be able to poke 8 bit value into the memory.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param memAddress [in] = memory location where the data will be written

   \param datalength [in] = number of bytes taht need to be written.

   \param data [in] = data that is going to be written into the memory.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_EDL_POKE8_REQ(HANDLE hResourceContext, unsigned long memAddress, unsigned char datalength, unsigned char *data);




   /******************************************************************************/
   /**

   This method provides user to be able to peek 32 bit value into the memory.  The method sends the "EDL_PEEK32_REQ" command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param memAddress [in] = memory location for reading the content of memory

   \param datalength [in] = number of bytes taht need to be read from the memory. Number of bytes must be multiple of 4 and must be >=  4.

   \param data [out] = data that is going to be read from the memory.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_EDL_PEEK32_REQ(HANDLE hResourceContext, unsigned long memAddress, unsigned char dataLengthToBeRead, unsigned char *data);

   /******************************************************************************/
   /**

   This method provides user to be able to peek 16 bit value into the memory.  The method sends the "EDL_PEEK16_REQ" command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param memAddress [in] = memory location for reading the content of memory

   \param datalength [in] = number of bytes taht need to be read from the memory. Number of bytes must be multiple of 4 and must be >=  4.

   \param data [out] = data that is going to be read from the memory.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_EDL_PEEK16_REQ(HANDLE hResourceContext, unsigned long memAddress, unsigned char dataLengthToBeRead, unsigned char *data);

   /******************************************************************************/
   /**

   This method provides user to be able to peek 8 bit value into the memory.  The method sends the "EDL_PEEK8_REQ" command.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param memAddress [in] = memory location for reading the content of memory

   \param datalength [in] = number of bytes taht need to be read from the memory. Number of bytes must be multiple of 4 and must be >=  4.

   \param data [out] = data that is going to be read from the memory.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_EDL_PEEK8_REQ(HANDLE hResourceContext, unsigned long memAddress, unsigned char dataLengthToBeRead, unsigned char *data);


   /******************************************************************************/
   /**

   This method retrieves accumulated test statistics on received packets and the number of errors in the packets received during the testing.
   The method sends the "VS_PROD_TEST_SUBCOMMAND_TEST_STATS" command to gather the statistics.
   The method returns the unsigned char *, but BlueTooth_Prod_Test_Channel_Stat structure should be utilized while casting the parameteres.
   See section 2.5.1.3 of document 80-VE132-7.


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param memAddress [in] = memory location for reading the content of memory

   \param datalength [in] = number of bytes taht need to be read from the memory. Number of bytes must be multiple of 4 and must be >=  4.

   \param channel1Stats [out] = statistics for channel 1

   \param channel2Stats [out] = statistics for channel 2

   \param channel3Stats [out] = statistics for channel 3

   \param channel4Stats [out] = statistics for channel 4

   \param channel5Stats [out] = statistics for channel 5

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_STATS(HANDLE hResourceContext,
         unsigned char *channel1Stats,
         unsigned char *channel2Stats,
         unsigned char *channel3Stats,
         unsigned char *channel4Stats,
         unsigned char *channel5Stats);

   /******************************************************************************/   
   /**

   This method retrieves accumulated test statistics for AR3002 chipset on received packets and
   the number of packets received with CRC errors during the testing.
   The method sends the "VS HCI_GET_PER" command to gather the statistics. Refer:80-Y0519-5
   The method returns the unsigned char *, but BlueTooth_Prod_Test_PER structure should be utilized while casting the parameteres.   
  
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param perStatistics [out] = PER statistics, total packets received and packets with CRC errors

   \param ipacketType [in] = defines the type of packet that will be transmitted. This parameter maps to BlueTooth_PacketType.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_HCI_GET_PER_AR3002(HANDLE hResourceContext,
         unsigned char *perStatistics,unsigned char ipacketType);


    /******************************************************************************/   
   /**

   This method retrieves the WiPower Register values.
   The method sends the "VS HCI_READ_WPR_REGISTER" command to Get the register values.
   Refer:Section 6 of 80-N2785-41  
  
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
    QLIB_API unsigned char QLIB_FTM_BT_HCI_READ_WPR_REGISTER(HANDLE hResourceContext, 
       short* confReg,
       short* currReg,
       short* inteReg,
       short* impeReg);
   /******************************************************************************/   
  /**

   This method retrieves the WiPower Register values.
   The method sends the "VS HCI_READ_WPR_STARK_REGISTER" command to Get the register values.
   Refer:Section 6 of 80-N2785-41  
  
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
    QLIB_API unsigned char QLIB_FTM_BT_HCI_READ_WPR_STARK_REGISTER(HANDLE hResourceContext, short* vthRegister, short* vclRegister, short* controlRegister, short* senseRegister);
   
   /******************************************************************************/   
   /**

   This method Writes the WiPower Register values.
   The method sends the "VS HCI_WRITE_WPR_REGISTER" command to Get the register values.
   Refer:Section 6 of 80-N2785-41  
  
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
    QLIB_API unsigned char QLIB_FTM_BT_HCI_WRITE_WPR_REGISTER(HANDLE hResourceContext, char configByteValue, char  currentLimitValue, char  latchAlarms, char impedanceLimitValue);
   /******************************************************************************/   
   /**

   This method retrieves the WiPower Register values.
   The method sends the "VS HCI_WRITE_WPR_STARK_REGISTER" command to Get the register values.
   Refer:Section 6 of 80-N2785-41  
  
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
    QLIB_API unsigned char QLIB_FTM_BT_HCI_WRITE_WPR_STARK_REGISTER(HANDLE hResourceContext, char vthRegister, char vclRegister, char controlRegister, char senseRegister);
   /******************************************************************************/   
   /**

   This method retrieves the WiPower STATUS Register value.
   The method sends the "VS HCI_READ_WPR_STATUS_REGISTER" command to Get the register values.
   Refer:Section 6 of 80-N2785-41  
  
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
    QLIB_API unsigned char QLIB_FTM_BT_HCI_READ_WPR_STATUS_REGISTER(HANDLE hResourceContext, short* ocal, short* otal, short* comErl, short* oval, short* chgok);
   
   /******************************************************************************/ 
   /**
   Factory Test Mode Bluetooth, custom command for Vendor specific Power Level Correction.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iFlags = where to save power level correction data.  [Description: 0x0 = Write to RAM, 0x1 = Write to OTP]
   \param iCount = number of technology, powerLevel, offsetValue pairs.
   \param iTechnology = technology [Description: 0x0 = BR, 0x01 = EDR, 0x02 = LE]
   \param iPowerLevel = power level index [Description: Range from 0x08 to 0x0B]
   \param iOffsetValue = 6bit signed data with resolution of 0.25 dBm [Description: Range: -32d to 31d (0xE0 - 0xFF, 0x00 - 0x1F)]
   \return true if successful, false if fail.

   *******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_SET_TECH_PL_MAPPING_CORRECTION(HANDLE hResourceContext, unsigned char iFlags, unsigned char iCount, unsigned char* iTechnology, unsigned char* iPowerLevel,  float* iOffsetValue);
  /******************************************************************************/
  /**
  Factory Test Mode Bluetooth, custom command for Vendor specific Power Level Correction.

  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
  \param iFlags = where to save power level correction data.  [Description: 0x0 = Write to RAM, 0x1 = Write to OTP]
  \param iCount = number of technology, powerLevel, offsetValue pairs.
  \param iRFChain = RF Chain CHO and CH1 BIT_4 (000X0000 bit), low = chain 0, high = chain 1.
  \param iTechnology = technology [Description: 0x0 = BR, 0x01 = EDR, 0x02 = LE]
  \param iPowerLevel = power level index [Description: Range from 0x08 to 0x0B]
  \param iOffsetValue = 6bit signed data with resolution of 0.25 dBm [Description: Range: -32d to 31d (0xE0 - 0xFF, 0x00 - 0x1F)]
  \return true if successful, false if fail.

  *******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_SET_TECH_PL_MAPPING_CORRECTION_With_RFChain(HANDLE hResourceContext, unsigned char iFlags, unsigned char iCount, unsigned char *iRFChain,unsigned char* iTechnology,unsigned char* iPowerLevel, float* iOffsetValue);
  /******************************************************************************/
  /**
  Factory Test Mode Bluetooth, custom command for Vendor specific technology based max Radiated power.

  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
  \param iNum_pairs = number of pair of technology and power .
  \param iTechnology = technology [Description: 0x0 = BR, 0x01 = EDR, 0x02 = LE]
  \param iPower = power (dBm) index [Description: Device maximum power to transmit. For the given tech. Resolution=0.25dBm , FF Disable the Max power limit for the given Technology]
  \return true if successful, false if fail.
  *******************************************************************************/

  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_SET_TECH_BASED_MAX_RADIATED_POWER(HANDLE hResourceContext, unsigned char iNum_pairs, unsigned char *iTechnology, float *iPower);
  /******************************************************************************/
  /**
  Factory Test Mode Bluetooth, custom command for Vendor specific for setting Tx Rx ChainID .

  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
  \param iTxChainId = Tx ChainID.
  \param iRxChainID = Rx ChainID.
  \return true if successful, false if fail.
  /******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_SET_RF_Chain(HANDLE hResourceContext, unsigned char iTxChainID, unsigned char iRxChainID);
  /******************************************************************************/
  /**
  Factory Test Mode Bluetooth, custom command for transmit BT LE and QHS packets while bursting, without being in a connection with other device.

  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
  \param ihopChannelsStr [in] = device hops over if limited hopping is available
  \param ipayloadLength [in] = payload length
  \param ipayload_type [in] = defines the trasmit pattern.
  \param ipacketType [in] = defines the type of packet that will be transmitted. This parameter maps to BlueTooth_PacketType.
  \param itransitOutputPower [in] = determine the output power. It needs to be between 0-9.

  \return true if successful, false if fail.
  *******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_Prod_Test_LE_Tx_Burst(HANDLE hResourceContext, unsigned char ihopChannelsStr, unsigned short ipayloadLength, unsigned char ipayload_type, unsigned char ipacket_type, unsigned char itransmitOutputPower);
  /******************************************************************************/
  /**
  Factory Test Mode Bluetooth, custom command is used to read the Technology and PL specific corrections from OTP.

  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
  \param NumBanks [out] = Number of OTP banks  \param NumTech [out] = Number of technologies  \param NumPL [out] = Number of power levels  \param OffsetArray [out] = Offset correction values from OTP

  \return true if successful, false if fail.
  *******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_GET_TECH_PL_MAPPING_CORRECTION(HANDLE hResourceContext, unsigned char* NumBanks, unsigned char* NumTech, unsigned char* NumPL, unsigned char* OffsetArray);
  /******************************************************************************/
  /**
  Factory Test Mode Bluetooth, custom command is used to read the Tx DCOC DBG CAL data
  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
  \param calDataType [in] = calibration Data Type
  Description:
  0x0 = Disable_DBG_read
  0x1 = CLPC_DBG
  0x2 = TXDCOC
  0x3 = TXRSB_CAL
  0x4 = RXRSB_CAL
  0x5 = AGC_CAL
  0x6 = PROCESS_CAL
  0x7 = TEMP_CAL
  0x8 = RXDCOC_CAL
  0x9 = DPD_BOOTTIME_CAL
  0xA = DPD_RXRSB_CAL
  0xB = DPD_RX_GAIN_DC_CA
  0xC = IBF_CAL
  0xD = TPCDCOC
  0xFF = All_DBG_cal

  \return true if successful, false if fail.
  *******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_RADIO_CAL_DBG_DATA_GET(HANDLE hResourceContext, unsigned char calDataType);
  /******************************************************************************/
  /**
  This method  transmits Bluetooth BR/EDR, LE or QHS packets in Burst or Continuous transmission mode.

  \param  hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()
  \param  Channel [in]      = Value for Channel Maximum value: 0x4E
  \param  iPacket_Type [in]   = Value for Packet_Type
  \code
    0x0 = NULL_Packets, 0x1 = POLL_Packets, 0x2 = FHS_Packets, 0x3 = DM1_Packets
    0x4 = DH1_Packets, 0xA = DM3_Packets, 0xB = DH3_Packets, 0xE = DM5_Packets
    0xF = DH5_Packets, 0x9 = AUX1_Packets, 0x24 = TWO_DH1_Packets, 0x2A = TWO_DH3_Packets
    0x2E = TWO_DH5_Packets, 0x28 = THREE_DH1_Packets, 0x2B = THREE_DH3_Packets, 0x2F = THREE_DH5_Packets
    0x5 = HV1_Packets, 0x6 = HV2_Packets, 0x7 = HV3_Packets, 0x8 = DV_Packets
    0x17 = EV3_Packets, 0x1C = EV4_Packets, 0x1D = EV5_Packets, 0x36 = TWO_EV3_Packets
    0x3C = TWO_EV5_Packets, 0x37 = THREE_EV3_Packets, 0x3D = THREE_EV5_Packets, 0x40 = LE_1M_Packets
    0x41 = LE_2M_Packets, 0x42 = LE_Coded_125k_Packets, 0x43 = LE_Coded_500k_Packets, 0x52 = QHS_P2_Packets
    0x53 = QHS_P3_Packets, 0x54 = QHS_P4_Packets, 0x55 = QHS_P5_Packets, 0x56 = QHS_P6_Packets
  \endcode
  \param  iPayload_Length [in]  = Value for Payload_Length
  \param  iPayload_Type [in]    = Value for Payload_Type
  \code
    0x0 = ALL_ZEROES, 0x1 = ALL_ONES,0x2 = ALTERNATE_BITS_10101010, 0x3 = ALTERNATE_NIBBLES_11110000,
    0x4 = PRBS9, 0x5 = PRBS15,0x6 = ALTERNATE_BITS_01010101,
    0x7 = ALTERNATE_NIBBLES_00001111,0x8 = PATTERN_DEFINED_IN_PAYLOAD_PATTERN
  \endcode
  \param iPayload_Pattern [in]  = Pattern to be used in test packets when Payload_Type = 0x08
  \param iTransmit_Type [in]    = Transmit_Type
  \code
    0x0 = BURST_PACKETS,
    0x1 = CONTINUOUS_MODE
  \endcode
  \param iLT_ADDR [in]      = Value for LT_ADDR  0x0-0x7 = BREDR_LOGICAL_TRANSPORT_ADDRESS, 0x8-0xFF = LE_QHS
  \param iHopping [in]      = Value for Hopping 0x0 = ONLY_CHANNEL_USED, 0x1 = Device starts at Channel, then hops over the full range ofchannels using the pattern: next_channel = (current_channel + 1) MOD max_channel
  \param iDeviceAddress [in]    = Value for BD_ADDR
  \param iMax_Packets [in]      = Max Number of packets to transmit after TX mode is initiated
  \param ioff_Slots [in]      = Number of off slots between TX packets. Note - field only for burst mode (not continuous)
  \param iPower_Level [in]    = Value for Power_Level 0x00 = Lowest power, 0x0B = Highest power (depends on chip)
  \return true if successful, false if fail.
  *******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_PROD_TEST_TX_Enhanced(HANDLE hResourceContext, unsigned char iChannel, unsigned char iPacket_Type, unsigned short iPayload_Length, unsigned char iPayload_Type, unsigned long iPayload_Pattern, unsigned char iTransmit_Type, unsigned char iLT_ADDR, unsigned char iHopping, unsigned char *iDeviceAddress, unsigned long iMax_Packets, unsigned char ioff_Slots, unsigned char iPower_Level);
   /******************************************************************************/
  /**

   This method measures and returns the RSSI for the specified channel.
  
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param rssi [out] = averaged rssi value

   \param iChannelOffset [in] = the channel offset for which rssi measurement  is requested, example 0ffset 0 = 2402 MHz, 1 = 2403 MHz... 4 = 2406 MHz

   \param noOfAverages [in][Optional] = Specify the number of averages for the rssi measurement 

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_GET_RSSI(HANDLE hResourceContext,
      short *rssi,unsigned char iChannelOffset, unsigned char noOfAverages = 255);

   /******************************************************************************/
   /**

   This method measures and returns the RSSI for the specified connection handle.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param connectionHandle [in] = connection handle

   \param rssi [out] = rssi, RSSI values in dBm

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_DEBUG_READ_RSSI(HANDLE hResourceContext, unsigned short connectionHandle,
     short *rssi);

    /******************************************************************************/
  /**
  This method is used to retrieve the NVM tag length and NVM tag value of given tag number.

  \param hResourceContext [in] = Resource context that was returned from the call to QLIB_ConnectServer()
  \param invmTagNumber [in] = NVM tag number
  \param invmTagLength [out] = NVM tag length
  \param onvmTagValues [out] = NVM tag values

  \return true if successful, false if fail.
  \warning
  *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_NVM_Access_Get(HANDLE hResourceContext, unsigned char invmTagNumber, unsigned char *invmTagLength, unsigned char *onvmTagValues);
   /******************************************************************************/
   /**
   This method is used to set the NVM tag value of given tag number.

   \param hResourceContext [in] = Resource context that was returned from the call to QLIB_ConnectServer()
   \param invmTagNumber [in] = NVM tag number
   \param invmTagLength [in] = NVM tag length
   \param invmTagValues [in] = NVM tag values

   \return true if successful, false if fail.
   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_VS_NVM_Access_Set(HANDLE hResourceContext, unsigned char invmTagNumber, unsigned char invmTaglength, unsigned char *invmTagValues);
   /******************************************************************************* /
   /**
   This method is used to Run the tcl script..
   \param hResourceContext [in] = Resource context that was returned from the call to QLIB_ConnectServer()
   \param tclFile [in] = TCL file name
   \param workingDir [in] = WorkingDir name
   \param timeOutMS [in] = Time out in MS 
   \return true if successful, false if fail.
   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_RUN_TCL_SCRIPT(HANDLE hResourceContext, unsigned char* tclFile, unsigned char* workingDir, int timeOutMS);
   /******************************************************************************* /
   /**

   This method downloads the nvm from the desired location.
  
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param invmFileLocation [in] = the nvm file location for downloading
   \param skipResponseChecks [in] = If true, we do not wait or check status of ANY event
   \      that we may receive during NVM download. Typically used when response from the DUT
   \      is expected to be delayed and we do not want to see a timeout exception because of it.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_NVM_DOWNLOAD(HANDLE hResourceContext,
      char *invmFileLocation , byte skipResponseChecks = 0);

    /******************************************************************************/
   /**

   This method downloads the patch from the desired location.
  
   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param ipatchFileLocation [in] = the patch file location for downloading

   \param skipResponseChecks [in] = If true, we do not wait or check status of ANY event
   \      that we may receive during PATCH download. Typically used when response from the DUT
   \      is expected to be delayed and we do not want to see a timeout exception because of it.
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PATCH_DOWNLOAD(HANDLE hResourceContext,
      char *ipatchFileLocation , byte skipResponseChecks = 0);

    /******************************************************************************/
   /**

   The method causes the device to transmit full Bluetooth packets while bursting, without
   being in a connection with another device.   The method accomplish this by sending the "VS_PROD_TEST_SUBCOMMAND_TEST_TX" command.
   See section 2.5.1.4 of document 80-VE132-7.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param hopChannels [in] = contains 5 channels the device hops over if limited hopping is available

   \param itransmitPattern [in] = defines the trasmit pattern.  this parameter maps to BlueTooth_TransmittPatternPayload.

   \param ipacketType [in] = defines the type of packet that will be transmitted. This parameter maps to BlueTooth_PacketType.

   \param idataWhiteningEnable [in] = TRUE will enable the whitening. FALSE will disable the whitening.

   \param itransitOutputPower [in] = determine the output power. It needs to be between 0-9.

   \param ireceiverHighGainEnable [in] = determine the receiver gain. TRUE enables high gain. FALSE enables low gain.

   \param itragetDeviceAddress [in] = 6 byte Master Bluetooth device address

   \param ipayloadLength [in] = payload length

   \param ilogicalTransportAddress [in] = logical transport address

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_TX(HANDLE hResourceContext,
         unsigned char ihopChannels[5],
         unsigned char itransmitPattern,
         unsigned char ipacketType,
         unsigned char idataWhiteningEnable,
         unsigned char itransitOutputPower,
         unsigned char ireceiverHighGainEnable,
         unsigned char itragetDeviceAddress[6],
         unsigned char ihoppingEnable,
         unsigned short ipayloadLength,
         unsigned char ilogicalTransportAddress);

    /******************************************************************************/
   /**

   The method causes the device to receive certain number of packets to test receiving rates of device transmitting in production
   test mode.  The method accomplish this by sending the "PROD_TEST_RX_BURST_CONFIG" command to the BT device.
   See document 80-VE132-7.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iNumberofPacketsToReceive = The packets number on which the Receiver test needs to receive

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_BURST_CONFIG( HANDLE hResourceContext,
      unsigned short iNumberofPacketsToReceive);

   /******************************************************************************/
   /**

   The method causes the device to transmit full Bluetooth packets in list mode , without
   being in a connection with another device.   The method accomplish this by sending the "PROD_TEST_SEQ_TX" command.
   See document 80-Y2013-04.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param iTxTestMode [in] = defines the trasmit pattern.

   \param iTxChannel [in] = defines the TX channel be transmitted.

   \param itransitOutputPower [in] = determine the output power. It needs to be between 0-9.

   \param itragetDeviceAddress [in] = 6 byte Master Bluetooth device address

   \param ilogicalTransportAddress [in] = logical transport address

   \param iNumberofPacketsForEachMode [in] = number of packets to use for each modulation type

   \param iNullPacketsEnable [in] = if null packets need to be sent before TX sequence

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_SEQ_TX(HANDLE hResourceContext,
         unsigned char iTxTestMode,
         unsigned char iTxChannel,
         unsigned char itragetDeviceAddress[6],
         unsigned char ilogicalTransportAddress,
       unsigned char itransitOutputPower,
       unsigned short iNumberofPacketsForEachMode,
         unsigned char iNullPacketsEnable);

    /******************************************************************************/
   /**

   The method causes the device to receive full Bluetooth packets in list mode , without
   being in a connection with another device.   The method accomplish this by sending the "PROD_TEST_SEQ_RX" command.
   See document 80-Y2013-04.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param iRxTestMode [in] = defines the receive pattern.

   \param iRxChannel [in] = defines the RX channel be received.

   \param itragetDeviceAddress [in] = 6 byte Master Bluetooth device address

   \param ilogicalTransportAddress [in] = logical transport address

   \param iNumberofRotation [in] = number of rotation to use for sequence

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_SEQ_RX(HANDLE hResourceContext,
         unsigned char iRxChannel,
         unsigned char itragetDeviceAddress[6],
         unsigned char ilogicalTransportAddress,
       unsigned short iNumberofRotation);

    /******************************************************************************/
   /**

   This method retrieves accumulated test statistics on received packets and the number of errors in the packets received during the testing.
   The method sends the "VS_PROD_TEST_SUBCOMMAND_TEST_SEQ_RX_STATUS" command to gather the statistics.
   The method returns the unsigned char *, but BlueTooth_Prod_Test_Channel_Stat structure should be utilized while casting the parameteres.
   See document 80-Y2013-04.


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param DH1Stats [out] = statistics for DH1

   \param DH5Stats [out] = statistics for DH5

   \param twoDH5Stats [out] = statistics for 2DH5

   \param threeDH5Stats [out] = statistics for 3DH5

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_SEQ_RX_STATUS(HANDLE hResourceContext,
         unsigned char *DH1Stats,
         unsigned char *DH5Stats,
         unsigned char *twoDH5Stats,
         unsigned char *threeDH5Stats,
       unsigned char *leStats);

   /******************************************************************************/   
   /**

   The method sets the device into slave mode.  The device then specified frequency for the presence of a master device.
   Once the master device is found, the two devices transmit data.
   The method accomplish this by sending the "VS_PROD_TEST_SUBCOMMAND_TEST_SLAVE" command.
   See section 2.5.1.1 of document 80-VE132-7.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param hopChannels [in] = contains 5 channels the device hops over if limited hopping is available

   \param itransmitPattern [in] = defines the trasmit pattern.  this parameter maps to BlueTooth_TransmittPatternPayload.

   \param ipacketType [in] = defines the type of packet that will be transmitted. This parameter maps to BlueTooth_PacketType.

   \param idataWhiteningEnable [in] = TRUE will enable the whitening. FALSE will disable the whitening.

   \param itransitOutputPower [in] = determine the output power. It needs to be between 0-9.

   \param ireceiverHighGainEnable [in] = determine the receiver gain. TRUE enables high gain. FALSE enables low gain.

   \param itragetDeviceAddress [in] = 6 byte Master Bluetooth device address

   \param ipayloadLength [in] = payload length

   \param ilogicalTransportAddress [in] = logical transport address

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_SLAVE(HANDLE hResourceContext,
         unsigned char  ihopChannels[5],
         unsigned char  itransmitPattern,
         unsigned char  ipacketType,
         unsigned char  idataWhiteningEnable,
         unsigned char  itransitOutputPower,
         unsigned char  ireceiverHighGainEnable,
         unsigned char  itragetDeviceAddress[6],
         unsigned char  ihoppingEnable,
         unsigned short ipayloadLength,
         unsigned char  ilogicalTransportAddress);

   /******************************************************************************/
   /**

   The method causes the device to transmit continuously on the specified channel.
   The method accomplish this by sending the "VS_PROD_TEST_SUBCOMMAND_TEST_TXC_ONLY" command to the BT device.
   See section 2.5.1.5 of document 80-VE132-7.

   Note:
      In order to support 4020/4021/4025 and Marimba devices, following algorithm is been used.
         Method gets the unit app version by looking at the PF values (as per Bill Chens request).
         It is done by looking at the 4 bytes starting at 0x29 location of the event that was sent back.

         if power level less than or equal to 7 send the command as normal.  If the unit is 4025 then send a series of poke8 commands provided by the apps team.
         if power level is either 8 or 9 then take actions as described below.
            If unit 4025 send the Tx packet with Power level '7' in it and then send series of poke8 commands provided by Apps team.
            If unit is either 4020 / 4021 / 4000 then power level is set back to 7 and command is sent to the unit. (no Poke commands are sent)
            if some other unit is found (e.g .. Marimba), command is sent as is where the unit is expected to support power 8 and 9.



   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param ichannel [in] = device transmit channel

   \param itransmitOutputPower [in] = determine the output power. It needs to be between 0-9.

   \param itransmitType [in] = defines the trasmit type.  this parameter maps to BlueTooth_TransmitType.

   \param ipatternLength [in] = specifies the length of the repeated pattern

   \param ibitPattern [in] = bit pattern transmitted repeatdly

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_TXC_ONLY( HANDLE hResourceContext,
         unsigned char ichannel,
         unsigned char itransmitOutputPower,
         unsigned char itransmitType,
         unsigned char ipatternLength,
         unsigned long ibitPattern);


   /******************************************************************************/
   /**


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param ichannel [in] = device transmit channel, even only

   \param itransmitOutputPower [in] = determine the output power. It needs to be between 0-15.

   \param itransmitType [in] = defines the trasmit type.  this parameter maps to BlueTooth_TransmitType.

   \param ipatternLength [in] = specifies the length of the repeated pattern

   \param ibitPattern [in] = bit pattern transmitted repeatdly

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_PROD_TEST_TXCONT_QCA402x(HANDLE hResourceContext,
     unsigned char ichannel,
     unsigned char itransmitOutputPower,
     unsigned char itransmitType,
       unsigned char ipatternLength,
       unsigned long ibitPattern);

   /******************************************************************************/

   /******************************************************************************/
   /**

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param itestMode [in] = test mode options
   0x2 = QHS_Tx_Test_Mode
   0x3 = QHS_Rx_Test_Mode

   \param ichannel [in] = device transmit channel, even only   
   Channel number. N = (F - 2402) / 2; 0x00 = 2402MHz; 0x27 = 2480MHz

   \param ipayloadLenth [in] = specifies the length of payload

   \param ipayloadPattern [in] = 
   0x0 = PRBS9 sequence '11111111100000111101...'
   0x1 = Repeated '11110000'
   0x2 = Repeated '10101010'
   0x3 = PRBS15 sequence
   0x4 = Repeated '11111111'
   0x5 = Repeated '00000000'
   0x6 = Repeated '00001111'
   0x7 = Repeated '01010101'   

   \param iphyRate [in] = PHY rate 
   0x6 = QHS2P_2Mbps_PSK_Modulation
   0x7 = QHS2P_3Mbps_PSK_Modulation
   0x8 = QHS2P_4Mbps_PSK_Modulation
   0x9 = QHS2P_5Mbps_PSK_Modulation
   0xA = QHS2P_6Mbps_PSK_Modulation

   \param itxPowerIndex [in] = determine the output power. It needs to be between 0-9.
   0x0 = Tx_Lowest_Power
   0x1-0x8 = Tx_Power_Index
   0x9 = Tx_Highest_Power

   \param imaxPackets [in] = Max number of packets to transmit after Tx mode is initiated. 0 to transmit indefinitely

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_QHS_PROD_TEST(HANDLE hResourceContext,
     unsigned char itestMode,
     unsigned char ichannel,
     unsigned short ipayloadLength,
     unsigned char ipayloadPattern,
     unsigned char iphyRate,
     unsigned char itxPowerIndex,
     unsigned short imaxPackets);

   /******************************************************************************/

   /**


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param ichannel [in] = device transmit channel, even only

   \param itransmitOutputPower [in] = determine the output power. It needs to be between 0-15.

   \param itransmitType [in] = defines the trasmit type.  this parameter maps to BlueTooth_TransmitType.

   \param ipatternLength [in] = specifies the length of the repeated pattern

   \param ibitPattern [in] = bit pattern transmitted repeatdly

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_PROD_TEST_TXCONT(HANDLE hResourceContext,
     unsigned char ichannel,
     unsigned char itransmitOutputPower,
     unsigned char itransmitType,
     unsigned char ipatternLength,
     unsigned long ibitPattern);

   /******************************************************************************/
   /**


   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param iTechnologyType [in] = 0 for BR/EDR and 1 for LE

   \param ichannel [in] = device receive channel

   \param iSWGain [in] = Receiver SW gain

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_CONTINUOUS(HANDLE hResourceContext, 
     byte iTechnologyType, 
     byte ichannel,
     byte iSWGain
   );
   /******************************************************************************* /
    /**

   The method causes the device to transmit continuously on the specified channel and power level for AR3002 chipset.  

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param ichannel [in] = device transmit channel  (0-78)

   \param ichannel [in] = device power level (0-7)  

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_TXC_AR3002( HANDLE hResourceContext,
         unsigned char ichannel,unsigned char iPowerLevel);

   /******************************************************************************/
   
   /**

   The method causes the device to receive continuously on the specified channel to test receiving rates of device transmitting in production
   test mode.  The method accomplish this by sending the "VS_PROD_TEST_SUBCOMMAND_TEST_RX_ONLY" command to the BT device.
   See section 2.5.1.6 of document 80-VE132-7.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param hopChannels [in] = contains 5 channels the device hops over if limited hopping is available

   \param itransmitPattern [in] = defines the trasmit pattern.  this parameter maps to BlueTooth_TransmittPatternPayload.

   \param ipacketType [in] = defines the type of packet that will be transmitted. This parameter maps to BlueTooth_PacketType.

   \param idataWhiteningEnable [in] = TRUE will enable the whitening. FALSE will disable the whitening.

   \param itransitOutputPower [in] = determine the output power. It needs to be between 0-9.

   \param ireceiverHighGainEnable [in] = determine the receiver gain. TRUE enables high gain. FALSE enables low gain.

   \param itragetDeviceAddress [in] = 6 byte Master Bluetooth device address

   \param ipayloadLength [in] = payload length

   \param ilogicalTransportAddress [in] = logical transport address

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_RX_ONLY( HANDLE hResourceContext,
         unsigned char ihopChannels[5],
         unsigned char itransmitPattern,
         unsigned char ipacketType,
         unsigned char idataWhiteningEnable,
         unsigned char itransitOutputPower,
         unsigned char ireceiverHighGainEnable,
         unsigned char itragetDeviceAddress[6],
         unsigned char ihoppingEnable,
         unsigned short ipayloadLength,
         unsigned char ilogicalTransportAddress);
   /******************************************************************************/
   /**

   This function will start the BT LE Receiver test.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency = The test frequency on which the Receiver test needs to be run

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Receiver_Test(HANDLE hResourceContext,
         unsigned char iTestFrequency);

   /******************************************************************************/
   /**

   This function will start the BT_PIN_CONNECTIVITY_Test.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param \param iInterfaceID = interface to check

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PIN_CONNECTIVITY_Test(HANDLE hResourceContext,
     unsigned char iInterfaceID);

   /******************************************************************************/
   /**

   This function will start the BT_SELECT_INTERFACE.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param \param iInterfaceID = interface to check

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SELECT_INTERFACE(HANDLE hResourceContext,
     unsigned char iInterfaceID);


   /******************************************************************************/
   /**

   This function will start the BT LE Receiver test.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency = The test frequency on which the Receiver test needs to be run
   \param iPhyType = 1 for LE 1M and 2 for LE 2M

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Enhanced_Receiver_Test(HANDLE hResourceContext,
         unsigned char iTestFrequency, unsigned char iPhyType);

   /******************************************************************************/
   /**

   This function will start the BT LE Receiver test with modulation index option.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency = The test frequency on which the Receiver test needs to be run
   \param iPhyType = 1 for LE 1M and 2 for LE 2M
   \param iModulationIndex = 1 to assume transmitter will have a stable modulation index and 0 to assume transmitter will have a standard modulation index

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Enhanced_Receiver_Test_ModulationIndex(HANDLE hResourceContext,
         unsigned char iTestFrequency, unsigned char iPhyType, unsigned char iModulationIndex);

   /******************************************************************************/
   /**

   This function will start the BT LE Transmitter test.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency [in] = The test frequency on which the Transmitter test needs to be run
   \param iTestPayloadLength [in] = The length of data for the tx test
   \param iTestPayload [in] = The pattern of the payload
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Transmitter_Test(HANDLE hResourceContext,
         unsigned char iTestFrequency,
         unsigned char iTestPayloadLength,
         unsigned char iTestPayload);

      /******************************************************************************/
   /**

   This function will start the BT LE Transmitter test.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency [in] = The test frequency on which the Transmitter test needs to be run
   \param iTestPayloadLength [in] = The length of data for the tx test
   \param iTestPayload [in] = The pattern of the payload
   \param iPhyType [in] = 1 = 1LE and 2 = 2LE
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Enhanced_Transmitter_Test(HANDLE hResourceContext,
         unsigned char iTestFrequency,
         unsigned char iTestPayloadLength,
         unsigned char iTestPayload,
         unsigned char iPhyType);

   /******************************************************************************/
   /**

     This function will start the BT LE Transmitter test V3.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency [in] = The test frequency on which the Transmitter test needs to be run
   \param iTestDataLength [in] = The length of test data for the tx test
   \param iTestPayloadLength [in] = The length of data for the tx test
   \param iPhyType [in] = 1 = 1LE, 2 = 2LE, 3 = BLR S=8, 4 = BLR S=2
   \param iCTELength [in] = 0 = DONOT TRANSMIT CTE, 1 = RESERVED , 2 to 14 = LENGTH OF CONSTANT TONE EXTENSION
   \param iSupplementalSlotType [in] = 0 = NO SUPPLEMENTAL, 1 = 1US SLOTS, 2 = 2US SLOTS   
   \param iLengthOfSwitchingPattern [in] = range 2 to 0x4B, number of antenna IDs in the pattern
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Transmitter_Test_V3(HANDLE hResourceContext,
     unsigned char iTestFrequency,
     unsigned char iTestDataLength,
     unsigned char iTestPayloadLength,
     unsigned char iPhyType,
     unsigned char iCTELength,
     unsigned char iSupplementalSlotType,
     unsigned char iLengthOfSwitchingPattern);

   /******************************************************************************/
   /**

   This function will start the BT LE Receiver test V3.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency = The test frequency on which the Transmitter test needs to be run
   \param iPhyType [in] = 1 = 1LE, 2 = 2LE, 3 = BLR S=8, 4 = BLR S=2
   \param iTestDataLength [in] = The length of test data for the tx test
   \param iTestPayloadLength [in] = The length of data for the tx test
   \param iCTELength [in] = 0 = DONOT TRANSMIT CTE, 1 = RESERVED , 2 to 14 = LENGTH OF CONSTANT TONE EXTENSION
   \param iSupplementalSlotType [in] = 0 = NO SUPPLEMENTAL, 1 = 1US SLOTS, 2 = 2US SLOTS
   \param iLengthOfSwitchingPattern [in] = range 2 to 0x4B, number of antenna IDs in the pattern
   \param pAntennaIDS char array length based on iLengthOfSwitchingPatterns
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Receiver_Test_V3(HANDLE hResourceContext,
     unsigned char iTestFrequency,
     unsigned char iPhyType,
     unsigned char iModulationIndex,
     unsigned char iExpectedCTELength,
     unsigned char iExpectedCTEType,
     unsigned char iSlotDuration,
     unsigned char iLengthOfSwitchingPattern,
     unsigned char pAntennaIDs[248]);

   /******************************************************************************/
   /**

     This function will start the BT LE Transmitter test V4.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iTestFrequency = The test frequency on which the Transmitter test needs to be run
   \param iTestDataLength [in] = The length of test data for the tx test
   \param iPacketPayload [in] = The pattern of the payload
   \param iPhyType [in] = 1 = 1LE, 2 = 2LE, 3 = BLR S=8, 4 = BLR S=2
   \param iCTELength [in] = 0 = DONOT TRANSMIT CTE, 1 = RESERVED , 2 to 14 = LENGTH OF CONSTANT TONE EXTENSION
   \param iCTELType [in] = 0x0 = AoA Constant Tone Extension, 0x1 = AoD Constant Tone Extension with 1 us slots, 0x2 = AoD Constant Tone Extension with 2 us slots, 0x3-0xFF = Reserved for Future Use
   \param iLengthOfSwitchingPattern [in] = 0x0-0x1 = Reserved_For_Future_Use, 0x2-0x4B = The number of Antenna IDs in the pattern, 0x4C-0xFF = Reserved for Future Use
   \Param iTransmitPowerLevel [in] = Set transmitter to the specified or the nearest transmit powerlevel, Range -127 to 20, Units: dBm
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char  QLIB_FTM_BT_LE_HCI_Transmitter_Test_V4(HANDLE hResourceContext,
	   int iTestFrequency,
	   unsigned char iTestDataLength,
	   unsigned char iPacketPayload,
	   unsigned char iPhyType,
	   unsigned char iCTELength,
	   unsigned char iCTEType,
	   unsigned char iLengthOfSwitchingPattern,
	   unsigned char iTransmitPowerLevel);
   /**

   This function will end the BT LE Test running.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_LE_HCI_Test_End(HANDLE hResourceContext,
         unsigned short* noOfPackets);

   /******************************************************************************/
   /**

   This function will end the BT GET PIN Connectivity command.

   \param BTResponse [out] 
   \param WLANResponse [out] 
   \param FMResponse [out] 
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_Get_PIN_CONNECTIVITIY(HANDLE hResourceContext,unsigned short* BTResponse,
      unsigned short* WLANResponse, unsigned short* FMResponse);

    /******************************************************************************/
   /**

   This function will end the BT Tx Continous Test running.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_PROD_TEST_SUBCOMMAND_TEST_STOP(HANDLE hResourceContext);

   /******************************************************************************/
   /**

   This function will write the BT SSBI value

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param sAddress [in] = The address of the value to write in
   \param lValue [in] = The value to write in 
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/

   QLIB_API unsigned char QLIB_FTM_BT_SSBI_POKE(HANDLE phoneHandle, unsigned short sAddress, unsigned long lValue);

    /******************************************************************************/
   /**

   This function will read the BT SSBI value

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param sAddress [in] = The address of the value to read
   \param lValue [out] = The value that reads 
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_SSBI_PEEK(HANDLE phoneHandle, unsigned short sAddress, unsigned char* lValue);

   QLIB_API unsigned char QLIB_FTM_BT_HCI_OTP_DUMP(HANDLE phoneHandle, unsigned char cPSType, unsigned short sOffset, unsigned char cDataLength, unsigned char* lValue);

   QLIB_API unsigned char QLIB_FTM_BT_HCI_EDL_DNLOAD_REQ(HANDLE phoneHandle, unsigned long sAddress, unsigned short sOffset, unsigned char dataLength, unsigned char* lValue);

   QLIB_API unsigned char QLIB_FTM_BT_HCI_EDL_TOPCFG_BURN_REQ(HANDLE phoneHandle, unsigned short sAddress, unsigned short sLength, unsigned short sEnable, unsigned short sOTPOffset);


   /******************************************************************************/
   /**

   This function will detach and erase the BT patch and NVM in the serial flash.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLegacyOpcode [in] = 0x00: Use current opcode; 0x01~0xFF: Use legacy opcode
   \return true if successful, false if fail.

   \warning For USB standalone devices, the driver will automatically download NVM 
            and patch to the serial flash during re-enumeration.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_SFLASH_DETACH_ERASE(HANDLE hResourceContext, unsigned char iLegacyOpcode);

   /******************************************************************************/
   /**

   This function will send a Bluetooth "GET_FLASH_BURNING_STATUS" HCI command to
   the Bluetooth module and then capture the resulting Bluetooth event log, which
   contains the burn status of the serial flash.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLegacyOpcode [in] = 0x00: Use current opcode; 0x01~0xFF: Use legacy opcode
   \param patchStatus [out] = Patch burn status
   \param nvmStatus [out] = NVM burn status
   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_HCI_SFLASH_GET_BURN_STATUS(HANDLE hResourceContext, unsigned char iLegacyOpcode, unsigned char* patchStatus, unsigned char* nvmStatus);


   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 Set TX Power level Command

     \param iTxPowerLevel= TX Power level in dB                        

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_SetTxPower(  HANDLE hResourceContext, unsigned char iTxPowerLevel );

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 Set BLE TX Power level Command

     \param iBLETxPowerLevel= BLE TX Power level in dB                        

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_SetBLETxPower(  HANDLE hResourceContext, short iBLETxPowerLevel );

      /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 Send Warm Reset Command                   

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_WarmReset(  HANDLE hResourceContext );

    /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 Set Packet Type Command

   \param iPacketType= Packet Type
   \param iPayloadLength = payload length

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_SetPacketType(  HANDLE hResourceContext, unsigned short iPacketType , unsigned short iPayloadLength);

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 Send Pause Command                   

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_Pause(  HANDLE hResourceContext );
   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 Config XTAL FTrim Command

   \param iXtalFtrim= xtal ftrim value  
  
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_ConfigXtalFTrim(  HANDLE hResourceContext, unsigned short iXtalFTrim);

    /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 CSR8811_Tx_Data_ON Command

   \param Freq= freq in MHz
   \param iPayloadType = modulation type
   \param packetType = BR or EDR
  
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_Tx_Data_ON(  HANDLE hResourceContext, unsigned short iTxFreq, unsigned char iPayloadType, unsigned char packetType );

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 QLIB_FTM_BT_CSR8811_Rx_BER_ON Command

   \param iRxFreq= freq in MHz 
   \param iNumberOfPackets = number of expected packets
     
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_Rx_BER_ON(  HANDLE hResourceContext, unsigned short iRxFreq, unsigned short iNumberOfPackets );

    /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 QLIB_FTM_BT_CSR8811_Rx_BER_ON Command

   \param iBitsInError= total bits in error 
   \param iTotalBitsReceived = total bits received
     
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_Get_Rx_Stats(  HANDLE hResourceContext, unsigned long *iBitsInError, unsigned long* iTotalBitsReceived );

   /******************************************************************************/
   /**
   Factory Test Mode Bluetooth, CSR8811 QLIB_FTM_BT_CSR8811_Rx_BER_ON Command

   \param UAP = UAP byte of BD address
   \param LAP = LAP part of BD address
     
   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_BT_CSR8811_SET_BD_ADDR(HANDLE hResourceContext, unsigned char UAP, unsigned char LAP[3]);

#ifdef __cplusplus
}   // extern "C"
#endif

#endif   // defined(_QLIB_BLUETOOTH_H)
