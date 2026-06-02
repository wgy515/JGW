/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/GPIBController.cpp#14 $
 * $DateTime: 2015/10/28 14:56:58 $
 *
 * DESCRIPTION: CAgilenGPIBController
 ******************************************************************************
 *
 * Copyright (c) 2007-2015 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#include "stdafx.h"

#include <stdio.h>
#include <JGW_NIGPIBControllImpl/AgilentGPIBController.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
using namespace JGW;
/******************************************************************************
 * FUNCTION: CAgilenGPIBController::CAgilenGPIBController
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
CAgilentGPIBController::CAgilentGPIBController(int primAddr , int boardId/* = 0*/, int secAddr/* = 96*/) :
   _primaryAddress(primAddr),
   _secondaryAddress(secAddr),
   _boardId(boardId),
   m_dwDevice(-1),
   m_criticalSection(CreateMutex(NULL,FALSE,L"CTSTIMutexAgilent"))
{
}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::~CAgilenGPIBController
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
CAgilentGPIBController::~CAgilentGPIBController()
{
    Close();
}

void err_handler(int id, int error)
{
    Log4WF_F(L"Error: %s\n",JGW_A2W_A(igeterrstr (error)).c_str());
}
/******************************************************************************
 * FUNCTION: CAgilenGPIBController::GetDeviceAddress
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
int CAgilentGPIBController::GetDeviceAddress()
{
   return _primaryAddress;
}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::GetID
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
int CAgilentGPIBController::GetID()
{
   return m_dwDevice;
}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::Send
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CAgilentGPIBController::Send(const char* buf, long iLen)
{
    if ( WAIT_OBJECT_0!=WaitForSingleObject(m_criticalSection,90000) )
        return false;

//     memset(Buffer, 0x00, sizeof(Buffer));
//     strncpy(Buffer, buf, AGILENT_BUFFER_LEN - 1);
//     strcat(Buffer, "\n");
    if (I_ERR_NOERROR != iprintf(m_dwDevice,"%s\n", buf))
    {
        ReleaseMutex(m_criticalSection);
        return false;
    }
    ReleaseMutex(m_criticalSection);

    return true;
}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::Receive
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CAgilentGPIBController::Receive(char* buffer,int size)
{
   ZeroMemory( buffer, size );

   if ( WAIT_OBJECT_0!=WaitForSingleObject(m_criticalSection,90000) )
      return false;

   //ibrd(ud, buffer, size);
   if(iscanf(m_dwDevice, "%t", buffer) != I_ERR_NOERROR)
   {
      ReleaseMutex(m_criticalSection);
      return false;
   }

   if (buffer == NULL)
   {
      ReleaseMutex(m_criticalSection);
      return false;
   }

   ReleaseMutex(m_criticalSection);
   return true;
}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::Open
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
//Auto Test Functionality
bool CAgilentGPIBController::Open()
{
#if 0
   //read timeout from station config. default to 30s if not exist
   EnumTimeOut timeoutVal = GetTimeOutFromConfig();
   short int iblisten = 0;
   // try for NI
   //open and initialize a device
   ud = ibdev(_boardId, _primaryAddress, _secondaryAddress, timeoutVal, 1, 0);//timeoutVal is used for this single transaction.
   Log4WD_F(L"ibdev device ID: %d",ud);

   if (!(ERR & ibsta))
   {
	   ibln(ud, _primaryAddress, _secondaryAddress, &iblisten);
	   Log4WD(L" if (!(ERR & ibsta)) ");
	   if(!(iblisten))
	   {
		   throw  "Interface board not installed or properly configured [ibln]" ;
	   }
   }

   if ( ud == -1 )
   {
	   ud = ibdev(_boardId, _primaryAddress, _secondaryAddress, T10s, 1, 0);
	   Log4WD_F(L"second ibdev device ID: %d",ud);
   }

   if ( ud != -1 ) // NI
   {
      //check for GPIB error
      if( !GetErrorStatus() ) // if an error
      {
         //System error or Nonexistent GPIB board
         if((iberr == EDVR) || (iberr == ENEB))
         {
            throw  "Interface board not installed or properly configured [GetErrorStatus]" ;
         }
      }
      else
      {
         // A valid descriptor is found. It is a valid device attached to NI card.
         short listen = 0;

         ibln(ud, _primaryAddress, _secondaryAddress, &listen);

         // Device is not present
         if(!listen)
         {
            char msg[75];
            sprintf_s(msg,"Device not found or online at address GPIB%d::%d::%d::INSTR",_boardId,_primaryAddress,_secondaryAddress);
            throw msg ;
         }
      }
   }
   else
   {
      throw  "Interface board not installed or properly configured" ;
   }

   //if device is online, set timeout for this device for all future gpib transactions.
   SetTimeOut(timeoutVal);

    return true;
#else

    char szDeviceAddress[128] = {0};
    ionerror(err_handler);
    sprintf_s(szDeviceAddress,128, "gpib%d,%d", _boardId, _primaryAddress);
    //sprintf_s(szDeviceAddress,128,"GPIB%d::%d::INSTR",_boardId, _primaryAddress);
    m_dwDevice = iopen(szDeviceAddress);
    itimeout(m_dwDevice, 10000);

    return true;
#endif 
  
}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::Close
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CAgilentGPIBController::Close()
{
    if ( m_dwDevice != -1 )
    {
        ilocal(m_dwDevice);
        _siclcleanup();
        m_dwDevice = -1;
    }
   return true;
}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::ClearDevice
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CAgilentGPIBController::ClearDevice()
{
   if ( WAIT_OBJECT_0!=WaitForSingleObject(m_criticalSection,90000) )
      return false;

   iclear(m_dwDevice);
   ReleaseMutex(m_criticalSection);
   return true;

}

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::SetTimeOut
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
// bool CAgilentGPIBController::SetTimeOut(EnumTimeOut TimeOut)
// {
//    if ( GetTimeOut() != TimeOut )
//    {
//       ibtmo(ud, ConvertTimeOutValue(TimeOut));
//       if ( !GetErrorStatus() )
//          return true;
//       else
//          return false;
//    }
//    return true;
// }

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::GetTimeOut
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
// CAgilentGPIBController::EnumTimeOut CAgilentGPIBController::GetTimeOut()
// {
//    EnumTimeOut timeout_s = T_30S;
// 
//    int buff = 0;
//    ibask(ud, IbaTMO, &buff);
//    timeout_s = (EnumTimeOut)buff;
// 
//    return timeout_s;
// }

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::ConvertTimeOutValue
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
// int CAgilentGPIBController::ConvertTimeOutValue(EnumTimeOut TimeOut)
// {
//    switch ( TimeOut )
//    {
//       case T_DISABLE:      return TNONE;
//       case T_10US:         return T10us;
//       case T_30US:         return T30us;
//       case T_100US:        return T100us;
//       case T_300US:        return T300us;
//       case T_1MS:          return T1ms;
//       case T_3MS:          return T3ms;
//       case T_10MS:         return T10ms;
//       case T_30MS:         return T30ms;
//       case T_100MS:        return T100ms;
//       case T_300MS:        return T300ms;
//       case T_1S:           return T1s;
//       case T_3S:           return T3s;
//       case T_10S:          return T10s;
//       case T_30S:          return T30s;
//       case T_100S:         return T100s;
//       case T_300S:         return T300s;
//       case T_1000S:        return T1000s;
//       case T_NO_CHANGE:
//       default:             return -1;
//    }
// }

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::GetErrorStatus
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
// bool CAgilentGPIBController::GetErrorStatus()
// {
//    if ( ibsta & ERR )
//       return false;
//    else
//       return true;
// }

/******************************************************************************
 * FUNCTION: CAgilenGPIBController::GetTimeOutFromConfig
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
/*CAgilentGPIBController::EnumTimeOut CAgilentGPIBController::GetTimeOutFromConfig()
{
   EnumTimeOut gpibTimeout = T_30S;
   long timeoutSec = 30;
#ifdef QSEQ_UTILITY_QUALCOMM_USE_DLL
   if (CGlobalRepositoryFactory::getGlobalRepository()->itemExists(g_configID_StationConfigGPIBTimeout_s))
   {
      timeoutSec = CGlobalRepositoryFactory::getGlobalRepository()->getLong(g_configID_StationConfigGPIBTimeout_s);
   }
#endif
   if ( timeoutSec <= 1 )//minimum is 1 sec
      gpibTimeout = T_1S;
   else if ( timeoutSec <= 3 )
      gpibTimeout = T_3S;
   else if ( timeoutSec <= 10 )
      gpibTimeout = T_10S;
   else if ( timeoutSec <= 30 )
      gpibTimeout = T_30S;
   else if ( timeoutSec <= 100 )
      gpibTimeout = T_100S;
   else if ( timeoutSec <= 300)
      gpibTimeout = T_300S;
   else
      gpibTimeout = T_1000S;

   return gpibTimeout;
}*/
