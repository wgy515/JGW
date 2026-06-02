//
// xxx.h
//
// $Id: xxx.h#3 $
//
// Library: 
// Package: 
// Module:  
//
// TDO:
//
// Copyright (c) 2021-2021
//
// SPDX-License-Identifier:	
//
#pragma once

namespace JGW
{
    enum WLAN_TpcBand
    {
        ALL,
        _5G_6G,
        _2G
    };

    enum BAND
    {
        _5G_6G_BAND,
        _2G_BAND
    };

    enum DutTypeId
    {
        AR6003,
        AR6004,
        AR6005,
        AR9800,
        WCN3660_FTMSTARTSTOPONTX_legacy,
        WCN3680_FTMSTARTSTOPONTX_legacy,
        WCN3660,
        WCN3680,
        WCN3660_DEPRECATED_legacy,
        QC6174,
        QC99xx,
        WCN3610_15_20,
        AR93xx,
        QC9984,
        WCN39xx,
        QC6180,
        AR6006,
        IPQ807x,
        QC639x,
        IPQ6018,
        QC649x,
        WCN6750,
        WCN7850,
        VirtualQ5
    };

    enum XTAL_ALGORITHM
    {
        SEARCH_BINARY,
        SEARCH_LINEAR
    };

    //typedef struct
    //{
    //    double m_freqErr;
    //    uint m_capVal;
    //} _FREQ_CAP;
}
