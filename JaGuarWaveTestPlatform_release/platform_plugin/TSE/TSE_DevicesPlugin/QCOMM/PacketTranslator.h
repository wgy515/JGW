/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/Utilities/PacketTranslator/PacketTranslator.h#9 $
 * $DateTime: 2016/04/04 18:48:26 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2007-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <Windows.h>
//#include "utilities\SUITE_STL\STLInclude.h"

//////////////////////////////////////
//
// Typedefs
//
//////////////////////////////////////

//////////////////////////////////////
//   vector of strings

typedef std::vector< std::string >           StringVector;
typedef std::vector< std::string >::iterator StrVecIt;

///////////////////////////////////////
//   vector of pointers to const strings

typedef std::vector< LPCSTR >           LPCTSTRVector;
typedef std::vector< LPCSTR >::iterator LPCTSTRVectorIt;

//////////////////////////////////////
//   vector of DWORDS

typedef std::vector< DWORD >           DWORDVector;
typedef std::vector< DWORD >::iterator DWORDVectorIt;


//////////////////////////////////////
//   map of data types vs sizes

typedef std::map< std::string, DWORD >           TypeSizeMap;
typedef std::map< std::string, DWORD >::iterator TypeSizeMapIt;


//////////////////////////////////////
//   map of data types vs signed/unsigned (true for signed)

typedef std::map< std::string, bool >           SignedMap;
typedef std::map< std::string, bool >::iterator SignedMapIt;

class CPacketTranslator
{



////////////////////////////////////////////////////////////////
//
//
//      C O N S T R U C T O R S  &  D E S T R U C T O R S
//
//
////////////////////////////////////////////////////////////////

public:
   CPacketTranslator();
   CPacketTranslator( char paramDelim, char typeValDelim );
   virtual   ~CPacketTranslator();
   CPacketTranslator(const CPacketTranslator& src);
   CPacketTranslator& operator=(const CPacketTranslator& src);


////////////////////////////////////////////////////////////////
//
//
//      M E M B E R   V A R I A B L E S
//
//
////////////////////////////////////////////////////////////////

private:
   std::string            m_str;
   TypeSizeMap				m_type_size_map;
   SignedMap				m_signed_map;
//   LPCTSTRVector       //m_SupportedTypes;
//   DWORDVector         m_SupportedTypesSize;
   StringVector
   m_val_vector,   // vector of values in the packet
   m_type_vector;   // vector of types in the packet

   const char				m_param_delim;
   const char				m_type_val_delim;
   char*					m_pdiag_pkt_str;
   DWORD					packetLen;
////////////////////////////////////////////////////////////////
//
//
//      M E T H O D S
//
//
////////////////////////////////////////////////////////////////

public:
   virtual char* FormatDiagPkt(LPCSTR diagPkt, DWORD& pktLen);

   /*!
      Unpacks a buffer based on the description provided in diagPkt.  For example:
         if
           diagPktFmtString = "BYTE:therm_min,BYTE:therm_max"
           pktBuf = 0x12 0xE0
           pktLen = 2
          then
            unpackedVec = {"therm_min","18"},{"therm_min","224"}
   */
   virtual void UnpackDiagPktToStrings(LPCSTR diagPktFmtString, unsigned char* pktBuf, DWORD pktLen , std::vector< std::pair<std::string,std::string> >& unpackedVec);

private:
   void   AddToPkt(DWORD data, DWORD dataSize, int offset = 0);
   int AddToUnpackedString(unsigned char* pktBuf, int offset, LPCSTR strSize, LPCSTR name, std::vector< std::pair<std::string,std::string> >& unpackedVec );
   void   AddToVector(int start, int stop);
   void   AllocateString(DWORD numBytes);
   void   Init( void );

   void   CreateTypeSizeMap( LPCSTR type, DWORD typeSize);

   void   DeleteString(void);
   void   DeleteVectors(void);

   DWORD  GetPktSize(void);

   DWORD  ParseString(LPCSTR string2parse);

};
