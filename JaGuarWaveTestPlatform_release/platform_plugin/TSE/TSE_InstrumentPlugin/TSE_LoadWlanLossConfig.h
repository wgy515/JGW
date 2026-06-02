#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    typedef struct
    {
        std::wstring mstrPathName;
        std::map<int,float> mmapFreqLoss;
    } S_LOSS_DATA_LIST,*LPS_LOSS_DATA_LIST;

    class CTSE_LoadWlanLossConfig : public CTSE_TestBase
    {
    public:
        CTSE_LoadWlanLossConfig(void);
        ~CTSE_LoadWlanLossConfig(void);    
    public:
        float GetWlanLoss(const std::wstring& strPathName,int dFrequency);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //bool TSE_Init();
        bool TSE_Run();
        LPS_LOSS_DATA_LIST GetLossDataList(const std::wstring& strPathName);
        float CalcLoss(int fLFreq,int fHFreq,float fLLoss,float fHLoss);
    private:

        std::vector<S_LOSS_DATA_LIST> mvsLossDataList;
        std::wstring mstrWlanLossPath;
        std::wstring mstrLossInstanceEnvionment;
    };
}

