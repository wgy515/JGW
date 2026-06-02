/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/GPIBController.h#12 $
 * $DateTime: 2015/12/30 13:40:45 $
 *
 * DESCRIPTION: CGPIBController
 ******************************************************************************
 *
 * Copyright (c) 2007-2015 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#pragma once

//#include <afxmt.h>

// National Instruments GPIB Driver (requires gpib-32.obj)
#include "National Instruments\Shared\ExternalCompilerSupport\C\include\decl-32.h"

/**
 * Class CGPIBController
 * This class provides a high level interface for user to read/write GPIB interface
 * directly.  The purpose of this class is to hide the lower level implementation of GPIB
 * I/O from the user.  As a result, changes of the actual GPIB's card or device
 * driver should not lead to changes in user code that read/write GPIB interface.
 * This class provides more error checking/handling abilities than the basic GPIB I/O libaries.
 * This class can be a wrapper to a low level GPIB I/O libraries.
 */
class CGPIBController
{
public:

   /** Timeout enumerations */
   enum EnumTimeOut
   {
      T_NO_CHANGE = -1,
      T_DISABLE,
      T_10US,
      T_30US,
      T_100US,
      T_300US,
      T_1MS,
      T_3MS,
      T_10MS,
      T_30MS,
      T_100MS,
      T_300MS,
      T_1S,
      T_3S,
      T_10S,
      T_30S,
      T_100S,
      T_300S,
      T_1000S
   };

   /**
    * Constructor
    * @param primAddr
    * @param secAddr
    * @param boardId
    */
   CGPIBController(int primAddr = 20, int boardId = 0, int secAddr = 96);

   /**
    * Destructor
    */
   ~CGPIBController();
   
   /**
    * Send data over the transport.
    * @param buf A pointer to the send data buffer.
    * @param iLen The size in bytes of the data buffer.
    * @return bool TRUE if operation is successful, FALSE otherwisee.
    */
   bool Send (const char*, long len);
   
   /**
    * Receive data from the transport.
    * @param buffer A pointer to the receive data buffer.
    * @param size The size in bytes of the data buffer.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   bool Receive (char*, int maxLen);

   /**
    * Open the session.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   bool Open();

   /**
    * Close the session.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   bool Close();

   /**
    * Clear the device.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   bool ClearDevice();

   /**
    * Get the primary address of this device.
    * @return int The primary GPIB address.
    */
   int GetDeviceAddress();
   
   /** 
    * Get the session ID.
    * @return int session id.
    */
   int GetID();

   /**
    * Set the command timeout value.
    * @param timeoutMs Timout value as enumerated
    * @return bool TRUE if operation is successful, otherwise FALSE.
    */
   bool SetTimeOut(EnumTimeOut TimeOut);
   
   /**
    * Get the current command timeout value.
    * @return EnumTimeOut Timeout value as enumerated.
    */   
   EnumTimeOut GetTimeOut();

   /**
    * Convert the timeout enumeration to milliseconds.
    * @param Timeout Timout value as enumerated.
    * @return int The timeout value in milliseconds.
    */
   int ConvertTimeOutValue(EnumTimeOut TimeOut);

   /**
    * Check the error status of the GPIB interface.
    * @return bool TRUE if no error, otherwise FALSE.
    */
   bool GetErrorStatus();

private:
   int ud;      //NI
   int    _primaryAddress;
   int    _secondaryAddress;
   int    _boardId;
   HANDLE m_criticalSection;

   /**
    * Get user configured timeout value from global repository.
    * @return EnumTimeOut Timeout value as enumerated.
    */
   EnumTimeOut GetTimeOutFromConfig();
};
