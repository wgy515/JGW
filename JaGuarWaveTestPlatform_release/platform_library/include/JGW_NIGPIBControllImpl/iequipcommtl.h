/******************************************************************************
 * $File: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/iequipcommtl.h $
 *
 * DESCRIPTION: IEquipCommTL
 ******************************************************************************
 *
 * Copyright (c) 2014 QUALCOMM Technologies Incorporated.
 * All rights reserved.
 * Qualcomm Confidential and Proprietary
 *
 ******************************************************************************
 *
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/iequipcommtl.h#9 $
 * $DateTime: 2015/04/16 13:39:57 $
 *
 */
#pragma once

#include <string>
#include "CMW500_Montior.h"
/**
 * Class IEquipCommTL
 */
class IEquipCommTL
{
public:

   /**
    * Constructor
    */
   IEquipCommTL(){};

   /**
    * Destructor
    */
   virtual ~IEquipCommTL(){}

//       /**
//     * Write command
//     * @param cmd Write a string.
//     * @return bool TRUE if operation is successful, FALSE otherwise.
//     */
//    virtual void SetLog4IxPtr( void* pIxLogWriter ) = 0;

   /**
    * Write command
    * @param cmd Write a string.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   virtual bool write( const std::string& cmd ) = 0;

   /**
    * Write command
    * @param cmd Write a data buffer.
    * @param count The size of the data buffer.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   virtual bool write( void * buffer, long count ) = 0;

   /**
    * Read command
    * @param retVal Read a string.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   virtual bool read( std::string& retVal ) = 0;
   
   /**
    * Read command
    * @param buffer Read to a data buffer.
    * @param bufferSize The size of the data buffer.
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   virtual bool read( void * buffer, long bufferSize ) = 0;

   /**
    * Read command
    * @param buffer Read to a data buffer.
    * @param bufferSize The size of the data buffer.
    * @param bytesRead Returns the number of bytes read from the interface
    * @return bool TRUE if operation is successful, FALSE otherwise.
    */
   virtual bool read( void * buffer, long bufferSize, long &bytesRead ) = 0;

   /**
    * Query command
    * @param cmd The query command string.
    * @param retVal The response string.
    */
   virtual void query( const std::string & cmd, std::string & retVal ) = 0;

   virtual bool query_ok( const std::string & cmd, std::string & retVal ) = 0;

   virtual bool query( const char* buffer, float& fVal ) = 0;
   /**
    * Check to see if the instrument is present and communicating.
    * @return bool TRUE if instrument is present, FALSE otherwise.
    */
   virtual bool IsPresent() = 0;

   /**
    * Get the error state of the instrument.
    * @return bool TRUE if there is no error, FALSE otherwise.
    */
   virtual bool GetErrorStatus() = 0;

   /**
    * Get the error state of the instrument.
    * @return bool TRUE if there is no error, FALSE otherwise.
    */
   virtual bool ClearDevice() = 0;

   virtual bool CloseSession() = 0;

   /**
    * Reads the serial poll status.  This is specific to GPIB.
    * @return bool TRUE if there is no error, FALSE otherwise.
    */
   virtual bool SerialPoll( char& buffer ) = 0;

   /**
    * Set the interface timeout value.  The timeout value is the time
    * required for response after query.
    * @param timeoutSec The timeout value in seconds.
    * @return bool TRUE if operation was successful, FALSE otherwise.
    */
   virtual bool SetTimeOut( double timeoutSec ) = 0;

   /**
    * Get the interface timeout value.  The timeout value is the time
    * required for response after query.
    * @return double The timeout value in seconds.
    */
   virtual double GetTimeOut() = 0;

   /**
    * Get the instrument interface address.  This is specific to GPIB.
    * @return int The address of the instrument.
    */
   virtual int GetDeviceAddress() = 0;
};


