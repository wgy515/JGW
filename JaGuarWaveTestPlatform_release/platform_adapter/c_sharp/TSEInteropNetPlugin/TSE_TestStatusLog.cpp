#include "StdAfx.h"
#include "TSE_TestStatusLog.h"

namespace JGW
{
   void CTSE_TestStatusLog::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
   {
       if (gobalTestStatusCallBack)
       {
           gobalTestStatusCallBack(eTestStatus,strTestName);
       }
   }

   void CTSE_TestStatusLog::OnTestIndexChange(size_t index,int subIndex /* = -1 */)
   {
       
   }
}