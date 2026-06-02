#pragma once

namespace JGW
{
    typedef struct
    {
        std::wstring mstrDescription;
        std::wstring mstrValue2;
    }S_DATA_INFO;

    typedef struct  
    {
        int mnPeopleNumber;
        int mnColCount;
        int mnCardWidth;
        int mnCardHeight;
        std::wstring mstrDataFilePath;
        //int mnPerRow;
        //int mnPerCol;
        std::vector<S_DATA_INFO> mvsDataInfo;
    }S_LUCKY_DRAW_CONFIG,*PS_LUCKY_DRAW_CONFIG;
}