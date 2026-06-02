#pragma once
#include "IEquipCommTL.h"
namespace JGW
{
    class CDebugEquipComm : public IEquipCommTL
    {
    public:
        CDebugEquipComm(void);
        ~CDebugEquipComm(void);
    public:

        //       /**
        //     * Write command
        //     * @param cmd Write a string.
        //     * @return bool TRUE if operation is successful, FALSE otherwise.
        //     */
        //    virtual void SetLog4IxPtr( void* pIxLogWriter );

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

        virtual bool CloseSession();

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
    };
}

