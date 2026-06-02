#include "StdAfx.h"
#include <JGW_FoundationFunc/CJGW_NumberRule.h>

namespace JGW
{
    CCJGW_NumberRule::CCJGW_NumberRule(void)
    {
    }


    CCJGW_NumberRule::~CCJGW_NumberRule(void)
    {
    }

    bool CCJGW_NumberRule::CheckIMEIIsNumber(const std::wstring& strIMEI)
    {
        for( int i = 0; i < 14; i ++)
        {
            if (strIMEI[i] < L'0' || strIMEI[i] > '9') return false;
        }
        return true;
    }

    bool CCJGW_NumberRule::GetIMEIChecksum(const std::wstring& strIMEI,std::wstring& strIMEIChecksum)
    {
        if (!(14 == strIMEI.length() || 15 == strIMEI.length()) || !CheckIMEIIsNumber(strIMEI)) return false;

        int szMinBuffer[14] = {0};
        int nCheckSum = 0;
        strIMEIChecksum = strIMEI.substr(0,14);

        for( int i = 1; i < 14; i += 2 )
        {
            szMinBuffer[i/2] = (strIMEI[i] - '0') * 2;
        }

        //与偶数位相加
        for( int i = 0; i < 14; i += 2 )
        {
            if(szMinBuffer[i/2] >= 10)
            {
                nCheckSum += szMinBuffer[i/2]/10 + szMinBuffer[i/2]%10 + (strIMEI[i] - '0') ;
            }
            else
            {
                nCheckSum += szMinBuffer[i/2] +  (strIMEI[i] - '0');
            }
        }

        if(!(nCheckSum % 10))
        {
            strIMEIChecksum += (0 + '0');
        }
        else
        {
            int nMinuend = ((nCheckSum + 10) / 10)  * 10;
            strIMEIChecksum += ((nMinuend - nCheckSum) + '0');
        }
        return true;
    }
}