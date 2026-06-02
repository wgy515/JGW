/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Core/inc/QMSL_Quts_Intf.h#13 $
 * $DateTime: 2021/02/11 04:29:13 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2018-2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#if !defined(_QLIB_QUTS_INTF_H)
#define _QLIB_QUTS_INTF_H

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
   QUTS INTERFACE Functions
   *******************************************************************************/


   /******************************************************************************/
   /**
   Start QUTS TCP Server with the specified port number.
   TCP/IP diag client can connect to the specified port of QUTS TCP/IP server

   \param type = Set 0 to indicate diag type
   \param port = must be valid TCP/IP port number
   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Start_TCP_Server(int type, unsigned int port);

   /******************************************************************************/
   /**
   Connect QUTS to specified TCP/IP diag server

   \param port = must be valid TCP/IP port number
   \param address_length = addres length
   \param tcp_ip_addr = TCP/IP address string 
   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Set_ComPort_TCP_Client(unsigned short port, unsigned int address_length, unsigned char * tcp_ip_addr);

   /******************************************************************************/
   /**
   Disconnect TCP/IP connection of the specified TCP/IP port

   \param port = must be valid TCP/IP port number

   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Stop_TCP_Server(unsigned int port);

   /******************************************************************************/
   /**
   Start HDF logging

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer
   \param iError = Error code. 0: No error. 1: General Error.

   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Start_HDF_Logging(HANDLE hResourceContext, unsigned int* iError);

   /******************************************************************************/
   /**
   Save all HDF logs on to the specified folder. Note: We can specify folder name only.
   Please use QLIB_QUTS_SaveHDFLogFilesWithFilenames to specify filename

   \param folder_name_length = folder name string length
   \param folder_name = folder name string

   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Save_Logs(unsigned int folder_name_length, unsigned char * folder_name);

   /******************************************************************************/
   /**
   Save HDF log as the specified filepath. this API may not overwrite an existing file.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer
   \param filepath = filepath with NULL character ending
   \param iError = Error code. 0: No error. 1: General Error.
   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_SaveHDFLogFilesWithFilenames(HANDLE hResourceContext, char* filepath, unsigned int* iError);

   /******************************************************************************/
   /**
   Load QXDM DMC file for HDF logging.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer
   \param filepath_size = size of filepath size
   \param filepath = filepath with NULL character ending
   \param iError = Error code. 0: No error. 1: General Error.
   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Set_Logmask(HANDLE hResourceContext, unsigned int filepath_size, unsigned char * filepath, unsigned int * err_code);
   
   /******************************************************************************/
   /**
   Set QMSL_QUTS debug message level. 

   \param level = 0: No QMSL_QUTS log generated.  1:  Generate QMSL_QUTS log at C:\\Qualcomm\\QDART\\temp\\ folder
   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Set_Debug_Msg_Level(int level);

   /******************************************************************************/
   /**
   Get Device info in JSON format

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer
   \param text_size = response buffer size
   \param device_info = response buffer (must allocate buffer by the size of text_size
   \param iError = Error code. 0: No error. 1: General Error.
   
   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Get_Device_Info(HANDLE hResourceContext, unsigned int* text_size, unsigned char* device_info, unsigned int* err_code);

   /******************************************************************************/
   /**
   Get Virtual COM port number used by QMSL 

   \param protocol_type = use 0
   \param device_handle_id = device handle id
   \param protocol_handle_id = protocol handle id
      
   \param iError = Error code. 0: No error. 1: General Error.

   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Get_VirtualComPortByHandleID(unsigned int protocol_type, long long device_handle_id, long long protocol_handle_id, unsigned short* comPort);
   
   /******************************************************************************/
   /**
   Reset HDF log. This API clears current HDF log contents on memory

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer
   \param iError = Error code. 0: No error. 1: General Error.

   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_ResetHDFLogs(HANDLE hResourceContext, unsigned int* iError);

   /******************************************************************************/
   /**
   Add annotation on HDF log. 

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer
   \param messageId = Set 0
   \param message = message text with NULL termination
   \param iError = Error code. 0: No error. 1: General Error.

   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_AddAnnotationHDFLog(HANDLE hResourceContext, long long messageId, char* message, unsigned int* iError);


   /******************************************************************************/
   /**
   Add TCP cpmnnection .

   \param protocol_type = Set 0. 0 is diag mode.  
   \param bIsClient = Set 1 or true for the case QUTS is TCP/IP client case. Set 0 for  the case QUTS is TCP/IP server 
   \param description = Add description text you would like to add. NULL terminate. 
   \param host = Add host ip address. NULL terminate.
   \param port = port number to connect
   \param protocolhandle = pointer to protocol handle. once this API succeeds. protocol handle value will be given  
   \param devicehandle = pointer to devicehandle. This is only for the case QUTS is TCP/IP client case. Set NULL or any value.
   \param iError = Error code. 0: No error. 1: General Error.

   int protocol_type, unsigned char bIsClient, const char* description, const char* host, int port, unsigned long long* protocolhandle, unsigned long long* devicehandle, unsigned int* iFTM_ERROR);

   \return 1: Succcess, 0: failure
   *******************************************************************************/
   QLIB_API unsigned char QLIB_QUTS_Add_TCP_Connection(int protocol_type, unsigned char bIsClient, const char* description, const char* host, int port, unsigned long long* protocolhandle, unsigned long long* devicehandle, unsigned int* iFTM_ERROR);

   QLIB_API unsigned char QLIB_QUTS_GetQShrinkState(HANDLE hResourceContext, unsigned int* state, unsigned int* iError);
   QLIB_API unsigned char QLIB_QUTS_LoadQShrinkFile(HANDLE hResourceContext, char* path, unsigned int* iError);
   QLIB_API unsigned char QLIB_QUTS_ResetPhone(HANDLE hResourceContext, unsigned int resetTimoutMS, unsigned int* iError);
   QLIB_API unsigned char QLIB_QUTS_GenericQuery(HANDLE hResourceContext, char * request, unsigned int * res_size, char * res_buffer, unsigned int* iError);
   QLIB_API unsigned char QLIB_QUTS_Set_DiagConnectionOptions(unsigned char bEnableHDLCModeonly, unsigned char bOpenQDSS, unsigned char bEnableDefaultQdssConfiguration, unsigned int* iError);
   QLIB_API unsigned char QLIB_QUTS_Remove_TCP_Connection(unsigned long long protocolhandle, unsigned int * iFTM_ERROR);
   QLIB_API unsigned char QLIB_QUTS_Set_Default_BAUD_Rate(int baudrate, unsigned int * iFTM_Error);
#ifdef __cplusplus
}   // extern "C"
#endif

#endif   // defined(_QLIB_DIAG_H)