/*!
EquipCommTL_Type
Enum to identify the type equipment communication used.
*/
enum EquipCommTL_Type
{
	EQUIP_COMM_TYPE_GPIB,
	EQUIP_COMM_TYPE_TCP_IP,
	EQUIP_COMM_TYPE_SERIAL,
	EQUIP_COMM_TYPE_VISA,
	EQUIP_COMM_TYPE_USER_DEFINED_VIRTUAL,  // This is not a real transport, just a logical representation of one.
	EQUIP_COMM_TYPE_SIMULATED,
	EQUIP_COMM_TYPE_UNINIT = 0xFFFFFFFF
};

typedef enum
{
	//! NetworkInstrumentProtocol
	NetworkInstrumentProtocol,
	//! TCP_IP
	TCP_IP_CLIENT
} E_TCP_IP_CLIENT;
/*!
EquipmentDefinition

A structure to hold the contents of a single record from the equipment configuration file.
*/
struct EquipmentDefinition
{
	E_TCP_IP_CLIENT eTcpIpClient;
   /*! The unique identifier, eg. CALL_PROCESSOR_1. */
   std::string identifier;
   /*! The model of the test equipment.  For SUITE dlls values can be found in
   X-Functional\Drivers\ieee488\ieee488_cmdcodegenerator.xls valid model values
   must exactly match the contents found in the header row (eg "RS SMIQ") */
   std::string model;
   /*! The type of the test equipment (eg ASG, DSG, CP, etc..).  For SUITE dlls values can be
   found in X-Functional\Drivers\ieee488\ieee488_cmdcodegenerator.xls
   valid types values must exactly match the worksheet names */
   std::string type;
   //! NI OR AGILENT
   bool isNiGpibType;
   //! true only if the equipment is piped through a fading emulators.  Only valid for SGs and CPs
   bool faded;
   //! board ID, valid only if comm_link_type == EQUIP_COMM_TYPE_GPIB
   int boardId;
   //! primary GPIB address, valid only if comm_link_type == EQUIP_COMM_TYPE_GPIB
   int address;
   //! secondary GPIB address, valid only if comm_link_type == EQUIP_COMM_TYPE_GPIB
   int secAddress;
   //! When used with a sig gen, the Cf of filter will be tuned in conjunction with the sg freq
   int filterIndex;
   //! ip address, valid only if comm_link_type == EQUIP_COMM_TYPE_TCP_IP
   std::string ip_address;
   std::string source_address;
   //! if comm_link_type == EQUIP_COMM_TYPE_TCP_IP, this is used to map to the subinstrument being used
   //! inst0/1 correspond to subinstrument1/2
   int inst_number;
   unsigned int socketPort;
   //! com port number, valid only if comm_link_type == EQUIP_COMM_TYPE_SERIAL
   int comPortNumber;
   //! VISA address, valid only if comm_link_type == EQUIP_COMM_TYPE_VISA
   //! has the form of GPIB0::1::INSTR in the equipconfig.xml. The entire VISA address string must be entered.
   std::string visaAddress;
   //! type of comm link
   EquipCommTL_Type comm_link_type;
   //! command delay
   int commandDelay;
   //! the language used to communicate between this tool and the instrument, comma delimited
   std::string languages;
   //! user defined DLL, file and path
   std::string usrDllFilePath;
   //! simulator equip address
   int simAddr;

   /**
    * Constructor
    */
   EquipmentDefinition() :
      identifier(""),
      model(""),
	  isNiGpibType(true),
	  eTcpIpClient(NetworkInstrumentProtocol),
      type(""),
      faded(false),
      boardId(-1),
      address(-1),
      secAddress(-1),
      filterIndex(0),
      ip_address(""),
      inst_number(0),
      comPortNumber(-1),
      visaAddress(""),
      comm_link_type(EQUIP_COMM_TYPE_UNINIT),
      commandDelay(0),
      languages(""),
      usrDllFilePath(""),
      simAddr(0)
   {
   }
};
