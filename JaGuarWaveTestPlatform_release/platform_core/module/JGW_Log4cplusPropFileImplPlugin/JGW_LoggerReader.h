#pragma once

namespace JGW
{
    class JGW_LoggerReader
    {
    public:
        JGW_LoggerReader(void);
        ~JGW_LoggerReader(void);
    private:
         //logger_reader
         std::wstring mstrReadBuf;
    };
}