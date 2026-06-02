/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_VISA.cpp#23 $
 * $DateTime: 2018/03/16 14:49:39 $
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

#include <JGW_NIGPIBControllImpl/equipcommtl_VISA.h>
#include <sstream>

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The constructor opens up both the VISA driver handle and device instruction handle. Both are required.
CEquipCommTL_VISA::CEquipCommTL_VISA(const std::string& visaAddress):
   m_visaAddress(visaAddress),
   m_timeout(10000),
   m_isPresent(false)
{
   initVisaStatusStr();

   // open up the resource manager and an Instrument session
   // More instrument sessions can be opened later with the same resource manager
   OpenResourceMgr();
   OpenInstrSession();

   // Set the write terminator to EOI for GPIB and VXI, set to "\n" for all others
   unsigned int writeTermChar;
   switch(GetInterfaceType())
   {
   case VI_INTF_GPIB:
   case VI_INTF_VXI:
   case VI_INTF_GPIB_VXI:
      writeTermChar = 0x00;
      break;
   case VI_INTF_ASRL:
	   if("INSTR" == GetResourceName())
      {
         writeTermChar = 0x0A;
         break;
      }
   case VI_INTF_TCPIP:
      if("INSTR" == GetResourceName())
      {
         writeTermChar = 0x00;
         break;
      }
   case VI_INTF_USB:
   default:
      writeTermChar = 0x0A;
      break;
   }
   SetWriteTerminator(writeTermChar);

   // Set the read terminator to "\n"
   SetReadTerminator(0x0A);

   // set to true since instrument session is opened successfully
   m_isPresent = true;
}

// The destructor closes both the VISA driver and instrument handles. Must be done in order shown.
CEquipCommTL_VISA::~CEquipCommTL_VISA()
{
   // viClose handles the resources, no need to check for VI_NULL or status before closing
   viClose(m_instrHandle);
   viClose(m_defaultRM);
}

bool CEquipCommTL_VISA::CloseSession()
{
    viClose(m_instrHandle);
    viClose(m_defaultRM);
    return true;
}

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

bool CEquipCommTL_VISA::OpenResourceMgr()
{
   ViStatus status = 0;
   // opening up the connection to the VISA driver; return VISA handle
   try
   {
      status = viOpenDefaultRM(&m_defaultRM);
   }
   catch(...)
   {
      Log4WE(L"OpenResourceMgr:Cannot open VISA resource manager. The VISA drivers may not be installed properly.");
       return false;
   }
   if (status != VI_SUCCESS)
   {
      m_VisaStatusStr.defaultRM_stat = false;
      return false;
   }
   m_VisaStatusStr.defaultRM_stat = true;
   return true;
}

bool CEquipCommTL_VISA::OpenInstrSession()
{
   ViStatus status = 0;
   // opening up the connection to the VISA device; return device instrument handle
   // VI_NULL indicates default access mode, use m_timeout
   status = viOpen(m_defaultRM, const_cast<ViRsrc>(m_visaAddress.c_str()), VI_NULL, m_timeout, &m_instrHandle);
   if (status != VI_SUCCESS)
   {
      m_VisaStatusStr.instr_stat = false;
      return false;
   }

   m_VisaStatusStr.instr_stat = true;

   // Set timeout attribute to match m_timeout property
   status = viSetAttribute(m_instrHandle, VI_ATTR_TMO_VALUE, m_timeout);
   if (status != VI_SUCCESS)
      return false;

   return true;
}

void CEquipCommTL_VISA::CheckSessionStatus()
{
//    ostringstream ostr;
//    ostr.str("");

   if(!m_VisaStatusStr.defaultRM_stat)
   {
       Log4WE(L"CheckSessionStatus:Cannot open VISA resource manager.");
      //ostr << "Cannot open VISA resource manager."<<__FUNCTION__;
/*      throw CApplicationException( g_exc_general_exception, ostr.str() );*/
   }

   if (!m_VisaStatusStr.instr_stat)
   {
       Log4WE_F(L"CheckSessionStatus : Cannot open VISA instrument session named: %s",m_visaAddress.c_str());
    //  ostr << "Cannot open VISA instrument session named: " << m_visaAddress << ", in " <<__FUNCTION__;
/*      throw CApplicationException( g_exc_general_exception, ostr.str() );*/
   }
}

