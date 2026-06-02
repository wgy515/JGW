/******************************************************************************
* $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_usrdll.cpp#6 $
* $DateTime: 2016/06/17 14:21:36 $
*
* DESCRIPTION: CEquipCommTL_UsrDll
******************************************************************************
*
* Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
* All rights reserved.
* Qualcomm Technologies, Inc. Confidential and Proprietary.
*
******************************************************************************
*/
#include "stdafx.h"
//#include "equipcommtl_usrdll.h"
#include <JGW_NIGPIBControllImpl\equipcommtl_usrdll.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
// #include "Utilities\GlobalRepository\iglobalrepository.h"
// #include "utilities\UI\uimanagerfactory.h"
// #include "utilities\ApplicationException\ApplicationException.h"


/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::CEquipCommTL_UsrDll
*
* DESCRIPTION: See header file
*****************************************************************************/
CEquipCommTL_UsrDll::CEquipCommTL_UsrDll(const EquipmentDefinition & def) :
mDllHandle(nullptr),
	mSessionHandle(nullptr)
	//,mUi(CUIManagerFactory::GetUIManager())

{
	Log4WD_F(L"User transport driver: %s - %s",JGW::JGW_A2W(def.identifier).c_str(),JGW::JGW_A2W(def.usrDllFilePath).c_str());
	//mUi->PrintToDebug("User transport driver: " + def.identifier + " - " + def.usrDllFilePath);

	// Load the DLL
	std::string msg;
	mDllHandle = LoadAssembly(def.usrDllFilePath.c_str(),msg);
	if(!mDllHandle)
	{
		throw std::string("GENERAL_EXCEPTION : ") + msg;//CApplicationException(g_exc_general_exception, msg);
	}

	// Map exported interfaces
	if(!InitializeExportDecl(mDllHandle,mDllExported))
	{
		throw std::string("GENERAL_EXCEPTION : One or more required interfaces are not exported from the user DLL.");
		/*throw CApplicationException(g_exc_general_exception,
			"One or more required interfaces are not exported from the user DLL.");*/
	}

	// Create the session
	mSessionHandle = CreateSession(def);
	if(!mSessionHandle)
	{
		throw std::string("GENERAL_EXCEPTION : Failed to create user transport driver session.");
		/*
		throw CApplicationException(g_exc_general_exception,
			"Failed to create user transport driver session.");
			*/
	}   
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::GetDeviceAddress
*
* DESCRIPTION: See header file
*****************************************************************************/
CEquipCommTL_UsrDll::~CEquipCommTL_UsrDll()
{
	// Release the session handle
	mDllExported.instrument_release_handle(&mSessionHandle);

	// Unload the DLL
	FreeLibrary(mDllHandle);
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::write
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::write( const std::string& cmd )
{
	return write((void*)cmd.c_str(),cmd.length());
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::write
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::write( void * buffer, long count )
{
	bool success = false;
	if(mSessionHandle && mDllExported.instrument_write)
	{
		success = (count==mDllExported.instrument_write(mSessionHandle,buffer,count));
	}
	return success;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::read
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::read( std::string& retVal )
{
	static const unsigned int BUF_SIZE = 65536;
	char buffer [BUF_SIZE] = {0};   

	bool result = read((void*)buffer,BUF_SIZE);
	if(result)
	{
		retVal = buffer;
	}
	return result;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::read
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::read( void * buffer, long bufferSize )
{
	bool success = false;
	if(mSessionHandle && mDllExported.instrument_read)
	{
		success = true;
		mDllExported.instrument_read(mSessionHandle,buffer,bufferSize);
	}
	return success;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::read
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::read( void * buffer, long bufferSize, long &bytesRead )
{
	bool status = read(buffer, bufferSize);
	char* cBuffer = static_cast<char*>(buffer);
	bytesRead = strlen(cBuffer);
	return status;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::query
*
* DESCRIPTION: See header file
*****************************************************************************/
void CEquipCommTL_UsrDll::query( const std::string & cmd, std::string & retVal )
{
	if( write(cmd) )
	{
		read( retVal );
	}
}

bool CEquipCommTL_UsrDll::query_ok( const std::string & cmd, std::string & retVal )
{
	return write(cmd) && read(retVal);
}

bool CEquipCommTL_UsrDll::query( const char* buffer, float& fVal )
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

bool CEquipCommTL_UsrDll::CloseSession()
{
	return true;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::IsPresent
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::IsPresent()
{
	bool isPresent = false;
	if(mSessionHandle && mDllExported.instrument_get_status)
	{
		isPresent = (0==mDllExported.instrument_get_status(mSessionHandle));
	}
	return isPresent;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::GetErrorStatus
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::GetErrorStatus()
{
	bool status = false;
	if(mSessionHandle && mDllExported.instrument_get_status)
	{
		status = (0!=mDllExported.instrument_get_status(mSessionHandle));
	}
	return status;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::ClearDevice
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::ClearDevice()
{
	bool success = false;
	if(mSessionHandle && mDllExported.instrument_clear_device)
	{
		success = true;
		mDllExported.instrument_clear_device(mSessionHandle);
	}
	return success;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::SerialPoll
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::SerialPoll( char& buffer )
{
	// No implementation
	buffer;
	return false;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::SetTimeOut
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::SetTimeOut( double timeoutSec )
{
	bool success = false;
	if(mSessionHandle && mDllExported.instrument_set_iface_timeout)
	{
		long timeoutMs = (long)(timeoutSec * 1000.0);
		success = (timeoutMs==mDllExported.instrument_set_iface_timeout(mSessionHandle,timeoutMs));
	}
	return success;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::GetTimeOut
*
* DESCRIPTION: See header file
*****************************************************************************/
double CEquipCommTL_UsrDll::GetTimeOut()
{
	double timeoutSec = 0.0;
	if(mSessionHandle && mDllExported.instrument_get_iface_timeout)
	{
		long timeoutMs = mDllExported.instrument_get_iface_timeout(mSessionHandle);
		timeoutSec = (double)(timeoutMs * 1000.0);
	}
	return timeoutSec;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::GetDeviceAddress
*
* DESCRIPTION: See header file
*****************************************************************************/
int CEquipCommTL_UsrDll::GetDeviceAddress()
{
	// No implementation
	return 0;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::LoadAssembly
*
* DESCRIPTION: See header file
*****************************************************************************/
HMODULE CEquipCommTL_UsrDll::LoadAssembly(const std::string & fileAndPath, std::string & msg)
{
	msg.clear();
	HMODULE ptr = LoadLibraryA(fileAndPath.c_str());
	if(!ptr)
	{
		msg = GetWin32Error();
	}
	return ptr;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::InitializeExportDecl
*
* DESCRIPTION: See header file
*****************************************************************************/
bool CEquipCommTL_UsrDll::InitializeExportDecl(HMODULE asmPtr, instrument_exported_func_decl & exported)
{
	if(!asmPtr)
	{
		return false;
	}

	exported.instrument_gpib_create_session = (instrument_gpib_create_session_fn)GetProcAddress(asmPtr,"instrument_gpib_create_session");
	exported.instrument_visa_create_session = (instrument_visa_create_session_fn)GetProcAddress(asmPtr,"instrument_visa_create_session");
	exported.instrument_tcpip_create_session = (instrument_tcpip_create_session_fn)GetProcAddress(asmPtr,"instrument_tcpip_create_session");
	exported.instrument_rs232_create_session = (instrument_rs232_create_session_fn)GetProcAddress(asmPtr,"instrument_rs232_create_session");
	exported.instrument_release_handle = (instrument_release_handle_fn)GetProcAddress(asmPtr,"instrument_release_handle");
	exported.instrument_write = (instrument_write_fn)GetProcAddress(asmPtr,"instrument_write");
	exported.instrument_read = (instrument_read_fn)GetProcAddress(asmPtr,"instrument_read");
	exported.instrument_get_status = (instrument_get_status_fn)GetProcAddress(asmPtr,"instrument_get_status");
	exported.instrument_clear_device = (instrument_clear_device_fn)GetProcAddress(asmPtr,"instrument_clear_device");
	exported.instrument_set_iface_timeout = (instrument_set_iface_timeout_fn)GetProcAddress(asmPtr,"instrument_set_iface_timeout");
	exported.instrument_get_iface_timeout = (instrument_get_iface_timeout_fn)GetProcAddress(asmPtr,"instrument_get_iface_timeout");

	// Check that we obtained addresses to all the expected export functions
	// At least one create_session function must exist.
	bool success = false;
	success |= (0!=exported.instrument_gpib_create_session);
	success |= (0!=exported.instrument_visa_create_session);
	success |= (0!=exported.instrument_tcpip_create_session);
	success |= (0!=exported.instrument_rs232_create_session);
	// Other mandatory interfaces
	success &= (0!=exported.instrument_release_handle);
	success &= (0!=exported.instrument_write);
	success &= (0!=exported.instrument_read);
	success &= (0!=exported.instrument_get_status);
	success &= (0!=exported.instrument_clear_device);
	success &= (0!=exported.instrument_set_iface_timeout);
	success &= (0!=exported.instrument_get_iface_timeout);

	return success;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::CreateSession
*
* DESCRIPTION: See header file
*****************************************************************************/
void * CEquipCommTL_UsrDll::CreateSession(const EquipmentDefinition & def)
{
	void * inst = nullptr;
	switch(def.comm_link_type)
	{
	case EQUIP_COMM_TYPE_GPIB:
		if(mDllExported.instrument_gpib_create_session)
		{
			inst = mDllExported.instrument_gpib_create_session(def.identifier.c_str(),def.address,def.secAddress,def.boardId);
		}
		break;
	case EQUIP_COMM_TYPE_TCP_IP:
		if(mDllExported.instrument_tcpip_create_session)
		{
			inst = mDllExported.instrument_tcpip_create_session(def.identifier.c_str(),def.ip_address.c_str(),def.inst_number);
		}
		break;
	case EQUIP_COMM_TYPE_SERIAL:
		if(mDllExported.instrument_rs232_create_session)
		{
			inst = mDllExported.instrument_rs232_create_session(def.identifier.c_str(),def.comPortNumber);
		}
		break;
	case EQUIP_COMM_TYPE_VISA:
		if(mDllExported.instrument_visa_create_session)
		{
			inst = mDllExported.instrument_visa_create_session(def.identifier.c_str(),def.visaAddress.c_str());
		}
		break;
	default:
		break;
	}

	return inst;
}

/******************************************************************************
* FUNCTION: CEquipCommTL_UsrDll::GetWin32Error
*
* DESCRIPTION: See header file
*****************************************************************************/
std::string CEquipCommTL_UsrDll::GetWin32Error() const
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);

	std::string errmsg = JGW::JGW_W2A_W((LPCTSTR)lpMsgBuf);   
	LocalFree( lpMsgBuf );  // Free the buffer.

	return errmsg;
}
