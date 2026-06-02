/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/GPIBController.cpp#14 $
 * $DateTime: 2015/10/28 14:56:58 $
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
#include "stdafx.h"

#include <stdio.h>
#include <JGW_NIGPIBControllImpl/GPIBController.h>

/******************************************************************************
 * FUNCTION: CGPIBController::CGPIBController
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
CGPIBController::CGPIBController(int primAddr , int boardId/* = 0*/, int secAddr/* = 96*/) :
   _primaryAddress(primAddr),
   _secondaryAddress(secAddr),
   _boardId(boardId),
   ud(-1),
   m_criticalSection(CreateMutex(NULL,FALSE,L"CTSTIMutex"))
{
}

/******************************************************************************
 * FUNCTION: CGPIBController::~CGPIBController
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
CGPIBController::~CGPIBController()
{
}

/******************************************************************************
 * FUNCTION: CGPIBController::GetDeviceAddress
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
int CGPIBController::GetDeviceAddress()
{
   return _primaryAddress;
}

/******************************************************************************
 * FUNCTION: CGPIBController::GetID
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
int CGPIBController::GetID()
{
   return ud;
}

/******************************************************************************
 * FUNCTION: CGPIBController::Send
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CGPIBController::Send(const char* buf, long iLen)
{
   if ( WAIT_OBJECT_0!=WaitForSingleObject(m_criticalSection,90000) )
      return false;

   ibwrt(ud,const_cast<char*>(buf),iLen);

   if ( !GetErrorStatus() )
   {
      ReleaseMutex(m_criticalSection);
      return false;
   }
   ReleaseMutex(m_criticalSection);

   return true;
}

/******************************************************************************
 * FUNCTION: CGPIBController::Receive
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CGPIBController::Receive(char* buffer,int size)
{
   ZeroMemory( buffer, size );

   if ( WAIT_OBJECT_0!=WaitForSingleObject(m_criticalSection,90000) )
      return false;

   ibrd(ud, buffer, size);
   if ( !GetErrorStatus() )
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
 * FUNCTION: CGPIBController::Open
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
//Auto Test Functionality
bool CGPIBController::Open()
{
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
}

/******************************************************************************
 * FUNCTION: CGPIBController::Close
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CGPIBController::Close()
{
   if ( ud != -1 )
   {
      ibonl(ud, 0);
      ud = -1;
   }
   return true;
}

/******************************************************************************
 * FUNCTION: CGPIBController::ClearDevice
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CGPIBController::ClearDevice()
{
   if ( WAIT_OBJECT_0!=WaitForSingleObject(m_criticalSection,90000) )
      return false;

   ibclr(ud);
   ReleaseMutex(m_criticalSection);
   return true;

}

/******************************************************************************
 * FUNCTION: CGPIBController::SetTimeOut
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CGPIBController::SetTimeOut(EnumTimeOut TimeOut)
{
   if ( GetTimeOut() != TimeOut )
   {
      ibtmo(ud, ConvertTimeOutValue(TimeOut));
      if ( !GetErrorStatus() )
         return true;
      else
         return false;
   }
   return true;
}

/******************************************************************************
 * FUNCTION: CGPIBController::GetTimeOut
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
CGPIBController::EnumTimeOut CGPIBController::GetTimeOut()
{
   EnumTimeOut timeout_s = T_30S;

   int buff = 0;
   ibask(ud, IbaTMO, &buff);
   timeout_s = (EnumTimeOut)buff;

   return timeout_s;
}

/******************************************************************************
 * FUNCTION: CGPIBController::ConvertTimeOutValue
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
int CGPIBController::ConvertTimeOutValue(EnumTimeOut TimeOut)
{
   switch ( TimeOut )
   {
      case T_DISABLE:      return TNONE;
      case T_10US:         return T10us;
      case T_30US:         return T30us;
      case T_100US:        return T100us;
      case T_300US:        return T300us;
      case T_1MS:          return T1ms;
      case T_3MS:          return T3ms;
      case T_10MS:         return T10ms;
      case T_30MS:         return T30ms;
      case T_100MS:        return T100ms;
      case T_300MS:        return T300ms;
      case T_1S:           return T1s;
      case T_3S:           return T3s;
      case T_10S:          return T10s;
      case T_30S:          return T30s;
      case T_100S:         return T100s;
      case T_300S:         return T300s;
      case T_1000S:        return T1000s;
      case T_NO_CHANGE:
      default:             return -1;
   }
}

/******************************************************************************
 * FUNCTION: CGPIBController::GetErrorStatus
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
bool CGPIBController::GetErrorStatus()
{
   if ( ibsta & ERR )
      return false;
   else
      return true;
}

/******************************************************************************
 * FUNCTION: CGPIBController::GetTimeOutFromConfig
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
CGPIBController::EnumTimeOut CGPIBController::GetTimeOutFromConfig()
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
}
