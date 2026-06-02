#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/spi/factory.h>
#include <string>
#include <codecvt>

class ChineseUtf8LocaleFactory : public log4cplus::spi::LocaleFactory {
private:
    static log4cplus::tstring const  CHS_UTF8;
public:
    std::locale createObject(const log4cplus::helpers::Properties & props) {
        return std::locale(std::locale("Chinese_China"), new std::codecvt_utf8<wchar_t>());
    };

    log4cplus::tstring const & getTypeName() const {
        return CHS_UTF8;
    };
};

log4cplus::tstring const ChineseUtf8LocaleFactory::CHS_UTF8 = LOG4CPLUS_TEXT("UTF-8");