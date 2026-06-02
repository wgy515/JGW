/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_tcp_ip.cpp#14 $
 * $DateTime: 2016/06/02 13:04:09 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2008-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#include "stdafx.h"
#include <JGW_NIGPIBControllImpl\equipcommtl_tcp_ip.h>
#include <sstream>
/*#include "utilities\xmldatalogging\timeprofiler.h"*/
//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// static initializers
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEquipCommTL_TCP_IP::CEquipCommTL_TCP_IP(const std::string& ip_address , const int& inst_number,const std::string& strSourceAddress /* = "" */) :
   m_forcedQueryPreIdendifier("{\""),
   m_forcedQueryPostIdendifier("\"}"),
   m_dev_addr(ip_address),
   m_timeout(10000)
{
    m_buffer = (char*)calloc(0x01,maxBufferSize);
   m_forcedQueryPreSize = m_forcedQueryPreIdendifier.size();
   m_forcedQueryPostSize = m_forcedQueryPostIdendifier.size();

   // Create a new instance of the Network Instrument Protocol class and
   // fill out the link creation parameters structure
   m_pInterface=new CNetworkInstrumentProtocol();
   Create_LinkParms parms;
   parms.clientId=3735928559;
   parms.lockDevice=false;
   parms.lock_timeout=0;
   parms.device=ip_address;
   parms.inst=inst_number;
   parms.sourceAddress = strSourceAddress;

   // Attempt to create the link
   Create_LinkResp resp=m_pInterface->create_link(parms);

   // If link creation was successful then store the lid
   if(resp.error==0)
   {
      m_lid=resp.lid;
      m_isPresent = true;
   }
   else
   {
      m_lid=0;
      m_isPresent = false;
   }
}

CEquipCommTL_TCP_IP::~CEquipCommTL_TCP_IP()
{
    if (m_buffer) free(m_buffer);
   // Destroy the link, delete the instance of the class and invalidate the lid.
   m_pInterface->destroy_link(m_lid);
   delete m_pInterface;
   m_pInterface = nullptr;
   m_lid=0;
}

bool CEquipCommTL_TCP_IP::CloseSession()
{
    m_pInterface->destroy_link(m_lid);
    delete m_pInterface;
    m_pInterface = nullptr;
    return true;
}

CEquipCommTL_TCP_IP::CEquipCommTL_TCP_IP( const CEquipCommTL_TCP_IP& src)
{
    m_buffer = (char*)calloc(0x01,maxBufferSize);
   m_pInterface = src.m_pInterface;
   memcpy_s(m_buffer, sizeof(char) * maxBufferSize, src.m_buffer, sizeof(char) * maxBufferSize);
   m_isPresent = src.m_isPresent;
   m_lid = src.m_lid;
   m_dev_addr = src.m_dev_addr;
   m_timeout = src.m_timeout;
}

CEquipCommTL_TCP_IP& CEquipCommTL_TCP_IP::operator=(const CEquipCommTL_TCP_IP& src)
{
   if(&src == this)
      return *this;
   m_pInterface = src.m_pInterface;
   m_buffer = (char*)calloc(0x01,maxBufferSize);
   memcpy_s(m_buffer, sizeof(char) * maxBufferSize, src.m_buffer, sizeof(char) * maxBufferSize);
   m_isPresent = src.m_isPresent;
   m_lid = src.m_lid;
   std::string m_dev_addr;
   m_timeout = src.m_timeout;
   return *this;
}

bool CEquipCommTL_TCP_IP::write( const std::string& cmd )
{
   bool status = false;
   Device_WriteParms parms;
   struct dataBlock_type
   {
      long size;
      char cmd[maxBufferSize];
   };
   dataBlock_type* dataBlock=new dataBlock_type;

   // Fill out the write parameters
   parms.lid=m_lid;
   parms.flags=0x08; /* Send the END character (0x0A) */
   parms.io_timeout=m_timeout;
   parms.lock_timeout=0;
   dataBlock->size=cmd.length();
   strncpy_s(dataBlock->cmd,cmd.c_str(),maxBufferSize);
   parms.data=reinterpret_cast<unsigned char*>(dataBlock);

   // Attempt to write the data
   Device_WriteResp resp=m_pInterface->device_write(parms);

   // Check for errors and that all bytes were sent.
   status=(0==resp.error)&&(static_cast<unsigned long>(dataBlock->size)==resp.size);

   delete dataBlock;


   // write to tpl window
//    CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(m_dev_addr+std::string(";")+ cmd,EVENT_CLASS_IEEE488_WR);
//    CTimeProfiler::getInstance()->addEvent(timeStamp);

   return status;
}

bool CEquipCommTL_TCP_IP::write( void * buffer, long count )
{
   buffer; // warning fix: unreferenced formal parameter
   count; // warning fix: unreferenced formal parameter
   return true;
}

