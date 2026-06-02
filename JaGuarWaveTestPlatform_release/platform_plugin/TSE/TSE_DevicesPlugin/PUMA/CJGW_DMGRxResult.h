#pragma once
#include <string>
#define DMG_RX_INVALID -99999 
namespace JGW
{
    class CCJGW_DMGRxResult
    {
    public:
        CCJGW_DMGRxResult(const std::string& strDMGRxResult);
        ~CCJGW_DMGRxResult(void);
    public:
        //! 
        int GetRssi();
        //! 
        int GetFerPerimile();
        //!
        int GetUgain();
        //!
        int GetPPDUDetects();
        //!
        int GetPPDUSuccess();
        //! 
        std::wstring GetRssiString();
        //! 
        std::wstring GetFerPerimileString();
        //!
        std::wstring GetUgainString();
        //!
        std::wstring GetPPDUDetectsString();
        //!
        std::wstring GetPPDUSuccessString();
    private:
        //! 
        void PareseDMGRxResult(const std::string& strDMGRxResult);
        //! 
        std::string GetRxStatusResultToFlag(const std::string& result,const std::string& flag, std::string end = "\n");
        //! 
        int GetDMGRxResultToFlag(const std::string& result,const std::string& flag, std::string end = "\n");
    private:
        int mnRssi;
        int mnFerPerimile;
        int mnUgain;
        int mnPPDUDetects;
        int mnPPDUSuccess;
    };

}

