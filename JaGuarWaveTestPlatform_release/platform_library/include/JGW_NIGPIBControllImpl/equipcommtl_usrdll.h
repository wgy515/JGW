/******************************************************************************
* $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_usrdll.h#3 $
* $DateTime: 2015/10/28 14:56:58 $
*
* DESCRIPTION: CEquipCommTL_UsrDll
******************************************************************************
*
* Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
* All rights reserved.
* Qualcomm Technologies, Inc. Confidential and Proprietary.
*
******************************************************************************
*/
#pragma once

#include "IEquipCommTL.h"

class IUIManager;
//struct EquipmentDefinition;

class CEquipCommTL_UsrDll : public IEquipCommTL
{
public:

	/**
	* Constructor
	*/
	CEquipCommTL_UsrDll(const EquipmentDefinition & def);

	/**
	* Destructor
	*/
	virtual ~CEquipCommTL_UsrDll();

	/**
	* Write command
	* @param cmd Write a string.
	* @return bool TRUE if operation is successful, FALSE otherwise.
	*/
	virtual bool write( const std::string& cmd );

	/**
	* Write command
	* @param cmd Write a data buffer.
	* @param count The size of the data buffer.
	* @return bool TRUE if operation is successful, FALSE otherwise.
	*/
	virtual bool write( void * buffer, long count );

	/**
	* Read command
	* @param retVal Read a string.
	* @return bool TRUE if operation is successful, FALSE otherwise.
	*/
	virtual bool read( std::string& retVal );

	/**
	* Read command
	* @param buffer Read to a data buffer.
	* @param bufferSize The size of the data buffer.
	* @return bool TRUE if operation is successful, FALSE otherwise.
	*/
	virtual bool read( void * buffer, long bufferSize );

	/**
	* Read command
	* @param buffer Read to a data buffer.
	* @param bufferSize The size of the data buffer.
	* @param bytesRead Returns the number of bytes read from the interface
	* @return bool TRUE if operation is successful, FALSE otherwise.
	*/
	virtual bool read( void * buffer, long bufferSize, long &bytesRead );

	/**
	* Query command
	* @param cmd The query command string.
	* @param retVal The response string.
	*/
	virtual void query( const std::string & cmd, std::string & retVal );

	virtual bool query_ok( const std::string & cmd, std::string & retVal );

	virtual bool query( const char* buffer, float& fVal );

	virtual bool CloseSession();

	/**
	* Check to see if the instrument is present and communicating.
	* @return bool TRUE if instrument is present, FALSE otherwise.
	*/
	virtual bool IsPresent();

	/**
	* Get the error state of the instrument.
	* @return bool TRUE if there is no error, FALSE otherwise.
	*/
	virtual bool GetErrorStatus();

	/**
	* Get the error state of the instrument.
	* @return bool TRUE if there is no error, FALSE otherwise.
	*/
	virtual bool ClearDevice();

	/**
	* Reads the serial poll status.  This is specific to GPIB.
	* @return bool TRUE if there is no error, FALSE otherwise.
	*/
	virtual bool SerialPoll( char& buffer );

	/**
	* Set the interface timeout value.  The timeout value is the time
	* required for response after query.
	* @param timeoutSec The timeout value in seconds.
	* @return bool TRUE if operation was successful, FALSE otherwise.
	*/
	virtual bool SetTimeOut( double timeoutSec );

	/**
	* Get the interface timeout value.  The timeout value is the time
	* required for response after query.
	* @return double The timeout value in seconds.
	*/
	virtual double GetTimeOut();