bool CEquipCommTL_TCP_IP::read( std::string& retVal )
{
   bool status = false;
   Device_ReadParms parms;

   // Fill out the parameter structure
   parms.requestSize=maxBufferSize;
   parms.flags=0;
   parms.io_timeout=m_timeout;
   parms.lid=m_lid;
   parms.lock_timeout=0;
   parms.termChar=0x0A;

   // Attempt to read in data
   Device_ReadResp resp=m_pInterface->device_read(parms);

   // Check for error and copy the return std::string over in the abscence of any error
   if(resp.error==0)
   {
      status = true;
      strcpy_s(m_buffer,maxBufferSize,reinterpret_cast<const char*>(resp.data+sizeof(long)));
     // strncpy_s(m_buffer,maxBufferSize,reinterpret_cast<const char*>(resp.data+sizeof(long)),sizeof(m_buffer));
      retVal = static_cast<std::string>(m_buffer);
   }

   // write to tpl window
//    CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(m_dev_addr+std::string(";")+retVal,EVENT_CLASS_IEEE488_RD);
//    CTimeProfiler::getInstance()->addEvent(timeStamp);

   return status;
}

bool CEquipCommTL_TCP_IP::read( void *buffer, long bufferSize )
{
   bool status = false;
   Device_ReadParms parms;

   // Fill out the parameter structure
   parms.requestSize=bufferSize;
   parms.flags=0;
   parms.io_timeout=m_timeout + static_cast<unsigned long>(bufferSize/2000); // Timeout + 1ms for every 2000 bytes
   parms.lid=m_lid;
   parms.lock_timeout=0;
   parms.termChar=0x0A;

   // Attempt to read in data
   Device_ReadResp resp=m_pInterface->device_read(parms);

   // Check for error and copy the return data over in the abscence of any error
   if(resp.error==0)
   {
      status = true;
      memcpy(buffer,reinterpret_cast<const char*>(resp.data+sizeof(long)),*reinterpret_cast<unsigned long*>(resp.data));

      // write to tpl window
      std::ostringstream ostr;
      ostr << m_dev_addr << ";";
      char* ptr=reinterpret_cast<char*>(resp.data+sizeof(long));
      for(int i=0; i<10; i++,ptr++)
      {
         if(reinterpret_cast<long>(resp.data)<10)
         {
            // No more data
            break;
         }
         ostr << "0x" << std::ios_base::hex << int(*ptr) << ", ";
      }
      ostr << "...";
//       CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(ostr.str(),EVENT_CLASS_IEEE488_RD);
//       CTimeProfiler::getInstance()->addEvent(timeStamp);
   }

   return status;
}

bool CEquipCommTL_TCP_IP::read( void *buffer, long bufferSize, long &bytesRead )
{
   bytesRead = bufferSize;
   return read(buffer, bufferSize);
}

bool CEquipCommTL_TCP_IP::IsPresent()
{
   return m_isPresent;
}

void CEquipCommTL_TCP_IP::query( const std::string& cmd, std::string& retVal )
{
   // check to see if command is in between {" and "}
   if( cmd.substr( 0, m_forcedQueryPreSize ) == m_forcedQueryPreIdendifier &&
         cmd.substr( (int)cmd.size() > m_forcedQueryPostSize ? cmd.size() - m_forcedQueryPostSize : cmd.size(),  m_forcedQueryPostSize ) == m_forcedQueryPostIdendifier )
   {
      // this is a "forced query". We don't really send a command to the box, we just return a std::string literal from the command spreadsheet
      retVal = cmd.substr( m_forcedQueryPreSize, cmd.size() - (m_forcedQueryPreSize + m_forcedQueryPostSize) );
   }
   else
   {
      write( cmd );
      read( retVal );
   }
}

bool CEquipCommTL_TCP_IP::query_ok( const std::string & cmd, std::string & retVal )
{
    return (write(cmd) && read(retVal));
}

bool CEquipCommTL_TCP_IP::query(const char* buffer, float& fVal)
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

bool CEquipCommTL_TCP_IP::GetErrorStatus()
{
   // TODO:
   bool status = false;
   return status;
}

bool CEquipCommTL_TCP_IP::ClearDevice()
{
   // TODO:
   bool status = false;
   return status;
}

bool CEquipCommTL_TCP_IP::SerialPoll( char& buffer )
{
   buffer; // warning fix: unreferenced formal parameter
   // this probably can't be supported by TCP/IP equip
   bool status = false ;
   return status;
}

bool CEquipCommTL_TCP_IP::SetTimeOut(double timeoutSec)
{
   m_timeout = static_cast<unsigned long>(timeoutSec*1000);

   return true;
}
