/******************************************************************************
* $Header: //depot/HTE/QDART/SUITE/Utilities/PacketTranslator/PacketTranslator.cpp#9 $
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


// StringsExperiment.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "PacketTranslator.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>



/*#include "utilities\StringManipulator\StringManipulator.h"*/


///////////////////////////////////////////////////////////////////////
//
//      CONSTRUCTOR  &  DESTRUCTOR
//
///////////////////////////////////////////////////////////////////////

CPacketTranslator::CPacketTranslator() :
m_param_delim(','),
	m_type_val_delim(':'),
	m_pdiag_pkt_str(NULL)
{
	Init();
}

CPacketTranslator::CPacketTranslator( char paramDelim, char typeValDelim ) :
m_param_delim(paramDelim),
	m_type_val_delim(typeValDelim),
	m_pdiag_pkt_str(NULL)
{
	Init();
}


void CPacketTranslator::Init( void )
{
	////////////////////////////////////////////////////////////////
	// Create the vector of supported data types
	// Get their sizes into the map

	// support for BYTE type
	CreateTypeSizeMap( "BYTE", (DWORD)sizeof(BYTE) );
	m_signed_map.insert( SignedMap::value_type( "BYTE", false ) ); // unsigned

	// support for WORD type
	CreateTypeSizeMap( "WORD", (DWORD)sizeof(WORD) );
	m_signed_map.insert( SignedMap::value_type( "WORD", false ) ); // unsigned

	// support for DWORD type
	CreateTypeSizeMap( "DWORD", (DWORD)sizeof(DWORD) );
	m_signed_map.insert( SignedMap::value_type( "DWORD", false ) ); // unsigned

	// support for BOOLEAN type
	CreateTypeSizeMap( "BOOLEAN", (DWORD)sizeof(BOOLEAN) );
	m_signed_map.insert( SignedMap::value_type( "BOOLEAN", false ) ); // unsigned

	// support for "short int" type
	CreateTypeSizeMap( "SHORTINT", (DWORD)sizeof(short int) );
	m_signed_map.insert( SignedMap::value_type( "SHORTINT", true ) ); // signed

	// support for "long int" type
	CreateTypeSizeMap( "LONGINT", (DWORD)sizeof(long int) );
	m_signed_map.insert( SignedMap::value_type( "LONGINT", true ) ); // signed
}

CPacketTranslator::~CPacketTranslator()
{
	// Make sure that whatever memory was allocated
	// for m_pDiagPktString is freed up
	DeleteString();
}

CPacketTranslator::CPacketTranslator(const CPacketTranslator& src):
m_param_delim(src.m_param_delim),
	m_type_val_delim(src.m_type_val_delim)
{
	packetLen = src.packetLen;
	memcpy_s(m_pdiag_pkt_str, packetLen * sizeof(char), src.m_pdiag_pkt_str, src.packetLen * sizeof(char));
}

CPacketTranslator& CPacketTranslator::operator=(const CPacketTranslator& src)
{
	if(&src == this)
		return *this;
	packetLen = src.packetLen;
	memcpy_s(m_pdiag_pkt_str, packetLen * sizeof(char), src.m_pdiag_pkt_str, src.packetLen * sizeof(char));
	return *this;
}

////////////////////////////////////////////////////////////////
//
//
//      M E T H O D S
//
//
////////////////////////////////////////////////////////////////

char* CPacketTranslator::FormatDiagPkt(LPCSTR diagPkt, DWORD& pktLen)
{
	int offset = 0;
	TypeSizeMapIt tsm_it;

	// fill in the type and data value(s) info
	ParseString(diagPkt);

	// calculate the length of the required buffer, and allocate it
	AllocateString( pktLen = GetPktSize() );

	if ( m_pdiag_pkt_str )
	{
		for (   StrVecIt tv_it = m_type_vector.begin(), vv_it = m_val_vector.begin();
			tv_it != m_type_vector.end() && vv_it != m_val_vector.end();
			++tv_it, ++vv_it)
		{
			/////////////////////////////////
			//  Go through the type and value
			//   vectors and add the values to
			//   the string representation of
			//   data packet according to the
			//   type size
			tsm_it = m_type_size_map.find( *tv_it );
			if ( tsm_it == m_type_size_map.end() )
			{
				// the type was not found
				DeleteString();
				DeleteVectors();
				break;
			}
			else
			{
				// the type was found in the map and is supported
				AddToPkt( atol( (*vv_it).c_str() ), tsm_it->second, offset );
				offset += tsm_it->second;
			}
		}
	}   // m_pDiagPktString is not NULL
	else
		DeleteVectors();

	return m_pdiag_pkt_str;
}

void CPacketTranslator::UnpackDiagPktToStrings(LPCSTR diagPktFmtString, unsigned char* pktBuf, DWORD pktLen , std::vector< std::pair<std::string,std::string> >& unpackedVec)
{
	int offset = 0;
	TypeSizeMapIt tsm_it;
	std::vector<std::string> itemString;
	std::vector<std::string> fmtStrings /*= CStringManipulator::Parse( string(diagPktFmtString), m_param_delim )*/;
	char szBuffer[2] = {0};
	szBuffer[0] = m_param_delim;
	JGW::ParserStrA(diagPktFmtString,szBuffer,fmtStrings);
	// clear the output vector
	unpackedVec.clear();

	// loop through each SIZE:NAME entry and add it to the unpacked string vector
	for( unsigned int i = 0; i < fmtStrings.size(); ++i )
	{
		// seperate the size from the name
		szBuffer[0] = m_type_val_delim;
		JGW::ParserStrA(diagPktFmtString,szBuffer,itemString);

		if( itemString.size() == 2 )  // size must be 2 or there is a problem
		{
			offset += AddToUnpackedString(pktBuf, offset, itemString[0].c_str(), itemString[1].c_str(), unpackedVec);
		}
		else
		{
			_ASSERT(FALSE);
		}
	} // end for
}

