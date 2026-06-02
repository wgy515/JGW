/*!
 *********************************************************************
 *  $Id: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtlv1_0_HIVE.h#9 $
 *
 *  Project : QCT SUITE
 *
 *  Package : CEquipCommTLV1_0_HIVE
 *
 *  Company : Qualcomm Technologies Incorporated
 *
 *  Purpose : Declaration of CEquipCommTLV1_0_HIVE
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

#if !defined(__CEQUIPCOMTLV1_0_HIVE_H__INCLUDED__)
#define __CEQUIPCOMTLV1_0_HIVE_H__INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IEquipCommTL.h"
#include <memory>
using namespace std;

class CGPIBController;

//!class CEquipCommTLV1_0_HIVE
class CEquipCommTLV1_0_HIVE : public IEquipCommTL
{
public:

   explicit CEquipCommTLV1_0_HIVE( int address, int secAddress = 96 , int boardId = 0);
   virtual ~CEquipCommTLV1_0_HIVE();
   virtual bool write( const string& cmd );
   virtual bool write( void * buffer, long count );

   virtual bool read( string& retVal );
   virtual bool read( void * buffer, long bufferSize );
   virtual bool read( void * buffer, long bufferSize, long &bytesRead );

   virtual void query( const string& cmd, string& retVal );

   virtual bool query( const char* buffer, float& fVal );

   virtual bool query_ok( const std::string & cmd, std::string & retVal );

   const string m_forcedQueryPreIdendifier;
   const string m_forcedQueryPostIdendifier;
   int m_forcedQueryPreSize;
   int m_forcedQueryPostSize;

   virtual bool IsPresent();
   virtual bool GetErrorStatus();
   virtual bool ClearDevice();
   virtual bool CloseSession();
   virtual bool SerialPoll( char& buffer );

   virtual bool SetTimeOut( double timeoutSec);
   virtual double GetTimeOut();
   virtual int GetDeviceAddress();

protected:
   CGPIBController * m_pInterface;

   char m_buffer[65536];
   bool m_isPresent;
};

#endif // !defined(AFX_CEquipCommTLV1_0_HIVE_H__CF7FA5F1_8DC2_11D5_B6C9_00508B72B0D2__INCLUDED_)
