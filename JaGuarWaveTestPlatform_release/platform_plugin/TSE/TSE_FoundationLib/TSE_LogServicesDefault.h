#pragma once
#include <TSE_SequenceManagerPlugin/TSE_LogServicesImpl.h>

namespace JGW
{
    class CTSE_LogServicesDefault : public CTSE_LogServicesImpl
    {
    public:
        //! 
        void DebugLog(const wchar_t* strDebugLog);
        //! 
        void DebugLog(const char* strDebugLog);
        //!
        void DebugLogFormat(const wchar_t* strDebugLog,...);
        //!
        void DebugLogFormat(const char* strDebugLog,...);
        //! 
        void ErrorLog(const wchar_t* strErrorLog);
        //! 
        void ErrorLogFormat(const wchar_t* strErrorLog,...);
        //! 
        void ErrorLog(const char* strErrorLog);
        //! 
        void ErrorLogFormat(const char* strErrorLog,...);
        //! 
        void InfoLog(const wchar_t* strInfoLog);
        //!
        void InfoLogFormat(const wchar_t* strInfoLog,...);
        //! 
        void InfoLog(const char* strInfoLog);
        //!
        void InfoLogFormat(const char* strInfoLog,...);
        //!
        void UserDefinedLog(int id,const wchar_t* strCustomLog);
        //!
        void UserDefinedLogFormat(int id,const wchar_t* strCustomLog,...);
    };
}