	/**
	* Get the instrument interface address.  This is specific to GPIB.
	* @return int The address of the instrument.
	*/
	virtual int GetDeviceAddress();

private:
	//
	// Function pointers for DLL exported interfaces.
	//
	/**
	* Request to the driver to create a GPIB session.
	* @param identifier The name of the session.
	* @param pad The primary address of the instrument.
	* @param sad The secondary address of the instrument.
	* @param board_id The integer id of the board to use for the session.
	* @return void* The driver session object.
	*/
	typedef void*(_cdecl *instrument_gpib_create_session_fn)(const char * identifier, int pad, int sad, int board_id);
	/**
	* Request to the driver to create a VISA session.
	* @param identifier The name of the session.
	* @param address The address of the instrument.
	* @return void* The driver session object.
	*/
	typedef void*(_cdecl *instrument_visa_create_session_fn)(const char * identifier, const char * address);
	/**
	* Request to the driver to create a TCP/IP session.
	* @param identifier The name of the session.
	* @param address The address of the instrument.
	* @param inst_number An integer value which describes the instrument number.
	* @return void* The driver session object.
	*/
	typedef void*(_cdecl *instrument_tcpip_create_session_fn)(const char * identifier, const char * address, int inst_number);
	/**
	* Request to the driver to create a RS232 session.
	* @param identifier The name of the session.
	* @param address The COM port of the instrument.
	* @return void* The driver session object.
	*/
	typedef void*(_cdecl *instrument_rs232_create_session_fn)(const char * identifier, int port);
	/**
	* Release driver session resources.  This method should be safe
	* from NULL parameters.
	* @param handle The pointer to the driver session handle from one of the 
	* create_session interfaces.
	*/   
	typedef void(_cdecl *instrument_release_handle_fn)(void** handle);
	/**
	* Write a data buffer to the instrument.
	* @param handle The pointer to the driver session handle from one of the 
	* create_session interfaces.
	* @param buffer Pointer to the data buffer.
	* @param count The number of bytes contained in the data buffer.
	* @ return long The number of bytes successfully written to the instrument.
	*/
	typedef long(_cdecl *instrument_write_fn)(void * handle, const void * buffer, long count);
	/**
	* Read a data buffer from the instrument.
	* @param handle The pointer to the driver session handle from one of the 
	* create_session interfaces.
	* @param buffer Pointer to the data buffer.
	* @param count The size of the data buffer in bytes.
	* @param long The number of bytes that were read from the instrument.
	*/   
	typedef long(_cdecl *instrument_read_fn)(void * handle, void * buffer, long count);
	/**
	* Get the status of the instrument.
	* @param handle The pointer to the driver session handle from one of the 
	* create_session interfaces.
	* @ param long A non-zero value indicates an instrument error is present.
	*/   
	typedef long(_cdecl *instrument_get_status_fn)(void * handle);
	/**
	* Clear the device.
	* @param handle The pointer to the driver session handle from one of the 
	* create_session interfaces.
	*/   
	typedef void(_cdecl *instrument_clear_device_fn)(void * handle);
	/**
	* Sets the driver interface timeout value.
	* @param handle The pointer to the driver session handle from one of the 
	* create_session interfaces.
	* @param timeoutMs Timeout value in milliseconds to set to the instrument.
	* @return The timeout value in milliseconds of the instrument.  If operation was
	* successful, it should match the parameter 'timeoutMs'.
	*/
	typedef long(_cdecl *instrument_set_iface_timeout_fn)(void * handle, long timeoutMs);
	/**
	* Gets the driver interface timeout value.
	* @param handle The pointer to the driver session handle from one of the 
	* create_session interfaces.
	* @return The timeout value in milliseconds of the instrument.
	*/
	typedef long(_cdecl *instrument_get_iface_timeout_fn)(void * handle);
	//
	// End Function Pointers
	//

	/** Object for function pointers of DLL exported interfaces. */
	struct instrument_exported_func_decl
	{
		instrument_gpib_create_session_fn  instrument_gpib_create_session;
		instrument_visa_create_session_fn  instrument_visa_create_session;
		instrument_tcpip_create_session_fn instrument_tcpip_create_session;
		instrument_rs232_create_session_fn instrument_rs232_create_session;
		instrument_release_handle_fn       instrument_release_handle;
		instrument_write_fn                instrument_write;
		instrument_read_fn                 instrument_read;
		instrument_get_status_fn           instrument_get_status;
		instrument_clear_device_fn         instrument_clear_device;
		instrument_set_iface_timeout_fn    instrument_set_iface_timeout;
		instrument_get_iface_timeout_fn    instrument_get_iface_timeout;

		/** default constructor */
		instrument_exported_func_decl() :
		instrument_gpib_create_session(nullptr),
			instrument_visa_create_session(nullptr),
			instrument_tcpip_create_session(nullptr),
			instrument_rs232_create_session(nullptr),
			instrument_release_handle(nullptr),
			instrument_write(nullptr),
			instrument_read(nullptr),
			instrument_get_status(nullptr),
			instrument_clear_device(nullptr),
			instrument_set_iface_timeout(nullptr),
			instrument_get_iface_timeout(nullptr)
		{}
	};

	/** DLL exported interfaces */
	instrument_exported_func_decl mDllExported;

	/** Pointer to the DLL in memory */
	HMODULE mDllHandle;

	/** Pointer to the driver session */
	void * mSessionHandle;

	/* The UI API */
	//IUIManager * mUi;

	/**
	* Load the user DLL
	* @param fileAndPath The path and file of the DLL to load.
	* @param err An error value associated with the load result.
	* @return HMODULE Pointer to the DLL if loaded successfully.
	*/
	HMODULE LoadAssembly(const std::string & fileAndPath, std::string & err);

	/**
	* Obtain the exported functions from the DLL.
	* @param asmPtr Pointer to the DLL module.
	* @param exported The exported function pointer structure.
	* @return bool Interfaces were successfully exported from the DLL.
	*/
	bool InitializeExportDecl(HMODULE asmPtr, instrument_exported_func_decl & exported);

	/**
	* Create the instrument comm session.
	* @param def The equipment definition.
	* @return void* Pointer to the session object.
	*/
	void * CreateSession(const EquipmentDefinition & def);

	/**
	* Wrapper function to the Windows "GetLastError" function.  It obtains
	* the string associated with the integer error value.
	* @return std::string The text associated with GetLastError
	*/
	std::string GetWin32Error() const;
};
