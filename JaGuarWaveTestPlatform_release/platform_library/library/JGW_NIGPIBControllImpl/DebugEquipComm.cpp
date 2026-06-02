#include "StdAfx.h"
#include <JGW_NIGPIBControllImpl/DebugEquipComm.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CDebugEquipComm::CDebugEquipComm(void)
    {
    }


    CDebugEquipComm::~CDebugEquipComm(void)
    {
    }

    bool CDebugEquipComm::write( const std::string& cmd )
    {
        Log4WI(JGW_A2W(cmd).c_str());
        return true;
    }

    bool CDebugEquipComm::write( void * buffer, long count ) 
    {
        Log4WI(JGW_A2W((const char*)buffer).c_str());
        return true;
    }

    bool CDebugEquipComm::read( std::string& retVal )
    {
        retVal = "1,RDY,ON,OK";
        Log4WI(JGW_A2W(retVal).c_str());
        return true;
    }

    bool CDebugEquipComm::read( void * buffer, long bufferSize )
    {
         strcpy_s((char*)buffer,bufferSize,"1,RDY,ON,OK");
         bufferSize = strlen((const char*)buffer);
         Log4WI(JGW_A2W((const char*)buffer).c_str());
         return true;
    }

    bool CDebugEquipComm::read( void * buffer, long bufferSize, long &bytesRead )
    {
        strcpy_s((char*)buffer,bufferSize,"1,RDY,ON,OK");
        bytesRead = strlen((const char*)buffer);
        Log4WI(JGW_A2W((const char*)buffer).c_str());
        return true;
    }

    void CDebugEquipComm::query( const std::string & cmd, std::string & retVal )
    {
        write( cmd );
        read( retVal );
    }

    bool CDebugEquipComm::query_ok( const std::string & cmd, std::string & retVal )
    {
        return ( write(cmd) && read(retVal) );
    }

    bool CDebugEquipComm::query( const char* buffer, float& fVal )
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

    bool CDebugEquipComm::IsPresent()
    {
        return true;
    }

    bool CDebugEquipComm::GetErrorStatus()
    {
        return true;
    }

    bool CDebugEquipComm::ClearDevice()
    {
        return true;
    }

    bool CDebugEquipComm::CloseSession()
    {
        return true;
    }

    bool CDebugEquipComm::SerialPoll( char& buffer )
    {
        return true;
    }

    bool CDebugEquipComm::SetTimeOut( double timeoutSec )
    {
        return true;
    }

    double CDebugEquipComm::GetTimeOut() 
    {
        return 200;
    }

    int CDebugEquipComm::GetDeviceAddress()
    {
        return 0;
    }
}