bool CEquipCommTL_VISA::write(const std::string& cmd)
{
   const void* buffer = reinterpret_cast<const void*>(cmd.c_str());
   return write( const_cast<void*>(buffer), cmd.length());
}

bool CEquipCommTL_VISA::write( void * buffer, long count )
{
   CheckSessionStatus();

   ViStatus status = 0;
   ViUInt32 bytesWritten = 0;

   std::string sBuffer(reinterpret_cast<char*>(buffer));
   switch( m_writeTerminator )
   {
   case 0x0D: // append "\r"
      sBuffer += "\r";
      count++;
      buffer = const_cast<void*>(reinterpret_cast<const void*>(sBuffer.c_str()));
      break;
   case 0x0A: // append "\n"
      sBuffer += "\n";
      count++;
      buffer = const_cast<void*>(reinterpret_cast<const void*>(sBuffer.c_str()));
      break;
   default:
	  // do nothing
      break;
   }

   status = viWrite(m_instrHandle, (ViBuf)(buffer), static_cast<ViUInt32>(count), &bytesWritten);
   if (status != VI_SUCCESS)
   {
      m_VisaStatusStr.write_stat = false;
      return false;
   }
   m_VisaStatusStr.write_stat = true;

   // write to tpl window
//    CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(m_visaAddress+std::string(";")+sBuffer,EVENT_CLASS_IEEE488_WR);
//    CTimeProfiler::getInstance()->addEvent(timeStamp);
   return true;
}

bool CEquipCommTL_VISA::read(std::string& retVal)
{
   CheckSessionStatus();
   retVal.clear();
   ViStatus status = VI_SUCCESS_MAX_CNT;
   ViUInt32 bytesRead = 0;
   char * tempBuff = new char[maxBufferSize];

   while( status == VI_SUCCESS_MAX_CNT )
   {
      memset( tempBuff, 0, maxBufferSize );
      status = viRead(m_instrHandle, (ViPBuf)tempBuff, maxBufferSize, &bytesRead);
     retVal.append( tempBuff, bytesRead );  
   }

   if( status == VI_SUCCESS || status == VI_SUCCESS_TERM_CHAR )
   {
      m_VisaStatusStr.read_stat = true;
   }
   else // error condition
   {
      m_VisaStatusStr.read_stat = false;
   }

   delete [] tempBuff;
   tempBuff = nullptr;

   // write to tpl window
//    CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(m_visaAddress+std::string(";")+retVal,EVENT_CLASS_IEEE488_RD);
//    CTimeProfiler::getInstance()->addEvent(timeStamp);
   return m_VisaStatusStr.read_stat;
}

bool CEquipCommTL_VISA::read( void *buffer, long bufferSize )
{
   long bytesRead = 0;
   return read( buffer, bufferSize, bytesRead );
}

bool CEquipCommTL_VISA::read( void *buffer, long bufferSize, long &iBytesRead )
{
   CheckSessionStatus();

   ViStatus status = 0;
   ViUInt32 bytesRead = 0;

   // disable terminating char for binary transfers
   viSetAttribute(m_instrHandle,VI_ATTR_TERMCHAR_EN, VI_FALSE);

   status = viRead(m_instrHandle, (ViBuf)buffer, bufferSize, &bytesRead);

   // enable reading terminating character for nonbinary reads
   viSetAttribute(m_instrHandle,VI_ATTR_TERMCHAR_EN, VI_TRUE);

   if( status != VI_SUCCESS && status != VI_SUCCESS_TERM_CHAR && status != VI_SUCCESS_MAX_CNT )
   {
      m_VisaStatusStr.read_stat = false;
      return false;
   }
   iBytesRead = (long)bytesRead;
   m_VisaStatusStr.read_stat = true;

   // write to tpl window
//    CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(m_visaAddress+std::string(";BIN_RD"),EVENT_CLASS_IEEE488_RD);
//    CTimeProfiler::getInstance()->addEvent(timeStamp);
   return true;
}

