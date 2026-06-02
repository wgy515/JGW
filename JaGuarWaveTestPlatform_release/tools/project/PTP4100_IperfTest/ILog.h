#pragma once
namespace JGW
{
    class ILog
    {
    public:
        ILog(void);
        ~ILog(void);
    public:
        virtual void Verbose(const wchar_t* szFormat,...);
        virtual void Verbose(const char* szFormat,...);

        virtual void Debug(const wchar_t* szFormat,...);
        virtual void Debug(const char* szFormat,...);

        virtual void Info(const wchar_t* szFormat,...);
        virtual void Info(const char* szFormat,...);

        virtual void Warning(const wchar_t* szFormat,...);
        virtual void Warning(const char* szFormat,...);

        virtual void Error(const wchar_t* szFormat,...);
        virtual void Error(const char* szFormat,...);

        virtual void Fatal(const wchar_t* szFormat,...);
        virtual void Fatal(const char* szFormat,...);
    };
}
