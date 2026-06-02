/*!
 *********************************************************************
 *  $Id: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtlv1_0_HIVE.cpp#13 $
 *
 *  Project : QCT SUITE
 *
 *  Package :
 *
 *  Company : Qualcomm Technologies Incorporated
 *
 *  Purpose : Implementation of methods for CEquipCommTLV1_0_HIVE
 *
 *********************************************************************
<b>Version History:</b>
\verbatim
V0.1	07/xx/2003	bcheadle	First Revision
V0.2	09/xx/2003	MTruscott	Added IsPresent()
V0.3	02/12/2004	bcheadle	Added support for forced queries (fixed return values, no equip communication)
\endverbatim
 *********************************************************************
 */

#include "stdafx.h"
#include <JGW_NIGPIBControllImpl/equipcommtlv1_0_HIVE.h>
#include <JGW_NIGPIBControllImpl/GPIBController.h>
#include <sstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
using namespace JGW;
#ifdef __SUITE__HIVE__
   #include "utilities\xmldatalogging\timeprofiler.h"
#endif
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
CEquipCommTLV1_0_HIVE::CEquipCommTLV1_0_HIVE( int address, int secAddress, int boardId ) : 
   m_pInterface( new CGPIBController(address,boardId,secAddress) ), 
   m_forcedQueryPreIdendifier("{\""), 
   m_forcedQueryPostIdendifier("\"}")
{
   m_forcedQueryPreSize = m_forcedQueryPreIdendifier.size();
   m_forcedQueryPostSize = m_forcedQueryPostIdendifier.size();
   m_isPresent = true;
   Log4WD_F(L"boardId: %d,address: %d,secAddress: %d",boardId,address,secAddress);
   m_pInterface->Open();
}

CEquipCommTLV1_0_HIVE::~CEquipCommTLV1_0_HIVE()
{
	if( m_pInterface )
	{
		m_pInterface->Close();
		delete m_pInterface;
		m_pInterface = nullptr;
	}
}

bool CEquipCommTLV1_0_HIVE::write( const string& cmd )
{
   const void* buffer = reinterpret_cast<const void*>(cmd.c_str());
   return write( const_cast<void*>(buffer), cmd.length());
}

bool CEquipCommTLV1_0_HIVE::write( void * buffer, long count )
{
   //buffer; // warning fix: unreferenced formal parameter
   //count; // warning fix: unreferenced formal parameter
   //return true;
   const char* cBuffer = reinterpret_cast<char*>(buffer);
   bool status = m_pInterface->Send( cBuffer, count );
   Log4WD(JGW_A2W_A(cBuffer).c_str());
   int pad = m_pInterface->GetDeviceAddress();
   ostringstream ostr;
   ostr << pad << ";" << string(cBuffer);
   
#ifdef __SUITE__HIVE__
   CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(ostr.str(),EVENT_CLASS_IEEE488_WR);
   CTimeProfiler::getInstance()->addEvent( timeStamp );
#endif
   return status;
}

bool CEquipCommTLV1_0_HIVE::read( string& retVal )
{
   bool status = m_pInterface->Receive(m_buffer,sizeof( m_buffer ));
   retVal = static_cast<string>(m_buffer);
   Log4WD(JGW_A2W_A(m_buffer).c_str());

   int pad = m_pInterface->GetDeviceAddress();
   ostringstream ostr;
   ostr << pad << ";" << retVal;
#ifdef __SUITE__HIVE__
   CTimeProfiler::CTimestamp timeStamp = CTimeProfiler::CTimestamp(ostr.str(),EVENT_CLASS_IEEE488_RD);
   CTimeProfiler::getInstance()->addEvent( timeStamp );
#endif
   return status;
}

bool CEquipCommTLV1_0_HIVE::read( void * buffer, long bufferSize )
{
   bool status = m_pInterface->Receive(static_cast<char*>(buffer), bufferSize);
   Log4WD(JGW_A2W_A(static_cast<const char*>(buffer)).c_str());
   return status;
}

bool CEquipCommTLV1_0_HIVE::read( void *buffer, long bufferSize, long &bytesRead )
{
   bool status = read(buffer, bufferSize);
   char* cBuffer = static_cast<char*>(buffer);
   bytesRead = strlen(cBuffer);
   return status;
}

bool CEquipCommTLV1_0_HIVE::IsPresent()
{
   return m_isPresent;
}

bool CEquipCommTLV1_0_HIVE::query_ok( const std::string & cmd, std::string & retVal )
{
	return ( write(cmd) && read(retVal) );
}

bool CEquipCommTLV1_0_HIVE::query( const char* buffer, float& fVal )
{
	string retVal;
	fVal = 0.00f;

	if ( write((void*)buffer,strlen(buffer)) && read(retVal) )
	{
		fVal =  static_cast<float>(atof(retVal.c_str()));
		return true;
	}

	return false;
}

void CEquipCommTLV1_0_HIVE::query( const string& cmd, string& retVal )
{
   // check to see if command is in between {" and "}
   if( cmd.substr( 0, m_forcedQueryPreSize ) == m_forcedQueryPreIdendifier &&
         cmd.substr( (int)cmd.size() > m_forcedQueryPostSize ? cmd.size() - m_forcedQueryPostSize : cmd.size(),  m_forcedQueryPostSize ) == m_forcedQueryPostIdendifier )
   {
      retVal = cmd.substr( m_forcedQueryPreSize, cmd.size() - (m_forcedQueryPreSize + m_forcedQueryPostSize) );
   }
   else
   {
      write( cmd );
      read( retVal );
   }
}

