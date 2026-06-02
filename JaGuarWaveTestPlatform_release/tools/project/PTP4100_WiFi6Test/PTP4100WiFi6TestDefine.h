#pragma once 
#include <string>
#include <Windows.h>

namespace JGW
{
    typedef struct
    {
        HWND mhMainWindow;
        bool mbIsUseSerialPort;
        unsigned char muTxAttenVga1Step;
        unsigned char muTxAttenVga1Start;
        unsigned char muTxAttenVga1End;
        unsigned char muTxAttenVga2Step;
        unsigned char muTxAttenVga2Start;
        unsigned char muTxAttenVga2End;
        unsigned char muRxAttenStep;
        unsigned char muRxAttenStart;
        unsigned char muRxAttenEnd;

        int mnTxPowerStep;
        int mnTxPowerStart;
        int mnTxPowerEnd;
        int mnPacketCollectionTime;
        int mnErrorRetryTest;
        int mnMinRssi;
        int mnMinTotalPackets;
        int mnMinGoodPackets;
        float mfMinEvm;
        //std::wstring mstrRxSerialPort;
        //std::wstring mstrTxSerialPort;
        std::wstring mstrTxIPQAddress;
        std::wstring mstrRxIPQAddress;
        std::wstring mstrTxUsbNetCardAddress;
        std::wstring mstrRxUsbNetCardAddress;
    }S_TEST_PARAM,*PS_TEST_PARAM;
}

