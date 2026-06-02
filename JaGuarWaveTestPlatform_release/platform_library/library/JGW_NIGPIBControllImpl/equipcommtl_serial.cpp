/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_serial.cpp#10 $
 * $DateTime: 2016/04/04 18:48:45 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2012-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#include "stdafx.h"
//#include "Utilities\xmldatalogging\timeprofiler.h"
#include <JGW_NIGPIBControllImpl\EquipCommTL_Serial.h>
#include <sstream>
//#include "X-Functional\Drivers\ieee488\EquipCommTL_Serial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef unsigned char byte;
//////////////////////////////////////////////////////////////////////
// static initializers
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEquipCommTL_Serial::CEquipCommTL_Serial(int comPortNumber):
   m_forcedQueryPreIdentifier("{\""),
   m_forcedQueryPostIdentifier("\"}"),
   m_comPortNumber(comPortNumber),
   m_timeout(30000)
{
   m_forcedQueryPreSize=m_forcedQueryPreIdentifier.size();
   m_forcedQueryPostSize=m_forcedQueryPostIdentifier.size();
   m_isPresent=true;
   std::ostringstream ostr;
   ostr<<"COM"<<comPortNumber;
   m_dev_addr=ostr.str();

   try
   {
      wchar_t szcommname[16];
      //ULONG   bRet = 0;
      COMMTIMEOUTS to;
      DCB dcb;
      wsprintf(szcommname,L"\\\\.\\COM%d",comPortNumber);
      m_handle=::CreateFile(szcommname,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);

      if( m_handle!=INVALID_HANDLE_VALUE )
      {

         // Get DCB state
         ::GetCommState(m_handle, &dcb);
         ::SetCommState(m_handle, &dcb);

         memset(&to,0,sizeof(to));
         // Get the curren timeout status
         ::GetCommTimeouts(m_handle, &to);

         //Set Timeout
         to.ReadIntervalTimeout=20;
         to.ReadTotalTimeoutMultiplier=0;
         to.ReadTotalTimeoutConstant=5000;
         to.WriteTotalTimeoutMultiplier=0;
         to.WriteTotalTimeoutConstant=5000;

         ::SetCommTimeouts(m_handle, &to);

      }

      m_error=::GetLastError();
   }
   catch(...)
   {
      m_handle=INVALID_HANDLE_VALUE;
   }
}

CEquipCommTL_Serial::~CEquipCommTL_Serial()
{
   if(m_handle!=INVALID_HANDLE_VALUE)
   {
      ::CloseHandle(m_handle);
   }
}

bool CEquipCommTL_Serial::write(const std::string& cmd)
{
   bool status=false;
   DWORD bytesWritten;
   std::string command=cmd+"\r";

   if(m_handle==INVALID_HANDLE_VALUE)
   {
      return status;
   }

   HANDLE hEventOverlapped=CreateEvent(0,true,false,0);
   OVERLAPPED overlapped;
   memset(&overlapped,0,sizeof(overlapped));
   overlapped.hEvent=hEventOverlapped;
   ResetEvent(hEventOverlapped);

   if(false==(status=(TRUE == ::WriteFile(m_handle,command.c_str(),command.length(),NULL,&overlapped))))
   {
      m_error=::GetLastError();
      if((ERROR_SUCCESS==m_error)||(ERROR_IO_PENDING==m_error))
      {
         // Allow 1ms per byte
         switch(::WaitForSingleObject(hEventOverlapped,command.length()))
         {
            case WAIT_OBJECT_0:
               GetOverlappedResult(m_handle,&overlapped,&bytesWritten,FALSE);
               status=bytesWritten==command.length();
               break;
            default:
               CancelIo(m_handle);
               break;
         }
      }
   }

   CloseHandle(overlapped.hEvent);

   // write to tpl window
   //CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(m_dev_addr+string(";")+cmd,EVENT_CLASS_IEEE488_WR);
   //CTimeProfiler::getInstance()->addEvent(timeStamp);

   return status;
}

bool CEquipCommTL_Serial::write(void *buffer,long count)
{
   //send 'count' bytes from the supplied buffer
   bool status = false;
   DWORD bytesWritten;

   if(m_handle==INVALID_HANDLE_VALUE)
   {
      return status;
   }

   HANDLE hEventOverlapped=CreateEvent(0,true,false,0);
   OVERLAPPED overlapped;
   memset(&overlapped,0,sizeof(overlapped));
   overlapped.hEvent=hEventOverlapped;
   ResetEvent(hEventOverlapped);

   if(false==(status=(TRUE == ::WriteFile(m_handle,buffer,count,NULL,&overlapped))))
   {
      m_error=::GetLastError();
      if((ERROR_SUCCESS==m_error)||(ERROR_IO_PENDING==m_error))
      {
         // Allow 1ms per byte
         switch(::WaitForSingleObject(hEventOverlapped,count))
         {
            case WAIT_OBJECT_0:
               GetOverlappedResult(m_handle,&overlapped,&bytesWritten,FALSE);
               status = (bytesWritten==static_cast<DWORD>(count));
               break;
            default:
               CancelIo(m_handle);
               break;
         }
      }
   }

   // write to tpl window
   std::ostringstream ostr;
   ostr << m_dev_addr << ";";
   byte* ptr = reinterpret_cast<byte*>(buffer);
   for(int i=0; i<10; i++,ptr++)
   {
      if(i == count)
      {
         // No more data
         break;
      }
      ostr << "0x" << std::hex << int(*ptr) << ", ";
   }
   if(count>10)
   {
      ostr << "...";
   }

   CloseHandle(overlapped.hEvent);

   //CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(ostr.str(),EVENT_CLASS_IEEE488_WR);
   //CTimeProfiler::getInstance()->addEvent(timeStamp);

   return status;
}

