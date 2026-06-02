#pragma once
#include <string>
#include <vector>
#include <tchar.h>
#include <comdef.h>
namespace JGW
{
    struct SParameters_STRUCT
    {
        bstr_t paramName; 
        bstr_t paramVal;
        bstr_t unit;
        bstr_t upper;
        bstr_t lower;
        bstr_t type;
        bstr_t mode;
    };

    class STestProject_STRUCT
    {
    public:
        STestProject_STRUCT():m_bTest(false)
            ,TestName("")
            ,RealName("")
            ,m_lNumParams(0)
            ,m_bDestory(false)
        {

        }
        bool		m_bTest;
        bool		m_bDestory;
        bstr_t		TestName;
        bstr_t		RealName;
        long		m_lNumParams;
        std::vector<SParameters_STRUCT> m_vParameters;
    };
}