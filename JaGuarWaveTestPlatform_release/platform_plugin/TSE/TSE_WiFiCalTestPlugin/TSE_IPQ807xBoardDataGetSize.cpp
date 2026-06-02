#include "StdAfx.h"
#include "TSE_IPQ807xBoardDataGetSize.h"
#include "CJGW_IPQ807xPhone.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CTSE_IPQ807xBoardDataGetSize::CTSE_IPQ807xBoardDataGetSize(void)
    {
    }


    CTSE_IPQ807xBoardDataGetSize::~CTSE_IPQ807xBoardDataGetSize(void)
    {
    }

    const wchar_t* CTSE_IPQ807xBoardDataGetSize::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xBoardDataGetSize::TSE_Run()
    {
        std::string boardDataSize;
        bool result = CCJGW_IPQ807xPhone::GetIPQ807xPhone()->BoardDataGetSize(boardDataSize);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"BoardData Get Size : %s",JGW_A2W(boardDataSize).c_str());
        return result;
    }
}
