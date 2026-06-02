#pragma once
#include <string>
#include <tinyxml/tinyxml.h>
#include <vector>

namespace JGW
{
    struct S_LOSS_DATA
    {
        unsigned int muFrequency;
        float mfLoss;
        float mfDelta;
        TiXmlElement* mpDataNode;
        unsigned int mnPosIndex;

        bool operator <(const S_LOSS_DATA& other)const  
        {
            return muFrequency < other.muFrequency;
        }

        bool operator >(const S_LOSS_DATA& other)const  
        {
            return muFrequency > other.muFrequency;
        }
    };

    typedef struct  
    {
        std::string mstrPathName;
        std::vector<S_LOSS_DATA> mvsLossData;
    } S_LOSS_PATH;

    class CCJGW_ResolveStationCalDataFile
    {
    public:
        CCJGW_ResolveStationCalDataFile(void);
        ~CCJGW_ResolveStationCalDataFile(void);
    public:
        bool ResolveStationCalDataFile(const std::wstring& strStationCalDataFilePath,std::vector<S_LOSS_PATH>& vsLossPath);
    public:
        bool SaveStationCalDataFile(std::wstring& strTestLogPath);
    public:
        // 与金机线损无关，保证金机测试数据
        void UpdateFreqLoss(const std::wstring& strPathName,unsigned int uFrequency,float fPowerDiff);
        void ChangeDataListNode(TiXmlElement* pDataListNode,std::vector<S_LOSS_DATA>& vsLossData,bool bAddConfigLossOffset = true);
    private:
        float GetLossToToFreq(const std::vector<S_LOSS_DATA>& vsLossData,unsigned int fFreq);
        void TraverseDataListNodeNextDataNode(TiXmlElement* pDataListNode,std::vector<S_LOSS_DATA>& vsLossData);
    private:
        std::vector<S_LOSS_PATH> mvsLossPath;
    };
}