void CPacketTranslator::AddToPkt(DWORD data, DWORD dataSize, int offset)
{
	memcpy_s((void*) (m_pdiag_pkt_str + offset), sizeof(StrVecIt), (void *) &data, dataSize );
}

int CPacketTranslator::AddToUnpackedString(unsigned char* pktBuf, int offset, LPCSTR strSize, LPCSTR name, std::vector< std::pair<std::string,std::string> >& unpackedVec )
{
	int size = 0;
	DWORD temp = 0;
	TypeSizeMapIt tsm_it = m_type_size_map.find( strSize );
	SignedMapIt sm_it = m_signed_map.find( strSize );

	if ( tsm_it != m_type_size_map.end() && sm_it != m_signed_map.end() )
	{
		size = tsm_it->second;
		bool isSigned = sm_it->second;

		// just mem-copy into the largest item supported
		memcpy_s(&temp, sizeof(DWORD), (void*)(pktBuf + offset), size);

		if( isSigned ) // need to sign-extend
		{
			DWORD bitCheck = 1 << (size*8 - 1);
			if( bitCheck & temp ) // if the high bit is set
			{
				DWORD signExt = !(bitCheck-1); // bitCheck-1 makes everything to the right of the set bit flip, the ~ will flip all bits, resulting in all ones to the left of the high bit
				temp |= signExt;
			}
		}

		std::ostringstream ostr;
		ostr<<temp;
		unpackedVec.push_back( std::make_pair<std::string, std::string>(name, ostr.str()) );
	}
	else
	{
		_ASSERT(FALSE); // throw
	}

	return size;
}

void CPacketTranslator::AddToVector(int start, int stop)
{
	/*   CString t = m_str.Mid(start, stop - start);*/
	std::string t = m_str.substr(start, stop - start);
	int typeValDelimPos = t.find( m_type_val_delim );
	StrVecIt it;
	char whiteSpace = ' ';

	m_type_vector.push_back( t.substr(0, typeValDelimPos) );
	m_val_vector.push_back( t.substr(typeValDelimPos + 1) );


	it = m_type_vector.end() - 1;
	//it->Remove(whiteSpace);
	JGW::eraseChar(it[0],whiteSpace);
	JGW::MakestringToUpper(it[0]);
	//it->MakeUpper();

	it = m_val_vector.end() - 1;
	JGW::eraseChar(it[0],whiteSpace);
	JGW::MakestringToUpper(it[0]);

	//  it->Remove(whiteSpace);
	//  it->MakeUpper();

}

void CPacketTranslator::AllocateString(DWORD numBytes)
{
	// just clear up space, we only hold one packet at a time
	DeleteString();
	if ( numBytes )
	{
		m_pdiag_pkt_str = new char[numBytes];
		packetLen = numBytes;
	}
}

void   CPacketTranslator::CreateTypeSizeMap( LPCSTR type, DWORD typeSize)
{
	// put the data type information and size into the map
	m_type_size_map.insert( TypeSizeMap::value_type( type, typeSize ) );
}

void CPacketTranslator::DeleteString( void )
{
	if (m_pdiag_pkt_str)
	{
		delete[] m_pdiag_pkt_str;
		m_pdiag_pkt_str = nullptr;
	}
}

void CPacketTranslator::DeleteVectors( void )
{
	m_val_vector.erase(m_val_vector.begin(), m_val_vector.end());
	m_type_vector.erase(m_type_vector.begin(), m_type_vector.end());
}

DWORD CPacketTranslator::GetPktSize( void )
{
	DWORD pktSize = 0;
	TypeSizeMapIt it;   // map iterator

	if ( !m_type_vector.empty() )
	{
		// traverse the vector only if it is not empty
		for( StrVecIt m_it = m_type_vector.begin(); m_it != m_type_vector.end(); m_it++)
		{

			/////////////////////////////////
			// find the data type

			it = m_type_size_map.find( *m_it );
			if ( it == m_type_size_map.end() )
			{
				// the type information was not found (or not supported) -- error
				pktSize = 0;
				break;
			}
			else
			{
				// the type was successfully found
				pktSize += it->second;
			}
		}
	}

	return pktSize;
}

DWORD CPacketTranslator::ParseString(LPCSTR string2parse)
{

	m_str = string2parse;

	if( !(m_type_vector.empty() && m_val_vector.empty()) ) DeleteVectors();
	int
		firstExtChrPos = 0,   // start position of the extractor
		lastExtChrPos = 0;   //   end position of the extractor

	while( ( lastExtChrPos = m_str.find( m_param_delim, firstExtChrPos ) ) != -1 )
	{
		AddToVector(firstExtChrPos, lastExtChrPos);
		firstExtChrPos = lastExtChrPos + 1;
	}

	AddToVector( firstExtChrPos, m_str.length() );

	return (DWORD) m_str.length();
}