bool CEquipCommTL_VISA::SetTimeOut(double timeoutSec)
{
   CheckSessionStatus();

   m_timeout = static_cast<unsigned long>(timeoutSec*1000);
   ViStatus status = viSetAttribute(m_instrHandle, VI_ATTR_TMO_VALUE, m_timeout);

   if (status != VI_SUCCESS)
      return false;

   return true;
}

bool CEquipCommTL_VISA::SetReadTerminator(unsigned int terminator)
{
   CheckSessionStatus();
   ViStatus status;

   switch( terminator )
   {
   case 0x0D: // set the read terminator to "\r"
   case 0x0A: // set the read terminator to "\n"
      m_readTerminator = terminator;
      break;
   default:
      m_readTerminator = 0;
      break;
   }

   if( m_readTerminator != 0 )
   {
      status = viSetAttribute(m_instrHandle,VI_ATTR_TERMCHAR, m_readTerminator);
      if (status < VI_SUCCESS)
         return false;

      // enable the terminating character
      status = viSetAttribute(m_instrHandle,VI_ATTR_TERMCHAR_EN, VI_TRUE);
      if (status != VI_SUCCESS)
         return false;
   }
   else
   {
      // disable the terminating character, this is the default per Visa documentation
      status = viSetAttribute(m_instrHandle,VI_ATTR_TERMCHAR_EN, VI_FALSE);
      if (status != VI_SUCCESS)
         return false;
   }

   return true;
}

bool CEquipCommTL_VISA::SetWriteTerminator(unsigned int terminator)
{
   CheckSessionStatus();
   ViStatus status;
   ViAttrState assert_eoi;

   switch( terminator )
   {
   case 0x0D: // "\r"
   case 0x0A: // "\n"
      // Set the write terminator. This will be appended to all write commands.
      m_writeTerminator = terminator;

      // Prevent EOI sent on viWrite
      assert_eoi = VI_FALSE;
      break;

   default:
      // Do not append anything to write commands.
      m_writeTerminator = 0;

      // Set the terminator to EOI, this is default per Visa documentation
      assert_eoi = VI_TRUE;
      break;
   }

   status = viSetAttribute(m_instrHandle,VI_ATTR_SEND_END_EN,assert_eoi);
   if (status != VI_SUCCESS)
      return false;

   return true;
}

short CEquipCommTL_VISA::GetInterfaceType()
{
   short sVisaInterfaceType;
   viGetAttribute(m_instrHandle, VI_ATTR_INTF_TYPE, &sVisaInterfaceType);
   return sVisaInterfaceType;
}

std::string CEquipCommTL_VISA::GetResourceName()
{
   char sResourceName[32] = "";
   viGetAttribute(m_instrHandle, VI_ATTR_RSRC_CLASS, sResourceName);
   return sResourceName;
}

void CEquipCommTL_VISA::query(const std::string& cmd,std::string& retVal)
{
   write(cmd);
   read(retVal);
}

bool CEquipCommTL_VISA::query_ok( const std::string & cmd, std::string & retVal )
{
    return (write(cmd) && read(retVal));
}

bool CEquipCommTL_VISA::query( const char* buffer, float& fVal )
{
    std::string retVal;
    if (write((void*)buffer,strlen(buffer)) && read(retVal))
    {
        fVal = static_cast<float>(atof(retVal.c_str()));
    }
    return false;
}

// not using
bool CEquipCommTL_VISA::SerialPoll( char& buffer )
{
   buffer; // warning fix: unreferenced formal parameter
   CheckSessionStatus();

   return false;
}

bool CEquipCommTL_VISA::ClearDevice()
{
   CheckSessionStatus();

   viClear(m_instrHandle);
   return true;
}
