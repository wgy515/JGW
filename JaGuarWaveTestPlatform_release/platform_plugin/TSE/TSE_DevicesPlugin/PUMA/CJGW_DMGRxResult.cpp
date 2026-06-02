#include "StdAfx.h"
#include "CJGW_DMGRxResult.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_DMGRxResult::CCJGW_DMGRxResult(const std::string& strDMGRxResult) : mnRssi(DMG_RX_INVALID),mnFerPerimile(DMG_RX_INVALID),mnPPDUDetects(DMG_RX_INVALID),mnPPDUSuccess(DMG_RX_INVALID),mnUgain(DMG_RX_INVALID)
    {
        PareseDMGRxResult(strDMGRxResult);
    }


    CCJGW_DMGRxResult::~CCJGW_DMGRxResult(void)
    {
    }

    int CCJGW_DMGRxResult::GetRssi()
    {
        return mnRssi;
    }

    int CCJGW_DMGRxResult::GetFerPerimile()
    {
        return mnFerPerimile;
    }

    int CCJGW_DMGRxResult::GetUgain()
    {
        return mnUgain;
    }

    int CCJGW_DMGRxResult::GetPPDUDetects()
    {
        return mnPPDUDetects;
    }

    int CCJGW_DMGRxResult::GetPPDUSuccess()
    {
        return mnPPDUSuccess;
    }

    std::wstring CCJGW_DMGRxResult::GetRssiString()
    {
        return DMG_RX_INVALID == mnRssi?L"":JGW_GetFormatWString(L"%d",mnRssi);
    }

    std::wstring CCJGW_DMGRxResult::GetFerPerimileString()
    {
        return DMG_RX_INVALID == mnFerPerimile?L"":JGW_GetFormatWString(L"%d",mnFerPerimile);
    }

    std::wstring CCJGW_DMGRxResult::GetUgainString()
    {
        return DMG_RX_INVALID == mnUgain?L"":JGW_GetFormatWString(L"%d",mnUgain);
    }

    std::wstring CCJGW_DMGRxResult::GetPPDUDetectsString()
    {
        return DMG_RX_INVALID == mnPPDUDetects?L"":JGW_GetFormatWString(L"%d",mnPPDUDetects);
    }

    std::wstring CCJGW_DMGRxResult::GetPPDUSuccessString()
    {
        return DMG_RX_INVALID == mnPPDUSuccess?L"":JGW_GetFormatWString(L"%d",mnPPDUSuccess);
    }


    void CCJGW_DMGRxResult::PareseDMGRxResult(const std::string& strDMGRxResult)
    {
        mnPPDUDetects = GetDMGRxResultToFlag(strDMGRxResult,"PPDU Detects:");
        mnPPDUSuccess = GetDMGRxResultToFlag(strDMGRxResult,"PPDU Success:");
        mnFerPerimile = GetDMGRxResultToFlag(strDMGRxResult,"FER:");
        mnUgain = GetDMGRxResultToFlag(strDMGRxResult,"Ugain");
        mnRssi = GetDMGRxResultToFlag(strDMGRxResult,"RSSI(dBm)");
    }

    std::string CCJGW_DMGRxResult::GetRxStatusResultToFlag(const std::string& result,const std::string& flag, std::string end /* = "" */)
    {
        std::string strData = "";

        size_t index = result.find(flag);
        if (std::string::npos == index)
        {
            return "";
        }

        size_t endIndex = result.find(end,index);
        if (std::string::npos != endIndex)
        {
            strData = result.substr(index + flag.length(),endIndex - index - flag.length());
            JGW_EraseLastAndFristTwoChars(strData,' ');
        }
        return strData;
    }

    int CCJGW_DMGRxResult::GetDMGRxResultToFlag(const std::string& result,const std::string& flag, std::string end /* = "" */)
    {
        std::string rsp = GetRxStatusResultToFlag(result,flag,end);
        if (rsp.empty())
        {
            return DMG_RX_INVALID;
        }
        return atoi(rsp.c_str());
    }
}

