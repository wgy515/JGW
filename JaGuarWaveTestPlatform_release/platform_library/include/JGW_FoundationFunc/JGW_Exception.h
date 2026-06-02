#pragma once
#include <string>
#include <sstream>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    class CJGW_Exception
    {
    public:
        CJGW_Exception(std::wstring& strerror,DWORD dwErrorID,const char* file,long nline )
        {
            std::wostringstream strerrormsg;
            strerrormsg << strerror << L" ERROR_ID: " << JGW_GetSystemErrorString(dwErrorID) << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror;
            m_dwErrorID = dwErrorID;
            m_strerrormsg = strerrormsg.str();
        }
        CJGW_Exception( std::wstring& strerror,DWORD dwErrorID,const wchar_t* file ,long nline  )
        {
            std::wostringstream strerrormsg;
            strerrormsg << strerror << L" ERROR_ID: " << JGW_GetSystemErrorString(dwErrorID) << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror;
            m_dwErrorID = dwErrorID;
            m_strerrormsg = strerrormsg.str();
        }
        CJGW_Exception( std::wstring& strerror,const char* file,long nline )
        {
            std::wostringstream strerrormsg;
            m_dwErrorID = 0;
            strerrormsg << strerror << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror/*std::wstring(errortemp.begin(),errortemp.end())*/;
            m_strerrormsg = strerrormsg.str();
        }
        CJGW_Exception( std::wstring& strerror,const wchar_t* file,long nline )
        {
            std::wostringstream strerrormsg;
            m_dwErrorID = 0;
            strerrormsg << strerror << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror;
            m_strerrormsg = strerrormsg.str();
        }

        CJGW_Exception( std::wstring& strerror )
        {
            m_dwErrorID = 0;
            m_errorname = strerror;
            m_strerrormsg = strerror;
        }

        CJGW_Exception( const wchar_t* strerror,DWORD dwErrorID,const char* file,long nline )
        {
            std::wostringstream strerrormsg;
            strerrormsg << strerror << L" ERROR_ID: " << JGW_GetSystemErrorString(dwErrorID) << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror;
            m_dwErrorID = dwErrorID;
            m_strerrormsg = strerrormsg.str();
        }
        CJGW_Exception( const wchar_t* strerror,DWORD dwErrorID,const wchar_t* file,long nline )
        {
            std::wostringstream strerrormsg;
            strerrormsg << strerror << L" ERROR_ID: " << JGW_GetSystemErrorString(dwErrorID) << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror;
            m_dwErrorID = dwErrorID;
            m_strerrormsg = strerrormsg.str();
        }

        CJGW_Exception( const wchar_t* strerror,const char* file,long nline )
        {
            std::wostringstream strerrormsg;
            m_dwErrorID = 0;
            strerrormsg << strerror << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror;
            m_strerrormsg = strerrormsg.str();
        }

        CJGW_Exception( const wchar_t* strerror,const wchar_t* file,long nline )
        {
            std::wostringstream strerrormsg;
            m_dwErrorID = 0;
            strerrormsg << strerror << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = strerror;
            m_strerrormsg = strerrormsg.str();
        }
        CJGW_Exception( const wchar_t* strerror )
        {
            m_dwErrorID = 0;
            m_errorname = strerror;
            m_strerrormsg = strerror;
        }

        CJGW_Exception( const char* strerror,const wchar_t* file,long nline )
        {
            std::wostringstream strerrormsg;
            std::wstring errortemp = JGW_A2W(strerror);
            m_dwErrorID = 0;

            strerrormsg << errortemp << L" FILE: " << file << L" LINE: "<< nline;
            m_errorname = errortemp;
            m_strerrormsg = strerrormsg.str();
        }

        ~CJGW_Exception(void)
        {

        }

        std::wstring& GetErrorMsg(){return m_errorname;}
        DWORD GetErrorID(){return m_dwErrorID;}
        std::wstring& GetErrorDescription() const{return m_strerrormsg;}

        static std::wstring GetSystemErrorString(unsigned long errorcode)
        {
            return JGW_GetSystemErrorString(errorcode);
        }
    private:
        CJGW_Exception& operator=(const CJGW_Exception&);
    private:
        static  std::wstring m_errorname;
        static  std::wstring m_strerrormsg;
        static  DWORD m_dwErrorID;
    };

    std::wstring __declspec(selectany) CJGW_Exception::m_strerrormsg = L"";
    std::wstring __declspec(selectany) CJGW_Exception::m_errorname = L"";
    DWORD __declspec(selectany) CJGW_Exception::m_dwErrorID = 0;
}