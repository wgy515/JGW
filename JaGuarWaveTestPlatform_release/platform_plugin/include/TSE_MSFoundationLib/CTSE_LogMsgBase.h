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
#include <string>
namespace JGW
{
    class CTSE_LogMsgBase
    {
    public:
        virtual std::string GetID() = 0;
        /// 获取log MSG ID

        virtual bool ReadLog(std::string& strRead) = 0;
        /// 读取log，无数据返回false
    };
}