bool CEquipCommTL_Serial::read(std::string& retVal)
{
   bool status = false;
   DWORD bytesRead;

   retVal="";

   if(m_handle==INVALID_HANDLE_VALUE)
   {
      return status;
   }

   HANDLE hEventOverlapped=CreateEvent(0,true,false,0);
   OVERLAPPED overlapped;
   memset(&overlapped,0,sizeof(overlapped));
   overlapped.hEvent=hEventOverlapped;
   char dataByte;

   do
   {
      ResetEvent(hEventOverlapped);
      // Pull in the data, one byte at a time
      if(false==(status= (TRUE == ::ReadFile(m_handle,&dataByte,1,NULL,&overlapped))))
      {
         m_error=::GetLastError();
         if((ERROR_SUCCESS==m_error)||(ERROR_IO_PENDING==m_error))
         {
            switch(::WaitForSingleObject(hEventOverlapped,m_timeout))
            {
               case WAIT_OBJECT_0:
                  GetOverlappedResult(m_handle,&overlapped,&bytesRead,FALSE);
                  status=bytesRead==1;
                  break;
               default:
                  CancelIo(m_handle);
                  break;
            }
         }
      }
      if(status)
      {
         if((dataByte!='\n')&&(dataByte!='\r'))
         {
            retVal+=dataByte;
         }
      }
   }
   while(status&&(dataByte!='\r'));

   // write to tpl window
   //CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(m_dev_addr+string(";")+retVal,EVENT_CLASS_IEEE488_RD);
   //CTimeProfiler::getInstance()->addEvent(timeStamp);

   CloseHandle(overlapped.hEvent);

   return status;
}

bool CEquipCommTL_Serial::read( void * buffer, long bufferSize )
{
   long totalBytesRead = 0; // keep track of total bytes read
   return read( buffer, bufferSize, totalBytesRead);
}

bool CEquipCommTL_Serial::read(void *buffer,long bufferSize, long &totalBytesRead)
{
   bool status = false;
   DWORD bytesRead;

   if(m_handle==INVALID_HANDLE_VALUE)
   {
      return status;
   }

   HANDLE hEventOverlapped=CreateEvent(0,true,false,0);
   OVERLAPPED overlapped;
   memset(&overlapped,0,sizeof(overlapped));
   overlapped.hEvent=hEventOverlapped;

   SetTimeOut(1); // set the timeout to 1 second
   DWORD timePassed = 0;
   do
   {
      DWORD start = GetTickCount();
      ResetEvent(hEventOverlapped);
      // Pull in the data, one byte at a time
      if(false==(status= (TRUE == ::ReadFile(m_handle,(byte*)buffer+totalBytesRead,1,NULL,&overlapped))))
      {
         m_error=::GetLastError();
         if((ERROR_SUCCESS==m_error)||(ERROR_IO_PENDING==m_error))
         {
            switch(::WaitForSingleObject(hEventOverlapped,m_timeout))
            {
               case WAIT_OBJECT_0:
                  GetOverlappedResult(m_handle,&overlapped,&bytesRead,FALSE);
                  status=bytesRead==1;
                  break;
               default:
                  CancelIo(m_handle);
                  break;
            }
         }
      }
      DWORD end = GetTickCount();
      timePassed += end - start;
      ++totalBytesRead;
   }
   while( totalBytesRead < bufferSize && timePassed < GetTimeOut()*1000 );

   // write to tpl window
   std::ostringstream ostr;
   ostr << m_dev_addr << ";";
   byte* ptr = reinterpret_cast<byte*>(buffer);
   for(int i=0; i<10; i++,ptr++)
   {
      if(i == bufferSize)
      {
         // No more data
         break;
      }
      ostr << "0x" << std::hex << int(*ptr) << ", ";
   }
   if(bufferSize>10)
   {
      ostr << "...";
   }

   CloseHandle(overlapped.hEvent);

   //CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(ostr.str(),EVENT_CLASS_IEEE488_RD);
   //CTimeProfiler::getInstance()->addEvent(timeStamp);

   return status;
}

void CEquipCommTL_Serial::query( const std::string& cmd, std::string& retVal )
{
   // check to see if command is in between {" and "}
   if( cmd.substr( 0, m_forcedQueryPreSize ) == m_forcedQueryPreIdentifier &&
         cmd.substr( (int)cmd.size() > m_forcedQueryPostSize ? cmd.size() - m_forcedQueryPostSize : cmd.size(),  m_forcedQueryPostSize ) == m_forcedQueryPostIdentifier )
   {
      // this is a "forced query". We don't really send a command to the box, we just return a string literal from the command spreadsheet
      retVal = cmd.substr( m_forcedQueryPreSize, cmd.size() - (m_forcedQueryPreSize + m_forcedQueryPostSize) );
   }
   else
   {
      write( cmd );
      read( retVal );
   }
}

bool CEquipCommTL_Serial::query_ok( const std::string & cmd, std::string & retVal )
{
	return ( write(cmd) && read(retVal) );
}

bool CEquipCommTL_Serial::query( const char* buffer, float& fVal )
{
	std::string retVal;
	fVal = 0.00f;

	if ( write((void*)buffer,strlen(buffer)) && read(retVal) )
	{
		fVal =  static_cast<float>(atof(retVal.c_str()));
		return true;
	}

	return false;
}

bool CEquipCommTL_Serial::CloseSession()
{
	//bool status = m_pInterface->Close();
	if(m_handle!=INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_handle);
	}
	return true;
}