bool CEquipCommTLV1_0_HIVE::GetErrorStatus()
{
   bool status = m_pInterface->GetErrorStatus();
   return status;
}

bool CEquipCommTLV1_0_HIVE::ClearDevice()
{
   bool status = m_pInterface->ClearDevice();
   return status;
}

bool CEquipCommTLV1_0_HIVE::CloseSession()
{
	bool status = m_pInterface->Close();
	return status;
}

bool CEquipCommTLV1_0_HIVE::SerialPoll( char& buffer )
{
   bool status = false ;
   status = ibrsp(m_pInterface->GetID(), &buffer) ? true : false;  // read serial poll
   return status;
}

bool CEquipCommTLV1_0_HIVE::SetTimeOut( double timeoutSec )
{
   CGPIBController::EnumTimeOut gpibTimeout;

   if ( timeoutSec == -1 )
      gpibTimeout = CGPIBController::T_NO_CHANGE;
   else if ( timeoutSec == 0 )
      gpibTimeout = CGPIBController::T_DISABLE;
   else if ( timeoutSec <= 0.00001 )
      gpibTimeout = CGPIBController::T_10US;
   else if ( timeoutSec <= 0.00003 )
      gpibTimeout = CGPIBController::T_30US;
   else if ( timeoutSec <= 0.0001 )
      gpibTimeout = CGPIBController::T_100US;
   else if ( timeoutSec <= 0.0003 )
      gpibTimeout = CGPIBController::T_300US;
   else if ( timeoutSec == 0.001 )
      gpibTimeout = CGPIBController::T_1MS;
   else if ( timeoutSec <= 0.003 )
      gpibTimeout = CGPIBController::T_3MS;
   else if ( timeoutSec <= 0.01 )
      gpibTimeout = CGPIBController::T_10MS;
   else if ( timeoutSec <= 0.03 )
      gpibTimeout = CGPIBController::T_30MS;
   else if ( timeoutSec <= 0.1 )
      gpibTimeout = CGPIBController::T_100MS;
   else if ( timeoutSec <= 0.3 )
      gpibTimeout = CGPIBController::T_300MS;
   else if ( timeoutSec <= 1.0 )
      gpibTimeout = CGPIBController::T_1S;
   else if ( timeoutSec <= 3.0 )
      gpibTimeout = CGPIBController::T_3S;
   else if ( timeoutSec <= 10.0 )
      gpibTimeout = CGPIBController::T_10S;
   else if ( timeoutSec <= 30.0 )
      gpibTimeout = CGPIBController::T_30S;
   else if ( timeoutSec <= 100.0 )
      gpibTimeout = CGPIBController::T_100S;
   else if ( timeoutSec <= 300.0 )
      gpibTimeout = CGPIBController::T_300S;
   else
      gpibTimeout = CGPIBController::T_1000S;

   bool status = m_pInterface->SetTimeOut(gpibTimeout);
   return status;
}

double CEquipCommTLV1_0_HIVE::GetTimeOut()
{
   CGPIBController::EnumTimeOut gpibTimeout = m_pInterface->GetTimeOut();
   double timeoutSec;

   switch ( gpibTimeout )
   {
   case CGPIBController::T_NO_CHANGE:
      timeoutSec = -1;
      break;
   case CGPIBController::T_DISABLE:
      timeoutSec = 0;
      break;
   case CGPIBController::T_10US:
      timeoutSec = 0.00001;
      break;
   case CGPIBController::T_30US:
      timeoutSec = 0.00003;
      break;
   case CGPIBController::T_100US:
      timeoutSec = 0.0001;
      break;
   case CGPIBController::T_300US:
      timeoutSec = 0.0003;
      break;
   case CGPIBController::T_1MS:
      timeoutSec = 0.001;
      break;
   case CGPIBController::T_3MS:
      timeoutSec = 0.003;
      break;
   case CGPIBController::T_10MS:
      timeoutSec = 0.01;
      break;
   case CGPIBController::T_30MS:
      timeoutSec = 0.03;
      break;
   case CGPIBController::T_100MS:
      timeoutSec = 0.1;
      break;
   case CGPIBController::T_300MS:
      timeoutSec = 0.3;
      break;
   case CGPIBController::T_1S:
      timeoutSec = 1.0;
      break;
   case CGPIBController::T_3S:
      timeoutSec = 3.0;
      break;
   case CGPIBController::T_10S:
      timeoutSec = 10.0;
      break;
   case CGPIBController::T_30S:
      timeoutSec = 30.0;
      break;
   case CGPIBController::T_100S:
      timeoutSec = 100.0;
      break;
   case CGPIBController::T_300S:
      timeoutSec = 300.0;
      break;
   case CGPIBController::T_1000S:
      timeoutSec = 1000.0;
      break;
   }

   return timeoutSec;
}


int CEquipCommTLV1_0_HIVE::GetDeviceAddress( void )
{
   return m_pInterface->GetDeviceAddress();
}

// void CEquipCommTLV1_0_HIVE::SetLog4IxPtr( void* pIxLogWriter )
// {
//	g_pIxLog = static_cast<x3::Object<Ix_LogWriter>>pIxLogWriter;
//}