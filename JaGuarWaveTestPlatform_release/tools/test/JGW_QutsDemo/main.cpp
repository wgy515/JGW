//
// 
//
// $Id: #3 $
//
// TDO:
//
// Copyright (c) 2021-2021
//
// SPDX-License-Identifier:	
//
#include "stdafx.h"
#include <string>
#include <JGW_FoundationFunc/rapidcsv.hpp>
#include <Windows.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/Utility.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Exception.h>
#include <Poco/Data/RecordSet.h>


class CHelpPackageUIInterface
{
public:

    ///! 
    bool InitHelpPackageUI(int wParam,int lParam)
    {
        return false;
    }
};

typedef void (CHelpPackageUIInterface::*PackageUICallBack)(int wParam,int lParam);
typedef void            *LPVOID;

// 192.168.179.139
// Parameters for barebone-test
#define MYSQL_USER "zhaowc"
#define MYSQL_PWD  "zwc@805718"
#define MYSQL_HOST "47.92.238.235"
#define MYSQL_PORT 3306
#define MYSQL_DB   "local_mes"
//
// Connection string
std::string _dbConnString = "host=" MYSQL_HOST
    ";user=" MYSQL_USER
    ";password=" MYSQL_PWD
    ";db=" MYSQL_DB
    ";compress=true"
    ";auto-reconnect=true"
    ";secure-auth=true"
    ";protocol=tcp";

struct Person
{
    std::string lastName;
    std::string firstName;
    std::string address;
    int age;
    Person(){age = 0;}
    Person(const std::string& ln, const std::string& fn, const std::string& adr, int a):lastName(ln), firstName(fn), address(adr), age(a)
    {
    }
    bool operator==(const Person& other) const
    {
        return lastName == other.lastName && firstName == other.firstName && address == other.address && age == other.age;
    }

    bool operator < (const Person& p) const
    {
        if (age < p.age)
            return true;
        if (lastName < p.lastName)
            return true;
        if (firstName < p.firstName)
            return true;
        return (address < p.address);
    }

    const std::string& operator () () const
        /// This method is required so we can extract data to a map!
    {
        // we choose the lastName as examplary key
        return lastName;
    }
};

namespace Poco {
    namespace Data {


        template <>
        class TypeHandler<Person>
        {
        public:
            static void bind(std::size_t pos, const Person& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
            {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                poco_assert_dbg (!pBinder.isNull());
                pBinder->bind(pos++, obj.lastName, dir);
                pBinder->bind(pos++, obj.firstName, dir);
                pBinder->bind(pos++, obj.address, dir);
                pBinder->bind(pos++, obj.age, dir);
            }

            static void prepare(std::size_t pos, const Person& obj, AbstractPreparator::Ptr pPrepare)
            {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                poco_assert_dbg (!pPrepare.isNull());
                pPrepare->prepare(pos++, obj.lastName);
                pPrepare->prepare(pos++, obj.firstName);
                pPrepare->prepare(pos++, obj.address);
                pPrepare->prepare(pos++, obj.age);
            }

            static std::size_t size()
            {
                return 4;
            }

            static void extract(std::size_t pos, Person& obj, const Person& defVal, AbstractExtractor::Ptr pExt)
            {
                poco_assert_dbg (!pExt.isNull());
                if (!pExt->extract(pos++, obj.lastName))
                    obj.lastName = defVal.lastName;
                if (!pExt->extract(pos++, obj.firstName))
                    obj.firstName = defVal.firstName;
                if (!pExt->extract(pos++, obj.address))
                    obj.address = defVal.address;
                if (!pExt->extract(pos++, obj.age))
                    obj.age = defVal.age;
            }

        private:
            TypeHandler();
            ~TypeHandler();
            TypeHandler(const TypeHandler&);
            TypeHandler& operator=(const TypeHandler&);
        };


    } } // namespace Poco::Data



using namespace Poco::Data;
using namespace Poco::Data::Keywords;


#include <Poco/LocalDateTime.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <regex>
#include <sstream>
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
    if (argc >= 2)
    {
        std::wcout << L"regex : " << argv[1] << std::endl;;
        std::wregex partten(argv[1]);
        std::wcout << L"value : " << argv[2] << std::endl;;
        std::wstring strValue (argv[2]);
        if (regex_match(strValue,partten))
        {
            std::wcout << L"regex_match true " << std::endl;
        }
        else
        {
            std::wcout << L"regex_match false " << std::endl;
        }
    }


    //wstring text = L"百度搜索引擎https://www.baidu.com/^_^";
    //wsmatch wsm;
    //wregex wre(L"https?://(.+?)/");
    //if (regex_search(text, wsm, wre))
    //{
    //    wcout << wsm.str(1) << endl;
    //}
    //const wchar_t *str = L"百度搜索引擎https://www.baidu.com/^_^";
    //wcmatch wcm;
    //if (regex_search(str, wcm, wre))
    //{
    //    wcout << wsm[1] << endl;
    //}
    //return 0;


#if 0
    std::string strTime ("Linux version 4.4.60 (android@ubuntu) (gcc version 5.2.0 (OpenWrt GCC 5.2.0 3bea831+r49254) ) #1 SMP PREEMPT Fri Jul 22 17:21:19 CST 2022");

    std::vector<std::string> vstrEnvValue;
    JGW::JGW_ParserStrA(strTime.c_str(),"#",vstrEnvValue);

    Poco::DateTime dt;
    int tzd;
    Poco::DateTimeParser::parse(Poco::DateTimeFormat::ASCTIME_FORMAT, strTime, dt, tzd);
    dt.makeUTC(tzd);
    Poco::LocalDateTime ldt(tzd, dt);

#endif


#if 0
    Poco::Data::MySQL::Connector::registerConnector();
    {

        Poco::SharedPtr<Poco::Data::Session> _pSession;
        try
        {
            _pSession = new Poco::Data::Session(Poco::Data::MySQL::Connector::KEY, _dbConnString);
        }
        catch (Poco::Data::ConnectionFailedException& ex)
        {
            std::cout << ex.displayText() << std::endl;
            std::cout << "Trying to connect without DB and create one ..." << std::endl;
            //connectNoDB();
            try
            {
                _pSession = new Poco::Data::Session(Poco::Data::MySQL::Connector::KEY, _dbConnString);
            }
            catch (Poco::Data::ConnectionFailedException& ex)
            {
                std::cout << ex.displayText() << std::endl;
                return 0;
            }
        }

        //try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
        //catch(Poco::Data::MySQL::ConnectionException& ce){ std::cout << ce.displayText() << std::endl;  }
        //catch(Poco::Data::MySQL::StatementException& se){ std::cout << se.displayText() << std::endl;  }

        //std::string funct = "complexTypeVector()";
        std::vector<Person> people;
        people.push_back(Person("LN1", "FN1", "ADDR1", 1));
        people.push_back(Person("LN2", "FN2", "ADDR2", 2));

        //try { *_pSession << "INSERT INTO Person VALUES (?,?,?,?)", use(people), now; }
        //catch(Poco::Data::MySQL::ConnectionException& ce){ std::cout << ce.displayText() << std::endl;}
        //catch(Poco::Data::MySQL::StatementException& se){ std::cout << se.displayText() << std::endl;  }

        int count = 0;
        try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
        catch(Poco::Data::MySQL::ConnectionException& ce){ std::cout << ce.displayText() << std::endl; }
        catch(Poco::Data::MySQL::StatementException& se){ std::cout << se.displayText() << std::endl; }
        assert (count == 2);

        std::vector<Person> result;
        try { *_pSession << "SELECT * FROM Person", into(result), now; }
        catch(Poco::Data::MySQL::ConnectionException& ce){ std::cout << ce.displayText() << std::endl;  }
        catch(Poco::Data::MySQL::StatementException& se){ std::cout << se.displayText() << std::endl; }
        assert (result == people);


        //int count = 0;
        try 
        { 
            
            *_pSession << "SELECT COUNT(*) FROM station_number", into(count), now; 
            Poco::Data::RecordSet recordSet(*_pSession,"SELECT * FROM station_number");
            std::cout << std::endl;
            //std::cout << std::endl << Poco::Data::RecordSet(*_pSession, "SELECT * FROM station_number");
        }
        catch(Poco::Data::MySQL::ConnectionException& ce)
        { 
            std::cout << ce.displayText() << std::endl; 
            //fail (funct); 
        }
        catch(Poco::Data::MySQL::StatementException& se)
        { 
            std::cout << se.displayText() << std::endl; 
            //fail (funct); 
        }

        //! select * from station_number;


    }
    //std::cout << "Server Info: " << Utility::serverInfo(session) << std::endl;
    //std::cout << "Server Version: " << Utility::serverVersion(session) << std::endl;
    //std::cout << "Host Info: " << Utility::hostInfo(session) << std::endl;
    Poco::Data::MySQL::Connector::unregisterConnector();
#endif


















    //wchar_t strFilterW[260] = {0};
    //size_t len = 0;
    //std::wstring strFilter = L"excel File(*.xls,*.xlsx)\\0*.xlsx;*.xls\\0all file(*.*)\\0*.*\\0\\0";
    //


    //for (size_t index = 0;index < strFilter.length() - 1;)
    //{
    //    if (L'\\' == strFilter[index] && L'0' == strFilter[index + 1])
    //    {
    //        strFilterW[len++] = L'\0';
    //        index += 2;
    //    }
    //    else
    //    {
    //        strFilterW[len++] = strFilter[index];
    //        index++;
    //    }
    //}


    //wchar_t strFilterWS[260] = L"excel File(*.xls,*.xlsx)\0*.xlsx;*.xls\0all file(*.*)\0*.*\0\0";
    //CHelpPackageUIInterface packageUIInterface;
    //auto func = CHelpPackageUIInterface::InitHelpPackageUI;
    //LPVOID funcPtr = &func;
    //PackageUICallBack UICallBack = (PackageUICallBack)funcPtr;
    //rapidcsv::Document doc("F:\\project\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\cache\\QIA\\WIFI_QIA_DEMO\\psthresh.csv", rapidcsv::LabelParams(0, 1));
    //const std::vector<std::string>& column = doc.GetColumn<std::string>("OtpDir");
    //const std::vector<std::string>& row = doc.GetRow<std::string>("PER4341907020054");
    //std::string value = doc.GetCell<std::string>("ProfileName", "PER4341907020054");
    //const double cell = doc.GetCell<double>("Close", "2016-05-23");
    //doc.SetCell<std::string>("OtpDir","PRM2141-01_PRA7211-01","E:\\工具包\\工厂测试工具包\\PTPV2_PRM2141\\PRM2141-04\\离线\\2022-1-13\\exec\\ProductScreen\\PRM2141-04\\OTPs\\");
    //doc.Save();

    return 0;
}